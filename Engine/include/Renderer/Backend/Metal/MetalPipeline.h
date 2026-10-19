#ifndef SHIBAENGINE_METALPIPELINE_H
#define SHIBAENGINE_METALPIPELINE_H
#if defined(__APPLE__)

#include <Renderer/RHI/RHIPipeline.h>

namespace MTL {
    class Device;
    class RenderPipelineState;
    class ComputePipelineState;
    class DepthStencilState;
}

class MetalPipeline final : public RHIPipeline {
public:
    MetalPipeline() = default;
    ~MetalPipeline() override;

    MetalPipeline(const MetalPipeline&) = delete;
    MetalPipeline& operator=(const MetalPipeline&) = delete;

    MetalPipeline(MetalPipeline&&) = delete;
    MetalPipeline& operator=(MetalPipeline&&) = delete;

    bool InitializeGraphics(const RHIGraphicsPipelineDesc& desc) override;
    bool InitializeCompute(const RHIComputePipelineDesc& desc) override;
    void Shutdown() override;

    bool InitializeGraphics(MTL::Device* device, const RHIGraphicsPipelineDesc& desc);
    bool InitializeCompute(MTL::Device* device, const RHIComputePipelineDesc& desc);

    [[nodiscard]] RHIPipelineType GetType() const override;
    [[nodiscard]] bool IsInitialized() const override;

    [[nodiscard]] MTL::RenderPipelineState* GetNativeRenderPipelineState() const;
    [[nodiscard]] MTL::ComputePipelineState* GetNativeComputePipelineState() const;
    [[nodiscard]] MTL::DepthStencilState* GetNativeDepthStencilState() const;

    [[nodiscard]] RHICullMode GetCullMode() const;
    [[nodiscard]] RHIFrontFace GetFrontFace() const;
    [[nodiscard]] RHIFillMode GetFillMode() const;

private:
    MTL::Device* m_Device = nullptr;
    MTL::RenderPipelineState* m_RenderPipelineState = nullptr;
    MTL::ComputePipelineState* m_ComputePipelineState = nullptr;
    MTL::DepthStencilState* m_DepthStencilState = nullptr;

    RHIPipelineType m_Type = RHIPipelineType::None;

    RHICullMode m_CullMode = RHICullMode::Back;
    RHIFrontFace m_FrontFace = RHIFrontFace::CounterClockwise;
    RHIFillMode m_FillMode = RHIFillMode::Solid;

    bool m_IsInitialized = false;
};

#endif
#endif //SHIBAENGINE_METALPIPELINE_H