import datetime

output_header = r"D:\Final_code_bms\Smart_bms_v1.1\rtc_config.h"  # use raw string

def generate_rtc_header():
    now = datetime.datetime.now()

    rtc_second = now.second
    rtc_minute = now.minute
    rtc_hour   = now.hour % 12

    rtc_day    = now.day
    rtc_month  = now.month
    rtc_year   = now.year % 100       # YYYY → YY (2025 → 25)
    rtc_dayofweek = now.isoweekday()  # Monday = 1

    content = f"""#ifndef CONFIG_H
#define CONFIG_H

// Auto-generated RTC values
#define RTC_SECOND             {rtc_second}
#define RTC_MINUTE             {rtc_minute}
#define RTC_HOUR               {rtc_hour}

#define RTC_DAY                {rtc_day}
#define RTC_DAYOFWEEK          {rtc_dayofweek} // Monday=1
#define RTC_MONTH              {rtc_month}
#define RTC_YEAR               {rtc_year}

#endif // CONFIG_H
"""

    with open(output_header, "w") as f:
        f.write(content)

    print(f"Generated header: {output_header}")

if __name__ == "__main__":
    generate_rtc_header()
