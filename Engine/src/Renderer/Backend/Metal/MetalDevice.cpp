#ifdef __APPLE__
#include <Metal/Metal.hpp>

#include <Renderer/Backend/Metal/MetalDevice.h>

#include <Renderer/Backend/Metal/MetalBuffer.h>
#include <Renderer/Backend/Metal/MetalPipeline.h>
#include <Renderer/Backend/Metal/MetalShader.h>
#include <Renderer/Backend/Metal/MetalSwapchain.h>
#include <Renderer/Backend/Metal/MetalTexture.h>

bool MetalDevice::Initialize(const RHIDeviceDesc& desc) {
    if (m_IsInitialized) {
        return true;
    }

    (void)desc;

    m_device = MTL::CreateSystemDefaultDevice();

    if (m_device == nullptr) {
        return false;
    }

    m_CommandQueue = m_device->newCommandQueue();

    if (m_CommandQueue == nullptr) {
        m_device->release();
        m_device = nullptr;

        return false;
    }

    if (const NS::String* deviceName = m_device->name(); deviceName != nullptr) {
        m_DeviceName = deviceName->utf8String();
    }

    m_CurrentCommandBuffer = nullptr;
    m_IsFrameActive = false;
    m_IsInitialized = true;

    return true;
}

void MetalDevice::Shutdown() {
    if (!m_IsInitialized) {
        return;
    }

    if (m_CurrentCommandBuffer != nullptr) {
        m_CurrentCommandBuffer->release();
        m_CurrentCommandBuffer = nullptr;
    }

    if (m_CommandQueue != nullptr) {
        m_CommandQueue->release();
        m_CommandQueue = nullptr;
    }

    if (m_device != nullptr) {
        m_device->release();
        m_device = nullptr;
    }

    m_DeviceName = "Metal";
    m_IsFrameActive = false;
    m_IsInitialized = false;
}

void MetalDevice::BeginFrame() {
    if (!m_IsInitialized || m_IsFrameActive) {
        return;
    }

    m_CurrentCommandBuffer = m_CommandQueue->commandBuffer();

    if (m_CurrentCommandBuffer == nullptr) {
        return;
    }

    m_CurrentCommandBuffer->retain();
    m_IsFrameActive = true;
}

void MetalDevice::EndFrame() {
    if (!m_IsInitialized || !m_IsFrameActive) {
        return;
    }

    if (m_CurrentCommandBuffer != nullptr) {
        m_CurrentCommandBuffer->commit();
        m_CurrentCommandBuffer->release();
        m_CurrentCommandBuffer = nullptr;
    }

    m_IsFrameActive = false;
}

std::unique_ptr<RHISwapchain> MetalDevice::CreateSwapchain(const RHISwapchainDesc& desc) {
    auto swapchain = std::make_unique<MetalSwapchain>();

    if (!swapchain->Initialize(this, desc)) {
        return nullptr;
    }

    return swapchain;
}

std::unique_ptr<RHIBuffer> MetalDevice::CreateBuffer(const RHIBufferDesc& desc) {
    auto buffer = std::make_unique<MetalBuffer>();

    if (!buffer->Initialize(m_device, desc)) {
        return nullptr;
    }

    return buffer;
}

std::unique_ptr<RHITexture> MetalDevice::CreateTexture(const RHITextureDesc& desc) {
    auto texture = std::make_unique<MetalTexture>();

    if (!texture->Initialize(m_device, desc)) {
        return nullptr;
    }

    return texture;
}

std::unique_ptr<RHIShader> MetalDevice::CreateShader(const RHIShaderDesc& desc) {
    auto shader = std::make_unique<MetalShader>();

    if (!shader->Initialize(m_device, desc)) {
        return nullptr;
    }

    return shader;
}

std::unique_ptr<RHIPipeline> MetalDevice::CreateGraphicsPipeline(const RHIGraphicsPipelineDesc& desc) {
    auto pipeline = std::make_unique<MetalPipeline>();

    if (!pipeline->InitializeGraphics(m_device, desc)) {
        return nullptr;
    }

    return pipeline;
}

std::unique_ptr<RHIPipeline> MetalDevice::CreateComputePipeline(const RHIComputePipelineDesc& desc) {
    auto pipeline = std::make_unique<MetalPipeline>();

    if (!pipeline->InitializeCompute(m_device, desc)) {
        return nullptr;
    }

    return pipeline;
}

RHIBackend MetalDevice::GetBackend() const {
    return RHIBackend::Metal;
}

const char* MetalDevice::GetDeviceName() const {
    return m_DeviceName.c_str();
}

MTL::Device * MetalDevice::GetNativeDevice() const {
    return m_device;
}

MTL::CommandQueue * MetalDevice::GetNativeCommandQueue() const {
    return m_CommandQueue;
}

MTL::CommandBuffer * MetalDevice::GetNativeCommandBuffer() const {
    return m_CurrentCommandBuffer;
}
#endif