// Copyright © 2016-present ichenq@outlook.com  All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.
//

package algo

import (
	"bytes"
	"strings"
	"unicode"
)

const WildCardChar = '\u002A' // 通配符'*'

type TrieNode struct {
	children map[rune]*TrieNode
	isEnd    bool
}

func NewTrieNode() *TrieNode {
	return &TrieNode{
		children: make(map[rune]*TrieNode),
		isEnd:    false,
	}
}

type SensitiveTrie struct {
	root *TrieNode
	size int
}

func NewSensitiveTrie() *SensitiveTrie {
	return &SensitiveTrie{
		root: NewTrieNode(),
		size: 0,
	}
}

func (t *SensitiveTrie) Init() error {
	t.root = NewTrieNode()
	t.size = 0
	return nil
}

func (t *SensitiveTrie) WordsCount() int {
	return t.size
}

//插入需要过滤的敏感词
func (t *SensitiveTrie) Insert(word string) {
	var node = t.root
	for _, r := range word {
		var ch = unicode.ToLower(r)
		if _, found := node.children[ch]; !found {
			node.children[ch] = NewTrieNode()
		}
		node = node.children[ch]
	}
	node.isEnd = true
	t.size++
}

//从指定位置开始匹配
func (t *SensitiveTrie) MatchAt(words []rune, start int) int {
	node := t.root
	for start >= 0 && start < len(words) {
		var word = unicode.ToLower(words[start])
		child, found := node.children[word]
		if !found {
			child, found = node.children[WildCardChar] //匹配通配符
			if found {
				if child.isEnd {
					return start
				}
				node = child
				if child, found := node.children[word]; found {
					if child.isEnd {
						return start
					}
					node = child
				}
				start++
				continue
			}
			return -1
		}
		if child.isEnd {
			return start
		}
		node = child
		start++
	}
	if node.isEnd {
		return start
	}
	return -1
}

//匹配一串文字
func (t *SensitiveTrie) MatchWords(words []rune) (int, int) {
	for i := 0; i < len(words); i++ {
		index := t.MatchAt(words, i)
		if index >= 0 {
			return i, index + 1
		}
	}
	return -1, -1
}

//是否有敏感词
func (t *SensitiveTrie) MatchString(text string) bool {
	var words = []rune(text)
	start, end := t.MatchWords(words)
	return start < 0 && end < 0
}

//将敏感字符替换为*
func (t *SensitiveTrie) FilterText(text string) string {
	var words = []rune(text)
	var start, end = t.MatchWords(words)
	if start < 0 && end < 0 { // common case
		return text
	}
	var buf bytes.Buffer
	for end > 0 {
		buf.WriteString(string(words[:start]))
		buf.WriteString(strings.Repeat("*", end-start))
		words = words[end:]
		start, end = t.MatchWords(words)
	}
	buf.WriteString(string(words))
	return buf.String()
}
