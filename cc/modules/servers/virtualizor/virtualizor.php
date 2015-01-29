<?php

// Disable warning messages - in PHP 5.4
error_reporting(E_ALL & ~E_STRICT & ~E_NOTICE);

include_once('virtualizor_conf.php');
include_once('functions.php');

function virtualizor_ConfigOptions() {
	
	# Should return an array of the module options for each product - Minimum of 24
    $configarray = array(
	 "Type" => array( "Type" => "dropdown", "Options" => "OpenVZ,Xen PV,Xen HVM,KVM,XCP HVM,XCP PV"),
	 "DiskSpace" => array( "Type" => "text", "Size" => "25", "Description" => "GB"),
	 "Inodes" => array( "Type" => "text", "Size" => "25", "Description" => " (OpenVZ)"),
	 "Guaranteed RAM" => array( "Type" => "text", "Size" => "25", "Description" => "MB"),
	 "Burstable RAM" => array( "Type" => "text", "Size" => "25", "Description" => "MB (OpenVZ)"), 
	 "SWAP RAM" => array( "Type" => "text", "Size" => "25", "Description" => "MB (Xen, XCP and KVM)"), 
	 "Bandwidth" => array( "Type" => "text", "Size" => "25", "Description" => "GB (Zero or empty for unlimited)"),
	 "CPU Units" => array ( "Type" => "text", "Size" => "25", "Description" => "Units"), 
	 "CPU Cores" => array( "Type" => "text", "Size" => "25", "Description" => ""),
	 "CPU%" => array( "Type" => "text", "Size" => "25", "Description" => ""),
	 "I/O Priority" => array( "Type" => "dropdown", "Options" => "0,1,2,3,4,5,6,7", "Description" => "(OpenVZ)"),
	 "VNC" => array( "Type" => "yesno", "Description" => "Enable VNC (Xen, XCP and KVM)" ),
	 "IPs" => array( "Type" => "text", "Size" => "25", "Description" => "Number of IPs"),
	 "Network Speed" => array( "Type" => "text", "Size" => "25", "Description" => "KB/s (Zero or empty for unlimited)"),
	 "Server" => array( "Type" => "text", "Size" => "25", "Description" => "Slave Servers name if any"),
	 "Server Group" => array( "Type" => "text", "Size" => "25", "Description" => "To choose a server"),
	 "IPv6" => array( "Type" => "text", "Size" => "25", "Description" => "Number of IPv6 Address"),
	 "IPv6 Subnets" => array( "Type" => "text", "Size" => "25", "Description" => "Number of IPv6 Subnets"),
	);
	
	return $configarray;
}

function virtualizor_CreateAccount($params) {

	global $virtualizor_conf;

    # ** The variables listed below are passed into all module functions **
	
	$loglevel = (int) @$_REQUEST['loglevel'];
	
	if(!empty($virtualizor_conf['loglevel'])){
		$loglevel = $virtualizor_conf['loglevel'];
	}
	
    $serviceid = $params["serviceid"]; # Unique ID of the product/service in the WHMCS Database
    $pid = $params["pid"]; # Product/Service ID
    $producttype = $params["producttype"]; # Product Type: hostingaccount, reselleraccount, server or other
    $domain = $params["domain"];
	$username = $params["username"];
	$password = $params["password"];
    $clientsdetails = $params["clientsdetails"]; # Array of clients details - firstname, lastname, email, country, etc...
    $customfields = $params["customfields"]; # Array of custom field values for the product
    $configoptions = $params["configoptions"]; # Array of configurable option values for the product
	
	if(!empty($params["customfields"]['vpsid'])){
		return 'The VPS exists';
	}

    # Additional variables if the product/service is linked to a server
    $server = $params["server"]; # True if linked to a server
    $serverid = $params["serverid"];
    $serverip = $params["serverip"];
    $serverusername = $params["serverusername"];
    $serverpassword = $params["serverpassword"];
    $serveraccesshash = $params["serveraccesshash"];
    $serversecure = $params["serversecure"]; # If set, SSL Mode is enabled in the server config
	$virttype = (preg_match('/xen/is', $params['configoption1']) ? 'xen' : (preg_match('/xcp/is', $params['configoption1']) ? 'xcp' : strtolower($params['configoption1'])));
	$hvm = (preg_match('/hvm/is', $params['configoption1']) ? 1 : 0);
	$numips = (empty($params['configoptions'][v_fn('ips')]) || $params['configoptions'][v_fn('ips')] == 0 ? $params['configoption13'] : $params['configoptions'][v_fn('ips')]);
	$numips6 = (empty($params['configoptions'][v_fn('ips6')]) || $params['configoptions'][v_fn('ips6')] == 0 ? $params['configoption17'] : $params['configoptions'][v_fn('ips6')]);
	$numips6_subnet = (empty($params['configoptions'][v_fn('ips6_subnet')]) || $params['configoptions'][v_fn('ips6_subnet')] == 0 ? $params['configoption18'] : $params['configoptions'][v_fn('ips6_subnet')]);
	$ctrlpanel = (empty($params['configoptions'][v_fn('ctrlpanel')]) ? -1 : strtolower(trim($params['configoptions'][v_fn('ctrlpanel')])));
	if($loglevel > 0) logActivity('VIRT : '.$virttype.' - '.$hvm);
	if($loglevel > 0) logActivity(var_export($params, 1));
	
	// Get the Data
	$data = Virtualizor_Curl::make_api_call($params["serverip"], $params["serverpassword"], 'index.php?act=addvs');
			
	if(empty($data)){
		return 'Could not load the server data.'.Virtualizor_Curl::error($params["serverip"]);
	}
	
// echo "<pre>";print_r($params);echo "</pre>";
// echo "<pre>";print_r($data);echo "</pre>";

	$cookies = array();
	
	$slave_server = (empty($params['configoptions'][v_fn('slave_server')]) ? $params['configoption15'] : $params['configoptions'][v_fn('slave_server')]);
	$server_group = (empty($params['configoptions'][v_fn('server_group')]) ? $params['configoption16'] : $params['configoptions'][v_fn('server_group')]);
	
	// Is there a Slave server ?
	if(!empty($slave_server) && $slave_server != 'localhost'){
		
		// Do we have to Auto Select
		if($slave_server == 'auto'){
			
			foreach($data['servers'] as $k => $v){
				
				// Master servers cannot be here
				if(empty($k)) continue;
				
				// Only the Same type of Virtualization is supported
				if($virttype != $v['virt']){
					continue;
				}
				
				// Xen HVM additional check
				if(!empty($hvm) && empty($v['hvm'])){
					continue;
				}
				
				$tmpsort[$k] = $v['numvps'];
				
			}
			
			// Did we get a list of Slave Servers
			if(empty($tmpsort)){
				return 'No server present in the Cluster which is of the Virtualization Type : '.$params['configoption1'];
			}
			
			asort($tmpsort);
			
			$newserid = key($tmpsort);
			//return 'Tests'.$newserid.var_export($tmpsort, 1);
			
		}else{
		
			foreach($data['servers'] as $k => $v){
				if(trim(strtolower($v['server_name'])) == trim(strtolower($slave_server))){
					$newserid = $k;
				}
			}
		
		}
		
		// Is there a valid slave server ?
		if(empty($newserid)){
			return 'There is no slave server - '.$slave_server.'. Please correct the <b>Product / Service</b> with the right slave server name.';
		}
	
		if($loglevel > 1) logActivity('Slave Server : '.$newserid);
	
	// Is there a Server Group ?
	}elseif(!empty($server_group)){
		
		foreach($data['servergroups'] as $k => $v){
			
			// Match the Server Group
			if(trim(strtolower($v['sg_name'])) == trim(strtolower($server_group))){					
				$sgid = $k;					
			}
			
		}
	
		// OH SHIT ! We didnt find anything 
		if(!isset($sgid)){
			return 'Could not find the server group - '.$server_group.'. Please correct the <b>Product / Service</b> with the right slave server name.';
		}
		
		// Make an array of available servers in this group
		foreach($data['servers'] as $k => $v){
			
			// Do you belong to this group
			if($v['sgid'] != $sgid){
				continue;
			}
			
			// Only the Same type of Virtualization is supported
			if($virttype != $v['virt']){
				continue;
			}
			
			// Xen HVM additional check
			if(!empty($hvm) && empty($v['hvm'])){
				continue;
			}
			
			$tmpsort[$k] = $v['numvps'];
			
		}
		
		asort($tmpsort);
		
		// Is there a valid slave server ?
		if(empty($tmpsort)){
			return 'No server present in the Server Group which is of the Virtualization Type : '.$params['configoption1'].'. Please correct the <b>Product / Service</b> with the right slave server name.';
		}
		
		$newserid = key($tmpsort);
		
		if($loglevel > 1) logActivity('Slave Group Server Chosen : '.$newserid);
	
	}
	
	// If a new server ID was found. Even if its 0 (Zero) then there is no need to reload data as the DATA is by default of 0
	if(!empty($newserid)){
		
		$cookies[$data['globals']['cookie_name'].'_server'] = $newserid;
		
		$data = Virtualizor_Curl::make_api_call($params["serverip"], $params["serverpassword"], 'index.php?act=addvs', array(), array(), $cookies);
		
		if(empty($data)){
			return 'Could not load the slave server data';
		}
	
	}
	
	if($loglevel > 2) logActivity(var_export($data, 1));
	
	$post = array();
	
	// Search does the user exist
	foreach($data['users'] as $k => $v){
		if(strtolower($v['email']) == strtolower($params["clientsdetails"]['email'])){
			$post['uid'] = $v['uid'];
		}
	}
	
	// Was the user there ?
	if(empty($post['uid'])){
		$post['user_email'] = $params["clientsdetails"]['email'];
		$post['user_pass'] = $params["password"];
	}
	
	// Get the OS from the fields set
	$OS = strtolower(trim($params['configoptions'][v_fn('OS')]));
	if(empty($OS)){
		$OS = strtolower(trim($params["customfields"]['OS']));		
	}
	
	// Search the OS ID
	if($OS != 'none'){
	
		foreach($data['oslist'][$virttype] as $k => $v){
			foreach($v as $kk => $vv){
				
				// Xen/XCP Stuff!
				if($virttype == 'xen' || $virttype == 'xcp'){
				
					// Xen/XCP HVM templates
					if(!empty($hvm) && empty($vv['hvm'])){
						continue;
						
					// Xen/XCP PV templates
					}elseif(empty($hvm) && !empty($vv['hvm'])){
						continue;
					}
				}
				
				// Does the String match ?
				if(strtolower($vv['name']) == $OS){
					$post['osid'] = $kk;
				}
				
			}
		}
	
	}
	
	// Is the OS template there
	if(empty($post['osid']) && $OS != 'none'){
		return 'Could not find the OS Template '.$OS;
	}
	
	// Search the ISO
	if(!empty($params["customfields"]['iso']) && strtolower($params["customfields"]['iso']) != 'none'){
		
		// ISO restricted in OVZ and XEN-PV
		if($virttype == 'openvz' || (($virttype == 'xen' || $virttype == 'xcp') && empty($hvm))){
			return 'You can not select ISO for OpenVZ, XEN-PV and XCP-PV VPS';
		}
	
		foreach($data['isos'] as $k => $v){
		
			foreach($v as $kk => $vv){
				
				//echo $vv['name'].' - '.$params["customfields"]['iso'].'<br>';
				
				// Does the String match ?
				if(strtolower($vv) == strtolower(trim($params["customfields"]['iso']))){
					$post['iso'] = $vv;
				}
			}
		}
		
		// Is the ISO there
		if(empty($post['iso'])){
			return 'Could not find the ISO '.$params["customfields"]['iso'];
		}
	}
	
	// If ISO and OS both not selected ?
	if(empty($post['iso']) && empty($post['osid']) && strtolower($params["customfields"]['iso']) == 'none' && $OS == 'none'){
		return 'ISO or OS is not selected';
	}
	
	// No emails
	if(!empty($params["customfields"]['noemail'])){
		$post['noemail'] = 1;
	}
	
	// Are there any IPv4 to assign ?
	if($numips > 0){
	
		// Assign the IPs
		foreach($data['ips'] as $k => $v){
			$i = $numips;
			$_ips[] = $v['ip'];
			
			if($i == count($_ips)){
				break;
			}
		}
		
		// Were there enough IPs
		if(empty($_ips) || count($_ips) < $numips){
			return 'There are insufficient IPs on the server';
		}
	
	}
	
	// Are there any IPv6 to assign ?
	if($numips6 > 0){
		
		$_ips6 = array();
		
		// Assign the IPs
		foreach($data['ips6'] as $k => $v){
			
			if($numips6 == count($_ips6)){
				break;
			}
			
			$_ips6[] = $v['ip'];
		}
		
		// Were there enough IPs
		if(empty($_ips6) || count($_ips6) < $numips6){
			return 'There are insufficient IPv6 Addresses on the server';
		}
	
	}
	
	// Are there any IPv6 Subnets to assign ?
	if($numips6_subnet > 0){
		
		$_ips6_subnet = array();
		
		// Assign the IPs
		foreach($data['ips6_subnet'] as $k => $v){
			
			if($numips6_subnet == count($_ips6_subnet)){
				break;
			}
			
			$_ips6_subnet[] = $v['ip'];
		}
		
		// Were there enough IPs
		if(empty($_ips6_subnet) || count($_ips6_subnet) < $numips6_subnet){
			return 'There are insufficient IPv6 Subnets on the server';
		}
	
	}
	
	$post['hostname'] = $params['domain'];
	$post['rootpass'] = $params['password'];
	$post['space'] = (empty($params['configoptions'][v_fn('space')]) || $params['configoptions'][v_fn('space')] == 0 ? $params['configoption2'] : $params['configoptions'][v_fn('space')]);
	$post['ram'] = (empty($params['configoptions'][v_fn('ram')]) || $params['configoptions'][v_fn('ram')] == 0 ? $params['configoption4'] : $params['configoptions'][v_fn('ram')]);
	$post['bandwidth'] = (empty($params['configoptions'][v_fn('bandwidth')]) || $params['configoptions'][v_fn('bandwidth')] == 0 ? (empty($params['configoption7']) ? '0' : $params['configoption7']) : $params['configoptions'][v_fn('bandwidth')]);
	$post['cores'] = (empty($params['configoptions'][v_fn('cores')]) || $params['configoptions'][v_fn('cores')] == 0 ? $params['configoption9'] : $params['configoptions'][v_fn('cores')]);
	$post['network_speed'] = (empty($params['configoptions'][v_fn('network_speed')]) || $params['configoptions'][v_fn('network_speed')] == 0 ? $params['configoption14'] : $params['configoptions'][v_fn('network_speed')]);
	$post['cpu_percent'] = (empty($params['configoptions'][v_fn('cpu_percent')]) || $params['configoptions'][v_fn('cpu_percent')] == 0 ? $params['configoption10'] : $params['configoptions'][v_fn('cpu_percent')]);
	$post['cpu'] = $params['configoption8'];
	$post['addvps'] = 1;
	$post['band_suspend'] = 1;
	
	
	// Is is OpenVZ
	if($virttype == 'openvz'){
	
		$post['inodes'] = $params['configoption3'];
		$post['burst'] = $params['configoption5'];
		$post['priority'] = $params['configoption11'];
		
	// Is it Xen PV?
	}elseif(($virttype == 'xen' || $virttype == 'xcp') && empty($hvm)){
		
		$post['swapram'] = $params['configoption6'];
		if($params['configoption12'] == 'yes' || $params['configoption12'] == 'on'){
			$post['vnc'] = 1;
			$post['vncpass'] = generateRandStr(8);
		}
		
	// Is it Xen HVM?
	}elseif(($virttype == 'xen' || $virttype == 'xcp') && !empty($hvm)){
		
		$post['hvm'] = 1;
		$post['shadow'] = 8;
		$post['swapram'] = $params['configoption6'];
		if($params['configoption12'] == 'yes' || $params['configoption12'] == 'on'){
			$post['vnc'] = 1;
			$post['vncpass'] = generateRandStr(8);
		}
		
	// Is it KVM ?
	}elseif($virttype == 'kvm'){
	
		$post['swapram'] = $params['configoption6'];
		if($params['configoption12'] == 'yes' || $params['configoption12'] == 'on'){
			$post['vnc'] = 1;
			$post['vncpass'] = generateRandStr(8);
		}
		
	}
	
	// Suspend on bandwidth
	//$post['band_suspend'] = 1;
	
	// Add the IPs
	if(!empty($_ips)){
		$post['ips'] = $_ips;
	}
	
	// Add the IPv6
	if(!empty($_ips6)){
		$post['ipv6'] = $_ips6;
	}
	
	// Add the IPv6 Subnet
	if(!empty($_ips6_subnet)){
		$post['ipv6_subnet'] = $_ips6_subnet;
	}
	
	// Are there any configurable options
	if(!empty($params['configoptions'])){
		foreach($params['configoptions'] as $k => $v){
			if(!isset($post[$k])){
				$post[$k] = $v;
			}
		}
	}
	
	if($loglevel > 0) logActivity('POST : '.var_export($post, 1));
	
 //echo "<pre>";print_r($cookies);echo "</pre>";
 //echo "<pre>";print_r($post);echo "</pre>";
// return 'TEST'.var_export($params, 1);
	
	// Setup cPanel licenses if cPanel configurable option is set
	if($ctrlpanel != -1 && $ctrlpanel != 'none'){
	
		if($ctrlpanel == 'cpanel' && !empty($virtualizor_conf['cp']['buy_cpanel_login']) && !empty($virtualizor_conf['cp']['buy_cpanel_apikey'])){
			logActivity("CPANEL : cPanel issued for ip $_ips[0] of ordertype $cpanel");
			
			$url = 'https://www.buycpanel.com/api/order.php?';
			$login = 'login='.$virtualizor_conf['cp']['buy_cpanel_login'].'&';
			$key = 'key='.$virtualizor_conf['cp']['buy_cpanel_apikey'].'&';
			$domain = 'domain='.$params['domain'].'&';
			$serverip = 'serverip='.$_ips[0].'&';
			$ordertype = 'ordertype=10';
			
			$url .= $login.$key.$domain.$serverip.$ordertype;
			
			$ret = file_get_contents($url);
			
			$ret = json_decode($ret);
			
			if($ret->success == 0){
				return 'Errors : cPanel Licensing : '.$ret->faultstring;
			}
		}
	}
	
	$ret = Virtualizor_Curl::make_api_call($params["serverip"], $params["serverpassword"], 'index.php?act=addvs', array(), $post, $cookies);
	
// echo "<pre>";print_r($ret);echo "</pre>";
	
	// Was the VPS Inserted
	if(!empty($ret['newvs']['vpsid'])){
		
		// vpsid of virtualizor
		$query = mysql_query("SELECT `id` FROM `tblcustomfields` WHERE `relid` = '$pid' AND `fieldname` = 'vpsid'");
		$res = mysql_fetch_array($query);
		
		mysql_query("UPDATE `tblcustomfieldsvalues` SET `value` = '".$ret['newvs']['vpsid']."' WHERE `relid` = '$serviceid' AND `fieldid` = '$res[id]'") or mysql_error();
		
		// Change the Username to the email
		mysql_query("UPDATE `tblhosting` SET `username` = '".$params['clientsdetails']['email']."' WHERE `id` = '$serviceid';");

		// The Dedicated IP
		mysql_query("UPDATE `tblhosting` SET `dedicatedip` = '".(!empty($_ips[0]) ? $_ips[0] : $_ips6[0])."' WHERE `id` = '$serviceid'");
		
		$tmp_ips = empty($_ips) ? array() : $_ips;
		
		if(!empty($_ips6_subnet)){
			foreach($_ips6_subnet as $k => $v){
				$tmp_ips[] = $v;
			}
		}
		
		if(!empty($_ips6)){
			foreach($_ips6 as $k => $v){
				$tmp_ips[] = $v;
			}
		}
		
		// Extra IPs
		if(count($tmp_ips) > 1){
			unset($tmp_ips[0]);
			mysql_query("UPDATE `tblhosting` SET `assignedips` = '".implode("\n", $tmp_ips)."' WHERE `id` = '$serviceid'");
		}
		
		// Did it start ?
		if(!empty($ret['done'])){		
			return 'success';	
		}else{
			return 'Errors : '.implode('<br>', $ret['error']);
		}
		
	} else {
		return 'Errors : '.implode('<br>', $ret['error']);
	}
	
}

