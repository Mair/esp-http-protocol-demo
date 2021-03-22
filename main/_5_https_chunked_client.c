#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_http_client.h"

#define TAG "1 Simple GET"



typedef struct data_received_t
{
  char *body;
  int content_len;
} data_received_t;

esp_err_t get_device_data_handler(esp_http_client_event_t *evt)
{
  data_received_t *data_received = evt->user_data;
  switch (evt->event_id)
  {
  case HTTP_EVENT_ON_DATA:
    data_received->body = realloc(data_received->body, data_received->content_len + evt->data_len + 1);
    if (!data_received->body)
    {
      ESP_LOGE(TAG, "OUT OF MEMORY");
      return -1;
    }
    memmove(&data_received->body[data_received->content_len], evt->data, evt->data_len);
    data_received->content_len += evt->data_len;
    data_received->body[data_received->content_len] = 0;
    break;
  default:
    break;
  }
  return ESP_OK;
}

void https_chunked_client(void)
{
  //only do this for small payloads
  data_received_t data_received = {
      .body = malloc(10),
      .content_len = 0};
  memset(data_received.body, 0, 10);

  extern const unsigned char google_cert[] asm("_binary_google_pem_start");

  esp_http_client_config_t clientConfig = {
      .url = "https://www.google.com/",
      //.url= "https://www.bing.com/",
      .event_handler = get_device_data_handler,
      .user_data = &data_received,
      .cert_pem = (const char *)google_cert
  };

  esp_http_client_handle_t client = esp_http_client_init(&clientConfig);
  esp_err_t err = esp_http_client_perform(client);

  if (err == ESP_OK)
  {
    printf("%s\n", data_received.body);
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
