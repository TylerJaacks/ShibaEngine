
#ifndef SHIBAENGINE_METALBUFFER_H
#define SHIBAENGINE_METALBUFFER_H

#if defined(__APPLE__)

#include <cstddef>
#include <cstdint>

#include <Renderer/RHI/RHIBuffer.h>

namespace MTL {
    class Device;
    class Buffer;
}

class MetalBuffer final : public RHIBuffer {
public:
    MetalBuffer() = default;
    ~MetalBuffer() override;

    MetalBuffer(const MetalBuffer&) = delete;
    MetalBuffer& operator=(const MetalBuffer&) = delete;

    MetalBuffer(const MetalBuffer&&) = delete;
    MetalBuffer& operator=(const MetalBuffer&&) = delete;

    bool Initialize(MTL::Device* device, const RHIBufferDesc& desc, const void* initialData = nullptr);
    void Shutdown();

    void SetData(const void *data, std::size_t size, std::size_t offset) override;

    [[nodiscard]] const RHIBufferDesc& GetDesc() const override;
    [[nodiscard]] std::size_t GetSize() const override;
    [[nodiscard]] MTL::Buffer* GetNativeBuffer() const;

private:
    MTL::Buffer* m_buffer = nullptr;
    RHIBufferDesc m_desc{};
    bool m_IsInitialized = false;
};

#endif
#endif //SHIBAENGINE_METALBUFFER_H