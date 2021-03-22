#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "cJSON.h"

static esp_err_t on_url_hit(httpd_req_t *req)
{
  cJSON *payload = cJSON_CreateObject();
  cJSON_AddStringToObject(payload, "first_name", "John");
  cJSON_AddStringToObject(payload, "sur_name", "Smith");
  cJSON_AddNumberToObject(payload, "age", 21);
  cJSON *pets_array = cJSON_AddArrayToObject(payload, "pets");
  cJSON *pet1 = cJSON_CreateObject();
  cJSON_AddItemToArray(pets_array, pet1);
  cJSON_AddStringToObject(pet1, "type", "dog");
  cJSON_AddStringToObject(pet1, "name", "Bob");
  cJSON *pet2 = cJSON_CreateObject();
  cJSON_AddItemToArray(pets_array, pet2);
  cJSON_AddStringToObject(pet2, "type", "cat");
  cJSON_AddStringToObject(pet2, "name", "Bill");

  char *message = cJSON_Print(payload);
  cJSON_Delete(payload);
  printf("sending %s\n", message);

  httpd_resp_set_type(req, "application/json");
  httpd_resp_send(req, message, strlen(message));

  free(message);

  return ESP_OK;
}

void json_server_responce(void)
{
  httpd_handle_t server = NULL;
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  ESP_ERROR_CHECK(httpd_start(&server, &config));

  httpd_uri_t first_end_point_config = {
      .uri = "/",
      .method = HTTP_GET,
      .handler = on_url_hit};
  httpd_register_uri_handler(server, &first_end_point_config);
  printf("server registered\n");
}