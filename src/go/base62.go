package types

var b62Alphabet = []rune("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789")
var b62IndexTable = map[rune]int64{}

func init() {
	b62IndexTable = buildIndexTable(b62Alphabet)
}

// build alphabet index
func buildIndexTable(s []rune) map[rune]int64 {
	var table = make(map[rune]int64, len(s))
	for i := 0; i < len(s); i++ {
		table[s[i]] = int64(i)
	}
	return table
}

// EncodeToBase62String convert an interger to base-62 string
func EncodeToBase62String(id int64) string {
	if id == 0 {
		return string(b62Alphabet[:1])
	}
	var short = make([]rune, 0, 12)
	for id > 0 {
		var rem = id % 62
		id = id / 62
		short = append(short, b62Alphabet[rem])
	}
	// reverse
	for i, j := 0, len(short)-1; i < j; i, j = i+1, j-1 {
		short[i], short[j] = short[j], short[i]
	}
	return string(short)
}

// DecodeBase62String convert an base-62 string back to interger
func DecodeBase62String(s string) int64 {
	var n int64
	for _, r := range s {
		n = (n * 62) + b62IndexTable[r]
	}
	return n
}
