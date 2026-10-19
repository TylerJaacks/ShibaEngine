#ifndef SHIBAENGINE_METALCOMMANDLIST_H
#define SHIBAENGINE_METALCOMMANDLIST_H
#if defined(__APPLE__)
#include <Renderer/RHI/RHICommandList.h>

namespace MTL {
    class CommandBuffer;
    class RenderCommandEncoder;
    class ComputeCommandEncoder;
    class BlitCommandEncoder;
    class Buffer;
}

class MetalPipeline;

class MetalCommandList final : public RHICommandList {
public:
    MetalCommandList() = default;
    ~MetalCommandList() override = default;

    MetalCommandList(const MetalCommandList&) = delete;
    MetalCommandList& operator=(const MetalCommandList&) = delete;

    MetalCommandList(MetalCommandList&&) = delete;
    MetalCommandList& operator=(MetalCommandList&&) = delete;

    bool Initialize(MTL::CommandBuffer* commandBuffer, RHICommandListType type = RHICommandListType::Graphics);
    void Shutdown();

    bool Begin() override;
    void End() override;

    void BeginRenderPass(const RHIRenderPassDesc& desc) override;
    void EndRenderPass() override;

    void SetPipeline(RHIPipeline* pipeline) override;
    void SetPrimitiveTopology(RHIPrimitiveTopology topology) override;

    void SetViewport(const RHIViewport& viewport) override;
    void SetScissorRect(const RHIRect& rect) override;

    void SetVertexBuffer(RHIBuffer* buffer, RHIUInt32 slot, RHIUInt64 offset) override;
    void SetIndexBuffer(RHIBuffer* buffer, RHIUInt64 offset) override;

    void Draw(
        RHIUInt32 vertexCount,
        RHIUInt32 instanceCount,
        RHIUInt32 firstVertex,
        RHIUInt32 firstInstance
    ) override;

    void DrawIndexed(
        RHIUInt32 indexCount,
        RHIUInt32 instanceCount,
        RHIUInt32 firstIndex,
        RHIInt32 vertexOffset,
        RHIUInt32 firstInstance
    ) override;

    void Dispatch(
        RHIUInt32 groupCountX,
        RHIUInt32 groupCountY,
        RHIUInt32 groupCountZ
    ) override;

    void CopyBuffer(
        RHIBuffer* source,
        RHIBuffer* destination,
        RHIUInt64 size,
        RHIUInt64 sourceOffset,
        RHIUInt64 destinationOffset
    ) override;

    [[nodiscard]] RHICommandListType GetType() const override;
    [[nodiscard]] bool IsRecording() const override;

    [[nodiscard]] MTL::CommandBuffer* GetNativeCommandBuffer() const;

private:
    MTL::CommandBuffer* m_commandBuffer = nullptr;
    MTL::RenderCommandEncoder* m_renderCommandEncoder = nullptr;
    MTL::ComputeCommandEncoder* m_computeCommandEncoder = nullptr;
    MTL::BlitCommandEncoder* m_blitCommandEncoder = nullptr;

    RHICommandListType m_type = RHICommandListType::Graphics;
    RHIPrimitiveTopology m_primitiveTopology = RHIPrimitiveTopology::TriangleList;

    MetalPipeline* m_currentPipeline = nullptr;
    MTL::Buffer* m_indexBuffer = nullptr;
    RHIUInt64 m_indexBufferOffset = 0;

    bool m_isInitialized = false;
    bool m_isRecording = false;
    bool m_isInRenderPass = false;
};

#endif
#endif //SHIBAENGINE_METALCOMMANDLIST_H