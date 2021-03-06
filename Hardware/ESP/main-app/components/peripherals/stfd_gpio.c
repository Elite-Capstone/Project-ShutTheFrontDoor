/*
    Project:    ShutTheFrontDoor
    File:       stfd_gpio.c
    Author:     Benjamin Lanthier
    Date:       18/02/2021

    Desc;       This file contains the gpio setup and interrupts
*/

#ifndef STFD_GPIO_C_
#define STFD_GPIO_C_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stfd_peripherals.h"
#include "stfd_gpio_config.h"

#define ESP_INTR_FLAG_DEFAULT 0

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

#define MOTOR_FAULT_COND_CNT 10
#define MOTOR_FULL_CYCLE_MS 2600

#define TIMER_DIVIDER 16
#define TIMER_SCALE (TIMER_BASE_CLK / TIMER_DIVIDER)
#define MOTOR_AUTOLOCK_TIMER 10 // 5min,  *TIMER_SCALE to use in setting timer values
#define CAM_SERVER_TIMER 60    // 5min


static const char* TAG = "stfd_gpio";

static const adc_bits_width_t battery_adc_width     = ADC_WIDTH_BIT_12;
static const adc_atten_t      battery_adc_atten     = ADC_ATTEN_DB_11;

static const adc_unit_t       bat_monitor_unit      = BATTERY_ADC_UNIT;
static const adc_channel_t    bat_monitor_channel   = BATTERY_ADC_CH;
static esp_adc_cal_characteristics_t* bat_monitor_chars = NULL;

const timer_group_t autotimer_group       = TIMER_GROUP_0;
const timer_idx_t   lock_timer_num        = TIMER_0;
const timer_idx_t   camserver_timer_num   = TIMER_1;

const uint32_t adc_val[99] = {
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

const uint32_t bat_val[100] = {
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

// Forward Declaration of local functions
static void check_efuse(void);
static void print_char_val_type(esp_adc_cal_value_t val_type);
static void exec_brake_motor(void);

// ===== Peripherals functions =====

bool trig_valid_gpio(uint32_t io_num, gpio_sig_level_t sg_level) {
    if (sg_level == SIGNAL_ANY)
        return true;
    else
        return gpio_get_level(io_num) == (uint8_t) sg_level;
}

// Door is closed when Reed switch is open, i.e. the line is 
// still pulled up by the resistor
bool get_door_is_closed(void) {
    if (get_reedsw_pos() == SW_OPEN)
        return true;
    else return false;
}

bool get_door_is_locked(void) {
    if (get_nsw_pos() == SW_CLOSED)
        return true;
    else return false; // if (get_nsw_pos() == NSW_CLOSED);
}

// Locked position is closed N-switch
// Unlocked is open N-switch
sw_pos_t get_nsw_pos(void) {
    
    if (gpio_get_level(GPIO_INPUT_NSW) == SIGNAL_HIGH) {
        ESP_LOGI(TAG, "N-switch is open");
        return SW_OPEN;
    }
    else if (gpio_get_level(GPIO_INPUT_NSW) == SIGNAL_LOW) {
        ESP_LOGI(TAG, "N-switch is closed");
        return SW_CLOSED;
    }
    else {
        ESP_LOGW(TAG, "N-switch position unknown");
        return SW_UNKNOWN;
    }
}

sw_pos_t get_reedsw_pos(void) {
    if (gpio_get_level(GPIO_INPUT_REEDSW_STATUS) == SIGNAL_HIGH) {
        ESP_LOGI(TAG, "Reed switch is open");
        return SW_OPEN;
    }
    else if (gpio_get_level(GPIO_INPUT_REEDSW_STATUS) == SIGNAL_LOW) {
        ESP_LOGI(TAG, "Reed switch is closed");
        return SW_CLOSED;
    }
    else {
        ESP_LOGW(TAG, "Reed switch position unknown");
        return SW_UNKNOWN;
    }
}

// bool trig_motor_gpio(uint32_t io_num, uint8_t sg_level) {
//     if (io_num == GPIO_INPUT_BOOT)
//         return gpio_get_level(io_num) == sg_level;
//     return false;
// }

void get_io_type(uint32_t io_num, mcu_content_t* mcu_content) {
    switch(io_num) {
        case GPIO_INPUT_MS:
            mcu_content->content_type = MS;
            mcu_content->trig_signal  = SIGNAL_HIGH;
            break;
        case GPIO_INPUT_NSW:
            mcu_content->content_type = NSW;
            mcu_content->trig_signal  = SIGNAL_ANY;
            break;
        case GPIO_INPUT_BATTERY:
            mcu_content->content_type = BATTERY;
            mcu_content->trig_signal  = SIGNAL_LOW;
            break;
        case GPIO_INPUT_DRBELL_NOTIF:
            mcu_content->content_type = DRBELL;
            mcu_content->trig_signal  = SIGNAL_HIGH;
            break;
        case GPIO_INPUT_REEDSW_STATUS:
            mcu_content->content_type = REEDSW;
            mcu_content->trig_signal  = SIGNAL_ANY;
            break;
        case GPIO_INPUT_MOTOR_FAULT:
            mcu_content->content_type = MTR_CTRL;
            mcu_content->trig_signal  = SIGNAL_LOW;
            break;
        case GPIO_OUTPUT_MOTOR_IN1:
        case GPIO_OUTPUT_MOTOR_IN2:
            mcu_content->content_type = MTR_CTRL;
            mcu_content->trig_signal  = SIGNAL_IGNORED;
            break;
        case GPIO_INPUT_BOOT:
            mcu_content->content_type = STANDBY;
            mcu_content->trig_signal  = SIGNAL_LOW;
            break;
        default:
            mcu_content->content_type = (mcu_content_type_t) INVALID;
            mcu_content->trig_signal  = SIGNAL_LOW;
    }
}

mcu_content_type_t return_io_type(uint32_t io_num) {
    switch(io_num) {
        case GPIO_INPUT_MS:
            return MS;
        case GPIO_INPUT_NSW:
            return NSW;
        case GPIO_INPUT_BATTERY:
            return BATTERY;
        case GPIO_INPUT_DRBELL_NOTIF:
            return DRBELL;
        case GPIO_INPUT_REEDSW_STATUS:
           return REEDSW;
        case GPIO_INPUT_MOTOR_FAULT:
            return MTR_CTRL;
        case GPIO_OUTPUT_MOTOR_IN1:
        case GPIO_OUTPUT_MOTOR_IN2:
            return MTR_CTRL;
        case GPIO_INPUT_BOOT:
            return STANDBY;
        default:
            return INVALID;
    }
}

static void exec_brake_motor(void) {
    // Braking
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, 1);
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, 1);
    vTaskDelay(100 / portTICK_RATE_MS);
    // Sleep
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, 0);
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, 0);
}

