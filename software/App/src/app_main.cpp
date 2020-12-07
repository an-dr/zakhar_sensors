// *************************************************************************
//
// Copyright (c) 2020 Andrei Gramakov. All rights reserved.
//
// This file is licensed under the terms of the MIT license.
// For a copy, see: https://opensource.org/licenses/MIT
//
// site:    https://agramakov.me
// e-mail:  mail@agramakov.me
//
// *************************************************************************

#include <cstdio>
#include "FreeRTOS.h"
#include "app_main.hpp"
#include "app_tasks.hpp"
#include "cmsis_os2.h"
#include "gpio.h"
#include "i2c.h"
#include "log.h"
#include "registers.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_def.h"
#include "svr_types.h"
#include "task.h"
#include "usart.h"


void test_task(void *params) {
    while (1) {
        printf("test1\r\n");
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}

void test2_task(void *params) {
    while (1) {
        printf("test2\r\n");
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}


void app_main() {
    HAL_UART_Transmit(&huart1, (uint8_t *) "START\r\n", 8, 10);
    log_info("Start");
    RegistersInit();
    unsigned amount;
    const SVR_reg_t *regs = SVR_get_regs(&registers, &amount);
    // status-

    log_debug("I2C ready? %d", HAL_I2C_GetState(&hi2c1));
    HAL_I2C_EnableListen_IT(&hi2c1);  // https://community.st.com/s/question/0D50X00009sThWGSA0/how-hal-i2c-slave-works
    // log_set_level(LOG_DEBUG);

    xTaskCreate(vTaskPhotoresistor, "vTaskPhotoresistor", 1024, NULL, 5, NULL);
    xTaskCreate(vTaskDistance, "vTaskDistance", 1024, NULL, 5, NULL);

    while (1) {
        log_debug("    \tCMD: 0x%x\tMODE: 0x%x\tD_cm: 0x%x\tLIGHT: 0x%x 0x%x",
                  regs[0], regs[1], regs[2], regs[3], regs[4]);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);  // PORTA, Pin PA5
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}