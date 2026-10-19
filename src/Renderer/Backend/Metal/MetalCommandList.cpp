#if defined(__APPLE__)
#include <Metal/Metal.hpp>
#include <Metal/MTLCommandBuffer.hpp>

#include <Metal/MTLBlitCommandEncoder.hpp>
#include <Metal/MTLComputeCommandEncoder.hpp>
#include <Metal/MTLRenderCommandEncoder.hpp>

#include "Renderer/Backend/Metal/MetalCommandList.h"

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

    (void) desc;

    m_isInRenderPass = true;
}

void MetalCommandList::EndRenderPass() {
    if (!m_isInRenderPass) {
        return;
    }

    if (m_renderCommandEncoder != nullptr) {
        m_renderCommandEncoder->endEncoding();
        m_renderCommandEncoder = nullptr;
    }

    m_isInRenderPass = false;
}

void MetalCommandList::SetPipeline(RHIPipeline *pipeline) {
    if (!m_isRecording) {
        return;
    }

    (void) pipeline;
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

    (void) buffer;
    (void) slot;
    (void) offset;
}

void MetalCommandList::SetIndexBuffer(RHIBuffer *buffer, const RHIUInt64 offset) {
    if (!m_isRecording) {
        return;
    }

    (void) buffer;
    (void) offset;
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

    (void) vertexCount;
    (void) instanceCount;
    (void) firstVertex;
    (void) firstInstance;
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

    (void) indexCount;
    (void) instanceCount;
    (void) firstIndex;
    (void) vertexOffset;
    (void) firstInstance;
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
