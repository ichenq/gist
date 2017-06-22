// +build !ignore

package types

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
