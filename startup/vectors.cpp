#include <stdint.h>

using VectorHandler = void (*)(void);

extern "C" uint32_t _estack;
extern "C" void Reset_Handler() noexcept;
extern "C" void SysTick_Handler() noexcept;

extern "C" void Default_Handler() noexcept
{
    for (;;) {
        __asm volatile ("nop");
    }
}

extern "C" void NMI_Handler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void HardFault_Handler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void MemManage_Handler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void BusFault_Handler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void UsageFault_Handler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void SVC_Handler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DebugMon_Handler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void PendSV_Handler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void WWDG_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void PVD_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TAMP_STAMP_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void RTC_WKUP_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void FLASH_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void RCC_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void EXTI0_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void EXTI1_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void EXTI2_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void EXTI3_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void EXTI4_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA1_Stream0_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA1_Stream1_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA1_Stream2_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA1_Stream3_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA1_Stream4_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA1_Stream5_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA1_Stream6_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void ADC_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void CAN1_TX_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void CAN1_RX0_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void CAN1_RX1_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void CAN1_SCE_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void EXTI9_5_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM1_BRK_TIM9_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM1_UP_TIM10_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM1_TRG_COM_TIM11_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM1_CC_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM2_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM3_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM4_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void I2C1_EV_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void I2C1_ER_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void I2C2_EV_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void I2C2_ER_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void SPI1_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void SPI2_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void USART1_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void USART2_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void USART3_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void EXTI15_10_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void RTC_Alarm_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void OTG_FS_WKUP_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM8_BRK_TIM12_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM8_UP_TIM13_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM8_TRG_COM_TIM14_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM8_CC_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA1_Stream7_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void FSMC_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void SDIO_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM5_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void SPI3_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void UART4_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void UART5_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM6_DAC_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void TIM7_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA2_Stream0_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA2_Stream1_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA2_Stream2_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA2_Stream3_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA2_Stream4_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void ETH_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void ETH_WKUP_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void CAN2_TX_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void CAN2_RX0_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void CAN2_RX1_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void CAN2_SCE_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void OTG_FS_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA2_Stream5_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA2_Stream6_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DMA2_Stream7_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void USART6_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void I2C3_EV_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void I2C3_ER_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void OTG_HS_EP1_OUT_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void OTG_HS_EP1_IN_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void OTG_HS_WKUP_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void OTG_HS_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void DCMI_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void CRYP_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void HASH_RNG_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));
extern "C" void FPU_IRQHandler() noexcept __attribute__((weak, alias("Default_Handler")));

extern "C" __attribute__((section(".vectors"), used))
VectorHandler const vector_table[98] = {
    reinterpret_cast<VectorHandler>(&_estack),
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    SVC_Handler,
    DebugMon_Handler,
    nullptr,
    PendSV_Handler,
    SysTick_Handler,
    WWDG_IRQHandler,
    PVD_IRQHandler,
    TAMP_STAMP_IRQHandler,
    RTC_WKUP_IRQHandler,
    FLASH_IRQHandler,
    RCC_IRQHandler,
    EXTI0_IRQHandler,
    EXTI1_IRQHandler,
    EXTI2_IRQHandler,
    EXTI3_IRQHandler,
    EXTI4_IRQHandler,
    DMA1_Stream0_IRQHandler,
    DMA1_Stream1_IRQHandler,
    DMA1_Stream2_IRQHandler,
    DMA1_Stream3_IRQHandler,
    DMA1_Stream4_IRQHandler,
    DMA1_Stream5_IRQHandler,
    DMA1_Stream6_IRQHandler,
    ADC_IRQHandler,
    CAN1_TX_IRQHandler,
    CAN1_RX0_IRQHandler,
    CAN1_RX1_IRQHandler,
    CAN1_SCE_IRQHandler,
    EXTI9_5_IRQHandler,
    TIM1_BRK_TIM9_IRQHandler,
    TIM1_UP_TIM10_IRQHandler,
    TIM1_TRG_COM_TIM11_IRQHandler,
    TIM1_CC_IRQHandler,
    TIM2_IRQHandler,
    TIM3_IRQHandler,
    TIM4_IRQHandler,
    I2C1_EV_IRQHandler,
    I2C1_ER_IRQHandler,
    I2C2_EV_IRQHandler,
    I2C2_ER_IRQHandler,
    SPI1_IRQHandler,
    SPI2_IRQHandler,
    USART1_IRQHandler,
    USART2_IRQHandler,
    USART3_IRQHandler,
    EXTI15_10_IRQHandler,
    RTC_Alarm_IRQHandler,
    OTG_FS_WKUP_IRQHandler,
    TIM8_BRK_TIM12_IRQHandler,
    TIM8_UP_TIM13_IRQHandler,
    TIM8_TRG_COM_TIM14_IRQHandler,
    TIM8_CC_IRQHandler,
    DMA1_Stream7_IRQHandler,
    FSMC_IRQHandler,
    SDIO_IRQHandler,
    TIM5_IRQHandler,
    SPI3_IRQHandler,
    UART4_IRQHandler,
    UART5_IRQHandler,
    TIM6_DAC_IRQHandler,
    TIM7_IRQHandler,
    DMA2_Stream0_IRQHandler,
    DMA2_Stream1_IRQHandler,
    DMA2_Stream2_IRQHandler,
    DMA2_Stream3_IRQHandler,
    DMA2_Stream4_IRQHandler,
    ETH_IRQHandler,
    ETH_WKUP_IRQHandler,
    CAN2_TX_IRQHandler,
    CAN2_RX0_IRQHandler,
    CAN2_RX1_IRQHandler,
    CAN2_SCE_IRQHandler,
    OTG_FS_IRQHandler,
    DMA2_Stream5_IRQHandler,
    DMA2_Stream6_IRQHandler,
    DMA2_Stream7_IRQHandler,
    USART6_IRQHandler,
    I2C3_EV_IRQHandler,
    I2C3_ER_IRQHandler,
    OTG_HS_EP1_OUT_IRQHandler,
    OTG_HS_EP1_IN_IRQHandler,
    OTG_HS_WKUP_IRQHandler,
    OTG_HS_IRQHandler,
    DCMI_IRQHandler,
    CRYP_IRQHandler,
    HASH_RNG_IRQHandler,
    FPU_IRQHandler
};
