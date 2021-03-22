#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "cJSON.h"

static esp_err_t on_url_hit(httpd_req_t *req)
{
    if (req->content_len == 0)
    {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "invalid json 😭");
        return ESP_OK;
    }
    char *buffer = malloc(req->content_len) + 1;
    memset(buffer, 0, req->content_len + 1);

    httpd_req_recv(req, buffer, req->content_len);

    cJSON *json = cJSON_Parse(buffer);
    if (json)
    {
        cJSON *received_message = cJSON_GetObjectItemCaseSensitive(json, "message");
        if (received_message)
        {
            char *response_string = malloc(strlen(received_message->valuestring) + 200);
            sprintf(response_string, "received your message: %s. 👍", received_message->valuestring);
            cJSON_AddStringToObject(json, "response", response_string);
            char *response_payload = cJSON_Print(json);
            cJSON_Delete(json);
            httpd_resp_set_type(req, "application/json");
            httpd_resp_send(req, response_payload, strlen(response_payload));
            free(response_payload);
            return ESP_OK;
        }
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "message missing 😭");
        cJSON_Delete(json);
        return ESP_OK;
    }
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "invalid json 😭");
    return ESP_OK;
}

void json_server_post(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    ESP_ERROR_CHECK(httpd_start(&server, &config));

    httpd_uri_t first_end_point_config = {
        .uri = "/",
        .method = HTTP_POST,
        .handler = on_url_hit};
    httpd_register_uri_handler(server, &first_end_point_config);
    printf("server registered\n");
}