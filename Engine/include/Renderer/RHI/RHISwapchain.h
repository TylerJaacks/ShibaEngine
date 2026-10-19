#ifndef SHIBAENGINE_RHISWAPCHAIN_H
#define SHIBAENGINE_RHISWAPCHAIN_H

#include <Renderer/RHI/RHI.h>

class RHIDevice;
class RHITexture;

enum class RHIPresentMode {
    Immediate,
    VSync
};

struct RHISwapchainDesc {
    void* nativeWindow = nullptr;

    RHIUInt32 width = 0;
    RHIUInt32 height = 0;

    RHIFormat colorFormat = RHIFormat::BGRA8;
    RHIUInt32 imageCount = 3;

    RHIPresentMode presentMode = RHIPresentMode::VSync;
    bool enableHDR = false;
};

class RHISwapchain {
public:
    RHISwapchain() = default;
    virtual ~RHISwapchain() = default;

    RHISwapchain(const RHISwapchain&) = delete;
    RHISwapchain& operator=(const RHISwapchain&) = delete;

    RHISwapchain(RHISwapchain&&) = delete;
    RHISwapchain& operator=(RHISwapchain&&) = delete;

    virtual bool Initialize(RHIDevice* device, const RHISwapchainDesc& desc) = 0;
    virtual void Shutdown() = 0;

    virtual bool Resize(RHIUInt32 width, RHIUInt32 height) = 0;

    virtual bool AcquireNextImage() = 0;
    virtual void Present() = 0;

    [[nodiscard]] virtual RHITexture* GetCurrentBackBuffer() const = 0;

    [[nodiscard]] virtual RHIUInt32 GetWidth() const = 0;
    [[nodiscard]] virtual RHIUInt32 GetHeight() const = 0;

    [[nodiscard]] virtual RHIFormat GetColorFormat() const = 0;

    [[nodiscard]] virtual bool IsInitialized() const = 0;
};

#endif //SHIBAENGINE_RHISWAPCHAIN_H