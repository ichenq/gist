// Copyright © 2015-present prototyped.cn. All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.
//
// Package snowflake implements Sonyflake, a distributed unique ID generator inspired by Twitter's Snowflake.
//
// A Snowflake ID is composed of
//     39 bits for timestamp in centiseconds
//     12 bits for a machine id
//     10 bits for a sequence number
package uuid

import (
	"log"
	"sync"
	"time"
)

// These constants are the bit lengths of SnowFlake ID parts.
const (
	DefaultSequenceBits  = 10
	DefaultMachineIDBits = 12
	MachineSeqBitsLimit  = 24

	Twepoch = 1569888000000000000 // custom epoch in nanosecond, (2019-10-01 00:00:00 UTC)
)

func currentTimeUnit() int64 {
	return int64(time.Now().UTC().UnixNano()-Twepoch) / 1e7 // nanoseconds to centiseconds
}

// Snowflake with locking
type SnowFlake struct {
	sync.Mutex
	sequence       int64  // last sequence
	lastTimestamp  int64  // last timestamp
	lastID         int64  // last generated id
	epoch          int64  //
	genCount       uint64 // uuid generated count
	sequenceShift  uint16 // bits of sequence no. take
	timestampShift uint16 // bits to shift timestamp
	machineID      uint16 // id of this machine(process)
}

// sequence + machineID should less than 23 bits
func NewSnowFlake(sequenceBits, machineIDBits, machineId uint16, relative bool) *SnowFlake {
	if sequenceBits == 0 {
		sequenceBits = DefaultSequenceBits
	}
	if machineIDBits == 0 {
		machineIDBits = DefaultMachineIDBits
	}
	if int(sequenceBits+machineIDBits) > MachineSeqBitsLimit {
		log.Panicf("snowflake: <%d %d> bits too long", machineIDBits, sequenceBits)
	}

	var sf = &SnowFlake{
		sequenceShift:  sequenceBits,
		timestampShift: sequenceBits + machineIDBits,
		machineID:      machineId % ((1 << machineIDBits) - 1),
	}
	if relative {
		sf.epoch = currentTimeUnit()
	}
	sf.lastTimestamp = sf.getTimeUnit()
	return sf
}

func (sf *SnowFlake) getTimeUnit() int64 {
	return currentTimeUnit() - sf.epoch
}

func (sf *SnowFlake) Next() int64 {
	sf.Lock()
	var ts = sf.getTimeUnit()
	if ts < sf.lastTimestamp {
		sf.Unlock()
		log.Panicf("SnowFlake: time has gone backwards, %v -> %v, %v", ts, sf.lastTimestamp, sf.genCount)
	}
	if ts == sf.lastTimestamp {
		sf.sequence++
		if sf.sequence >= (1 << sf.sequenceShift) { // sequence expired, tick to next time unit
			sf.sequence = 0
			ts = sf.tilNextTimeUnit(ts)
		}
	} else {
		sf.sequence = 0
	}
	sf.lastTimestamp = ts

	var id = (uint64(ts) << uint64(sf.timestampShift)) | (uint64(sf.machineID) << uint64(sf.sequenceShift)) | uint64(sf.sequence)
	if int64(id) <= sf.lastID {
		sf.Unlock()
		log.Panicf("SnowFlake: ID has gone backwards, %x -> %x, %x, %v", id, sf.lastID, ts, sf.genCount)
	}

	sf.lastID = int64(id)
	sf.genCount++
	sf.Unlock()

	return int64(id)
}

// tick to next
func (sf *SnowFlake) tilNextTimeUnit(ts int64) int64 {
	for {
		time.Sleep(time.Millisecond * 5)
		var now = sf.getTimeUnit()
		if now > ts {
			return now
		}
	}
}
