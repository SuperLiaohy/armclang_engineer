cmake_minimum_required(VERSION 3.22)

include(cmake/arm-none-eabi-toolchain.cmake)

# 在project命令之前设置工具链
#set(CMAKE_TOOLCHAIN_FILE cmake/arm-none-eabi-toolchain.cmake)
set(COMMON_OPTIMIZATION_FLAGS "-O0")
# 项目名称
project(armclang_engineer
        VERSION 1.0.0
        LANGUAGES C CXX ASM
        DESCRIPTION "STM32H723 armclang_engineer Project"
)


# 编译器标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 通用编译标志
set(COMMON_FLAGS "--target=arm-arm-none-eabi -mcpu=cortex-m7  -mfpu=fpv5-d16 -mfloat-abi=hard ${COMMON_OPTIMIZATION_FLAGS} -w")
set(DEBUG_FLAGS "-gdwarf-4 ${COMMON_OPTIMIZATION_FLAGS} -ffunction-sections")

# 设置编译器标志
set(CMAKE_C_FLAGS "${COMMON_FLAGS} ${DEBUG_FLAGS} -xc -std=c99 -fno-rtti -funsigned-char -fshort-enums -fshort-wchar")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} ${DEBUG_FLAGS} -xc++ -std=c++20 -fno-rtti -funsigned-char -fshort-enums -fshort-wchar")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS} ${DEBUG_FLAGS} -masm=auto -Wa,--diag_suppress=A1950W")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -Wa,--pd,\"__UVISION_VERSION SETA 539\" -Wa,--pd,\"_RTE_ SETA 1\" -Wa,--pd,\"STM32H723xx SETA 1\"")

# 链接器标志
set(CMAKE_EXE_LINKER_FLAGS
        "--cpu Cortex-M7.fp.dp \
    --strict \
    --scatter \"${CMAKE_SOURCE_DIR}/MDK-ARM/${PROJECT_NAME}/${PROJECT_NAME}.sct\" \
    --summary_stderr --info summarysizes --map --load_addr_map_info --xref --callgraph --symbols \
    --info sizes --info totals --info unused --info veneers \
    --list \"${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map\""
)

# 获取源文件
file(GLOB_RECURSE SOURCES
        "MDK-ARM/*.s"
        "Core/Src/*.c"
        "Drivers/STM32H7xx_HAL_Driver/Src/*.c"
        "USB_DEVICE/App/*.c"
        "USB_DEVICE/Target/*.c"
        "Middlewares/Third_Party/FreeRTOS/Source/*.c"
        "Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/*.c"
        "Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c"
        "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/*.c"
        "Middlewares/ST/STM32_USB_Device_Library/Core/Src/*.c"
        "Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/*.c"
)

file(GLOB_RECURSE OWN_SOURCES
        "Own/CallBack/*.c" "Own/CallBack/*.cpp"
        "Own/App/*.c" "Own/App/*.cpp"
        "Own/Bsp/*.c" "Own/Bsp/*.cpp"
        "Own/Mod/*.c" "Own/Mod/*.cpp"
        "Own/Obj/*.c" "Own/Obj/*.cpp"
        "Own/OperateSystem/*.c" "Own/OperateSystem/*.cpp"
)

list(APPEND INCLUDE_DIR
        "Core/Inc"
        "USB_DEVICE/App"
        "USB_DEVICE/Target"
        "Drivers/STM32H7xx_HAL_Driver/Inc"
        "Drivers/STM32H7xx_HAL_Driver/Inc/Legacy"
        "Middlewares/Third_Party/FreeRTOS/Source/include"
        "Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS"
        "Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F"
        "Middlewares/ST/STM32_USB_Device_Library/Core/Inc"
        "Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc"
        "Drivers/CMSIS/Device/ST/STM32H7xx/Include"
        "Drivers/CMSIS/Include"
        "MDK-ARM/RTE/_armclang_engineer"

        "D:/Keil_v5_Packs/ARM/CMSIS-Compiler/2.1.0/include"
        "D:/Keil_v5_Packs/ARM/CMSIS/6.1.0/CMSIS/Core/Include"
        "D:/Keil_v5_Packs/Keil/STM32H7xx_DFP/3.1.1/Drivers/CMSIS/Device/ST/STM32H7xx/Include"

        "Own/App"
        "Own/App/Imu/BMI088/inc"
        "Own/Bsp"
        "Own/Mod"
        "Own/Obj"
        "Own/OperateSystem")

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${OWN_SOURCES})

# 添加头文件路径
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

# 添加CMSISDSP库
include(cmake/cmsiscompiler.cmake)
# 添加compiler补丁
include(cmake/cmsisdsp.cmake)

# 为主项目添加编译定义
target_compile_definitions(${PROJECT_NAME} PRIVATE
        USE_HAL_DRIVER
        STM32H723xx
        __UVISION_VERSION=539
        _RTE_
)

# 设置输出文件扩展名
set_target_properties(${PROJECT_NAME} PROPERTIES
        SUFFIX ".axf"
)

# 生成hex和bin文件
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${TOOLCHAIN_PATH}/fromelf.exe --i32combined --output="${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.axf"
        COMMAND ${TOOLCHAIN_PATH}/fromelf.exe --bin --output="${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.axf"
        COMMENT "Building ${PROJECT_NAME}.hex and ${PROJECT_NAME}.bin"
)
# 可视化内存占用

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_SOURCE_DIR}/cac.bat"
)
