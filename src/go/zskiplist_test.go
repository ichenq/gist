// Copyright © 2016-present ichenq@outlook.com  All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.
//

// +build !ignore

package algo

import (
	"fmt"
	"log"
	"math/rand"
	"os"
	"sort"
	"testing"
	"time"
)

var _ = os.Open

var testRandSeed = time.Now().UnixNano()

func init() {
	rand.Seed(testRandSeed)
}

type testPlayer struct {
	Uid      uint64
	Populace uint64
	Level    uint16
}

func (p *testPlayer) Uuid() uint64 {
	return p.Uid
}

func makeTestPlayers(count, maxScore int, dupScore bool) map[uint64]*testPlayer {
	var set = make(map[uint64]*testPlayer, count)
	var nextID uint64 = 100000000
	for i := 0; i < count; i++ {
		nextID++
		obj := &testPlayer{
			Uid:   nextID,
			Level: uint16(rand.Int() % 60),
		}
		if dupScore {
			obj.Populace = uint64(rand.Int()%maxScore) + 1
		} else {
			obj.Populace = uint64(maxScore)
			maxScore--
		}
		set[obj.Uid] = obj
	}
	return set
}

func checkDupObject(zsl *ZSkipList, t *testing.T) {
	if zsl.Len() == 0 {
		return
	}
	var set = make(map[uint64]bool, zsl.Len())
	var rank = zsl.Len()
	var node = zsl.HeaderNode().Next()
	for node != nil {
		rank--
		var player = node.Obj.(*testPlayer)
		if _, found := set[player.Uid]; found {
			t.Fatalf("Duplicate rank object found: %d, %d", rank, player.Uid)
		}
		set[player.Uid] = true
		node = node.Next()
	}
}

func dumpToFile(zsl *ZSkipList, filename string) {
	f, err := os.OpenFile(filename, os.O_CREATE|os.O_WRONLY|os.O_TRUNC, os.ModePerm)
	if err != nil {
		log.Fatalf("OpenFile: %v", err)
	}
	zsl.Dump(f)
}

func dumpSliceToFile(players []*testPlayer, filename string) {
	f, err := os.OpenFile(filename, os.O_CREATE|os.O_WRONLY|os.O_TRUNC, os.ModePerm)
	if err != nil {
		log.Fatalf("OpenFile: %v", err)
	}

	fmt.Fprint(f, "    uid    rank    score    level\n")
	var count = 0
	for i := 0; i < len(players); i++ {
		var item = players[i]
		count++
		fmt.Fprintf(f, "%8d, %5d %5d, %4d\n", item.Uid, count, item.Populace, item.Level)
	}
	f.Close()
}

func mapToSlice(set map[uint64]*testPlayer) []*testPlayer {
	var slice = make([]*testPlayer, 0, len(set))
	for _, v := range set {
		slice = append(slice, v)
	}
	return slice
}

type tester interface {
	Fatalf(format string, args ...interface{})
}

// Update score(zskiplist insert and delete) in many times
func manyUpdate(t tester, zsl *ZSkipList, set map[uint64]*testPlayer, count int) {
	for _, v := range set {
		var oldScore = v.Populace
		if node := zsl.Delete(oldScore, v); node == nil {
			dumpToFile(zsl, "zskiplist.dat")
			t.Fatalf("manyUpdate: delete old item[%d-%d] fail", v.Uid, v.Populace)
			break
		}
		v.Populace += uint64(rand.Uint32()%100) + 1
		if node := zsl.Insert(v.Populace, v); node == nil {
			t.Fatalf("manyUpdate: insert new item[%d-%d] fail, old score: %d", v.Uid, v.Populace, oldScore)
			break
		}
		count--
		if count == 0 {
			break
		}
	}
}

func TestZSkipListInsertRemove(t *testing.T) {
	const units = 100000
	var set = makeTestPlayers(units, 1000, true)
	var zsl = NewZSkipList()
	var maxTurn = 100
	for i := 0; i < maxTurn; i++ {
		// First insert all player to zskiplist
		for _, v := range set {
			if node := zsl.Insert(v.Populace, v); node == nil {
				t.Fatalf("insert item[%d-%d] failed", v.Populace, v.Uid)
			}
		}
		if zsl.Len() != units {
			t.Fatalf("unexpected skiplist element count, %d != %d", zsl.Len(), units)
		}

		checkDupObject(zsl, t)

		// Second remove all players in zskiplist
		for _, v := range set {
			var node = zsl.Delete(v.Populace, v)
			if node == nil {
				t.Fatalf("delete item[%d-%d] failed", v.Populace, v.Uid)
			}
			if brief := node.Obj.(*testPlayer); brief.Uid != v.Uid {
				t.Fatalf("delete item, %d not equal to %d", brief.Uid, v.Uid)
			}
		}

		if zsl.Len() != 0 {
			t.Fatalf("skiplist not empty")
		}
	}
}

