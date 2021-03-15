#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_http_client.h"
#include "web_demo.h"

void OnConnected(void *params)
{
  //comment out all except 1
  //Simple_get();  
 // Simple_get_cb();
  simple_server();

  //leave this in place
  vTaskDelete(NULL);
}

void app_main(void)
{
  connect_simple();
  xTaskCreate(&OnConnected, "handel comms", 1024 * 10, NULL, 5, NULL);
}
