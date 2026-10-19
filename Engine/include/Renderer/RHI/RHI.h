#ifndef SHIBAENGINE_RHI_H
#define SHIBAENGINE_RHI_H

using RHIUInt8 = unsigned char;
using RHIUInt32 = unsigned int;
using RHIUInt64 = unsigned long long;
using RHIInt32 = int;

enum class RHIBackend {
    None = 0,
    Vulkan,
    DirectX12,
    Metal,
    OpenGL
};

enum class RHIFormat {
    None = 0,
    R8, RG8, RGB8, RGBA8, BGRA8,
    Depth24Stencil8,
    Depth32,
    Float2,
    Float3,
    Float4
};

enum class RHIShaderStage {
    None = 0,
    Vertex,
    Fragment,
    Compute
};

enum class RHIPipelineType {
    None = 0,
    Graphics,
    Compute
};

#endif //SHIBAENGINE_RHI_H