func TestZSkipListChangedInsert(t *testing.T) {
	const units = 100000
	var set = makeTestPlayers(units, 1000, true)
	var zsl = NewZSkipList()

	// Insert all player to zskiplist
	for _, v := range set {
		var node = zsl.Insert((v.Populace), v)
		if node == nil {
			t.Fatalf("insert item[%d-%d] failed", v.Populace, v.Uid)
		}
	}

	// Update half elements
	manyUpdate(t, zsl, set, units/2)

	if zsl.Len() != units {
		t.Fatalf("unexpected skiplist element count")
	}

	// Delete all elements
	for _, v := range set {
		var node = zsl.Delete((v.Populace), v)
		if node == nil {
			t.Fatalf("delete set item[%d-%d] failed", v.Populace, v.Uid)
		}
		if player := node.Obj.(*testPlayer); player.Uid != v.Uid {
			t.Fatalf("delete set item, %d not equal to %d", player.Uid, v.Uid)
		}
	}
	if zsl.Len() != 0 {
		t.Fatalf("skiplist expected empty, but got size: %d", zsl.Len())
	}
}

func TestZSkipListGetRank(t *testing.T) {
	const units = 10000
	var set = makeTestPlayers(units, units, false)
	var zsl = NewZSkipList()
	for _, v := range set {
		var node = zsl.Insert(v.Populace, v)
		if node == nil {
			t.Fatalf("insert item[%d-%d] failed", v.Populace, v.Uid)
		}
	}

	// rank by sort package
	var ranks = mapToSlice(set)
	sort.SliceStable(ranks, func(i, j int) bool {
		return ranks[i].Populace < ranks[j].Populace
	})

	for i := len(ranks); i > 0; i-- {
		var v = ranks[i-1]
		var thisRank = len(ranks) - i + 1
		var rank = zsl.Len() - zsl.GetRank(v.Populace, v) + 1
		if rank != thisRank {
			dumpSliceToFile(ranks, "slice.dat")
			dumpToFile(zsl, "zskiplist.dat")
			t.Fatalf("%v not equal at rank, %d != %d", v, rank, thisRank)
			break
		}
	}
}

func TestZSkipListUpdateGetRank(t *testing.T) {
	const units = 10000
	var set = makeTestPlayers(units, units, false)
	var zsl = NewZSkipList()
	for _, v := range set {
		var node = zsl.Insert(v.Populace, v)
		if node == nil {
			t.Fatalf("insert item[%d-%d] failed", v.Populace, v.Uid)
		}
	}

	var maxTurn = 100
	for i := 0; i < maxTurn; i++ {
		manyUpdate(t, zsl, set, units/2)

		// rank by sort package
		var ranks = mapToSlice(set)
		sort.SliceStable(ranks, func(i, j int) bool {
			return ranks[i].Populace < ranks[j].Populace
		})

		for i := len(ranks); i > 0; i-- {
			var v = ranks[i-1]
			var rank = zsl.GetRank(v.Populace, v)
			var myRank = zsl.Len() - rank + 1
			var thisRank = len(ranks) - i + 1
			if myRank != thisRank {
				var node = zsl.GetElementByRank(rank)
				if node == nil {
					dumpSliceToFile(ranks, "slice.dat")
					dumpToFile(zsl, "zskiplist.dat")
					t.Fatalf("%v GetElementByRank return nil: %d", v, rank)
					break
				}
				var player = node.Obj.(*testPlayer)
				if player.Populace == v.Populace {
					// OK, cuz skip list sort is not stable
				} else {
					dumpSliceToFile(ranks, "slice.dat")
					dumpToFile(zsl, "zskiplist.dat")
					t.Fatalf("rank not equal, %v, %v as %d != %d", v, player, myRank, thisRank)
					break
				}
			}
		}
	}
}

func BenchmarkZSkipListInsert(b *testing.B) {
	b.StopTimer()
	var zsl = NewZSkipList()
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		obj := &testPlayer{
			Uid:      uint64(i),
			Level:    uint16(i),
			Populace: uint64(i),
		}
		if node := zsl.Insert((obj.Populace), obj); node == nil {
			b.Fatalf("insert item[%d-%d] failed", obj.Populace, obj.Uid)
		}
	}
}

func BenchmarkZSkipListUpdate(b *testing.B) {
	b.StopTimer()
	const units = 100000
	var set = makeTestPlayers(units, units, true)
	var zsl = NewZSkipList()
	for _, v := range set {
		var node = zsl.Insert(v.Populace, v)
		if node == nil {
			b.Fatalf("insert item[%d-%d] failed", v.Populace, v.Uid)
		}
	}
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		manyUpdate(b, zsl, set, units/2)
	}
}

func BenchmarkZSkipListGetRank(b *testing.B) {
	b.StopTimer()
	const units = 100000
	var set = makeTestPlayers(units, units, false)
	var zsl = NewZSkipList()
	for _, v := range set {
		var node = zsl.Insert(v.Populace, v)
		if node == nil {
			b.Fatalf("insert item[%d-%d] failed", v.Populace, v.Uid)
		}
	}
	b.StartTimer()
	for i := 1; i < b.N; i++ {
		var obj *testPlayer
		for _, v := range set {
			obj = v
			break
		}
		zsl.GetRank(obj.Populace, obj)
	}
}

func BenchmarkZSkipListGetElementByRank(b *testing.B) {
	b.StopTimer()
	const units = 100000
	var set = makeTestPlayers(units, units, false)
	var zsl = NewZSkipList()
	for _, v := range set {
		var node = zsl.Insert(v.Populace, v)
		if node == nil {
			b.Fatalf("insert item[%d-%d] failed", v.Populace, v.Uid)
		}
	}
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		var rank = (i % units) + 1
		zsl.GetElementByRank(rank)
	}
}
