file(GLOB_RECURSE SOURCES_LIST "*.cpp")

include_directories(${PROJECT_SOURCE_DIR}/mclib/include)
add_executable(client
    ${SOURCES_LIST}
)

target_link_libraries(client mclib)