function virtualizor_AdminServicesTabFields($params) {
	
	$vps_manage = Virtualizor_Curl::action($params, 'act=vpsmanage&');
	
	if($vps_manage['info']['status'] == 0){
		$fieldsarray = array(
		 'VPS Information' => '<div style="color:#FF0000;font-size:20px">VPS is Offline</div>',
		);
		return $fieldsarray;
	}
	
	$code = '
		<table cellpadding="8" cellspacing="1" border="0" width="100%">
			<tr class="oddrowcolor">
				<th>Hostname </th><td>'.$vps_manage['info']['hostname'].'</td>
			</tr>
			
			<tr class="oddrowcolor">
				<th>IP(s) </th><td>'.implode(', ', $vps_manage['info']['ip']).'</td>
			</tr>
		</table>
	';
	
	if(!preg_match('/openvz/is', $params['configoption1'])){
	
		$code .= '<img src="../modules/servers/virtualizor/images/vpsinfo.png">';
	
		$fieldsarray = array(
		 'VPS Information' => $code,
		);
		
		return $fieldsarray;
	}
	
	if(empty($vps_manage)){
		$fieldsarray = array(
		 'VPS Information' => 'Virtualizor Server is Offline',
		);
		return $fieldsarray;
	}
	
	$code .= '		 
		<script src="../modules/servers/virtualizor/js/jquery.flot.stack.js"></script>
		<script src="../modules/servers/virtualizor/js/jquery.flot.min.js"></script>
		<script src="../modules/servers/virtualizor/js/jquery.flot.pie.min.js"></script>
		
		<link rel="stylesheet" type="text/css" href="modules/servers/virtualizor/'.(!empty($virtualizor_conf['style']) ? $virtualizor_conf['style'] : 'style.css').'" />		
		
		<script type="text/javascript">

			$(document).ready(function(){
			
			
				function makedata(data){
				
					var fdata = [];
					i = 0;
					for (x in data){
						fdata.push([i, (data[x])]);
						i++;
					}
				
					return fdata;
					
				}
				
				var d1 = makedata([0, '.implode(', ', $vps_manage['info']['bandwidth']['usage']).']);';
				
				// Inbound and outbound data is currently not available in KVM and XEN
				if(!empty($vps_manage['info']['bandwidth']['in']) && !empty($vps_manage['info']['bandwidth']['out'])){
					
					$code .= '
					var indata = makedata([0, '.implode(', ', $vps_manage['info']['bandwidth']['in']).']);
					var outdata = makedata([0, '.implode(', ', $vps_manage['info']['bandwidth']['out']).']);';
				}
				
				
				$code .= '
				var bandwidth_graph = [
					{ label: "Usage",  data: d1},';
				
					if(!empty($vps_manage['info']['bandwidth']['in']) && !empty($vps_manage['info']['bandwidth']['out'])){
						
						$code .= '
						{ label: "In",  data: indata},
						{ label: "Out",  data: outdata}';
					}
					
				$code .= '
				];
				
				$.plot($("#bwband_holder"), bandwidth_graph, {
					series: {
						points: { show: true },
						lines: { show: true, fill: true, steps: false }
					},
					legend: {
						show: true
					},
					grid: { hoverable: true}
				});
				
				function showTooltip(x, y, contents) {
					$(\'<div id="tooltip">\' + contents + \'</div>\').css( {
						position: "absolute",
						display: "none",
						top: y ,
						left: x + 20,
						border: "1px solid #CCCCCC",
						padding: "2px",
						"background-color": "#EFEFEF",
						"z-index" : 10000,
						opacity: 0.80
					}).appendTo("body").fadeIn(200);
				}

				var previousPoint = null;
				$("#bwband_holder").bind("plothover", function (event, pos, item) {
					$("#x").text(pos.x.toFixed(2));
					$("#y").text(pos.y.toFixed(2));

					if (item) {
						
						if (previousPoint != item.dataIndex) {
							previousPoint = item.dataIndex;
							$("#tooltip").remove();
							var x = item.datapoint[0].toFixed(2),
								y = item.datapoint[1].toFixed(2);
								
							showTooltip(item.pageX, item.pageY,
										"Total : " + parseInt(y) + " MB <br>Day : " + parseInt(x));
						}
					} else {
						$("#tooltip").remove();
						previousPoint = null;
					}
				});
			
			
		
				$(function () {
					
					var ram_resource = [
						{ label: "Used",  data: '.$vps_manage['info']['ram']['used'].'},
						{ label: "Free",  data: '.$vps_manage['info']['ram']['limit'].'}
					];
					
					var space_resource = [
						{ label: "Used",  data: '.$vps_manage['info']['disk']['used'].'},
						{ label: "Free",  data: '.$vps_manage['info']['disk']['limit'].'}
					];
					var bandwidth_resource = [
						{ label: "Used",  data: '.$vps_manage['info']['bandwidth']['used'].'},
						{ label: "Free",  data: '.$vps_manage['info']['bandwidth']['limit'].'}
					];
					
					// Ram Resource Graph
					resource_graph("ram_resource", ram_resource);
					
					// Space Resource Graph
					resource_graph("space_resource", space_resource);
					
					// IPv4 Resource Graph
					resource_graph("bandwidth_resource", bandwidth_resource);
					
				});

				// Draw a Resource Graph
				function resource_graph(id, data){

					$.plot($("#"+id), data, 
					{
						series: {
							pie: { 
								innerRadius: 0.7,
								radius: 1,
								show: true,
								label: {
									show: true,
									radius: 0,
									formatter: function(label, series){
										if(label != "Used") return "";
										return \'<div style="font-size:18px;text-align:center;padding:2px;color:black;">\'+Math.round(series.percent)+\'%</div><div style="font-size:10px;">\'+label+\'</div>\';	
									}
								}
							}
						},
						legend: {
							show: false
						}
					});
				}
			
			
			});
			
		</script>

		<table cellpadding="8" cellspacing="1" border="0" width="100%" class="cat-title">
		<tr><td>
			<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
			<tr>
				<td colspan="3"><div class="roundheader">VPS Information</div></td>
			</tr>
			<tr>';
		
			if(preg_match('/openvz/is', $params['configoption1'])){
				$code .= '
					<td align="center" width="25%" valign="top">
						<b>RAM</b>
						<div id="ram_resource" class="resource_graph" style="height:80px;width:80px;"></div>
						<br />'.$vps_manage['info']['ram']['used'].' MB / '.$vps_manage['info']['ram']['limit'].' MB
					</td>
					<td align="center" width="25%" valign="top">
						<b>Space</b>
						<div id="space_resource" class="resource_graph" style="height:80px;width:80px;"></div>
						<br />'.$vps_manage['info']['disk']['used'].' MB / '.$vps_manage['info']['disk']['limit'].' MB
					</td>';
			}
			
			$code .= '
				<td align="center" width="25%" valign="top">
					<b>Bandwidth</b>
					<div id="bandwidth_resource" class="resource_graph" style="height:80px;width:80px;"></div>
					<br />'.$vps_manage['info']['bandwidth']['used'].' MB / '.$vps_manage['info']['bandwidth']['limit'].' MB
				</td>
			</tr>
			<tr class="oddrowcolor">
				<th align="right">Load : </th><td colspan="2">'.$vps_manage['info']['uptime'].'</td>
			</tr>
			
			</table>
		</td>
		<td>
			<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
			<tr>
				<td><div class="roundheader">Bandwidth Information</div></td>
			</tr>
			<tr>
				<td><center><div style="width:400px; height:190px;" id="bwband_holder"></div></center></td>
			</tr>
			</table>
		</td>
		</tr>
		</table>';
	
	//r_print($vps_manage);
    $fieldsarray = array(
     'VPS Information' => $code,
    );
    return $fieldsarray;

}


