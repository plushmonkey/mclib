#ifndef VECTOR_H_
#define VECTOR_H_

#include <cstdint>
#include <cmath>
#include <limits>
#include <ostream>

template <typename VecType>
class Vector3 {
public:
    VecType x, y, z;

    Vector3() : x(0), y(0), z(0) { }
    Vector3(VecType x, VecType y) : x(x), y(y), z(0) { }
    Vector3(VecType x, VecType y, VecType z) : x(x), y(y), z(z) { }

    Vector3& operator=(const Vector3& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    bool operator==(const Vector3& rhs) const {
        VecType epsilon = 0.00001;
        return std::fabs(x - rhs.x) < epsilon && 
               std::fabs(y - rhs.y) < epsilon &&
               std::fabs(z - rhs.z) < epsilon;
    }

    bool operator!=(const Vector3& rhs) const {
        return !(*this == rhs);
    }

    Vector3 operator-() {
        return Vector3(-x, -y, -z);
    }

    inline Vector3& operator+=(VecType v) {
        x += v;
        y += v;
        z += v;
        return *this;
    }

    inline Vector3& operator-=(VecType v) {
        x -= v;
        y -= v;
        z -= v;
        return *this;
    }

    inline Vector3& operator*=(VecType v) {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    inline Vector3& operator/=(VecType v) {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }

    inline Vector3 operator+(const Vector3& v) {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    inline Vector3 operator-(const Vector3& v) {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    inline Vector3 operator/(const Vector3& v) {
        return Vector3(x / v.x, y / v.y, z / v.z);
    }

    inline Vector3 operator+(VecType v) {
        return Vector3(x + v, y + v, z + v);
    }

    inline Vector3 operator-(VecType v) {
        return Vector3(x - v, y - v, z - v);
    }

    inline Vector3 operator*(VecType v) {
        return Vector3(x * v, y * v, z * v);
    }

    inline Vector3 operator/(VecType v) {
        return Vector3(x / v, y / v, z / v);
    }

    inline double operator*(const Vector3& rhs) {
        return Dot(rhs);
    }

    inline double Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    inline double LengthSq() const {
        return x * x + y * y + z * z;
    }

    inline void Normalize() {
        double length = Length();
        if (length > std::numeric_limits<VecType>::epsilon() * 2) {
            x /= length;
            y /= length;
            z /= length;
        }
    }

    inline double Dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    inline Vector3& Cross(const Vector3& v2) {
        *this = Vector3(y*v2.z - z*v2.y,
                        z*v2.x - x*v2.z,
                        x*v2.y - y*v2.x);
        return *this;
    }

    inline double Distance(const Vector3& other) {
        return Vector3(*this - other).Length();
    }
};

template <typename T>
inline double DotProduct(const Vector3<T>& v1, const Vector3<T>& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

template <typename T>
inline Vector3<T> CrossProduct(const Vector3<T>& v1, const Vector3<T>& v2) {
    return Vector3<T>(v1.y*v2.z - v1.z*v2.y,
                   v1.z*v2.x - v1.x*v2.z,
                   v1.x*v2.y - v1.y*v2.x);
}

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Vector3<T>& v) {
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

template <typename T>
inline Vector3<T> operator+(const Vector3<T>& v1, const Vector3<T>& v2) {
    return Vector3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

template <typename T>
inline Vector3<T> operator-(const Vector3<T>& v1, const Vector3<T>& v2) {
    return Vector3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

typedef Vector3<int64_t> Vector3i;
typedef Vector3<int16_t> Vector3s;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

#endif
