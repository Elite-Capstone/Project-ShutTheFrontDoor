/*
    Project:    ShutTheFrontDoor
    File:       stfd_gpio_config.h
    Author:     Benjamin Lanthier
    Date:       06/03/2021

    Desc:       This file contains the gpio setup and gpio assignment 
                error detection for both input/output
                Sets up the following GPIOs:
                    GPIO_INPUT_BOOT             (Boot button)
                    GPIO_INPUT_MS               (Motion Sensor)
                    GPIO_INPUT_MOTOR_FAULT      (Motor fault current detection)
                    GPIO_INPUT_NSW              (N-switch)
                    GPIO_INPUT_BATTERY
                    GPIO_INPUT_DRBELL_NOTIF     (Doorbell button press)
                    GPIO_INPUT_REEDSW_STATUS    (Reed switch current status)

                    GPIO_OUTPUT_MOTOR_IN1       (Clockwise rotation output)
                    GPIO_OUTPUT_MOTOR_IN2       (Counter-clockwise rotation output)
*/

#include <string.h>

//===========================================
//
//              INPUT CONFIG
//
//===========================================

#if CONFIG_MAIN_MCU

    #if CONFIG_GPIO_INPUT_0_BOOT
    #define GPIO_INPUT_BOOT 0
    #elif CONFIG_GPIO_INPUT_2_BOOT
    #define GPIO_INPUT_BOOT 2
    #elif CONFIG_GPIO_INPUT_4_BOOT
    #define GPIO_INPUT_BOOT 4
    #elif CONFIG_GPIO_INPUT_5_BOOT
    #define GPIO_INPUT_BOOT 5
    #elif CONFIG_GPIO_INPUT_6_BOOT
    #define GPIO_INPUT_BOOT 6
    #elif CONFIG_GPIO_INPUT_7_BOOT
    #define GPIO_INPUT_BOOT 7
    #elif CONFIG_GPIO_INPUT_8_BOOT
    #define GPIO_INPUT_BOOT 8
    #elif CONFIG_GPIO_INPUT_9_BOOT
    #define GPIO_INPUT_BOOT 9
    #elif CONFIG_GPIO_INPUT_10_BOOT
    #define GPIO_INPUT_BOOT 10
    #elif CONFIG_GPIO_INPUT_11_BOOT
    #define GPIO_INPUT_BOOT 11
    #elif CONFIG_GPIO_INPUT_12_BOOT
    #define GPIO_INPUT_BOOT 12
    #elif CONFIG_GPIO_INPUT_13_BOOT
    #define GPIO_INPUT_BOOT 13
    #elif CONFIG_GPIO_INPUT_14_BOOT
    #define GPIO_INPUT_BOOT 14
    #elif CONFIG_GPIO_INPUT_15_BOOT
    #define GPIO_INPUT_BOOT 15
    #elif CONFIG_GPIO_INPUT_16_BOOT
    #define GPIO_INPUT_BOOT 16
    #elif CONFIG_GPIO_INPUT_17_BOOT
    #define GPIO_INPUT_BOOT 17
    #elif CONFIG_GPIO_INPUT_18_BOOT
    #define GPIO_INPUT_BOOT 18
    #elif CONFIG_GPIO_INPUT_19_BOOT
    #define GPIO_INPUT_BOOT 19
    #elif CONFIG_GPIO_INPUT_21_BOOT
    #define GPIO_INPUT_BOOT 21
    #elif CONFIG_GPIO_INPUT_22_BOOT
    #define GPIO_INPUT_BOOT 22
    #elif CONFIG_GPIO_INPUT_23_BOOT
    #define GPIO_INPUT_BOOT 23
    #elif CONFIG_GPIO_INPUT_25_BOOT
    #define GPIO_INPUT_BOOT 25
    #elif CONFIG_GPIO_INPUT_26_BOOT
    #define GPIO_INPUT_BOOT 26
    #elif CONFIG_GPIO_INPUT_27_BOOT
    #define GPIO_INPUT_BOOT 27
    #elif CONFIG_GPIO_INPUT_32_BOOT
    #define GPIO_INPUT_BOOT 32
    #elif CONFIG_GPIO_INPUT_33_BOOT
    #define GPIO_INPUT_BOOT 33
    #elif CONFIG_GPIO_INPUT_34_BOOT
    #define GPIO_INPUT_BOOT 34
    #elif CONFIG_GPIO_INPUT_35_BOOT
    #define GPIO_INPUT_BOOT 35
    #elif CONFIG_GPIO_INPUT_36_BOOT
    #define GPIO_INPUT_BOOT 36
    #elif CONFIG_GPIO_INPUT_39_BOOT
    #define GPIO_INPUT_BOOT 39
    #else
    #define GPIO_INPUT_BOOT 22
    #endif /* CONFIG_GPIO_INPUT_BOOT */

    #if CONFIG_GPIO_INPUT_0_MS
    #define GPIO_INPUT_MS 0
    #elif CONFIG_GPIO_INPUT_2_MS
    #define GPIO_INPUT_MS 2
    #elif CONFIG_GPIO_INPUT_4_MS
    #define GPIO_INPUT_MS 4
    #elif CONFIG_GPIO_INPUT_5_MS
    #define GPIO_INPUT_MS 5
    #elif CONFIG_GPIO_INPUT_6_MS
    #define GPIO_INPUT_MS 6
    #elif CONFIG_GPIO_INPUT_7_MS
    #define GPIO_INPUT_MS 7
    #elif CONFIG_GPIO_INPUT_8_MS
    #define GPIO_INPUT_MS 8
    #elif CONFIG_GPIO_INPUT_9_MS
    #define GPIO_INPUT_MS 9
    #elif CONFIG_GPIO_INPUT_10_MS
    #define GPIO_INPUT_MS 10
    #elif CONFIG_GPIO_INPUT_11_MS
    #define GPIO_INPUT_MS 11
    #elif CONFIG_GPIO_INPUT_12_MS
    #define GPIO_INPUT_MS 12
    #elif CONFIG_GPIO_INPUT_13_MS
    #define GPIO_INPUT_MS 13
    #elif CONFIG_GPIO_INPUT_14_MS
    #define GPIO_INPUT_MS 14
    #elif CONFIG_GPIO_INPUT_15_MS
    #define GPIO_INPUT_MS 15
    #elif CONFIG_GPIO_INPUT_16_MS
    #define GPIO_INPUT_MS 16
    #elif CONFIG_GPIO_INPUT_17_MS
    #define GPIO_INPUT_MS 17
    #elif CONFIG_GPIO_INPUT_18_MS
    #define GPIO_INPUT_MS 18
    #elif CONFIG_GPIO_INPUT_19_MS
    #define GPIO_INPUT_MS 19
    #elif CONFIG_GPIO_INPUT_21_MS
    #define GPIO_INPUT_MS 21
    #elif CONFIG_GPIO_INPUT_22_MS
    #define GPIO_INPUT_MS 22
    #elif CONFIG_GPIO_INPUT_23_MS
    #define GPIO_INPUT_MS 23
    #elif CONFIG_GPIO_INPUT_25_MS
    #define GPIO_INPUT_MS 25
    #elif CONFIG_GPIO_INPUT_26_MS
    #define GPIO_INPUT_MS 26
    #elif CONFIG_GPIO_INPUT_27_MS
    #define GPIO_INPUT_MS 27
    #elif CONFIG_GPIO_INPUT_32_MS
    #define GPIO_INPUT_MS 32
    #elif CONFIG_GPIO_INPUT_33_MS
    #define GPIO_INPUT_MS 33
    #elif CONFIG_GPIO_INPUT_34_MS
    #define GPIO_INPUT_MS 34
    #elif CONFIG_GPIO_INPUT_35_MS
    #define GPIO_INPUT_MS 35
    #elif CONFIG_GPIO_INPUT_36_MS
    #define GPIO_INPUT_MS 36
    #elif CONFIG_GPIO_INPUT_39_MS
    #define GPIO_INPUT_MS 39
    #else
    #define GPIO_INPUT_MS 15
    #endif /* CONFIG_GPIO_INPUT_MS */

    #if CONFIG_GPIO_INPUT_0_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 0
    #elif CONFIG_GPIO_INPUT_2_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 2
    #elif CONFIG_GPIO_INPUT_4_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 4
    #elif CONFIG_GPIO_INPUT_5_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 5
    #elif CONFIG_GPIO_INPUT_6_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 6
    #elif CONFIG_GPIO_INPUT_7_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 7
    #elif CONFIG_GPIO_INPUT_8_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 8
    #elif CONFIG_GPIO_INPUT_9_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 9
    #elif CONFIG_GPIO_INPUT_10_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 10
    #elif CONFIG_GPIO_INPUT_11_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 11
    #elif CONFIG_GPIO_INPUT_12_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 12
    #elif CONFIG_GPIO_INPUT_13_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 13
    #elif CONFIG_GPIO_INPUT_14_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 14
    #elif CONFIG_GPIO_INPUT_15_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 15
    #elif CONFIG_GPIO_INPUT_16_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 16
    #elif CONFIG_GPIO_INPUT_17_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 17
    #elif CONFIG_GPIO_INPUT_18_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 18
    #elif CONFIG_GPIO_INPUT_19_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 19
    #elif CONFIG_GPIO_INPUT_21_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 21
    #elif CONFIG_GPIO_INPUT_22_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 22
    #elif CONFIG_GPIO_INPUT_23_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 23
    #elif CONFIG_GPIO_INPUT_25_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 25
    #elif CONFIG_GPIO_INPUT_26_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 26
    #elif CONFIG_GPIO_INPUT_27_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 27
    #elif CONFIG_GPIO_INPUT_32_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 32
    #elif CONFIG_GPIO_INPUT_33_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 33
    #elif CONFIG_GPIO_INPUT_34_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 34
    #elif CONFIG_GPIO_INPUT_35_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 35
    #elif CONFIG_GPIO_INPUT_36_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 36
    #elif CONFIG_GPIO_INPUT_39_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 39
    #else
    #define GPIO_INPUT_DRBELL_NOTIF 34
    #endif /* CONFIG_GPIO_INPUT_DRBELL_NOTIF */

    #if CONFIG_GPIO_INPUT_0_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 0
    #elif CONFIG_GPIO_INPUT_2_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 2
    #elif CONFIG_GPIO_INPUT_4_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 4
    #elif CONFIG_GPIO_INPUT_5_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 5
    #elif CONFIG_GPIO_INPUT_6_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 6
    #elif CONFIG_GPIO_INPUT_7_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 7
    #elif CONFIG_GPIO_INPUT_8_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 8
    #elif CONFIG_GPIO_INPUT_9_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 9
    #elif CONFIG_GPIO_INPUT_10_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 10
    #elif CONFIG_GPIO_INPUT_11_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 11
    #elif CONFIG_GPIO_INPUT_12_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 12
    #elif CONFIG_GPIO_INPUT_13_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 13
    #elif CONFIG_GPIO_INPUT_14_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 14
    #elif CONFIG_GPIO_INPUT_15_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 15
    #elif CONFIG_GPIO_INPUT_16_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 16
    #elif CONFIG_GPIO_INPUT_17_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 17
    #elif CONFIG_GPIO_INPUT_18_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 18
    #elif CONFIG_GPIO_INPUT_19_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 19
    #elif CONFIG_GPIO_INPUT_21_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 21
    #elif CONFIG_GPIO_INPUT_22_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 22
    #elif CONFIG_GPIO_INPUT_23_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 23
    #elif CONFIG_GPIO_INPUT_25_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 25
    #elif CONFIG_GPIO_INPUT_26_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 26
    #elif CONFIG_GPIO_INPUT_27_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 27
    #elif CONFIG_GPIO_INPUT_32_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 32
    #elif CONFIG_GPIO_INPUT_33_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 33
    #elif CONFIG_GPIO_INPUT_34_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 34
    #elif CONFIG_GPIO_INPUT_35_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 35
    #elif CONFIG_GPIO_INPUT_36_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 36
    #elif CONFIG_GPIO_INPUT_39_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 39
    #else
    #define GPIO_INPUT_REEDSW_STATUS 35
    #endif /* CONFIG_GPIO_INPUT_REEDSW_STATUS */

    #if CONFIG_GPIO_INPUT_0_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 0
    #elif CONFIG_GPIO_INPUT_2_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 2
    #elif CONFIG_GPIO_INPUT_4_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 4
    #elif CONFIG_GPIO_INPUT_5_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 5
    #elif CONFIG_GPIO_INPUT_6_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 6
    #elif CONFIG_GPIO_INPUT_7_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 7
    #elif CONFIG_GPIO_INPUT_8_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 8
    #elif CONFIG_GPIO_INPUT_9_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 9
    #elif CONFIG_GPIO_INPUT_10_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 10
    #elif CONFIG_GPIO_INPUT_11_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 11
    #elif CONFIG_GPIO_INPUT_12_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 12
    #elif CONFIG_GPIO_INPUT_13_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 13
    #elif CONFIG_GPIO_INPUT_14_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 14
    #elif CONFIG_GPIO_INPUT_15_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 15
    #elif CONFIG_GPIO_INPUT_16_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 16
    #elif CONFIG_GPIO_INPUT_17_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 17
    #elif CONFIG_GPIO_INPUT_18_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 18
    #elif CONFIG_GPIO_INPUT_19_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 19
    #elif CONFIG_GPIO_INPUT_21_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 21
    #elif CONFIG_GPIO_INPUT_22_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 22
    #elif CONFIG_GPIO_INPUT_23_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 23
    #elif CONFIG_GPIO_INPUT_25_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 25
    #elif CONFIG_GPIO_INPUT_26_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 26
    #elif CONFIG_GPIO_INPUT_27_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 27
    #elif CONFIG_GPIO_INPUT_32_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 32
    #elif CONFIG_GPIO_INPUT_33_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 33
    #elif CONFIG_GPIO_INPUT_34_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 34
    #elif CONFIG_GPIO_INPUT_35_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 35
    #elif CONFIG_GPIO_INPUT_36_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 36
    #elif CONFIG_GPIO_INPUT_39_MOTOR_FAULT
    #define GPIO_INPUT_MOTOR_FAULT 39
    #else
    #define GPIO_INPUT_MOTOR_FAULT 25
    #endif /* CONFIG_GPIO_INPUT_MOTOR_FAULT */

    #if CONFIG_GPIO_INPUT_0_NSW
    #define GPIO_INPUT_NSW 0
    #elif CONFIG_GPIO_INPUT_2_NSW
    #define GPIO_INPUT_NSW 2
    #elif CONFIG_GPIO_INPUT_4_NSW
    #define GPIO_INPUT_NSW 4
    #elif CONFIG_GPIO_INPUT_5_NSW
    #define GPIO_INPUT_NSW 5
    #elif CONFIG_GPIO_INPUT_6_NSW
    #define GPIO_INPUT_NSW 6
    #elif CONFIG_GPIO_INPUT_7_NSW
    #define GPIO_INPUT_NSW 7
    #elif CONFIG_GPIO_INPUT_8_NSW
    #define GPIO_INPUT_NSW 8
    #elif CONFIG_GPIO_INPUT_9_NSW
    #define GPIO_INPUT_NSW 9
    #elif CONFIG_GPIO_INPUT_10_NSW
    #define GPIO_INPUT_NSW 10
    #elif CONFIG_GPIO_INPUT_11_NSW
    #define GPIO_INPUT_NSW 11
    #elif CONFIG_GPIO_INPUT_12_NSW
    #define GPIO_INPUT_NSW 12
    #elif CONFIG_GPIO_INPUT_13_NSW
    #define GPIO_INPUT_NSW 13
    #elif CONFIG_GPIO_INPUT_14_NSW
    #define GPIO_INPUT_NSW 14
    #elif CONFIG_GPIO_INPUT_15_NSW
    #define GPIO_INPUT_NSW 15
    #elif CONFIG_GPIO_INPUT_16_NSW
    #define GPIO_INPUT_NSW 16
    #elif CONFIG_GPIO_INPUT_17_NSW
    #define GPIO_INPUT_NSW 17
    #elif CONFIG_GPIO_INPUT_18_NSW
    #define GPIO_INPUT_NSW 18
    #elif CONFIG_GPIO_INPUT_19_NSW
    #define GPIO_INPUT_NSW 19
    #elif CONFIG_GPIO_INPUT_21_NSW
    #define GPIO_INPUT_NSW 21
    #elif CONFIG_GPIO_INPUT_22_NSW
    #define GPIO_INPUT_NSW 22
    #elif CONFIG_GPIO_INPUT_23_NSW
    #define GPIO_INPUT_NSW 23
    #elif CONFIG_GPIO_INPUT_25_NSW
    #define GPIO_INPUT_NSW 25
    #elif CONFIG_GPIO_INPUT_26_NSW
    #define GPIO_INPUT_NSW 26
    #elif CONFIG_GPIO_INPUT_27_NSW
    #define GPIO_INPUT_NSW 27
    #elif CONFIG_GPIO_INPUT_32_NSW
    #define GPIO_INPUT_NSW 32
    #elif CONFIG_GPIO_INPUT_33_NSW
    #define GPIO_INPUT_NSW 33
    #elif CONFIG_GPIO_INPUT_34_NSW
    #define GPIO_INPUT_NSW 34
    #elif CONFIG_GPIO_INPUT_35_NSW
    #define GPIO_INPUT_NSW 35
    #elif CONFIG_GPIO_INPUT_36_NSW
    #define GPIO_INPUT_NSW 36
    #elif CONFIG_GPIO_INPUT_39_NSW
    #define GPIO_INPUT_NSW 39
    #else
    #define GPIO_INPUT_NSW 26
    #endif /* CONFIG_GPIO_INPUT_NSW */

    // Commented GPIOs don't have ADCs
    #if CONFIG_GPIO_INPUT_0_BATTERY
    #define GPIO_INPUT_BATTERY 0
    #define BATTERY_ADC_CH 1
    #define BATTERY_ADC_UNIT 2
    #elif CONFIG_GPIO_INPUT_2_BATTERY
    #define GPIO_INPUT_BATTERY 2
    #define BATTERY_ADC_CH 2
    #define BATTERY_ADC_UNIT 2
    #elif CONFIG_GPIO_INPUT_4_BATTERY
    #define GPIO_INPUT_BATTERY 4
    #define BATTERY_ADC_CH 0
    #define BATTERY_ADC_UNIT 2
    // #elif CONFIG_GPIO_INPUT_5_BATTERY
    // #define GPIO_INPUT_BATTERY 5
    // #elif CONFIG_GPIO_INPUT_6_BATTERY
    // #define GPIO_INPUT_BATTERY 6
    // #elif CONFIG_GPIO_INPUT_7_BATTERY
    // #define GPIO_INPUT_BATTERY 7
    // #elif CONFIG_GPIO_INPUT_8_BATTERY
    // #define GPIO_INPUT_BATTERY 8
    // #elif CONFIG_GPIO_INPUT_9_BATTERY
    // #define GPIO_INPUT_BATTERY 9
    // #elif CONFIG_GPIO_INPUT_10_BATTERY
    // #define GPIO_INPUT_BATTERY 10
    // #elif CONFIG_GPIO_INPUT_11_BATTERY
    // #define GPIO_INPUT_BATTERY 11
    #elif CONFIG_GPIO_INPUT_12_BATTERY
    #define GPIO_INPUT_BATTERY 12
    #define BATTERY_ADC_CH 5
    #define BATTERY_ADC_UNIT 2
    #elif CONFIG_GPIO_INPUT_13_BATTERY
    #define GPIO_INPUT_BATTERY 13
    #define BATTERY_ADC_CH 4
    #define BATTERY_ADC_UNIT 2
    #elif CONFIG_GPIO_INPUT_14_BATTERY
    #define GPIO_INPUT_BATTERY 14
    #define BATTERY_ADC_CH 6
    #define BATTERY_ADC_UNIT 2
    #elif CONFIG_GPIO_INPUT_15_BATTERY
    #define GPIO_INPUT_BATTERY 15
    #define BATTERY_ADC_CH 3
    #define BATTERY_ADC_UNIT 2
    // #elif CONFIG_GPIO_INPUT_16_BATTERY
    // #define GPIO_INPUT_BATTERY 16
    // #elif CONFIG_GPIO_INPUT_17_BATTERY
    // #define GPIO_INPUT_BATTERY 17
    // #elif CONFIG_GPIO_INPUT_18_BATTERY
    // #define GPIO_INPUT_BATTERY 18
    // #elif CONFIG_GPIO_INPUT_19_BATTERY
    // #define GPIO_INPUT_BATTERY 19
    // #elif CONFIG_GPIO_INPUT_21_BATTERY
    // #define GPIO_INPUT_BATTERY 21
    // #elif CONFIG_GPIO_INPUT_22_BATTERY
    // #define GPIO_INPUT_BATTERY 22
    // #elif CONFIG_GPIO_INPUT_23_BATTERY
    // #define GPIO_INPUT_BATTERY 23
    #elif CONFIG_GPIO_INPUT_25_BATTERY
    #define GPIO_INPUT_BATTERY 25
    #define BATTERY_ADC_CH 8
    #define BATTERY_ADC_UNIT 2
    #elif CONFIG_GPIO_INPUT_26_BATTERY
    #define GPIO_INPUT_BATTERY 26
    #define BATTERY_ADC_CH 9
    #define BATTERY_ADC_UNIT 2
    #elif CONFIG_GPIO_INPUT_27_BATTERY
    #define GPIO_INPUT_BATTERY 27
    #define BATTERY_ADC_CH 7
    #define BATTERY_ADC_UNIT 2
    #elif CONFIG_GPIO_INPUT_32_BATTERY
    #define GPIO_INPUT_BATTERY 32
    #define BATTERY_ADC_CH 4
    #define BATTERY_ADC_UNIT 1
    #elif CONFIG_GPIO_INPUT_33_BATTERY
    #define GPIO_INPUT_BATTERY 33
    #define BATTERY_ADC_CH 5
    #define BATTERY_ADC_UNIT 1
    #elif CONFIG_GPIO_INPUT_34_BATTERY
    #define GPIO_INPUT_BATTERY 34
    #define BATTERY_ADC_CH 7
    #define BATTERY_ADC_UNIT 1
    #elif CONFIG_GPIO_INPUT_35_BATTERY
    #define GPIO_INPUT_BATTERY 35
    #define BATTERY_ADC_CH 7
    #define BATTERY_ADC_UNIT 1
    #elif CONFIG_GPIO_INPUT_36_BATTERY
    #define GPIO_INPUT_BATTERY 36
    #define BATTERY_ADC_CH 0
    #define BATTERY_ADC_UNIT 1
    #elif CONFIG_GPIO_INPUT_39_BATTERY
    #define GPIO_INPUT_BATTERY 39
    #define BATTERY_ADC_CH 3
    #define BATTERY_ADC_UNIT 1
    #else
    #define GPIO_INPUT_BATTERY 36
    #define BATTERY_ADC_CH 7
    #define BATTERY_ADC_UNIT 2
    #endif /* CONFIG_GPIO_INPUT_BATTERY */