function virtualizor_TerminateAccount($params) {

	global $virtualizor_conf;
	
	$ctrlpanel = (empty($params['configoptions'][v_fn('ctrlpanel')]) ? -1 : $params['configoptions'][v_fn('ctrlpanel')]);
	
	if(!empty($virtualizor_conf['admin_ui']['disable_terminate'])){
		return 'Termination has been disabled by the Global Administrator';
	}

	// Setup cPanel licenses if cPanel configurable option is set
	if($ctrlpanel != -1 && $ctrlpanel != 'none'){
		
		if($ctrlpanel == 'cpanel' && !empty($virtualizor_conf['cp']['buy_cpanel_login']) && !empty($virtualizor_conf['cp']['buy_cpanel_apikey'])){
		
			$data = Virtualizor_Curl::make_api_call($params["serverip"], $params["serverpassword"], 'index.php?act=vs');

			$data = $data['vs'][$params['customfields']['vpsid']]['ips'];
		
			list($cpanel_ip_id, $cpanel_ip) = array_shift($data);
			
			logActivity("CPANEL : cPanel delete for ip $_ips[0]");
			
			$url = 'https://www.buycpanel.com/api/cancel.php?';
			$login = 'login='.$virtualizor_conf['cp']['buy_cpanel_login'].'&';
			$key = 'key='.$virtualizor_conf['cp']['buy_cpanel_apikey'].'&';
			$currentip = 'currentip='.$cpanel_ip.'&';
			$url .= $login.$key.$currentip;
			
			$ret = file_get_contents($url);
			
			$ret = json_decode($ret);
			
			if($ret->success == 0){
				return 'Errors : cPanel Licensing : '.$ret->faultstring;
			}
		}
	}

	$data = Virtualizor_Curl::make_api_call($params["serverip"], $params["serverpassword"], 'index.php?act=vs&delete='.$params['customfields']['vpsid']);
			
	if(empty($data)){
		return 'Could not load the server data.'.Virtualizor_Curl::error($params["serverip"]);
	}

// echo "<pre>";print_r($params);echo "</pre>";
// echo "<pre>";print_r($data);echo "</pre>";
	
	// If the VPS has been deleted
    if ($data['done']) {
	
		// vpsid of virtualizor
		$query = mysql_query("SELECT `id` FROM `tblcustomfields` WHERE `relid` = '".$params["pid"]."' AND `fieldname` = 'vpsid'");
		$res = mysql_fetch_array($query);
		mysql_query("UPDATE `tblcustomfieldsvalues` SET `value` = '' WHERE `relid` = '".$params["serviceid"]."' AND `fieldid` = '$res[id]'") or mysql_error();

		// The Dedicated IP
		mysql_query("UPDATE `tblhosting` SET `dedicatedip` = '' WHERE `id` = '".$params["serviceid"]."'");
		
		mysql_query("UPDATE `tblhosting` SET `assignedips` = '' WHERE `id` = '".$params["serviceid"]."'");
		
		$result = "success";
	} else {
		$result = "There was some error deleting the VPS";
	}
	
	return $result;
}

function virtualizor_SuspendAccount($params) {

	$data = Virtualizor_Curl::make_api_call($params["serverip"], $params["serverpassword"], 'index.php?act=vs&suspend='.$params['customfields']['vpsid']);
			
	if(empty($data)){
		return 'Could not load the server data.'.Virtualizor_Curl::error($params["serverip"]);
	}

// echo "<pre>";print_r($params);echo "</pre>";
// echo "<pre>";print_r($data);echo "</pre>";

    if ($data['done']) {
		$result = "success";
	} else {
		$result = "There was some error suspending the VPS";
	}
	return $result;
}

function virtualizor_UnsuspendAccount($params) {

	$data = Virtualizor_Curl::make_api_call($params["serverip"], $params["serverpassword"], 'index.php?act=vs&unsuspend='.$params['customfields']['vpsid']);
			
	if(empty($data)){
		return 'Could not load the server data.'.Virtualizor_Curl::error($params["serverip"]);
	}

// echo "<pre>";print_r($params);echo "</pre>";
// echo "<pre>";print_r($data);echo "</pre>";

    if ($data['done']) {
		$result = "success";
	} else {
		$result = "There was some error unsuspending the VPS";
	}
	return $result;
}

function virtualizor_ChangePassword($params) {

	# Code to perform action goes here...

    if ($successful) {
		$result = "success";
	} else {
		$result = "Error Message Goes Here...";
	}
	return $result;
}

