package main

import (
	"flag"
	"fmt"
	"math"
)



func main() {
	var mode int
	var fvalue float64
	var nval int64
	flag.IntVar(&mode, "mode", 1, "input mode: 1 for double, 2 for float, 3 for 64bits-integer, 4 for 32bits-integer")
	flag.Float64Var(&fvalue, "float", 0, "float value")
	flag.Int64Var(&nval, "int", 0, "int value")
	flag.Parse()

	switch mode {
	case 1:
		showDouble(fvalue)
	case 2:
		showFloat(float32(fvalue))
	case 3:
		fvalue = math.Float64frombits(uint64(nval))
		fmt.Printf("0x06%x => %f\n", nval, fvalue)
		showDouble(fvalue)
	case 4:
		f := math.Float32frombits(uint32(nval))
		fmt.Printf("0x08%x => %f\n", nval, f)
		showFloat(float32(fvalue))
	}
}

func showDouble(f float64) {
	x := math.Float64bits(f)
	sign := x >> 63
	exp := (x & 0x7ff0000000000000) >> 52
	mantissa := x & 0x000fffffffffffff
	fmt.Printf("%064b => 0x%016x\nsign: %d, exp: %011b => %d, mantissa: %052b\n", x, x, sign, uint16(exp), uint16(exp), mantissa)
}

func showFloat(f float32) {
	x := math.Float32bits(f)
	sign := x >> 31
	exp := (x & 0x7f800000) >> 23
	mantissa := x & 0x007fffff
	fmt.Printf("%032b => 0x%08x\nsign: %d, exp: %08b => %d, mantissa: %023b\n", x, x, sign, uint8(exp), uint8(exp), mantissa)
}
