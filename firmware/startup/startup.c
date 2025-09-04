#include <stdint.h>

#define WEAK_ALIAS_DEFAULT __attribute__((weak, alias("Default_Handler"))) 

// Defined in link.ld
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sidata;
extern uint32_t _estack;


__attribute__((noreturn, section(".text")))
void Reset_Handler(void)
{
    // Zero-initialize .bss
    for (uint32_t *dst = &_sbss; dst < &_ebss; ++dst) *dst = 0;

    // Copy .data from flash to ram
    for (uint32_t *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

    extern int main(void);
    main();

    while(1) (void)0;
}

void Default_Handler(void)
{
    while(1) (void)0;
}


void NMI_Handler(void) WEAK_ALIAS_DEFAULT;
void HardFault_Handler(void) WEAK_ALIAS_DEFAULT;
void MemManage_Handler(void) WEAK_ALIAS_DEFAULT;
void BusFault_Handler(void) WEAK_ALIAS_DEFAULT;
void UsageFault_Handler(void) WEAK_ALIAS_DEFAULT;
void SVCall_Handler(void) WEAK_ALIAS_DEFAULT;
void DebugMonitor_Handler(void) WEAK_ALIAS_DEFAULT;
void PendSV_Handler(void) WEAK_ALIAS_DEFAULT;
void SysTick_Handler(void) WEAK_ALIAS_DEFAULT;
void WWDG_Handler(void) WEAK_ALIAS_DEFAULT;
void PVD_Handler(void) WEAK_ALIAS_DEFAULT;
void TAMP_STAMP_Handler(void) WEAK_ALIAS_DEFAULT;
void RTC_WKUP_Handler(void) WEAK_ALIAS_DEFAULT;
void FLASH_Handler(void) WEAK_ALIAS_DEFAULT;
void RCC_Handler(void) WEAK_ALIAS_DEFAULT;
void EXTI0_Handler(void) WEAK_ALIAS_DEFAULT;
void EXTI1_Handler(void) WEAK_ALIAS_DEFAULT;
void EXTI2_Handler(void) WEAK_ALIAS_DEFAULT;
void EXTI3_Handler(void) WEAK_ALIAS_DEFAULT;
void EXTI4_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA1_Stream0_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA1_Stream1_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA1_Stream2_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA1_Stream3_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA1_Stream4_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA1_Stream5_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA1_Stream6_Handler(void) WEAK_ALIAS_DEFAULT;
void ADC_Handler(void) WEAK_ALIAS_DEFAULT;
void CAN1_TX_Handler(void) WEAK_ALIAS_DEFAULT;
void CAN1_RX0_Handler(void) WEAK_ALIAS_DEFAULT;
void CAN1_RX1_Handler(void) WEAK_ALIAS_DEFAULT;
void Can1_SCE_Handler(void) WEAK_ALIAS_DEFAULT;
void EXTI9_5_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM1_BRK_TIM9_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM1_UP_TIM10_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM1_TRG_COM_TIM11_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM1_CC_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM2_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM3_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM4_Handler(void) WEAK_ALIAS_DEFAULT;
void I2C1_EV_Handler(void) WEAK_ALIAS_DEFAULT;
void I2C1_ER_Handler(void) WEAK_ALIAS_DEFAULT;
void I2C2_EV_Handler(void) WEAK_ALIAS_DEFAULT;
void I2C2_ER_Handler(void) WEAK_ALIAS_DEFAULT;
void SPI1_Handler(void) WEAK_ALIAS_DEFAULT;
void SPI2_Handler(void) WEAK_ALIAS_DEFAULT;
void USART1_Handler(void) WEAK_ALIAS_DEFAULT;
void USART2_Handler(void) WEAK_ALIAS_DEFAULT;
void USART3_Handler(void) WEAK_ALIAS_DEFAULT;
void EXTI15_10_Handler(void) WEAK_ALIAS_DEFAULT;
void RTC_Alarm_Handler(void) WEAK_ALIAS_DEFAULT;
void OTG_FS_WKUP_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM8_BRK_TIM12_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM8_UP_TIM13_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM8_TRG_COM_TIM14_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM8_CC_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA1_Stream7_Handler(void) WEAK_ALIAS_DEFAULT;
void FMC_Handler(void) WEAK_ALIAS_DEFAULT;
void SDIO_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM5_Handler(void) WEAK_ALIAS_DEFAULT;
void SPI3_Handler(void) WEAK_ALIAS_DEFAULT;
void UART4_Handler(void) WEAK_ALIAS_DEFAULT;
void UART5_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM6_DAC_Handler(void) WEAK_ALIAS_DEFAULT;
void TIM7_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA2_Stream0_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA2_Stream1_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA2_Stream2_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA2_Stream3_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA2_Stream4_Handler(void) WEAK_ALIAS_DEFAULT;
void CAN2_TX_Handler(void) WEAK_ALIAS_DEFAULT;
void CAN2_RX0_Handler(void) WEAK_ALIAS_DEFAULT;
void CAN2_RX1_Handler(void) WEAK_ALIAS_DEFAULT;
void CAN2_SCE_Handler(void) WEAK_ALIAS_DEFAULT;
void OTG_FS_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA2_Stream5_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA2_Stream6_Handler(void) WEAK_ALIAS_DEFAULT;
void DMA2_Stream7_Handler(void) WEAK_ALIAS_DEFAULT;
void USART6_Handler(void) WEAK_ALIAS_DEFAULT;
void I2C3_EV_Handler(void) WEAK_ALIAS_DEFAULT;
void I2C3_ER_Handler(void) WEAK_ALIAS_DEFAULT;
void OTG_HS_EP1_OUT_Handler(void) WEAK_ALIAS_DEFAULT;
void OTG_HS_EP1_IN_Handler(void) WEAK_ALIAS_DEFAULT;
void OTG_HS_WKUP_Handler(void) WEAK_ALIAS_DEFAULT;
void OTG_HS_Handler(void) WEAK_ALIAS_DEFAULT;
void DCMI_Handler(void) WEAK_ALIAS_DEFAULT;
void FPU_Handler(void) WEAK_ALIAS_DEFAULT;
void SPI4_Handler(void) WEAK_ALIAS_DEFAULT;
void SAI1_Handler(void) WEAK_ALIAS_DEFAULT;
void SAI2_Handler(void) WEAK_ALIAS_DEFAULT;
void QuadSPI_Handler(void) WEAK_ALIAS_DEFAULT;
void HDMI_CEC_Handler(void) WEAK_ALIAS_DEFAULT;
void SPDIF_Rx_Handler(void) WEAK_ALIAS_DEFAULT;
void FMPI2C1_EV_Handler(void) WEAK_ALIAS_DEFAULT;
void FMPI2C1_ER_Handler(void) WEAK_ALIAS_DEFAULT;


// Vector table
__attribute__((section(".vector_table")))
void(*const vectors[113])(void) = {
    (void (*)(void))(&_estack),
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVCall_Handler,
    DebugMonitor_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    WWDG_Handler,
    PVD_Handler,
    TAMP_STAMP_Handler,
    RTC_WKUP_Handler,
    FLASH_Handler,
    RCC_Handler,
    EXTI0_Handler,
    EXTI1_Handler,
    EXTI2_Handler,
    EXTI3_Handler,
    EXTI4_Handler,
    DMA1_Stream0_Handler,
    DMA1_Stream1_Handler,
    DMA1_Stream2_Handler,
    DMA1_Stream3_Handler,
    DMA1_Stream4_Handler,
    DMA1_Stream5_Handler,
    DMA1_Stream6_Handler,
    ADC_Handler,
    CAN1_TX_Handler,
    CAN1_RX0_Handler,
    CAN1_RX1_Handler,
    Can1_SCE_Handler,
    EXTI9_5_Handler,
    TIM1_BRK_TIM9_Handler,
    TIM1_UP_TIM10_Handler,
    TIM1_TRG_COM_TIM11_Handler,
    TIM1_CC_Handler,
    TIM2_Handler,
    TIM3_Handler,
    TIM4_Handler,
    I2C1_EV_Handler,
    I2C1_ER_Handler,
    I2C2_EV_Handler,
    I2C2_ER_Handler,
    SPI1_Handler,
    SPI2_Handler,
    USART1_Handler,
    USART2_Handler,
    USART3_Handler,
    EXTI15_10_Handler,
    RTC_Alarm_Handler,
    OTG_FS_WKUP_Handler,
    TIM8_BRK_TIM12_Handler,
    TIM8_UP_TIM13_Handler,
    TIM8_TRG_COM_TIM14_Handler,
    TIM8_CC_Handler,
    DMA1_Stream7_Handler,
    FMC_Handler,
    SDIO_Handler,
    TIM5_Handler,
    SPI3_Handler,
    UART4_Handler,
    UART5_Handler,
    TIM6_DAC_Handler,
    TIM7_Handler,
    DMA2_Stream0_Handler,
    DMA2_Stream1_Handler,
    DMA2_Stream2_Handler,
    DMA2_Stream3_Handler,
    DMA2_Stream4_Handler,
    0,
    0,
    CAN2_TX_Handler,
    CAN2_RX0_Handler,
    CAN2_RX1_Handler,
    CAN2_SCE_Handler,
    OTG_FS_Handler,
    DMA2_Stream5_Handler,
    DMA2_Stream6_Handler,
    DMA2_Stream7_Handler,
    USART6_Handler,
    I2C3_EV_Handler,
    I2C3_ER_Handler,
    OTG_HS_EP1_OUT_Handler,
    OTG_HS_EP1_IN_Handler,
    OTG_HS_WKUP_Handler,
    OTG_HS_Handler,
    DCMI_Handler,
    0,
    0,
    FPU_Handler,
    0,
    0,
    SPI4_Handler,
    0,
    0,
    SAI1_Handler,
    0,
    0,
    0,
    SAI2_Handler,
    QuadSPI_Handler,
    HDMI_CEC_Handler,
    SPDIF_Rx_Handler,
    FMPI2C1_EV_Handler,
    FMPI2C1_ER_Handler
};
