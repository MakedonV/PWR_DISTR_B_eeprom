/*
** ###################################################################
**     Processor:           S32K144 with 64 KB SRAM
**     Compiler:            GNU C Compiler
**
**     Abstract:
**         Linker file for the GNU C Compiler
**
**     Copyright (c) 2015-2016 Freescale Semiconductor, Inc.
**     Copyright 2017 NXP
**     All rights reserved.
**
**     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
**     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
**     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
**     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
**     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
**     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**     THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
** ###################################################################
*/

/* Entry Point */
ENTRY(Reset_Handler)

HEAP_SIZE  = DEFINED(__heap_size__)  ? __heap_size__  : 0x00000400;
STACK_SIZE = DEFINED(__stack_size__) ? __stack_size__ : 0x00001000;

__APP_HEADER_ADDR   = 0x10000; /* Bootloader can be up to 64 KB */
__APP_HEADER_SIZE   = 0x1000;
__EE_RO_FLASH_ADDR  	= 0xF000;  		/* Last part of bootloader is reserved for EEPROM Read-only fields */
__EE_CALIBR_FLASH_ADDR 	= 0xF200;		/* Last part of bootloader is also reserved to keep the calibration values*/

/* If symbol __flash_vector_table__=1 is defined at link time
 * the interrupt vector will not be copied to RAM.
 * Warning: Using the interrupt vector from Flash will not allow
 * INT_SYS_InstallHandler because the section is Read Only.
 */
M_VECTOR_RAM_SIZE = DEFINED(__flash_vector_table__) ? 0x0 : 0x0400;

/* Specify the memory areas */

/* NOTE: According to the datasheet, everytime the flash memory is depicted its
   size is 512KB (atleast for the S32K144).However, calculating the actual
   addresses, also found in the datasheets, results in actual more bytes of
   usable space.
   see: https://www.nxp.com/docs/en/application-note/AN11983.pdf page 2 */
MEMORY
{
  /* Flash (524KB) refer to NOTE obove*/
  m_text_bootloader     (R)   : ORIGIN = 0x00000000, LENGTH = 0x00010000 /* (64KB) definition only used for metric analysis */
  m_text_header         (RX)  : ORIGIN = 0x00010000, LENGTH = 0x00001000
  m_interrupts          (RX)  : ORIGIN = 0x00011000, LENGTH = 0x00000400
  m_flash_config        (RX)  : ORIGIN = 0x00011400, LENGTH = 0x00000010
  m_text                (RX)  : ORIGIN = 0x00011410, LENGTH = 0x0006EBF0

  /* FlexRAM (4KB) used for EEPROM*/
  m_flexram             (RW)  : ORIGIN = 0x14000000, LENGTH = 0x00001000

  /* SRAM_L (32KB)*/
  m_data                (RW)  : ORIGIN = 0x1FFF8000, LENGTH = 0x00008000

  /* SRAM_U (28KB)*/
  m_data_2              (RW)  : ORIGIN = 0x20000000, LENGTH = 0x00007000

  /* FlexNVM (64KB) used for EEPROM emulation */
}

