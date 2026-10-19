#ifndef SHIBAENGINE_METALTEXTURE_H
#define SHIBAENGINE_METALTEXTURE_H
#if defined(__APPLE__)

#include "Renderer/RHI/RHITexture.h"

namespace MTL {
    class Device;
    class Texture;
}

class MetalTexture final : public RHITexture {
public:
    MetalTexture() = default;
    ~MetalTexture() override;

    MetalTexture(const MetalTexture&) = delete;
    MetalTexture& operator=(const MetalTexture&) = delete;

    MetalTexture(MetalTexture&&) = delete;
    MetalTexture& operator=(MetalTexture&&) = delete;

    bool Initialize(const RHITextureDesc& desc) override;
    bool Initialize(MTL::Device* device, const RHITextureDesc& desc);
    bool InitializeWrapped(MTL::Texture* texture, const RHITextureDesc& desc, bool retainTexture = true);

    void Shutdown() override;

    void SetData(const RHITextureSubresourceData& data) override;

    [[nodiscard]] const RHITextureDesc& GetDesc() const override;
    [[nodiscard]] RHIUInt32 GetWidth() const override;
    [[nodiscard]] RHIUInt32 GetHeight() const override;
    [[nodiscard]] RHIFormat GetFormat() const override;
    [[nodiscard]] bool IsInitialized() const override;

    [[nodiscard]] MTL::Texture* GetNativeTexture() const;
    [[nodiscard]] bool OwnsTexture() const;

private:
    MTL::Device* m_Device = nullptr;
    MTL::Texture* m_Texture = nullptr;

    RHITextureDesc m_Desc = {};

    bool m_OwnsTexture = true;
    bool m_IsInitialized = false;
};
#endif
#endif //SHIBAENGINE_METALTEXTURE_H