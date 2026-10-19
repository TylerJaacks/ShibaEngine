#ifndef SHIBAENGINE_RHITEXTURE_H
#define SHIBAENGINE_RHITEXTURE_H
#include <cstddef>

#include <Renderer/RHI/RHI.h>

enum class RHITextureType : RHIUInt8 {
    Texture2D,
    TextureCube,
    Texture3D
};

enum class RHITextureUsage : RHIUInt32 {
    None = 0,
    Sampled = 1 << 0,
    Storage = 1 << 1,
    RenderTarget = 1 << 2,
    DepthStencil = 1 << 3,
    TransferSource = 1 << 4,
    TransferDestination = 1 << 5
};

constexpr RHITextureUsage operator|(const RHITextureUsage lhs, const RHITextureUsage rhs) {
    return static_cast<RHITextureUsage>(
        static_cast<RHIUInt32>(lhs) | static_cast<RHIUInt32>(rhs)
    );
}

constexpr RHITextureUsage operator&(const RHITextureUsage lhs, const RHITextureUsage rhs) {
    return static_cast<RHITextureUsage>(
        static_cast<RHIUInt32>(lhs) & static_cast<RHIUInt32>(rhs)
    );
}

constexpr bool HasUsage(const RHITextureUsage usage, const RHITextureUsage flag) {
    return static_cast<RHIUInt32>(usage & flag) != 0;
}

struct RHITextureDesc {
    RHITextureType type = RHITextureType::Texture2D;

    RHIUInt32 width = 1;
    RHIUInt32 height = 1;
    RHIUInt32 depth = 1;

    RHIUInt32 mipLevels = 1;
    RHIUInt32 arrayLayers = 1;

    RHIFormat format = RHIFormat::RGBA8;
    RHITextureUsage usage = RHITextureUsage::Sampled;

    bool cpuWritable = false;
    const char* debugName = nullptr;
};

struct RHITextureSubresourceData {
    const void* data = nullptr;
    std::size_t size = 0;

    RHIUInt32 width = 0;
    RHIUInt32 height = 0;

    std::size_t bytesPerRow = 0;
    std::size_t bytesPerImage = 0;

    RHIUInt32 mipLevel = 0;
    RHIUInt32 arrayLayer = 0;
};

class RHITexture {
public:
    RHITexture() = default;
    virtual ~RHITexture() = default;

    RHITexture(const RHITexture&) = delete;
    RHITexture& operator=(const RHITexture&) = delete;

    RHITexture(RHITexture&&) = delete;
    RHITexture& operator=(RHITexture&&) = delete;

    virtual bool Initialize(const RHITextureDesc& desc) = 0;
    virtual void Shutdown() = 0;

    virtual void SetData(const RHITextureSubresourceData& data) = 0;

    [[nodiscard]] virtual const RHITextureDesc& GetDesc() const = 0;
    [[nodiscard]] virtual RHIUInt32 GetWidth() const = 0;
    [[nodiscard]] virtual RHIUInt32 GetHeight() const = 0;
    [[nodiscard]] virtual RHIFormat GetFormat() const = 0;
    [[nodiscard]] virtual bool IsInitialized() const = 0;
};
#endif //SHIBAENGINE_RHITEXTURE_H