#endif /* MAIN_MCU specific GPIOs */


#if CONFIG_ESP32_CAM_MCU

    #if CONFIG_GPIO_INPUT_0_BOOT
    #define GPIO_INPUT_BOOT 0
    #elif CONFIG_GPIO_INPUT_2_BOOT
    #define GPIO_INPUT_BOOT 2
    #elif CONFIG_GPIO_INPUT_4_BOOT
    #define GPIO_INPUT_BOOT 4
    #elif CONFIG_GPIO_INPUT_12_BOOT
    #define GPIO_INPUT_BOOT 12
    #elif CONFIG_GPIO_INPUT_13_BOOT
    #define GPIO_INPUT_BOOT 13
    #elif CONFIG_GPIO_INPUT_14_BOOT
    #define GPIO_INPUT_BOOT 14
    #elif CONFIG_GPIO_INPUT_15_BOOT
    #define GPIO_INPUT_BOOT 15
    #elif CONFIG_GPIO_INPUT_16_BOOT
    #define GPIO_INPUT_BOOT 16
    #else
    #define GPIO_INPUT_BOOT 0
    #endif /* CONFIG_GPIO_INPUT_BOOT */

    // Commented out GPIOs are used for SD card comms.
    #if CONFIG_GPIO_INPUT_0_MS
    #define GPIO_INPUT_MS 0
    // #elif CONFIG_GPIO_INPUT_2_MS
    // #define GPIO_INPUT_BOOT 2
    // #elif CONFIG_GPIO_INPUT_4_MS
    // #define GPIO_INPUT_MS 4
    // #elif CONFIG_GPIO_INPUT_12_MS
    // #define GPIO_INPUT_MS 12
    // #elif CONFIG_GPIO_INPUT_13_MS
    // #define GPIO_INPUT_MS 13
    #elif CONFIG_GPIO_INPUT_14_MS
    #define GPIO_INPUT_MS 14
    // #elif CONFIG_GPIO_INPUT_15_MS
    // #define GPIO_INPUT_MS 15
    #elif CONFIG_GPIO_INPUT_16_MS
    #define GPIO_INPUT_MS 16
    #else
    #define GPIO_INPUT_MS 16
    #endif /* CONFIG_GPIO_INPUT_MS */

    // Commented out GPIOs are used for SD card comms.
    #if CONFIG_GPIO_INPUT_0_PIC
    #define GPIO_INPUT_PIC 0
    // #elif CONFIG_GPIO_INPUT_2_PIC
    // #define GPIO_INPUT_PIC 2
    // #elif CONFIG_GPIO_INPUT_4_PIC
    // #define GPIO_INPUT_PIC 4
    // #elif CONFIG_GPIO_INPUT_12_PIC
    // #define GPIO_INPUT_PIC 12
    // #elif CONFIG_GPIO_INPUT_13_PIC
    // #define GPIO_INPUT_PIC 13
    #elif CONFIG_GPIO_INPUT_14_PIC
    #define GPIO_INPUT_PIC 14
    // #elif CONFIG_GPIO_INPUT_15_PIC
    // #define GPIO_INPUT_PIC 15
    #elif CONFIG_GPIO_INPUT_16_PIC
    #define GPIO_INPUT_PIC 16
    #else
    #define GPIO_INPUT_PIC 14
    #endif /* CONFIG_GPIO_INPUT_PIC */

    #if  CONFIG_GPIO_INPUT_0_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 0
    #elif CONFIG_GPIO_INPUT_2_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 2
    #elif  CONFIG_GPIO_INPUT_4_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 4
    #elif  CONFIG_GPIO_INPUT_12_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 12
    #elif  CONFIG_GPIO_INPUT_13_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 13
    #elif  CONFIG_GPIO_INPUT_14_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 14
    #elif  CONFIG_GPIO_INPUT_15_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 15
    #elif  CONFIG_GPIO_INPUT_16_DRBELL_NOTIF
    #define GPIO_INPUT_DRBELL_NOTIF 16
    #else
    #define GPIO_INPUT_DRBELL_NOTIF 13
    #endif /* CONFIG_GPIO_INPUT_DRBELL_NOTIF */

    #if  GPIO_INPUT_0_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 0
    #elif CONFIG_GPIO_INPUT_2_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 2
    #elif  CONFIG_GPIO_INPUT_4_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 4
    #elif  CONFIG_GPIO_INPUT_12_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 12
    #elif  CONFIG_GPIO_INPUT_13_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 13
    #elif  CONFIG_GPIO_INPUT_14_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 14
    #elif  CONFIG_GPIO_INPUT_15_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 15
    #elif  CONFIG_GPIO_INPUT_16_REEDSW_STATUS
    #define GPIO_INPUT_REEDSW_STATUS 16
    #else
    #define GPIO_INPUT_REEDSW_STATUS 15
    #endif /* CONFIG_GPIO_INPUT_REEDSW_STATUS */

