#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_http_client.h"

#define TAG "1 Simple GET"

void Simple_get(void)
{
  esp_http_client_config_t clientConfig = {
      .url = "http://worldtimeapi.org/api/timezone/Europe/London/",
  };

  esp_http_client_handle_t client = esp_http_client_init(&clientConfig);
  esp_err_t err = esp_http_client_perform(client);

  if (err == ESP_OK)
  {
    int length = esp_http_client_get_content_length(client);
    char *buffer = malloc(length + 1);
    memset(buffer, 0, length + 1);
    esp_http_client_read(client, buffer, length);
    printf("%s\n", buffer);
  }
  else
  {
    ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
  }
  esp_http_client_close(client);

  while (true)
  {
    printf("done!\n");
    vTaskDelay(portMAX_DELAY);
  }
}
