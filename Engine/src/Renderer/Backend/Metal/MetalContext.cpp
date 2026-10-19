#if defined(__APPLE__)
#include <Renderer/Backend/Metal/MetalContext.h>

#include <Metal/MTLCommandBuffer.hpp>

MetalContext::~MetalContext() {
    Shutdown();
}

bool MetalContext::Initialize(const RHIContextDesc &desc) {
    if (m_isInitialized) {
        return true;
    }

    if (desc.backend != RHIBackend::Metal) {
        return false;
    }

    m_device = std::make_unique<MetalDevice>();

    const RHIDeviceDesc device_desc {
        .backend =  RHIBackend::Metal,
        .EnableValidation = desc.enableValidation,
    };

    if (!m_device->Initialize(device_desc)) {
        m_device.reset();

        return false;
    }

    m_isFrameActive = false;
    m_isInitialized = true;

    return true;
}

void MetalContext::Shutdown() {
    if (!m_isInitialized) {
        return;
    }

    if (m_isFrameActive) {
        EndFrame();
    }

    m_frameCommandList.reset();

    if (m_device != nullptr) {
        m_device->Shutdown();
        m_device.reset();
    }

    m_isFrameActive = false;
    m_isInitialized = false;
}

bool MetalContext::BeginFrame() {
    if (!m_isInitialized || m_isFrameActive || m_device == nullptr) {
        return false;
    }

    m_device->BeginFrame();

    MTL::CommandBuffer* command_buffer = m_device->GetNativeCommandBuffer();

    if (command_buffer == nullptr) {
        return false;
    }

    m_frameCommandList = std::make_unique<MetalCommandList>();

    if (!m_frameCommandList->Initialize(command_buffer, RHICommandListType::Graphics)) {
        m_frameCommandList.reset();
        m_device->EndFrame();
        return false;
    }

    if (!m_frameCommandList->Begin()) {
        m_frameCommandList->Shutdown();
        m_frameCommandList.reset();
        m_device->EndFrame();
        return false;
    }

    m_isFrameActive = true;

    return true;
}

void MetalContext::EndFrame() {
    if (!m_isInitialized || !m_isFrameActive) {
        return;
    }

    if (m_frameCommandList != nullptr) {
        if (m_frameCommandList->IsRecording()) {
            m_frameCommandList->End();
        }

        m_frameCommandList->Shutdown();
        m_frameCommandList.reset();
    }

    if (m_device != nullptr) {
        m_device->EndFrame();
    }

    m_isFrameActive = false;
}

RHIBackend MetalContext::GetBackend() const {
    return RHIBackend::Metal;
}

RHIDevice * MetalContext::GetDevice() const {
    return m_device.get();
}

RHICommandList * MetalContext::GetCommandList() const {
    return m_frameCommandList.get();
}

bool MetalContext::IsInitialized() const {
    return m_isInitialized;
}

bool MetalContext::IsFrameActive() const {
    return m_isFrameActive;
}

MetalDevice * MetalContext::GetMetalDevice() const {
    return m_device.get();
}

MetalCommandList * MetalContext::GetMetalFrameCommandList() const {
    return m_frameCommandList.get();
}

#endif