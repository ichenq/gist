// Copyright © 2017 prototyped.cn. All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

// +build !ignore

package datetime

import (
	"strings"
	"testing"
	"time"
)

const timeFormat = "2006-01-02 15:04:05"

func TestParseTimeFormat(t *testing.T) {
	type testCase struct {
		shouldPass bool
		text       string
		hour       int
		minute     int
		second     int
	}

	var cases = []testCase{
		{true, "12:34:56", 12, 34, 56},
		{true, "00:00:00", 0, 0, 0},
		{true, "23:59:59", 23, 59, 59},
		{false, "-12:-34:-56", 0, 0, 0},
		{false, "24:60:60", 0, 0, 0},
	}
	for _, tcase := range cases {
		hour, min, sec, err := ParseTimeFormat(strings.Split(tcase.text, ":"))
		if tcase.shouldPass {
			if err != nil {
				t.Fatalf("ParseTimeFormat: failed %v, %v", tcase.text, err)
			}
		} else {
			if err == nil {
				t.Fatalf("ParseTimeFormat: should not pass, %v, %v", tcase.text, err)
			} else {
				continue
			}
		}
		if hour != tcase.hour {
			t.Fatalf("ParseTimeFormat: hour %v, %v", tcase.text, err)
		}
		if min != tcase.minute {
			t.Fatalf("ParseTimeFormat: min %v, %v", tcase.text, err)
		}
		if sec != tcase.second {
			t.Fatalf("ParseTimeFormat: sec %v, %v", tcase.text, err)
		}
	}
}

func TestParseDateFormat(t *testing.T) {
	type testCase struct {
		shouldPass bool
		text       string
		year       int
		month      int
		day        int
	}

	var cases = []testCase{
		{true, "0001-01-01", 1, 1, 1},
		{true, "1234-01-01", 1234, 1, 1},
		{true, "9999-12-31", 9999, 12, 31},
		{false, "-12:-34:-56", 0, 0, 0},
		{false, "1234:0:1", 0, 0, 0},
		{false, "1234:-1:0", 0, 0, 0},
		{false, "1234:13:32", 0, 0, 0},
	}
	for _, tcase := range cases {
		year, mon, day, err := ParseFullDateFormat(strings.Split(tcase.text, "-"))
		if tcase.shouldPass {
			if err != nil {
				t.Fatalf("ParseDateFormat: failed %v, %v", tcase.text, err)
			}
		} else {
			if err == nil {
				t.Fatalf("ParseDateFormat: should not pass, %v, %v", tcase.text, err)
			} else {
				continue
			}
		}
		//println(tcase.text, year, mon, day)
		if year != tcase.year {
			t.Fatalf("ParseDateFormat: year %v, %v", tcase.text, err)
		}
		if mon != tcase.month {
			t.Fatalf("ParseDateFormat: month %v, %v", tcase.text, err)
		}
		if day != tcase.day {
			t.Fatalf("ParseDateFormat: day %v, %v", tcase.text, err)
		}
	}
}

type testGedSuit1 struct {
	s1  string
	s2  string
	day int
}

func TestGetElapsedCalendarDay(t *testing.T) {
	var gedCases = []testGedSuit1{
		{"2018-01-01 00:00:00", "2018-01-01 23:59:59", 0},
		{"2018-01-01 00:00:00", "2018-02-01 00:00:00", 31},
		{"2016-02-28 00:00:00", "2016-03-01 00:00:00", 2},
		{"2018-01-01 00:00:00", "2017-12-30 00:00:00", -2},
	}
	for _, item := range gedCases {
		var t1, _ = time.Parse(timeFormat, item.s1)
		var t2, _ = time.Parse(timeFormat, item.s2)
		var d = GetElapsedCalendarDay(t1, t2)
		if d != item.day {
			t.Fatalf("%s, %s, %d != %d", item.s1, item.s2, d, item.day)
		}
	}
}

func TestGetZeroTimeOfThisDay(t *testing.T) {
	var cases = []testGedSuit1{
		{"2018-01-01 00:00:00", "2018-01-01 00:00:00", 0},
		{"2018-02-01 12:34:56", "2018-02-01 00:00:00", 0},
	}
	for _, item := range cases {
		var t1, _ = time.Parse(timeFormat, item.s1)
		var t2, _ = time.Parse(timeFormat, item.s2)
		var tm = GetZeroTimeOfThisDay(t1)
		if tm != t2 {
			t.Fatalf("%s, %s, %v != %v", item.s1, item.s2, tm, t2)
		}
	}
}

func TestGetThisTimeDays(t *testing.T) {
	var cases = []testGedSuit1{
		{"2018-01-01 00:00:00", "2018-01-01 00:00:00", 0},
		{"2018-02-28 12:34:56", "2018-03-01 12:34:56", 1},
		{"2017-02-28 12:34:56", "2018-02-28 12:34:56", 365},
		{"2000-02-28 12:34:56", "2000-03-01 12:34:56", 2},
		{"2016-03-01 12:34:56", "2016-02-28 12:34:56", -2},
	}
	for _, item := range cases {
		var t1, _ = time.Parse(timeFormat, item.s1)
		var t2, _ = time.Parse(timeFormat, item.s2)
		var tm = GetThisTimeAtDays(t1, item.day)
		if tm != t2 {
			t.Fatalf("%s, %s, %v != %v", item.s1, item.s2, tm, t2)
		}
	}
}
