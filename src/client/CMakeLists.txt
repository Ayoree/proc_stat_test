project(Client)

file(GLOB_RECURSE CLIENT_SOURCES CONFIGURE_DEPENDS "*.c")
add_executable(proc_stat_client ${CLIENT_SOURCES})

target_precompile_headers(proc_stat_client PRIVATE "../pch.h")
target_compile_definitions(proc_stat_client PRIVATE $<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Release>:RELEASE> _GNU_SOURCE)