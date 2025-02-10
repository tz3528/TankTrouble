# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("TankTrouble")

set(CMAKE_CXX_STANDARD 17)

# Unicode编码
add_definitions("-DUNICODE" "-D_UNICODE")

# 查找 DirectX 库和头文件
find_path(DIRECTX_INCLUDE_DIR d3d12.h
  PATHS "C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/um"
        "C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/shared"
        "C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/ucrt"
        "C:/Program Files (x86)/Windows Kits/10/Include/10.0.22621.0/winrt"
)

find_library(D3D12_LIBRARY d3d12
  PATHS "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64"
)

find_library(DXGI_LIBRARY dxgi
  PATHS "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64"
)

find_library(D3DCOMPILER_LIBRARY d3dcompiler
  PATHS "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64"
)

find_library(DXGUID_LIBRARY dxguid
  PATHS "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64"
)

# 确保定义 _WIN32_WINNT 和 NTDDI_VERSION 为 Windows 11
add_definitions(-D_WIN32_WINNT=0x0A00)
add_definitions(-DNTDDI_VERSION=0x0A00000C)

# 添加 DirectX 头文件路径
include_directories(${DIRECTX_INCLUDE_DIR})

# 添加 EasyX s
link_directories("./lib")

file(GLOB MAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(TankTrouble WIN32 ${MAIN_SRC})

# 链接 DirectX 库
target_link_libraries(TankTrouble ${D3D12_LIBRARY} ${DXGI_LIBRARY} ${D3DCOMPILER_LIBRARY} ${DXGUID_LIBRARY})

# 链接 EasyX 库
# target_link_libraries(TankTrouble EasyXa)