#endif /* ESP32_CAM_MCU specific GPIOs */

#define GPIO_INPUT_BOOT_PIN_SEL         (1ULL<<GPIO_INPUT_BOOT)
#define GPIO_INPUT_MS_PIN_SEL           (1ULL<<GPIO_INPUT_MS)
#define GPIO_INPUT_DRBELL_PIN_SEL       (1ULL<<GPIO_INPUT_DRBELL_NOTIF)
#define GPIO_INPUT_REEDSW_PIN_SEL       (1ULL<<GPIO_INPUT_REEDSW_STATUS)

#if CONFIG_MAIN_MCU
    #define GPIO_INPUT_MOTOR_FAULT_PIN_SEL  (1ULL<<GPIO_INPUT_MOTOR_FAULT)
    #define GPIO_INPUT_NSW_PIN_SEL          (1ULL<<GPIO_INPUT_NSW)
    #define GPIO_INPUT_BATTERY_PIN_SEL      (1ULL<<GPIO_INPUT_BATTERY)
#endif /* MAIN_MCU specific GPIOs */

#if CONFIG_ESP32_CAM_MCU
    #define GPIO_INPUT_PIC_PIN_SEL          (1ULL<<GPIO_INPUT_PIC)
#endif /* ESP32_CAM specific GPIOs */

