<?php

require_once './vendor/aliyun-oss/autoload.php'; // https://github.com/aliyun/aliyun-oss-php-sdk

use OSS\OssClient;
use OSS\Core\OssException;

define('MAX_FILE_SIZE', 4*1024*1024);	// 不超过4M


//外部访问的URL前缀
$ExposeUrl = 'http://img.cdn.mydomain.com';
$AliOssUrl = 'http://img.cdn.mydomain.com';

// 保存上传文件目录
$UploadPrefixDir = 'images/';

// 是否上传到OSS
$IsUploadOss = false;

// OSS配置项
$AliOssCfg = [
	'accessKeyId'=> (''),
	'accessKeySecret'=> (''),
	'endpoint'=> 'http://oss-cn-shenzhen.aliyuncs.com',
	'bucket'=> 'images'
];

//上传到OSS
function uploadAliOSS($file, $resp) {
    global $AliOssUrl, $UploadPrefixDir, $AliOssCfg;
    $object = makeFileName($UploadPrefixDir, $file);
    try {
        $ossClient = new OssClient($AliOssCfg['accessKeyId'], $AliOssCfg['accessKeySecret'], $AliOssCfg['endpoint']);
        $result = $ossClient->uploadFile($AliOssCfg['bucket'], $object, $file['tmp_name']);
        $resp['code'] = 0;
        //$resp['what'] = $result;
        $resp['data']['src'] = sprintf('%s/%s', $AliOssUrl, $object);
    } catch (OssException $e) {
        $resp['code'] = 101;
        $resp['msg'] = $e->getMessage();
    }
    return $resp;
}

//保存为本地文件
function saveLocalFile($file, $resp) {
    global $UploadPrefixDir, $ExposeUrl;
    $filename = makeFileName($UploadPrefixDir, $file);
    $filepath = dirname($filename);
    if (!is_dir($filepath)) {
        mkdir($filepath, 0755, true);
    }
    //$resp['tmp_name'] = $file['tmp_name'];
    if (!move_uploaded_file($file['tmp_name'], $filename)) {
        $resp['code'] = 6;
        $resp['msg'] = "无法保存文件";
        $resp['what'] = $filename;
    } else {
        $resp['code'] = 0;
        $resp['msg'] = 'Succeed';
        $resp['data']['src'] = sprintf('%s/%s', $ExposeUrl, $filename);
    }
    return $resp;
}

// 生成文件名, $rootdir以'/'结尾
function makeFileName($rootdir, $file) {
    $extention = pathinfo($file['name'], PATHINFO_EXTENSION);
    $name = md5_file($file['tmp_name']);    //保证相同内容只有一份，但会增加I/O时间
    //$name = md5(uniqid('', true));        //每次上传同样的文件都会新存储一份
    $index_dir = substr($name, 0, 2);
    return sprintf('%s%s/%s.%s', $rootdir, $index_dir, $name, $extention);
}


function checkUploadFile($input_name) {
    $resp = ['code'=>0, 'msg'=>'', 'data'=>[]];
	if(!isset($_FILES[$input_name])) {
		$resp['code'] = 1;
		$resp['msg'] = '参数错误';
		return $resp;
	}
    $file = $_FILES[$input_name];
	if($file['error'])
	{
		$resp['code'] = 2;
		$resp['msg'] = $file['error']; 
		return $resp;
	}	
	// 判断上传文件类型
	$extensions = ['png', 'jpg', 'bmp', 'ico'];
	$file_ext = pathinfo($file['name'], PATHINFO_EXTENSION);
	if (!in_array(strtolower($file_ext), $extensions)) {
		$resp['code'] = 3;
		$resp['msg'] = '文件类型不匹配';
		$resp['what'] = $file_ext;
		unlink($file['tmp_name']);
		return $resp;
	}	
	if ($file['size'] > MAX_FILE_SIZE) {
		$resp['code'] = 4;
		$resp['msg'] = '文件大小限制';
		$resp['what'] = $file['size'];
		unlink($file['tmp_name']);
		return $resp;
	}
	return $resp;
}


header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: POST');
header('Content-Type: application/json');

$input_key = 'file';
$resp = checkUploadFile($input_key);
if ($resp['code'] == 0) {
    $file = $_FILES[$input_key];
    if ($IsUploadOss) {
        $resp = uploadAliOSS($file, $resp);
        unlink($file['tmp_name']);
    } else {
        $resp = saveLocalFile($file, $resp);
    }
}

echo json_encode($resp);
