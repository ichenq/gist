package gridmap

import (
	"geometry"
	"sort"
)

const (
	MAP_WIDTH       = 7200 // 地图度(x)
	MAP_HEIGHT      = 7200 // 地图高(y)
	MIST_BLOCK_SIZE = 90   // 迷雾块大小
	MIST_CELL_SIZE  = 15   // 迷雾格子大小
)

// 正方形左下角
func GetSquareBottom(center geometry.Coordinate, width int32) geometry.Coordinate {
	var originX = center.X - width/2
	if originX < 0 {
		originX = 0
	} else if originX >= MAP_WIDTH {
		originX = MAP_WIDTH - 1
	}
	var originZ = center.Z - width/2
	if originZ < 0 {
		originZ = 0
	} else if originZ >= MAP_HEIGHT {
		originZ = MAP_HEIGHT - 1
	}
	return geometry.NewCoordinate(originX, originZ)
}

// 坐标所在的迷雾格子
func MistCellOfPoint(pt geometry.Coordinate) geometry.Coordinate {
	var cellX = pt.X - (pt.X % MIST_CELL_SIZE)
	var cellZ = pt.Z - (pt.Z % MIST_CELL_SIZE)
	return geometry.Coordinate{X: cellX, Z: cellZ}
}

// 获取正方形在地图所占格子
func GetMistCellsBySquare(pos geometry.Coordinate, width int32) []geometry.Coordinate {
	var bottomLeft = MistCellOfPoint(pos)
	var bottomRight = MistCellOfPoint(geometry.Coordinate{X: pos.X + width, Z: pos.Z})
	var count = (bottomRight.X - bottomLeft.X) / MIST_CELL_SIZE
	if pos.X%MIST_CELL_SIZE > 0 {
		count++
	}
	var cells = make([]geometry.Coordinate, 0, count*count)
	for i := int32(0); i < count; i++ {
		for j := int32(0); j < count; j++ {
			var x = bottomLeft.X + i*MIST_CELL_SIZE
			var z = bottomLeft.Z + j*MIST_CELL_SIZE
			var pt = geometry.Coordinate{X: x, Z: z}
			cells = append(cells, pt)
		}
	}
	return cells
}

func GetMistCellsBySquareSlow(pos geometry.Coordinate, width int32) map[geometry.Coordinate]bool {
	var cells = make(map[geometry.Coordinate]bool, 9)
	for x := pos.X; x < pos.X+width; x += 1 {
		for y := pos.Z; y < pos.Z+width; y += 1 {
			var pt = MistCellOfPoint(geometry.Coordinate{X: x, Z: y})
			cells[pt] = true
		}
	}
	return cells
}

// 获取正方形在地图所占格子
func GetMistCellsBy(center geometry.Coordinate, width int32) map[geometry.Coordinate]bool {
	var bottom = GetSquareBottom(center, width)
	return GetMistCellsBySquareSlow(bottom, width)
}

func GetSortedMistCellsSlow(center geometry.Coordinate, width int32) []geometry.Coordinate {
	var cells = GetMistCellsBy(center, width)
	var slice = make([]geometry.Coordinate, 0, len(cells))
	for pt, _ := range cells {
		slice = append(slice, pt)
	}
	sort.Slice(slice, func(i, j int) bool {
		if slice[i].X == slice[j].X {
			return slice[i].Z < slice[j].Z
		}
		return slice[i].X < slice[j].X
	})
	return slice
}

func GetSortedMistCells(center geometry.Coordinate, width int32) []geometry.Coordinate {
	var bottom = GetSquareBottom(center, width)
	var cells = GetMistCellsBySquare(bottom, width)
	sort.Slice(cells, func(i, j int) bool {
		if cells[i].X == cells[j].X {
			return cells[i].Z < cells[j].Z
		}
		return cells[i].X < cells[j].X
	})
	return cells
}
