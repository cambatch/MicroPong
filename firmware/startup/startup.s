    .syntax unified
    .cpu cortex-m4
    .thumb

    .extern main

    .extern _sbss
    .extern _ebss
    .extern _sdata
    .extern _edata
    .extern _sidata
    .extern _estack

/* --------------------------------------------------------------------------
 * Vector table
 * -------------------------------------------------------------------------- */
    .section .vector_table, "a", %progbits
    .align  2
    .global vectors
vectors:
    .word   _estack
    .word   Reset_Handler
    .word   NMI_Handler
    .word   HardFault_Handler
    .word   MemManage_Handler
    .word   BusFault_Handler
    .word   UsageFault_Handler
    .word   0
    .word   0
    .word   0
    .word   0
    .word   SVCall_Handler
    .word   DebugMonitor_Handler
    .word   0
    .word   PendSV_Handler
    .word   SysTick_Handler
    .word   WWDG_Handler
    .word   PVD_Handler
    .word   TAMP_STAMP_Handler
    .word   RTC_WKUP_Handler
    .word   FLASH_Handler
    .word   RCC_Handler
    .word   EXTI0_Handler
    .word   EXTI1_Handler
    .word   EXTI2_Handler
    .word   EXTI3_Handler
    .word   EXTI4_Handler
    .word   DMA1_Stream0_Handler
    .word   DMA1_Stream1_Handler
    .word   DMA1_Stream2_Handler
    .word   DMA1_Stream3_Handler
    .word   DMA1_Stream4_Handler
    .word   DMA1_Stream5_Handler
    .word   DMA1_Stream6_Handler
    .word   ADC_Handler
    .word   CAN1_TX_Handler
    .word   CAN1_RX0_Handler
    .word   CAN1_RX1_Handler
    .word   Can1_SCE_Handler
    .word   EXTI9_5_Handler
    .word   TIM1_BRK_TIM9_Handler
    .word   TIM1_UP_TIM10_Handler
    .word   TIM1_TRG_COM_TIM11_Handler
    .word   TIM1_CC_Handler
    .word   TIM2_Handler
    .word   TIM3_Handler
    .word   TIM4_Handler
    .word   I2C1_EV_Handler
    .word   I2C1_ER_Handler
    .word   I2C2_EV_Handler
    .word   I2C2_ER_Handler
    .word   SPI1_Handler
    .word   SPI2_Handler
    .word   USART1_Handler
    .word   USART2_Handler
    .word   USART3_Handler
    .word   EXTI15_10_Handler
    .word   RTC_Alarm_Handler
    .word   OTG_FS_WKUP_Handler
    .word   TIM8_BRK_TIM12_Handler
    .word   TIM8_UP_TIM13_Handler
    .word   TIM8_TRG_COM_TIM14_Handler
    .word   TIM8_CC_Handler
    .word   DMA1_Stream7_Handler
    .word   FMC_Handler
    .word   SDIO_Handler
    .word   TIM5_Handler
    .word   SPI3_Handler
    .word   UART4_Handler
    .word   UART5_Handler
    .word   TIM6_DAC_Handler
    .word   TIM7_Handler
    .word   DMA2_Stream0_Handler
    .word   DMA2_Stream1_Handler
    .word   DMA2_Stream2_Handler
    .word   DMA2_Stream3_Handler
    .word   DMA2_Stream4_Handler
    .word   0
    .word   0
    .word   CAN2_TX_Handler
    .word   CAN2_RX0_Handler
    .word   CAN2_RX1_Handler
    .word   CAN2_SCE_Handler
    .word   OTG_FS_Handler
    .word   DMA2_Stream5_Handler
    .word   DMA2_Stream6_Handler
    .word   DMA2_Stream7_Handler
    .word   USART6_Handler
    .word   I2C3_EV_Handler
    .word   I2C3_ER_Handler
    .word   OTG_HS_EP1_OUT_Handler
    .word   OTG_HS_EP1_IN_Handler
    .word   OTG_HS_WKUP_Handler
    .word   OTG_HS_Handler
    .word   DCMI_Handler
    .word   0
    .word   0
    .word   FPU_Handler
    .word   0
    .word   0
    .word   SPI4_Handler
    .word   0
    .word   0
    .word   SAI1_Handler
    .word   0
    .word   0
    .word   0
    .word   SAI2_Handler
    .word   QuadSPI_Handler
    .word   HDMI_CEC_Handler
    .word   SPDIF_Rx_Handler
    .word   FMPI2C1_EV_Handler
    .word   FMPI2C1_ER_Handler

/* --------------------------------------------------------------------------
 * Reset_Handler: zero .bss, copy .data, call main, then loop
 * -------------------------------------------------------------------------- */
    .text
    .align  2
    .thumb
    .thumb_func
    .global Reset_Handler
Reset_Handler:
    /* Zero-initialize .bss */
    ldr     r0, =_sbss
    ldr     r1, =_ebss
    movs    r2, #0
1:
    cmp     r0, r1
    bcc     2f
    b       3f
2:
    str     r2, [r0], #4
    b       1b

3:  /* Copy .data */
    ldr     r0, =_sdata
    ldr     r1, =_edata
    ldr     r2, =_sidata
4:
    cmp     r0, r1
    bcc     5f
    b       6f
5:
    ldr     r3, [r2], #4
    str     r3, [r0], #4
    b       4b

6:  /* Call main(); */
    bl      main

    /* If main returns, loop forever */
7:
    b       7b

/* --------------------------------------------------------------------------
 * Default_Handler and weak aliases for all ISRs
 * -------------------------------------------------------------------------- */
    .thumb
    .thumb_func
    .global Default_Handler
