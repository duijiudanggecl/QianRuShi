#include "cie_tool.h"

static const char *TAG = "cie_tool";
extern UI_state ui_state;

static UINT8 get_utf8_bytes(UINT8 utf8)
{
    for (UINT8 i = 0; i < 6; i++)
    {
        if ((utf8 & (0x80 >> i)) == 0)
        {
            return i == 0 ? 1 : i;
        }
    }

    return 1;
}

UINT16 utf8_TO_utf16(UINT8 *in, UINT16 inLen, UINT16 *out, UINT16 outLen)
{
    UINT16 length = 0;
    UINT8 bytes = 0;

    for (UINT16 i = 0; i < inLen && length < outLen; i++, length++)
    {
        bytes = get_utf8_bytes(in[i]);

        if (bytes > 1)
        {
            out[length] = in[i] & (0xFF >> (bytes + 1));
            for (UINT8 j = 1; j < bytes; j++)
            {
                i++;
                out[length] <<= 6;
                out[length] += in[i] & 0x3F;
            }
        }
        else
        {
            out[length] = in[i];
        }
    }

    out[length] = 0;
    return length;
}

char CRC16_addition(unsigned char *pData, UINT16 length)
{
    char len;
    short reg_crc = 0x00;

    len = length;

    while (len--)
    {
        reg_crc = ((*pData + reg_crc) & 0xff);
        pData++;
    }

    return (reg_crc);
}

void cie_clear_wifi_ssid_pw(void)
{
    memset(ui_state.wifi_state.SSID, 0, 32);
    memset(ui_state.wifi_state.PASSWORD, 0, 64);
}

void cie_reconnect_smart_config(void)
{
    cie_disconnect();
    // cie_net_init();
}

void cie_clear_SSID_PW_from_flash(void)
{
    // 打开NVS命名空间
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // 写入字符串
    err = nvs_set_str(my_handle, "ssid", "");
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to write SSID");
        nvs_close(my_handle);
        return;
    }

    err = nvs_set_str(my_handle, "password", "");
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to write PASSWORD");
        nvs_close(my_handle);
        return;
    }

    // 提交更改
    err = nvs_commit(my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to commit changes");
        nvs_close(my_handle);
        return;
    }

    // 关闭NVS句柄
    nvs_close(my_handle);
}

extern char server_url[64];

void cie_read_param_from_flash(void)
{
    nvs_handle_t my_handle;
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 确保命名空间存在
    ret = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(ret));
        if (ret == ESP_ERR_NVS_NOT_INITIALIZED)
        {
            ESP_LOGE(TAG, "NVS not initialized!");
        }
        else if (ret == ESP_ERR_NVS_NOT_FOUND)
        {
            ESP_LOGE(TAG, "Namespace 'storage' not found, creating it...");
            ESP_ERROR_CHECK(nvs_flash_init_partition("storage"));
            ESP_ERROR_CHECK(nvs_open("storage", NVS_READWRITE, &my_handle));
        }
        return;
    }
    nvs_close(my_handle);

    // 读取凭据
    ret = nvs_open("storage", NVS_READONLY, &my_handle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(ret));
        return;
    }

    size_t required_size = sizeof(ui_state.wifi_state.SSID);
    ret = nvs_get_str(my_handle, "ssid", ui_state.wifi_state.SSID, &required_size);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to read SSID from NVS");
    }

    required_size = sizeof(server_url);
    ret = nvs_get_str(my_handle, "server_url", server_url, &required_size);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to read server_url from NVS");
    }

    required_size = sizeof(ui_state.wifi_state.smartconfig);
    ret = nvs_get_str(my_handle, "smartconfig", ui_state.wifi_state.smartconfig, &required_size);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to read smartconfig from NVS");
    }

    required_size = sizeof(ui_state.wifi_state.PASSWORD);
    ret = nvs_get_str(my_handle, "password", ui_state.wifi_state.PASSWORD, &required_size);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to read PASSWORD from NVS");
    }

    nvs_close(my_handle);
}

void cie_write_server_url_to_flash(void)
{
    // 打开NVS命名空间
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // 写入字符串
    err = nvs_set_str(my_handle, "server_url", server_url);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to write server_url");
        nvs_close(my_handle);
        return;
    }

    // 提交更改
    err = nvs_commit(my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to commit changes");
        nvs_close(my_handle);
        return;
    }

    // 关闭NVS句柄
    nvs_close(my_handle);
}




void cie_write_smartconfig_disable_from_flash(void)
{
    // 打开NVS命名空间
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // 写入字符串
    err = nvs_set_str(my_handle, "smartconfig", "0");
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to write smarfconfig");
        nvs_close(my_handle);
        return;
    }

    // 提交更改
    err = nvs_commit(my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to commit changes");
        nvs_close(my_handle);
        return;
    }

    // 关闭NVS句柄
    nvs_close(my_handle);
}

void cie_write_smartconfig_enable_from_flash(void)
{
    // 打开NVS命名空间
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        return;
    }

    // 写入字符串
    err = nvs_set_str(my_handle, "smartconfig", "1");
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to write smarfconfig");
        nvs_close(my_handle);
        return;
    }

    // 提交更改
    err = nvs_commit(my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to commit changes");
        nvs_close(my_handle);
        return;
    }

    // 关闭NVS句柄
    nvs_close(my_handle);
}