find_package(GTest REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/mclib/include)
file(GLOB_RECURSE SOURCES_LIST "*.cpp")

add_executable(UnitTests
    ${SOURCES_LIST}
)

target_link_libraries(UnitTests
                    ${GTEST_LIBRARIES}
                    ${GTEST_MAIN_LIBRARIES}
                    mclib)