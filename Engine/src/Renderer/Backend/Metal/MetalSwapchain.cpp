#if defined(__APPLE__)
#include <Renderer/Backend/Metal/MetalSwapchain.h>

#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/CAMetalDrawable.hpp>
#include <Metal/Metal.hpp>

#include <Renderer/Backend/Metal/MetalDevice.h>

namespace {
    MTL::PixelFormat ToMetalPixelFormat(const RHIFormat format) {
        switch (format) {
            case RHIFormat::RGBA8:
                return MTL::PixelFormatRGBA8Unorm;
            case RHIFormat::BGRA8:
                return MTL::PixelFormatBGRA8Unorm;
            default:
                return MTL::PixelFormatBGRA8Unorm;
        }
    }
}

MetalSwapchain::~MetalSwapchain() {
    Shutdown();
}

bool MetalSwapchain::Initialize(RHIDevice *device, const RHISwapchainDesc &desc) {
    if (device == nullptr || device->GetBackend() != RHIBackend::Metal) {
        return false;
    }

    return Initialize(static_cast<MetalDevice*>(device), desc);
}

bool MetalSwapchain::Initialize(MetalDevice *device, const RHISwapchainDesc &desc) {
    if (m_IsInitialized) {
        return true;
    }

    if (device == nullptr || device->GetNativeDevice() == nullptr) {
        return false;
    }

    m_Device = device;
    m_Width = desc.width;
    m_Height = desc.height;
    m_ColorFormat = desc.colorFormat;
    m_PresentMode = desc.presentMode;

    if (!ConfigureLayer(desc)) {
        Shutdown();
        return false;
    }

    m_IsInitialized = true;

    return true;
}

void MetalSwapchain::Shutdown() {
    m_BackBuffer.reset();

    if (m_CurrentDrawable != nullptr) {
        m_CurrentDrawable->release();
        m_CurrentDrawable = nullptr;
    }

    if (m_Layer != nullptr) {
        m_Layer->release();
        m_Layer = nullptr;
    }

    m_Device = nullptr;
    m_Width = 0;
    m_Height = 0;
    m_ColorFormat = RHIFormat::BGRA8;
    m_PresentMode = RHIPresentMode::VSync;
    m_OwnsLayer = false;
    m_IsInitialized = false;
}

bool MetalSwapchain::Resize(RHIUInt32 width, RHIUInt32 height) {
    if (!m_IsInitialized || m_Layer == nullptr || width == 0 || height == 0) {
        return false;
    }

    m_Width = width;
    m_Height = height;

    const CGSize drawableSize{
        static_cast<CGFloat>(width),
        static_cast<CGFloat>(height)
    };

    m_Layer->setDrawableSize(drawableSize);

    return true;
}

bool MetalSwapchain::AcquireNextImage() {
    if (!m_IsInitialized || m_Layer == nullptr) {
        return false;
    }

    m_BackBuffer.reset();

    if (m_CurrentDrawable != nullptr) {
        m_CurrentDrawable->release();
        m_CurrentDrawable = nullptr;
    }

    m_CurrentDrawable = m_Layer->nextDrawable();

    if (m_CurrentDrawable == nullptr) {
        return false;
    }

    m_CurrentDrawable->retain();

    const RHITextureDesc backBufferDesc{
        .width = m_Width,
        .height = m_Height,
        .format = m_ColorFormat,
        .usage = RHITextureUsage::RenderTarget,
        .debugName = "SwapchainBackBuffer"
    };

    m_BackBuffer = std::make_unique<MetalTexture>();

    if (!m_BackBuffer->InitializeWrapped(m_CurrentDrawable->texture(), backBufferDesc, false)) {
        m_BackBuffer.reset();
        return false;
    }

    return true;
}

void MetalSwapchain::Present() {
    MTL::CommandBuffer* command_buffer = nullptr;

    if (m_Device != nullptr) {
        command_buffer = m_Device->GetNativeCommandBuffer();
    }

    Present(command_buffer);
}

void MetalSwapchain::Present(MTL::CommandBuffer *commandBuffer) {
    if (!m_IsInitialized || m_CurrentDrawable == nullptr || commandBuffer == nullptr) {
        return;
    }

    commandBuffer->presentDrawable(m_CurrentDrawable);

    m_BackBuffer.reset();

    m_CurrentDrawable->release();
    m_CurrentDrawable = nullptr;
}

RHITexture* MetalSwapchain::GetCurrentBackBuffer() const {
    return m_BackBuffer.get();
}

RHIUInt32 MetalSwapchain::GetWidth() const {
    return m_Width;
}

RHIUInt32 MetalSwapchain::GetHeight() const {
    return m_Height;
}

RHIFormat MetalSwapchain::GetColorFormat() const {
    return m_ColorFormat;
}

bool MetalSwapchain::IsInitialized() const {
    return m_IsInitialized;
}

CA::MetalLayer* MetalSwapchain::GetNativeLayer() const {
    return m_Layer;
}

CA::MetalDrawable* MetalSwapchain::GetCurrentDrawable() const {
    return m_CurrentDrawable;
}

MTL::Texture* MetalSwapchain::GetCurrentDrawableTexture() const {
    if (m_CurrentDrawable == nullptr) {
        return nullptr;
    }

    return m_CurrentDrawable->texture();
}

bool MetalSwapchain::ConfigureLayer(const RHISwapchainDesc &desc) {
    if (m_Device == nullptr || m_Device->GetNativeDevice() == nullptr) {
        return false;
    }

    if (desc.nativeWindow != nullptr) {
        m_Layer = static_cast<CA::MetalLayer *>(desc.nativeWindow);
        m_Layer->retain();
        m_OwnsLayer = false;
    } else {
        m_Layer = CA::MetalLayer::layer();
        m_Layer->retain();
        m_OwnsLayer = true;
    }

    if (m_Layer == nullptr) {
        return false;
    }

    m_Layer->setDevice(m_Device->GetNativeDevice());
    m_Layer->setPixelFormat(ToMetalPixelFormat(desc.colorFormat));
    m_Layer->setFramebufferOnly(true);
    m_Layer->setAllowsNextDrawableTimeout(false);
    m_Layer->setDisplaySyncEnabled(desc.presentMode == RHIPresentMode::VSync);

    const CGSize drawableSize {
        static_cast<CGFloat>(m_Width),
        static_cast<CGFloat>(m_Height)
    };

    m_Layer->setDrawableSize(drawableSize);

    return true;
}
#endif