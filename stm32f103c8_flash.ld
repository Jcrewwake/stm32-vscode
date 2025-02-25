/* 定义程序入口点为 Reset_Handler（在启动文件中实现） */
ENTRY(Reset_Handler)

/* 定义存储区域：Flash 和 RAM 的地址和大小 */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K   /* Flash: 64KB */
  RAM (xrw)   : ORIGIN = 0x20000000, LENGTH = 20K   /* RAM: 20KB */
}

/* 定义堆和栈的最小大小 */
_Min_Heap_Size = 0x200;   /* 最小堆大小 512B */
_Min_Stack_Size = 0x400;  /* 最小栈大小 1KB */

/* 定义程序段如何分配到存储区域 */
SECTIONS
{
  /* 1. 中断向量表 */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >FLASH

  /* 2. 代码段（.text） */
  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)
    KEEP(*(.init))
    KEEP(*(.fini))
    . = ALIGN(4);
    _etext = .;
  } >FLASH

  /* 3. 只读数据段（.rodata） */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  /* 4. 初始化数据段（.data） */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } >RAM AT >FLASH
  _sidata = LOADADDR(.data);

  /* 5. 未初始化数据段（.bss） */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } >RAM

  /* 6. 定义堆的起始地址（_end）和栈顶地址（_estack） */
  . = ALIGN(4);
  _end = .;              /* 堆起始地址（紧接.bss段之后） */
  . = . + _Min_Heap_Size;/* 保留堆空间 */
  . = ALIGN(4);

  /* 7. 栈段 */
  .stack (NOLOAD) :
  {
    . = ALIGN(8);
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
    _estack = .;         /* 栈顶地址（位于RAM末尾） */
  } >RAM

  /* 其他可选段 */
  /DISCARD/ :
  {
    *(.ARM.exidx*)
  }
}