void exec_toggle_motor(void) {
    ESP_LOGI(TAG, "toggle e-motor...");
    int lock = 1;

    // If initially locked, start by unlocking
    if (get_nsw_pos() == SW_CLOSED)
        lock = 0;
    else if (get_nsw_pos() == SW_OPEN)
        lock = 1;
    else {
        ESP_LOGE(TAG, "Unknown N-switch position, will not operate toggle");
        return;
    }

    gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, lock);
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, !lock);
    vTaskDelay(MOTOR_FULL_CYCLE_MS / portTICK_RATE_MS);

    // Brake mode
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, 1);
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, 1);
    vTaskDelay(1000 / portTICK_RATE_MS);

    gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, !lock);
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, lock);
    vTaskDelay(MOTOR_FULL_CYCLE_MS / portTICK_RATE_MS);

    exec_brake_motor();
}

stfd_lock_err_t exec_operate_lock(bool lock) {
    //int operation_cnt = 0;
    sw_pos_t old_pos = get_nsw_pos();
    /*
    When the door is already locked (bolt out), the N-switch is in closed position, right at the end
    WHen the door is unlocked (bolt in), the N-switch is in open position for most the rotation
    */
    if (old_pos == SW_UNKNOWN)
        return LOCK_POS_UNKNOWN;
    else if (lock && (old_pos == SW_CLOSED))
        return LOCK_ALREADY_CLOSED;
    else if (!lock && (old_pos == SW_OPEN))
        return LOCK_ALREADY_OPEN;
    else if (lock) {
        // CW
        ESP_LOGI(TAG, "Closing door lock");
        gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, 0);
        gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, 1);
    }
    else if (!lock) {
        // CCW
        ESP_LOGI(TAG, "Opening door lock");
        gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, 1);
        gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, 0);
    }

    vTaskDelay(MOTOR_FULL_CYCLE_MS / portTICK_RATE_MS);

    // while ((lock && gpio_get_level(GPIO_INPUT_MOTOR_FAULT) == SIGNAL_HIGH && get_nsw_pos() == NSW_OPEN) ||
    //       (!lock && gpio_get_level(GPIO_INPUT_MOTOR_FAULT) == SIGNAL_HIGH)) 
    // {
    //     operation_cnt++;
    //     vTaskDelay(50 / portTICK_RATE_MS);
    //     if (operation_cnt > MOTOR_NORMAL_CYCLE_CNT) {
    //         ESP_LOGE(TAG, "Lock/Unlock operation cannot complete succesfully - aborting");
    //         exec_brake_motor();
    //         return LOCK_FAIL;
    //     }
    // }

    // End of operation
    exec_brake_motor();
    return LOCK_OK;
}

