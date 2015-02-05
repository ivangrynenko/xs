<link rel="stylesheet" type="text/css" href="templates/orderforms/singlepage/style.css" />

<div id="productconfigcontainer">

{if $editconfig}
<form method="post" action="{$smarty.server.PHP_SELF}?a=confproduct&i={$i}" name="productconfig" target="act_wnd" onsubmit="document.getElementById('loadinggraphic').style.display='block'">
<input type="hidden" name="configure" value="true">
{else}
<form method="post" action="{$smarty.server.PHP_SELF}?a=add&pid={$pid}" name="productconfig" target="act_wnd" onsubmit="document.getElementById('loadinggraphic').style.display='block'">
<input type="hidden" name="configure" value="true">
{/if}

{if $errormessage}<div class="errorbox">{$errormessage|replace:'<li>':' &nbsp;#&nbsp; '} &nbsp;#&nbsp; </div><br />{/if}

<div class="seperatorbar">{$LANG.orderbillingcycle}</div>

<input type="hidden" name="previousbillingcycle" value="{$billingcycle}" />
<p>{if $pricing.type eq "free"}
<input type="hidden" name="billingcycle" value="free" />
{$LANG.orderfree}
{elseif $pricing.type eq "onetime"}
<input type="hidden" name="billingcycle" value="onetime" />
{$pricing.onetime} {$LANG.orderpaymenttermonetime}
{else}
<select name="billingcycle" onchange="submit()">
{if $pricing.monthly}<option value="monthly"{if $billingcycle eq "monthly"} selected="selected"{/if}>{$pricing.monthly}</option>{/if}
{if $pricing.quarterly}<option value="quarterly"{if $billingcycle eq "quarterly"} selected="selected"{/if}>{$pricing.quarterly}</option>{/if}
{if $pricing.semiannually}<option value="semiannually"{if $billingcycle eq "semiannually"} selected="selected"{/if}>{$pricing.semiannually}</option>{/if}
{if $pricing.annually}<option value="annually"{if $billingcycle eq "annually"} selected="selected"{/if}>{$pricing.annually}</option>{/if}
{if $pricing.biennially}<option value="biennially"{if $billingcycle eq "biennially"} selected="selected"{/if}>{$pricing.biennially}</option>{/if}
{if $pricing.triennially}<option value="triennially"{if $billingcycle eq "triennially"} selected="selected"{/if}>{$pricing.triennially}</option>{/if}
</select>
{/if}</p>

{if $productinfo.type eq "server"}
<div class="seperatorbar">{$LANG.cartconfigserver}</div>
<br />
<div class="cartbox">
{$LANG.serverhostname}: <input type="text" name="hostname" size="15" value="{$server.hostname}" /> eg. server1(.yourdomain.com)<br />
{$LANG.serverns1prefix}: <input type="text" name="ns1prefix" size="10" value="{$server.ns1prefix}" /> eg. ns1(.yourdomain.com)<br />
{$LANG.serverns2prefix}: <input type="text" name="ns2prefix" size="10" value="{$server.ns2prefix}" /> eg. ns2(.yourdomain.com)<br />
{$LANG.serverrootpw}: <input type="password" name="rootpw" size="20" value="{$server.rootpw}" />
</div>
{/if}

{if $configurableoptions}
<div class="seperatorbar">{$LANG.orderconfigpackage}</div>
<p>{$LANG.cartconfigoptionsdesc}</p>
<table cellspacing="0" cellpadding="0">
{foreach key=num item=configoption from=$configurableoptions}
<tr><td>{$configoption.optionname}:</td><td width="5"></td><td>
{if $configoption.optiontype eq 1}
<select name="configoption[{$configoption.id}]">
{foreach key=num2 item=options from=$configoption.options}
<option value="{$options.id}"{if $configoption.selectedvalue eq $options.id} selected="selected"{/if}>{$options.name}</option>
{/foreach}
</select>
{elseif $configoption.optiontype eq 2}
{foreach key=num2 item=options from=$configoption.options}
<input type="radio" name="configoption[{$configoption.id}]" value="{$options.id}"{if $configoption.selectedvalue eq $options.id} checked="checked"{/if}> {$options.name}<br />
{/foreach}
{elseif $configoption.optiontype eq 3}
<input type="checkbox" name="configoption[{$configoption.id}]" value="1"{if $configoption.selectedqty} checked{/if}> {$configoption.options.0.name}
{elseif $configoption.optiontype eq 4}
<input type="text" name="configoption[{$configoption.id}]" value="{$configoption.selectedqty}" size="5"> x {$configoption.options.0.name}
{/if}
</td></tr>
{/foreach}
</table>
<br />
{/if}

{if $addons}
<div class="seperatorbar">{$LANG.cartaddons}</div>
<br />
<table>
{foreach key=num item=addon from=$addons}
<tr><td>{$addon.checkbox}</td><td><label for="a{$addon.id}"><strong>{$addon.name}</strong> - {$addon.description} ({$addon.pricing})</label></td></tr>
{/foreach}
</table>
<br />
{/if}

{if $customfields}
<div class="seperatorbar">{$LANG.orderadditionalrequiredinfo}</div>
<p>{$LANG.cartcustomfieldsdesc}</p>
{foreach key=num item=customfield from=$customfields}
{$customfield.name}: {$customfield.input} {$customfield.description}<br />
{/foreach}
{/if}

{if $domainoption}

{if $domains}
<input type="hidden" name="domainoption" value="{$domainoption}" />
{foreach key=num item=domain from=$domains}
<input type="hidden" name="domains[]" value="{$domain.domain}" />
<input type="hidden" name="domainsregperiod[{$domain.domain}]" value="{$domain.regperiod}" />
{/foreach}
{/if}

{/if}

<p align="center">{if $editconfig}<input type="submit" value="{$LANG.updatecart}" id="continuebut4" />{else}<input type="submit" value="{$LANG.ordercontinuebutton}" id="continuebut4" />{/if}</p>

</form>

</div>
<div id="domainchoose"></div>
<div id="productconfig"></div>
<div id="domainconfig"></div>
<div id="ordersummary"></div>

<div id="loadinggraphic" align="center" style="display:none;"><img src="images/loading.gif" /></div>
<iframe src="images/logo.jpg" id="act_wnd" name="act_wnd" style="display:none;"></iframe>

<script language="javascript" type="text/javascript">
{literal}
if(window.parent!=window.self){
	window.parent.document.getElementById("productconfig").innerHTML = document.getElementById("productconfigcontainer").innerHTML;
	window.parent.document.getElementById("loadinggraphic").style.display="none";
	if (window.parent.document.forms['productchoose']) {
	    var els = window.parent.document.forms['productchoose'].elements;
	    for(var i=0;i<els.length;i++) {
	        if (els[i].type=="submit") {
	            els[i].style.display="none";
	        } else {
	            els[i].disabled=true;
	        }
	    }
	}
	if (window.parent.document.forms['domainchoose']) {
	    var els = window.parent.document.forms['domainchoose'].elements;
	    for(var i=0;i<els.length;i++) {
	        if (els[i].type=="submit") {
	            els[i].style.display="none";
	        } else {
	            els[i].disabled=true;
	        }
	    }
	}
}
{/literal}
</script>