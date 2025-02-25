# 工具链配置
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size

# 目标配置
TARGET = MY_PID
MCU = -mcpu=cortex-m3 -mthumb
CFLAGS = $(MCU) -O0 -g3 -Wall -fdata-sections -ffunction-sections
CFLAGS += -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD
CFLAGS += -IUser/ -ILibrary/ -IStart/ -ISystem/ -IHardware/ -IStart/

# 链接脚本
LDSCRIPT = stm32f103c8_flash.ld
LDFLAGS = $(MCU) -T$(LDSCRIPT) -Wl,--gc-sections -specs=nano.specs
LDFLAGS += -Wl,--start-group -lc -lm -Wl,--end-group

# 源文件列表
SRCS = \
	User/main.c \
	Start/system_stm32f10x.c \
	Library/stm32f10x_gpio.c \
	Library/stm32f10x_rcc.c \
	Library/stm32f10x_usart.c \
	Library/stm32f10x_tim.c \
	Library/stm32f10x_adc.c \
	Library/misc.c \
	System/Delay.c \
	System/Timer.c \
	Hardware/Key.c \
	Hardware/RP.c \
	Hardware/LED.c \
    Hardware/syscalls.c \
	Hardware/OLED.c \
	Hardware/OLED_Data.c \
	Start/startup_stm32f10x_md.s

# 对象文件生成规则
OBJS = $(addprefix Objects/,$(notdir $(SRCS:.c=.o)))
OBJS := $(OBJS:.s=.o)

# 构建目标
all: $(TARGET).elf $(TARGET).hex $(TARGET).bin

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

Objects/%.o: User/%.c
	$(CC) -c $(CFLAGS) $< -o $@

Objects/%.o: System/%.c
	$(CC) -c $(CFLAGS) $< -o $@

Objects/%.o: Hardware/%.c
	$(CC) -c $(CFLAGS) $< -o $@

Objects/%.o: Library/%.c
	$(CC) -c $(CFLAGS) $< -o $@

Objects/%.o: Start/%.c
	$(CC) -c $(CFLAGS) $< -o $@

Objects/%.o: Start/%.s
	$(CC) -c $(MCU) -x assembler-with-cpp $< -o $@

%.hex: %.elf
	$(OBJCOPY) -O ihex $< $@

%.bin: %.elf
	$(OBJCOPY) -O binary -S $< $@

clean:
	rm -f Objects/*.o
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).bin

# 依赖关系（通过gcc自动生成）
DEPS = $(OBJS:.o=.d)
-include $(DEPS)

.PHONY: all clean