#ifndef SHIBAENGINE_RENDERER_H
#define SHIBAENGINE_RENDERER_H

#include <Renderer/Camera.h>
#include <Renderer/RHI/RHIDevice.h>
#include <Renderer/RHI/RHIContext.h>
#include <Renderer/RHI/RHISwapchain.h>
#include <ECS/Components/MeshComponent.h>
#include <unordered_map>
#include <memory>
#include <Platform/Window.h>
#include <cstdint>
#include <Renderer/Primitives/Primitive.h>

struct GPUMesh {
    std::unique_ptr<RHIBuffer> vertexBuffer;
    std::unique_ptr<RHIBuffer> indexBuffer;
    std::uint32_t indexCount = 0;
};

struct RendererConfig {
    RHIBackend backend = RHIBackend::None;
    bool enableValidation = true;
};

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Initialize(const Window& window, const RendererConfig& config);
    void BeginFrame() const;

    static void RenderScene(const Camera& camera);
    void EndFrame() const;
    void Shutdown();

    [[nodiscard]] RHIDevice& GetDevice() const;
    [[nodiscard]] RHIContext& GetContext() const;
    [[nodiscard]] RHISwapchain& GetSwapchain() const;

    static constexpr MeshHandle CubeMesh = -1;

private:
    void CreateBuiltInMeshes();
    bool UploadMesh(MeshHandle handle, const PrimitiveData &data);
    bool CreatePipeline();

    RHIDevice* m_device = nullptr;

    std::unordered_map<MeshHandle, GPUMesh> m_meshes;

    std::unique_ptr<RHIShader> m_vertexShader;
    std::unique_ptr<RHIShader> m_fragmentShader;
    std::unique_ptr<RHIPipeline> m_pipeline;
    std::unique_ptr<RHIContext> m_context;
    std::unique_ptr<RHISwapchain> m_swapchain;
};

#endif //SHIBAENGINE_RENDERER_H