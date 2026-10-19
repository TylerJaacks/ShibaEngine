#include <Renderer/Backend/Vulkan/VulkanDevice.h>

bool VulkanDevice::Initialize(const RHIDeviceDesc &desc) {
    if (m_IsInitialized) {
        return true;
    }

    m_IsInitialized = true;

    return true;
}

void VulkanDevice::Shutdown() {
    if (!m_IsInitialized) {
        return;
    }

    m_IsInitialized = false;
}

void VulkanDevice::BeginFrame() {

}

void VulkanDevice::EndFrame() {
}

RHIBackend VulkanDevice::GetBackend() const {
    return RHIBackend::Vulkan;
}

const char * VulkanDevice::GetDeviceName() const {
    return "Vulkan";
}