//===========================================
//
//              OUTPUT CONFIG
//
//===========================================

#if CONFIG_MAIN_MCU

    #if CONFIG_GPIO_OUTPUT_0_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 0
    #elif CONFIG_GPIO_OUTPUT_2_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 2
    #elif CONFIG_GPIO_OUTPUT_4_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 4
    #elif CONFIG_GPIO_OUTPUT_5_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 5
    #elif CONFIG_GPIO_OUTPUT_6_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 6
    #elif CONFIG_GPIO_OUTPUT_7_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 7
    #elif CONFIG_GPIO_OUTPUT_8_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 8
    #elif CONFIG_GPIO_OUTPUT_9_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 9
    #elif CONFIG_GPIO_OUTPUT_10_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 10
    #elif CONFIG_GPIO_OUTPUT_11_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 11
    #elif CONFIG_GPIO_OUTPUT_12_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 12
    #elif CONFIG_GPIO_OUTPUT_13_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 13
    #elif CONFIG_GPIO_OUTPUT_14_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 14
    #elif CONFIG_GPIO_OUTPUT_15_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 15
    #elif CONFIG_GPIO_OUTPUT_16_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 16
    #elif CONFIG_GPIO_OUTPUT_17_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 17
    #elif CONFIG_GPIO_OUTPUT_18_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 18
    #elif CONFIG_GPIO_OUTPUT_19_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 19
    #elif CONFIG_GPIO_OUTPUT_21_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 21
    #elif CONFIG_GPIO_OUTPUT_22_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 22
    #elif CONFIG_GPIO_OUTPUT_23_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 23
    #elif CONFIG_GPIO_OUTPUT_25_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 25
    #elif CONFIG_GPIO_OUTPUT_26_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 26
    #elif CONFIG_GPIO_OUTPUT_27_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 27
    #elif CONFIG_GPIO_OUTPUT_32_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 32
    #elif CONFIG_GPIO_OUTPUT_33_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 33
    #elif CONFIG_GPIO_OUTPUT_34_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 34
    #elif CONFIG_GPIO_OUTPUT_35_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 35
    #elif CONFIG_GPIO_OUTPUT_36_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 36
    #elif CONFIG_GPIO_OUTPUT_39_MOTOR_IN1
    #define GPIO_OUTPUT_MOTOR_IN1 39
    #else
    #define GPIO_OUTPUT_MOTOR_IN1 33
    #endif /* CONFIG_GPIO_OUTPUT_MOTOR_IN1 */

    #if CONFIG_GPIO_OUTPUT_0_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 0
    #elif CONFIG_GPIO_OUTPUT_2_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 2
    #elif CONFIG_GPIO_OUTPUT_4_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 4
    #elif CONFIG_GPIO_OUTPUT_5_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 5
    #elif CONFIG_GPIO_OUTPUT_6_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 6
    #elif CONFIG_GPIO_OUTPUT_7_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 7
    #elif CONFIG_GPIO_OUTPUT_8_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 8
    #elif CONFIG_GPIO_OUTPUT_9_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 9
    #elif CONFIG_GPIO_OUTPUT_10_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 10
    #elif CONFIG_GPIO_OUTPUT_11_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 11
    #elif CONFIG_GPIO_OUTPUT_12_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 12
    #elif CONFIG_GPIO_OUTPUT_13_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 13
    #elif CONFIG_GPIO_OUTPUT_14_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 14
    #elif CONFIG_GPIO_OUTPUT_15_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 15
    #elif CONFIG_GPIO_OUTPUT_16_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 16
    #elif CONFIG_GPIO_OUTPUT_17_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 17
    #elif CONFIG_GPIO_OUTPUT_18_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 18
    #elif CONFIG_GPIO_OUTPUT_19_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 19
    #elif CONFIG_GPIO_OUTPUT_21_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 21
    #elif CONFIG_GPIO_OUTPUT_22_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 22
    #elif CONFIG_GPIO_OUTPUT_23_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 23
    #elif CONFIG_GPIO_OUTPUT_25_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 25
    #elif CONFIG_GPIO_OUTPUT_26_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 26
    #elif CONFIG_GPIO_OUTPUT_27_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 27
    #elif CONFIG_GPIO_OUTPUT_32_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 32
    #elif CONFIG_GPIO_OUTPUT_33_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 33
    #elif CONFIG_GPIO_OUTPUT_34_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 34
    #elif CONFIG_GPIO_OUTPUT_35_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 35
    #elif CONFIG_GPIO_OUTPUT_36_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 36
    #elif CONFIG_GPIO_OUTPUT_39_MOTOR_IN2
    #define GPIO_OUTPUT_MOTOR_IN2 39
    #else
    #define GPIO_OUTPUT_MOTOR_IN2 32
    #endif /* CONFIG_GPIO_OUTPUT_MOTOR_IN2 */

    #define GPIO_OUTPUT_MOTOR_IN1_PIN_SEL   (1ULL<<GPIO_OUTPUT_MOTOR_IN1)
    #define GPIO_OUTPUT_MOTOR_IN2_PIN_SEL   (1ULL<<GPIO_OUTPUT_MOTOR_IN2)

