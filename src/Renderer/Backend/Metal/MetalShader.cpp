#if defined(__APPLE__)
#include <fstream>
#include <sstream>
#include <string>

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

#include "Renderer/Backend/Metal/MetalShader.h"

namespace {
    std::string ReadTextFile(const char* filePath) {
        if (filePath == nullptr) {
            return {};
        }

        std::ifstream file(filePath, std::ios::in | std::ios::binary);

        if (!file.is_open()) {
            return {};
        }

        std::ostringstream stream;
        stream << file.rdbuf();

        return stream.str();
    }

    NS::String* MakeNSString(const char* text) {
        if (text == nullptr) {
            return NS::String::string("", NS::UTF8StringEncoding);
        }

        return NS::String::string(text, NS::UTF8StringEncoding);
    }

    NS::String* MakeNSString(const std::string& text) {
        return NS::String::string(text.c_str(), NS::UTF8StringEncoding);
    }
}

MetalShader::~MetalShader() {
    Shutdown();
}

bool MetalShader::Initialize(const RHIShaderDesc &desc) {
    (void) desc;

    return false;
}

bool MetalShader::Initialize(MTL::Device *device, const RHIShaderDesc &desc) {
    if (m_IsInitialized) {
        return true;
    }

    if (device == nullptr || desc.stage == RHIShaderStage::None) {
        return false;
    }

    m_Stage = desc.stage;
    m_EntryPoint = desc.entryPoint != nullptr ? desc.entryPoint : "main0";
    m_DebugName = desc.debugName != nullptr ? desc.debugName : "MetalShader";

    bool initialized = false;

    switch (desc.type) {
        case RHIShaderSourceType::Source:
            initialized = InitializeFromSource(device, desc);
            break;
        case RHIShaderSourceType::File:
            initialized = InitializeFromFile(device, desc);
            break;

        default:
            initialized = false;
            break;
    }

    if (!initialized) {
        Shutdown();

        return false;
    }

    m_Device = device;
    m_Device->retain();

    m_IsInitialized = true;

    return true;
}

void MetalShader::Shutdown() {
    if (m_Function != nullptr) {
        m_Function->release();
        m_Function = nullptr;
    }

    if (m_Library != nullptr) {
        m_Library->release();
        m_Library = nullptr;
    }

    if (m_Device != nullptr) {
        m_Device->release();
        m_Device = nullptr;
    }

    m_Stage = RHIShaderStage::None;
    m_EntryPoint = "main0";
    m_DebugName = "MetalShader";
    m_IsInitialized = false;
}

RHIShaderStage MetalShader::GetStage() const {
    return m_Stage;
}

const char* MetalShader::GetEntryPoint() const {
    return m_EntryPoint.c_str();
}

const char* MetalShader::GetDebugName() const {
    return m_DebugName.c_str();
}

bool MetalShader::IsInitialized() const {
    return m_IsInitialized;
}

MTL::Library * MetalShader::GetNativeLibrary() const {
    return m_Library;
}

MTL::Function * MetalShader::GetNativeFunction() const {
    return m_Function;
}

bool MetalShader::InitializeFromSource(MTL::Device *device, const RHIShaderDesc &desc) {
    if (device == nullptr || desc.source == nullptr) {
        return false;
    }

    std::string source;

    if (desc.sourceSize > 0) {
        source.assign(desc.source, desc.sourceSize);
    } else {
        source = desc.source;
    }

    if (source.empty()) {
        return false;
    }

    NS::Error* error = nullptr;

    m_Library = device->newLibrary(
        MakeNSString(source),
        nullptr,
        &error);

    if (m_Library == nullptr) {
        return false;
    }

    m_Function = m_Library->newFunction(
        MakeNSString(m_EntryPoint),
        nullptr,
        &error);

    if (m_Function == nullptr) {
        return false;
    }

    return true;
}

bool MetalShader::InitializeFromFile(MTL::Device *device, const RHIShaderDesc &desc) {
    if (device == nullptr || desc.filePath == nullptr) {
        return false;
    }

    const std::string source = ReadTextFile(desc.filePath);

    if (source.empty()) {
        return false;
    }

    RHIShaderDesc sourceDesc = desc;

    sourceDesc.type = RHIShaderSourceType::Source;
    sourceDesc.source = source.c_str();
    sourceDesc.sourceSize = source.size();

    return InitializeFromSource(device, sourceDesc);
}
#endif