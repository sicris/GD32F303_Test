/*
 * (C) Copyright 2020
 * Zimplistic Private Limited
 * Sicris Rey Embay, sicris@zimplistic.com
 */

#include "autoconf.h"
#include "FreeRTOS.h"
#include "task.h"
#include "gd32f30x_libopt.h"
#include <stdio.h>

#define STRBUFSIZ	(32)

static TaskHandle_t xTaskHandleTest = NULL;

static void _SendStrToUart(uint8_t * buf, uint32_t bufSize)
{
	uint32_t idx;

	if((uint8_t *)0 != buf) {
		for(idx = 0; idx < bufSize; idx++) {
			usart_data_transmit(USART1, buf[idx]);
			while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
			if(buf[idx] == '\0') {
				break;
			}
		}
	}
}

static void vTaskUartTest(void *pvParam)
{
	TickType_t xPrevWakeTime;
	uint8_t strBuf[STRBUFSIZ];
	(void)pvParam;  // unused

    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOD);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);

    rcu_periph_clock_enable(RCU_AF);

    /* connect port to UART1_Tx */
    gpio_init(GPIOD, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);

    /* connect port to USART1_Rx */
    gpio_init(GPIOD, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    gpio_pin_remap_config(GPIO_USART1_REMAP, ENABLE);

    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1, 115200U);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_enable(USART1);

    snprintf((char *)strBuf, STRBUFSIZ - 1, "This is a simple test!\n\n\r");
	strBuf[STRBUFSIZ-1] = '\0';
	_SendStrToUart(strBuf, STRBUFSIZ);

    xPrevWakeTime = xTaskGetTickCount();

	while(1) {
#if defined(CONFIG_FOO)
		snprintf((char *)strBuf, STRBUFSIZ - 1, "FOO: Tick: %d\n\r", (int)xPrevWakeTime);
#elif defined(CONFIG_BAR)
        snprintf((char *)strBuf, STRBUFSIZ - 1, "BAR: Tick: %d\n\r", (int)xPrevWakeTime);
#else
#error ("Invalid Foo-Bar");
#endif
		strBuf[STRBUFSIZ-1] = '\0';
		_SendStrToUart(strBuf, STRBUFSIZ);
		vTaskDelayUntil(&xPrevWakeTime, 1000);
	}
}

int main (void)
{
	if(pdPASS == xTaskCreate(vTaskUartTest, /* Function that implements the task. */
				"Test",                 /* Text name for the task. */
				128,                    /* Stack size in words, not bytes. */
				( void * ) 0,           /* Parameter passed into the task. */
				tskIDLE_PRIORITY + 1,   /* Priority at which the task is created. */
	            &xTaskHandleTest )) {     /* Used to pass out the created task's handle. */
        vTaskStartScheduler();
    }

    /* should not reach here */
    return 0;
}

