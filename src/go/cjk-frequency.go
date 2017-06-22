// CJK character frequency stats
package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"sort"
	_ "unicode/utf8"
)

func main() {
	var url string
	flag.StringVar(&url, "U", "http://movie.douban.com/review/7763971/", "default URL")
	flag.Parse()

	response, err := http.Get(url)
	if err != nil {
		log.Fatal(err)
	}
	defer response.Body.Close()
	body, err := ioutil.ReadAll(response.Body)
	if err != nil {
		log.Fatal(err)
	}
	content := string(body)
	fmt.Println(content)
	stats := parseFrequency(content)
	fmt.Println("total cjk count: ", len(stats))
	for _, v := range stats {
		fmt.Printf("%s: 0x%x %d\n", string(v.char), v.char, v.freq)
	}
}

func parseFrequency(content string) SortedStats {
	dict := map[rune]int{}
	for _, char := range content {
		if IsCJK(char) {
			dict[char]++
		}
	}
	var stats SortedStats
	for k, v := range dict {
		stats = append(stats, Stats{k, v})
	}
	sort.Sort(stats)
	return stats
}

// https://en.wikipedia.org/wiki/CJK_Unified_Ideographs
func IsCJK(r rune) bool {
	return (r >= 0x4E00 && r <= 0x62FF) ||
		(r >= 0x6300 && r <= 0x77FF) ||
		(r >= 0x8D00 && r <= 0x9FFFF)
}

type Stats struct {
	char rune
	freq int
}

type SortedStats []Stats

func (s SortedStats) Len() int {
	return len(s)
}

func (s SortedStats) Swap(i, j int) {
	s[i], s[j] = s[j], s[i]
}

func (s SortedStats) Less(i, j int) bool {
	return s[i].freq > s[j].freq
}
