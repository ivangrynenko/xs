<?php

//////////////////////////////////////////////////////////////
//===========================================================
// inc.php
//===========================================================
// SOFTACULOUS 
// Version : 1.1
// Inspired by the DESIRE to be the BEST OF ALL
// ----------------------------------------------------------
// Started by: Alons
// Date:       10th Jan 2009
// Time:       21:00 hrs
// Site:       http://www.softaculous.com/ (SOFTACULOUS)
// ----------------------------------------------------------
// Please Read the Terms of use at http://www.softaculous.com
// ----------------------------------------------------------
//===========================================================
// (c)Softaculous Inc.
//===========================================================
//////////////////////////////////////////////////////////////

global $virtualizor_conf;

// Common Functions

if(!function_exists('v_fn')){

function v_fn($f){
	global $virtualizor_conf;
	
	if(empty($virtualizor_conf['fields'][$f])){
		$r = $f;
	}else{
		$r = $virtualizor_conf['fields'][$f];
	}
	
	return $r;	
}

}

// The following function is a variation of v_fn() to support virtualizor_cloud_account as is uses another config variable $virtcloud_acc
if(!function_exists('vc_fn')){

function vc_fn($f){
	global $virtcloud_acc;
	
	if(empty($virtcloud_acc['fields'][$f])){
		$r = $f;
	}else{
		$r = $virtcloud_acc['fields'][$f];
	}
	
	return $r;	
}

}


if(!function_exists('make_apikey')){

function make_apikey($key, $pass){
	return $key.md5($pass.$key);
}

}



if(!function_exists('_unserialize')){
	
function _unserialize($str){

	$var = @unserialize($str);
	
	if(empty($var)){
	
		$str = preg_replace('!s:(\d+):"(.*?)";!se', "'s:'.strlen('$2').':\"$2\";'", $str);
		
		$var = @unserialize($str);
	
	}
	
	//If it is still empty false
	if(empty($var)){
	
		return false;
	
	}else{
	
		return $var;
	
	}

}

}


if(!function_exists('generateRandStr')){
//generates random strings
function generateRandStr($length){	
	$randstr = "";	
	for($i = 0; $i < $length; $i++){	
		$randnum = mt_rand(0,61);		
		if($randnum < 10){		
			$randstr .= chr($randnum+48);			
		}elseif($randnum < 36){		
			$randstr .= chr($randnum+55);			
		}else{		
			$randstr .= chr($randnum+61);			
		}		
	}	
	return strtolower($randstr);	
}

}


if(!function_exists('valid_ipv6')){

function valid_ipv6($ip){

	$pattern = '/^((([0-9A-Fa-f]{1,4}:){7}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){6}:[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){5}:([0-9A-Fa-f]{1,4}:)?[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){4}:([0-9A-Fa-f]{1,4}:){0,2}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){3}:([0-9A-Fa-f]{1,4}:){0,3}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){2}:([0-9A-Fa-f]{1,4}:){0,4}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){6}((\b((25[0-5])|(1\d{2})|(2[0-4]\d)|(\d{1,2}))\b)\.){3}(\b((25[0-5])|(1\d{2})|(2[0-4]\d)|(\d{1,2}))\b))|(([0-9A-Fa-f]{1,4}:){0,5}:((\b((25[0-5])|(1\d{2})|(2[0-4]\d)|(\d{1,2}))\b)\.){3}(\b((25[0-5])|(1\d{2})|(2[0-4]\d)|(\d{1,2}))\b))|(::([0-9A-Fa-f]{1,4}:){0,5}((\b((25[0-5])|(1\d{2})|(2[0-4]\d)|(\d{1,2}))\b)\.){3}(\b((25[0-5])|(1\d{2})|(2[0-4]\d)|(\d{1,2}))\b))|([0-9A-Fa-f]{1,4}::([0-9A-Fa-f]{1,4}:){0,5}[0-9A-Fa-f]{1,4})|(::([0-9A-Fa-f]{1,4}:){0,6}[0-9A-Fa-f]{1,4})|(([0-9A-Fa-f]{1,4}:){1,7}:))$/';
	
	if(!preg_match($pattern, $ip)){
		return false;	
	}
	
	return true;
	
}

}

if(!function_exists('cexplode')){

// Clean explode a string
function cexplode($chars, $str, $int = 0){
	
	$r = explode($chars, $str);
	
	foreach($r as $k => $v){
		if($int){
			$r[$k] = (int) trim($v);	
		}else{
			$r[$k] = trim($v);
		}
	}
	
	return $r;
	
}

}


?>