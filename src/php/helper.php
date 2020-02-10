<?
function currentTimeMillis()
{
    return intval(round(microtime(true) * 1000));
}

function localIdentifier()
{
    $hostname = php_uname('n');
    $hash = crc32(gethostbyname($hostname));
    return $hash & 0xFFF;
}

function getClientIp()
{
    $ip = "";
    if (!empty($_SERVER['HTTP_CLIENT_IP'])) {
        $ip = $_SERVER['HTTP_CLIENT_IP'];
    } elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
        $ips = explode(", ", $_SERVER['HTTP_X_FORWARDED_FOR']);
        if (!empty($ips)) {
            $ip = $ips[0]; // newest at first
        }
    }
    if (empty($ip)) {
        $ip = $_SERVER['REMOTE_ADDR'];
    }
    return $ip;
}


// HTTP POST请求
function httpGet($url, $header = [])
{
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
    if (count($header) > 0)
    {
        curl_setopt($ch, CURLOPT_HTTPHEADER  , $header);
    }
    curl_setopt($ch,CURLOPT_TIMEOUT,30);
    $data = curl_exec($ch);
    $err = null;
    if (curl_error($ch)) {
        $err = curl_error($ch);
    }
    $status_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    curl_close ($ch);
    return ["code"=>$status_code, "data"=>$data, "err"=>$err];
}

// HTTP POST请求
function httpPost($url, $header = [], $body = '')
{
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
    if (count($header) > 0)
    {
        curl_setopt($ch, CURLOPT_HTTPHEADER  , $header);
    }
    curl_setopt($ch, CURLOPT_POST, 1);
    curl_setopt($ch, CURLOPT_POSTFIELDS, $body);
    curl_setopt($ch,CURLOPT_TIMEOUT,30);
    $data = curl_exec($ch);
    $err = null;
    if (curl_error($ch)) {
        $err = curl_error($ch);
    }
    $status_code = curl_getinfo($ch, CURLINFO_HTTP_CODE);
    curl_close ($ch);
    return ["code"=>$status_code, "data"=>$data, "err"=>$err];
}

// HTTP POST with JSON
function httpJsonPost($url, $fields = [])
{
    //trace('HTTP request: ' . json_encode($fields, JSON_UNESCAPED_UNICODE ));
    $resp = self::httpPost($url, ["Content-Type: application/json"], http_build_query($fields));
    //trace('HTTP response' . json_encode($response,JSON_UNESCAPED_UNICODE ));
    if (!empty($resp["err"]) || $resp["code"] != 200)
    {
        $err = $resp["err"] ? $resp["err"] : ("HTTP " . $resp["code"]);
        return ["error"=> $err];
    }
    $data = json_decode($resp["data"], true);
    if (isset($data["code"])) {
        if (intval($data["code"]) != 200) {
            $message = sprintf("HTTP API返回错误：code=%s, msg=%s", strval($data["code"]), $data["message"]);
            return ["error"=>$message];
        }
    }
    return ["data"=>$data];
}
