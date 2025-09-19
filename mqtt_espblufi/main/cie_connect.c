/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
// #include "test_etharp.h"
#include "lwip/udp.h"
#include "lwip/etharp.h"
#include "netif/ethernet.h"
#include "lwip/stats.h"
#include "lwip/prot/iana.h"

#include <string.h>
#include "protocol_examples_common.h"
#include "example_common_private.h"
#include "sdkconfig.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_wifi_default.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "cie_wifi.h"
#include "cie_state.h"

extern UI_state ui_state;
static const char *TAG = "cie_connect";

extern unsigned char cie_mqtt_info[MQTT_INFO_LENGTH];
extern unsigned char cie_wifi_info[WIFI_INFO_LENGTH];

#if CONFIG_EXAMPLE_CONNECT_IPV6
/* types of ipv6 addresses to be displayed on ipv6 events */
const char *example_ipv6_addr_types_to_str[6] = {
    "ESP_IP6_ADDR_IS_UNKNOWN",
    "ESP_IP6_ADDR_IS_GLOBAL",
    "ESP_IP6_ADDR_IS_LINK_LOCAL",
    "ESP_IP6_ADDR_IS_SITE_LOCAL",
    "ESP_IP6_ADDR_IS_UNIQUE_LOCAL",
    "ESP_IP6_ADDR_IS_IPV4_MAPPED_IPV6"};
#endif

/**
 * @brief Checks the netif description if it contains specified prefix.
 * All netifs created withing common connect component are prefixed with the module TAG,
 * so it returns true if the specified netif is owned by this module
 */
bool example_is_our_netif(const char *prefix, esp_netif_t *netif)
{
    return strncmp(prefix, esp_netif_get_desc(netif), strlen(prefix) - 1) == 0;
}

static bool netif_desc_matches_with(esp_netif_t *netif, void *ctx)
{
    return strcmp(ctx, esp_netif_get_desc(netif)) == 0;
}

esp_netif_t *get_example_netif_from_desc(const char *desc)
{
    return esp_netif_find_if(netif_desc_matches_with, (void *)desc);
}

static esp_err_t print_all_ips_tcpip(void *ctx)
{
    const char *prefix = ctx;
    // iterate over active interfaces, and print out IPs of "our" netifs
    esp_netif_t *netif = NULL;
    while ((netif = esp_netif_next_unsafe(netif)) != NULL)
    {
        if (example_is_our_netif(prefix, netif))
        {
            ESP_LOGI(TAG, "Connected to %s", esp_netif_get_desc(netif));
#if CONFIG_LWIP_IPV4
            esp_netif_ip_info_t ip;
            ESP_ERROR_CHECK(esp_netif_get_ip_info(netif, &ip));

            ESP_LOGI(TAG, "- IPv4 address: " IPSTR ",", IP2STR(&ip.ip));
#endif
#if CONFIG_EXAMPLE_CONNECT_IPV6
            esp_ip6_addr_t ip6[MAX_IP6_ADDRS_PER_NETIF];
            int ip6_addrs = esp_netif_get_all_ip6(netif, ip6);
            for (int j = 0; j < ip6_addrs; ++j)
            {
                esp_ip6_addr_type_t ipv6_type = esp_netif_ip6_get_addr_type(&(ip6[j]));
                ESP_LOGI(TAG, "- IPv6 address: " IPV6STR ", type: %s", IPV62STR(ip6[j]), example_ipv6_addr_types_to_str[ipv6_type]);
            }
#endif
        }
    }
    return ESP_OK;
}

void example_print_all_netif_ips(const char *prefix)
{
    // Print all IPs in TCPIP context to avoid potential races of removing/adding netifs when iterating over the list
    esp_netif_tcpip_exec(print_all_ips_tcpip, (void *)prefix);
}

// CIE_PROJECT
esp_err_t cie_connect(void)
{
#if 1 // CONFIG_EXAMPLE_CONNECT_WIFI
    ESP_LOGI(TAG, "goto cie_wifi_connect...");
    if (cie_wifi_connect() != ESP_OK) // 连接wifi
    {
        ui_state.wifi_state.connect_flag = DISABLE_0;
        return ESP_FAIL;
    }
    // ESP_ERROR_CHECK(esp_register_shutdown_handler(&example_wifi_shutdown));
    ESP_LOGI(TAG, "CIE example_wifi_connect over...");
#endif

#if 1
    static bool shutdown_handler_registered = false;
    // 确保只注册一次关闭处理程序
    if (!shutdown_handler_registered)
    {
        ESP_ERROR_CHECK(esp_register_shutdown_handler(&example_wifi_shutdown));
        shutdown_handler_registered = true;
    }
#endif

#if CONFIG_EXAMPLE_CONNECT_WIFI
    example_print_all_netif_ips(EXAMPLE_NETIF_DESC_STA);
#endif

    ui_state.wifi_state.connect_flag = ENABLE_1;

#if ENABLE_STATIC_IP
    cie_set_static_ip();
#endif

    return ESP_OK;
}

