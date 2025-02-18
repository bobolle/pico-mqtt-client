#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/mqtt.h"
#include <stdio.h>
#include <string.h>
#include "uss.h"

uint8_t trigger_pin = 17;
uint8_t echo_pin = 16;
uint8_t green_light_pin = 15;
uint8_t red_light_pin = 14;

static mqtt_client_t *mqtt_client;
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void mqtt_pub_request_cb(void *arg, err_t result);

int main() {
    stdio_init_all();
    uss_init(trigger_pin, echo_pin);

    if (cyw43_arch_init()) {
        printf("failed to initalise\n");
        return 1;
    }

    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWD, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
        printf("failed to connect\n");
        return 1;
    } else {
        printf("connected\n");
    }

    mqtt_client = mqtt_client_new();
    struct mqtt_connect_client_info_t client_info = {
        .client_id = "pico",
        .client_user = NULL,
        .client_pass = NULL,
        .keep_alive = 60,
        .will_topic = NULL,
        .will_msg = NULL,
        .will_qos = 0,
        .will_retain = 0
    };

    ip_addr_t broker_ip;
    ip4addr_aton("192.168.1.13", &broker_ip);

    err_t err = mqtt_client_connect(mqtt_client, &broker_ip, 1883, mqtt_connection_cb, NULL, &client_info);

    while (1) {

        const char *payload = "testing";
        err = mqtt_publish(mqtt_client, "pico/sensor/distance", payload, strlen(payload), 0, 0, mqtt_pub_request_cb, NULL);
        sleep_ms(500);

        if (err != ERR_OK) {
            break;
        }
    }

    cyw43_arch_deinit();
    return 0;
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    printf("connection status: %d\n", status);
}

static void mqtt_pub_request_cb(void *arg, err_t result) {
    if (result != ERR_OK) {
        printf("pub failed %d\n", result);
    } else {
        printf("pub success %d\n", result);
    }
}
