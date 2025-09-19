#include "cie_sntp.h"

static const char *TAG = "CIE_SNTP";

extern UI_state ui_state;
extern Send_168b_flag send_168b_flag;

void cie_initialize_sntp()
{
    ESP_LOGI(TAG, "Initializing SNTP");
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    // sntp_setservername(0, ""pool.ntp.org"");

    // 优先使用国内池服务器
    sntp_setservername(0, "0.cn.pool.ntp.org");
    sntp_setservername(1, "1.cn.pool.ntp.org");

    // 备用服务器
    sntp_setservername(2, "ntp.aliyun.com");
    sntp_setservername(3, "time.google.com");

    // sntp_setservername(0, "cn.ntp.org.cn"); // 设置NTP服务器  time.google.com  cn.ntp.org.cn
    sntp_init();
}

time_t now = 0;
void cie_get_sntp_time()
{

    if (now > 60)
        return;

    time(&now);

    if (now < 60)
    {
        ESP_LOGE(TAG, "Failed to obtain time");
    }
    else
    {
        // 获取UTC时间并转换为北京时间
        ui_state.beijing_time = now + BEIJING_TIMEZONE_OFFSET;
        struct tm *beijing_tm = localtime(&ui_state.beijing_time);

        char time_str[64];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", beijing_tm);
        ESP_LOGI(TAG, "get Beijing Time: %s", time_str);

        if (beijing_tm->tm_year == 70)
        {
            now = 0;
            return;
        }

        // 打印北京时间
        // ESP_LOGI(TAG, "get Beijing Time: %d", beijing_tm->tm_year);

        unsigned char year = (unsigned char)(beijing_tm->tm_year - 100);
        // ui_state.local_date.year = (unsigned char)(beijing_tm->tm_year - 100);
        ui_state.local_date.year = year; //(unsigned char)(beijing_tm->tm_year);
        ui_state.local_date.month = (unsigned char)(beijing_tm->tm_mon + 1);
        ui_state.local_date.date = (unsigned char)(beijing_tm->tm_mday);
        ui_state.local_date.hour = (unsigned char)(beijing_tm->tm_hour);
        ui_state.local_date.min = (unsigned char)(beijing_tm->tm_min);
        ui_state.local_date.sec = (unsigned char)(beijing_tm->tm_sec);

        send_168b_flag.set_net_time = true;
    }
}
