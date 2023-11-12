#include "bsp_rtc.h"
#include "stdio.h"


void rtc_init(void)
{
    SNVS->HPCOMR |= (1<<31);
    rtc_enable();
}

void rtc_enable(void)
{
    SNVS->LPCR |= 1<<0;
    while (!(SNVS->LPCR & 0x01));
}

void rtc_disable(void)
{
    SNVS->LPCR &= ~(1<<0);
    while (SNVS->LPCR & 0x01);
}

unsigned char rtc_isleapyear(unsigned short year)
{
    unsigned char value = 0;

    if (year % 400 == 0)
    {
        value = 1;
    }
    else if ((year % 4 == 0) && (year % 100 != 0))
    {
        value = 1;
    }
    return value;
}

unsigned int rtc_covertdate_to_seconds(struct rtc_datetime * datetime)
{
    unsigned short i = 0;
    unsigned int seconds = 0;
    unsigned int days = 0;
    unsigned short monthdays[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};

    for (i = 1970; i < datetime->year; ++i)
    {
        days += DAYS_IN_A_YEAR;
        if (rtc_isleapyear(i)) days += 1;
    }

    days += monthdays[datetime->month];
    if (rtc_isleapyear(i) && (datetime->month >= 3)) days += 1;

    seconds = days * SECONDS_IN_A_DAY + datetime->hour * SECONDS_IN_A_HOUR + datetime->minute * SECONDS_IN_A_MINUTE + datetime->second;

    return seconds;
}

void rtc_setdatetime(struct rtc_datetime * datetime)
{
    unsigned int seconds = 0;
    unsigned int temp = SNVS->LPCR;

    rtc_disable();  // when set RTC_MR and RTC_LR, we need to disable RTC.

    seconds = rtc_covertdate_to_seconds(datetime);

    SNVS->LPSRTCMR = (unsigned int)(seconds >> 17);
    SNVS->LPSRTCLR = (unsigned int)(seconds << 15);

    if (temp & 0x01) rtc_enable(); 
}

void rtc_convertseconds_to_datetime(unsigned int seconds, struct rtc_datetime * datetime)
{
    unsigned int x;
    unsigned int secondsRemaining, days;

    unsigned short daysInYear;

    unsigned char daysPerMonth[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

    secondsRemaining  = seconds;

    days = secondsRemaining / SECONDS_IN_A_DAY + 1;
    secondsRemaining %= SECONDS_IN_A_DAY;

    datetime->hour = secondsRemaining / SECONDS_IN_A_HOUR;
    secondsRemaining %= SECONDS_IN_A_HOUR;

    datetime->minute = secondsRemaining / SECONDS_IN_A_MINUTE;

    datetime->second = secondsRemaining % SECONDS_IN_A_MINUTE;

    daysInYear = DAYS_IN_A_YEAR;
    datetime->year = YEAR_RANGE_START;

    while (days > daysInYear)
    {
        days -= daysInYear;
        datetime->year++;

        if(!rtc_isleapyear(datetime->year))
        {
            daysInYear = DAYS_IN_A_YEAR;
        }
        else
        {
            daysInYear = DAYS_IN_A_YEAR + 1;
        }

    }

    if (rtc_isleapyear(datetime->year)) daysPerMonth[2] = 29U;

    for (x = 1; x <= 12; x++)
    {
        if (days <= daysPerMonth[x])
        {
            datetime->month = x;
            break;
        }
        else
        {
            days -= daysPerMonth[x];
        }
    }
    datetime->day = days;

}

unsigned int rtc_getseconds(void)
{
    unsigned seconds = 0;

    seconds = (SNVS->LPSRTCMR << 17) | (SNVS->LPSRTCLR >> 15);

    return seconds;
}

void rtc_getdatetime(struct rtc_datetime * datetime)
{
    unsigned int seconds = 0;

    seconds = rtc_getseconds();

    rtc_convertseconds_to_datetime(seconds, datetime);
}