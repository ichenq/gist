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
	"bytes"
	"testing"
)

func checkStrEqual(t *testing.T, s1, s2 string) {
	if s1 != s2 {
		t.Fatalf("string not equal, %s != %s", s1, s2)
	}
}

func checkBytesEqual(t *testing.T, b1, b2 []byte) {
	if !bytes.Equal(b1, b2) {
		t.Fatalf("bytes not equal, %v != %v", b1, b2)
	}
}

func TestFastBytesToString(t *testing.T) {
	var rawbytes = RandBytes(1024)
	checkStrEqual(t, string(rawbytes), FastBytesToString(rawbytes))
}

func TestFastStringToBytes(t *testing.T) {
	var text = RandAlphbetString(1024)
	checkBytesEqual(t, []byte(text), FastStringToBytes(text))
}

func TestFindString(t *testing.T) {
	array := []string{"one", "two", "three", "four"}
	if i := FindString(array, "two"); i != 1 {
		t.Fatalf("string not found")
	}
	if FindString(array, "") >= 0 {
		t.Fatalf("unexpected result")
	}
}

func TestReverse(t *testing.T) {
	tests := []struct {
		input    string
		expected string
	}{
		{"", ""},
		{"abc", "cba"},
		{"a", "a"},
		{"çınar", "ranıç"},
		{"    yağmur", "rumğay    "},
		{"επαγγελματίες", "ςείταμλεγγαπε"},
	}

	for i, test := range tests {
		output := Reverse(test.input)
		if test.expected != output {
			t.Fatalf("Test case %d failed, expect %s, got %s", i, test.expected, output)
		}
	}
}

func TestValidatePhoneNumber(t *testing.T) {
	tests := []struct {
		input    string
		expected bool
	}{
		{"", false},
		{"1abc", false},
		{"+1a", false},
		{"13156781234", true},
		{"+8613156781234", true},
		{"+13156781234", false},
	}
	for i, test := range tests {
		output := ValidateCNPhoneNumber(test.input)
		if test.expected != output {
			t.Fatalf("Test case %d failed, %s, expect %v, got %v", i, test.input, test.expected, output)
		}
	}
}

func TestValidateEmailAddress(t *testing.T) {
	tests := []struct {
		input    string
		expected bool
	}{
		{"", false},
		{"@", false},
		{"@.com", false},
		{"a.@a.com", false},
		{"a@.com", false},
		{"@a@.com", false},
		{".a@.com", false},
		{"a+b@.com", false},
		{"a&b@.com", false},
		{"abc@abc.com", true},
		{"ab.c@abc.com", true},
	}
	for i, test := range tests {
		output := ValidateEmailAddress(test.input)
		if test.expected != output {
			t.Fatalf("Test case %d failed, %s, expect %v, got %v", i, test.input, test.expected, output)
		}
	}
}

func BenchmarkBytesToString(b *testing.B) {
	b.StopTimer()
	var rawbytes = RandBytes(2048)
	b.StartTimer()
	var text string
	for i := 0; i < b.N; i++ {
		text = string(rawbytes)
	}
	text = text[:0]
}

func BenchmarkFastBytesToString(b *testing.B) {
	b.StopTimer()
	var rawbytes = RandBytes(2048)
	b.StartTimer()
	var text string
	for i := 0; i < b.N; i++ {
		text = FastBytesToString(rawbytes)
	}
	text = text[:0]
}

func BenchmarkStringToBytes(b *testing.B) {
	b.StopTimer()
	var text = string(RandAlphbetString(2048))
	b.StartTimer()
	var rawbytes []byte
	for i := 0; i < b.N; i++ {
		rawbytes = []byte(text)
	}
	rawbytes = rawbytes[:0]
}

func BenchmarkFastStringToBytes(b *testing.B) {
	b.StopTimer()
	var text = string(RandAlphbetString(2048))
	b.StartTimer()
	var rawbytes []byte
	for i := 0; i < b.N; i++ {
		rawbytes = FastStringToBytes(text)
	}
	rawbytes = rawbytes[:0]
}
