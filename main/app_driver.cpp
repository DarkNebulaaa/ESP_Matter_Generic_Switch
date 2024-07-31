/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_log.h>
#include <stdlib.h>
#include <string.h>
#include "driver/gpio.h"
#include "bsp/esp-bsp.h"
#include <esp_matter.h>
#include <app-common/zap-generated/attributes/Accessors.h>

#include <app_priv.h>

using namespace chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::cluster;

static const char *TAG = "app_driver";
extern uint16_t plugin_endpoint_id;

esp_err_t update_gpio(esp_matter_attr_val_t *val){
    esp_err_t err = ESP_OK;
    if(val->val.b){
        gpio_set_level(PLUGIN_GPIO, 1);
        ESP_LOGI(TAG, "PLUGIN_GPIO_HIGH");
    }else{
        gpio_set_level(PLUGIN_GPIO, 0);
        ESP_LOGI(TAG, "PLUGIN_GPIO_LOW");
    }
    
    return err;
}

esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;
    if(endpoint_id == plugin_endpoint_id){
        if(cluster_id == OnOff::Id){
            if(attribute_id == OnOff::Attributes::OnOff::Id){
                err = update_gpio(val);
            }
        }
    }
    return err;
}

app_driver_handle_t app_driver_plugin_init(){
    ESP_LOGI(TAG, "PLUGIN_GPIO_INIT");
    gpio_reset_pin(PLUGIN_GPIO);
    gpio_reset_pin(LED_INDECATOR);
    gpio_set_direction(PLUGIN_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_INDECATOR, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_INDECATOR, 0);
    return (app_driver_handle_t) NULL;
}

bool perform_factory_reset = false;
static void button_factory_reset_pressed_cb(void *arg, void *data)
{
    if (!perform_factory_reset) {
        ESP_LOGI(TAG, "Factory reset triggered. Release the button to start factory reset.");
        gpio_set_level(LED_INDECATOR, 1);
        perform_factory_reset = true;
    }
}

static void button_factory_reset_released_cb(void *arg, void *data)
{
    if (perform_factory_reset) {
        ESP_LOGI(TAG, "Starting factory reset");
        factory_reset();
        perform_factory_reset = false;
    }
}
static void button_single_click_cb(void *arg, void *data)
{
        ESP_LOGI(TAG, "button single click!");
}

app_driver_handle_t app_driver_button_init(){
    button_config_t gpio_btn_cfg = {
    .type = BUTTON_TYPE_GPIO,
    .long_press_time = CONFIG_BUTTON_LONG_PRESS_TIME_MS,
    .short_press_time = CONFIG_BUTTON_SHORT_PRESS_TIME_MS,
    .gpio_button_config = {
        .gpio_num = 9,
        .active_level = 0,
    },
};
    button_handle_t button_handle = iot_button_create(&gpio_btn_cfg);
    iot_button_register_cb(button_handle, BUTTON_LONG_PRESS_HOLD, button_factory_reset_pressed_cb, NULL);
    iot_button_register_cb(button_handle, BUTTON_PRESS_UP, button_factory_reset_released_cb, NULL);
    iot_button_register_cb(button_handle, BUTTON_SINGLE_CLICK, button_single_click_cb, NULL);
    return (app_driver_handle_t)button_handle;
}



