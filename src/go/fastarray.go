package fastarray

//  Base class for objects stored in the array.
type ArrayItemBase struct {
	arrayIndex int
}

func (i *ArrayItemBase) SetArrayIndex(index int) {
	i.arrayIndex = index
}

func (i *ArrayItemBase) ArrayIndex() int {
	return i.arrayIndex
}

type ArrayElementer interface {
	SetArrayIndex(int)
	ArrayIndex() int
}

//  Fast array implementation with O(1) access to item, insertion and
//  removal. Array stores pointers rather than objects.
type FastArray struct {
	array []ArrayElementer
}

func (a *FastArray) Len() int {
	return len(a.array)
}

func (a *FastArray) Get(idx int) ArrayElementer {
	return a.array[idx]
}

func (a *FastArray) Clear() {
	a.array = a.array[:0]
}

func (a *FastArray) Push(val ArrayElementer) {
	if val != nil {
		val.SetArrayIndex(len(a.array))
	}
	a.array = append(a.array, val)
}

func (a *FastArray) Remove(val ArrayElementer) {
	a.RemoveAt(val.ArrayIndex())
}

func (a *FastArray) RemoveAt(idx int) {
	var back = a.array[len(a.array)-1]
	if back != nil {
		back.SetArrayIndex(idx)
	}
	a.array[idx] = back
	a.array = a.array[:len(a.array)-1]
}

func (a *FastArray) Swap(idx1, idx2 int) {
	var x = a.array[idx1]
	var y = a.array[idx2]
	if x != nil {
		a.array[idx1].SetArrayIndex(idx2)
	}
	if y != nil {
		a.array[idx2].SetArrayIndex(idx1)
	}
	a.array[idx1], a.array[idx2] = a.array[idx2], a.array[idx1]
}
