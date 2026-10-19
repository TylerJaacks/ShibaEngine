#include <Renderer/Renderer.h>

#if defined(__APPLE__)
#include <Renderer/Backend/Metal/MetalDevice.h>
#include <Renderer/Backend/Metal/MetalSwapchain.h>
#endif

#include <Platform/Window.h>
#include <Renderer/RHI/RHICommandList.h>

Renderer::Renderer()
    : m_device(nullptr),
      m_context(nullptr),
      m_swapchain(nullptr) {
}

Renderer::~Renderer() {
    Shutdown();
}

void Renderer::Initialize(Window& window, const RendererConfig& config) {
    RHIContextDesc context_desc{
        .backend = config.backend,
        .enableValidation = config.enableValidation
    };

    m_context = RHIContext::Create(context_desc);

    if (m_context == nullptr) {
        return;
    }

    if (!m_context->Initialize(context_desc)) {
        m_context.reset();
        return;
    }

    m_device = m_context->GetDevice();

    if (m_device == nullptr) {
        m_context->Shutdown();
        m_context.reset();
        return;
    }

    const NativeWindowHandle native_window_handle = window.GetNativeHandle();

    RHISwapchainDesc swapchain_desc{
        .nativeWindow = native_window_handle.layer,
        .width = window.GetWidth(),
        .height = window.GetHeight(),
        .colorFormat = RHIFormat::BGRA8,
        .imageCount = 3,
        .presentMode = window.IsVSync() ? RHIPresentMode::VSync : RHIPresentMode::Immediate,
        .enableHDR = false
    };

    m_swapchain = m_device->CreateSwapchain(swapchain_desc);

    if (m_swapchain == nullptr) {
        m_context->Shutdown();
        m_context.reset();
        m_device = nullptr;
    }
}

void Renderer::BeginFrame() const {
    if (m_context == nullptr || m_swapchain == nullptr) {
        return;
    }

    if (!m_context->BeginFrame()) {
        return;
    }

    if (!m_swapchain->AcquireNextImage()) {
        m_context->EndFrame();
        return;
    }

    RHICommandList* command_list = m_context->GetCommandList();

    if (command_list == nullptr) {
        m_context->EndFrame();
        return;
    }

    const RHIRenderPassDesc render_pass_desc{
        .colorTarget = m_swapchain->GetCurrentBackBuffer(),
        .depthStencilTarget = nullptr,
        .clearColor = {
            .r = 0.1f,
            .g = 0.1f,
            .b = 0.15f,
            .a = 1.0f
        },
        .clearDepth = 1.0f,
        .clearStencil = 0,
        .clearColorTarget = true,
        .clearDepthStencilTarget = true,
        .clearStencilTarget = true
    };

    command_list->BeginRenderPass(render_pass_desc);
}

void Renderer::RenderScene(const Camera& camera) {
    (void)camera;
}

void Renderer::EndFrame() const {
    if (m_context == nullptr || m_swapchain == nullptr) {
        return;
    }

    RHICommandList* command_list = m_context->GetCommandList();

    if (command_list != nullptr) {
        command_list->EndRenderPass();
    }

    m_swapchain->Present();
    m_context->EndFrame();
}

void Renderer::Shutdown() {
    if (m_swapchain != nullptr) {
        m_swapchain->Shutdown();
        m_swapchain.reset();
    }

    if (m_context != nullptr) {
        m_context->Shutdown();
        m_context.reset();
    }

    m_device = nullptr;
}

RHIDevice& Renderer::GetDevice() const {
    return *m_device;
}

RHIContext& Renderer::GetContext() const {
    return *m_context;
}

RHISwapchain& Renderer::GetSwapchain() const {
    return *m_swapchain;
}