#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_http_client.h"

#define TAG "HTTPS CLIENT"

void https_client(void)
{
  extern const unsigned char wordtime_cert[] asm("_binary_worldtime_pem_start");
  
  esp_http_client_config_t clientConfig = {
      .url = "https://worldtimeapi.org/api/timezone/Europe/London/",
      //.url = "https://www.google.com",
      .cert_pem = (char *)wordtime_cert
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
    ESP_LOGE(TAG, "HTTPS GET request failed: %s", esp_err_to_name(err));
  }
  esp_http_client_close(client);

  while (true)
  {
    printf("done!\n");
    vTaskDelay(portMAX_DELAY);
  }
}
