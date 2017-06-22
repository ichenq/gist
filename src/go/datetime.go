package types

import (
	"log"
	"time"
)

const timeFormat = "15:04:05"
const dateFormat = "01-02"

// SpliceTimeByDate create a new time.Time with `yy-mm-dd` part of date and `hh:mm:ss` of tm
func SpliceTimeByDate(tm time.Time, date time.Time) time.Time {
	return time.Date(date.Year(), date.Month(), date.Day(), tm.Hour(), tm.Minute(), tm.Second(), 0, date.Location())
}

// ParseNowTime parse string format `hh:mm:ss` to a time.Time object
func ParseNowTime(tm time.Time, s string) (time.Time, error) {
	t, err := time.Parse(timeFormat, s)
	if err != nil {
		return t, err
	}
	return SpliceTimeByDate(t, tm), nil
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
	t, err := time.Parse(dateFormat, s)
	if err != nil {
		return t, err
	}
	var t1 = time.Date(tm.Year(), t.Month(), t.Day(), tm.Hour(), tm.Minute(), tm.Second(), 0, tm.Location())
	return t1, nil
}

func MustParseDate(tm time.Time, s string) time.Time {
	t, err := ParseDate(tm, s)
	if err != nil {
		log.Panicf("MustParseDate: %s, %v", s, err)
	}
	return t
}
