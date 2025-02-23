/* 定义程序入口点为 Reset_Handler（在启动文件中实现） */
ENTRY(Reset_Handler)

/* 定义存储区域：Flash 和 RAM 的地址和大小 */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K   /* Flash: 64KB */
  RAM (xrw)   : ORIGIN = 0x20000000, LENGTH = 20K   /* RAM: 20KB */
}

/* 定义程序段如何分配到存储区域 */
SECTIONS
{
  /* 1. 中断向量表必须放在 Flash 起始地址 */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))  /* 保留中断向量表 */
    . = ALIGN(4);
  } >FLASH

  /* 2. 代码段（.text） */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* 主程序代码 */
    *(.text*)          /* 其他代码（如库函数） */
    *(.glue_7)         /* Thumb-2 与 ARM 代码粘合段 */
    *(.glue_7t)
    *(.eh_frame)       /* 异常处理框架（某些工具链需要） */
    KEEP(*(.init))     /* 初始化代码 */
    KEEP(*(.fini))     /* 终止代码 */
    . = ALIGN(4);
    _etext = .;        /* 代码段结束地址，用于初始化数据复制 */
  } >FLASH

  /* 3. 只读数据段（.rodata） */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* 常量数据（如字符串、配置表） */
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  /* 4. 初始化数据段（.data）：存储在 Flash，上电后复制到 RAM */
  /* 修改后的链接脚本片段 */
  .data :
  {
  . = ALIGN(4);
  _sdata = .;          /* .data 段在 RAM 中的起始地址 */
  *(.data)             /* 初始化数据 */
  *(.data*)
  . = ALIGN(4);
  _edata = .;          /* .data 段在 RAM 中的结束地址 */
  } >RAM AT >FLASH       /* 物理存储在 Flash，运行时在 RAM */
  
  /* 定义 _sidata 为 Flash 中 .data 段的加载地址 */
  _sidata = LOADADDR(.data);

  /* 5. 未初始化数据段（.bss）：清零处理 */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;         /* .bss 段起始地址 */
    *(.bss)            /* 未初始化全局变量 */
    *(.bss*)
    *(COMMON)          /* 公共符号（如未初始化的全局变量） */
    . = ALIGN(4);
    _ebss = .;         /* .bss 段结束地址 */
  } >RAM

  /* 6. 堆栈段 */
  ._stack (NOLOAD) :
  {
    . = ALIGN(8);
    . = . + 0x400;     /* 定义堆栈大小（例如 1KB） */
    . = ALIGN(8);
    _estack = .;       /* 堆栈顶部地址 */
  } >RAM

  /* 其他可选段（如调试信息） */
  /DISCARD/ :
  {
    *(.ARM.exidx*)     /* 丢弃 ARM 异常索引表（若不需要 C++ 异常处理） */
  }
}