function virtualizor_ChangePackage($params) {

	global $virtualizor_conf;
	
	$loglevel = (int) @$_REQUEST['loglevel'];
	
	if(!empty($virtualizor_conf['loglevel'])){
		$loglevel = $virtualizor_conf['loglevel'];
	}
	
	// Get the Data
	$data = Virtualizor_Curl::make_api_call($params["serverip"], $params["serverpassword"], 'index.php?act=editvs&vpsid='.$params['customfields']['vpsid']);
			
	if(empty($data)){
		return 'Could not load the server data.'.Virtualizor_Curl::error($params["serverip"]);
	}
	
	$post_vps = $data['vps'];	
	
	if($loglevel > 0) logActivity('Change Package Params : '.var_export($params, 1));
	if($loglevel > 0) logActivity('Orig VPS : '.var_export($post_vps, 1));
	
	// POST Variables
	$post_vps['space'] = (empty($params['configoptions'][v_fn('space')]) || $params['configoptions'][v_fn('space')] == 0 ? $params['configoption2'] : $params['configoptions'][v_fn('space')]);
	$post_vps['ram'] = (empty($params['configoptions'][v_fn('ram')]) || $params['configoptions'][v_fn('ram')] == 0 ? $params['configoption4'] : $params['configoptions'][v_fn('ram')]);
	$post_vps['bandwidth'] = (empty($params['configoptions'][v_fn('bandwidth')]) || $params['configoptions'][v_fn('bandwidth')] == 0 ? (empty($params['configoption7']) ? '0' : $params['configoption7']) : $params['configoptions'][v_fn('bandwidth')]);
	$post_vps['cores'] = (empty($params['configoptions'][v_fn('cores')]) || $params['configoptions'][v_fn('cores')] == 0 ? $params['configoption9'] : $params['configoptions'][v_fn('cores')]);
	$post_vps['network_speed'] = (empty($params['configoptions'][v_fn('network_speed')]) || $params['configoptions'][v_fn('network_speed')] == 0 ? $params['configoption14'] : $params['configoptions'][v_fn('network_speed')]);
	$post_vps['cpu_percent'] = (empty($params['configoptions'][v_fn('cpu_percent')]) || $params['configoptions'][v_fn('cpu_percent')] == 0 ? $params['configoption10'] : $params['configoptions'][v_fn('cpu_percent')]);
	$post_vps['cpu'] = $params['configoption8'];

	$post_vps['inodes'] = $params['configoption3'];
	$post_vps['burst'] = $params['configoption5'];
	$post_vps['priority'] = $params['configoption11'];
	$post_vps['swapram'] = $params['configoption6'];
	
	if($params['configoption12'] == 'yes' || $params['configoption12'] == 'on'){
		$post_vps['vnc'] = 1;
		if(empty($vps['vnc'])){
			$post_vps['vncpass'] = generateRandStr(8);
		}
	}
	
	$virttype = $post_vps['virt'];
	
	// IPs are the same always
	$post_vps['ips'] = $post_vps['ips'];
	
	// Add the IPv6
	if(!empty($post_vps['ips6'])){
		$post_vps['ipv6'] = $post_vps['ips6'];
	}
	
	// Add the IPv6 Subnet
	if(!empty($post_vps['ips6_subnet'])){
		$post_vps['ipv6_subnet'] = $post_vps['ips6_subnet'];
		foreach($post_vps['ipv6_subnet'] as $k => $v){
			$tmp = explode('/', $v);
			$post_vps['ipv6_subnet'][$k] = $tmp[0];
		}
	}
	
	$numips = (empty($params['configoptions'][v_fn('ips')]) || $params['configoptions'][v_fn('ips')] == 0 ? $params['configoption13'] : $params['configoptions'][v_fn('ips')]);
	$numips6 = (empty($params['configoptions'][v_fn('ips6')]) || $params['configoptions'][v_fn('ips6')] == 0 ? $params['configoption17'] : $params['configoptions'][v_fn('ips6')]);
	$numips6_subnet = (empty($params['configoptions'][v_fn('ips6_subnet')]) || $params['configoptions'][v_fn('ips6_subnet')] == 0 ? $params['configoption18'] : $params['configoptions'][v_fn('ips6_subnet')]);
	
	// Remove some IPs
	if($numips < count($post_vps['ips'])){
		
		$i = 0;
		$newips = array();
		
		foreach($post_vps['ips'] as  $k => $v){
			
			// We have completed
			if($numips == $i){
				break;
			}
			
			$newips[$k] = $v;
			$i++;
		}
		
		$post_vps['ips'] = $newips;
	
	// Add some IPs
	}elseif($numips > count($post_vps['ips'])){
		
		$toadd = $numips - count($post_vps['ips']);
		
		// Assign the IPs
		foreach($data['ips'] as $k => $v){
			
			if(in_array($v['ip'], $post_vps['ips'])){
				continue;
			}
			
			$post_vps['ips'][$k] = $v['ip'];
			
			if($numips == count($post_vps['ips'])){
				break;
			}
		}
		
		// Were there enough IPs
		if(count($post_vps['ips']) < $numips){
			return 'There are insufficient IPs on the server';
		}
		
	}
	
	// Remove some IPv6 Subnets
	if($numips6_subnet < count($post_vps['ipv6_subnet'])){
		
		$i = 0;
		$newips = array();
		
		foreach($post_vps['ipv6_subnet'] as  $k => $v){
			
			// We have completed
			if($numips6_subnet == $i){
				break;
			}
			
			$newips[$k] = $v;
			$i++;
			
		}
		
		$post_vps['ipv6_subnet'] = $newips;
	
	// Add some IP Subnet
	}elseif($numips6_subnet > count($post_vps['ipv6_subnet'])){
		
		$toadd = $numips6_subnet - count($post_vps['ipv6_subnet']);
		
		// Assign the IP Subnets
		foreach($data['ips6_subnet'] as $k => $v){
			
			if(in_array($v['ip'], $post_vps['ipv6_subnet'])){
				continue;
			}
			
			$post_vps['ipv6_subnet'][$k] = $v['ip'];
			
			if($numips6_subnet == count($post_vps['ipv6_subnet'])){
				break;
			}
		}
		
		// Were there enough IPs
		if(count($post_vps['ipv6_subnet']) < $numips6_subnet){
			return 'There are insufficient IPv6 Subnets on the server';
		}
		
	}
	
	// Remove some IPv6
	if($numips6 < count($post_vps['ipv6'])){
		
		$i = 0;
		$newips = array();
		
		foreach($post_vps['ipv6'] as  $k => $v){
			
			// We have completed
			if($numips6 == $i){
				break;
			}
			
			$newips[$k] = $v;
			$i++;
			
		}
		
		$post_vps['ipv6'] = $newips;
	
	// Add some IPs
	}elseif($numips6 > count($post_vps['ipv6'])){
		
		$toadd = $numips6 - count($post_vps['ipv6']);
		
		// Assign the IPs
		foreach($data['ips6'] as $k => $v){
			
			if(in_array($v['ip'], $post_vps['ipv6'])){
				continue;
			}
			
			$post_vps['ipv6'][$k] = $v['ip'];
			
			if($numips6 == count($post_vps['ipv6'])){
				break;
			}
		}
		
		// Were there enough IPs
		if(count($post_vps['ipv6']) < $numips6){
			return 'There are insufficient IPv6 Addresses on the server';
		}
		
	}
	
	// Are there any configurable options
	if(!empty($params['configoptions'])){
		foreach($params['configoptions'] as $k => $v){
			if(!isset($post_vps[$k])){
				$post_vps[$k] = $v;
			}
		}
	}
	
	$post_vps['editvps'] = 1;
	
	if($loglevel > 0) logActivity('Post Array : '.var_export($post_vps, 1));
	
	$ret = Virtualizor_Curl::make_api_call($params["serverip"], $params["serverpassword"], 'index.php?act=editvs&vpsid='.$params['customfields']['vpsid'], array(), $post_vps);
	
	unset($ret['scripts']);
	unset($ret['iscripts']);
	unset($ret['ostemplates']);
	unset($ret['isos']);
	
	if($loglevel > 0) logActivity('Post Result : '.var_export($ret, 1));
			
	if(empty($ret)){
		return 'Could not load the server data after processing.'.Virtualizor_Curl::error($params["serverip"]);
	}

    if(!empty($ret['done'])){
		
		$result = "success";

	}else{
			
		if(!empty($ret['error'])){
			return 'Errors : '.implode('<br>', $ret['error']);
		}
		
		$result = 'Unknown error occured. Please check logs';
		
	}
	
	return $result;
	
}

function virtualizor_AdminLink($params) {
	$code = '<a href="https://'.$params["serverip"].':4085/index.php?act=login" target="_blank">Virtualizor Admin Panel</a>';
	return $code;
}

function virtualizor_LoginLink($params) {
	echo $code = "<a href=\"https://".$params["serverip"].":4083/\" target=\"_blank\" style=\"color:#cc0000\">Login to Virtualizor</a>";
	return $code;
	
}

function virtualizor_AdminCustomButtonArray() {
	# This function can define additional functions your module supports, the example here is a reboot button and then the reboot function is defined below
    $buttonarray = array(
	 "Start VPS" => "admin_start",
	 "Reboot VPS" => "admin_reboot",
 	 "Stop VPS"=> "admin_stop",
	 "Poweroff VPS"=> "admin_poweroff"
	);
	return $buttonarray;
}


function virtualizor_ClientAreaCustomButtonArray() {
	# This function can define additional functions your module supports, the example here is a reboot button and then the reboot function is defined below
    $buttonarray = array(
	 "Start VPS" => "start",
	 "Reboot VPS" => "reboot",
 	 "Stop VPS"=> "stop",
	 "Poweroff VPS"=> "poweroff", 
	 "Launch VNC"=> "vnc"	  
	);
	return $buttonarray;
}


class Virtualizor_Curl {
	
	function error($ip = ''){
		
		$err = '';
		
		if(!empty($GLOBALS['virt_curl_err'])){
			$err .= ' Curl Error: '.$GLOBALS['virt_curl_err'];
		}
		
		if(!empty($ip)){
			$err .= ' (Server IP : '.$ip.')';
		}
		
		return $err;
	}
	
	function make_api_call($ip, $pass, $path, $data = array(), $post = array(), $cookies = array()){
		
		$key = generateRandStr(8);
		$apikey = make_apikey($key, $pass);
		
		$url = 'https://'.$ip.':4085/'.$path;	
		$url .= (strstr($url, '?') ? '' : '?');	
		$url .= '&api=serialize&apikey='.rawurlencode($apikey);
		
		// Pass some data if there
		if(!empty($data)){
			$url .= '&apidata='.rawurlencode(base64_encode(serialize($data)));
		}
		
		// Set the curl parameters.
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $url);
			
		// Time OUT
		curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 3);
		
		// Turn off the server and peer verification (TrustManager Concept).
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
			
		// UserAgent
		curl_setopt($ch, CURLOPT_USERAGENT, 'Softaculous');
		
		// Cookies
		if(!empty($cookies)){
			curl_setopt($ch, CURLOPT_COOKIESESSION, true);
			curl_setopt($ch, CURLOPT_COOKIE, http_build_query($cookies, '', '; '));
		}
		
		if(!empty($post)){
			curl_setopt($ch, CURLOPT_POST, 1);
			curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($post));
		}
		
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		
		// Get response from the server.
		$resp = curl_exec($ch);
		
		if(empty($resp)){
			$GLOBALS['virt_curl_err'] = curl_error($ch);
		}
			
		curl_close($ch);
		
		// The following line is a method to test
		//if(preg_match('/sync/is', $url)) echo $resp;
		
		if(empty($resp)){
			return false;
		}
		
		$r = _unserialize($resp);
		
		if(empty($r)){
			return false;
		}
		
		return $r;
	}	

	function e_make_api_call($ip, $pass, $vid, $path, $post = array()){
		$key = generateRandStr(8);
		$apikey = make_apikey($key, $pass);
		
		$url = 'https://'.$ip.':4083/'.$path;	
		$url .= (strstr($url, '?') ? '' : '?');	
		$url .= '&svs='.$vid.'&api=serialize&apikey='.rawurlencode($apikey);
		
		// Set the curl parameters.
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $url);
			
		// Time OUT
		curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 3);
		
		// Turn off the server and peer verification (TrustManager Concept).
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);
			
		// UserAgent and Cookies
		curl_setopt($ch, CURLOPT_USERAGENT, 'Softaculous');
		
		if(!empty($post)){
			curl_setopt($ch, CURLOPT_POST, 1);
			curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($post));
		}
		
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		
		// Get response from the server.
		$resp = curl_exec($ch);
		curl_close($ch);
		
		// The following line is a method to test
		//if(preg_match('/os/is', $url)) echo $resp;
		
		if(empty($resp)){
			return false;
		}
		
		$r = _unserialize($resp);
		
		if(empty($r)){
			return false;
		}
		
		return $r;
	}	
	
	function action($params, $action, $post = array()){
		
		global $virt_verify, $virt_errors;
		
		/*// Verify its this user
		if(empty($virt_verify)){
		
			$virt_verify = Virtualizor_Curl::make_api_call($params["serverip"], $params["serverpassword"], 'index.php?act=addvs');
			
		}*/
		
		// Make the call
		$response = Virtualizor_Curl::e_make_api_call($params["serverip"], $params["serverpassword"], $params['customfields']['vpsid'], 'index.php?'.$action, $post);

		if(empty($response)){
			$virt_errors[] = 'The action could not be completed as no response was received.';
			return false;
		}
		
		return $response;
	
	} // function virt_curl_action ends	

} // class virtualizor_curl ends


