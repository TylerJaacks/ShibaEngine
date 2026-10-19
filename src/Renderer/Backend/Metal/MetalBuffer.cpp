#if defined(__APPLE__)
#include <Metal/Metal.hpp>
#include <Metal/MTLBuffer.hpp>
#include <Metal/MTLDevice.hpp>

#include "Renderer/Backend/Metal/MetalBuffer.h"

namespace {
    MTL::ResourceOptions ToMetalResourceOptions(RHIBufferMemoryUsage memory_usage) {
        switch (memory_usage) {
            case RHIBufferMemoryUsage::GPU:
                return MTL::ResourceStorageModePrivate;
            case RHIBufferMemoryUsage::CPUToGPU:
                return MTL::ResourceStorageModeShared;
            case RHIBufferMemoryUsage::GPUToCPU:
                return MTL::ResourceStorageModeShared;
        }

        return MTL::ResourceStorageModeShared;
    }

    bool IsCPUVisible(const RHIBufferMemoryUsage memory_usage) {
        switch (memory_usage) {
            case RHIBufferMemoryUsage::GPU:
                return false;
            case RHIBufferMemoryUsage::GPUToCPU:
            case RHIBufferMemoryUsage::CPUToGPU:
                return true;
        }

        return false;
    }
}

MetalBuffer::~MetalBuffer() {
    Shutdown();
}

bool MetalBuffer::Initialize(MTL::Device *device, const RHIBufferDesc &desc, const void *initialData) {
    if (m_IsInitialized) {
        return true;
    }

    if (device == nullptr || desc.size == 0) {
        return false;
    }

    const MTL::ResourceOptions resource_options = ToMetalResourceOptions(desc.memoryUsage);

    m_buffer = device->newBuffer(desc.size, resource_options);

    if (m_buffer == nullptr) {
        return false;
    }

    m_desc = desc;
    m_IsInitialized = true;

    if (initialData != nullptr && IsCPUVisible(m_desc.memoryUsage)) {
        SetData(initialData, desc.size, 0);
    }

    return true;
}

void MetalBuffer::Shutdown() {
    if (!m_IsInitialized) {
        return;
    }

    if (m_buffer != nullptr) {
        m_buffer->release();
        m_buffer = nullptr;
    }

    m_desc = {};
    m_IsInitialized = false;
}

void MetalBuffer::SetData(const void *data, const std::size_t size, std::size_t offset) {
    if (!m_IsInitialized || m_buffer == nullptr) {
        return;
    }

    if (!IsCPUVisible(m_desc.memoryUsage)) {
        return;
    }

    if (offset + size > m_desc.size) {
        return;
    }

    auto* destination = static_cast<std::uint8_t*>(m_buffer->contents()) + offset;

    std::memcpy(destination, data, size);

    m_buffer->didModifyRange(NS::Range::Make(offset, size));
}

const RHIBufferDesc & MetalBuffer::GetDesc() const {
    return m_desc;
}

std::size_t MetalBuffer::GetSize() const {
    return m_desc.size;
}

MTL::Buffer * MetalBuffer::GetNativeBuffer() const {
    return m_buffer;
}
#endif