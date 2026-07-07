
if (CMAKE_SYSTEM_NAME MATCHES "linux")
    set(LIB_EXT_NAME "a")
	set(LIB_PRE_NAME "lib")
	set(DYN_EXT_NAME "so")
    set(OS_NAME "linux")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()