#ifndef SHIBAENGINE_METALSHADER_H
#define SHIBAENGINE_METALSHADER_H
#if defined(__APPLE__)

#include <string>

#include "Renderer/RHI/RHIShader.h"

namespace MTL {
    class Device;
    class Library;
    class Function;
}

class MetalShader final : public RHIShader {
public:
    MetalShader() = default;
    ~MetalShader() override;

    MetalShader(const MetalShader&) = delete;
    MetalShader& operator=(const MetalShader&) = delete;

    MetalShader(MetalShader&&) = delete;
    MetalShader& operator=(MetalShader&&) = delete;

    bool Initialize(const RHIShaderDesc& desc) override;
    bool Initialize(MTL::Device* device, const RHIShaderDesc& desc);

    void Shutdown() override;

    [[nodiscard]] RHIShaderStage GetStage() const override;
    [[nodiscard]] const char* GetEntryPoint() const override;
    [[nodiscard]] const char* GetDebugName() const override;
    [[nodiscard]] bool IsInitialized() const override;

    [[nodiscard]] MTL::Library* GetNativeLibrary() const;
    [[nodiscard]] MTL::Function* GetNativeFunction() const;

private:
    bool InitializeFromSource(MTL::Device* device, const RHIShaderDesc& desc);
    bool InitializeFromFile(MTL::Device* device, const RHIShaderDesc& desc);

    MTL::Device* m_Device = nullptr;
    MTL::Library* m_Library = nullptr;
    MTL::Function* m_Function = nullptr;

    RHIShaderStage m_Stage = RHIShaderStage::None;

    std::string m_EntryPoint = "main0";
    std::string m_DebugName = "MetalShader";

    bool m_IsInitialized = false;
};

#endif
#endif //SHIBAENGINE_METALSHADER_H
