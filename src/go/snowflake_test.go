package snowflake

import (
	"fmt"
	"strings"
	"testing"
	"time"
)

func TestSnowFlakeLimitation(t *testing.T) {
	var year = 2080
	for {
		var tm = time.Date(year, time.December, 1, 1, 0, 0, 0, time.Local)
		var now = (tm.UnixNano() - twepoch) / 1e7
		var a = fmt.Sprintf("%x", now)
		var b = fmt.Sprintf("%x", now<<timestampShift)
		if !strings.HasPrefix(b, a) {
			fmt.Printf("snowflake works well till year %d", year)
			break
		}
		year++
	}
}

func TestSnowFlakeDuplicate(t *testing.T) {
	var sf = New(1024)
	var exist = make(map[uint64]bool)
	for i := 0; i < 1e5; i++ {
		var id = sf.Next()
		if _, found := exist[id]; found {
			t.Fatalf("already exist %d", id)
		}
		exist[id] = true
	}
}

func TestSnowFlakeSpeed(t *testing.T) {
	var sf = New(0xFF)
	var count = 0
	var done = make(chan int)
	go func() {
		var ticker = time.NewTicker(time.Second)
		for {
			select {
			case <-ticker.C:
				done <- count
				return
			default:
				count++
				sf.Next()
			}
		}
	}()
	<-done
	fmt.Printf("Snow can gen %d uuid per second\n", count)
}

func BenchmarkGenerate(b *testing.B) {
	var sf = New(1024)
	for i := 0; i < b.N; i++ {
		sf.Next()
	}
}