function virtualizor_ClientArea($params) {
	
	global $virt_action_display, $virt_errors, $virt_resp, $virtualizor_conf;
	
	$GLOBALS['virt_img_url'] = 'modules/servers/virtualizor/images/';
	
	if(!empty($virtualizor_conf['client_ui']['direct_login'])){
		return "<center><a href=\"https://".$params["serverip"].":4083/\" target=\"_blank\">Login to Virtualizor</a></center>";
	}

	$code = '
	<script src="modules/servers/virtualizor/js/jquery.flot.stack.js"></script>
	<script src="modules/servers/virtualizor/js/jquery.flot.min.js"></script>
	<script src="modules/servers/virtualizor/js/jquery.flot.pie.min.js"></script>
	
	<link rel="stylesheet" type="text/css" href="modules/servers/virtualizor/'.(!empty($virtualizor_conf['style']) ? $virtualizor_conf['style'] : 'style.css').'" />';
	
	$vps_manage = Virtualizor_Curl::action($params, 'act=vpsmanage&');
	
	//r_print($vps_manage);
	
	$code .='
		<script type="text/javascript">
		$(function () {
			
			var ram_resource = [
				{ label: "Used",  data: '.$vps_manage['info']['ram']['used'].'},
				{ label: "Free",  data: '.$vps_manage['info']['ram']['limit'].'}
			];
			
			var space_resource = [
				{ label: "Used",  data: '.$vps_manage['info']['disk']['used'].'},
				{ label: "Free",  data: '.$vps_manage['info']['disk']['limit'].'}
			];
			var bandwidth_resource = [
				{ label: "Used",  data: '.$vps_manage['info']['bandwidth']['used'].'},
				{ label: "Free",  data: '.$vps_manage['info']['bandwidth']['limit'].'}
			];
			
			// Ram Resource Graph
			resource_graph("ram_resource", ram_resource);
			
			// Space Resource Graph
			resource_graph("space_resource", space_resource);
			
			// IPv4 Resource Graph
			resource_graph("bandwidth_resource", bandwidth_resource);
			
		});

		// Draw a Resource Graph
		function resource_graph(id, data){

			$.plot($("#"+id), data, 
			{
				series: {
					pie: { 
						innerRadius: 0.7,
						radius: 1,
						show: true,
						label: {
							show: true,
							radius: 0,
							formatter: function(label, series){
								if(label != "Used") return "";
								return \'<div style="font-size:18px;text-align:center;padding:2px;color:black;">\'+Math.round(series.percent)+\'%</div><div style="font-size:10px;">\'+label+\'</div>\';	
							}
						}
					}
				},
				legend: {
					show: false
				}
			});
		}
		</script>
		';
	
	if($_GET['b'] == 'hostname'){
		$theme = virt_hostname($params);
	}
	
	if($_GET['b'] == 'changeRootPass'){
		$theme = virt_changeRootPass($params);
	}
	
	if($_GET['b'] == 'changeVncPass'){
		$theme = virt_changeVncPass($params);
	}
	
	if($_GET['b'] == 'rescue'){
		$theme = virt_rescue($params);
	}
	
	if($_GET['b'] == 'backup'){
		$theme = virt_backup($params);
	}
	
	if($_GET['b'] == 'ips'){
		$theme = virt_ips($params);
	}
	
	if($_GET['b'] == 'osreinstall'){
		$theme = virt_osreinstall($params);
	}

	if($_GET['b'] == 'bootorder'){
		$theme = virt_bootorder($params);
	}
	
	if($_GET['b'] == 'controlpanel'){
		$theme = virt_controlpanel($params);
	}
	
	if($_GET['b'] == 'cpu'){
		$theme = virt_cpu($params);
	}
	
	if($_GET['b'] == 'ram'){
		$theme = virt_ram($params);
	}
	
	if($_GET['b'] == 'disk'){
		$theme = virt_disk($params);
	}
	
	if($_GET['b'] == 'bandwidth'){
		$theme = virt_bandwidth($params);
	}
	
	if($_GET['b'] == 'performance'){
		$theme = virt_performance($params);
	}
	
	if($_GET['b'] == 'processes'){
		$theme = virt_processes($params);
	}
	
	if($_GET['b'] == 'services'){
		$theme = virt_services($params);
	}
	
	// Show a Done message
	if(!empty($virt_action_display)){		
		$code .= virt_done($virt_action_display);
	}
	
	// Any errors
	if(!empty($virt_errors)){
		$code .= virt_error($virt_errors);
	}
	
	// Show a Form
	if(!empty($theme)){		
		$code .= $theme;
	}
	
	
	$code .= '
	<table width="550" align="center" border="0"><tr><td align="center"><h3>Click on the icons to perform action </h3>';
	
	if($vps_manage['info']['status'] == 0){
		$code .= '<div style="color:#FF0000;font-size:20px">VPS is Offline</div>';
		
	}elseif(preg_match('/openvz/is', $params['configoption1'])){
	
		$code .= '<div class="cat_wrapper">
			<table cellpadding="8" cellspacing="1" border="0" width="100%" class="cat-title">
			<tr>
				<td colspan="3"><span class="cat-title-text">VPS Information</span></td>
			</tr>
			<tr>
				<td align="center" width="25%" valign="top">
					<b>RAM</b>
					<div id="ram_resource" class="resource_graph" style="height:100px;width:100px;"></div>
					<br />'.$vps_manage['info']['ram']['used'].' MB / '.$vps_manage['info']['ram']['limit'].' MB
				</td>
				<td align="center" width="25%" valign="top">
					<b>Space</b>
					<div id="space_resource" class="resource_graph" style="height:100px;width:100px;"></div>
					<br />'.$vps_manage['info']['disk']['used'].' MB / '.$vps_manage['info']['disk']['limit'].' MB
				</td>
				<td align="center" width="25%" valign="top">
					<b>Bandwidth</b>
					<div id="bandwidth_resource" class="resource_graph" style="height:100px;width:100px;"></div>
					<br />'.$vps_manage['info']['bandwidth']['used'].' MB / '.$vps_manage['info']['bandwidth']['limit'].' MB
				</td>
			</tr>
			<tr class="oddrowcolor">
				<th align="right">OS : </th><td colspan="2">'.$vps_manage['info']['os']['name'].'</td>
			</tr>
			<tr>
				<th align="right">IP : </th><td colspan="2">'.$vps_manage['info']['ip'][0].'</td>
			</tr>
			<tr class="oddrowcolor">
				<th align="right">Hostname : </th><td colspan="2">'.$vps_manage['info']['hostname'].'</td>
			</tr>
			<tr>
				<th align="right">Load : </th><td colspan="2">'.$vps_manage['info']['uptime'].'</td>
			</tr>
			
			</table>
			</div>';
	}
	
	$code .= '<br /><br />
	<div class="cat_wrapper">
	<table cellpadding="5" border="0" class="cat-title" width="100%">
	<tr>
		<td align="left" colspan="5"><span class="cat-title-text">VPS Management</span></td>
	</tr><tr><td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'start.gif" width="34" height="34" title="Start VPS"  alt="Start VPS" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&a=start\'" /></img><br />Start VPS</td>
	<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'stop.gif" width="34" height="34" title="Stop VPS" alt="Stop VPS" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&a=stop\'" /></img><br />Stop VPS</td>  
	<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'restart.gif" width="34" height="34" title="Reboot VPS" alt="Reboot VPS" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&a=reboot\'" /></img><br />Reboot VPS</td>
	<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'poweroff.gif" width="34" height="34" title="Power off VPS" alt="Power off VPS" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&a=poweroff\'" /></img><br />Power off</td>';
	
	if(!preg_match('/openvz/is', $params['configoption1'])){
		$code .= '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'vnc.gif" width="34" height="34" title="Launch VNC" alt="Launch VNC" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&a=vnc\'" /></img><br />Launch VNC</td>';
	}
	
	$code .= '</tr>
	</table>
	</div>
	<br /><br />
	<div class="cat_wrapper">
	<table cellpadding="5" border="0" class="cat-title" width="100%">
	<tr>
		<td align="left" colspan="5"><span class="cat-title-text">Configuration</span></td>
	</tr>
	<tr>';
	
	// OpenVZ
	if(preg_match('/openvz/is', $params['configoption1'])){
	
		$opts[] = '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'process.gif" width="34" height="34" title="Processes" alt="Processes" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=processes\'" /></img><br />Processes</td>';
	
		$opts[] = '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'services.gif" width="34" height="34" title="Services" alt="Services" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=services\'" /></img><br />Services</td>';
	
	}
	
	if(!empty($virtualizor_conf['client_ui']['hostname'])){
		$opts[] = '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'host.gif" width="34" height="34" title="Hostname" alt="Hostname" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=hostname\'" /></img><br />Hostname</td>';
	}
	
	$opts[] = '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'changepass.gif" width="34" height="34" title="Change Password" alt="Change Password" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=changeRootPass\'" /></img><br />Change Password</td>';
	
	if(!preg_match('/openvz/is', $params['configoption1'])){
		$opts[] = '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'vncpass.gif" width="34" height="34" title="Change VNC Password" alt="Change VNC Password" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=changeVncPass\'" /></img><br />Change VNC <br />Password</td>';
	}
	
	if(!empty($virtualizor_conf['client_ui']['control_panel_install'])){
		$opts[] = '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'cp.gif" width="34" height="34" title="CPinstall" alt="CPinstall" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=controlpanel\'" /></img><br />Control Panel Install</td>';
	}
	
	if(!preg_match('/openvz/is', $params['configoption1'])){
		$opts[] = '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'rescue.gif" width="34" height="34" title="rescue" alt="rescue" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=rescue\'" /></img><br />Rescue Mode</td>';
	}
	
	if(!preg_match('/openvz/is', $params['configoption1']) && !preg_match('/pv/is', $params['configoption1'])){
		$opts[] = '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'hvmsettingsdp.gif" width="34" height="34" title="bootorder" alt="bootorder" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=bootorder\'" /></img><br />VPS Configuration</td>';
	}
	
	$opts[] = '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'ipdp.gif" width="34" height="34" title="ips" alt="ips" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=ips\'" /></img><br />IPs</td>';
	
	
	if(!empty($virtualizor_conf['client_ui']['os_reinstall'])){
		$opts[] = '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'ostemp.gif" width="34" height="34" title="OSReinstall" alt="OSReinstall" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=osreinstall\'" /></img><br />OS Reinstall</td>';
	}
	
	if(preg_match('/openvz/is', $params['configoption1']) && !empty($virtualizor_conf['client_ui']['backups'])){
		$opts[] = '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'backups.gif" width="34" height="34" title="backup" alt="backup" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=backup\'" /></img><br />Backup</td>';
	}
	
	$ii = 0;
	foreach($opts as $ok => $ov){
		$code .= $ov;
		$ii++;
		if($ii == 5){
			$code .= '</tr><tr>';
		}
	}
	
	$code .= '</tr>
	</table>
	</div>
	<br /><br />
	<div class="cat_wrapper">
	<table cellpadding="5" border="0" class="cat-title" width="100%">
	<tr>
		<td align="left" colspan="5"><span class="cat-title-text">Info</span></td>
	</tr>
	<tr>';
	
	if(preg_match('/openvz/is', $params['configoption1'])){
		$code .= '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'cpu.gif" onMouseOver="this.style.cursor=\'pointer\'" width="34" height="34" title="CPU" alt="CPU" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=cpu\'" /></img><br />CPU</td>
		<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'ram.gif" width="34" height="34" title="RAM" alt="RAM" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=ram\'" /></img><br />RAM</td>
		<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'disk.gif" width="34" height="34" title="Disk" alt="Disk" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=disk\'" /></img><br />Disk</td>';
	}
	
	$code .= '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'bandwidth.gif" width="34" height="34" title="Bandwidth" alt="Bandwidth" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=bandwidth\'" /></img><br />Bandwidth</td>';
	
	if(preg_match('/openvz/is', $params['configoption1'])){
		$code .= '<td height="80" width="80" align="center"><img src="'.$GLOBALS['virt_img_url'].'performancedp.gif" width="34" height="34" title="Performance" alt="Performance" onMouseOver="this.style.cursor=\'pointer\'" onClick="window.location=\'clientarea.php?action=productdetails&id=' . $params['serviceid'] . '&modop=custom&b=performance\'" /></img><br />Performance</td>';
	}
	
	$code .= '</tr>
	</table>
	</div>
	<br /><br />';
	
	$code .= '<br /><br /></td></tr></table>';
	
	return $code;

}

function virt_error($error, $table_width = '500', $center = true, $ret = true){
	
	$str = '';
	
	//on error call the form
	if(!empty($error)){
		
		$str = '<table width="'.$table_width.'" cellpadding="2" cellspacing="1" style="background-color: rgb(230, 230, 230);" '.(($center) ? 'align="center"' : '' ).'>
			<tr>
			<td>
			The following errors occured :
			<ul type="square">';
		
		foreach($error as $ek => $ev){
		
			$str .= '<li>'.$ev.'</li>';
		
		}
		
		
		$str .= '</ul>
			</td>
			</tr>
			</table>'.(($center) ? '</center>' : '' ).'
			<br />';
		
		if(empty($ret)){
			echo $str;
		}else{
			return $str;
		}
		
	}

}

function virt_done($done){
	return '<center><div style="background-color: #FAFBD9; font-size:13px; padding:8px; text-align:center; margin-bottom: 20px; width: 500px"><img src="'.$GLOBALS['virt_img_url'].'notice.gif" /> &nbsp; '.$done.'</div></center>';
}

