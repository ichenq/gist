// +build !ignore

package types

import (
	"testing"
)

func TestLCGRand(t *testing.T) {
	var lcgArray []int
	for i := 0; i < 10; i++ {
		lcgArray = append(lcgArray, LCGRand())
	}
}

func TestRandInt(t *testing.T) {
	for i := 0; i < 1000; i++ {
		v := RandInt(0, 1000)
		if v < 0 {
			t.Fatalf("%v < 0", v)
		}
		if v > 1000 {
			t.Fatalf("v > 1000", v)
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
			t.Fatalf("v > 1.0", v)
		}
	}
}

func TestRangePerm(t *testing.T) {
	var appeared = make(map[int]bool, 1000)
	var list = RangePerm(0, 1000)
	for _, v := range list {
		if v < 0 {
			t.Fatalf("v < 0", v)
		}
		if v > 1000 {
			t.Fatalf("v > 1000", v)
		}
		if _, found := appeared[v]; found {
			t.Fatalf("duplicate %v", v)
		}
		appeared[v] = true
	}
}
