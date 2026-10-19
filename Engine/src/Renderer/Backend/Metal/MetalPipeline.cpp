#if defined(__APPLE__)
#include <Renderer/Backend/Metal/MetalPipeline.h>

#include <Metal/Metal.hpp>

#include <Renderer/Backend/Metal/MetalShader.h>

namespace {
    MTL::PixelFormat ToMetalPixelFormat(const RHIFormat format) {
        switch (format) {
            case RHIFormat::R8:
                return MTL::PixelFormatR8Unorm;
            case RHIFormat::RG8:
                return MTL::PixelFormatRG8Unorm;
            case RHIFormat::RGBA8:
                return MTL::PixelFormatRGBA8Unorm;
            case RHIFormat::BGRA8:
                return MTL::PixelFormatBGRA8Unorm;
            case RHIFormat::Depth32:
                return MTL::PixelFormatDepth32Float;
            case RHIFormat::Depth24Stencil8:
                return MTL::PixelFormatDepth24Unorm_Stencil8;
            case RHIFormat::None:
            case RHIFormat::RGB8:
            default:
                return MTL::PixelFormatInvalid;
        }
    }

    MTL::CompareFunction ToMetalCompareFunction(const RHICompareOp compareOp) {
        switch (compareOp) {
            case RHICompareOp::Never:
                return MTL::CompareFunctionNever;
            case RHICompareOp::Less:
                return MTL::CompareFunctionLess;
            case RHICompareOp::Equal:
                return MTL::CompareFunctionEqual;
            case RHICompareOp::LessEqual:
                return MTL::CompareFunctionLessEqual;
            case RHICompareOp::Greater:
                return MTL::CompareFunctionGreater;
            case RHICompareOp::NotEqual:
                return MTL::CompareFunctionNotEqual;
            case RHICompareOp::GreaterEqual:
                return MTL::CompareFunctionGreaterEqual;
            case RHICompareOp::Always:
            default:
                return MTL::CompareFunctionAlways;
        }
    }

    MTL::BlendFactor ToMetalBlendFactor(const RHIBlendFactor blendFactor) {
        switch (blendFactor) {
            case RHIBlendFactor::Zero:
                return MTL::BlendFactorZero;
            case RHIBlendFactor::One:
                return MTL::BlendFactorOne;
            case RHIBlendFactor::SrcColor:
                return MTL::BlendFactorSourceColor;
            case RHIBlendFactor::OneMinusSrcColor:
                return MTL::BlendFactorOneMinusSourceColor;
            case RHIBlendFactor::DstColor:
                return MTL::BlendFactorDestinationColor;
            case RHIBlendFactor::OneMinusDstColor:
                return MTL::BlendFactorOneMinusDestinationColor;
            case RHIBlendFactor::SrcAlpha:
                return MTL::BlendFactorSourceAlpha;
            case RHIBlendFactor::OneMinusSrcAlpha:
                return MTL::BlendFactorOneMinusSourceAlpha;
            case RHIBlendFactor::DstAlpha:
                return MTL::BlendFactorDestinationAlpha;
            case RHIBlendFactor::OneMinusDstAlpha:
                return MTL::BlendFactorOneMinusDestinationAlpha;
            default:
                return MTL::BlendFactorOne;
        }
    }

    MTL::BlendOperation ToMetalBlendOperation(const RHIBlendOp blendOp) {
        switch (blendOp) {
            case RHIBlendOp::Add:
                return MTL::BlendOperationAdd;
            case RHIBlendOp::Subtract:
                return MTL::BlendOperationSubtract;
            case RHIBlendOp::ReverseSubtract:
                return MTL::BlendOperationReverseSubtract;
            case RHIBlendOp::Min:
                return MTL::BlendOperationMin;
            case RHIBlendOp::Max:
                return MTL::BlendOperationMax;
            default:
                return MTL::BlendOperationAdd;
        }
    }

