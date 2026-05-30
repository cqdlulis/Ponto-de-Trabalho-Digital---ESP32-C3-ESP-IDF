#ifndef TEMPO_WIFI_H
#define TEMPO_WIFI_H

#include <stdio.h>
#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_sntp.h"

#include "esp_log.h"
#include "esp_err.h"

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASS ""

void wifi_init(void);
void ntp_init(void);
void esperar_horario(void);
void tempo_init(void);

void wifi_init(void) {

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "Wokwi-GUEST",
            .password = "",
            .threshold.authmode = WIFI_AUTH_OPEN,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(
        esp_wifi_set_config(WIFI_IF_STA, &wifi_config)
    );

    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
}

void ntp_init(void) {
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();
}

void esperar_horario(void) {
    time_t now = 0;
    struct tm timeinfo = {0};

    while (timeinfo.tm_year < (2025 - 1900)) {
        time(&now);
        localtime_r(&now, &timeinfo);

        printf("Esperando horario NTP...\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    printf("Horario sincronizado!\n");
}

void tempo_init(void) {

    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init();
    vTaskDelay(pdMS_TO_TICKS(5000));
    ntp_init();
    esperar_horario();
    setenv("TZ", "BRT3", 1);
    tzset();
}

#endif