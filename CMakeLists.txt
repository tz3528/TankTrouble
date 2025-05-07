# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required(VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("TankTrouble")

set(CMAKE_CXX_STANDARD 20)

# Unicode编码
add_definitions("-DUNICODE" "-D_UNICODE")

# 添加 EasyX s
link_directories("${CMAKE_SOURCE_DIR}/lib")

set(BOOST_ROOT "D:/C++/library/boost")
set(BOOST_INCLUDE "D:/C++/library/boost/include")

find_package(Boost REQUIRED COMPONENTS
        thread
)

if(NOT BOOST_FOUND)
    message("NOT found Boost")
endif()

include_directories(${BOOST_INCLUDE})

file(GLOB_RECURSE MAIN_SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")
include_directories("${CMAKE_SOURCE_DIR}/include")

file(GLOB_RECURSE MODULE_SRC "${CMAKE_SOURCE_DIR}/module/*.cpp")
include_directories("${CMAKE_SOURCE_DIR}/module")

# 将所有 .cpp 文件合并到一起
set(ALL_SRC ${MAIN_SRC} ${MODULE_SRC})

# 添加可执行文件
add_executable(TankTrouble WIN32 ${ALL_SRC})

# 链接 Windows 库
target_link_libraries(TankTrouble PRIVATE gdi32 user32 kernel32 ws2_32)

# 静态链接运行时库
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

# 配置预编译头文件
target_precompile_headers(TankTrouble PRIVATE "${CMAKE_SOURCE_DIR}/include/pch.h")

#在这里根据名字boost_thread去寻找libboost_thread.a文件
target_link_libraries(${PROJECT_NAME} ${BOOST_LIBRARIES})
