
## LAN

局域网 192.168.0.42  --> 192.168.0.112

64 bytes ICMP ttl=64 time=0.044 ms


## 阿里云

### client <-> DataCenter

成都 --> 深圳

ping dev.3bodygame.com
64 bytes ICMP ttl=51 time=56.8 ms

成都 --> 杭州

ping ce.3bodygame.com
64 bytes ICMP ttl=53 time=45.1 ms


### DataCenter <-> DataCenter


深圳 --> 杭州

64 bytes ICMP ttl=55 time=26.9 ms

杭州 --> 深圳

64 bytes ICMP ttl=56 time=27.0 ms

## AWS

### client <--> DataCenter

成都  --> AWS US Virginia

ping 3body.makingga.me

64 bytes ICMP ttl=128 time=248.964 ms

### DataCenter <--> DataCenter

阿里云上海 --> AWS US Virginia

64 bytes ICMP ttl=128 time=219.957 ms

AWS US Virginia --> 阿里云上海

64 bytes ICMP ttl=128 time=270.232 ms





