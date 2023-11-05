#ifndef _BSP_RTC_H_

#define _BSP_RTC_H_

#include "imx6ull.h"

#define SECONDS_IN_A_DAY    (86400)
#define SECONDS_IN_A_HOUR   (3600)
#define SECONDS_IN_A_MINUTE (60)
#define DAYS_IN_A_YEAR      (365)
#define YEAR_RANGE_START    (1970)
#define YEAR_RANGE_END      (2099)

struct rtc_datetime
{
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
};

void rtc_init(void);

void rtc_enable(void);

void rtc_disable(void);

unsigned char rtc_isleapyear(unsigned short year);

unsigned int rtc_covertdate_to_seconds(struct rtc_datetime * datetime);

void rtc_convertseconds_to_datetime(unsigned int seconds, struct rtc_datetime * datetime);

unsigned int rtc_getseconds(void);

void rtc_setdatetime(struct rtc_datetime * datetime);

void rtc_getdatetime(struct rtc_datetime * datetime);

#endif // !_BSP_RTC_H_
