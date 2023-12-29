cmake_minimum_required(VERSION 3.17)
project(GameEngine)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    add_compile_options(/wd4819)#忽略保存为unicode警告
endif ()


#头文件目录
include_directories("depends/include")
include_directories("source")
#添加第三方库
link_directories("depends/libs")
file(GLOB_RECURSE glad_c "depends/include/glad.c")

file(GLOB_RECURSE rttr_src "depends/include/rttr/*.cpp")
file(GLOB_RECURSE source_src "source/*.cpp")


add_executable(GameEngine ${glad_c} ${rttr_src}  ${source_src})
target_link_libraries(GameEngine glfw3)