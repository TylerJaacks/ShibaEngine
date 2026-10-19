#include <Renderer/Renderer.h>

#if defined(__APPLE__)
#include <Renderer/Backend/Metal/MetalDevice.h>
#include <Renderer/Backend/Metal/MetalSwapchain.h>
#endif

#include <Platform/Window.h>
#include <Renderer/Primitives/Primitive.h>
#include <Renderer/RHI/RHICommandList.h>

namespace {
    constexpr const char* CUBE_SHADER_SOURCE = R"(
#include <metal_stdlib>
using namespace metal;

struct VertexIn {
    float3 position [[attribute(0)]];
    float3 color    [[attribute(1)]];
};

struct VertexOut {
    float4 position [[position]];
    float3 color;
};

struct Uniforms { float4x4 mvp; };

vertex VertexOut vertexMain(VertexIn in [[stage_in]],
                            constant Uniforms& u [[buffer(1)]]) {
    VertexOut out;
    out.position = u.mvp * float4(in.position, 1.0);
    out.color = in.color;
    return out;
}

fragment float4 fragmentMain(VertexOut in [[stage_in]]) {
    return float4(in.color, 1.0);
}
)";
}

Renderer::Renderer()
    : m_device(nullptr),
      m_context(nullptr),
      m_swapchain(nullptr) {
}

Renderer::~Renderer() {
    Shutdown();
}

void Renderer::Initialize(const Window& window, const RendererConfig& config) {
    RHIContextDesc context_desc{
        .backend = config.backend,
        .enableValidation = config.enableValidation
    };

    m_context = RHIContext::Create(context_desc);

    if (m_context == nullptr) {
        return;
    }

    if (!m_context->Initialize(context_desc)) {
        m_context.reset();
        return;
    }

    m_device = m_context->GetDevice();

    if (m_device == nullptr) {
        m_context->Shutdown();
        m_context.reset();
        return;
    }

    const NativeWindowHandle native_window_handle = window.GetNativeHandle();

    RHISwapchainDesc swapchain_desc{
        .nativeWindow = native_window_handle.layer,
        .width = window.GetWidth(),
        .height = window.GetHeight(),
        .colorFormat = RHIFormat::BGRA8,
        .imageCount = 3,
        .presentMode = window.IsVSync() ? RHIPresentMode::VSync : RHIPresentMode::Immediate,
        .enableHDR = false
    };

    m_swapchain = m_device->CreateSwapchain(swapchain_desc);

    if (m_swapchain == nullptr) {
        m_context->Shutdown();
        m_context.reset();
        m_device = nullptr;
    }

    CreateBuiltInMeshes();
    CreatePipeline();
}

void Renderer::BeginFrame() const {
    if (m_context == nullptr || m_swapchain == nullptr) {
        return;
    }

    if (!m_context->BeginFrame()) {
        return;
    }

    if (!m_swapchain->AcquireNextImage()) {
        m_context->EndFrame();
        return;
    }

    RHICommandList* command_list = m_context->GetCommandList();

    if (command_list == nullptr) {
        m_context->EndFrame();
        return;
    }

    const RHIRenderPassDesc render_pass_desc{
        .colorTarget = m_swapchain->GetCurrentBackBuffer(),
        .depthStencilTarget = nullptr,
        .clearColor = {
            .r = 0.1f,
            .g = 0.1f,
            .b = 0.15f,
            .a = 1.0f
        },
        .clearDepth = 1.0f,
        .clearStencil = 0,
        .clearColorTarget = true,
        .clearDepthStencilTarget = true,
        .clearStencilTarget = true
    };

    command_list->BeginRenderPass(render_pass_desc);
}

void Renderer::RenderScene(const Camera& camera) {
    (void)camera;
}

void Renderer::EndFrame() const {
    if (m_context == nullptr || m_swapchain == nullptr) {
        return;
    }

    RHICommandList* command_list = m_context->GetCommandList();

    if (command_list != nullptr) {
        command_list->EndRenderPass();
    }

    m_swapchain->Present();
    m_context->EndFrame();
}

void Renderer::Shutdown() {
    m_meshes.clear();
    m_pipeline.reset();
    m_vertexShader.reset();
    m_fragmentShader.reset();

    if (m_swapchain != nullptr) {
        m_swapchain->Shutdown();
        m_swapchain.reset();
    }

    if (m_context != nullptr) {
        m_context->Shutdown();
        m_context.reset();
    }

    m_device = nullptr;
}

RHIDevice& Renderer::GetDevice() const {
    return *m_device;
}

RHIContext& Renderer::GetContext() const {
    return *m_context;
}

RHISwapchain& Renderer::GetSwapchain() const {
    return *m_swapchain;
}

void Renderer::CreateBuiltInMeshes() {
    UploadMesh(CubeMesh, Primitives::Cube());
}

bool Renderer::CreatePipeline() {
    m_vertexShader = m_device->CreateShader({
        .stage = RHIShaderStage::Vertex,
        .type = RHIShaderSourceType::Source,
        .source = CUBE_SHADER_SOURCE,
        .entryPoint = "vertexMain",
        .debugName = "CubeVS"
    });

    m_fragmentShader = m_device->CreateShader({
        .stage = RHIShaderStage::Fragment,
        .type = RHIShaderSourceType::Source,
        .source = CUBE_SHADER_SOURCE,
        .entryPoint = "fragmentMain",
        .debugName = "CubeFS"
    });
    if (m_vertexShader == nullptr || m_fragmentShader == nullptr) {
        return false;
    }

    static const RHIVertexAttributeDesc attributes[] = {
        {.location = 0, .binding = 0, .format = RHIFormat::Float3, .offset = offsetof(PrimitiveVertex, position)},
        {.location = 1, .binding = 0, .format = RHIFormat::Float3, .offset = offsetof(PrimitiveVertex, color)},
    };

    static const RHIVertexBindingDesc bindings[] = {
        {.binding = 0, .stride = sizeof(PrimitiveVertex)},
    };

    RHIGraphicsPipelineDesc desc;
    desc.vertexShader = m_vertexShader.get();
    desc.fragmentShader = m_fragmentShader.get();
    desc.vertexLayout = {attributes, std::size(attributes), bindings, std::size(bindings)};
    desc.depthStencilFormat = RHIFormat::None;
    desc.depthStencilState.depthTestEnabled = false;
    desc.depthStencilState.depthWriteEnabled = false;

    m_pipeline = m_device->CreateGraphicsPipeline(desc);

    return m_pipeline != nullptr;
}

bool Renderer::UploadMesh(const MeshHandle handle, const PrimitiveData &data) {
    if (m_device == nullptr) {
        return false;
    }

    GPUMesh mesh;

    mesh.vertexBuffer = m_device->CreateBuffer({
        .size = data.VertexBytes(),
        .usage = RHIBufferUsage::Vertex,
    });

    mesh.indexBuffer = m_device->CreateBuffer({
        .size = data.IndexBytes(),
        .usage = RHIBufferUsage::Index,
    });

    if (mesh.vertexBuffer == nullptr || mesh.indexBuffer == nullptr) {
        return false;
    }

    mesh.vertexBuffer->SetData(data.vertices, data.VertexBytes());
    mesh.indexBuffer->SetData(data.indices, data.IndexBytes());

    mesh.indexCount = static_cast<std::uint32_t>(data.indexCount);

    m_meshes[handle] = std::move(mesh);

    return true;
}