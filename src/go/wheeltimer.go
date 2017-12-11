// Copyright © 2015-2017 ichenq@outlook.com. All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

package context

import (
	"time"
)

// a hierarchical wheel timer implementation
// inspired by linux kernel v2.6, see links below
//   1) https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git/tree/kernel/timer.c?h=v2.6.39
//   2) https://github.com/cloudwu/skynet/blob/master/skynet-src/skynet_timer.c
//
// complexity compared with min-heap
//------------------------------------------------
//             | StartTimer  StopTimer  PerTick  |
// min-heap    |  O(lnN)      O(1)        O(1)   |
// timer-wheel |  O(1)        O(1)        O(1)   |
//------------------------------------------------

const (
	TVN_BITS = 6                   // time vector level shift bits
	TVR_BITS = 8                   // timer vector shift bits
	TVN_SIZE = (1 << TVN_BITS)     // wheel slots of level vector
	TVR_SIZE = (1 << TVR_BITS)     // wheel slots of vector
	TVN_MASK = (TVN_SIZE - 1)      //
	TVR_MASK = (TVR_SIZE - 1)      //
	twepoch  = 1415302200000000000 // custom epoch in nanosecond
	TimeUnit = 1e7                 // nsec, i.e. 10 msec
)

type timerNode struct {
	next   *timerNode
	Expire int64  // Absolute expiry time
	delay  int64  // Expiry time relative to now
	id     int    // Unique timer ID
	repeat int    // Timer repeat count
	R      Runner // Runner object
}

type timerList struct {
	head timerNode
	tail *timerNode
}

func (l *timerList) reset() {
	l.tail = &l.head
}

// Hierarchical Wheel Timer
type WheelTimer struct {
	jiffies int64
	current int64
	near    [TVR_SIZE]timerList
	bucket  [4][TVN_SIZE]timerList
	bus     chan Runner
}

func NewWheelTimer() *WheelTimer {
	t := &WheelTimer{}
	t.Init()
	return t
}

// we make time resolution to 10ms every tick
func gettime() int64 {
	return (time.Now().UnixNano() - twepoch) / TimeUnit
}

func (t *WheelTimer) Init() {
	var now = gettime()
	t.jiffies = now
	t.current = now
	t.bus = make(chan Runner, 64)
	for i := 0; i < TVR_SIZE; i++ {
		t.near[i].reset()
	}
	for i := 0; i < 4; i++ {
		for j := 0; i < TVN_SIZE; i++ {
			t.bucket[i][j].reset()
		}
	}
}

func (t *WheelTimer) addNode(node *timerNode) {
	var expires = node.Expire
	var idx = uint64(expires - t.jiffies)
	var list *timerList
	if idx < TVR_SIZE { // [0, 0x100)
		i := expires & TVR_MASK
		list = &t.near[i]
	} else if idx < (1 << (TVR_BITS + TVN_BITS)) { // [0x100, 0x4000)
		i := (expires >> TVR_BITS) & TVN_MASK
		list = &t.bucket[0][i]
	} else if idx < (1 << (TVR_BITS + 2*TVN_BITS)) { // [0x4000, 0x100000)
		i := (expires >> (TVR_BITS + TVN_BITS)) & TVN_MASK
		list = &t.bucket[1][i]
	} else if idx < (1 << (TVR_BITS + 3*TVN_BITS)) { // [0x100000, 0x4000000)
		i := (expires >> (TVR_BITS + 2*TVN_BITS)) & TVN_MASK
		list = &t.bucket[2][i]
	} else if int64(idx) < 0 {
		// Can happen if you add a timer with expires == jiffies,
		// or you set a timer to go off in the past
		i := t.jiffies & TVR_MASK
		list = &t.near[i]
	} else {
		// If the timeout is larger than 0xffffffff on 64-bit
		// architectures then we use the maximum timeout
		if idx > 0xFFFFFFFF {
			idx = 0xFFFFFFFF
			expires = int64(idx) + t.jiffies
		}
		i := (expires >> (TVR_BITS + 3*TVN_BITS)) & TVN_MASK
		list = &t.bucket[3][i]
	}
	// add to linked list
	list.tail.next = node
	list.tail = node
}

// cascade all the timers from tv up one level
func (t *WheelTimer) cascade(i, idx int) {
	var list = &t.bucket[i][idx]
	var node = list.head.next
	for node != nil {
		t.addNode(node)
		node = node.next
	}
	list.reset()
}

func (t *WheelTimer) Tick() {
	var index = t.jiffies & TVR_MASK
	if index == 0 {
		// cascade timers
		for i := 0; i < 4; i++ {
			var idx = (t.jiffies >> uint64(TVR_BITS+i*TVN_BITS)) & TVN_MASK
			t.cascade(i, int(idx))
			if idx != 0 {
				break
			}
		}
	}

	t.jiffies++

	// queue expired action
	var list = &t.near[index]
	var node = list.head.next
	for node != nil {
		t.bus <- node.R
		node = node.next
	}
	// empty list
	list.reset()
}

func (t *WheelTimer) Update() {
	var current = gettime()
	if current < t.current {
		log.Fatalf("WheelTimer: time go backward, %d -> %d", current, t.jiffies)
	} else if current >= t.current {
		var diff = int(current - t.current)
		t.current = current
		for i := 0; i < diff; i++ {
			t.Tick()
		}
	}
}
