#if defined(__APPLE__)
#include <Metal/Metal.hpp>
#include <Metal/MTLCommandBuffer.hpp>

#include <Metal/MTLBlitCommandEncoder.hpp>
#include <Metal/MTLComputeCommandEncoder.hpp>
#include <Metal/MTLRenderCommandEncoder.hpp>

#include <Renderer/Backend/Metal/MetalCommandList.h>
#include <Renderer/Backend/Metal/MetalBuffer.h>
#include <Renderer/Backend/Metal/MetalPipeline.h>
#include <Renderer/Backend/Metal/MetalTexture.h>

namespace {
    MTL::PrimitiveType ToMetalPrimitiveType(const RHIPrimitiveTopology topology) {
        switch (topology) {
            case RHIPrimitiveTopology::TriangleStrip:
                return MTL::PrimitiveTypeTriangleStrip;
            case RHIPrimitiveTopology::LineList:
                return MTL::PrimitiveTypeLine;
            case RHIPrimitiveTopology::LineStrip:
                return MTL::PrimitiveTypeLineStrip;
            case RHIPrimitiveTopology::PointList:
                return MTL::PrimitiveTypePoint;
            case RHIPrimitiveTopology::TriangleList:
            default:
                return MTL::PrimitiveTypeTriangle;
        }
    }

    MTL::CullMode ToMetalCullMode(const RHICullMode cullMode) {
        switch (cullMode) {
            case RHICullMode::Front:
                return MTL::CullModeFront;
            case RHICullMode::Back:
                return MTL::CullModeBack;
            case RHICullMode::None:
            default:
                return MTL::CullModeNone;
        }
    }
}

bool MetalCommandList::Initialize(MTL::CommandBuffer *commandBuffer, RHICommandListType type) {
    if (m_isInitialized) {
        return true;
    }

    if (commandBuffer == nullptr) {
        return false;
    }

    m_commandBuffer = commandBuffer;
    m_commandBuffer->retain();

    m_type = type;
    m_primitiveTopology = RHIPrimitiveTopology::TriangleList;
    m_isRecording = false;
    m_isInRenderPass = false;
    m_isInitialized = true;

    return true;
}

void MetalCommandList::Shutdown() {
    if (!m_isInitialized) {
        return;
    }

    if (m_renderCommandEncoder != nullptr) {
        m_renderCommandEncoder->endEncoding();
        m_renderCommandEncoder->release();
        m_renderCommandEncoder = nullptr;
    }

    if (m_computeCommandEncoder != nullptr) {
        m_computeCommandEncoder->endEncoding();
        m_computeCommandEncoder = nullptr;
    }

    if (m_blitCommandEncoder != nullptr) {
        m_blitCommandEncoder->endEncoding();
        m_blitCommandEncoder = nullptr;
    }

    if (m_commandBuffer != nullptr) {
        m_commandBuffer->release();
        m_commandBuffer = nullptr;
    }

    m_type = RHICommandListType::Graphics;
    m_primitiveTopology = RHIPrimitiveTopology::TriangleList;
    m_isRecording = false;
    m_isInRenderPass = false;
    m_isInitialized = false;
}

bool MetalCommandList::Begin() {
    if (!m_isInitialized || m_commandBuffer == nullptr || m_isRecording) {
        return false;
    }

    m_isRecording = true;

    return true;
}

void MetalCommandList::End() {
    if (!m_isRecording) {
        return;
    }

    if (m_renderCommandEncoder != nullptr) {
        m_renderCommandEncoder->endEncoding();
        m_renderCommandEncoder->release();
        m_renderCommandEncoder = nullptr;
    }

    if (m_computeCommandEncoder != nullptr) {
        m_computeCommandEncoder->endEncoding();
        m_computeCommandEncoder = nullptr;
    }

    if (m_blitCommandEncoder != nullptr) {
        m_blitCommandEncoder->endEncoding();
        m_blitCommandEncoder = nullptr;
    }

    m_isInRenderPass = false;
    m_isRecording = false;
}

