#include <iterator>
#include <Renderer/Primitives/Primitive.h>

namespace {
    constexpr PrimitiveVertex CUBE_VERTICES[] = {
        {.position = {-0.5f, -0.5f,  0.5f}, .color = {1, 0, 0}},
        {.position = { 0.5f, -0.5f,  0.5f}, .color = {0, 1, 0}},
        {.position = { 0.5f,  0.5f,  0.5f}, .color = {0, 0, 1}},
        {.position = {-0.5f,  0.5f,  0.5f}, .color = {1, 1, 0}},
        {.position = {-0.5f, -0.5f, -0.5f}, .color = {1, 0, 1}},
        {.position = { 0.5f, -0.5f, -0.5f}, .color = {0, 1, 1}},
        {.position = { 0.5f,  0.5f, -0.5f}, .color = {1, 1, 1}},
        {.position = {-0.5f,  0.5f, -0.5f}, .color = {0, 0, 0}},
    };

    constexpr std::uint32_t CUBE_INDICES[] = {
        0, 1, 2,  0, 2, 3,  // +Z
        5, 4, 7,  5, 7, 6,  // -Z
        1, 5, 6,  1, 6, 2,  // +X
        4, 0, 3,  4, 3, 7,  // -X
        3, 2, 6,  3, 6, 7,  // +Y
        4, 5, 1,  4, 1, 0,  // -Y
    };
}

PrimitiveData Primitives::Cube() {
    return {
        .vertices = CUBE_VERTICES,
        .vertexCount = std::size(CUBE_VERTICES),
        .indices = CUBE_INDICES,
        .indexCount = std::size(CUBE_INDICES)
    };
}