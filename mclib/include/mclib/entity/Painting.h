#ifndef MCLIB_ENTITY_PAINTING_H_
#define MCLIB_ENTITY_PAINTING_H_

#include <mclib/entity/Entity.h>

namespace mc {
namespace entity {

class PaintingEntity : public Entity {
public:
    enum class Direction { South, West, North, East };

private:
    std::wstring m_Title;
    Direction m_Direction;

public:
    PaintingEntity(EntityId eid, protocol::Version protocolVersion)
        : Entity(eid, protocolVersion),
          m_Title(L""),
          m_Direction(Direction::South) {
        SetType(EntityType::Painting);
    }

    const std::wstring& GetTitle() const noexcept { return m_Title; }
    Direction GetDirection() const noexcept { return m_Direction; }

    inline void SetTitle(const std::wstring& title) { m_Title = title; }
    inline void SetDirection(Direction dir) noexcept { m_Direction = dir; }
};

}  // namespace entity
}  // namespace mc

#endif
