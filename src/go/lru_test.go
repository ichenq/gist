package types

import (
	"testing"
)

func TestLRUCacheExample(t *testing.T) {
	c := NewLRUCache(32)
	for i := 1; i <= 32; i++ {
		c.Put(i, i*10)
	}
	for i := 1; i <= 32; i++ {
		v := c.Get(i)
		if n := v.(int); n != i*10 {
			t.Fatalf("not equal")
		}
	}
	for i := 33; i <= 40; i++ {
		c.Put(i, i*10)
	}
}