void MetalCommandList::BeginRenderPass(const RHIRenderPassDesc &desc) {
    if (!m_isRecording || m_commandBuffer == nullptr || m_isInRenderPass) {
        return;
    }

    const auto *colorTexture = static_cast<MetalTexture *>(desc.colorTarget);

    if (colorTexture == nullptr || colorTexture->GetNativeTexture() == nullptr) {
        return;
    }

    const auto *renderPassDescriptor = MTL::RenderPassDescriptor::renderPassDescriptor();

    auto *colorAttachment = renderPassDescriptor->colorAttachments()->object(0);
    colorAttachment->setTexture(colorTexture->GetNativeTexture());
    colorAttachment->setLoadAction(desc.clearColorTarget ? MTL::LoadActionClear : MTL::LoadActionLoad);
    colorAttachment->setStoreAction(MTL::StoreActionStore);
    colorAttachment->setClearColor(MTL::ClearColor::Make(
        desc.clearColor.r,
        desc.clearColor.g,
        desc.clearColor.b,
        desc.clearColor.a
    ));

    const auto *depthTexture = static_cast<MetalTexture *>(desc.depthStencilTarget);

    if (depthTexture != nullptr && depthTexture->GetNativeTexture() != nullptr) {
        auto *depthAttachment = renderPassDescriptor->depthAttachment();
        depthAttachment->setTexture(depthTexture->GetNativeTexture());
        depthAttachment->setLoadAction(desc.clearDepthStencilTarget ? MTL::LoadActionClear : MTL::LoadActionLoad);
        depthAttachment->setStoreAction(MTL::StoreActionDontCare);
        depthAttachment->setClearDepth(desc.clearDepth);
    }

    m_renderCommandEncoder = m_commandBuffer->renderCommandEncoder(renderPassDescriptor);

    if (m_renderCommandEncoder == nullptr) {
        return;
    }

    m_renderCommandEncoder->retain();
    m_isInRenderPass = true;
}

void MetalCommandList::EndRenderPass() {
    if (!m_isInRenderPass) {
        return;
    }

    if (m_renderCommandEncoder != nullptr) {
        m_renderCommandEncoder->endEncoding();
        m_renderCommandEncoder->release();
        m_renderCommandEncoder = nullptr;
    }

    m_currentPipeline = nullptr;
    m_indexBuffer = nullptr;
    m_isInRenderPass = false;
}

void MetalCommandList::SetPipeline(RHIPipeline *pipeline) {
    if (!m_isRecording || m_renderCommandEncoder == nullptr || pipeline == nullptr) {
        return;
    }

    auto *metalPipeline = static_cast<MetalPipeline *>(pipeline);

    if (metalPipeline->GetNativeRenderPipelineState() == nullptr) {
        return;
    }

    m_currentPipeline = metalPipeline;

    m_renderCommandEncoder->setRenderPipelineState(metalPipeline->GetNativeRenderPipelineState());

    if (metalPipeline->GetNativeDepthStencilState() != nullptr) {
        m_renderCommandEncoder->setDepthStencilState(metalPipeline->GetNativeDepthStencilState());
    }

    m_renderCommandEncoder->setCullMode(ToMetalCullMode(metalPipeline->GetCullMode()));
    m_renderCommandEncoder->setFrontFacingWinding(
        metalPipeline->GetFrontFace() == RHIFrontFace::Clockwise
            ? MTL::WindingClockwise
            : MTL::WindingCounterClockwise
    );
    m_renderCommandEncoder->setTriangleFillMode(
        metalPipeline->GetFillMode() == RHIFillMode::Wireframe
            ? MTL::TriangleFillModeLines
            : MTL::TriangleFillModeFill
    );
}

void MetalCommandList::SetPrimitiveTopology(RHIPrimitiveTopology topology) {
    m_primitiveTopology = topology;
}

void MetalCommandList::SetViewport(const RHIViewport &viewport) {
    if (!m_isRecording || m_renderCommandEncoder == nullptr) {
        return;
    }

    const MTL::Viewport metalViewport{
        .originX = viewport.x,
        .originY = viewport.y,
        .width = viewport.width,
        .height = viewport.height,
        .znear = viewport.minDepth,
        .zfar = viewport.maxDepth
    };

    m_renderCommandEncoder->setViewport(metalViewport);
}

