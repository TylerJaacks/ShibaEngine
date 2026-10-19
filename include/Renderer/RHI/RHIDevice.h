#ifndef SHIBAENGINE_RHIDEVICE_H
#define SHIBAENGINE_RHIDEVICE_H

#include "RHI.h"
#include "Renderer/RHI/RHIBuffer.h"
#include "Renderer/RHI/RHIPipeline.h"
#include "Renderer/RHI/RHIShader.h"
#include "Renderer/RHI/RHISwapchain.h"
#include "Renderer/RHI/RHITexture.h"

#include <memory>

struct RHIDeviceDesc {
    RHIBackend backend;
    bool EnableValidation;
};

class RHIDevice {
public:
    RHIDevice() = default;
    virtual ~RHIDevice() = default;

    RHIDevice(const RHIDevice&) = delete;
    RHIDevice& operator=(const RHIDevice&) = delete;

    RHIDevice(RHIDevice&&) = delete;
    RHIDevice& operator=(RHIDevice&&) = delete;

    virtual bool Initialize(const RHIDeviceDesc& desc) = 0;
    virtual void Shutdown() = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual std::unique_ptr<RHISwapchain> CreateSwapchain(const RHISwapchainDesc& desc) = 0;
    virtual std::unique_ptr<RHIBuffer> CreateBuffer(const RHIBufferDesc& desc) = 0;
    virtual std::unique_ptr<RHITexture> CreateTexture(const RHITextureDesc& desc) = 0;
    virtual std::unique_ptr<RHIShader> CreateShader(const RHIShaderDesc& desc) = 0;
    virtual std::unique_ptr<RHIPipeline> CreateGraphicsPipeline(const RHIGraphicsPipelineDesc& desc) = 0;
    virtual std::unique_ptr<RHIPipeline> CreateComputePipeline(const RHIComputePipelineDesc& desc) = 0;

    [[nodiscard]] virtual RHIBackend GetBackend() const = 0;
    [[nodiscard]] virtual const char* GetDeviceName() const = 0;

protected:
    bool m_IsInitialized = false;
};

#endif //SHIBAENGINE_RHIDEVICE_H