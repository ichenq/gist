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
	"testing"
)

func TestIsAlmostEqualFloat32(t *testing.T) {
	var a float32 = 0.15 + 0.15
	var b float32 = 0.1 + 0.2
	if a != b {
		t.Fatalf("%v != %v", a, b)
	}
	if !IsAlmostEqualFloat32(b, a) {
		t.Fatalf("%v is not almost equal to %v", b, a)
	}
}

func TestIsAlmostEqualFloat64(t *testing.T) {
	var a float64 = 1.0
	var b = float64(RoundHalf(0.5001))
	if a != b {
		t.Fatalf("%v != %v", a, b)
	}
	if !IsAlmostEqualFloat64(b, a) {
		t.Fatalf("%v is not almost equal to %v", b, a)
	}
}
