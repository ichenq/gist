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
	"sync"
	"time"
)

var (
	lazySeed uint32     = 0 //
	mtx      sync.Mutex     //thread-safety
)

func init() {
	lazySeed = uint32(time.Now().Unix())
}

// 线性同余法的随机数生成器
// see https://en.wikipedia.org/wiki/Linear_congruential_generator
func LCGRand() int {
	mtx.Lock()
	lazySeed = lazySeed*214013 + 2531011
	mtx.Unlock()
	return int(lazySeed>>16) & 0x7fff
}

func SetLCGSeed(v int) {
	mtx.Lock()
	lazySeed = uint32(v)
	mtx.Unlock()
}