void MetalCommandList::SetScissorRect(const RHIRect &rect) {
    if (!m_isRecording || m_renderCommandEncoder == nullptr) {
        return;
    }

    const MTL::ScissorRect metalScissorRect{
        .x = static_cast<NS::UInteger>(rect.x),
        .y = static_cast<NS::UInteger>(rect.y),
        .width = static_cast<NS::UInteger>(rect.width),
        .height = static_cast<NS::UInteger>(rect.height)
    };

    m_renderCommandEncoder->setScissorRect(metalScissorRect);
}

void MetalCommandList::SetVertexBuffer(RHIBuffer *buffer, const RHIUInt32 slot, const RHIUInt64 offset) {
    if (!m_isRecording || m_renderCommandEncoder == nullptr) {
        return;
    }

    auto *metalBuffer = static_cast<MetalBuffer *>(buffer);

    if (metalBuffer == nullptr) {
        return;
    }

    m_renderCommandEncoder->setVertexBuffer(metalBuffer->GetNativeBuffer(), offset, slot);
}

void MetalCommandList::SetIndexBuffer(RHIBuffer *buffer, const RHIUInt64 offset) {
    if (!m_isRecording) {
        return;
    }

    auto *metalBuffer = static_cast<MetalBuffer *>(buffer);

    m_indexBuffer = metalBuffer != nullptr ? metalBuffer->GetNativeBuffer() : nullptr;
    m_indexBufferOffset = offset;
}

void MetalCommandList::Draw(
    const RHIUInt32 vertexCount,
    const RHIUInt32 instanceCount,
    const RHIUInt32 firstVertex,
    const RHIUInt32 firstInstance
) {
    if (!m_isRecording || m_renderCommandEncoder == nullptr) {
        return;
    }

    m_renderCommandEncoder->drawPrimitives(
        ToMetalPrimitiveType(m_primitiveTopology),
        firstVertex,
        vertexCount,
        instanceCount,
        firstInstance
    );
}

void MetalCommandList::DrawIndexed(
    const RHIUInt32 indexCount,
    const RHIUInt32 instanceCount,
    const RHIUInt32 firstIndex,
    const RHIInt32 vertexOffset,
    const RHIUInt32 firstInstance
) {
    if (!m_isRecording || m_renderCommandEncoder == nullptr) {
        return;
    }

    if (m_indexBuffer == nullptr) {
        return;
    }

    // Index buffers are expected to hold 32-bit indices.
    constexpr NS::UInteger indexSize = sizeof(std::uint32_t);

    m_renderCommandEncoder->drawIndexedPrimitives(
        ToMetalPrimitiveType(m_primitiveTopology),
        indexCount,
        MTL::IndexTypeUInt32,
        m_indexBuffer,
        m_indexBufferOffset + firstIndex * indexSize,
        instanceCount,
        vertexOffset,
        firstInstance
    );
}

void MetalCommandList::Dispatch(
    const RHIUInt32 groupCountX,
    const RHIUInt32 groupCountY,
    const RHIUInt32 groupCountZ
) {
    if (!m_isRecording) {
        return;
    }

    (void) groupCountX;
    (void) groupCountY;
    (void) groupCountZ;
}

void MetalCommandList::CopyBuffer(
    RHIBuffer *source,
    RHIBuffer *destination,
    const RHIUInt64 size,
    const RHIUInt64 sourceOffset,
    const RHIUInt64 destinationOffset
) {
    if (!m_isRecording) {
        return;
    }

    (void) source;
    (void) destination;
    (void) size;
    (void) sourceOffset;
    (void) destinationOffset;
}

RHICommandListType MetalCommandList::GetType() const {
    return m_type;
}

bool MetalCommandList::IsRecording() const {
    return m_isRecording;
}

MTL::CommandBuffer *MetalCommandList::GetNativeCommandBuffer() const {
    return m_commandBuffer;
}

#endif
