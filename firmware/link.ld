

ENTRY(Reset_Handler)


MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512k
  RAM   (rwx): ORIGIN = 0x20000000, LENGTH = 128k
}


/* End of RAM memory */
_estack = ORIGIN(RAM) + LENGTH(RAM);


SECTIONS
{
  .vector_table :
  {
    . = ALIGN(4);
    KEEP(*(.vector_table))
    . = ALIGN(4);
  } >FLASH

  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    . = ALIGN(4);
  } >FLASH

  .rodata :
  {
    . = ALIGN(4);
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  .data :
  {
    . = ALIGN(4);
    _sdata = .; /* .data section start */
    *(.first_data)
    *(.data)
    *(SORT(.data.*))
    . = ALIGN(4);
    _edata = .; /* .data section end */
  } >RAM AT> FLASH

  _sidata = LOADADDR(.data);

  .bss :
  {
    _sbss = .; /* .bss section start */
    *(.bss)
    *(SORT(.bss.*) COMMON)
    _ebss = .; /* .bss section end */
  } >RAM

  . = ALIGN(8);
  _end = .;
}