#endif /* MAIN_MCU specific GPIOs */

#if CONFIG_ESP32_CAM_MCU

    #if CONFIG_GPIO_OUTPUT_0_CONFIRM
    #define GPIO_OUTPUT_CONFIRM_UPLOAD 0
    #elif CONFIG_GPIO_OUTPUT_2_CONFIRM
    #define GPIO_OUTPUT_CONFIRM_UPLOAD 4
    #elif CONFIG_GPIO_OUTPUT_12_CONFIRM
    #define GPIO_OUTPUT_CONFIRM_UPLOAD 12
    #elif CONFIG_GPIO_OUTPUT_13_CONFIRM
    #define GPIO_OUTPUT_CONFIRM_UPLOAD 13
    #elif CONFIG_GPIO_OUTPUT_14_CONFIRM
    #define GPIO_OUTPUT_CONFIRM_UPLOAD 14
    #elif CONFIG_GPIO_OUTPUT_15_CONFIRM
    #define GPIO_OUTPUT_CONFIRM_UPLOAD 15
    #elif CONFIG_GPIO_OUTPUT_16_CONFIRM
    #define GPIO_OUTPUT_CONFIRM_UPLOAD 16
    #else
    #define GPIO_OUTPUT_CONFIRM_UPLOAD 4
    #endif /* CONFIG_GPIO_OUTPUT_CONFIRM_UPLOAD */
 
    #define GPIO_OUTPUT_CONFIRM_PIN_SEL   (1ULL<<GPIO_OUTPUT_CONFIRM_UPLOAD)

