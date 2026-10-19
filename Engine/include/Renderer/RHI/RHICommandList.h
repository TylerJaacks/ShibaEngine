#ifndef SHIBAENGINE_RHICOMMANDLIST_H
#define SHIBAENGINE_RHICOMMANDLIST_H

#include <Renderer/RHI/RHI.h>

class RHIBuffer;
class RHITexture;
class RHIPipeline;

enum class RHICommandListType : RHIUInt32 {
    Graphics,
    Compute,
    Copy
};

enum class RHIPrimitiveTopology : RHIUInt8 {
    TriangleList,
    TriangleStrip,
    LineList,
    LineStrip,
    PointList
};

struct RHIViewport {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
    float minDepth = 0.0f;
    float maxDepth = 1.0f;
};

struct RHIRect {
    RHIInt32 x = 0;
    RHIInt32 y = 0;
    RHIInt32 width = 0;
    RHIInt32 height = 0;
};

struct RHIColor {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 1.0f;
};

struct RHIRenderPassDesc {
    RHITexture *colorTarget = nullptr;
    RHITexture *depthStencilTarget = nullptr;

    RHIColor clearColor = {};
    float clearDepth = 1.0f;
    RHIUInt32 clearStencil = 0;

    bool clearColorTarget = true;
    bool clearDepthStencilTarget = true;
    bool clearStencilTarget = true;
};

class RHICommandList {
public:
    RHICommandList() = default;

    virtual ~RHICommandList() = default;

    RHICommandList(const RHICommandList &) = delete;

    RHICommandList &operator=(const RHICommandList &) = delete;

    RHICommandList(RHICommandList &&) = delete;

    RHICommandList &operator=(RHICommandList &&) = delete;

    virtual bool Begin() = 0;

    virtual void End() = 0;

    virtual void BeginRenderPass(const RHIRenderPassDesc &renderPassDesc) = 0;

    virtual void EndRenderPass() = 0;

    virtual void SetPipeline(RHIPipeline *pipeline) = 0;

    virtual void SetPrimitiveTopology(RHIPrimitiveTopology topology) = 0;

    virtual void SetViewport(const RHIViewport &viewport) = 0;

    virtual void SetScissorRect(const RHIRect &rect) = 0;

    virtual void SetVertexBuffer(RHIBuffer *buffer, RHIUInt32 slot, RHIUInt64 offset) = 0;

    virtual void SetIndexBuffer(RHIBuffer *buffer, RHIUInt64 offset) = 0;

    virtual void Draw(
        RHIUInt32 vertexCount,
        RHIUInt32 instanceCount,
        RHIUInt32 firstVertex,
        RHIUInt32 firstInstance
    ) = 0;

    virtual void DrawIndexed(
        RHIUInt32 indexCount,
        RHIUInt32 instanceCount,
        RHIUInt32 firstIndex,
        RHIInt32 vertexOffset,
        RHIUInt32 firstInstance
    ) = 0;

    virtual void Dispatch(
        RHIUInt32 groupCountX,
        RHIUInt32 groupCountY,
        RHIUInt32 groupCountZ
    ) = 0;

    virtual void CopyBuffer(
        RHIBuffer *source,
        RHIBuffer *destination,
        RHIUInt64 size,
        RHIUInt64 sourceOffset,
        RHIUInt64 destinationOffset
    ) = 0;

    [[nodiscard]] virtual RHICommandListType GetType() const = 0;
    [[nodiscard]] virtual bool IsRecording() const = 0;
};

#endif //SHIBAENGINE_RHICOMMANDLIST_H
