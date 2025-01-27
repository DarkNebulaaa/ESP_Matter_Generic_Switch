/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#pragma once

#include <esp_err.h>
#include <esp_matter.h>
#include <hal/gpio_types.h>

#define PLUGIN_GPIO GPIO_NUM_10
#define RESET_GPIO GPIO_NUM_9
#define LED_INDECATOR GPIO_NUM_11
#define GPIO_MODE GPIO_MODE_OUTPUT
struct gpio_button
{
      gpio_num_t GPIO_PIN_VALUE;
};


struct button_endpoint
{
    gpio_button* button;
    uint16_t endpoint;
};

extern int get_endpoint(gpio_button* button);

typedef void *app_driver_handle_t;

app_driver_handle_t app_driver_plugin_init();
/** Initialize the button driver
 *
 * This initializes the button driver associated with the selected board.
 *
 * @param[in] button Pointer to `gpio_button`.For boot button value is NULL.
 *
 * @return Handle on success.
 * @return NULL in case of failure.
 */
app_driver_handle_t app_driver_button_init();

/** Driver Update
 *
 * This API should be called to update the driver for the attribute being updated.
 * This is usually called from the common `app_attribute_update_cb()`.
 *
 * @param[in] endpoint_id Endpoint ID of the attribute.
 * @param[in] cluster_id Cluster ID of the attribute.
 * @param[in] attribute_id Attribute ID of the attribute.
 * @param[in] val Pointer to `esp_matter_attr_val_t`. Use appropriate elements as per the value type.
 *
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val);


esp_err_t update_gpio(esp_matter_attr_val_t *val);
