<link rel="stylesheet" type="text/css" href="templates/orderforms/singlepage/style.css" />

<div id="domainchoosecontainer">

<div class="seperatorbar">{$LANG.domainname}</div>

<br />

<form method="post" action="{$smarty.server.PHP_SELF}?a=add&pid={$pid}" name="domainchoose" target="act_wnd" onsubmit="document.getElementById('loadinggraphic').style.display='block'">
{foreach from=$passedvariables key=name item=value}
<input type="hidden" name="{$name}" value="{$value}" />
{/foreach}

{if $errormessage}<div class="errorbox">{$errormessage|replace:'<li>':' &nbsp;#&nbsp; '} &nbsp;#&nbsp; </div><br />{/if}

{if $incartdomains}
<input type="radio" name="domainoption" value="incart" id="selincart" onclick="document.getElementById('register').style.display='none';document.getElementById('transfer').style.display='none';document.getElementById('owndomain').style.display='none';document.getElementById('subdomain').style.display='none';document.getElementById('incart').style.display=''"{if $domainoption eq "incart"} checked{/if} /> <label for="selincart">{$LANG.cartproductdomainuseincart}</label><br />
{/if}

{if $registerdomainenabled}
<input type="radio" name="domainoption" value="register" id="selregister" onclick="document.getElementById('register').style.display='';document.getElementById('transfer').style.display='none';document.getElementById('owndomain').style.display='none';document.getElementById('subdomain').style.display='none';document.getElementById('incart').style.display='none'"{if $domainoption eq "register"} checked="checked"{/if} /> <label for="selregister">{$LANG.orderdomainoption1part1} {$companyname} {$LANG.orderdomainoption1part2}</label><br />
{/if}

{if $transferdomainenabled}
<input type="radio" name="domainoption" value="transfer" id="seltransfer" onclick="document.getElementById('register').style.display='none';document.getElementById('transfer').style.display='';document.getElementById('owndomain').style.display='none';document.getElementById('subdomain').style.display='none';document.getElementById('incart').style.display='none'"{if $domainoption eq "transfer"} checked="checked"{/if} /> <label for="seltransfer">{$LANG.orderdomainoption3} {$companyname}</label><br />
{/if}

{if $owndomainenabled}
<input type="radio" name="domainoption" value="owndomain" id="selowndomain" onclick="document.getElementById('register').style.display='none';document.getElementById('transfer').style.display='none';document.getElementById('owndomain').style.display='';document.getElementById('subdomain').style.display='none';document.getElementById('incart').style.display='none'"{if $domainoption eq "owndomain"} checked="checked"{/if} /> <label for="selowndomain">{$LANG.orderdomainoption2}</label><br />
{/if}

{if $subdomain}
<input type="radio" name="domainoption" value="subdomain" id="selsubdomain" onclick="document.getElementById('register').style.display='none';document.getElementById('transfer').style.display='none';document.getElementById('owndomain').style.display='none';document.getElementById('subdomain').style.display='';document.getElementById('incart').style.display='none'"{if $domainoption eq "subdomain"} checked="checked"{/if} /> <label for="selsubdomain">{$LANG.orderdomainoption4}</label><br />
{/if}

<br />

<div id="incart" align="center">{$LANG.cartproductdomainchoose}: <select name="incartdomain">
{foreach key=num item=incartdomain from=$incartdomains}
<option value="{$incartdomain}">{$incartdomain}</option>
{/foreach}
</select>
</div>

<div id="register" align="center">www. <input type="text" name="sld[0]" size="40" value="{$sld}" /> <select name="tld[0]">
{foreach key=num item=listtld from=$registertlds}
<option value="{$listtld}"{if $listtld eq $tld} selected="selected"{/if}>{$listtld}</option>
{/foreach}
</select>
</div>

<div id="transfer" align="center">www. <input type="text" name="sld[1]" size="40" value="{$sld}" /> <select name="tld[1]">
{foreach key=num item=listtld from=$transfertlds}
<option value="{$listtld}"{if $listtld eq $tld} selected="selected"{/if}>{$listtld}</option>
{/foreach}
</select>
</div>

<div id="owndomain" align="center">www. <input type="text" name="sld[2]" size="40" value="{$sld}" /> . <input type="text" name="tld[2]" size="7" value="{$tld}" />
</div>

<div id="subdomain" align="center">http:// <input type="text" name="sld[3]" size="40" value="{$sld}" /> {$subdomain}</div>

<p align="center"><input type="submit" value="{$LANG.checkavailability}" /></p>

{if $availabilityresults}

<div class="seperatorbar">{$LANG.choosedomains}</div>

<br />

<table class="clientareatable" style="width:90%;" align="center" cellspacing="1">
<tr class="clientareatableheading"><td>{$LANG.domainname}</td><td>{$LANG.domainstatus}</td><td>{$LANG.domainmoreinfo}</td></tr>
{foreach key=num item=result from=$availabilityresults}
<tr class="clientareatableactive"><td>{$result.domain}</td><td class="{if $result.status eq $searchvar}textgreen{else}textred{/if}">{if $result.status eq $searchvar}<input type="checkbox" name="domains[]" value="{$result.domain}" /> {$LANG.domainavailable}{else}{$LANG.domainunavailable}{/if}</td><td>{if $result.regoptions}<select name="domainsregperiod[{$result.domain}]">{foreach key=period item=regoption from=$result.regoptions}{if $regoption.$domainoption}<option value="{$period}">{$period} {$LANG.orderyears} @ {$regoption.$domainoption}</option>{/if}{/foreach}</select>{/if}</td></tr>
{/foreach}
</table>

<p align="center"><input type="submit" value="{$LANG.ordercontinuebutton}" /></p>

{/if}

{if $freedomaintlds}* <em>{$LANG.orderfreedomainregistration} {$LANG.orderfreedomainappliesto}: {$freedomaintlds}</em>{/if}

</form>

</div>
<div id="domainchoose"></div>
<div id="productconfig"></div>
<div id="domainconfig"></div>
<div id="ordersummary"></div>

<div id="loadinggraphic" align="center" style="display:none;"><img src="images/loading.gif" /></div>
<iframe src="images/logo.jpg" id="act_wnd" name="act_wnd" style="display:none;"></iframe>

<script language="javascript" type="text/javascript">
document.getElementById('incart').style.display='none';
document.getElementById('register').style.display='none';
document.getElementById('transfer').style.display='none';
document.getElementById('owndomain').style.display='none';
document.getElementById('subdomain').style.display='none';
document.getElementById('sel{$domainoption}').checked='true';
document.getElementById('{$domainoption}').style.display='';
{literal}
if(window.parent!=window.self){
    if(window.parent.document.getElementById("domainchoosecontainer")){
		window.parent.document.getElementById("domainchoosecontainer").innerHTML = document.getElementById("domainchoosecontainer").innerHTML;
	}else{
		window.parent.document.getElementById("domainchoose").innerHTML = document.getElementById("domainchoosecontainer").innerHTML;
	}
	window.parent.document.getElementById("loadinggraphic").style.display="none";
	var els = window.parent.document.forms['productchoose'].elements;
	for(var i=0;i<els.length;i++) {
	    if (els[i].type=="submit") {
	    els[i].style.display="none";
	    } else {
	        els[i].disabled=true;
	    }
	}
}
{/literal}
</script>