#endif /* ESP32_CAM_MCU specific GPIOs */

#if CONFIG_MAIN_MCU
#define NUM_GPIO 9
#elif CONFIG_ESP32_CAM_MCU
#define NUM_GPIO 6
#else
#define NUM_GPIO 4
#endif

//===========================================
//
//        Wrong GPIO assignment check
//
//===========================================

#define GPIO_MULTA_ERR_MSG "Multiple functions assigned to same GPIO: "

static const uint8_t gpio_array[NUM_GPIO] = {
#if CONFIG_MAIN_MCU
    GPIO_INPUT_BATTERY, 
    GPIO_INPUT_MOTOR_FAULT,
    GPIO_INPUT_NSW,
    GPIO_OUTPUT_MOTOR_IN1,
    GPIO_OUTPUT_MOTOR_IN2,
#elif CONFIG_ESP32_CAM_MCU
    GPIO_INPUT_PIC,
    GPIO_OUTPUT_CONFIRM_UPLOAD,
#endif
    GPIO_INPUT_BOOT, 
    GPIO_INPUT_DRBELL_NOTIF, 
    GPIO_INPUT_MS,
    GPIO_INPUT_REEDSW_STATUS
    };

/*#define CHECK_GPIO_ASSIGNMENT(_gpio) ({     \
    char err[100];                          \
    int cnt = 0;                            \
    (_gpio == GPIO_INPUT_MS)            ? strcat(err, "Motion Sensor & "); cnt++;   \
    (_gpio == GPIO_INPUT_MOTOR_FAULT)   ? strcat(err, "Motor Fault & "); cnt++;     \
    (_gpio == GPIO_INPUT_NSW)           ? strcat(err, "N-switch & "); cnt++;        \
    (_gpio == GPIO_INPUT_BATTERY)       ? strcat(err, "Battery & "); cnt++;         \
    (_gpio == GPIO_INPUT_DRBELL_NOTIF)  ? strcat(err, "Doorbell & "); cnt++;        \
    (_gpio == GPIO_INPUT_REEDSW_STATUS) ? strcat(err, "Reed Switch & "); cnt++;     \
    (_gpio == GPIO_OUTPUT_MOTOR_IN1)    ? strcat(err, "Motor CW (In 1) & "); cnt++; \
    (_gpio == GPIO_OUTPUT_MOTOR_IN2)    ? strcat(err, "Motor CCW (In 2) & "); cnt++;\
    (cnt > 1) ? memmove(err, err, strlen(err)-3); err; : NULL;                      \
})*/

