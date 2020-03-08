// Copyright © 2017 ichenq@outlook.com All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

package strutil

import (
	"math/rand"
	"reflect"
	"regexp"
	"unicode"
	"unsafe"
)

const alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-=~!@#$%^&*()_+[]\\;',./{}|:<>?"

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
	if length <= 0 {
		return ""
	}
	result := make([]byte, length)
	for i := 0; i < length; i++ {
		idx := rand.Int() % len(alphabet)
		result[i] = alphabet[idx]
	}
	return string(result)
}

func RandBytes(length int) []byte {
	if length <= 0 {
		return nil
	}
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

// 国际手机号验证 https://numverify.com/
//
// 国内11位手机号： 1XX-XXXX-XXXX， 网络识别号-地区编码-用户号码
func ValidateCNPhoneNumber(phoneNo string) bool {
	var n = len(phoneNo)
	if n < 11 {
		return false
	}
	if phoneNo[:3] == "+86" {
		phoneNo = phoneNo[3:]
		n -= 3
	}
	if n != 11 {
		return false
	}
	if phoneNo[0] != '1' {
		return false
	}
	for i := 1; i < n; i++ {
		if !unicode.IsDigit(rune(phoneNo[i])) {
			return false
		}
	}
	return true
}

// 合法email地址：
//  1. 必须包含一个并且只有一个符号“@”
//  2. 第一个字符不得是“@”或者“.”
//  3. 不允许出现“@.”或者.@
//  4. 结尾不得是字符“@”或者“.”
//  5. 允许“@”前的字符中出现“+”
//  6. 不允许“+”在最前面，或者“+@”
var emailRegexp = regexp.MustCompile("^\\s*\\w+(?:\\.{0,1}[\\w-]+)*@[a-zA-Z0-9]+(?:[-.][a-zA-Z0-9]+)*\\.[a-zA-Z]+\\s*$")

//邮箱地址校验
func ValidateEmailAddress(email string) bool {
	return emailRegexp.MatchString(email)
}

// Reverse reverses the string
func Reverse(str string) string {
	runes := []rune(str)
	l := len(runes)
	for i := 0; i < l/2; i++ {
		runes[i], runes[l-i-1] = runes[l-i-1], runes[i]
	}
	return string(runes)
}
