# mclib
C++ library for connecting to Minecraft servers.

## Features
- Full 1.11.2 and 1.12.0 protocol
- Premium and offline mode
- Yggdrasil API
- Logging in with passwords or tokens
- Reading and using token from launcher
- Encryption
- Compression
- Log in to Forge servers
- World data
- Entity data
- Basic inventory use
- NBT data
- Block entity data
- Block collisions
- Basic player controller for moving around

## Building

Dependencies:  
- C++14 compiler (g++/clang/msvc 2017)
- openssl
- libcurl
- jsoncpp

Ubuntu:  
`sudo apt-get install clang libssl-dev libcurl4-openssl-dev libjsoncpp-dev`  
`make`

Windows:  
`unzip lib.zip`  
Open `mclib.sln` with Visual Studio 2017 and build

## Examples
- [client](https://github.com/plushmonkey/mclib/blob/master/client/main.cpp) : Basic example client
- [mcbot](https://github.com/plushmonkey/mcbot) : Bot with pathfinding and melee attacking
- [avalanche](https://github.com/plushmonkey/avalanche) : Server stress test tool
- [mcqt](https://github.com/plushmonkey/mcqt) : Chat client made with qt