    MTL::VertexFormat ToMetalVertexFormat(const RHIFormat format) {
        switch (format) {
            case RHIFormat::R8:
                return MTL::VertexFormatUCharNormalized;
            case RHIFormat::RG8:
                return MTL::VertexFormatUChar2Normalized;
            case RHIFormat::RGBA8:
            case RHIFormat::BGRA8:
                return MTL::VertexFormatUChar4Normalized;
            default:
                return MTL::VertexFormatInvalid;
        }
    }

    MTL::VertexStepFunction ToMetalVertexStepFunction(const RHIVertexInputRate inputRate) {
        switch (inputRate) {
            case RHIVertexInputRate::Vertex:
                return MTL::VertexStepFunctionPerVertex;
            case RHIVertexInputRate::Instance:
                return MTL::VertexStepFunctionPerInstance;
            default:
                return MTL::VertexStepFunctionPerVertex;
        }
    }

    MTL::ColorWriteMask ToMetalColorWriteMask(const RHIBlendStateDesc& blendState) {
        MTL::ColorWriteMask writeMask = MTL::ColorWriteMaskNone;

        if (blendState.writeRed) {
            writeMask = static_cast<MTL::ColorWriteMask>(writeMask | MTL::ColorWriteMaskRed);
        }

        if (blendState.writeGreen) {
            writeMask = static_cast<MTL::ColorWriteMask>(writeMask | MTL::ColorWriteMaskGreen);
        }

        if (blendState.writeBlue) {
            writeMask = static_cast<MTL::ColorWriteMask>(writeMask | MTL::ColorWriteMaskBlue);
        }

        if (blendState.writeAlpha) {
            writeMask = static_cast<MTL::ColorWriteMask>(writeMask | MTL::ColorWriteMaskAlpha);
        }

        return writeMask;
    }
}

MetalPipeline::~MetalPipeline() {
    Shutdown();
}

bool MetalPipeline::InitializeGraphics(const RHIGraphicsPipelineDesc& desc) {
    (void) desc;
    return false;
}

bool MetalPipeline::InitializeCompute(const RHIComputePipelineDesc& desc) {
    (void) desc;
    return false;
}

void MetalPipeline::Shutdown() {
    if (!m_IsInitialized) {
        return;
    }

    if (m_DepthStencilState != nullptr) {
        m_DepthStencilState->release();
        m_DepthStencilState = nullptr;
    }

    if (m_ComputePipelineState != nullptr) {
        m_ComputePipelineState->release();
        m_ComputePipelineState = nullptr;
    }

    if (m_RenderPipelineState != nullptr) {
        m_RenderPipelineState->release();
        m_RenderPipelineState = nullptr;
    }

    if (m_Device != nullptr) {
        m_Device->release();
        m_Device = nullptr;
    }

    m_Type = RHIPipelineType::None;
    m_CullMode = RHICullMode::Back;
    m_FrontFace = RHIFrontFace::CounterClockwise;
    m_FillMode = RHIFillMode::Solid;
    m_IsInitialized = false;
}

