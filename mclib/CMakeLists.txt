find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Threads)

include_directories(include)
file(GLOB_RECURSE SOURCES_LIST "src/*.cpp")

add_library(mclib
	${SOURCES_LIST}
)

set_target_properties(mclib PROPERTIES VERSION 1.0.0)
set_target_properties(mclib PROPERTIES SOVERSION 1)


target_link_libraries(mclib
                    ${ZLIB_LIBRARIES}
                    ${CURL_LIBRARIES}
                    ${OPENSSL_LIBRARIES}
                    ${CMAKE_THREAD_LIBS_INIT}
                    ${CMAKE_DL_LIBS})
