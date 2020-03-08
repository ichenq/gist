// Copyright © 2017 ichenq@outlook.com All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

package mathutil

import (
	"math/bits"
	"math/rand"
	"testing"
)

func slowPopcount64(x uint64) int {
	var count = 0
	for x > 0 {
		count++
		x &= x - 1
	}
	return count
}

func TestMathPopcount(t *testing.T) {
	var i = 0
	for i < 0xFFFFFFFF {
		var a = slowPopcount64(uint64(i))
		var b = PopCount64(uint64(i))
		if a != b {
			t.Fatalf("popcount not equal %d != %d at %d", a, b, i)
		}
		i += rand.Int() % 1024
	}
}

func BenchmarkSlowPopcount(b *testing.B) {
	for i := 0; i < b.N; i++ {
		slowPopcount64(uint64(i))
	}
}

func BenchmarkPopcount(b *testing.B) {
	for i := 0; i < b.N; i++ {
		PopCount64(uint64(i))
	}
}

func BenchmarkBits(b *testing.B) {
	for i := 0; i < b.N; i++ {
		bits.OnesCount64(uint64(i))
	}
}
