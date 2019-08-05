#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

extern volatile unsigned char HaveRTC;

struct rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

void Rtc_Init(void);
void rtc_get (struct rtc_time *);
void rtc_set (struct rtc_time *);
void rtc_reset (void);


#ifdef __cplusplus
}
#endif

#endif /*__RTC_H__*/