function virt_controlpanel($params) {
	
	$theme = '<h2>Control Panel Installation</h2>';
	
	$ins = @array_keys($_POST['ins']);
	
	if(!empty($ins)){
			
		$fields = array(
						'ins' => $_POST['ins']
						);
	
		$virt_resp = Virtualizor_Curl::action($params, 'act=controlpanel&',$fields);
		
		if(isset($virt_resp['done'])){

			$theme .= virt_done('Control Panel Installation has been Started');
		} elseif(isset($virt_resp['onboot'])) {

			$theme .= virt_done('Please stop and start the VPS after which the control panel installtion will start');
		} else {
		
			$virt_errors[] = 'There was an error while reinstalling the Control Panel';
			$theme .= virt_error($virt_errors);
		}
		
	}
	
	$theme .= '
	<script language="javascript" type="text/javascript">
		function confirmpanel(){
			if(confirm("Are you sure you want to install this panel ? Data on the server will be altered significantly.")){
				return true;
			}else{
				return false;
			}
		}
	</script>
	
	<form method="post" action="">
		<table cellpadding="8" cellspacing="1" border="0" class="divroundshad">
		<tr>
			<td colspan="5"><div class="roundheader">Control Panel Installation</div></td>
		</tr>
		<tr>
			<td align="center" width="100px">
				<input type="image" name="ins[cpanel]" onclick="return confirmpanel()" src="'.$GLOBALS['virt_img_url'].'cpanel.gif" /><br />cPanel		
			</td>
			<td align="center" width="100px">
				<input type="image" name="ins[plesk]" onclick="return confirmpanel()" src="'.$GLOBALS['virt_img_url'].'plesk.gif" /><br />Plesk			
			</td>
			<td align="center" width="100px">
				<input type="image" name="ins[webuzo]" onclick="return confirmpanel()" src="'.$GLOBALS['virt_img_url'].'webuzo.gif" /><br />Webuzo		
			</td>
			<td align="center" width="100px">
				<input type="image" name="ins[interworx]" onclick="return confirmpanel()" src="'.$GLOBALS['virt_img_url'].'interworx.gif" /><br />Interworx
			</td>
			<td align="center" width="100px">
				<input type="image" name="ins[webmin]" onclick="return confirmpanel()" src="'.$GLOBALS['virt_img_url'].'webmin.gif" /><br />Webmin	
			</td>
		</tr>
		</table>
	</form><br />';
	
	return $theme;
	
}

function virtualizor_start($params) {
	
	global $virt_action_display, $virt_errors;
	
	$virt_resp = Virtualizor_Curl::action($params, 'act=start&do=1');
	
	if(empty($virt_resp['done'])){
		$virt_action_display = 'The VPS failed to start';
		return $virt_action_display;
	}
	
	// Started it
	$virt_action_display = 'The VPS has been started';
	
	return true;

}

function virtualizor_stop($params) {
	
	global $virt_action_display, $virt_errors;
	
	$virt_resp = Virtualizor_Curl::action($params, 'act=stop&do=1');
	
	if(empty($virt_resp)){
		$virt_action_display = 'Failed to stop the VPS';
		return $virt_action_display;
	}
	
	// Started it
	$virt_action_display = 'The Stop Signal has been sent to the VPS';
	
	return true;

}

function virtualizor_reboot($params) {
	
	global $virt_action_display, $virt_errors;
	
	$virt_resp = Virtualizor_Curl::action($params, 'act=restart&do=1');
	
	if(empty($virt_resp)){
		$virt_action_display = 'Failed to reboot the VPS';
		return $virt_action_display;
	}
	
	// Started it
	$virt_action_display = 'The Restart Signal has been sent to the VPS';
	
	return true;

}


function virtualizor_poweroff($params) {
	
	global $virt_action_display, $virt_errors;
	
	$virt_resp = Virtualizor_Curl::action($params, 'act=poweroff&do=1');
	
	if(empty($virt_resp)){
		$virt_action_display = 'Failed to poweroff the VPS';
		return $virt_action_display;
	}
	
	// Started it
	$virt_action_display = 'The VPS has been Powered off';
	
	return true;

}


function virtualizor_admin_start($params) {
	$ret = virtualizor_start($params);
	
	if($ret === true){
		return 'success';
	}else{
		return $ret;
	}
}

function virtualizor_admin_stop($params) {
	$ret = virtualizor_stop($params);
	
	if($ret === true){
		return 'success';
	}else{
		return $ret;
	}
}

function virtualizor_admin_reboot($params) {
	$ret = virtualizor_reboot($params);
	
	if($ret === true){
		return 'success';
	}else{
		return $ret;
	}
}

function virtualizor_admin_poweroff($params) {
	$ret = virtualizor_poweroff($params);
	
	if($ret === true){
		return 'success';
	}else{
		return $ret;
	}
}


function virtualizor_vnc($params){
	
	global $virt_action_display, $virt_errors;
	
	$response = Virtualizor_Curl::action($params, 'act=vnc&launch=1', '', true);
	
	if(empty($response)){
		return false;
	}

	$virttype = preg_match('/xcp/is', $params['configoption1']) ? 'xcp' : strtolower($params['configoption1']);
	
	if($virttype != 'xcp'){
		if(!empty($response['info']['port']) && !empty($response['info']['ip']) && !empty($response['info']['password'])){
			
			$applet = '<APPLET ARCHIVE="http://'.$params['serverip'].':4082/themes/default/java/vnc/TightVncViewer.jar" CODE="com.tightvnc.vncviewer.VncViewer" WIDTH="1" HEIGHT="1">
				<PARAM NAME="HOST" VALUE="'.$response['info']['ip'].'">
				<PARAM NAME="PORT" VALUE="'.$response['info']['port'].'">
				<PARAM NAME="PASSWORD" VALUE="'.$response['info']['password'].'">
				<PARAM NAME="Open New Window" VALUE="yes">
			</APPLET>';	
		}
	}else{
		if(!empty($response['info']['port']) && !empty($response['info']['ip'])){
			$applet = '<APPLET ARCHIVE="http://'.$params['serverip'].':4082/themes/default/java/vnc/TightVncViewer.jar" CODE="com.tightvnc.vncviewer.VncViewer" WIDTH="1" HEIGHT="1">
				<PARAM NAME="SOCKETFACTORY" value="com.tightvnc.vncviewer.SshTunneledSocketFactory">
				<PARAM NAME="SSHHOST" value="'.$response['info']['ip'].'">
				<PARAM NAME="HOST" value="localhost">
				<PARAM NAME="PORT" value="'.$response['info']['port'].'">
				<PARAM NAME="Open New Window" VALUE="yes">
			</APPLET>';
		}
	}
	
	if(empty($applet)){
		$virt_errors[] = 'There were errors while launching the VNC Viewer. It could be disabled for this VPS';
		$virt_action_display = 'There were errors while launching the VNC Viewer. It could be disabled for this VPS';
		return $virt_action_display;
	}
	
	$virt_action_display = 'The VNC has been launched.'.$applet;
	
	return true;
}

function virt_cpu($params){	
	
	$theme = '<h2>CPU Information</h2>';
	
	$virt_resp = Virtualizor_Curl::action($params, 'act=cpu&');
	
	if(empty($virt_resp)){
	
		$virt_errors[] = 'There was an error while fetching the Details';
		$theme .= virt_error($virt_errors);
		
	}else{
	
		$theme .= '
		<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
		<tr>
			<td colspan="3"><div class="roundheader">CPU Information</div></td>
		</tr>
		<tr>
			<td width="50%"><b>CPU : </b></td>
			<td>'.$virt_resp['cpu']['limit'].' units</td>
			<td rowspan="3"><div id="cpuholder" style="width:100px; height:100px;"></div></td>
		</tr>
		<tr>
			<td><b>Utilization : </b></td>
			<td>'.(preg_match('/xen|kvm/is', $params['configoption1']) ? '<i>No Info</i>' : $virt_resp['cpu']['percent'].' %').'</td>
		</tr>
		<tr>
			<td><b>Manufacturer : </b></td>
			<td>'.ucfirst($virt_resp['cpu']['manu']).'</td>
		</tr>
		</table>
		
		<script type="text/javascript" charset="utf-8">
		
			$(document).ready(function(){
			
				var cpu = [
					{ label: "Used",  data: '.$virt_resp['cpu']['percent'].'},
					{ label: "Free",  data: '.$virt_resp['cpu']['percent_free'].'}
				];

				$.plot($("#cpuholder"), cpu, 
				{
					series: {
						pie: { 
							innerRadius: 0.7,
							radius: 1,
							show: true,
							label: {
								show: true,
								radius: 0,
								formatter: function(label, series){
									if(label != "Used") return "";
									return \'<div style="font-size:18px;text-align:center;padding:2px;color:black;">\'+Math.round(series.percent)+\'%</div><div style="font-size:10px;">\'+label+\'</div>\';	
								}
							}
						}
					},
					legend: {
						show: false
					}
				});
			
			});
		</script>
		
		';
	}
	
	return $theme;
	
}

function virt_ram($params){
	
	$theme = '<h2>RAM Information</h2>';
	
	$virt_resp = Virtualizor_Curl::action($params, 'act=ram&');
	
	if(empty($virt_resp)){
	
		$virt_errors[] = 'There was an error while fetching the Details';
		$theme .= virt_error($virt_errors);
		
	}else{
	
		$theme .= '
		<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
		<tr>
			<td colspan="3"><div class="roundheader">RAM Information</div></td>
		</tr>
		<tr>
			<td width="50%"><b>RAM : </td>
			<td>'.$virt_resp['ram']['limit'].' MB</td>
			<td rowspan="3"><div id="ramholder" style="width:100px; height:100px;"></div></td>
		</tr>
		<tr>
			<td><b>Utilization : </b></td>
			<td>'.(preg_match('/xen|kvm/is', $params['configoption1']) ? '<i>No Info</i>' : $virt_resp['ram']['used'].' MB').'</td>
		</tr>
		</table>
		
		<script type="text/javascript" charset="utf-8">
		
			$(document).ready(function(){
			
				var cpu = [
					{ label: "Used",  data: '.$virt_resp['ram']['used'].'},
					{ label: "Free",  data: '.$virt_resp['ram']['free'].'}
				];

				$.plot($("#ramholder"), cpu, 
				{
					series: {
						pie: { 
							innerRadius: 0.7,
							radius: 1,
							show: true,
							label: {
								show: true,
								radius: 0,
								formatter: function(label, series){
									if(label != "Used") return "";
									return \'<div style="font-size:18px;text-align:center;padding:2px;color:black;">\'+Math.round(series.percent)+\'%</div><div style="font-size:10px;">\'+label+\'</div>\';	
								}
							}
						}
					},
					legend: {
						show: false
					}
				});
			
			});
		</script>
		';
	}
	
	return $theme;
	
}

function virt_disk($params){
	
	$theme = '<h2>Disk Information</h2>';
	
	$virt_resp = Virtualizor_Curl::action($params, 'act=disk&');
	
	if(empty($virt_resp)){
	
		$virt_errors[] = 'There was an error while fetching the Details';
		$theme .= virt_error($virt_errors);
		
	}else{
	
		$theme .= '
		<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
		<tr>
			<td colspan="3"><div class="roundheader">Disk Information</div></td>
		</tr>
		<tr>
			<td width="50%"><b>Disk Space : </b></td>
			<td>'.$virt_resp['disk']['limit_gb'].' GB</td>
			<td rowspan="3"><div id="diskholder" style="width:100px; height:100px;"></div></td>
		</tr>
		<tr>
			<td><b>Utilization : </b></td>
			<td>'.(preg_match('/xen|kvm/is', $params['configoption1']) ? '<i>No Info</i>' : $virt_resp['disk']['used_gb'].' GB').'</td>
		</tr>
		</table>
		
		<script type="text/javascript" charset="utf-8">
		
			$(document).ready(function(){
			
				var cpu = [
					{ label: "Used",  data: '.$virt_resp['disk']['used_gb'].'},
					{ label: "Free",  data: '.$virt_resp['disk']['limit_gb'].'}
				];

				$.plot($("#diskholder"), cpu, 
				{
					series: {
						pie: { 
							innerRadius: 0.7,
							radius: 1,
							show: true,
							label: {
								show: true,
								radius: 0,
								formatter: function(label, series){
									if(label != "Used") return "";
									return \'<div style="font-size:18px;text-align:center;padding:2px;color:black;">\'+Math.round(series.percent)+\'%</div><div style="font-size:10px;">\'+label+\'</div>\';	
								}
							}
						}
					},
					legend: {
						show: false
					}
				});
			
			});
		</script>
		
		';
	}
	
	return $theme;
	
}

