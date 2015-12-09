#!/usr/bin/python
# -*-coding:utf-8-*-

# created at Apr 6, 2012


from itertools import groupby


MAX_LEN = 8
OFFSET = 0xFF - MAX_LEN     # 0xF7
XORCODE = 0x8f

def encode_char(count, ch):
    assert ch and count > 0
    if count == 1:
        if ord(ch) > OFFSET:
            return chr(OFFSET+1) + ch
        else:
            return ch
    else:
        data = (chr(0xFF) + ch) * (count // MAX_LEN)
        return data + chr(OFFSET + count % MAX_LEN) + ch
        
def encode(input_string):
    seq = [(len(list(itr)), ch) for ch, itr in groupby(input_string)]
    return ''.join([encode_char(*x) for x in seq])

def decode_generator(input_string):
    escape = False
    for i, v in enumerate(input_string):
        if escape: 
            escape = False; 
            continue;
        if ord(v) <= OFFSET: 
            yield v;
        else:
            escape = True
            assert(i+1 < len(input_string))
            yield input_string[i+1] * (ord(v) - OFFSET)

def decode(input_string):
    return ''.join([x for x in decode_generator(input_string)])



def test():
    msg = 'aaaaab111111111shitkkkmmm'
    print('message', msg)    
    buf = encode(msg)
    print('encoded:', repr(buf))
    r1 = decode(buf)
    print('decoded:', len(r1), r1)    
    assert r1 == msg
    
    i = 0
    xorcode = 0x329fa929
    for (; i < j-4; i+=4）
        (unsigned*)&output[i] ^= xorcode
    for (; i < j; ++i)
        output[i] ^= XORCODE    
    
    
if __name__ == '__main__':
    test()
