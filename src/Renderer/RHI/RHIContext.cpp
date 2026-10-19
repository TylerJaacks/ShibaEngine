#include "Renderer/RHI/RHIContext.h"

#if defined(__APPLE__)
#include "Renderer/Backend/Metal/MetalContext.h"
#endif

std::unique_ptr<RHIContext> RHIContext::Create(const RHIContextDesc& desc) {
    switch (desc.backend) {
#if defined(__APPLE__)
        case RHIBackend::Metal:
            return std::make_unique<MetalContext>();
#endif

        default:
            return nullptr;
    }
}