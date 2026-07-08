file(GLOB_RECURSE platform_include_headers include/*.h)
message(STATUS "platform_include_headers:${platform_include_headers}")
add_subdirectory(${OS_NAME})