/* 
When the lock initially operates, it generates a stall current (high inrush current).
This forces FAULTn output low as it wakes up from sleep mode.
As long as the "fault" condition does not persist, the operation may continue normally.
*/
stfd_lock_err_t check_motor_fault_cond(void) {
    int operation_cnt = 0;
    while (gpio_get_level(GPIO_INPUT_MOTOR_FAULT) == SIGNAL_LOW) {
        operation_cnt++;
        vTaskDelay(100 / portTICK_RATE_MS);
        if (operation_cnt > MOTOR_FAULT_COND_CNT) {
            // Fault condition persisted
            ESP_LOGE(TAG, "Fault condition detected from motor lock - Stopping lock");
            exec_brake_motor();
            
            return LOCK_MOTOR_FAULT;
        }
    }
    operation_cnt = 0;
    return LOCK_OK;
}

void gpio_begin_stream(void) {
    gpio_set_level(GPIO_OUTPUT_STREAM, 1);
    vTaskDelay(500 / portTICK_RATE_MS);
    gpio_set_level(GPIO_OUTPUT_STREAM, 0);
}

static void start_auto_timer(timer_group_t group_num, timer_idx_t timer_num, uint64_t load_val) {
    double counter_time = 0;
    timer_get_counter_time_sec(group_num, timer_num, &counter_time);
    if (counter_time > 0) {
        ESP_LOGI(TAG, "Timer reset at %f", counter_time);
    }
    timer_pause(group_num, timer_num);
    timer_set_counter_value(group_num, timer_num, load_val * TIMER_SCALE);
    timer_set_alarm(group_num, timer_num, TIMER_ALARM_EN);
    timer_start(group_num, timer_num);
}

void stfd_start_autolock_timer(void) {
    start_auto_timer(autotimer_group, lock_timer_num, MOTOR_AUTOLOCK_TIMER);
}

void stfd_stop_autolock_timer(void) {
    timer_pause(autotimer_group, lock_timer_num);
}

void stfd_start_camserver_timer(void) {
    start_auto_timer(autotimer_group, camserver_timer_num, CAM_SERVER_TIMER);
}

uint32_t get_battery_level(void) {
    uint32_t adc_mV = get_adc_reading(bat_monitor_unit, bat_monitor_channel);
    // If battery is full
    if (adc_mV == adc_val[0]) {
        ESP_LOGI(TAG, "Battery full charge");
        return bat_val[0];
    }
    else if (adc_mV > adc_val[98]) {
        ESP_LOGW(TAG, "Battery is completely discharged!");
        return 0;
    }
    for (uint16_t i = 1; i < sizeof(adc_val)/sizeof(uint32_t); i++) {
        if (adc_mV > adc_val[i-1] && adc_mV <= adc_val[i]) {
            ESP_LOGI(TAG, "Battery voltage is : %i", bat_val[i]);
            return bat_val[i];
        }
    }
    ESP_LOGW(TAG, "Could not read battery voltage properly");
    return 0;
}

uint32_t get_adc_reading(adc_unit_t unit, adc_channel_t channel) {

    uint32_t adc_reading = 0;

    //Multisampling
    for (int i = 0; i < NO_OF_SAMPLES; i++) {
        if (unit == ADC_UNIT_1) {
            adc_reading += adc1_get_raw((adc1_channel_t)channel);
        } else {
            int raw;
            adc2_get_raw((adc2_channel_t)channel, battery_adc_width, &raw);
            adc_reading += raw;
        }
    }
    adc_reading /= NO_OF_SAMPLES;

    //Convert adc_reading to voltage in mV
    uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, bat_monitor_chars);
    ESP_LOGI(TAG, "Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
    return voltage;
}

