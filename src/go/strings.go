package types

import (
	"log"
	"math/rand"
	"reflect"
	"unicode"
	"unsafe"
)

const asciiAlphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

// CAUTION!
// 对byte的修改会影响string
func FastBytesToString(b []byte) string {
	bh := (*reflect.SliceHeader)(unsafe.Pointer(&b))
	sh := reflect.StringHeader{Data: bh.Data, Len: bh.Len}
	return *(*string)(unsafe.Pointer(&sh))
}

// CAUTION!
// 如果修改了作为返回值的byte会引起panic
func FastStringToBytes(s string) []byte {
	sh := (*reflect.StringHeader)(unsafe.Pointer(&s))
	bh := reflect.SliceHeader{Data: sh.Data, Len: sh.Len, Cap: 0}
	return *(*[]byte)(unsafe.Pointer(&bh))
}

//随机字符串
func RandAlphbetString(length int) string {
	result := make([]byte, length)
	for i := 0; i < length; i++ {
		idx := rand.Int() % len(asciiAlphabet)
		result[i] = asciiAlphabet[idx]
	}
	return string(result)
}

func RandBytes(length int) []byte {
	result := make([]byte, length)
	for i := 0; i < length; i++ {
		ch := uint8(rand.Int31() % 0xFF)
		result[i] = ch
	}
	return result
}

//查找一个字符串
func FindString(a []string, x string) int {
	for i, v := range a {
		if v == x {
			return i
		}
	}
	return -1
}

//查找第一个数字
func FindFirstDigit(s string) int {
	for i, r := range s {
		if unicode.IsDigit(r) {
			return i
		}
	}
	return -1
}

func StringSliceToMap(list []string) map[string]bool {
	dict := make(map[string]bool, len(list))
	for _, s := range list {
		if _, found := dict[s]; found {
			log.Printf("StringSliceToMap: duplicate element,[%s]", s)
		}
		dict[s] = true
	}
	return dict
}
