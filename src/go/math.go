// Copyright © 2017 ichenq@outlook.com All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

package mathutil

//Absolute value of an integer.
func Abs(x int) int {
	if x < 0 {
		return -x
	}
	if x == 0 {
		return 0
	}
	return x
}

// Max returns the larger of x or y.
func Max(x, y int) int {
	if x < y {
		return y
	}
	return x
}

// Min returns the smaller of x or y.
func Min(x, y int) int {
	if y < x {
		return y
	}
	return x
}

// Dim returns the maximum of x-y or 0.
func Dim(x, y int) int {
	return Max(x-y, 0)
}

// Greatest Common Divisor
func GCD(a, b int) int {
	if a == 0 {
		return b
	}
	return GCD(b%a, a)
}
