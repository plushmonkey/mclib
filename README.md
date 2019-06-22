# mclib

C++ library for connecting to Minecraft servers.

## Features

- Full protocol for Minecraft 1.10 through 1.14.2
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

Tools:

- C++14 compiler (g++/clang/msvc 2017)
- [Conan](https://conan.io/index.html) package manager
- CMake 3.1+

Libraries:

- Curl
- Pthread
- Openssl
- Zlib
- Gtest

### Installing required librarires with conan

```shell
mkdir build
cd build
conan install ..
```

There may be some problems with gtest link. To deal with it follow the [instructions](https://docs.conan.io/en/latest/howtos/manage_gcc_abi.html)

### Building project

```shell
cmake ..
# and run your favourite build tool (ninja, make, VS Studio...)
make
```

## Examples

- [terracotta](https://github.com/plushmonkey/Terracotta) : OpenGL client
- [client](https://github.com/plushmonkey/mclib/blob/master/client/main.cpp) : Basic example client for connecting to a server
- [mcbot](https://github.com/plushmonkey/mcbot) : Bot with pathfinding and melee attacking
- [avalanche](https://github.com/plushmonkey/avalanche) : Server stress test tool
- [mcqt](https://github.com/plushmonkey/mcqt) : Chat client made with qt
