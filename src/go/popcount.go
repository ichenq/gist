// Copyright © 2017 ichenq@outlook.com All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

package mathutil

var wordbits [1 << 8]byte

func init() {
	gentable()
}

func gentable() {
	for i := range wordbits {
		var n byte
		for x := i; x != 0; x >>= 1 {
			if x&1 != 0 {
				n++
			}
		}
		wordbits[i] = n
	}
}

// PopCount returns the number of 1-bits in x.
// See https://en.wikipedia.org/wiki/Hamming_weight
func PopCount64(x uint64) int {
	return int(wordbits[byte(x>>(0*8))] +
		wordbits[byte(x>>(1*8))] +
		wordbits[byte(x>>(2*8))] +
		wordbits[byte(x>>(3*8))] +
		wordbits[byte(x>>(4*8))] +
		wordbits[byte(x>>(5*8))] +
		wordbits[byte(x>>(6*8))] +
		wordbits[byte(x>>(7*8))])
}
