# release和debug模式区分，系统区分
set(BuildConfig ${OS_NAME}/${CMAKE_CFG_INTDIR})
#

#
# build path
set(bin_dir_path ${CMAKE_BINARY_DIR}/bin)

# 自写库的生成路径
set(lib_dir_path ${CMAKE_BINARY_DIR}/lib)
#

# 源码路径
set(src_dir_path ${CMAKE_SOURCE_DIR}/src)