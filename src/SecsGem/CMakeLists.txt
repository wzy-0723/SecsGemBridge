project(SecsGem)

set(src_path
    "./"
)

file(GLOB_RECURSE project_headers *.h *.hpp)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_files ${project_headers} ${project_cpps})
#source_group_by_dir(all_files ${src_path})

# include_directories(${glog_inc})
# include_directories(${foundation_dir}/common/)
# include_directories(${foundation_dir}/platform/include)
# include_directories(${sqlite_inc})
# include_directories(${ability_dir}/secs/include)
# include_directories(${ability_dir}/secs/steam_function)
# include_directories(${ability_dir}/secs/)

# link_directories(${glog_lib_path})
# link_directories(${lib_dir})

add_library(${PROJECT_NAME} STATIC ${all_files})

#set_target_properties(${proj_name} PROPERTIES FOLDER "ability")
#set_target_properties(${proj_name}
#	PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_dir}
#	ARCHIVE_OUTPUT_DIRECTORY ${lib_dir}
#	LIBRARY_OUTPUT_DIRECTORY ${lib_dir}
#)


