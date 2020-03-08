// Copyright © 2017 ichenq@outlook.com All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

// +build !ignore

package strutil

import (
	"testing"
	"time"
)

type myStruct struct {
	Name string

	U64 uint64
	U32 uint32
	U16 uint16
	U8  uint8

	I64 int64
	I32 int32
	I16 int16
	I8  int8

	Bool bool

	F32 float32
	F64 float64

	Array    []int
	Composed time.Time //角色创建日期

	internal int
}

func (m *myStruct) SetName(v string) {
	m.Name = v
}

func (m *myStruct) GetName() string {
	return m.Name
}

func checkCond(t *testing.T, v bool) {
	if !v {
		t.Fatal()
	}
}

func TestHasField(t *testing.T) {
	var s = &myStruct{}
	checkCond(t, HasField(s, "Name"))
	checkCond(t, HasField(s, "U32"))
	checkCond(t, HasField(s, "I32"))
	checkCond(t, HasField(s, "F64"))
	checkCond(t, HasField(s, "Array"))
	checkCond(t, HasField(s, "Composed"))
	checkCond(t, HasField(s, "internal"))
}

func TestGetIntField(t *testing.T) {
	var s = &myStruct{
		U64:      123456789012345,
		I64:      888888888888888,
		internal: 1234,
	}

	if v, err := GetIntField(s, "U64"); err != nil {
		t.Fatalf("GetIntField: %v", err)
	} else {
		checkCond(t, v == 123456789012345)
	}

	if v, err := GetIntField(s, "I64"); err != nil {
		t.Fatalf("GetIntField: %v", err)
	} else {
		checkCond(t, v == 888888888888888)
	}

	if v, err := GetIntField(s, "internal"); err != nil {
		t.Fatalf("GetIntField: %v", err)
	} else {
		checkCond(t, v == 1234)
	}
}

func TestSetIntField(t *testing.T) {
	var s = &myStruct{}
	checkCond(t, s.U64 == 0)
	checkCond(t, s.I64 == 0)
	checkCond(t, s.internal == 0)

	if err := SetIntField(s, "U64", 123456789012345); err != nil {
		t.Fatalf("SetIntField: %v", err)
	}
	checkCond(t, s.U64 == 123456789012345)

	if err := SetIntField(s, "I64", 888888888888888); err != nil {
		t.Fatalf("SetIntField: %v", err)
	}
	checkCond(t, s.I64 == 888888888888888)

	if err := SetIntField(s, "internal", 1234); err == nil {
		t.Fatalf("SetIntField: %v", err)
	} else {
		t.Logf("%v", err)
	}
	checkCond(t, s.internal == 0)
}

func TestGetFloatField(t *testing.T) {
	var s = &myStruct{
		F32: 3.14,
		F64: 1.618e20,
	}
	// caution! F32 -> v ==> float32 -> float64
	if v, err := GetFloatField(s, "F32"); err != nil {
		t.Fatalf("GetFloatField: %v", err)
	} else {
		checkCond(t, v-3.14 < 1e7) // less precision
	}

	if v, err := GetFloatField(s, "F64"); err != nil {
		t.Fatalf("GetFloatField: %v", err)
	} else {
		checkCond(t, v == 1.618e20)
	}
}

func TestSetFloatField(t *testing.T) {
	var s = &myStruct{}
	checkCond(t, s.F64 == 0)

	if err := SetFloatField(s, "F64", 3.1415926); err != nil {
		t.Fatalf("SetFloatField: %v", err)
	}
	checkCond(t, s.F64 == 3.1415926)
}

func TestGetBoolField(t *testing.T) {
	var s = &myStruct{
		Bool: true,
	}
	if v, err := GetBoolField(s, "Bool"); err != nil {
		t.Fatalf("GetBoolField: %v", err)
	} else {
		checkCond(t, v)
	}
}

func TestSetBoolField(t *testing.T) {
	var s = &myStruct{}
	checkCond(t, !s.Bool)
	if err := SetBoolField(s, "Bool", true); err != nil {
		t.Fatalf("SetBoolField: %v", err)
	}
	checkCond(t, s.Bool)
}

func TestGetStringField(t *testing.T) {
	var s = &myStruct{
		Name: "hellokitty",
	}
	if v, err := GetStringField(s, "Name"); err != nil {
		t.Fatalf("GetStringField: %v", err)
	} else {
		checkCond(t, v == "hellokitty")
	}
}

func TestSetStringField(t *testing.T) {
	var s = &myStruct{}
	checkCond(t, s.Name == "")
	if err := SetStringField(s, "Name", "hellokitty"); err != nil {
		t.Fatalf("SetStringField: %v", err)
	}
	checkCond(t, s.Name == "hellokitty")
}
