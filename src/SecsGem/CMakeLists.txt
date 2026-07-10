project(SecsGem)

set(src_path
    "./"
)

file(GLOB_RECURSE project_headers *.h *.hpp)
file(GLOB_RECURSE project_cpps *.cpp)
set(all_files ${project_headers} ${project_cpps})

add_library(${PROJECT_NAME} STATIC ${all_files}
    hsms_transport.h hsms_transport.cpp)


# why?
target_include_directories(${PROJECT_NAME}
    PUBLIC
#        ${src}/Common
#        ${src}/SecsGem/include
#        ${src}/Platform/include
)

include_directories(${src}/Common)
include_directories(${src}/Platform/include)
include_directories(${src}/SecsGem/include)


add_dependencies(${PROJECT_NAME}
    Platform
)

include_directories(${src}/Common)
include_directories(${src}/SecsGem/include)
include_directories(${src}/Platform)
include_directories(${src}/Platform/include)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Common
        Platform
)