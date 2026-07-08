# Windows 系统（32/64位通用）
if(WIN32)
    set(OS_NAME "Windows")
endif()

# Linux 系统
if(UNIX AND NOT APPLE)
    set(OS_NAME "linux")
endif()