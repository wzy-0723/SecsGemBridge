if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(LIB_EXT_NAME ".a")
	set(LIB_PRE_NAME "")
	set(DYN_EXT_NAME ".dll")
    set(OS_NAME "Windows")

elseif (CMAKE_SYSTEM_NAME MATCHES "linux")
    set(LIB_EXT_NAME ".a")
	set(LIB_PRE_NAME ".lib")
	set(DYN_EXT_NAME ".so")
    set(OS_NAME "linux")
else()
	#mac
endif()