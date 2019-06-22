#include <mclib/core/PlayerManager.h>

#include <mclib/protocol/packets/PacketDispatcher.h>

namespace mc {
namespace core {

PlayerManager::PlayerManager(protocol::packets::PacketDispatcher* dispatcher,
                             entity::EntityManager* entityManager)
    : protocol::packets::PacketHandler(dispatcher),
      m_EntityManager(entityManager) {
    dispatcher->RegisterHandler(protocol::State::Login,
                                protocol::login::LoginSuccess, this);

    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::PlayerListItem, this);
    dispatcher->RegisterHandler(protocol::State::Play,
                                protocol::play::PlayerPositionAndLook, this);

    m_EntityManager->RegisterListener(this);
}

PlayerManager::~PlayerManager() {
    m_EntityManager->UnregisterListener(this);
    GetDispatcher()->UnregisterHandler(this);
}

PlayerManager::iterator PlayerManager::begin() { return m_Players.begin(); }

PlayerManager::iterator PlayerManager::end() { return m_Players.end(); }

void PlayerManager::OnPlayerSpawn(entity::PlayerEntityPtr entity, UUID uuid) {
    auto iter = m_Players.find(uuid);

    if (iter == m_Players.end())
        m_Players[uuid] = std::make_shared<Player>(uuid, L"");

    m_Players[uuid]->SetEntity(entity);

    NotifyListeners(&PlayerListener::OnPlayerSpawn, m_Players[uuid]);
}

void PlayerManager::OnEntityDestroy(entity::EntityPtr entity) {
    EntityId eid = entity->GetEntityId();

    auto player = GetPlayerByEntityId(eid);

    if (player) {
        player->SetEntity(entity::PlayerEntityPtr());
        NotifyListeners(&PlayerListener::OnPlayerDestroy, player, eid);
    }
}

void PlayerManager::OnEntityMove(entity::EntityPtr entity, Vector3d oldPos,
                                 Vector3d newPos) {
    EntityId eid = entity->GetEntityId();
    PlayerPtr player = GetPlayerByEntityId(eid);

    if (!player) return;

    NotifyListeners(&PlayerListener::OnPlayerMove, player, oldPos, newPos);
}

PlayerPtr PlayerManager::GetPlayerByUUID(UUID uuid) const {
    auto iter = m_Players.find(uuid);

    if (iter != m_Players.end()) return iter->second;

    return nullptr;
}

PlayerPtr PlayerManager::GetPlayerByEntityId(EntityId eid) const {
    auto iter = std::find_if(m_Players.begin(), m_Players.end(),
                             [eid](std::pair<UUID, PlayerPtr> kv) {
                                 PlayerPtr player = kv.second;
                                 auto ptr = player->GetEntity();
                                 if (!ptr) return false;
                                 return ptr->GetEntityId() == eid;
                             });

    if (iter != m_Players.end()) return iter->second;

    return nullptr;
}

PlayerPtr PlayerManager::GetPlayerByName(const std::wstring& name) const {
    auto iter = std::find_if(m_Players.begin(), m_Players.end(),
                             [name](std::pair<UUID, PlayerPtr> kv) {
                                 PlayerPtr player = kv.second;
                                 return player->GetName().compare(name) == 0;
                             });

    if (iter != m_Players.end()) return iter->second;

    return nullptr;
}

void PlayerManager::HandlePacket(
    protocol::packets::in::LoginSuccessPacket* packet) {
    m_ClientUUID = UUID::FromString(packet->GetUUID());
}

void PlayerManager::HandlePacket(
    protocol::packets::in::PlayerPositionAndLookPacket* packet) {
    auto player = m_EntityManager->GetPlayerEntity();

    auto iter = m_Players.find(m_ClientUUID);
    if (iter == m_Players.end()) {
        m_Players[m_ClientUUID] = std::make_shared<Player>(m_ClientUUID, L"");
    }
    m_Players[m_ClientUUID]->SetEntity(player);

    NotifyListeners(&PlayerListener::OnClientSpawn, m_Players[m_ClientUUID]);
}

void PlayerManager::HandlePacket(
    protocol::packets::in::PlayerListItemPacket* packet) {
    using namespace protocol::packets::in;

    auto action = packet->GetAction();
    const auto& actionDataList = packet->GetActionData();

    for (const auto& actionData : actionDataList) {
        UUID uuid = actionData->uuid;

        if (action == PlayerListItemPacket::Action::AddPlayer) {
            auto iter = m_Players.find(uuid);

            if (iter != m_Players.end()) {
                bool newPlayer = iter->second->m_Name.empty();
                if (newPlayer) {
                    iter->second->m_Name = actionData->name;
                    NotifyListeners(&PlayerListener::OnPlayerJoin,
                                    m_Players[uuid]);
                }
                continue;
            }

            PlayerPtr player;

            player = std::make_shared<Player>(uuid, actionData->name);

            m_Players[uuid] = player;

            NotifyListeners(&PlayerListener::OnPlayerJoin, m_Players[uuid]);
        } else if (action == PlayerListItemPacket::Action::RemovePlayer) {
            if (m_Players.find(uuid) == m_Players.end()) continue;

            NotifyListeners(&PlayerListener::OnPlayerLeave, m_Players[uuid]);

            m_Players.erase(uuid);
        }
    }
}

}  // namespace core
}  // namespace mc
