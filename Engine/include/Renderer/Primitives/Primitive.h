#ifndef SHIBAENGINE_PRIMITIVE_H
#define SHIBAENGINE_PRIMITIVE_H

#include <cstddef>
#include <cstdint>

#include <Core/Math/Vector.h>

struct PrimitiveVertex {
    Vector3<float> position;
    Vector3<float> color;
};

struct PrimitiveData {
    const PrimitiveVertex* vertices = nullptr;
    std::size_t vertexCount = 0;

    const std::uint32_t* indices = nullptr;
    std::size_t indexCount = 0;

    [[nodiscard]] std::size_t VertexBytes() const { return vertexCount * sizeof(PrimitiveVertex); }
    [[nodiscard]] std::size_t IndexBytes() const { return indexCount * sizeof(std::uint32_t); }

    static_assert(sizeof(PrimitiveVertex) == 24);
};

namespace Primitives {
    PrimitiveData Cube();
}

#endif //SHIBAENGINE_PRIMITIVE_H
