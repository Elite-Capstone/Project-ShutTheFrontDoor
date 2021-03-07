/*
    Project:    ShutTheFrontDoor
    File:       stfd_gpio_config.h
    Author:     Benjamin Lanthier
    Date:       06/03/2021

    Desc:       This file contains the gpio setup and gpio assignment 
                error detection for both input/output
                Sets up the following GPIOs:
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

#if CONFIG_GPIO_INPUT_0_BATTERY
#define GPIO_INPUT_BATTERY 0
#elif CONFIG_GPIO_INPUT_2_BATTERY
#define GPIO_INPUT_BATTERY 2
#elif CONFIG_GPIO_INPUT_4_BATTERY
#define GPIO_INPUT_BATTERY 4
#elif CONFIG_GPIO_INPUT_5_BATTERY
#define GPIO_INPUT_BATTERY 5
#elif CONFIG_GPIO_INPUT_6_BATTERY
#define GPIO_INPUT_BATTERY 6
#elif CONFIG_GPIO_INPUT_7_BATTERY
#define GPIO_INPUT_BATTERY 7
#elif CONFIG_GPIO_INPUT_8_BATTERY
#define GPIO_INPUT_BATTERY 8
#elif CONFIG_GPIO_INPUT_9_BATTERY
#define GPIO_INPUT_BATTERY 9
#elif CONFIG_GPIO_INPUT_10_BATTERY
#define GPIO_INPUT_BATTERY 10
#elif CONFIG_GPIO_INPUT_11_BATTERY
#define GPIO_INPUT_BATTERY 11
#elif CONFIG_GPIO_INPUT_12_BATTERY
#define GPIO_INPUT_BATTERY 12
#elif CONFIG_GPIO_INPUT_13_BATTERY
#define GPIO_INPUT_BATTERY 13
#elif CONFIG_GPIO_INPUT_14_BATTERY
#define GPIO_INPUT_BATTERY 14
#elif CONFIG_GPIO_INPUT_15_BATTERY
#define GPIO_INPUT_BATTERY 15
#elif CONFIG_GPIO_INPUT_16_BATTERY
#define GPIO_INPUT_BATTERY 16
#elif CONFIG_GPIO_INPUT_17_BATTERY
#define GPIO_INPUT_BATTERY 17
#elif CONFIG_GPIO_INPUT_18_BATTERY
#define GPIO_INPUT_BATTERY 18
#elif CONFIG_GPIO_INPUT_19_BATTERY
#define GPIO_INPUT_BATTERY 19
#elif CONFIG_GPIO_INPUT_21_BATTERY
#define GPIO_INPUT_BATTERY 21
#elif CONFIG_GPIO_INPUT_22_BATTERY
#define GPIO_INPUT_BATTERY 22
#elif CONFIG_GPIO_INPUT_23_BATTERY
#define GPIO_INPUT_BATTERY 23
#elif CONFIG_GPIO_INPUT_25_BATTERY
#define GPIO_INPUT_BATTERY 25
#elif CONFIG_GPIO_INPUT_26_BATTERY
#define GPIO_INPUT_BATTERY 26
#elif CONFIG_GPIO_INPUT_27_BATTERY
#define GPIO_INPUT_BATTERY 27
#elif CONFIG_GPIO_INPUT_32_BATTERY
#define GPIO_INPUT_BATTERY 32
#elif CONFIG_GPIO_INPUT_33_BATTERY
#define GPIO_INPUT_BATTERY 33
#elif CONFIG_GPIO_INPUT_34_BATTERY
#define GPIO_INPUT_BATTERY 34
#elif CONFIG_GPIO_INPUT_35_BATTERY
#define GPIO_INPUT_BATTERY 35
#elif CONFIG_GPIO_INPUT_36_BATTERY
#define GPIO_INPUT_BATTERY 36
#elif CONFIG_GPIO_INPUT_39_BATTERY
#define GPIO_INPUT_BATTERY 39
#else
#define GPIO_INPUT_BATTERY 27
#endif /* CONFIG_GPIO_INPUT_BATTERY */

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

#define GPIO_INPUT_MS_PIN_SEL           (1ULL<<GPIO_INPUT_MS)
#define GPIO_INPUT_MOTOR_FAULT_PIN_SEL  (1ULL<<GPIO_INPUT_MOTOR_FAULT)
#define GPIO_INPUT_NSW_PIN_SEL          (1ULL<<GPIO_INPUT_NSW)
#define GPIO_INPUT_BATTERY_PIN_SEL      (1ULL<<GPIO_INPUT_BATTERY)
#define GPIO_INPUT_DRBELL_PIN_SEL       (1ULL<<GPIO_INPUT_DRBELL_NOTIF)
#define GPIO_INPUT_REEDSW_PIN_SEL       (1ULL<<GPIO_INPUT_REEDSW_STATUS)

//===========================================
//
//              OUTPUT CONFIG
//
//===========================================

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

#define GPIO_OUTPUT_MOTOR_IN1_PIN_SEL   1//(1ULL<<GPIO_OUTPUT_MOTOR_IN1)
#define GPIO_OUTPUT_MOTOR_IN2_PIN_SEL   1//(1ULL<<GPIO_OUTPUT_MOTOR_IN2)

//===========================================
//
//        Wrong GPIO assignment check
//
//===========================================

#define GPIO_MULTA_ERR_MSG "Multiple functions assigned to same GPIO: "

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

static void check_gpio_assignment(uint8_t _gpio, char* _err) {
    char err[100];
    int cnt = 0;
    if(_gpio == GPIO_INPUT_MS) {
        strcat(err, "Motion Sensor & ");
        cnt++;
    }
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
    if(_gpio == GPIO_INPUT_DRBELL_NOTIF) {
        strcat(err, "Doorbell & ");
        cnt++;
    }
    if(_gpio == GPIO_INPUT_REEDSW_STATUS) {
        strcat(err, "Reed Switch & ");
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
    if(cnt > 1) {
        memmove(_err, err, strlen(err)-3);
    }
    else {
        free(_err);
        _err = NULL;
    }
}

static void gpio_assign_check(const char* TAG) {
    char* err = NULL;
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
}