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
	"testing"
)

func checkEqual(t *testing.T, a, b int) {
	if a != b {
		t.Fatalf("%d != %d", a, b)
	}
}

func checkNotEqual(t *testing.T, a, b int) {
	if a == b {
		t.Fatalf("%d == %d", a, b)
	}
}

func TestMathAbs(t *testing.T) {
	checkEqual(t, 11, Abs(-11))
	checkEqual(t, 0, Abs(0))
	checkEqual(t, 11, Abs(11))
	checkNotEqual(t, 1, Abs(2))
}

func TestMathMax(t *testing.T) {
	checkEqual(t, 5, Max(3, 5))
	checkEqual(t, 5, Max(5, 3))
	checkEqual(t, 5, Max(5, 5))
	checkEqual(t, -3, Max(-5, -3))
	checkEqual(t, -3, Max(-3, -5))
	checkEqual(t, -1, Max(-1, -1))
}

func TestMathMin(t *testing.T) {
	checkEqual(t, 5, Min(7, 5))
	checkEqual(t, 5, Min(5, 7))
	checkEqual(t, 5, Min(5, 5))
	checkEqual(t, 0, Min(0, 0))
	checkEqual(t, -2, Min(-2, -1))
	checkEqual(t, -1, Min(-1, -1))
}

func TestMathDim(t *testing.T) {
	checkEqual(t, 2, Dim(7, 5))
	checkEqual(t, 0, Dim(5, 7))
	checkEqual(t, 0, Dim(0, 0))
	checkEqual(t, 0, Dim(5, 5))
	checkEqual(t, 0, Dim(-5, -3))
	checkEqual(t, 2, Dim(-3, -5))
}

func TestMathGCD(t *testing.T) {
	checkEqual(t, 1, GCD(3, 5))
	checkEqual(t, 1, GCD(17, 13))
	checkEqual(t, 5, GCD(25, 35))
}
