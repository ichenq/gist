package algorithm

// the core algorithm here was borrowed from:
// Twitter's `snowflake` https://github.com/twitter/snowflake

import (
	"errors"
	"fmt"
	"time"
)

const (
	workerIDBits   = uint64(10)
	sequenceBits   = uint64(12)
	workerIDShift  = sequenceBits
	timestampShift = sequenceBits + workerIDBits

	// custom epoch, (2014-11-06 00:00:00 UTC) in millisecond
	twepoch uint64 = 1415232000000
)

var (
	ErrWorkerOutOfRange = errors.New("worker ID out of range")
	ErrTimeBackwards    = errors.New("time has gone backwards")
	ErrIDBackwards      = errors.New("ID went backward")
)

// Snowflake without locking
type SnowFlake struct {
	lastTimestamp uint64 // 48 bit
	worker        uint64 // 10 bit, 0x3FF
	sequence      uint64 // 12 bit, 0xFFF
	lastId        uint64
}

var defaultObject SnowFlake

func DefaultSnowFlake() *SnowFlake {
	return &defaultObject
}

func NewSnowFlake(workerId uint16) *SnowFlake {
	var obj SnowFlake
	if err := obj.Init(workerId); err != nil {
		panic(err)
	}
	return &obj
}

func (sf *SnowFlake) Init(workerId uint16) error {
	fmt.Println("snowflake worker ID:", workerId)
	if workerId >= (1 << workerIDBits) {
		return ErrWorkerOutOfRange
	}
	sf.worker = uint64(workerId)
	sf.lastTimestamp = uint64(time.Now().UTC().UnixNano() / 1000000)
	return nil
}

func (sf *SnowFlake) Next() uint64 {
	ts := uint64(time.Now().UTC().UnixNano() / 1000000)
	if ts < sf.lastTimestamp {
		panic(ErrTimeBackwards)
	}
	if ts == sf.lastTimestamp {
		sf.sequence += 1
		if sf.sequence == (1 << sequenceBits) { // sequence expired, tick to next millisecond
			sf.sequence = 0
			ts = sf.tilNextMillisec(ts)
		}
	} else {
		sf.sequence = 0
	}
	sf.lastTimestamp = ts
	id := ((ts - twepoch) << timestampShift) | (sf.worker << workerIDShift) | sf.sequence
	if id <= sf.lastId {
		panic(ErrIDBackwards)
	}
	sf.lastId = id
	return uint64(id)
}

//Spin for 1 millisecond
func (sf *SnowFlake) tilNextMillisec(ts uint64) uint64 {
	for {
		now := uint64(time.Now().UTC().UnixNano() / 1000000)
		if now > ts {
			return now
		}
	}
}
