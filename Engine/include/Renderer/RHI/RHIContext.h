#ifndef SHIBAENGINE_RHICONTEXT_H
#define SHIBAENGINE_RHICONTEXT_H

#include <Renderer/RHI/RHI.h>
#include <Renderer/RHI/RHIDevice.h>

#include <memory>

class RHICommandList;

struct RHIContextDesc {
    RHIBackend backend = RHIBackend::None;
    bool enableValidation = false;
};

class RHIContext {
public:
    RHIContext() = default;
    virtual ~RHIContext() = default;

    RHIContext(const RHIContext&) = delete;
    RHIContext& operator=(const RHIContext&) = delete;

    RHIContext(RHIContext&&) = delete;
    RHIContext& operator=(RHIContext&&) = delete;

    virtual bool Initialize(const RHIContextDesc& desc) = 0;
    virtual void Shutdown() = 0;

    virtual bool BeginFrame() = 0;
    virtual void EndFrame() = 0;

    [[nodiscard]] virtual RHIBackend GetBackend() const = 0;
    [[nodiscard]] virtual RHIDevice* GetDevice() const = 0;
    [[nodiscard]] virtual RHICommandList* GetCommandList() const = 0;
    [[nodiscard]] virtual bool IsInitialized() const = 0;
    [[nodiscard]] virtual bool IsFrameActive() const = 0;

    static std::unique_ptr<RHIContext> Create(const RHIContextDesc& desc);

protected:
    bool m_isInitialized = false;
    bool m_isFrameActive = false;
};

#endif //SHIBAENGINE_RHICONTEXT_H