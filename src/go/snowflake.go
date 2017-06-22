// Package snowflake implements Sonyflake, a distributed unique ID generator inspired by Twitter's Snowflake.
//
// A Snowflake ID is composed of
//     39 bits for time in units of 10 msec
//      8 bits for a sequence number
//     16 bits for a machine id
package snowflake

import (
	"errors"
	"log"
	"sync"
	"time"
)

// These constants are the bit lengths of SnowFlake ID parts.
const (
	machineIDBits  = 16
	sequenceBits   = 8
	machineIDShift = sequenceBits
	timestampShift = sequenceBits + machineIDBits

	twepoch           = 1415302200000000000 // custom epoch in nanosecond, (2014-11-06 19:30:00 UTC)
	snowflakeTimeUnit = 1e7                 // nsec, i.e. 10 msec
)

var (
	ErrMachineIDOutOfRange = errors.New("machine ID out of range")
	ErrTimeBackwards       = errors.New("time has gone backwards")
	ErrIDBackwards         = errors.New("ID went backward")
)

// Snowflake with locking
type SnowFlake struct {
	sync.Mutex
	lastTime  uint64 // 39 bits
	lastID    uint64 //
	machineID uint16 // 16 bits, max 0xFFFF
	sequence  uint16 // 8 bits, max 0xFF
}

var std SnowFlake

func Default() *SnowFlake {
	return &std
}

func New(machineId uint16) *SnowFlake {
	var obj SnowFlake
	if err := obj.Init(machineId); err != nil {
		panic(err)
	}
	return &obj
}

func Next() uint64 {
	return std.Next()
}

func currentTime() uint64 {
	return uint64(time.Now().UTC().UnixNano()-twepoch) / snowflakeTimeUnit
}

func (sf *SnowFlake) Init(machineID uint16) error {
	if machineID > 0xFFFF {
		return ErrMachineIDOutOfRange
	}
	log.Printf("snowflake init with machine 0x%x\n", machineID)
	sf.machineID = machineID
	sf.lastTime = currentTime()
	return nil
}

func (sf *SnowFlake) Next() uint64 {
	var ts = currentTime()

	sf.Lock()
	defer sf.Unlock()

	if ts < sf.lastTime {
		panic(ErrTimeBackwards)
	}
	if ts == sf.lastTime {
		sf.sequence += 1
		if sf.sequence >= 0xFF { // sequence expired, tick to next millisecond
			ts = sf.tilNextTimeUnit(ts)
			sf.sequence = 0
		}
	} else {
		sf.sequence = 0
	}
	sf.lastTime = ts
	var id = (ts << timestampShift) | (uint64(sf.machineID) << machineIDShift) | uint64(sf.sequence)
	if id <= sf.lastID {
		panic(ErrIDBackwards)
	}
	sf.lastID = id
	return id
}

// tick to next
func (sf *SnowFlake) tilNextTimeUnit(ts uint64) uint64 {
	for {
		time.Sleep(time.Millisecond * 5)
		var now = currentTime()
		if now > ts {
			return now
		}
	}
}
