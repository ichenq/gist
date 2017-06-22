// +build !ignore

package types

import (
	"testing"
	"time"
)

func TestParseNowTime(t *testing.T) {
	var now = time.Now()
	var str = "01:59:59"
	tm, err := ParseNowTime(now, str)
	if err != nil {
		t.Fatalf("ParseNowTime: %v, %v", str, err)
	}

	if tm.Year() != now.Year() {
		t.Fatalf("year %d != %d", tm.Year(), now.Year())
	}
	if tm.Month() != now.Month() {
		t.Fatalf("month %d != %d", tm.Month(), now.Month())
	}
	if tm.Day() != now.Day() {
		t.Fatalf("day %d != %d", tm.Day(), now.Day())
	}
	if tm.Hour() != 1 {
		t.Fatalf("hour %d != %d", tm.Hour(), 1)
	}
	if tm.Minute() != 59 {
		t.Fatalf("minute %d != %d", tm.Minute(), 59)
	}
	if tm.Second() != 59 {
		t.Fatalf("second %d != %d", tm.Second(), 59)
	}
}