esp_err_t cie_set_static_ip(void)
{
    ui_state.static_ip = false;
#if 1                                            // CIE_MQTT_INFO_SELECT
    if (cie_wifi_info[IP_INFO_MODE_OFFSET] == 0) // 如果使用动态IP,则退出
        return ESP_OK;
#endif

    esp_netif_t *sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (sta_netif == NULL)
    {
        ESP_LOGE(TAG, "Failed to get STA netif");
        return ESP_FAIL;
    }

    // 停止DHCP服务
    esp_netif_dhcpc_stop(sta_netif);
    // ip_info[IP_MODE_OFFSET];

#if 1 // CIE_MQTT_INFO_SELECT
    esp_netif_ip_info_t ip_info;
    memset(&ip_info, 0, sizeof(ip_info));
    IP4_ADDR(&ip_info.ip, (int)cie_wifi_info[IP_INFO_ADDR_OFFSET], (int)cie_wifi_info[IP_INFO_ADDR_OFFSET + 1], (int)cie_wifi_info[IP_INFO_ADDR_OFFSET + 2], (int)cie_wifi_info[IP_INFO_ADDR_OFFSET + 3]);
    IP4_ADDR(&ip_info.netmask, (int)cie_wifi_info[IP_INFO_ZIWANG_OFFSET], (int)cie_wifi_info[IP_INFO_ZIWANG_OFFSET + 1], (int)cie_wifi_info[IP_INFO_ZIWANG_OFFSET + 2], (int)cie_wifi_info[IP_INFO_ZIWANG_OFFSET + 3]);
    IP4_ADDR(&ip_info.gw, (int)cie_wifi_info[IP_INFO_WANGGUAN_OFFSET], (int)cie_wifi_info[IP_INFO_WANGGUAN_OFFSET + 1], (int)cie_wifi_info[IP_INFO_WANGGUAN_OFFSET + 2], (int)cie_wifi_info[IP_INFO_WANGGUAN_OFFSET + 3]);
#else
    // 配置静态IP
    esp_netif_ip_info_t ip_info;
    memset(&ip_info, 0, sizeof(ip_info));
    IP4_ADDR(&ip_info.ip, 192, 168, 0, 191);      // 设置静态IP
    IP4_ADDR(&ip_info.gw, 192, 168, 0, 1);        // 设置网关
    IP4_ADDR(&ip_info.netmask, 255, 255, 255, 0); // 设置子网掩码
#endif

    esp_netif_set_ip_info(sta_netif, &ip_info);

    // 配置DNS服务器
    esp_netif_dns_info_t dns_info;
    memset(&dns_info, 0, sizeof(dns_info));
    IP4_ADDR(&dns_info.ip.u_addr.ip4, 8, 8, 8, 8); // 设置主DNS服务器
    esp_netif_set_dns_info(sta_netif, ESP_NETIF_DNS_MAIN, &dns_info);

    // 可选：配置备用DNS服务器
    IP4_ADDR(&dns_info.ip.u_addr.ip4, 8, 8, 4, 4); // 设置备用DNS服务器
    esp_netif_set_dns_info(sta_netif, ESP_NETIF_DNS_BACKUP, &dns_info);
    ui_state.static_ip = true;
    ESP_LOGI(TAG, "Static IP and DNS configured successfully");
    return ESP_OK;
}

esp_err_t cie_disconnect(void)
{
#if 1
    // 停止DHCP服务
    esp_netif_t *sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (sta_netif)
    {
        esp_netif_dhcpc_stop(sta_netif);
    }
    else
    {
        ESP_LOGW(TAG, "Failed to get STA network interface handle");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Stopping WiFi...");
    esp_err_t err = esp_wifi_stop();
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to stop WiFi: %s", esp_err_to_name(err));
        return err;
    }
    ESP_LOGI(TAG, "WiFi stopped successfully");

    // 销毁STA网络接口
    if (sta_netif)
    {
        esp_netif_destroy(sta_netif);
    }
    else
    {
        ESP_LOGW(TAG, "STA network interface already destroyed");
    }

    // 确保 example_wifi_sta_do_disconnect() 不会引用已销毁的资源
    example_wifi_sta_do_disconnect();

    return ESP_OK;

#else
    // 停止DHCP服务
    esp_netif_t *sta_netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (sta_netif)
    {
        esp_netif_dhcpc_stop(sta_netif);
    }

    ESP_LOGI(TAG, "stop before ");
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_LOGI(TAG, "stop after ");

    // 销毁STA网络接口
    if (sta_netif)
    {
        esp_netif_destroy(sta_netif);
    }

    // example_wifi_sta_do_disconnect();
    return ESP_OK;
#endif

#if CONFIG_EXAMPLE_CONNECT_ETHERNET
    example_ethernet_shutdown();
    ESP_ERROR_CHECK(esp_unregister_shutdown_handler(&example_ethernet_shutdown));
#endif
#if CONFIG_EXAMPLE_CONNECT_WIFI
    example_wifi_shutdown();
    ESP_ERROR_CHECK(esp_unregister_shutdown_handler(&example_wifi_shutdown));
#endif
    return ESP_OK;
}
