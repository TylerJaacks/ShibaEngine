#if defined(__APPLE__)
#include "Renderer/Backend/Metal/MetalTexture.h"

#include <Metal/Metal.hpp>

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

    MTL::TextureType ToMetalTextureType(const RHITextureDesc &desc) {
        if (desc.type == RHITextureType::TextureCube) {
            return MTL::TextureTypeCube;
        }

        if (desc.type == RHITextureType::Texture3D) {
            return MTL::TextureType3D;
        }

        if (desc.arrayLayers > 1) {
            return MTL::TextureType2DArray;
        }

        return MTL::TextureType2D;
    }

    MTL::TextureUsage ToMetalTextureUsage(const RHITextureUsage usage) {
        MTL::TextureUsage metalUsage = MTL::TextureUsageShaderRead;

        if (usage == RHITextureUsage::None) {
            return MTL::TextureUsageUnknown;
        }

        if (HasUsage(usage, RHITextureUsage::Sampled)) {
            metalUsage = static_cast<MTL::TextureUsage>(metalUsage | MTL::TextureUsageShaderRead);
        }

        if (HasUsage(usage, RHITextureUsage::Storage)) {
            metalUsage = static_cast<MTL::TextureUsage>(metalUsage | MTL::TextureUsageShaderWrite);
        }

        if (HasUsage(usage, RHITextureUsage::RenderTarget) ||
            HasUsage(usage, RHITextureUsage::DepthStencil)) {
            metalUsage = static_cast<MTL::TextureUsage>(metalUsage | MTL::TextureUsageRenderTarget);
        }

        return metalUsage;
    }

    MTL::StorageMode ToMetalStorageMode(const RHITextureDesc &desc) {
        if (desc.cpuWritable) {
            return MTL::StorageModeShared;
        }

        return MTL::StorageModePrivate;
    }

    NS::String *MakeNSString(const char *text) {
        if (text == nullptr) {
            return NS::String::string("", NS::UTF8StringEncoding);
        }

        return NS::String::string(text, NS::UTF8StringEncoding);
    }
}

MetalTexture::~MetalTexture() {
    Shutdown();
}

bool MetalTexture::Initialize(const RHITextureDesc &desc) {
    (void) desc;

    return false;
}

bool MetalTexture::Initialize(MTL::Device *device, const RHITextureDesc &desc) {
    if (m_IsInitialized) {
        return true;
    }

    if (device == nullptr ||
        desc.width == 0 ||
        desc.height == 0 ||
        desc.mipLevels == 0 ||
        desc.arrayLayers == 0 ||
        ToMetalPixelFormat(desc.format) == MTL::PixelFormatInvalid) {
        return false;
    }

    auto *textureDesc = MTL::TextureDescriptor::alloc()->init();

    textureDesc->setTextureType(ToMetalTextureType(desc));
    textureDesc->setPixelFormat(ToMetalPixelFormat(desc.format));
    textureDesc->setWidth(desc.width);
    textureDesc->setHeight(desc.height);
    textureDesc->setDepth(desc.depth);
    textureDesc->setMipmapLevelCount(desc.mipLevels);
    textureDesc->setArrayLength(desc.arrayLayers);
    textureDesc->setUsage(ToMetalTextureUsage(desc.usage));
    textureDesc->setStorageMode(ToMetalStorageMode(desc));

    m_Texture = device->newTexture(textureDesc);

    textureDesc->release();

    if (m_Texture == nullptr) {
        return false;
    }

    if (desc.debugName != nullptr) {
        m_Texture->setLabel(MakeNSString(desc.debugName));
    }

    m_Device = device;
    m_Device->retain();

    m_Desc = desc;
    m_OwnsTexture = true;
    m_IsInitialized = true;

    return true;
}

bool MetalTexture::InitializeWrapped(MTL::Texture *texture, const RHITextureDesc &desc, bool retainTexture) {
    if (m_IsInitialized) {
        return true;
    }

    if (texture == nullptr) {
        return false;
    }

    m_Texture = texture;

    if (retainTexture) {
        m_Texture->retain();
        m_OwnsTexture = true;
    } else {
        m_OwnsTexture = false;
    }

    m_Desc = desc;
    m_Device = nullptr;
    m_IsInitialized = true;

    return true;
}

void MetalTexture::Shutdown() {
    if (m_Texture != nullptr && m_OwnsTexture) {
        m_Texture->release();
        m_Texture = nullptr;
    } else {
        m_Texture = nullptr;
    }

    if (m_Device != nullptr) {
        m_Device->release();
        m_Device = nullptr;
    }

    m_Desc = {};
    m_OwnsTexture = true;
    m_IsInitialized = false;
}

void MetalTexture::SetData(const RHITextureSubresourceData &data) {
    if (!m_IsInitialized ||
        m_Texture == nullptr ||
        data.data == nullptr ||
        data.width == 0 ||
        data.height == 0 ||
        data.bytesPerRow == 0) {
        return;
    }

    const MTL::Region region = MTL::Region::Make2D(
        0,
        0,
        static_cast<NS::UInteger>(data.width),
        static_cast<NS::UInteger>(data.height)
    );

    if (m_Desc.type == RHITextureType::Texture3D) {
        return;
    }

    if (m_Desc.arrayLayers > 1 || m_Desc.type == RHITextureType::Texture3D) {
        m_Texture->replaceRegion(
            region,
            data.mipLevel,
            data.arrayLayer,
            data.data,
            data.bytesPerRow,
            data.bytesPerImage
        );

        return;
    }

    m_Texture->replaceRegion(
        region,
        data.mipLevel,
        data.data,
        data.bytesPerRow
    );
}

const RHITextureDesc &MetalTexture::GetDesc() const {
    return m_Desc;
}

RHIUInt32 MetalTexture::GetWidth() const {
    return m_Desc.width;
}

RHIUInt32 MetalTexture::GetHeight() const {
    return m_Desc.height;
}

RHIFormat MetalTexture::GetFormat() const {
    return m_Desc.format;
}

bool MetalTexture::IsInitialized() const {
    return m_IsInitialized;
}

MTL::Texture *MetalTexture::GetNativeTexture() const {
    return m_Texture;
}

bool MetalTexture::OwnsTexture() const {
    return m_OwnsTexture;
}
#endif
