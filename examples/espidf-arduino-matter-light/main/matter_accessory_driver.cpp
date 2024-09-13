/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <Arduino.h>
#include <esp_err.h>
#include <esp_matter_attribute_utils.h>
#include "builtinLED.h"
#include "matter_accessory_driver.h"

// set your board WS2812b pin here (e.g. 48 is the default pin for the ESP32-S3 devkit)
#define WS2812_PIN 48

/* Do any conversions/remapping for the actual value here */
esp_err_t light_accessory_set_power(void *led, uint8_t val)
{
    BuiltInLED *builtinLED = (BuiltInLED *) led;
    esp_err_t err = ESP_OK;
    if (val) {
        builtinLED->on();
    } else {
        builtinLED->off();
    }
    if (!builtinLED->write()) {
        err = ESP_FAIL;
    }
    log_i("LED set power: %d", val);
    return err;
}

esp_err_t light_accessory_set_brightness(void *led, uint8_t val)
{
    esp_err_t err = ESP_OK;
    BuiltInLED *builtinLED = (BuiltInLED *) led;
    int value = REMAP_TO_RANGE(val, MATTER_BRIGHTNESS, STANDARD_BRIGHTNESS);

    builtinLED->setBrightness(value);
    if (!builtinLED->write()) {
        err = ESP_FAIL;
    }
    log_i("LED set brightness: %d", value);
    return err;
}

esp_err_t light_accessory_set_hue(void *led, uint8_t val)
{
    esp_err_t err = ESP_OK;
    BuiltInLED *builtinLED = (BuiltInLED *) led;
    int value = REMAP_TO_RANGE(val, MATTER_HUE, STANDARD_HUE);
    led_indicator_color_hsv_t hsv = builtinLED->getHSV();
    hsv.h = value;
    builtinLED->setHSV(hsv);
    if (!builtinLED->write()) {
        err = ESP_FAIL;
    }
    log_i("LED set hue: %d", value);
    return err;
}

esp_err_t light_accessory_set_saturation(void *led, uint8_t val)
{
    esp_err_t err = ESP_OK;
    BuiltInLED *builtinLED = (BuiltInLED *) led;
    int value = REMAP_TO_RANGE(val, MATTER_SATURATION, STANDARD_SATURATION);
    led_indicator_color_hsv_t hsv = builtinLED->getHSV();
    hsv.s = value;
    builtinLED->setHSV(hsv);
    if (!builtinLED->write()) {
        err = ESP_FAIL;
    }
    log_i("LED set saturation: %d", value);
    return err;
}

esp_err_t light_accessory_set_temperature(void *led, uint16_t val)
{
    esp_err_t err = ESP_OK;
    BuiltInLED *builtinLED = (BuiltInLED *) led;
    uint32_t value = REMAP_TO_RANGE_INVERSE(val, STANDARD_TEMPERATURE_FACTOR);
    builtinLED->setTemperature(value);
    if (!builtinLED->write()) {
        err = ESP_FAIL;
    }
    log_i("LED set temperature: %ld", value);
    return err;
}

app_driver_handle_t light_accessory_init()
{
    /* Initialize led */
    static BuiltInLED builtinLED;

    const uint8_t pin = WS2812_PIN; // set your board WS2812b pin here
    builtinLED.begin(pin);
    builtinLED.setHSV({DEFAULT_HUE, DEFAULT_SATURATION, DEFAULT_BRIGHTNESS});
    builtinLED.write();    
    return (app_driver_handle_t) &builtinLED;
}