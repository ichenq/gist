// Copyright © 2015-present prototyped.cn. All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.
//
package uuid

import (
	"fmt"
	"strconv"
	"strings"
	"sync"
	"testing"
	"time"
)

func TestSnowFlakeLimitation(t *testing.T) {
	var sf = NewSnowFlake(0, 0, 1234, false)
	var uuid = sf.Next()
	t.Logf("a uuid is %v", uuid)

	var maxLimitYearBinary = strings.Repeat("1", 64-int(sf.timestampShift))
	maxTimestamp, err := strconv.ParseInt(maxLimitYearBinary, 2, 64)
	if err != nil {
		t.Errorf("ParseInt: %v", err)
	}
	var epoch = time.Date(2000, 1, 1, 0, 0, 0, 0, time.UTC)
	var total = float64(maxTimestamp/100-epoch.Unix()) / (86400 * 365)
	t.Logf("default snowfake will exhoused after %v years", total)
}

func TestSnowFlakeRelative(t *testing.T) {
	var sf = NewSnowFlake(0, 0, 1234, true)
	for i := 0; i < 10; i++ {
		var uuid = sf.Next()
		t.Logf("%d uuid is %v", i, uuid)
	}
}

func TestSnowFlakeConcurrency(t *testing.T) {
	var sf = NewSnowFlake(0, 0, 1234, false)
	var store = make(map[int64]bool)
	var bus = make(chan int64, 4000000)
	var wg sync.WaitGroup

	var worker = func(i int) {
		defer wg.Done()
		var ticker = time.NewTicker(time.Second)
		for {
			select {
			case <-ticker.C:
				return
			default:
				var uuid = sf.Next()
				//println("worker", i, uuid)
				bus <- uuid
			}
		}
	}
	var concurrency = 4
	for i := 1; i <= concurrency; i++ {
		wg.Add(1)
		go worker(i)
	}

	//
	go func() {
		for uuid := range bus {
			if _, found := store[uuid]; found {
				t.Fatalf("uuid already exist %d", uuid)
			}
			store[uuid] = true
		}
	}()

	wg.Wait()
	close(bus)

	fmt.Printf("Snowflake can generate %d uuid per second with %d concurrency\n", len(store), concurrency)
}

func BenchmarkGenerate(b *testing.B) {
	var sf = NewSnowFlake(0, 0, 1234, false)
	for i := 0; i < b.N; i++ {
		sf.Next()
	}
}
