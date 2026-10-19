#ifndef SHIBAENGINE_METALCONTEXT_H
#define SHIBAENGINE_METALCONTEXT_H
#if defined(__APPLE__)

#include <memory>

#include "Renderer/RHI/RHIContext.h"
#include "Renderer/Backend/Metal/MetalCommandList.h"
#include "Renderer/Backend/Metal/MetalDevice.h"

class MetalContext final : public RHIContext {
public:
    MetalContext() = default;
    ~MetalContext() override;

    MetalContext(const MetalContext&) = delete;
    MetalContext& operator=(const MetalContext&) = delete;

    MetalContext(MetalContext&&) = delete;
    MetalContext& operator=(MetalContext&&) = delete;

    bool Initialize(const RHIContextDesc& desc) override;
    void Shutdown() override;

    bool BeginFrame() override;
    void EndFrame() override;

    [[nodiscard]] RHIBackend GetBackend() const override;
    [[nodiscard]] RHIDevice* GetDevice() const override;
    [[nodiscard]] RHICommandList* GetCommandList() const override;
    [[nodiscard]] bool IsInitialized() const override;
    [[nodiscard]] bool IsFrameActive() const override;

    [[nodiscard]] MetalDevice* GetMetalDevice() const;
    [[nodiscard]] MetalCommandList* GetMetalFrameCommandList() const;

private:
    std::unique_ptr<MetalDevice> m_device;
    std::unique_ptr<MetalCommandList> m_frameCommandList;
};

#endif
#endif //SHIBAENGINE_METALCONTEXT_H