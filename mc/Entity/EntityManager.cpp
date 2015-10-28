#include "EntityManager.h"

namespace Minecraft {

EntityManager::EntityManager(Packets::PacketDispatcher& dispatcher)
    : m_Dispatcher(dispatcher),
    Packets::PacketHandler(&m_Dispatcher)
{

}

void EntityManager::HandlePacket(Packets::Inbound::SpawnPlayerPacket* packet) {
    EntityId id = packet->GetEntityId();

    Entity* entity = new Entity(id);

    m_Entities[id] = entity;

    entity->SetPosition(Vector3d(packet->GetX(), packet->GetY(), packet->GetZ()));

}

void EntityManager::HandlePacket(Packets::Inbound::SpawnMobPacket* packet) {
    EntityId id = packet->GetEntityId();

    Entity* entity = new Entity(id);

    m_Entities[id] = entity;
    entity->SetPosition(Vector3d(packet->GetX(), packet->GetY(), packet->GetZ()));
}

void EntityManager::HandlePacket(Packets::Inbound::EntityPacket* packet) {

}

void EntityManager::HandlePacket(Packets::Inbound::EntityRelativeMovePacket* packet) {
    EntityId id = packet->GetEntityId();

    Vector3d delta(packet->GetDeltaX(), packet->GetDeltaY(), packet->GetDeltaZ());
    Vector3d newPos = m_Entities[id]->GetPosition() + delta;
    m_Entities[id]->SetPosition(newPos);
}

void EntityManager::HandlePacket(Packets::Inbound::EntityHeadLookPacket* packet) {

}

void EntityManager::HandlePacket(Packets::Inbound::EntityMetadataPacket* packet) {
    EntityId id = packet->GetEntityId();

}

void EntityManager::HandlePacket(Packets::Inbound::EntityPropertiesPacket* packet) {

}

} // ns Minecraft
