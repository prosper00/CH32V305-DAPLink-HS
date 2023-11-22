#include "dap_main.h"
#include "debug.h"
#include "usb2uart.h"

void enable_power_output(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

void USBHS_RCC_Init(void)
{
    RCC_USBCLK48MConfig(RCC_USBCLK48MCLKSource_USBPHY);
    RCC_USBHSPLLCLKConfig(RCC_HSBHSPLLCLKSource_HSE);
    RCC_USBHSConfig(RCC_USBPLL_Div3);
    RCC_USBHSPLLCKREFCLKConfig(RCC_USBHSPLLCKREFCLK_4M);
    RCC_USBHSPHYPLLALIVEcmd(ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USBHS, ENABLE);
}

void usb_dc_low_level_init(void)
{
    USBHS_RCC_Init();
    NVIC_EnableIRQ(USBHS_IRQn);
}

void check_iap_status(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    volatile uint8_t io_status = 0;
    for (uint8_t i = 0; i < 10; i++) {
        io_status += GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
    }
    if (io_status == 0) {
        while (1) {
        }
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    SystemCoreClockUpdate();

    Delay_Init();
    check_iap_status();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    enable_power_output();
    //USART_Printf_Init(115200);

    uartx_preinit();
    chry_dap_init();
    while (!usb_device_is_configured()) {
    }

    //	printf("SystemClk:%d\r\n",SystemCoreClock);
    //	printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    //	printf("This is printf example\r\n");

    while (1) {
        chry_dap_handle();
        chry_dap_usb2uart_handle();
    }
}
