#include <mclib/core/Client.h>
#include <mclib/util/Utility.h>

#include <iostream>

// These were changed in MSVC 2015. Redefine them so the old lib files link
// correctly. legacy_stdio_definitions.lib is supposed to define these but it
// doesn't seem to work.
#if (defined _MSC_VER) && _MSC_VER >= 1900
extern "C" {
FILE __iob_func[3] = {*stdin, *stdout, *stderr};
}
int(WINAPIV* __vsnprintf)(char*, size_t, const char*, va_list) = _vsnprintf;
int(WINAPIV* _fprintf)(FILE* const, char const*, ...) = fprintf;
#endif

namespace mc {
namespace core {

Client::Client(protocol::packets::PacketDispatcher* dispatcher,
               protocol::Version version)
    : m_Dispatcher(dispatcher),
      m_Connection(m_Dispatcher, version),
      m_EntityManager(m_Dispatcher, version),
      m_PlayerManager(m_Dispatcher, &m_EntityManager),
      m_World(m_Dispatcher),
      m_PlayerController(std::make_unique<util::PlayerController>(
          &m_Connection, m_World, m_PlayerManager)),
      m_LastUpdate(0),
      m_Connected(false),
      m_InventoryManager(std::make_unique<inventory::InventoryManager>(
          m_Dispatcher, &m_Connection)),
      m_Hotbar(m_Dispatcher, &m_Connection, m_InventoryManager.get()) {
    m_Connection.RegisterListener(this);
}

Client::~Client() {
    m_Connection.Disconnect();
    m_Connected = false;
    if (m_UpdateThread.joinable()) m_UpdateThread.join();
    m_Connection.UnregisterListener(this);
}

void Client::OnSocketStateChange(network::Socket::Status newState) {
    m_Connected = (newState == network::Socket::Status::Connected);
}

void Client::Update() {
    try {
        m_Connection.CreatePacket();
    } catch (std::exception& e) {
        std::wcout << e.what() << std::endl;
    }

    entity::EntityPtr playerEntity = m_EntityManager.GetPlayerEntity();
    if (playerEntity) {
        // Keep entity manager and player controller in sync
        playerEntity->SetPosition(m_PlayerController->GetPosition());
    }

    s64 time = util::GetTime();
    if (time >= m_LastUpdate + (1000 / 20)) {
        m_PlayerController->Update();
        NotifyListeners(&ClientListener::OnTick);
        m_LastUpdate = time;
    }
}

void Client::UpdateThread() {
    while (m_Connected) {
        Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

bool Client::Login(const std::string& host, unsigned short port,
                   const std::string& user, const std::string& password,
                   UpdateMethod method) {
    if (m_UpdateThread.joinable()) {
        m_Connected = false;
        m_UpdateThread.join();
    }

    m_LastUpdate = 0;

    if (!m_Connection.Connect(host, port))
        throw std::runtime_error("Could not connect to server");

    if (!m_Connection.Login(user, password)) return false;

    if (method == UpdateMethod::Threaded) {
        m_UpdateThread = std::thread(&Client::UpdateThread, this);
    } else if (method == UpdateMethod::Block) {
        UpdateThread();
    }
    return true;
}

bool Client::Login(const std::string& host, unsigned short port,
                   const std::string& user, AuthToken token,
                   UpdateMethod method) {
    if (m_UpdateThread.joinable()) {
        m_Connected = false;
        m_UpdateThread.join();
    }

    m_LastUpdate = 0;

    if (!m_Connection.Connect(host, port))
        throw std::runtime_error("Could not connect to server");

    if (!m_Connection.Login(user, token)) return false;

    if (method == UpdateMethod::Threaded) {
        m_UpdateThread = std::thread(&Client::UpdateThread, this);
    } else if (method == UpdateMethod::Block) {
        UpdateThread();
    }
    return true;
}

void Client::Ping(const std::string& host, unsigned short port,
                  UpdateMethod method) {
    if (m_UpdateThread.joinable()) {
        m_Connected = false;
        m_UpdateThread.join();
    }

    if (!m_Connection.Connect(host, port))
        throw std::runtime_error("Could not connect to server");

    m_Connection.Ping();

    if (method == UpdateMethod::Threaded) {
        m_UpdateThread = std::thread(&Client::UpdateThread, this);
    } else if (method == UpdateMethod::Block) {
        UpdateThread();
    }
}

}  // namespace core
}  // namespace mc
