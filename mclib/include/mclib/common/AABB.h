#ifndef MCLIB_COMMON_AABB_H_
#define MCLIB_COMMON_AABB_H_

#include <mclib/common/Vector.h>
#include <mclib/mclib.h>

#include <algorithm>
#include <limits>

#undef min
#undef max

namespace mc {

class Ray {
private:
    Vector3d m_Origin;
    Vector3d m_Direction;
    Vector3d m_DirectionReciprocal;

public:
    Ray(const Vector3d& origin, const Vector3d& direction) noexcept
        : m_Origin(origin), m_Direction(direction) {
        if (m_Direction.x == 0.0)
            m_DirectionReciprocal.x = std::numeric_limits<double>::max();
        else
            m_DirectionReciprocal.x = 1.0 / direction.x;

        if (m_Direction.y == 0.0)
            m_DirectionReciprocal.y = std::numeric_limits<double>::max();
        else
            m_DirectionReciprocal.y = 1.0 / direction.y;

        if (m_Direction.z == 0.0)
            m_DirectionReciprocal.z = std::numeric_limits<double>::max();
        else
            m_DirectionReciprocal.z = 1.0 / direction.z;
    }

    const Vector3d& GetOrigin() const noexcept { return m_Origin; }
    const Vector3d& GetDirection() const noexcept { return m_Direction; }
    const Vector3d& GetReciprocal() const noexcept {
        return m_DirectionReciprocal;
    }
};

struct AABB {
    Vector3d min;
    Vector3d max;

    MCLIB_API AABB() noexcept : min(0, 0, 0), max(0, 0, 0) {}
    MCLIB_API AABB(const Vector3d& min, const Vector3d& max) noexcept
        : min(min), max(max) {}

    bool MCLIB_API Contains(Vector3d point) const noexcept {
        return (point.x >= min.x && point.x <= max.x) &&
               (point.y >= min.y && point.y <= max.y) &&
               (point.z >= min.z && point.z <= max.z);
    }

    AABB operator+(Vector3d offset) const {
        return AABB(min + offset, max + offset);
    }

    AABB operator+(Vector3i offset) const {
        return AABB(min + ToVector3d(offset), max + ToVector3d(offset));
    }

    bool MCLIB_API Intersects(const AABB& other) const noexcept {
        return (max.x > other.min.x && min.x < other.max.x &&
                max.y > other.min.y && min.y < other.max.y &&
                max.z > other.min.z && min.z < other.max.z);
    }

    bool MCLIB_API Intersects(const Ray& ray, double* length) const noexcept {
        double t1 = (min.x - ray.GetOrigin().x) * ray.GetReciprocal().x;
        double t2 = (max.x - ray.GetOrigin().x) * ray.GetReciprocal().x;

        double t3 = (min.y - ray.GetOrigin().y) * ray.GetReciprocal().y;
        double t4 = (max.y - ray.GetOrigin().y) * ray.GetReciprocal().y;

        double t5 = (min.z - ray.GetOrigin().z) * ray.GetReciprocal().z;
        double t6 = (max.z - ray.GetOrigin().z) * ray.GetReciprocal().z;

        using std::max;
        using std::min;

        double tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
        double tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

        if (tmax < 0) {
            if (length) *length = tmax;
            return false;
        }

        if (tmin > tmax) {
            if (length) *length = tmax;
            return false;
        }

        if (length) *length = tmin;
        return true;
    }
};

}  // namespace mc

#endif
