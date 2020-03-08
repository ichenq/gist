// Copyright © 2017 ichenq@outlook.com All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

// +build !ignore

package mathutil

import (
	"math"
	"testing"
)

func TestRandInt(t *testing.T) {
	for i := 0; i < 1000; i++ {
		v := RandInt(0, 1000)
		if v < 0 {
			t.Fatalf("%v < 0", v)
		}
		if v > 1000 {
			t.Fatalf("%v > 1000", v)
		}
	}
}

func TestRandFloat(t *testing.T) {
	for i := 0; i < 1000; i++ {
		v := RandFloat(0, 1.0)
		if v < 0 {
			t.Fatalf("%v < 0.0", v)
		}
		if v > 1.0 {
			t.Fatalf("%v > 1.0", v)
		}
	}
}

func TestRangePerm(t *testing.T) {
	var appeared = make(map[int]bool, 1000)
	var list = RangePerm(0, 1000)
	for _, v := range list {
		if v < 0 {
			t.Fatalf("%v < 0", v)
		}
		if v > 1000 {
			t.Fatalf("%v > 1000", v)
		}
		if _, found := appeared[v]; found {
			t.Fatalf("duplicate %v", v)
		}
		appeared[v] = true
	}
}

func TestMathRoundHalf(t *testing.T) {
	for i, pair := range roundTests {
		var a = pair[0]
		var b = pair[1]
		var c = RoundFloat(a)
		if math.IsNaN(c) {
			if !math.IsNaN(b) {
				t.Fatalf("%d: %f => %f != %f", i+1, a, c, b)
			}
		} else {
			if c != b {
				t.Fatalf("%d: %f => %f != %f", i+1, a, c, b)
			}
		}
	}
}

var roundTests = [][2]float64{
	{-0.49999999999999994, -0.0}, // -0.5+epsilon
	{-0.5, -1},
	{-0.5000000000000001, -1}, // -0.5-epsilon
	{0, 0},
	{0.49999999999999994, 0}, // 0.5-epsilon
	{0.5, 1},
	{-0.0, -0.0},
	{0.5000000000000001, 1},  // 0.5+epsilon
	{1.390671161567e-309, 0}, // denormal
	{2.2517998136852485e+15, 2.251799813685249e+15}, // 1 bit fraction
	{4.503599627370497e+15, 4.503599627370497e+15},  // large integer
	{math.Inf(-1), math.Inf(-1)},
	{math.Inf(1), math.Inf(1)},
	{math.NaN(), math.NaN()},
}
