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

func TestLazyLCGRand(t *testing.T) {
	for i := 0; i < 1000; i++ {
		LCGRand()
	}
}

func TestSetLazyLCGSeed(t *testing.T) {
	SetLCGSeed(1234567890)
	for i := 0; i < 1000; i++ {
		LCGRand()
	}
}
