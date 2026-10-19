#ifndef SHIBAENGINE_RENDERER_H
#define SHIBAENGINE_RENDERER_H

#include "Renderer/Camera.h"
#include "Renderer/RHI/RHIDevice.h"
#include "Renderer/RHI/RHIContext.h"
#include "Renderer/RHI/RHISwapchain.h"

#include <memory>

#include "Platform/Window.h"

struct RendererConfig {
    RHIBackend backend = RHIBackend::None;
    bool enableValidation = true;
};

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Initialize(Window& window, const RendererConfig& config);
    void BeginFrame() const;

    static void RenderScene(const Camera& camera);
    void EndFrame() const;
    void Shutdown();

    [[nodiscard]] RHIDevice& GetDevice() const;
    [[nodiscard]] RHIContext& GetContext() const;
    [[nodiscard]] RHISwapchain& GetSwapchain() const;

private:
    RHIDevice* m_device = nullptr;
    std::unique_ptr<RHIContext> m_context;
    std::unique_ptr<RHISwapchain> m_swapchain;
};

#endif //SHIBAENGINE_RENDERER_H