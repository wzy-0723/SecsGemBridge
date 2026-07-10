project(Common)

set(src_path
    "./"
)

file(GLOB_RECURSE project_headers *.h *.hpp)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_files ${project_headers} ${project_cpps})

add_library(${PROJECT_NAME} STATIC ${all_files})