/*
#if (CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_MS) != NULL)
#error GPIO_MULTA_ERR_MSG CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_MS)
#endif

#if (CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_MOTOR_FAULT) != NULL)
#error GPIO_MULTA_ERR_MSG CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_MOTOR_FAULT)
#endif

#if (CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_NSW) != NULL)
#error GPIO_MULTA_ERR_MSG CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_NSW)
#endif

#if (CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_BATTERY) != NULL)
#error GPIO_MULTA_ERR_MSG CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_BATTERY)
#endif

#if (CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_DRBELL_NOTIF) != NULL)
#error GPIO_MULTA_ERR_MSG CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_DRBELL_NOTIF)
#endif

#if (CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_REEDSW_STATUS) != NULL)
#error GPIO_MULTA_ERR_MSG CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_REEDSW_STATUS)
#endif

#if (CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_MOTOR_IN1) != NULL)
#error GPIO_MULTA_ERR_MSG CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_MOTOR_IN1)
#endif

#if (CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_MOTOR_IN2) != NULL)
#error GPIO_MULTA_ERR_MSG CHECK_GPIO_ASSIGNMENT(GPIO_INPUT_MOTOR_IN2)
#endif*/

static void check_single_gpio_assignment(uint8_t _gpio, char* _err) {
    char err[100];
    int cnt = 0;
    if(_gpio == GPIO_INPUT_BOOT) {
        strcat(err, "Boot & ");
        cnt++;
    }
    if(_gpio == GPIO_INPUT_MS) {
        strcat(err, "Motion Sensor & ");
        cnt++;
    }
    if(_gpio == GPIO_INPUT_DRBELL_NOTIF) {
        strcat(err, "Doorbell & ");
        cnt++;
    }
    if(_gpio == GPIO_INPUT_REEDSW_STATUS) {
        strcat(err, "Reed Switch & ");
        cnt++;
    }
#if CONFIG_MAIN_MCU
    if(_gpio == GPIO_INPUT_MOTOR_FAULT) {
        strcat(err, "Motor Fault & ");
        cnt++;
    }
    if(_gpio == GPIO_INPUT_NSW) {
        strcat(err, "N-switch & ");
        cnt++;
    }
    if(_gpio == GPIO_INPUT_BATTERY) {
        strcat(err, "Battery & ");
        cnt++;
    }
    if(_gpio == GPIO_OUTPUT_MOTOR_IN1) {
        strcat(err, "Motor CW (In 1) & ");
        cnt++;
    }
    if(_gpio == GPIO_OUTPUT_MOTOR_IN2) {
        strcat(err, "Motor CCW (In 2) & ");
        cnt++;
    }
#elif CONFIG_ESP32_CAM_MCU
    if(_gpio == GPIO_INPUT_PIC) {
        strcat(err, "Picture & ");
        cnt++;
    }
    if(_gpio == GPIO_OUTPUT_CONFIRM_UPLOAD) {
        strcat(err, "Confirm Upload & ");
        cnt++;
    }
#endif
    if(cnt > 1) {
        memmove(_err, err, strlen(err)-3);
    }
    else {
        free(_err);
        _err = NULL;
    }
}

