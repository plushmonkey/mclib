#ifndef CLIENT_SNEAKENFORCER_H_
#define CLIENT_SNEAKENFORCER_H_

#include <mclib/core/Client.h>
#include <mclib/core/PlayerManager.h>

namespace example {

class SneakEnforcer : public mc::core::PlayerListener,
                      public mc::core::ClientListener {
private:
    mc::core::Client* m_Client;
    mc::core::PlayerManager* m_PlayerManager;
    mc::core::Connection* m_Connection;
    s64 m_StartTime;

public:
    SneakEnforcer(mc::core::Client* client);
    ~SneakEnforcer();

    void OnTick() override;
    void OnClientSpawn(mc::core::PlayerPtr player) override;
};

}  // namespace example

#endif
