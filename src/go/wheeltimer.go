package types

import (
	"time"
)

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

type Runner interface {
	Run() error
}

type TimerNode struct {
	next   *TimerNode
	expire int64
	runner Runner
}

type timerList struct {
	head TimerNode
	tail *TimerNode
}

func (l *timerList) reset() {
	l.tail = &l.head
}

// Hierarchical Wheel Timer
type WheelTimer struct {
	jiffies int64
	near    [TVR_BITS]timerList
	bucket  [4][TVN_BITS]timerList
	bus     chan Runner
}

func NewWheelTimer() *WheelTimer {
	t := &WheelTimer{
		bus: make(chan Runner, 64),
	}
	for i := 0; i < TVR_SIZE; i++ {
		t.near[i].reset()
	}
	for i := 0; i < 4; i++ {
		for j := 0; i < TVN_SIZE; i++ {
			t.bucket[i][j].reset()
		}
	}
	return t
}

func getCurrentTime() int64 {
	return (time.Now().UnixNano() - twepoch) / TimeUnit
}

func (t *WheelTimer) addNewNode(node *TimerNode) {
	var expires = node.expire
	var idx = uint64(expires - t.jiffies)
	var list *timerList
	if idx < TVR_SIZE {
		i := expires & TVR_MASK
		list = &t.near[i]
	} else if idx < (1 << (TVR_BITS + TVN_BITS)) {
		i := (expires >> (TVR_BITS + TVN_BITS)) & TVN_MASK
		list = &t.bucket[0][i]
	} else if idx < (1 << (TVR_BITS + 2*TVN_BITS)) {
		i := (expires >> (TVR_BITS + TVN_BITS)) & TVN_MASK
		list = &t.bucket[1][i]
	} else if idx < (1 << (TVR_BITS + 3*TVN_BITS)) {
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

func (t *WheelTimer) cascade(i int) int {
	var idx = (t.jiffies >> uint64(TVR_BITS+i*TVN_BITS)) & TVN_MASK
	var list = &t.bucket[i][idx]
	var node = list.head.next
	for node != nil {
		t.addNewNode(node)
		node = node.next
	}
	list.reset()
	return int(idx)
}

func (t *WheelTimer) Tick() {
	var idx = t.jiffies & TVR_MASK
	if idx == 0 {
		for i := 0; i < 4; i++ {
			if t.cascade(i) == 0 {
				break
			}
		}
	}

	t.jiffies++

	// queue expired action
	var list = &t.near[idx]
	var node = list.head.next
	for node != nil {
		t.bus <- node.runner
		node = node.next
	}
	// empty list
	list.head.next = nil
	list.tail = &list.head
}

func (t *WheelTimer) Remove(node *TimerNode) {

}