static void gpio_overlap_check(const char* TAG) {
    char* err = NULL;
    for (uint32_t active_gpio = 0; active_gpio < sizeof(gpio_array)/sizeof(uint8_t); active_gpio++) {
        check_single_gpio_assignment(gpio_array[active_gpio], err);
        if (err != NULL)
            ESP_LOGE(TAG, "%s %s", GPIO_MULTA_ERR_MSG, err);
    }
    /*
    check_gpio_assignment(GPIO_INPUT_BOOT, err);
    if (err != NULL)
        ESP_LOGE(TAG, "%s %s", GPIO_MULTA_ERR_MSG, err);
    check_gpio_assignment(GPIO_INPUT_MS, err);
    if (err != NULL)
        ESP_LOGE(TAG, "%s %s", GPIO_MULTA_ERR_MSG, err);
    check_gpio_assignment(GPIO_INPUT_MOTOR_FAULT, err);
    if (err != NULL)
        ESP_LOGE(TAG, "%s %s", GPIO_MULTA_ERR_MSG, err);
    check_gpio_assignment(GPIO_INPUT_NSW, err);
    if (err != NULL)
        ESP_LOGE(TAG, "%s %s", GPIO_MULTA_ERR_MSG, err);
    check_gpio_assignment(GPIO_INPUT_BATTERY, err);
    if (err != NULL)
        ESP_LOGE(TAG, "%s %s", GPIO_MULTA_ERR_MSG, err);
    check_gpio_assignment(GPIO_INPUT_DRBELL_NOTIF, err);
    if (err != NULL)
        ESP_LOGE(TAG, "%s %s", GPIO_MULTA_ERR_MSG, err);
    check_gpio_assignment(GPIO_INPUT_REEDSW_STATUS, err);
    if (err != NULL)
        ESP_LOGE(TAG, "%s %s", GPIO_MULTA_ERR_MSG, err);
    check_gpio_assignment(GPIO_OUTPUT_MOTOR_IN1, err);
    if (err != NULL)
        ESP_LOGE(TAG, "%s %s", GPIO_MULTA_ERR_MSG, err);
    check_gpio_assignment(GPIO_OUTPUT_MOTOR_IN2, err);
    if (err != NULL)
        ESP_LOGE(TAG, "%s %s", GPIO_MULTA_ERR_MSG, err);
        */
}

//================= ADC Array ==================
/** 
 * ADC values are in inverse order with respect to battery voltage
 * as in a full battery (6V) shows 0 mV, 
 * while an empty battery (4.5V) shows 2450+ mV
 */
extern const uint32_t adc_val[99] = {
    0, 25, 50, 75, 100, 125, 150, 175, 200, 225, 
    250, 275, 300, 325, 350, 375, 400, 425, 450, 475, 
    500, 525, 550, 575, 600, 625, 650, 675, 700, 725,
    750, 775, 800, 825, 850, 875, 900, 925, 950, 975, 
    1000, 1025, 1050, 1075, 1100, 1125, 1150, 1175, 1200, 1225,
    1250, 1275, 1300, 1325, 1350, 1375, 1400, 1425, 1450, 1475,
    1500, 1525, 1550, 1575, 1600, 1625, 1650, 1675, 1700, 1725,
    1750, 1775, 1800, 1825, 1850, 1875, 1900, 1925, 1950, 1975,
    2000, 2025, 2050, 2075, 2100, 2125, 2150, 2175, 2200, 2225,
    2250, 2275, 2300, 2325, 2350, 2375, 2400, 2425, 2450
};

extern const uint32_t bat_val[100] = {
    100, 99, 98, 97, 96, 95, 94, 93, 92, 91,
    90, 89, 88, 87, 86, 85, 84, 83, 82, 81,
    80, 79, 78, 77, 76, 75, 74, 73, 72, 71,
    70, 69, 68, 67, 66, 65, 64, 63, 62, 61,
    60, 59, 58, 57, 56, 55, 54, 53, 52, 51,
    50, 49, 48, 47, 46, 45, 44, 43, 42, 41,
    40, 39, 38, 37, 36, 35, 34, 33, 32, 31,
    30, 29, 28, 27, 26, 25, 24, 23, 22, 21,
    20, 19, 18, 17, 16, 15, 14, 13, 12, 11,
    10, 9, 8, 7, 6, 5, 4, 3, 2, 1
};