esp_err_t stfd_gpio_config(GPIO_INT_TYPE int_type, uint64_t bit_mask, gpio_mode_t gpio_mode, gpio_pulldown_t pull_down, gpio_pullup_t pull_up) {
    gpio_config_t io_conf;

    ///interrupt of rising edge
    io_conf.intr_type = int_type;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = bit_mask;
    //set as input mode    
    io_conf.mode = gpio_mode;

    if(pull_up && pull_down == 0x1) {
        ESP_LOGE(TAG, "Simulaneous Pull up and Pull down requested");
        return ESP_FAIL;
    }
    else {
        // enable/disable pull-up mode
        io_conf.pull_up_en = pull_up;
        // enable/disable pull-down mode
        io_conf.pull_down_en = pull_down;
    }
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    return ESP_OK;
}

static void gpio_setup_input(gpio_isr_t isr_handler) {
    // BOOT
    if( stfd_gpio_config(
        GPIO_PIN_INTR_NEGEDGE, 
        GPIO_INPUT_BOOT_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_ENABLE
        ) != ESP_OK) 
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_MS);
    }
    // MS
    if( stfd_gpio_config(
        GPIO_PIN_INTR_POSEDGE, 
        GPIO_INPUT_MS_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_ENABLE, 
        GPIO_PULLUP_DISABLE
        ) != ESP_OK) 
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_MS);
    }
    // DRBELL
    if( stfd_gpio_config(
        GPIO_PIN_INTR_POSEDGE, 
        GPIO_INPUT_DRBELL_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_ENABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_DRBELL_NOTIF);
    }
    // REEDSW
    if( stfd_gpio_config(
        GPIO_PIN_INTR_ANYEDGE, 
        GPIO_INPUT_REEDSW_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_ENABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_REEDSW_STATUS);
    }
#if CONFIG_MAIN_MCU
    // NSW
    if( stfd_gpio_config(
        GPIO_PIN_INTR_ANYEDGE, 
        GPIO_INPUT_NSW_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_ENABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_NSW);
    }
    // MOTOR FAULT
    if( stfd_gpio_config(
        GPIO_PIN_INTR_POSEDGE, 
        GPIO_INPUT_MOTOR_FAULT_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_ENABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_MOTOR_FAULT);
    }

#elif CONFIG_ESP32_CAM_MCU
    // PIC
    if( stfd_gpio_config(
        GPIO_PIN_INTR_NEGEDGE, 
        GPIO_INPUT_PIC_PIN_SEL, 
        GPIO_MODE_INPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_ENABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_PIC);
    }
#endif

    //install gpio isr service
    //hook isr handler for specific gpio pin
    //Must only be done once. But each gpio can have their own isr handler
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_BOOT, isr_handler, (void*) GPIO_INPUT_BOOT));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_MS, isr_handler, (void*) GPIO_INPUT_MS));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_DRBELL_NOTIF, isr_handler, (void*) GPIO_INPUT_DRBELL_NOTIF));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_REEDSW_STATUS, isr_handler, (void*) GPIO_INPUT_REEDSW_STATUS));

#if CONFIG_MAIN_MCU
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_NSW, isr_handler, (void*) GPIO_INPUT_NSW));
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_MOTOR_FAULT, isr_handler, (void*) GPIO_INPUT_MOTOR_FAULT));
#elif CONFIG_ESP32_CAM_MCU
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_PIC, isr_handler, (void*) GPIO_INPUT_PIC));
#endif
}

static void gpio_setup_adc(void) {
#if CONFIG_MAIN_MCU

    //Check if Two Point or Vref are burned into eFuse
    check_efuse();

    //Configure ADC
    if (bat_monitor_unit == ADC_UNIT_1) {
        adc1_config_width(battery_adc_width);
        adc1_config_channel_atten(bat_monitor_channel, battery_adc_atten);
    } else {
        adc2_config_channel_atten((adc2_channel_t)bat_monitor_channel, battery_adc_atten);
    }

    //Characterize ADC
    bat_monitor_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(bat_monitor_unit, 
                                                            battery_adc_atten, 
                                                            battery_adc_width, 
                                                            adc2_vref_to_gpio(GPIO_INPUT_BATTERY), 
                                                            bat_monitor_chars
                                                            );
    print_char_val_type(val_type);

    // if( stfd_gpio_config(
    //     GPIO_PIN_INTR_POSEDGE, 
    //     GPIO_INPUT_BATTERY_PIN_SEL, 
    //     GPIO_MODE_INPUT, 
    //     GPIO_PULLDOWN_DISABLE, 
    //     GPIO_PULLUP_ENABLE
    //     ) != ESP_OK)
    // {
    //     ESP_LOGE(TAG, "GPIO %i config failed", GPIO_INPUT_BATTERY);
    // }
    
#endif
}

