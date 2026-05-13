project(Server)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED Core Widgets PrintSupport)

file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS "*.cpp")
add_executable(proc_stat_server ${SERVER_SOURCES})

target_precompile_headers(proc_stat_server PRIVATE "../pch.h")
target_compile_definitions(proc_stat_server PRIVATE $<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Release>:RELEASE> _GNU_SOURCE)

target_link_libraries(proc_stat_server
    Qt6::Widgets
    Qt6::Core
    Qt6::PrintSupport
)
