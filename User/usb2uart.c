#include "usb2uart.h"

uint8_t RxBuffer1[UART_DMA_BUF_LEN];
//void USART3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void UART3_CfgInit(uint32_t baudrate, uint8_t stopbits, uint8_t parity);

void DMA_INIT(void)
{
    DMA_InitTypeDef DMA_InitStructure = { 0 };
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    //    DMA_DeInit(DMA1_Channel2);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DATAR); /* USART2->DATAR:0x40004404 */
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    //    DMA_Init(DMA1_Channel2, &DMA_InitStructure);

    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART3->DATAR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)RxBuffer1;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = UART_DMA_BUF_LEN;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel3, ENABLE);
}

void UART3_CfgInit(uint32_t baudrate, uint8_t stopbits, uint8_t parity)
{
    //    __disable_irq();
    USART_InitTypeDef USART_InitStructure = { 0 };

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;

    /* Number of stop bits (0: 1 stop bit; 1: 1.5 stop bits; 2: 2 stop bits). */
    if (stopbits == 1) {
        USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
    } else if (stopbits == 2) {
        USART_InitStructure.USART_StopBits = USART_StopBits_2;
    } else {
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
    }

    /* Check digit (0: None; 1: Odd; 2: Even; 3: Mark; 4: Space); */
    if (parity == 1) {
        USART_InitStructure.USART_Parity = USART_Parity_Odd;
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    } else if (parity == 2) {
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    } else {
        USART_InitStructure.USART_Parity = USART_Parity_No;
    }
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
}

void uartx_preinit(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = { 0 };
    uint16_t dat = dat;

    /* delete contains in ( ... )  */
    /* First set the serial port introduction to output high then close the TE and RE of CTLR1 register (note that USARTx->CTLR1 register setting 9 bits has a limit) */
    /* Note: This operation must be performed, the TX pin otherwise the level will be pulled low */
    GPIO_SetBits(GPIOA, GPIO_Pin_10);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* clear te/re */
    USART3->CTLR1 &= ~(USART_CTLR1_TE | USART_CTLR1_RE);

    /* USART2 Hard configured: */
    /* Configure USART1 Rx (PA3) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure USART2 Tx (PA2) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //    NVIC_InitTypeDef  NVIC_InitStructure = {0};
    //    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    //    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    //    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    //    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    //    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //    NVIC_Init(&NVIC_InitStructure);
    //    USART_ClearFlag( USART3, USART_FLAG_TC );

    DMA_INIT();
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
    /* Enable USART2 */
    UART3_CfgInit(115200, 0, 0);
    USART_Cmd(USART3, ENABLE);
    //    __enable_irq();
}

//void USART3_IRQHandler(void)
//{
//    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//    {
//        chry_ringbuffer_write_byte(&g_uartrx, USART_ReceiveData(USART3));
//    }
//    if(USART_GetFlagStatus(USART3, USART_IT_ORE) != RESET)
//    {
//        USART_ClearFlag(USART3, USART_FLAG_ORE);//���ORE��־λ
//        USART_ReceiveData(USART3);             //�������յ�������
//    }
//}

void chry_dap_usb2uart_uart_config_callback(struct cdc_line_coding *line_coding)
{
    UART3_CfgInit(line_coding->dwDTERate, line_coding->bCharFormat, line_coding->bParityType);
}
