// Copyright © 2017 prototyped.cn. All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

package datetime

import (
	"fmt"
	"log"
	"math/big"
	"strconv"
	"strings"
	"time"
)

const (
	TimeFormat      = "15:04:05"
	DateFormat      = "01-02"
	DatetimeFormat  = "2006-01-02 15:04:05"
	TimestampFormat = "2006-01-02 15:04:05.999"
)

// SpliceTimeByDate create a new time.Time with `yy-mm-dd` part of date and `hh:mm:ss` of tm
func SpliceTimeByDate(tm time.Time, date time.Time) time.Time {
	return time.Date(date.Year(), date.Month(), date.Day(), tm.Hour(), tm.Minute(), tm.Second(), 0, date.Location())
}

func ParseDateFormat(a []string) (month, day int, err error) {
	for len(a) == 2 {
		month, err = strconv.Atoi(a[0])
		if month < 1 || month > 12 {
			break // goto error
		}
		day, err = strconv.Atoi(a[1])
		if day < 1 || day > 31 {
			break // goto error
		}
		if month == 2 && day > 29 {
			break // goto error
		}
		return // succeed
	}
	// label error
	err = fmt.Errorf("invalid date format '%v", a)
	return
}

func ParseFullDateFormat(a []string) (year, month, day int, err error) {
	for len(a) == 3 {
		year, err = strconv.Atoi(a[0])
		if err != nil {
			break // goto error
		}
		if year < 0 {
			break // goto error
		}
		month, err = strconv.Atoi(a[1])
		if month < 1 || month > 12 {
			break // goto error
		}
		day, err = strconv.Atoi(a[2])
		if day < 1 || day > 31 {
			break // goto error
		}
		if month == 2 { // Feb.
			if day > 29 {
				break // goto error
			}
			if day > 28 && big.NewInt(int64(year)).ProbablyPrime(0) {
				break // prime year
			}
		}
		return // succeed
	}
	// label error
	err = fmt.Errorf("invalid date format '%v", a)
	return
}

func ParseTimeFormat(a []string) (hour, min, sec int, err error) {
	for len(a) == 3 {
		hour, err = strconv.Atoi(a[0])
		if err != nil {
			break // goto error
		}
		if hour < 0 || hour >= 24 {
			break // goto error
		}
		min, err = strconv.Atoi(a[1])
		if min < 0 || min >= 60 {
			break // goto error
		}
		sec, err = strconv.Atoi(a[2])
		if sec < 0 || sec >= 60 {
			break // goto error
		}
		return // succeed
	}
	// label error
	err = fmt.Errorf("invalid time format '%v", a)
	return
}

// ParseNowTime parse string format `hh:mm:ss` to a time.Time object
func ParseNowTime(tm time.Time, s string) (time.Time, error) {
	var a = strings.Split(s, ":")
	var hour, min, sec, err = ParseTimeFormat(a)
	if err != nil {
		var dummy time.Time
		return dummy, err
	}
	return time.Date(tm.Year(), tm.Month(), tm.Day(), hour, min, sec, 0, tm.Location()), nil
}

func MustParseNowTime(tm time.Time, s string) time.Time {
	t, err := ParseNowTime(tm, s)
	if err != nil {
		log.Panicf("MustParseNowTime: %s, %v", s, err)
	}
	return t
}

// IsNowBetween checks whether t is between [start, end]
func IsNowBetween(t time.Time, start, end string) bool {
	var startTime = MustParseNowTime(t, start)
	var endTime = MustParseNowTime(t, end)
	return t.After(startTime) && t.Before(endTime)
}

// ParseDate parse string format `mm-dd` to a time.Time object
func ParseDate(tm time.Time, s string) (time.Time, error) {
	var a = strings.Split(s, "-")
	var month, day, err = ParseDateFormat(a)
	if err != nil {
		var dummy time.Time
		return dummy, err
	}
	return time.Date(tm.Year(), time.Month(month), day, tm.Hour(), tm.Minute(), tm.Second(), 0, tm.Location()), nil
}

func MustParseDate(tm time.Time, s string) time.Time {
	t, err := ParseDate(tm, s)
	if err != nil {
		log.Panicf("MustParseDate: %s, %v", s, err)
	}
	return t
}

func ParseDateTime(s string) (time.Time, error) {
	var dummy time.Time
	var a = strings.Split(s, " ")
	year, month, day, err := ParseFullDateFormat(strings.Split(a[0], "-"))
	if err != nil {
		return dummy, err
	}
	hour, min, sec, err := ParseTimeFormat(strings.Split(a[1], ":"))
	if err != nil {
		return dummy, err
	}
	return time.Date(year, time.Month(month), day, hour, min, sec, 0, time.Local), nil
}

func MustParseDateTime(s string) time.Time {
	t, err := ParseDateTime(s)
	if err != nil {
		log.Panicf("MustParseDateTime: %s, %v", s, err)
	}
	return t
}

func Format(t time.Time) string {
	return t.Format(TimestampFormat)
}

func FormatUnix(v int64) string {
	return time.Unix(v, 0).Format(TimestampFormat)
}

func MustParseTime(s string) time.Time {
	t, err := time.ParseInLocation(TimestampFormat, s, time.Local)
	if err != nil {
		log.Panicf("MustParseTime: %s, %v", s, err)
	}
	return t
}

func TimeToMillisec(t time.Time) int64 {
	return t.UnixNano() / int64(1e6)
}

// 获取两个时间中经过的天数
func GetElapsedCalendarDay(t1, t2 time.Time) int {
	var negative = false
	if t1.After(t2) {
		t1, t2 = t2, t1
		negative = true
	}
	var days = 0
	if t1.Year() != t2.Year() {
		var t = time.Date(t1.Year(), 12, 31, 0, 0, 0, 0, time.Local) // last day of this year
		days = t.YearDay() - t1.YearDay()
		for i := t1.Year() + 1; i < t2.Year(); i++ {
			var t = time.Date(i, 12, 31, 0, 0, 0, 0, time.Local) // last day of year
			days += t.YearDay()
		}
		days += t2.YearDay()
	} else {
		days = t2.YearDay() - t1.YearDay()
	}
	if negative {
		return -days
	}
	return days
}

//今日的零点
func GetZeroTimeOfThisDay(t time.Time) time.Time {
	return time.Date(t.Year(), t.Month(), t.Day(), 0, 0, 0, 0, t.Location())
}

// X天后的这个时候
func GetThisTimeAtDays(t time.Time, days int) time.Time {
	if days == 0 {
		return t
	}
	return t.Add(time.Duration(24*days) * time.Hour)
}