static void gpio_setup_output(void) {
    
#if CONFIG_MAIN_MCU
    if( stfd_gpio_config(
        GPIO_PIN_INTR_DISABLE, 
        GPIO_OUTPUT_MOTOR_IN1_PIN_SEL, 
        GPIO_MODE_OUTPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_DISABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_OUTPUT_MOTOR_IN1);
    }
    if( stfd_gpio_config(
        GPIO_PIN_INTR_DISABLE, 
        GPIO_OUTPUT_MOTOR_IN2_PIN_SEL, 
        GPIO_MODE_OUTPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_DISABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_OUTPUT_MOTOR_IN2);
    }
    if( stfd_gpio_config(
        GPIO_PIN_INTR_DISABLE, 
        GPIO_OUTPUT_STREAM_PIN_SEL, 
        GPIO_MODE_OUTPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_DISABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_OUTPUT_STREAM);
    }

    //Initialize to 0
    ESP_ERROR_CHECK(gpio_set_level((gpio_num_t) GPIO_OUTPUT_MOTOR_IN1_PIN_SEL, 0));
    ESP_ERROR_CHECK(gpio_set_level((gpio_num_t) GPIO_OUTPUT_MOTOR_IN2_PIN_SEL, 0));
    ESP_ERROR_CHECK(gpio_set_level((gpio_num_t) GPIO_OUTPUT_STREAM_PIN_SEL, 0));

#elif CONFIG_ESP32_CAM_MCU
    if( stfd_gpio_config(
        GPIO_PIN_INTR_DISABLE, 
        GPIO_OUTPUT_CONFIRM_PIN_SEL, 
        GPIO_MODE_OUTPUT, 
        GPIO_PULLDOWN_DISABLE, 
        GPIO_PULLUP_DISABLE
        ) != ESP_OK)
    {
        ESP_LOGE(TAG, "GPIO %i config failed", GPIO_OUTPUT_CONFIRM_UPLOAD);
    }
    ESP_ERROR_CHECK(gpio_set_level((gpio_num_t) GPIO_OUTPUT_CONFIRM_PIN_SEL, 0));
#endif
}

void gpio_init_setup(gpio_isr_t isr_handler) {
    gpio_overlap_check(TAG);
    ESP_LOGI(TAG, "GPIO setup...");
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    gpio_setup_input(isr_handler);
    gpio_setup_adc();
    gpio_setup_output();
}

// The timer value input is in seconds
static void single_timer_setup(timer_config_t timer_config, timer_isr_t isr_handler, timer_group_t timer_group, timer_idx_t timer_idx, uint64_t timer_val, uint64_t alarm_val) {
    timer_init(timer_group, timer_idx, &timer_config);
    timer_set_counter_value(timer_group, timer_idx, timer_val * TIMER_SCALE);
    timer_set_alarm_value(timer_group, timer_idx, alarm_val * TIMER_SCALE);
    timer_enable_intr(timer_group, timer_idx);
    timer_isr_register(timer_group, timer_idx, isr_handler, (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);   
}

void timer_init_setup(timer_isr_t isr_handler) {
    ESP_LOGI(TAG, "Timer setup...");
    timer_config_t timer_config = {
        .divider     = TIMER_DIVIDER,
        .counter_en  = TIMER_PAUSE,
        .counter_dir = TIMER_COUNT_DOWN,
        .alarm_en    = TIMER_ALARM_EN,
        .auto_reload = TIMER_AUTORELOAD_DIS,
    };
    single_timer_setup(timer_config, isr_handler, autotimer_group, lock_timer_num, MOTOR_AUTOLOCK_TIMER, 0x00000000ULL);
    single_timer_setup(timer_config, isr_handler, autotimer_group, camserver_timer_num, CAM_SERVER_TIMER, 0x00000000ULL);
}

static void check_efuse(void)
{
#if CONFIG_IDF_TARGET_ESP32
    //Check if TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }
    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
#elif CONFIG_IDF_TARGET_ESP32S2
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("Cannot retrieve eFuse Two Point calibration values. Default calibration values will be used.\n");
    }
#else
#error "This example is configured for ESP32/ESP32S2."
#endif
}

static void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}

#endif /* STFD_GPIO_C_ */