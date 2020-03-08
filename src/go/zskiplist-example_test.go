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
	"fmt"
)

//简单的角色信息示例
type rankPlayer struct {
	id    uint64
	name  string
	level uint32
	score uint64
}

func (p *rankPlayer) Uuid() uint64 {
	return p.id
}

func ExampleZSkipList() {
	var playerMap = make(map[uint64]*rankPlayer)
	var zsl = NewZSkipList()

	//简单的测试角色数据
	var p1 = &rankPlayer{id: 1001, name: "jack", level: 12, score: 2012}
	var p2 = &rankPlayer{id: 1002, name: "tom", level: 13, score: 2015}
	var p3 = &rankPlayer{id: 1003, name: "mike", level: 14, score: 2014}
	var p4 = &rankPlayer{id: 1004, name: "john", level: 11, score: 2014}
	var p5 = &rankPlayer{id: 1005, name: "kevin", level: 14, score: 2011}
	playerMap[p1.id] = p1
	playerMap[p2.id] = p2
	playerMap[p3.id] = p3
	playerMap[p4.id] = p4
	playerMap[p5.id] = p5

	//插入角色数据到zskiplist
	for _, v := range playerMap {
		zsl.Insert(v.score, v)
	}

	//打印调试信息
	fmt.Printf("%v\n", zsl)

	//获取角色的排行信息
	var rank = zsl.GetRank(p1.score, p1) // in ascend order
	var myRank = zsl.Len() - rank + 1    // get descend rank
	fmt.Printf("rank of %s: %d\n", p1.name, myRank)

	//根据排行获取角色信息
	var node = zsl.GetElementByRank(rank)
	var player = playerMap[node.Obj.Uuid()]
	fmt.Printf("rank at %d is: %s\n", rank, player.name)

	//遍历整个zskiplist
	zsl.Walk(true, func(rank int, v RankInterface) bool {
		fmt.Printf("rank %d: %v", rank, v)
		return true
	})

	//从zskiplist中删除p1
	if zsl.Delete(p1.score, p1) == nil {
		// error handling
	}

	p1.score += 10
	if zsl.Insert(p1.score, p1) == nil {
		// error handling
	}
}
