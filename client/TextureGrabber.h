#ifndef CLIENT_TEXTUREGRABBER_H_
#define CLIENT_TEXTUREGRABBER_H_

#include <mclib/protocol/packets/PacketHandler.h>

namespace example {

class TextureGrabber : public mc::protocol::packets::PacketHandler {
private:
    bool ContainsTextureURL(const mc::json& root);

public:
    TextureGrabber(mc::protocol::packets::PacketDispatcher* dispatcher);
    ~TextureGrabber();

    void HandlePacket(mc::protocol::packets::in::PlayerListItemPacket* packet);
};

}  // namespace example

#endif