/* Define output sections */
SECTIONS
{
  /* Presumably the application will start at flash address 0x10000 + X (64KB + X)*/
  /* where X will be the size of APP_HEADER section */
  /* at address __APP_HEADER_ADDR; */
  .APP_HEADER __APP_HEADER_ADDR :
  {
  	KEEP(*(.APP_HEADER))
  	. = ALIGN(__APP_HEADER_SIZE);
  } > m_text_header

  /* The startup code goes first into internal flash */
  .interrupts :
  {
    __VECTOR_TABLE = .;
   __interrupts_start__ = .;
    . = ALIGN(4);
    KEEP(*(.isr_vector))     /* Startup code */
    __interrupts_end__ = .;
    . = ALIGN(4);
  } > m_interrupts

  .flash_config :
  {
    . = ALIGN(4);
    KEEP(*(.FlashConfig))    /* Flash Configuration Field (FCF) */
    . = ALIGN(4);
  } > m_flash_config

  /* The program code and other data goes into internal flash */
  .text :
  {
    . = ALIGN(4);
    *(.text)                 /* .text sections (code) */
    *(.text*)                /* .text* sections (code) */
    *(.rodata)               /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)              /* .rodata* sections (constants, strings, etc.) */
    *(.glue_7)               /* glue arm to thumb code */
    *(.glue_7t)              /* glue thumb to arm code */
    *(.eh_frame)
    KEEP (*(.init))
    KEEP (*(.fini))
    . = ALIGN(4);
  } > m_text

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > m_text

  .ARM :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } > m_text

 .ctors :
  {
    __CTOR_LIST__ = .;
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       from the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    __CTOR_END__ = .;
  } > m_text

  .dtors :
  {
    __DTOR_LIST__ = .;
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
    __DTOR_END__ = .;
  } > m_text

  .preinit_array :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } > m_text

  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > m_text

  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > m_text

  __etext = .;    /* Define a global symbol at end of code. */
  __DATA_ROM = .; /* Symbol is used by startup for data initialization. */
  .interrupts_ram :
  {
    . = ALIGN(4);
    __VECTOR_RAM__ = .;
    __interrupts_ram_start__ = .; /* Create a global symbol at data start. */
    *(.m_interrupts_ram)          /* This is a user defined section. */
    . += M_VECTOR_RAM_SIZE;
    . = ALIGN(4);
    __interrupts_ram_end__ = .;   /* Define a global symbol at data end. */
  } > m_data

  __VECTOR_RAM = DEFINED(__flash_vector_table__) ? ORIGIN(m_interrupts) : __VECTOR_RAM__ ;
  __RAM_VECTOR_TABLE_SIZE = DEFINED(__flash_vector_table__) ? 0x0 : (__interrupts_ram_end__ - __interrupts_ram_start__) ;

  .data : AT(__DATA_ROM)
  {
    . = ALIGN(4);
    __DATA_RAM = .;
    __data_start__ = .;      /* Create a global symbol at data start. */
    *(.data)                 /* .data sections */
    *(.data*)                /* .data* sections */
    KEEP(*(.jcr*))
    . = ALIGN(4);
    __data_end__ = .;        /* Define a global symbol at data end. */
  } > m_data

  __DATA_END = __DATA_ROM + (__data_end__ - __data_start__);
  __CODE_ROM = __DATA_END; /* Symbol is used by code initialization. */
  .code : AT(__CODE_ROM)
  {
    . = ALIGN(4);
    __CODE_RAM = .;
    __code_start__ = .;      /* Create a global symbol at code start. */
    *(.code_ram)             /* Custom section for storing code in RAM */
    . = ALIGN(4);
    __code_end__ = .;        /* Define a global symbol at code end. */
  } > m_data

  __CODE_END = __CODE_ROM + (__code_end__ - __code_start__);
  __CUSTOM_ROM = __CODE_END;
 
  /* Uninitialized data section for specific MRS internal data structures. */
  .bss_mrs : 
  {
    /* This is used by the startup in order to initialize the .bss_mrs section. */
    . = ALIGN(4);
    __BSS_MRS_START = .;
    __bss_mrs_start__ = .;
    *(.bss_mrs)
    *(.bss_mrs*)
    *(COMMON)
    . = ALIGN(4);
    __bss_mrs_end__ = .;
    __BSS_MRS_END = .;
  } > m_data
 
  /* Custom Section Block that can be used to place data at absolute address. */
  /* Use __attribute__((section (".BL_ACCESS_KEY"))) to place data here. */
  .BL_ACCESS_KEY ORIGIN(m_data_2) (NOLOAD):
  {
    __customSection_start__ = .;
  	KEEP(*(.BL_ACCESS_KEY))
    __customSection_end__ = .;
  } > m_data_2

  __CUSTOM_END = __CUSTOM_ROM + (__customSection_end__ - __customSection_start__);

  /* Uninitialized data section. */
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss section. */
    . = ALIGN(4);
    __BSS_START = .;
    __bss_start__ = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    __BSS_END = .;
  } > m_data_2

  .heap :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    PROVIDE(_end = .);
    PROVIDE(__end = .);
    __HeapBase = .;
    . += HEAP_SIZE;
    __HeapLimit = .;
    __heap_limit = .;
  } > m_data_2

  .stack :
  {
    . = ALIGN(8);
    . += STACK_SIZE;
  } > m_data_2

  /* Initializes stack on the end of block */
  __StackTop   = ORIGIN(m_data_2) + LENGTH(m_data_2);
  __StackLimit = __StackTop - STACK_SIZE;
  PROVIDE(__stack = __StackTop);

  .ARM.attributes 0 : { *(.ARM.attributes) }

  ASSERT(__StackLimit >= __HeapLimit, "region m_data_2 overflowed with stack and heap")

  /* ************************************************************************************ */
  /* ************************************************************************************ */
  /* ************************************************************************************ */
  /* Set some addresses/values for metrical analysis.
     How to read/interprete the addresses/values:
     .METRIC_xxx_size           Read this value as an "amount",  e.g. 0001fbbc = 129980[byte] (available bytes of RAM/ROM).
     .METRIC_xxx_used           Read this value as an "amount",  e.g. 0000a580 = 42368[byte] (used bytes of RAM/ROM).
  */

  .METRIC_FLASH_size              =   LENGTH(m_interrupts) + LENGTH(m_flash_config) + LENGTH(m_text_header) + LENGTH(m_text) + LENGTH(m_text_bootloader);
  .METRIC_FLASH_used              =   SIZEOF(.APP_HEADER) + SIZEOF(.interrupts) + SIZEOF(.flash_config) + SIZEOF(.text) + SIZEOF(.ARM.extab) + SIZEOF(.ARM) + SIZEOF(.ctors) + SIZEOF(.dtors) + SIZEOF(.preinit_array) + SIZEOF(.init_array) + SIZEOF(.init_array) + LENGTH(m_text_bootloader);

  .METRIC_RAM_size              =   LENGTH(m_data) + LENGTH(m_data_2);
  .METRIC_RAM_used              =   SIZEOF(.interrupts_ram) + SIZEOF(.data) + SIZEOF(.code) + SIZEOF(.BL_ACCESS_KEY) + SIZEOF(.bss) + SIZEOF(.heap) + SIZEOF(.stack) + SIZEOF(.bss_mrs);
}

