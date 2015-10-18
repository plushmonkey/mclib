#ifndef PROTOCOL_H_
#define PROTOCOL_H_

namespace Minecraft {

enum ProtocolState {
    Handshake,
    Status,
    Login,
    Play
};

}

#endif
