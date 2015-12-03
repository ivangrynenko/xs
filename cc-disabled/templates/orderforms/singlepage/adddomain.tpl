<link rel="stylesheet" type="text/css" href="templates/orderforms/singlepage/style.css" />

<div class="seperatorbar">{$LANG.ordercategories}</div>

<p>{foreach key=num item=productgroup from=$productgroups}<input type="radio" name="gid" value="{$productgroup.gid}" onclick="window.location='{$smarty.server.PHP_SELF}?gid={$productgroup.gid}'" /> {$productgroup.name}{/foreach}
{if $loggedin}<input type="radio" onclick="window.location='{$smarty.server.PHP_SELF}?gid=addons'"{if $smarty.get.gid eq "addons"} checked{/if} /> {$LANG.cartproductaddons}{/if}
{if $registerdomainenabled}<input type="radio" onclick="window.location='{$smarty.server.PHP_SELF}?a=add&domain=register'"{if $domain eq "register"} checked{/if} /> {$LANG.registerdomain}{/if}
{if $transferdomainenabled}<input type="radio" onclick="window.location='{$smarty.server.PHP_SELF}?a=add&domain=transfer'"{if $domain eq "transfer"} checked{/if} /> {$LANG.transferdomain}{/if}</p>

<div class="seperatorbar">{$LANG.domainname}</div>

{if $errormessage}<div class="errorbox">{$errormessage|replace:'<li>':' &nbsp;#&nbsp; '} &nbsp;#&nbsp; </div><br />{/if}

<form method="post" action="{$smarty.server.PHP_SELF}?a=add&domain={$domain}">

<p align="center">www. <input type="text" name="sld" size="40" value="{$sld}" /> <select name="tld">
{foreach key=num item=listtld from=$tlds}
<option value="{$listtld}"{if $listtld eq $tld} selected="selected"{/if}>{$listtld}</option>
{/foreach}
</select></p>

<p align="center"><input type="submit" value="{$LANG.checkavailability}" id="continuebut2" /></p>

</form>

{if $availabilityresults}

<form method="post" action="{$smarty.server.PHP_SELF}?a=add&domain={$domain}" name="domainorderonly" target="act_wnd" onsubmit="document.getElementById('loadinggraphic').style.display='block'">

<table class="clientareatable" style="width:90%;" align="center" cellspacing="1">
<tr class="clientareatableheading"><td>{$LANG.domainname}</td><td>{$LANG.domainstatus}</td><td>{$LANG.domainmoreinfo}</td></tr>
{foreach key=num item=result from=$availabilityresults}
<tr class="clientareatableactive"><td>{$result.domain}</td><td class="{if $result.status eq $searchvar}textgreen{else}textred{/if}">{if $result.status eq $searchvar}<input type="checkbox" name="domains[]" value="{$result.domain}"{if $result.domain|in_array:$domains} checked{/if} /> {$LANG.domainavailable}{else}{$LANG.domainunavailable}{/if}</td><td>{if $result.regoptions}<select name="domainsregperiod[{$result.domain}]">{foreach key=period item=regoption from=$result.regoptions}{if $regoption.$domain}<option value="{$period}">{$period} {$LANG.orderyears} @ {$regoption.$domain}</option>{/if}{/foreach}</select>{/if}</td></tr>
{/foreach}
</table>

{if $domain eq "transfer" && $eppcode}
<p align="center"><strong>{$LANG.domaineppcode}:</strong> <input type="text" name="eppcode" size="20" /> [Required]*<br />{$LANG.domaineppcodedesc}</p>
{/if}

<p align="center"><input type="submit" value="{$LANG.ordercontinuebutton}" /></p>

{/if}

</form>

<div id="domainchoose"></div>
<div id="productconfig"></div>
<div id="domainconfig"></div>
<div id="ordersummary"></div>

<div id="loadinggraphic" align="center" style="display:none;"><img src="images/loading.gif" /></div>

<iframe src="images/logo.jpg" id="act_wnd" name="act_wnd" style="display:none;"></iframe>