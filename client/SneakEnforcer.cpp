#include "SneakEnforcer.h"

#include <mclib/util/Utility.h>

namespace example {

SneakEnforcer::SneakEnforcer(mc::core::Client* client)
    : m_Client(client),
      m_PlayerManager(client->GetPlayerManager()),
      m_Connection(client->GetConnection()),
      m_StartTime(mc::util::GetTime()) {
    m_PlayerManager->RegisterListener(this);
    m_Client->RegisterListener(this);
}

SneakEnforcer::~SneakEnforcer() {
    m_PlayerManager->UnregisterListener(this);
    m_Client->UnregisterListener(this);
}

void SneakEnforcer::OnTick() {
    s64 ticks = mc::util::GetTime() - m_StartTime;
    float pitch =
        (((float)std::sin(ticks * 3 * 3.14 / 1000) * 0.5f + 0.5f) * 360.0f) -
        180.0f;
    pitch = (pitch / 5.5f) + 130.0f;

    m_Client->GetPlayerController()->SetPitch(pitch);
}

void SneakEnforcer::OnClientSpawn(mc::core::PlayerPtr player) {
    using namespace mc::protocol::packets::out;
    EntityActionPacket::Action action = EntityActionPacket::Action::StartSneak;

    EntityActionPacket packet(player->GetEntity()->GetEntityId(), action);
    m_Connection->SendPacket(&packet);
}

}  // namespace example