function virt_bandwidth($params){	
	
	$theme = '<h2>Bandwidth Information</h2>';
	
	$band_resp = Virtualizor_Curl::action($params, 'act=bandwidth&');
	
	if(empty($band_resp)){
	
		$virt_errors[] = 'There was an error while fetching the Details';
		$theme .= virt_error($virt_errors);
		
	}else{
	
		$theme .= '
		<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
		<tr>
			<td colspan="3"><div class="roundheader">Bandwidth Information</div></td>
		</tr>
		<tr>
			<td colspan="3"><center><div style="width:85%; height:300px;" id="bwband_holder"></div></center></td>
		</tr>
		<tr>
			<td width="50%"><b>Allowed Limit : </b></td>
			<td>'.$band_resp['bandwidth']['limit_gb'].' GB</td>
			<td rowspan="3"><div style="height:100px;width:100px; " id="bwpiechart_holder"></div></td>
		</tr>
		<tr>
			<td><b>Utilization : </b></td>
			<td>'.$band_resp['bandwidth']['used_gb'].' GB'.'</td>
		</tr>
		<tr>
			<td><b>% usage : </b></td>
			<td>'.$band_resp['bandwidth']['percent'].' %'.'</td>
		</tr>
		</table>
		
		<script type="text/javascript" charset="utf-8">
		
			$(document).ready(function(){
			
				var res = [
					{ label: "Used",  data: '.$band_resp['bandwidth']['used_gb'].'},
					{ label: "Free",  data: '.$band_resp['bandwidth']['free_gb'].'}
				];
				
				resource_graph("bwpiechart_holder", res);
				
				function resource_graph(id, data){

					$.plot($("#"+id), data, 
					{
						series: {
							pie: { 
								innerRadius: 0.7,
								radius: 1,
								show: true,
								label: {
									show: true,
									radius: 0,
									formatter: function(label, series){
										if(label != "Used") return "";
										return \'<div style="font-size:18px;text-align:center;padding:2px;color:black;">\'+Math.round(series.percent)+\'%</div><div style="font-size:10px;">\'+label+\'</div>\';	
									}
								}
							}
						},
						legend: {
							show: false
						}
					});
				}

				function makedata(data){
				
					var fdata = [];
					i = 0;
					for (x in data){
						fdata.push([i, (data[x])]);
						i++;
					}
				
					return fdata;
					
				}
				
				var d1 = makedata([0, '.implode(', ', $band_resp['bandwidth']['usage']).']);';
				
				// Inbound and outbound data is currently not available in KVM and XEN
				if(!empty($band_resp['bandwidth']['in']) && !empty($band_resp['bandwidth']['out'])){
					
					$theme .= '
					var indata = makedata([0, '.implode(', ', $band_resp['bandwidth']['in']).']);
					var outdata = makedata([0, '.implode(', ', $band_resp['bandwidth']['out']).']);';
				}
				
				$theme .= '
				var bandwidth_graph = [
					{ label: "Usage",  data: d1},';
				
					if(!empty($band_resp['bandwidth']['in']) && !empty($band_resp['bandwidth']['out'])){
						
						$theme .= '
						{ label: "In",  data: indata},
						{ label: "Out",  data: outdata}';
					}
					
				$theme .= '
				];
				
				$.plot($("#bwband_holder"), bandwidth_graph, {
					series: {
						points: { show: true },
						lines: { show: true, fill: true, steps: false }
					},
					legend: {
						show: true
					},
					grid: { hoverable: true}
				});
				
				function showTooltip(x, y, contents) {
					$(\'<div id="tooltip">\' + contents + \'</div>\').css( {
						position: "absolute",
						display: "none",
						top: y ,
						left: x + 20,
						border: "1px solid #CCCCCC",
						padding: "2px",
						"background-color": "#EFEFEF",
						"z-index" : 10000,
						opacity: 0.80
					}).appendTo("body").fadeIn(200);
				}

				var previousPoint = null;
				$("#bwband_holder").bind("plothover", function (event, pos, item) {
					$("#x").text(pos.x.toFixed(2));
					$("#y").text(pos.y.toFixed(2));

					if (item) {
						
						if (previousPoint != item.dataIndex) {
							previousPoint = item.dataIndex;
							$("#tooltip").remove();
							var x = item.datapoint[0].toFixed(2),
								y = item.datapoint[1].toFixed(2);
								
							showTooltip(item.pageX, item.pageY,
										"Total : " + parseInt(y) + " MB <br>Day : " + parseInt(x));
						}
					} else {
						$("#tooltip").remove();
						previousPoint = null;
					}
				});
				
			});
		</script>';
		
		//r_print($band_resp);
		
	}
	
	return $theme;
	
}

function virt_performance($params){	
	
	$theme = '<h2>VPS Performance</h2>';
	
	$virt_resp = Virtualizor_Curl::action($params, 'act=performance&');
	
	if(empty($virt_resp)){
	
		$virt_errors[] = 'There was an error while fetching the Details';
		$theme .= virt_error($virt_errors);
		
	}else{
	
		$theme .= '
		<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
		<tr>
			<td colspan="2"><div class="roundheader">VPS Performance</div></td>
		</tr>
		<tr>
			<td width="50%"><b>CPU Used : </b></td>
			<td>'.(preg_match('/xen|kvm/is', $params['configoption1']) ? '<i>No Info</i>' : $virt_resp['perfomance']['cpu'].' %').'</td>
		</tr>
		<tr>
			<td><b>RAM Used : </b></td>
			<td>'.(preg_match('/xen|kvm/is', $params['configoption1']) ? '<i>No Info</i>' : $virt_resp['perfomance']['ram'].' %').'</td>
		</tr>
		</table>';
	}
	
	return $theme;
	
}

function virt_processes($params){	
	
	$theme = '<h2>VPS Processes</h2>';
	
	$virt_resp = Virtualizor_Curl::action($params, 'act=processes&');
	
	if(empty($virt_resp)){
	
		$virt_errors[] = 'There was an error while fetching the Details';
		$theme .= virt_error($virt_errors);
		
	}else{
		
		if(preg_match('/xen|kvm/is', $params['configoption1'])){
		
			$theme .= virt_done('This feature is not available');
			
		}else{
		
			$processes = $virt_resp['processes'];
			//r_print($processes);
			
			$theme .= '<table class="summaryBlock" align="center" cellpadding="6" cellspacing="2" border="0" width="100%">
			<tr>';
			
			$keys = array_keys(current($processes));
			
			foreach($keys as $k => $v){
				$theme .= '<td align="center" class="summaryTitle">'.$v.'</td>';
			}
			$theme .= '</tr>';
			//r_print($processes);	
			foreach($processes as $k=>$v){
				$theme .= '<tr>';	
				foreach($v as $vv){
					$theme .= '<td align="center">'.$vv.'</td>';
				}
				$theme .= '</tr>';
			}
					
			$theme .= '</table>';
		
		}
		
	}
	
	return $theme;
	
}

function virt_services($params){
	
	$theme = '<h2>VPS Services</h2>';
	
	$virt_resp = Virtualizor_Curl::action($params, 'act=services&');
	
	if(empty($virt_resp)){
	
		$virt_errors[] = 'There was an error while fetching the Details';
		$theme .= virt_error($virt_errors);
		
	}else{
		
		if(preg_match('/xen|kvm/is', $params['configoption1'])){
		
			$theme .= virt_done('This feature is not available');
			
		}else{
		
			$services = $virt_resp['services'];
			$autostart = $virt_resp['autostart'];
			$running = $virt_resp['running'];
			
			$theme .= '<table align="center" cellpadding="5" cellspacing="2" border="0" width="100%" class="summaryBlock">
		<tr>
			<td class="summaryTitle" align="center">Services</td>
			<td align="center" class="summaryTitle">Status</td>
			<td align="center" class="summaryTitle">Auto Start</td>
		</tr>';
		
			foreach($services as $k=>$v){
				$theme .= '<tr>
					<td>'.$v.'</td>		
					<td align="center">'.(in_array($v, $running) ? '&nbsp;&nbsp;Running': '&nbsp;&nbsp;OFF').'</td>
					<td align="center">'.(in_array($v, $autostart) ? '&nbsp;&nbsp;ON' : '&nbsp;&nbsp;OF').'</td>
				</tr>';
			}
	
					
			$theme .= '</table>';
		
		}
		
	}
	
	return $theme;
	
}

// Show the Hostname Form
function virt_hostname($params){
	
	$theme = '<h2>Change Hostname</h2>';
	
	if(isset($_POST['virt_changehostname'])){
		$fields = array(
						'newhost' => $_POST['virt_newhostname'],
						'changehost' => 'Change Hostname'
						);
			
		$virt_resp = Virtualizor_Curl::action($params, 'act=hostname&', $fields);
		
		if(empty($virt_resp['done'])){
			$virt_errors[] = 'There was an error changing the Hostname';
			$theme .= virt_error($virt_errors);
		}else{
			$theme .= virt_done('The Hostname was changed successfully');
		
			// Change the Hostname
			mysql_query("UPDATE `tblhosting` SET `domain` = '".$_POST['virt_newhostname']."' WHERE `id` = '".$params['serviceid']."';");
			
		}
	}else{
		$virt_resp = Virtualizor_Curl::action($params, 'act=hostname&');
	}
	
	$theme .= '
	<form method="post" action="">
		<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
		<tr>
			<td colspan="2"><div class="roundheader">Change Hostname</div></td>
		</tr>
		<tr>
			<td width="50%"><b>Hostname : </b></td>
			<td>'.$virt_resp['current'].'</td>
		</tr>
		<tr>
			<td>New Hostname : </td>
			<td><input type="text" name="virt_newhostname" id="virt_newhostname" value="" /></td>
		</tr>
		<tr>
			<td colspan="2" align="center"><input type="submit" value="Submit" name="virt_changehostname" />
		</td>
	</table>
	</form><br />';
	
	return $theme;
		
}

function virt_changeRootPass($params) {	
	
	$theme = '<h2>Change Root Password</h2>';
	
	if(isset($_POST['virt_changepassword'])){
		$fields = array(
					'newpass' => $_POST['virt_newpassword'],
					'conf' => $_POST['virt_newpasswordconf'],
					'changepass' => 'Change Password'
					);
			
		$virt_resp = Virtualizor_Curl::action($params, 'act=changepassword&', $fields);
		
		if(isset($virt_resp['onboot'])){
			$theme .= virt_done('The Password will be changed when the VPS is booted again');
		}elseif(isset($virt_resp['done'])){
			$theme .= virt_done('The Password was changed successfully');
		}else{
			$virt_errors[] = 'There was an error changing the Password';
			$theme .= virt_error($virt_errors);
		}
		
		// Change the Password
		if(empty($virt_errors)){
			mysql_query("UPDATE `tblhosting` SET `password` = '".encrypt($_POST['virt_newpassword'])."' WHERE `id` = '".$params['serviceid']."';");
		}
	}
	
	$theme .= '
	<form method="post" action="">
		<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
		<tr>
			<td colspan="2"><div class="roundheader">Change Root Password</div></td>
		</tr>
		<tr>
			<td width="50%"><b>New Password : </b></td>
			<td><input type="password" name="virt_newpassword" value="" /></td>
		</tr>
		<tr>
			<td><b>Confirm Password : </b></td>
			<td><input type="password" name="virt_newpasswordconf" value="" /></td>
		</tr>
		<tr>
			<td colspan="2" align="center">
				<input type="submit" value="Submit" name="virt_changepassword" />
			</td>
		</tr>
		</table>
	</form><br />';
	
	return $theme;
	
}