Default_Handler:
    b       Default_Handler

    /* Weak aliases: each ISR resolves to Default_Handler unless defined elsewhere */
    .macro WEAK_ALIAS name
        .weak   \name
        .thumb_set \name, Default_Handler
    .endm

    WEAK_ALIAS NMI_Handler
    WEAK_ALIAS HardFault_Handler
    WEAK_ALIAS MemManage_Handler
    WEAK_ALIAS BusFault_Handler
    WEAK_ALIAS UsageFault_Handler
    WEAK_ALIAS SVCall_Handler
    WEAK_ALIAS DebugMonitor_Handler
    WEAK_ALIAS PendSV_Handler
    WEAK_ALIAS SysTick_Handler
    WEAK_ALIAS WWDG_Handler
    WEAK_ALIAS PVD_Handler
    WEAK_ALIAS TAMP_STAMP_Handler
    WEAK_ALIAS RTC_WKUP_Handler
    WEAK_ALIAS FLASH_Handler
    WEAK_ALIAS RCC_Handler
    WEAK_ALIAS EXTI0_Handler
    WEAK_ALIAS EXTI1_Handler
    WEAK_ALIAS EXTI2_Handler
    WEAK_ALIAS EXTI3_Handler
    WEAK_ALIAS EXTI4_Handler
    WEAK_ALIAS DMA1_Stream0_Handler
    WEAK_ALIAS DMA1_Stream1_Handler
    WEAK_ALIAS DMA1_Stream2_Handler
    WEAK_ALIAS DMA1_Stream3_Handler
    WEAK_ALIAS DMA1_Stream4_Handler
    WEAK_ALIAS DMA1_Stream5_Handler
    WEAK_ALIAS DMA1_Stream6_Handler
    WEAK_ALIAS ADC_Handler
    WEAK_ALIAS CAN1_TX_Handler
    WEAK_ALIAS CAN1_RX0_Handler
    WEAK_ALIAS CAN1_RX1_Handler
    WEAK_ALIAS Can1_SCE_Handler
    WEAK_ALIAS EXTI9_5_Handler
    WEAK_ALIAS TIM1_BRK_TIM9_Handler
    WEAK_ALIAS TIM1_UP_TIM10_Handler
    WEAK_ALIAS TIM1_TRG_COM_TIM11_Handler
    WEAK_ALIAS TIM1_CC_Handler
    WEAK_ALIAS TIM2_Handler
    WEAK_ALIAS TIM3_Handler
    WEAK_ALIAS TIM4_Handler
    WEAK_ALIAS I2C1_EV_Handler
    WEAK_ALIAS I2C1_ER_Handler
    WEAK_ALIAS I2C2_EV_Handler
    WEAK_ALIAS I2C2_ER_Handler
    WEAK_ALIAS SPI1_Handler
    WEAK_ALIAS SPI2_Handler
    WEAK_ALIAS USART1_Handler
    WEAK_ALIAS USART2_Handler
    WEAK_ALIAS USART3_Handler
    WEAK_ALIAS EXTI15_10_Handler
    WEAK_ALIAS RTC_Alarm_Handler
    WEAK_ALIAS OTG_FS_WKUP_Handler
    WEAK_ALIAS TIM8_BRK_TIM12_Handler
    WEAK_ALIAS TIM8_UP_TIM13_Handler
    WEAK_ALIAS TIM8_TRG_COM_TIM14_Handler
    WEAK_ALIAS TIM8_CC_Handler
    WEAK_ALIAS DMA1_Stream7_Handler
    WEAK_ALIAS FMC_Handler
    WEAK_ALIAS SDIO_Handler
    WEAK_ALIAS TIM5_Handler
    WEAK_ALIAS SPI3_Handler
    WEAK_ALIAS UART4_Handler
    WEAK_ALIAS UART5_Handler
    WEAK_ALIAS TIM6_DAC_Handler
    WEAK_ALIAS TIM7_Handler
    WEAK_ALIAS DMA2_Stream0_Handler
    WEAK_ALIAS DMA2_Stream1_Handler
    WEAK_ALIAS DMA2_Stream2_Handler
    WEAK_ALIAS DMA2_Stream3_Handler
    WEAK_ALIAS DMA2_Stream4_Handler
    WEAK_ALIAS CAN2_TX_Handler
    WEAK_ALIAS CAN2_RX0_Handler
    WEAK_ALIAS CAN2_RX1_Handler
    WEAK_ALIAS CAN2_SCE_Handler
    WEAK_ALIAS OTG_FS_Handler
    WEAK_ALIAS DMA2_Stream5_Handler
    WEAK_ALIAS DMA2_Stream6_Handler
    WEAK_ALIAS DMA2_Stream7_Handler
    WEAK_ALIAS USART6_Handler
    WEAK_ALIAS I2C3_EV_Handler
    WEAK_ALIAS I2C3_ER_Handler
    WEAK_ALIAS OTG_HS_EP1_OUT_Handler
    WEAK_ALIAS OTG_HS_EP1_IN_Handler
    WEAK_ALIAS OTG_HS_WKUP_Handler
    WEAK_ALIAS OTG_HS_Handler
    WEAK_ALIAS DCMI_Handler
    WEAK_ALIAS FPU_Handler
    WEAK_ALIAS SPI4_Handler
    WEAK_ALIAS SAI1_Handler
    WEAK_ALIAS SAI2_Handler
    WEAK_ALIAS QuadSPI_Handler
    WEAK_ALIAS HDMI_CEC_Handler
    WEAK_ALIAS SPDIF_Rx_Handler
    WEAK_ALIAS FMPI2C1_EV_Handler
    WEAK_ALIAS FMPI2C1_ER_Handler
