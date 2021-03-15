/*
    Project:    ShutTheFrontDoor
    File:       stfd_gpio.c
    Author:     Benjamin Lanthier
    Date:       18/02/2021

    Desc;       This file contains the gpio setup and interrupts
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stfd_peripherals.h"
#include "stfd_gpio_config.h"

#define ESP_INTR_FLAG_DEFAULT 0

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

static const char* TAG = "stfd_gpio";

static const adc_bits_width_t battery_adc_width     = ADC_WIDTH_BIT_12;
static const adc_atten_t      battery_adc_atten     = ADC_ATTEN_DB_11;

static const adc_unit_t       bat_monitor_unit      = BATTERY_ADC_UNIT;
static const adc_channel_t    bat_monitor_channel   = BATTERY_ADC_CH;
static esp_adc_cal_characteristics_t* bat_monitor_chars = NULL;

// Forward Declaration of local functions
static void check_efuse(void);
static void print_char_val_type(esp_adc_cal_value_t val_type);

// ===== Peripherals functions =====

bool trig_valid_gpio(uint32_t io_num, uint8_t sg_level) {
    return gpio_get_level(io_num) == sg_level;
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
            mcu_content->trig_signal  = SIGNAL_LOW;
            break;
        case GPIO_INPUT_BATTERY:
            mcu_content->content_type = BATTERY;
            mcu_content->trig_signal  = SIGNAL_LOW;
            break;
        case GPIO_INPUT_DRBELL_NOTIF:
            mcu_content->content_type = DRBELL;
            mcu_content->trig_signal  = SIGNAL_LOW;
            break;
        case GPIO_INPUT_REEDSW_STATUS:
            mcu_content->content_type = REEDSW;
            mcu_content->trig_signal  = SIGNAL_LOW;
            break;
        case GPIO_OUTPUT_MOTOR_IN1:
        case GPIO_OUTPUT_MOTOR_IN2:
            mcu_content->content_type = MTR_CTRL;
            mcu_content->trig_signal  = SIGNAL_LOW;
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

void exec_toggle_motor(void) {
    ESP_LOGI(TAG, "toggle e-motor...");

    // Rotate Clockwise
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, 1);
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, 0);
    vTaskDelay(5000 / portTICK_RATE_MS);

    // Sleep mode
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, 0);
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, 0);
    vTaskDelay(1000 / portTICK_RATE_MS);

    // Rotate Counter-clockwise
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, 0);
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, 1);
    vTaskDelay(5000 / portTICK_RATE_MS);

    // Sleep mode
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN1, 0);
    gpio_set_level(GPIO_OUTPUT_MOTOR_IN2, 0);
    vTaskDelay(1000 / portTICK_RATE_MS);
}

uint32_t get_battery_level(void) {
    return get_adc_reading(bat_monitor_unit, bat_monitor_channel);
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

void gpio_setup_input(gpio_isr_t isr_handler) {
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
        GPIO_PIN_INTR_POSEDGE, 
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
        GPIO_PIN_INTR_POSEDGE, 
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

void gpio_setup_adc(void) {
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

void gpio_setup_output(void) {
    
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
    //Initialize to 0
    ESP_ERROR_CHECK(gpio_set_level((gpio_num_t) GPIO_OUTPUT_MOTOR_IN1_PIN_SEL, 0));
    ESP_ERROR_CHECK(gpio_set_level((gpio_num_t) GPIO_OUTPUT_MOTOR_IN2_PIN_SEL, 0));

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
    gpio_setup_input(isr_handler);
    gpio_setup_adc();
    gpio_setup_output();
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