# release和debug模式区分，系统区分
set(BuildConfig ${OS_NAME}/${CMAKE_CFG_INTDIR})
#

#
# build path
set(bin ${CMAKE_BINARY_DIR}/bin)

# 自写库的生成路径
set(lib ${CMAKE_BINARY_DIR}/lib)
#

# 源码路径
set(src ${CMAKE_SOURCE_DIR}/src)
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_CFG_INTDIR: ${CMAKE_CFG_INTDIR}")
message(STATUS "BuildConfig: ${BuildConfig}")