#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_https_server.h"


//USE https://slproweb.com/products/Win32OpenSSL.html if on windows to install open ssl 
// openssl.exe  req -newkey rsa:2048 -nodes -keyout prvtkey.pem -x509 -days 3650 -out cacert.pem -subj "/CN=cert name"


static esp_err_t on_url_hit(httpd_req_t *req)
{
    char *message = "hello secured world!";
    httpd_resp_send(req, message, strlen(message));
    return ESP_OK;
}

void https_server(void) 
{
    httpd_handle_t server = NULL;
    httpd_ssl_config_t config = HTTPD_SSL_CONFIG_DEFAULT();

    extern const unsigned char cacert_pem_start[] asm("_binary_cacert_pem_start");
    extern const unsigned char cacert_pem_end[]   asm("_binary_cacert_pem_end");
    config.cacert_pem = cacert_pem_start;
    config.cacert_len = cacert_pem_end - cacert_pem_start;

    extern const unsigned char prvtkey_pem_start[] asm("_binary_prvtkey_pem_start");
    extern const unsigned char prvtkey_pem_end[]   asm("_binary_prvtkey_pem_end");
    config.prvtkey_pem = prvtkey_pem_start;
    config.prvtkey_len = prvtkey_pem_end - prvtkey_pem_start;

    ESP_ERROR_CHECK(httpd_ssl_start(&server, &config));

    httpd_uri_t first_end_point_config = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = on_url_hit};
    httpd_register_uri_handler(server, &first_end_point_config);
    printf("server registered\n");

}