project(SecsGem)
set(src_path
    "./"
)

file(GLOB_RECURSE project_headers *.h *.hpp)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_files ${project_headers} ${project_cpps})
if (WIN32)
	set(all_files ${all_files} "./secs.def")
endif()
source_group_by_dir(all_files ${src_path})

# include_directories(${glog_inc})
# include_directories(${foundation_dir}/common/)
# include_directories(${foundation_dir}/platform/include)
# include_directories(${sqlite_inc})
# include_directories(${ability_dir}/secs/include)
# include_directories(${ability_dir}/secs/steam_function)
# include_directories(${ability_dir}/secs/)

# link_directories(${glog_lib_path})
# link_directories(${lib_dir})



#platform_add_library(${proj_name} STATIC ${all_files})
add_library(${PROJECT_NAME} STATIC ${all_files})
add_dependencies(${PROJECT_NAME} Common Platform)

target_include_directories(${PROJECT_NAME} PUBLIC 
	${src_dir_path}/Foundation/Common
	${src_dir_path}/Foundation/Platform
)

target_link_libraries(${PROJECT_NAME} PUBLIC 
#
	Common
	Platform
#
)
#set_target_properties(${proj_name} PROPERTIES FOLDER "ability")
#set_target_properties(${proj_name}
#	PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_dir}
#	ARCHIVE_OUTPUT_DIRECTORY ${lib_dir}
#	LIBRARY_OUTPUT_DIRECTORY ${lib_dir}
#)


