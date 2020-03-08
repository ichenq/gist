// Copyright © 2017 ichenq@outlook.com All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

// +build !ignore

package strutil

import (
	"math/rand"
	"testing"
	"time"
)

func init() {
	rand.Seed(time.Now().UnixNano())
}

func TestBase62Encode(t *testing.T) {
	for i := 0; i < 10000; i++ {
		var id = rand.Int63()
		var shorten = EncodeToBase62String(id)
		var n = DecodeBase62String(shorten)
		if n != id {
			t.Fatalf("base62 not equal: %d != %d, %s", id, n, shorten)
		}
	}
}

func BenchmarkEncodeToBase62String(b *testing.B) {
	b.StopTimer()
	var id = rand.Int63()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		EncodeToBase62String(id)
	}
}

func BenchmarkDecodeBase62String(b *testing.B) {
	b.StopTimer()
	var id = rand.Int63()
	var shorten = EncodeToBase62String(id)
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		DecodeBase62String(shorten)
	}
}
