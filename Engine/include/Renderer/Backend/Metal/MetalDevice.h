#ifndef SHIBAENGINE_METALDEVICE_H
#define SHIBAENGINE_METALDEVICE_H
#ifdef __APPLE__

#include <string>

#include <Renderer/RHI/RHIDevice.h>

namespace MTL {
    class Device;
    class CommandQueue;
    class CommandBuffer;
}

class MetalDevice final : public RHIDevice {
public:
    MetalDevice() = default;
    ~MetalDevice() override = default;

    bool Initialize(const RHIDeviceDesc &desc) override;
    void Shutdown() override;

    void BeginFrame() override;
    void EndFrame() override;

    std::unique_ptr<RHISwapchain> CreateSwapchain(const RHISwapchainDesc& desc) override;
    std::unique_ptr<RHIBuffer> CreateBuffer(const RHIBufferDesc& desc) override;
    std::unique_ptr<RHITexture> CreateTexture(const RHITextureDesc& desc) override;
    std::unique_ptr<RHIShader> CreateShader(const RHIShaderDesc& desc) override;
    std::unique_ptr<RHIPipeline> CreateGraphicsPipeline(const RHIGraphicsPipelineDesc& desc) override;
    std::unique_ptr<RHIPipeline> CreateComputePipeline(const RHIComputePipelineDesc& desc) override;

    [[nodiscard]] RHIBackend GetBackend() const override;
    [[nodiscard]] const char* GetDeviceName() const override;

    [[nodiscard]] MTL::Device* GetNativeDevice() const;
    [[nodiscard]] MTL::CommandQueue* GetNativeCommandQueue() const;
    [[nodiscard]] MTL::CommandBuffer* GetNativeCommandBuffer() const;

private:
    MTL::Device* m_device = nullptr;
    MTL::CommandQueue* m_CommandQueue = nullptr;
    MTL::CommandBuffer* m_CurrentCommandBuffer = nullptr;

    std::string m_DeviceName = "Metal";
    bool m_IsFrameActive = false;
};
#endif // defined(__APPLE__)
#endif //SHIBAENGINE_METALDEVICE_H