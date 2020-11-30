package main

import (
	"encoding/binary"
	"flag"
	"fmt"
	"geometry"
	"gridmap"
	"hash/crc32"
	"log"
)

var (
	IsSlowFlag      = flag.Bool("slow", false, "use slow but accurate algorithm")
	ScoutWidth      = flag.Int("width", 30, "scout square width")
	SquareCenterPos = flag.Int("center", 60, "center position of square")
)

func testCellPoint() {
	var input = []int32{
		0, 0,
		10, 10,
		120, 120,
		110, 110,
		110, 0,
	}
	var expect = []int32{
		0, 0,
		0, 0,
		120, 120,
		105, 105,
		105, 0,
	}
	for i := 0; i < len(input); i += 2 {
		var pt = geometry.NewCoordinate(input[i], input[i+1])
		var ept = geometry.NewCoordinate(expect[i], expect[i+1])
		var cpt = gridmap.MistCellOfPoint(pt)
		if ept != cpt {
			log.Fatal("expect %v --> %v, but got %v\n", pt, ept, cpt)
		} else {
			fmt.Printf("point %v --> %v\n", pt, cpt)
		}
	}
}

func testRectCells() {
	var isSlow = *IsSlowFlag
	var width int32 = int32(*ScoutWidth)
	var center int32 = int32(*SquareCenterPos)
	fmt.Printf("is slow: %v, square width: %d\n", isSlow, width)
	var centerPos = geometry.NewCoordinate(center, center)
	var cells []geometry.Coordinate
	if isSlow {
		cells = gridmap.GetSortedMistCellsSlow(centerPos, width)
	} else {
		cells = gridmap.GetSortedMistCells(centerPos, width)
	}

	var hash = crc32.NewIEEE()
	fmt.Printf("#%d cells point of center %v\n", center, len(cells))
	for _, pt := range cells {
		fmt.Printf("cell point: %v\n", pt)
		var buf [4]byte
		binary.LittleEndian.PutUint32(buf[0:], uint32(pt.X))
		hash.Write(buf[0:])
		binary.LittleEndian.PutUint32(buf[0:], uint32(pt.Z))
		hash.Write(buf[0:])
	}
	fmt.Printf("checksum is %v\n", hash.Sum32())
}

func main() {
	flag.Parse()

	//testCellPoint()
	testRectCells()
}
