package types

// double-linked list
type LRUCacheNode struct {
	next, prev *LRUCacheNode
	key, value interface{}
}

type LRUCache struct {
	root     LRUCacheNode
	dict     map[interface{}]*LRUCacheNode
	size     int
	capacity int
}

func NewLRUCache(capacity int) *LRUCache {
	c := &LRUCache{
		capacity: capacity,
		dict:     make(map[interface{}]*LRUCacheNode, capacity),
	}
	c.root.next = &c.root
	c.root.prev = &c.root
	return c
}

func (c *LRUCache) Len() int {
	return c.size
}

func (c *LRUCache) Cap() int {
	return c.capacity
}

// removeListElement removes e from its list, decrements size, and returns e.
func (c *LRUCache) removeListElement(e *LRUCacheNode) *LRUCacheNode {
	e.prev.next = e.next
	e.next.prev = e.prev
	e.next = nil // avoid memory leaks
	e.prev = nil // avoid memory leaks
	c.size--
	return e
}

// insertListElement inserts e after at, increments size, and returns e.
func (c *LRUCache) insertListElement(e, at *LRUCacheNode) *LRUCacheNode {
	next := at.next
	at.next = e
	e.prev = at
	e.next = next
	next.prev = e
	c.size++
	return e
}

// moveElementToFront moves element e to the front of list.
func (c *LRUCache) moveElementToFront(e *LRUCacheNode) {
	c.insertListElement(c.removeListElement(e), &c.root)
}

// clean remove expired cache item.
func (c *LRUCache) clean() {
	for len(c.dict) > c.capacity {
		e := c.root.prev // back element
		c.removeListElement(e)
		delete(c.dict, e.key)
	}
}

func (c *LRUCache) Exist(key interface{}) bool {
	_, found := c.dict[key]
	return found
}

func (c *LRUCache) Get(key interface{}) interface{} {
	e, found := c.dict[key]
	if found {
		c.moveElementToFront(e)
		return e.value
	}
	return nil
}

func (c *LRUCache) Put(key, value interface{}) {
	e, exist := c.dict[key]
	if exist {
		e.value = value
		c.moveElementToFront(e)
	} else {
		e = &LRUCacheNode{key: key, value: value}
		c.insertListElement(e, &c.root) // push front
		c.dict[key] = e
		c.clean()
	}
}
