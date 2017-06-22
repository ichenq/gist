package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"strings"
	"time"
)

type LookupBaseInfo struct {
	Country  string `json:"country"`
	City     string `json:"city"`
	County   string `json:"county"`
	ISP      string `json:"isp"`
	Province string `json:"province"`
}

type LookupNetInfo struct {
	IsNTP   int `json:"is_ntp"`
	NTPPort int `json:"Ntp_port"`
	IsDNS   int `json:"Is_dns"`
	DNSPort int `json:"Dns_port"`
	IsProxy int `json:"Proxy_port"`
	IsVPN   int `json:"Is_vpn"`
	VPNPort int `json:"Vpn_port"`
}

type IPLookupResult struct {
	Status      int            `json:"Status"`
	Description string         `json:"Description"`
	BaseInfo    LookupBaseInfo `json:"Base_info"`
	NetInfo     LookupNetInfo  `json:"Net_info"`
}

//IP地址归属, http://apistore.baidu.com/apiworks/servicedetail/2367.html
func GetIPLocation(ip string) string {
	if strings.HasPrefix(ip, "127.0.0") || strings.HasPrefix(ip, "::1") {
		return "Loopback"
	}
	if strings.HasPrefix(ip, "192.168") {
		return "LAN"
	}
	url := `http://apis.baidu.com/bdyunfenxi/intelligence/ip?ip=` + ip
	client := &http.Client{
		Timeout: 15 * time.Second,
	}
	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		fmt.Println("NewRequest:", err)
		return "Unknown"
	}
	req.Header.Add("apikey", "7b073ccb34968e5c846801b37c8d9465")
	resp, err := client.Do(req)
	if err != nil {
		fmt.Println("client.Do:", err)
		return "Unknown"
	}
	defer resp.Body.Close()
	body, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println("ioutil.ReadAll:", err)
		return "Unknown"
	}
	fmt.Printf("%s\n", body)
	var res IPLookupResult
	if err := json.Unmarshal(body, &res); err != nil {
		fmt.Println("json.Unmarshal:", err)
		return "Unknown"
	}
	if res.Status != 0 {
		fmt.Println("error", res.Status)
		return "Unknown"
	}
	return fmt.Sprintf("%v", res.BaseInfo)
}

func main() {
	ip := "171.213.60.165"
	info := GetIPLocation(ip)
	fmt.Println(info)
}
