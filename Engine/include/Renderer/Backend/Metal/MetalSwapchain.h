#ifndef SHIBAENGINE_METALSWAPCHAIN_H
#define SHIBAENGINE_METALSWAPCHAIN_H
#if defined(__APPLE__)

#include <Renderer/Backend/Metal/MetalTexture.h>
#include <Renderer/RHI/RHISwapchain.h>

#include <memory>

namespace CA {
    class MetalLayer;
    class MetalDrawable;
}

namespace MTL {
    class CommandBuffer;
    class Device;
    class Texture;
}

class MetalDevice;

class MetalSwapchain final : public RHISwapchain {
public:
    MetalSwapchain() = default;
    ~MetalSwapchain() override;

    MetalSwapchain(const MetalSwapchain&) = delete;
    MetalSwapchain& operator=(const MetalSwapchain&) = delete;

    MetalSwapchain(MetalSwapchain&&) = delete;
    MetalSwapchain& operator=(MetalSwapchain&&) = delete;

    bool Initialize(RHIDevice* device, const RHISwapchainDesc& desc) override;
    bool Initialize(MetalDevice* device, const RHISwapchainDesc& desc);

    void Shutdown() override;

    bool Resize(RHIUInt32 width, RHIUInt32 height) override;

    bool AcquireNextImage() override;
    void Present() override;

    void Present(MTL::CommandBuffer* commandBuffer);

    [[nodiscard]] RHITexture* GetCurrentBackBuffer() const override;

    [[nodiscard]] RHIUInt32 GetWidth() const override;
    [[nodiscard]] RHIUInt32 GetHeight() const override;
    [[nodiscard]] RHIFormat GetColorFormat() const override;
    [[nodiscard]] bool IsInitialized() const override;

    [[nodiscard]] CA::MetalLayer* GetNativeLayer() const;
    [[nodiscard]] CA::MetalDrawable* GetCurrentDrawable() const;
    [[nodiscard]] MTL::Texture* GetCurrentDrawableTexture() const;

private:
    bool ConfigureLayer(const RHISwapchainDesc& desc);

    MetalDevice* m_Device = nullptr;

    CA::MetalLayer* m_Layer = nullptr;
    CA::MetalDrawable* m_CurrentDrawable = nullptr;
    std::unique_ptr<MetalTexture> m_BackBuffer;

    RHIUInt32 m_Width = 0;
    RHIUInt32 m_Height = 0;
    RHIFormat m_ColorFormat = RHIFormat::BGRA8;
    RHIPresentMode m_PresentMode = RHIPresentMode::VSync;

    bool m_OwnsLayer = false;
    bool m_IsInitialized = false;
};

#endif
#endif //SHIBAENGINE_METALSWAPCHAIN_H