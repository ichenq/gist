package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"strings"
	"time"
)

type IPLookupResult struct {
	ErrNum  int        `json:"errNum"`
	ErrMsg  string     `json:"errMsg"`
	RetData IPLocation `json:"retData"`
}

type IPLocation struct {
	Ip       string `json:"ip"`       //IP地址
	Country  string `json:"country"`  //国家
	Province string `json:"province"` //省份
	City     string `json:"city"`     //城市
	District string `json:"district"` //地区
	Carrier  string `json:"carrier"`  //运营商
}

//IP地址归属, http://apistore.baidu.com/apiworks/servicedetail/114.html
func GetIPLocation(ip string) string {
	if strings.HasPrefix(ip, "127.0.0") || strings.HasPrefix(ip, "::1") {
		return "Loopback"
	}
	if strings.HasPrefix(ip, "192.168") {
		return "LAN"
	}
	url := `http://apis.baidu.com/apistore/iplookupservice/iplookup?ip=` + ip
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
	println(string(body))
	var res IPLookupResult
	if err := json.Unmarshal(body, &res); err != nil {
		fmt.Println("json.Unmarshal:", err)
		return "Unknown"
	}
	if res.ErrNum != 0 {
		fmt.Println(res.ErrNum, res.ErrMsg)
		return "Unknown"
	}
	location := res.RetData
	fmt.Println(location)
	return fmt.Sprintf("%s %s %s %s", location.Carrier, location.Province, location.City, location.District)
}

func main() {
	ip := "171.213.60.165"
	fmt.Println(GetIPLocation(ip))
}
