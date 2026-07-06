project(WhyApp)
file(GLOB_RECURSE source CONFIGURE_DEPENDS *.h *.cpp *.qrc *.ui *.glsl *.hazel)


#link_directories(${lib_dir_path})

source_group_by_dir(source "./")
add_executable(${PROJECT_NAME} ${source})

config_output_paths(${PROJECT_NAME})

#target_include_directories(${PROJECT_NAME} PRIVATE 
#	${src_dir_path}/Hazelnut
#	${src_dir_path}/Hazelnut/panels
#
#)
#
#target_link_libraries(${PROJECT_NAME} PRIVATE 
##自写库
#	Hazel
##三方
#	
#)

#copy_dir_to_bin(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/assets assets)
#copy_dir_to_bin(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Resources Resources)



