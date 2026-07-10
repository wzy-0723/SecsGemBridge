project(Platform)

#
file(GLOB_RECURSE project_headers *.h)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_sourcefile ${project_headers} ${project_cpps})
set(src_path "./")

set(all_sourcefile ${all_sourcefile} ${platform_include_headers})
add_library(${PROJECT_NAME} STATIC ${all_sourcefile})

add_dependencies(${PROJECT_NAME} Common)


target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${src}/Common
	${src}/Platform/include
)

#include_directories(${src}/Common)
#include_directories(${src}/Platform/include)
target_link_libraries(${PROJECT_NAME} 
	PUBLIC
	        Common
)



