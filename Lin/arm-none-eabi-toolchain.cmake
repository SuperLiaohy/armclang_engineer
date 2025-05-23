# arm-none-eabi-toolchain.cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m7)  # 明确指定处理器类型
#set(CMAKE_SYSTEM_ARCH armv7e-m)        # 指定架构

# 设置工具链路径
set(TOOLCHAIN_PATH "/home/liaohy/.vcpkg/artifacts/2139c4c6/compilers.arm.armclang/6.23.0/bin/")

# 设置编译器
set(CMAKE_C_COMPILER "${TOOLCHAIN_PATH}/armclang")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH}/armclang")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_PATH}/armclang")
set(CMAKE_LINKER "${TOOLCHAIN_PATH}/armlink")
set(CMAKE_AR "${TOOLCHAIN_PATH}/armar")

# 设置编译器标志
set(CMAKE_C_FLAGS_INIT "--target=arm-arm-none-eabi -mcpu=cortex-m7")
set(CMAKE_CXX_FLAGS_INIT ${CMAKE_C_FLAGS_INIT})
set(CMAKE_ASM_FLAGS_INIT ${CMAKE_C_FLAGS_INIT})

# 强制使用交叉编译器
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)
set(CMAKE_ASM_COMPILER_FORCED TRUE)
