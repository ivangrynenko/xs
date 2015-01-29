<div id="domainconfigcontainer">

<div class="seperatorbar">{$LANG.cartdomainsconfig}</div>

<p>{$LANG.cartdomainsconfigdesc}</p>

{if $errormessage}<div class="errorbox">{$errormessage|replace:'<li>':' &nbsp;#&nbsp; '} &nbsp;#&nbsp; </div><br />{/if}

<form method="post" action="{$smarty.server.PHP_SELF}?a=confdomains" name="domainconfig" target="act_wnd" onsubmit="document.getElementById('loadinggraphic').style.display='block'">
<input type="hidden" name="update" value="true" />

{foreach key=num item=domain from=$domains}
<p>
<strong>&raquo; {$domain.domain}</strong><br />
{if $domain.configtoshow}
{if $domain.eppenabled}{$LANG.domaineppcode} <input type="text" name="epp[{$num}]" size="20" value="{$domain.eppvalue}" /> {$LANG.domaineppcodedesc}<br />{/if}
{if $domain.dnsmanagement}<input type="checkbox" name="dnsmanagement[{$num}]"{if $domain.dnsmanagementselected} checked{/if} /> {$LANG.domaindnsmanagement} ({$domain.dnsmanagementprice})<br />{/if}
{if $domain.emailforwarding}<input type="checkbox" name="emailforwarding[{$num}]"{if $domain.emailforwardingselected} checked{/if} /> {$LANG.domainemailforwarding} ({$domain.emailforwardingprice})<br />{/if}
{if $domain.idprotection}<input type="checkbox" name="idprotection[{$num}]"{if $domain.idprotectionselected} checked{/if} /> {$LANG.domainidprotection} ({$domain.idprotectionprice})<br />{/if}
{foreach key=domainfieldname item=domainfield from=$domain.fields}
{$domainfieldname}: {$domainfield}<br />
{/foreach}
</p>
{/if}
{/foreach}

{if $atleastonenohosting}
<div class="seperatorbar">{$LANG.domainnameservers}</div>
<p>{$LANG.cartnameserversdesc}</p>
<div class="cartbox">
<table>
<tr><td>{$LANG.domainnameserver1}:</td><td><input type="text" name="domainns1" size="40" value="{$domainns1}" /></td></tr>
<tr><td>{$LANG.domainnameserver2}:</td><td><input type="text" name="domainns2" size="40" value="{$domainns2}" /></td></tr>
<tr><td>{$LANG.domainnameserver3}:</td><td><input type="text" name="domainns3" size="40" value="{$domainns3}" /></td></tr>
<tr><td>{$LANG.domainnameserver4}:</td><td><input type="text" name="domainns4" size="40" value="{$domainns4}" /></td></tr>
</table>
</div>
{/if}

<p align="center"><input type="submit" value="{$LANG.ordercontinuebutton}" /></p>

</form>

</div>

<script language="javascript" type="text/javascript">
window.parent.document.getElementById("domainconfig").innerHTML = document.getElementById("domainconfigcontainer").innerHTML;
window.parent.document.getElementById("loadinggraphic").style.display="none";
{literal}
if (window.parent.document.forms['domainorderonly']) {
    var els = window.parent.document.forms['domainorderonly'].elements;
    for(var i=0;i<els.length;i++) {
        if (els[i].type=="submit") {
            els[i].style.display="none";
        } else {
            els[i].disabled=true;
        }
    }
}
if (window.parent.document.forms['productconfig']) {
    var els = window.parent.document.forms['productconfig'].elements;
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