#ifndef SHIBAENGINE_RHISHADER_H
#define SHIBAENGINE_RHISHADER_H

#include <cstddef>
#include <string>

#include "Renderer/RHI/RHI.h"

enum class RHIShaderSourceType : RHIUInt8 {
    Source,
    File
};

struct RHIShaderDesc {
    RHIShaderStage stage = RHIShaderStage::None;
    RHIShaderSourceType type = RHIShaderSourceType::Source;

    const char* source = nullptr;
    std::size_t sourceSize = 0;

    const char* filePath = nullptr;
    const char* entryPoint = "main0";

    const char* debugName = nullptr;
};

class RHIShader {
public:
    RHIShader() = default;
    virtual ~RHIShader() = default;

    RHIShader(const RHIShader&) = delete;
    RHIShader& operator=(const RHIShader&) = delete;

    RHIShader(RHIShader&&) = delete;
    RHIShader& operator=(RHIShader&&) = delete;

    virtual bool Initialize(const RHIShaderDesc& desc) = 0;
    virtual void Shutdown() = 0;

    [[nodiscard]] virtual RHIShaderStage GetStage() const = 0;
    [[nodiscard]] virtual const char* GetEntryPoint() const = 0;
    [[nodiscard]] virtual const char* GetDebugName() const = 0;
    [[nodiscard]] virtual bool IsInitialized() const = 0;
};

#endif //SHIBAENGINE_RHISHADER_H