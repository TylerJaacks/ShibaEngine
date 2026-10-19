#include <Core/Math/Vector.h>

Vector3i::Vector3i(const int x, const int y, const int z) : m_vector(x, y, z) {
}

Vector3i::Vector3i(const Vector3<int> &vector) : m_vector(vector) {
}

int Vector3i::GetX() const {
    return m_vector.x;
}

int Vector3i::GetY() const {
    return m_vector.y;
}

int Vector3i::GetZ() const {
    return m_vector.z;
}

const Vector3<int> & Vector3i::GetVector() const {
    return m_vector;
}
