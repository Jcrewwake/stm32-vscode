cmake_minimum_required(VERSION 3.20)

# 强制启用工具链配置（避免回退到主机编译器）
if (CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
else()
    message(FATAL_ERROR "Toolchain file not specified!")
endif()
project(MY_PID LANGUAGES C ASM)

# 工具链配置
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)

# MCU 配置
set(MCU_OPTIONS
    -mcpu=cortex-m3
    -mthumb
    -mfloat-abi=soft
)

# 编译选项
add_compile_options(
    ${MCU_OPTIONS}
    -Os
    -g3
    -Wall
    -fdata-sections
    -ffunction-sections
    -DUSE_STDPERIPH_DRIVER
    -DSTM32F10X_MD
)

# 包含目录
include_directories(
    User/
    Library/
    Start/
    System/
    Hardware/
)

# 源文件集合
file(GLOB_RECURSE SOURCES
    User/*.c
    Start/*.c
    System/*.c
    Hardware/*.c
    Library/*.c
)

# 添加汇编启动文件
set(ASSEMBLY_SOURCES
    Start/startup_stm32f10x_md.s
)

# 创建可执行目标
add_executable(${PROJECT_NAME}.elf
    ${SOURCES}
    ${ASSEMBLY_SOURCES}
)

# 链接库
target_link_libraries(${PROJECT_NAME}.elf PRIVATE m)
# 链接选项配置
target_link_options(${PROJECT_NAME}.elf PRIVATE
    ${MCU_OPTIONS}
    -T${CMAKE_SOURCE_DIR}/stm32f103c8_flash.ld
    -Wl,--gc-sections
    -specs=nano.specs
    -specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -Wl,--end-group
    -Wl,-u,_printf_float
)

# 生成 HEX/BIN 文件
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary -S ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMENT "Generating HEX and BIN files"
)

# 显示尺寸信息
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
    COMMENT "Size analysis:"
)

# 清理配置
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES
    "${CMAKE_SOURCE_DIR}/*.hex"
    "${CMAKE_SOURCE_DIR}/*.bin"
)