#ifndef SHIBAENGINE_VULKANDEVICE_H
#define SHIBAENGINE_VULKANDEVICE_H

#include <Renderer/RHI/RHIDevice.h>

class VulkanDevice final : public RHIDevice {
public:
    VulkanDevice() = default;
    ~VulkanDevice() override = default;

    bool Initialize(const RHIDeviceDesc &desc) override;
    void Shutdown() override;

    void BeginFrame() override;
    void EndFrame() override;

    [[nodiscard]] RHIBackend GetBackend() const override;
    [[nodiscard]] const char* GetDeviceName() const override;
};

#endif //SHIBAENGINE_VULKANDEVICE_H