bool MetalPipeline::InitializeGraphics(MTL::Device *device, const RHIGraphicsPipelineDesc &desc) {
    if (m_IsInitialized) {
        return true;
    }

    if (device == nullptr || desc.vertexShader == nullptr || desc.fragmentShader == nullptr) {
        return false;
    }

    m_Device = device;
    m_Device->retain();

    auto* pipelineDesc = MTL::RenderPipelineDescriptor::alloc()->init();

    pipelineDesc->colorAttachments()->object(0)->setPixelFormat(ToMetalPixelFormat(desc.colorFormat));
    pipelineDesc->setDepthAttachmentPixelFormat(ToMetalPixelFormat(desc.depthStencilFormat));

    auto* colorAttachment = pipelineDesc->colorAttachments()->object(0);

    colorAttachment->setBlendingEnabled(desc.blendState.blendEnabled);
    colorAttachment->setSourceRGBBlendFactor(ToMetalBlendFactor(desc.blendState.sourceColorBlendFactor));
    colorAttachment->setDestinationRGBBlendFactor(ToMetalBlendFactor(desc.blendState.destinationColorBlendFactor));
    colorAttachment->setRgbBlendOperation(ToMetalBlendOperation(desc.blendState.colorBlendOp));
    colorAttachment->setSourceAlphaBlendFactor(ToMetalBlendFactor(desc.blendState.sourceAlphaBlendFactor));
    colorAttachment->setDestinationAlphaBlendFactor(ToMetalBlendFactor(desc.blendState.destinationAlphaBlendFactor));
    colorAttachment->setAlphaBlendOperation(ToMetalBlendOperation(desc.blendState.alphaBlendOp));
    colorAttachment->setWriteMask(ToMetalColorWriteMask(desc.blendState));

    if (desc.vertexLayout.attributeCount > 0 || desc.vertexLayout.bindingCount > 0) {
        auto* vertexDescriptor = MTL::VertexDescriptor::alloc()->init();

        for (std::size_t i = 0; i < desc.vertexLayout.attributeCount; ++i) {
            const RHIVertexAttributeDesc& attribute = desc.vertexLayout.attributes[i];

            vertexDescriptor->attributes()->object(i)->setFormat(ToMetalVertexFormat(attribute.format));
            vertexDescriptor->attributes()->object(i)->setOffset(attribute.offset);
            vertexDescriptor->attributes()->object(i)->setBufferIndex(attribute.binding);
        }

        for (std::size_t j = 0; j < desc.vertexLayout.bindingCount; ++j) {
            const RHIVertexBindingDesc& binding = desc.vertexLayout.bindings[j];

            vertexDescriptor->layouts()->object(j)->setStride(binding.stride);
            vertexDescriptor->layouts()->object(j)->setStepFunction(ToMetalVertexStepFunction(binding.inputRate));
            vertexDescriptor->layouts()->object(j)->setStepRate(1);
        }

        pipelineDesc->setVertexDescriptor(vertexDescriptor);
        vertexDescriptor->release();
    }

    NS::Error* error = nullptr;

    m_RenderPipelineState = m_Device->newRenderPipelineState(pipelineDesc, &error);

    pipelineDesc->release();

    if (m_RenderPipelineState == nullptr) {
        if (m_Device != nullptr) {
            m_Device->release();
            m_Device = nullptr;
        }

        return false;
    }

    auto* depthStencilDesc = MTL::DepthStencilDescriptor::alloc()->init();

    depthStencilDesc->setDepthCompareFunction(ToMetalCompareFunction(desc.depthStencilState.depthCompareOp));

    m_DepthStencilState = m_Device->newDepthStencilState(depthStencilDesc);

    depthStencilDesc->release();

    m_CullMode = desc.rasterizerState.CullMode;
    m_FrontFace = desc.rasterizerState.FrontFace;
    m_FillMode = desc.rasterizerState.FillMode;

    m_Type = RHIPipelineType::Graphics;
    m_IsInitialized = true;

    return true;
}

bool MetalPipeline::InitializeCompute(MTL::Device *device, const RHIComputePipelineDesc &desc) {
    if (m_IsInitialized) {
        return true;
    }

    if (device == nullptr || desc.computeShader == nullptr) {
        return false;
    }

    m_Device = device;
    m_Device->retain();

    if (m_ComputePipelineState == nullptr) {
        if (m_Device != nullptr) {
            m_Device->release();
            m_Device = nullptr;
        }

        return false;
    }

    m_Type = RHIPipelineType::Compute;
    m_IsInitialized = true;

    return true;
}

RHIPipelineType MetalPipeline::GetType() const {
    return m_Type;
}

bool MetalPipeline::IsInitialized() const {
    return m_IsInitialized;
}

MTL::RenderPipelineState * MetalPipeline::GetNativeRenderPipelineState() const {
    return m_RenderPipelineState;
}

MTL::ComputePipelineState * MetalPipeline::GetNativeComputePipelineState() const {
    return m_ComputePipelineState;
}

MTL::DepthStencilState * MetalPipeline::GetNativeDepthStencilState() const {
    return m_DepthStencilState;
}

RHICullMode MetalPipeline::GetCullMode() const {
    return m_CullMode;
}

RHIFrontFace MetalPipeline::GetFrontFace() const {
    return m_FrontFace;
}

RHIFillMode MetalPipeline::GetFillMode() const {
    return m_FillMode;
}
#endif