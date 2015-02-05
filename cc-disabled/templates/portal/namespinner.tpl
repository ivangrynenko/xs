{if $domain neq "transfer"}
	{php}
		/**
		 * @version 1.4
		 * @since WHMCS 4.0
		 * @copyright 2009 http://ideamesh.com This script is owned and licensed by Ideamesh, Inc.
		 * You may use this script in commercial applications, however you may not resell
		 * this script without permission of Ideamesh, Inc.<br>
		 * Please contact (sales [at] ideamesh [dot] com) for more information
		 */

		//Set to true to get debug messages
		$debug = false;

		//Max number of spins to generate
		$maxspins = 20;

		//Choose which options to show in the spin results
		//Enom only will return .com, .net, .cc and .tv results that's why these were chosen
		$showdotcom = true;
		$showdotnet = true;
		$showdotcc = false;
		$showdottv = false;

		//Can be one of these values ("tld"|"sld"|"score")
		$sort1 = "score";
		$sort2 = "sld";
		//Can be one of these values (SORT_ASC|SORT_DESC)
		$sort1order = SORT_DESC;
		$sort2order = SORT_ASC;

		//Set this to "True" for blocking sensitive content or "False" to allow sensitive content
		$sensitivecontent = "True";

		// Extract Smarty variables
		extract($this->_tpl_vars);
		
		//Check for domainchecker.php vs cart
		if ($filename == 'domainchecker') {
			$tld = $ext;
			$sld = $domain;
			$this->assign('outputplace', 'domainchecker');
		} else {
			$this->assign('outputplace', 'cart');
		}

		//Pull ENOM login info from the database - *thanks MACscr*
		$query = "SELECT setting,value FROM `tblregistrars` WHERE registrar='enom' AND (setting='Username' OR setting='Password')";
		$result = mysql_query($query) or die(mysql_error());  
		while($row = @mysql_fetch_array( $result )) {
			$setting = $row['setting'];
			$enom[$setting] = $row['value'];
		}
		$enomid = decrypt($enom['Username']);
		$enompw = decrypt($enom['Password']);

		//Do not edit this. We're setting up the URL to retrieve the spins
		$namespinnerurl = "https://reseller.enom.com/interface.asp?command=namespinner&uid=".$enomid."&pw=".$enompw."&SLD=".$sld."&TLD=".$tld."&SensitiveContent=".$sensitivecontent."&MaxResults=".$maxspins."&ResponseType=XML";

		// Use cURL to get the XML response
		$ch = curl_init($namespinnerurl);
		curl_setopt($ch, CURLOPT_HEADER, false);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
		//curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
		$xml = curl_exec($ch);
		$curlerror = "ErrNo: ".curl_errno($ch)." ErrMsg: ".curl_error($ch);
		curl_close($ch);

		if ($xml) {
			$spinnerresults = new SimpleXmlElement($xml, LIBXML_NOCDATA);

			if ($spinnerresults->ErrCount == 0) {
				for ($i=0; $i<$maxspins; $i++) {
					if ($showdotcom && (string)$spinnerresults->namespin->domains->domain[$i]['com'] == "y")
						$spinner[] = array(
							'domain' => (string)$spinnerresults->namespin->domains->domain[$i]['name'].".com",
							'netscore' => (int)$spinnerresults->namespin->domains->domain[$i]['comscore'],
							'tld' => '.com');
					if ($showdotnet && (string)$spinnerresults->namespin->domains->domain[$i]['net'] == "y")
						$spinner[] = array(
							'domain' => (string)$spinnerresults->namespin->domains->domain[$i]['name'].".net",
							'netscore' => (int)$spinnerresults->namespin->domains->domain[$i]['netscore'],
							'tld' => '.net');
					if ($showdotcc && (string)$spinnerresults->namespin->domains->domain[$i]['cc'] == "y")
						$spinner[] = array(
							'domain' => (string)$spinnerresults->namespin->domains->domain[$i]['name'].".cc",
							'netscore' => (int)$spinnerresults->namespin->domains->domain[$i]['ccscore'],
							'tld' => '.cc');
					if ($showdottv && (string)$spinnerresults->namespin->domains->domain[$i]['tv'] == "y")
						$spinner[] = array(
							'domain' => (string)$spinnerresults->namespin->domains->domain[$i]['name'].".tv",
							'netscore' => (int)$spinnerresults->namespin->domains->domain[$i]['tvscore'],
							'tld' => '.tv');
				}
				$gotnamespinner = true;
			} else {
				if ($debug) echo $spinnerresults->errors->Err1;
				$gotnamespinner = false;
			}
		} else {
			if ($debug) echo "Cannot retrieve XML file. Please check your firewall settings.<br />".$curlerror;
			$gotnamespinner = false;
		}

		if ($debug) {
			echo "<pre>";
			echo htmlentities($xml);
			echo "</pre>";
		}

		//Get domain pricing for .com, .net, .cc and .tv
		if ($gotnamespinner) {
			$sql = "SELECT tdp.extension, msetupfee year1, qsetupfee year2, ssetupfee year3, asetupfee year4, bsetupfee year5, monthly year6, quarterly year7, semiannually year8, annually year9, biennially year10
					FROM tbldomainpricing tdp, tblpricing tp
					WHERE tdp.extension in ('.com', '.net', '.cc', '.tv')
					AND tp.type = 'domainregister'
					AND tp.relid = tdp.id
					AND tp.currency = ".$currency['id'];
			$query = mysql_query ($sql);
			while ($row = @mysql_fetch_array ($query, MYSQL_ASSOC)) {
				if ($row['year1'] != 0) $domainprices[$row['extension']][1] = $row['year1'];
				if ($row['year2'] != 0) $domainprices[$row['extension']][2] = $row['year2'];
				if ($row['year3'] != 0) $domainprices[$row['extension']][3] = $row['year3'];
				if ($row['year4'] != 0) $domainprices[$row['extension']][4] = $row['year4'];
				if ($row['year5'] != 0) $domainprices[$row['extension']][5] = $row['year5'];
				if ($row['year6'] != 0) $domainprices[$row['extension']][6] = $row['year6'];
				if ($row['year7'] != 0) $domainprices[$row['extension']][7] = $row['year7'];
				if ($row['year8'] != 0) $domainprices[$row['extension']][8] = $row['year8'];
				if ($row['year9'] != 0) $domainprices[$row['extension']][9] = $row['year9'];
				if ($row['year10'] != 0) $domainprices[$row['extension']][10] = $row['year10'];
			}
		}

		// Setup for the sorting
		foreach ($spinner as $key => $row) {
			$spin['sld'][$key]  = $row['domain'];
			$spin['score'][$key] = $row['netscore'];
			$spin['tld'][$key] = $row['tld'];
		}

		array_multisort($spin[$sort1], $sort1order, $spin[$sort2], $sort2order, $spinner);

		//Send some variables back to the template
		$this->assign('spinner', $spinner);
		$this->assign('gotnamespinner', $gotnamespinner);
		$this->assign('domainprices', $domainprices);
	{/php}

	{if $gotnamespinner && $outputplace == 'cart'}
		<p class="cartsubheading">We also recommend</p>

		<table style="width:100%;" align="center" cellspacing="0" cellpadding="10" class="data">
		<tr><th>{$LANG.domainname}</th><th>{$LANG.domainstatus}</th><th>{$LANG.domainmoreinfo}</th></tr>
		{foreach key=num item=result from=$spinner}
			<tr>
				<td>{$result.domain}</td>
				<td><input type="checkbox" name="domains[]" value="{$result.domain|lower}"{if $result.domain|lower|in_array:$domains} checked{/if} /> {$LANG.domainavailable}</td>
				<td><select name="domainsregperiod[{$result.domain|lower}]" style="width:175px;">
					{assign var=thistld value=$result.tld}
					{foreach key=years item=regoption from=$domainprices.$thistld}
						<option value="{$years}">{$years} {$LANG.orderyears} @ {$currency.prefix}{$regoption} {$currency.code}</option>
					{/foreach}</select>
				</td>
			</tr>
		{/foreach}
		</table>

		<p align="center"><input type="submit" value="{$LANG.addtocart}" /></p>
	{elseif $outputplace == 'domainchecker'}
		<p><strong>We also recommend</strong></p>
			<table class="clientareatable" cellspacing="1">
			<tr class="clientareatableheading">
				<td width="20"></td><td>{$LANG.domainname}</td><td>{$LANG.domainstatus}</td><td>{$LANG.domainmoreinfo}</td></tr>
				{foreach key=num item=result from=$spinner}
					<tr class="clientareatableactive">
						<td><input type="checkbox" name="domains[]" value="{$result.domain}"/>
							<input type="hidden" name="domainsregperiod[{$result.domain}]" value="{$result.period}" />
						</td>
						<td>{$result.domain}</td>
						<td class="textgreen">{$LANG.domainavailable}</td>
						<td>
							<select name="domainsregperiod[{$result.domain|lower}]">
							{assign var=thistld value=$result.tld}
							{foreach key=years item=regoption from=$domainprices.$thistld}
								<option value="{$years}">{$years} {$LANG.orderyears} @ {$currency.prefix}{$regoption} {$currency.code}</option>
							{/foreach}
							</select>
						</td>
					</tr>
				{/foreach}
		</table>
		<p align="center"><input type="submit" value="{$LANG.ordernowbutton} >>" /></p>
	{/if}
{/if}