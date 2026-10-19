#ifndef SHIBAENGINE_RHIBUFFER_H
#define SHIBAENGINE_RHIBUFFER_H

#include <cstddef>
#include <cstdint>

enum class RHIBufferUsage : std::uint8_t {
    Vertex,
    Index,
    Uniform,
    Storage,
    Staging
};

enum class RHIBufferMemoryUsage : std::uint8_t {
    GPU,
    CPUToGPU,
    GPUToCPU,
};

struct RHIBufferDesc {
    std::size_t size = 0;
    RHIBufferUsage usage = RHIBufferUsage::Vertex;
    RHIBufferMemoryUsage memoryUsage = RHIBufferMemoryUsage::CPUToGPU;
};

class RHIBuffer {
public:
    RHIBuffer() = default;
    virtual ~RHIBuffer() = default;

    RHIBuffer(const RHIBuffer&) = delete;
    RHIBuffer& operator=(const RHIBuffer&) = delete;

    RHIBuffer(const RHIBuffer&&) = delete;
    RHIBuffer& operator=(const RHIBuffer&&) = delete;

    virtual void SetData(const void* data, std::size_t size, std::size_t offset = 0) = 0;

    [[nodiscard]] virtual const RHIBufferDesc& GetDesc() const = 0;
    [[nodiscard]] virtual std::size_t GetSize() const = 0;
};

#endif //SHIBAENGINE_RHIBUFFER_H