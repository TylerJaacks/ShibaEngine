#ifndef SHIBAENGINE_VECTOR_H
#define SHIBAENGINE_VECTOR_H

template <typename T>
struct Vector3 {
    T x{};
    T y{};
    T z{};

    constexpr Vector3() = default;
    constexpr Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    constexpr Vector3 operator+(const Vector3& other) const { return {x + other.x, y + other.y, z + other.z}; }
    constexpr Vector3 operator-(const Vector3& other) const { return {x - other.x, y - other.y, z - other.z}; }
    constexpr Vector3 operator*(T scalar) const { return {x * scalar, y * scalar, z * scalar}; }
    constexpr Vector3 operator/(T scalar) const { return {x / scalar, y / scalar, z / scalar}; }

    constexpr Vector3& operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
    constexpr Vector3& operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
    constexpr Vector3& operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
    constexpr Vector3& operator/=(T scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
};

template <typename T>
constexpr Vector3<T> operator*(T scalar, const Vector3<T>& vector) { return vector * scalar; }

class Vector3i {
public:
    Vector3i(int x, int y, int z);

    explicit Vector3i(const Vector3<int>& vector);

    [[nodiscard]] int GetX() const;
    [[nodiscard]] int GetY() const;
    [[nodiscard]] int GetZ() const;

    [[nodiscard]] const Vector3<int> &GetVector() const;
private:
    Vector3<int> m_vector;
};

#endif // SHIBAENGINE_VECTOR_H