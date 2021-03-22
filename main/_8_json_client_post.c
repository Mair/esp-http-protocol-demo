#include <stdio.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "cJSON.h"

#define TAG "json client"

void json_client(void)
{

  cJSON * json = cJSON_CreateObject();
  cJSON_AddNumberToObject(json,"temperature", 25);
  char * mesage_payload = cJSON_Print(json);

  esp_http_client_config_t clientConfig = {
      .url = "http://192.168.0.116:2020",
      .method = HTTP_METHOD_POST
  };

  esp_http_client_handle_t client = esp_http_client_init(&clientConfig);
  esp_http_client_set_header(client,"CONTENT-TYPE","application/json");
  esp_http_client_set_post_field(client,mesage_payload, strlen((mesage_payload)));
  esp_err_t err = esp_http_client_perform(client);

  if (err == ESP_OK)
  {
    int length = esp_http_client_get_content_length(client);
    char *response_payload = malloc(length + 1);
    memset(response_payload, 0, length + 1);
    esp_http_client_read(client, response_payload, length);
    cJSON *json_response = cJSON_Parse(response_payload);
    cJSON *response_message = cJSON_GetObjectItem(json_response, "message");
    if(response_message)
    {
      printf("%s\n", response_message->valuestring);
    }
    cJSON_Delete(json_response);
  }
  else
  {
    ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
  }
  esp_http_client_close(client);

  cJSON_Delete(json);
  free(mesage_payload);
}
