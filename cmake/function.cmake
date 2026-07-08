#定义宏，查找当前文件夹下所有文件，用于设置工程目录结构
macro(source_group_by_dir source_files strip_head)
    set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
    foreach (sgbd_file ${${source_files}})
        string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
        string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
        string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
        string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
        string(COMPARE EQUAL ${sgbd_group_name} "." root_path)
        if (root_path)
            set(sgbd_group_name "\\")
        else()
            if (sgbd_nogroup)
                set(sgbd_group_name "\\")
            else ()
                set(strip_len 0)
                string(LENGTH ${strip_head} strip_len)
				
                set(group_len 0)
                string(LENGTH ${sgbd_group_name}, group_len)

                if (NOT (${strip_head} STREQUAL "./"))
                    string(SUBSTRING ${sgbd_group_name} ${strip_len} ${group_len} sgbd_group_name)
                endif ()
            endif (sgbd_nogroup)
        endif()    
        source_group(${sgbd_group_name} FILES ${sgbd_file})
    endforeach (sgbd_file)
endmacro(source_group_by_dir)


# 对目标指定生成位置,dll和pdb也会被配置到bin文件！！！可能只适用于windows
macro(config_output_paths TARGETNAME)
  set_target_properties(${TARGETNAME}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${bin_dir_path}
    ARCHIVE_OUTPUT_DIRECTORY ${lib_dir_path}
    LIBRARY_OUTPUT_DIRECTORY ${lib_dir_path}
  )
endmacro(config_output_paths)


# @brief 生成后事件，拷贝文件到bin
# target:哪个目标生成后执行该事件
# file_path:源文件路径
macro(copy_file_to_bin target file_path)
	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${file_path}"
			"${bin_dir_path}/${CMAKE_CFG_INTDIR}")
endmacro(copy_file_to_bin)


# @brief 生成后事件，拷贝文件夹到bin
# target:哪个目标生成后执行该事件
# file_path:源目录路径
# file_name:源目录名称
macro(copy_dir_to_bin target file_path file_name)
	add_custom_command(TARGET ${target} POST_BUILD
# copy_directory：递归拷贝文件夹
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${file_path}"
			"${bin_dir_path}/${CMAKE_CFG_INTDIR}/${file_name}")
endmacro(copy_dir_to_bin)