function virt_rescue($params){
	
	if(preg_match('/openvz/is', $params['configoption1'])){	
		global $virt_errors;
		$virt_errors[] = 'Rescue Mode is not available for OpenVZ';
		return false;
	}
	
	$theme = '<h2>Rescue Mode</h2>';
	
	if(isset($_POST['enablerescue'])){
	
		$fields = array(
					'password' => $_POST['password'],
					'conf_password' => $_POST['conf_password'],
					'enablerescue' => 'Enable Rescue Mode'
					);
			
		$virt_resp = Virtualizor_Curl::action($params, 'act=rescue&', $fields);
		
		if(isset($virt_resp['done'])){
			$theme .= virt_done('Rescue Mode has been Enabled');
		}else{
			$virt_errors[] = 'There was an error while enabling the Rescue Mode';
			$theme .= virt_error($virt_errors);
		}
		
	}elseif(isset($_POST['disablerescue'])){
	
		$fields = array(
					'disablerescue' => 'Enable Rescue Mode'
					);
			
		$virt_resp = Virtualizor_Curl::action($params, 'act=rescue&', $fields);
		
		if(isset($virt_resp['done'])){
			$theme .= virt_done('Rescue Mode has been Disabled');
		}else{
			$virt_errors[] = 'There was an error while disabling the Rescue Mode';
			$theme .= virt_error($virt_errors);
		}
	}else{
		$virt_resp = Virtualizor_Curl::action($params, 'act=rescue&');
	}
	
	if(!empty($virt_resp['rescue_enabled'])){
		$theme .= '<center>'.virt_done('Rescue Enabled').'</center>';
		
		// Disable rescue form
		$theme .= '<form action="" method="post">
		<br />
		<br />
		<table cellpadding="8" cellspacing="1">
		<tr><td><center>'.virt_done('Rescue Enabled').'</center><br /></td></tr>
		<tr>
			<td align="center">
			<input type="submit" value="Disable Rescue Mode" name="disablerescue" />
			</td>
		</tr>
		</table>
		</form><br />';
		
	}else{
	
		$theme .= '
		<form method="post" action="">
			<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
				<tr>
					<td colspan="2"><div class="roundheader">Rescue Mode</div></td>
				</tr>
				<tr>
					<td width="50%"><b>Root Password</b></td>
					<td><input type="password" name="password" value="" /></td>
				</tr>
				<tr>
					<td><b>Confirm Password : </b></td>
					<td><input type="password" name="conf_password" value="" /></td>
				</tr>
				<tr>
					<td colspan="2" align="center">
						<input type="submit" value="Enable Rescue Mode" name="enablerescue" />
					</td>
				</tr>
			</table>
		</form><br />';
	}
	
	return $theme;
	
}

function virt_changeVncPass($params) {
	
	if(!preg_match('/xen|kvm/is', $params['configoption1'])){	
		global $virt_errors;
		$virt_errors[] = 'There were errors while launching the VNC Viewer. It could be disabled for this VPS';
		return false;
	}
	
	$theme = '<h2>Change VNC Password</h2>';
	
	if(isset($_POST['virt_vncpass'])){
		$fields = array(
					'newpass' => $_POST['virt_newvncpass'],
					'conf' => $_POST['virt_vncpassconf'],
					'vncpass' => 'Change Password'
					);
			
		$virt_resp = Virtualizor_Curl::action($params, 'act=vncpass&', $fields);
		
		if(isset($virt_resp['done'])){
			$theme .= virt_done('The Password was changed successfully. The changes will take effect when the VPS is booted again.');
		}else{
			$virt_errors[] = 'There was an error changing the Password';
			$theme .= virt_error($virt_errors);
		}
	}
	
	$theme .= '
		<form method="post" action="">
			<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
				<tr>
					<td colspan="2"><div class="roundheader">Change VNC Password</div></td>
				</tr>
				<tr>
					<td width="50%"><b>New Password : </b></td>
					<td><input type="password" name="virt_newvncpass" value="" /></td>
				</tr>
				<tr>
					<td><b>Confirm Password : </b></td>
					<td><input type="password" name="virt_vncpassconf" value="" /></td>
				</tr>
				<tr>
					<td colspan="2" align="center">
						<input type="submit" value="Submit" name="virt_vncpass" />
					</td>
				</tr>
			</table>
		</form><br />';
	
	return $theme;
	
}

function virt_bootorder($params){

	if(preg_match('/openvz/is', $params['configoption1']) || preg_match('/pv/is', $params['configoption1'])){
		global $virt_errors;
		$virt_errors[] = 'VPS configuration is not available for OpenVZ, XEN-PV and XCP-PV VPS';
		return false;
	}
	
	$theme = '<h2>VPS Configuration</h2>';
	
	//$virt = preg_match('/xen/is', $params['configoption1']) ? 'xen' : (preg_match('/xcp/is', $params['configoption1']) ? 'xcp' : strtolower($params['configoption1']));
	
	if(isset($_POST['hvmsettings'])){
		$fields = array(
					'acpi' => $_POST['acpi'],
					'apic' => $_POST['apic'],
					'vnc' => $_POST['vnc'],
					'boot' => $_POST['boot'],
					'isos' => $_POST['isos'],
					'hvmsettings' => 'VPS Configuration'
					);
			
		$virt_resp = Virtualizor_Curl::action($params, 'act=hvmsettings&', $fields);
		
		if(isset($virt_resp['done'])){
			$theme .= virt_done('Settings has been saved');
		}else{
			$virt_errors[] = 'There was an error while saving';
			$theme .= virt_error($virt_errors);
		}
	}else{
		$virt_resp = Virtualizor_Curl::action($params, 'act=hvmsettings&');
	}
	
	//r_print($virt_resp);
	$theme .= '
		<form method="post" action="">
			<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
				<tr>
					<td colspan="2"><div class="roundheader">VPS Configuration</div></td>
				</tr>
				<tr>
					<td width="50%"><b>ACPI : </b></td><td><input type="checkbox" name="acpi" '.($virt_resp['vps']['acpi'] == 1 ? 'checked="checked"' : '').' /></td>
				</tr>
				<tr>
					<td><b>APIC : </b></td><td><input type="checkbox" name="apic" '.($virt_resp['vps']['apic'] == 1 ? 'checked="checked"' : '').' /></td>
				</tr>
				<tr>
					<td><b>Change Boot Order : </b></td>
					<td>
						<input name="boot" type="radio" value="cd" '.(current($virt_resp['boot']) == "Hard Disk" ? 'checked="checked"' : '').'/> &nbsp; Hard Disk<br />
						<input name="boot" type="radio" value="dc" '.(current($virt_resp['boot']) == "CD Drive" ? 'checked="checked"' : '').'/> &nbsp; CD Drive<br />
					</td>
				</tr>
				<tr>
					<td><b>Select ISO : </b></td>
					<td>
				
							<select name="isos">
							<option value="0">None</option>';
					foreach($virt_resp['isos'] as $k => $v){		 
						$theme .= '<option value="'.$k.'" '.($virt_resp['vps']['iso'] == $k ? 'selected="selected"' : '').'>'.$v['name'].'</option>';
					}
					$theme .= '</select>
					</td>
				</tr>
				<tr>
					<td colspan="2" align="center">
						<input type="submit" value="Submit" name="hvmsettings" />
					</td>
				</tr>
			</table>
		</form><br />';
	
	return $theme;
	
}

function virt_osreinstall($params) {
	
	$theme = '<h2>Reinstall OS</h2>';
	
	$virt = preg_match('/xen/is', $params['configoption1']) ? 'xen' : (preg_match('/xcp/is', $params['configoption1']) ? 'xcp' : strtolower($params['configoption1']));
	
	if(isset($_POST['virt_reinsos'])){
		$fields = array(
					'newos' => $_POST['virt_newos'],
					'newpass' => $_POST['virt_newpass'],
					'conf' => $_POST['virt_passconf'],
					'reinsos' => 'Reinstall'
					);
			
		$virt_resp = Virtualizor_Curl::action($params, 'act=ostemplate&', $fields);
		
		if(isset($virt_resp['done'])){
			$theme .= virt_done('The OS has been reinstalled');
		}else{
			$virt_errors[] = 'There was an error while reinstalling the OS';
			$theme .= virt_error($virt_errors);
		}
	}else{
		$virt_resp = Virtualizor_Curl::action($params, 'act=ostemplate&');
	}
	
	$theme .= '
		<form method="post" action="">
			<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
				<tr>
					<td colspan="2"><div class="roundheader">Reinstall OS</div></td>
				</tr>
				<tr>
					<td width="50%"><b>Available Templates : </b></td>
					<td align="left">';
					
					foreach($virt_resp['oslist'][$virt] as $kk => $vv){
						foreach($vv as $k => $v){
							$theme .= '<input name="virt_newos" type="radio" value="'.$k.'" /> &nbsp; '.$v['name'].'<br />';
						}
					}
					
					$theme .= '</td>
				</tr>
				<tr>
					<td><b>New Password : </b></td>
					<td><input type="password" name="virt_newpass" value="" /></td>
				</tr>
				<tr>
					<td><b>Confirm Password : </b></td>
					<td><input type="password" name="virt_passconf" value="" /></td>
				</tr>
				<tr>
					<td colspan="2" align="center">
						<input type="submit" value="Submit" name="virt_reinsos" />
					</td>
				</tr>
			</table>
		</form><br />';
	
	return $theme;
}

function virt_backup($params) {
	
	$theme = '<h2>Backups</h2>';
	
	if($params['configoption1'] != 'OpenVZ'){
		return false;
	}
	
	$virt = preg_match('/xen/is', $params['configoption1']) ? 'xen' : (preg_match('/xcp/is', $params['configoption1']) ? 'xcp' : strtolower($params['configoption1']));
	
	if(isset($_POST['cbackup'])){
	
		$virt_resp = Virtualizor_Curl::action($params, 'act=backup&cbackup=true');
		
		if(isset($virt_resp['done'])){
			$theme .= virt_done('The VPS backup process has been started. Please allow a few minutes for it to complete. You will recieve a notification email when its is completed');
		}else{
			$virt_errors[] = 'There was an error while Backup';
			$theme .= virt_error($virt_errors);
		}
	}elseif(isset($_POST['restore'])){
		$virt_resp = Virtualizor_Curl::action($params, 'act=backup&restore=true');
		
		if(isset($virt_resp['done'])){
			$theme .= virt_done('The VPS restore process has been started. Please allow a few minutes for it to complete. You will recieve a notification email when its is completed');
		}else{
			$virt_errors[] = 'There was an error while Restore';
			$theme .= virt_error($virt_errors);
		}
	}else{
		// Call to check backup list
		$virt_resp = Virtualizor_Curl::action($params, 'act=backup&');
	}
	
	$theme .= '
		<form method="post" action="">
			<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
				<tr>
					<td colspan="2"><div class="roundheader">Backups</div></td>
				</tr>
				<tr>
					<td colspan="2" align="center">Here you can create backups of your VPS and also restore from old backups.</td>
				</tr>
				<tr>
					<td align="center">
						<input type="submit" value="Backup" name="cbackup" />
					</td>';
					
					if($virt_resp['backups_list']){					
						$theme .= '<td align="center">
							<input type="submit" value="Restore" name="restore" />
						</td>';
					}
					
				$theme .= '</tr>
			</table>
		</form><br />';
	
	return $theme;
	
}

function virt_ips($params){
	
	$theme = '<h2>IPs</h2>';
	
	$virt = preg_match('/xen/is', $params['configoption1']) ? 'xen' : (preg_match('/xcp/is', $params['configoption1']) ? 'xcp' : strtolower($params['configoption1']));
	
	if(isset($_POST['reorderips'])){
	
		$fields = array(
					'ips' => $_POST['ips'],
					'reorderips' => 'IPs'
					);
					
		$virt_resp = Virtualizor_Curl::action($params, 'act=ips&', $fields);
		
		if(isset($virt_resp['done'])){
			$theme .= virt_done('IP settings have been saved. Your IP settings will be changed when the VPS is booted again');
		}else{
			$virt_errors[] = 'There was an error while setting up the IPs';
			$theme .= virt_error($virt_errors);
		}
	}else{
		$virt_resp = Virtualizor_Curl::action($params, 'act=ips&');
	}
	
	foreach($virt_resp['ips'] as $k => $v){
		if(valid_ipv6($v)){
			$ipv6[] = $v;
		}else{
			$ips[] = $v;
		}
	}
	
	$theme .= '
		<form method="post" action="">
			<table cellpadding="8" cellspacing="1" border="0" width="100%" class="divroundshad">
				<tr>
					<td colspan="2"><div class="roundheader">IPs</div></td>
				</tr>
				<tr>
					<td align="right">Select Primary IP : </td>
					<td>
						<select name="ips">';
						foreach($ips as $k => $v){
							$theme .= '<option value="'.$v.'"'.(($v == $_POST['ips']) ? 'selected="selected"' : '').'>'.$v.'</option>';
						}
						$theme .= '</select>
					</td>
				</tr>
				<tr>
					<td align="center" colspan="2">
						<input type="submit" value="Set Primary IP" name="reorderips" />
					</td>
				</tr>
			</table>
		</form><br />';
	
	return $theme;
	
}

/*
function r_print($re){
	echo '<pre>';
	print_r($re);
	echo '</pre>';	
}
function died(){
	print_r(error_get_last());
}

register_shutdown_function('died');
*/

?>