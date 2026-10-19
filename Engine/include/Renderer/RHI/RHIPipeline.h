#ifndef SHIBAENGINE_RHIPIPELINE_H
#define SHIBAENGINE_RHIPIPELINE_H

#include <cstddef>
#include <Renderer/RHI/RHI.h>

#include <Renderer/RHI/RHICommandList.h>

class RHIShader;

enum class RHICompareOp : RHIUInt8 {
    Never,
    Less,
    Equal,
    LessEqual,
    Greater,
    GreaterEqual,
    NotEqual,
    Always
};

enum class RHICullMode : RHIUInt8 {
    None,
    Front,
    Back
};

enum class RHIFrontFace : RHIUInt8 {
    Clockwise,
    CounterClockwise
};

enum class RHIFillMode : RHIUInt8 {
    Solid,
    Wireframe
};

enum class RHIBlendFactor : RHIUInt8 {
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    DstColor,
    OneMinusDstColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstAlpha,
    OneMinusDstAlpha
};

enum class RHIBlendOp : RHIUInt8 {
    Add,
    Subtract,
    ReverseSubtract,
    Min,
    Max
};

enum class RHIVertexInputRate : RHIUInt8 {
    Vertex,
    Instance
};

struct RHIRasterizerStateDesc {
    RHICullMode CullMode = RHICullMode::Back;
    RHIFrontFace FrontFace = RHIFrontFace::CounterClockwise;
    RHIFillMode FillMode = RHIFillMode::Solid;
    bool depthClipEnable = true;
};

struct RHIDepthStencilStateDesc {
    bool depthTestEnabled = true;
    bool depthWriteEnabled = true;
    RHICompareOp depthCompareOp = RHICompareOp::LessEqual;
    bool stencilTestEnabled = false;
};

struct RHIBlendStateDesc {
    bool blendEnabled = false;

    RHIBlendFactor sourceColorBlendFactor = RHIBlendFactor::SrcAlpha;
    RHIBlendFactor destinationColorBlendFactor = RHIBlendFactor::OneMinusSrcAlpha;
    RHIBlendOp colorBlendOp = RHIBlendOp::Add;

    RHIBlendFactor sourceAlphaBlendFactor = RHIBlendFactor::One;
    RHIBlendFactor destinationAlphaBlendFactor = RHIBlendFactor::Zero;
    RHIBlendOp alphaBlendOp = RHIBlendOp::Add;

    bool writeRed = true;
    bool writeGreen = true;
    bool writeBlue = true;
    bool writeAlpha = true;
};

struct RHIVertexAttributeDesc {
    RHIUInt32 location = 0;
    RHIUInt32 binding = 0;
    RHIFormat format = RHIFormat::None;
    RHIUInt32 offset = 0;
};

struct RHIVertexBindingDesc {
    RHIUInt32 binding = 0;
    RHIUInt32 stride = 0;
    RHIVertexInputRate inputRate = RHIVertexInputRate::Vertex;
};

struct RHIVertexLayoutDesc {
    const RHIVertexAttributeDesc* attributes = nullptr;
    std::size_t attributeCount = 0;

    const RHIVertexBindingDesc* bindings = nullptr;
    std::size_t bindingCount = 0;
};

struct RHIGraphicsPipelineDesc {
    RHIShader* vertexShader = nullptr;
    RHIShader* fragmentShader = nullptr;

    RHIVertexLayoutDesc vertexLayout = {};

    RHIFormat colorFormat = RHIFormat::BGRA8;
    RHIFormat depthStencilFormat = RHIFormat::Depth24Stencil8;

    RHIPrimitiveTopology primitiveTopology = RHIPrimitiveTopology::TriangleList;

    RHIRasterizerStateDesc rasterizerState = {};
    RHIDepthStencilStateDesc depthStencilState = {};
    RHIBlendStateDesc blendState = {};
};

struct RHIComputePipelineDesc {
    RHIShader* computeShader = nullptr;
};

class RHIPipeline {
public:
    RHIPipeline() = default;
    virtual ~RHIPipeline() = default;

    RHIPipeline(const RHIPipeline&) = delete;
    RHIPipeline& operator=(const RHIPipeline&) = delete;

    RHIPipeline(RHIPipeline&&) = delete;
    RHIPipeline& operator=(RHIPipeline&&) = delete;

    virtual bool InitializeGraphics(const RHIGraphicsPipelineDesc& desc) = 0;
    virtual bool InitializeCompute(const RHIComputePipelineDesc& desc) = 0;
    virtual void Shutdown() = 0;

    [[nodiscard]] virtual RHIPipelineType GetType() const = 0;
    [[nodiscard]] virtual bool IsInitialized() const = 0;
};

#endif //SHIBAENGINE_RHIPIPELINE_H