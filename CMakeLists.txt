cmake_minimum_required(VERSION 3.1)

project(mclib)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)

add_library(mclib SHARED
	mclib/src/mclib/block/Banner.cpp
	mclib/src/mclib/block/Beacon.cpp
	mclib/src/mclib/block/Bed.cpp
	mclib/src/mclib/block/Block.cpp
	mclib/src/mclib/block/BlockEntity.cpp
	mclib/src/mclib/block/BrewingStand.cpp
	mclib/src/mclib/block/Chest.cpp
	mclib/src/mclib/block/Dispenser.cpp
	mclib/src/mclib/block/Dropper.cpp
	mclib/src/mclib/block/EnchantmentTable.cpp
	mclib/src/mclib/block/EndGateway.cpp
	mclib/src/mclib/block/FlowerPot.cpp
	mclib/src/mclib/block/Furnace.cpp
	mclib/src/mclib/block/Hopper.cpp
	mclib/src/mclib/block/InventoryBlock.cpp
	mclib/src/mclib/block/Jukebox.cpp
	mclib/src/mclib/block/MonsterSpawner.cpp
	mclib/src/mclib/block/NoteBlock.cpp
	mclib/src/mclib/block/Piston.cpp
	mclib/src/mclib/block/RedstoneComparator.cpp
	mclib/src/mclib/block/ShulkerBox.cpp
	mclib/src/mclib/block/Sign.cpp
	mclib/src/mclib/block/Skull.cpp
	mclib/src/mclib/common/DataBuffer.cpp
	mclib/src/mclib/common/DyeColor.cpp
	mclib/src/mclib/common/MCString.cpp
	mclib/src/mclib/common/Position.cpp
	mclib/src/mclib/common/UUID.cpp
	mclib/src/mclib/common/VarInt.cpp
	mclib/src/mclib/core/AuthToken.cpp
	mclib/src/mclib/core/Client.cpp
	mclib/src/mclib/core/ClientSettings.cpp
	mclib/src/mclib/core/Compression.cpp
	mclib/src/mclib/core/Connection.cpp
	mclib/src/mclib/core/Encryption.cpp
	mclib/src/mclib/core/PlayerManager.cpp
	mclib/src/mclib/entity/EntityManager.cpp
	mclib/src/mclib/entity/Metadata.cpp
	mclib/src/mclib/inventory/Hotbar.cpp
	mclib/src/mclib/inventory/Inventory.cpp
	mclib/src/mclib/inventory/Slot.cpp
	mclib/src/mclib/nbt/NBT.cpp
	mclib/src/mclib/nbt/Tag.cpp
	mclib/src/mclib/network/IPAddress.cpp
	mclib/src/mclib/network/Network.cpp
	mclib/src/mclib/network/Socket.cpp
	mclib/src/mclib/network/TCPSocket.cpp
	mclib/src/mclib/network/UDPSocket.cpp
	mclib/src/mclib/protocol/packets/Packet.cpp
	mclib/src/mclib/protocol/packets/PacketDispatcher.cpp
	mclib/src/mclib/protocol/packets/PacketFactory.cpp
	mclib/src/mclib/protocol/packets/PacketHandler.cpp
	mclib/src/mclib/protocol/Protocol.cpp
	mclib/src/mclib/util/Forge.cpp
	mclib/src/mclib/util/Hash.cpp
	mclib/src/mclib/util/HTTPClient.cpp
	mclib/src/mclib/util/Utility.cpp
	mclib/src/mclib/util/VersionFetcher.cpp
	mclib/src/mclib/util/Yggdrasil.cpp
	mclib/src/mclib/world/Chunk.cpp
	mclib/src/mclib/world/World.cpp
)

add_definitions(-DMCLIB_EXPORTS -DCURL_STATICLIB)

set_target_properties(mclib PROPERTIES VERSION 1.0.0)
set_target_properties(mclib PROPERTIES SOVERSION 1)

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

target_include_directories(mclib 
	PUBLIC 
		mclib/include
	PRIVATE
		${OPENSSL_INCLUDE_DIR}
		${CURL_INCLUDE_DIRS}
		${ZLIB_INCLUDE_DIRS}
)

if (WIN32)
target_link_libraries(mclib ${ZLIB_LIBRARIES} ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES} Wldap32.lib Crypt32.lib)
else ()
target_link_libraries(mclib ${ZLIB_LIBRARIES} ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES})
endif ()

include(GNUInstallDirs)

install(TARGETS mclib
	DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY mclib/include DESTINATION ${CMAKE_INSTALL_PREFIX}/)
	
configure_file(mclib.pc.in mclib.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/mclib.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

add_executable(client
	client/BlockPlacer.cpp
	client/Logger.cpp
	client/main.cpp
	client/SneakEnforcer.cpp
	client/TextureGrabber.cpp
)

target_link_libraries(client PRIVATE mclib)
