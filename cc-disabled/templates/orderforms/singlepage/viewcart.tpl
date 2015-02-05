<link rel="stylesheet" type="text/css" href="templates/orderforms/singlepage/style.css" />

<div id="ordersummarycontainer">

{if $errormessage}<div class="errorbox">{$errormessage|replace:'<li>':' &nbsp;#&nbsp; '} &nbsp;#&nbsp; </div><br />{elseif $promotioncode && $rawdiscount eq "0.00"}<div class="errorbox">{$LANG.promoappliedbutnodiscount}</div><br />{/if}

<div class="seperatorbar">{$LANG.ordersummary}</div>

<p class="ordersummary">{$LANG.ordersubtotal}: {$subtotal}<br />
{if $promotioncode}{$promotiondescription}: {$discount}<br />{/if}
{if $taxrate}{$taxname} @ {$taxrate}%: {$taxtotal}<br />{/if}
{if $taxrate2}{$taxname2} @ {$taxrate2}%: {$taxtotal2}<br />{/if}
<b>{$LANG.ordertotalduetoday}: {$total}</b><br /><br />
{if $totalrecurringmonthly || $totalrecurringquarterly || $totalrecurringsemiannually || $totalrecurringannually || $totalrecurringbiennially || $totalrecurringtriennially}
{$LANG.ordertotalrecurring}: {if $totalrecurringmonthly}{$totalrecurringmonthly} {$LANG.orderpaymenttermmonthly}, {/if}
{if $totalrecurringquarterly}{$totalrecurringquarterly} {$LANG.orderpaymenttermquarterly}, {/if}
{if $totalrecurringsemiannually}{$totalrecurringsemiannually} {$LANG.orderpaymenttermsemiannually}, {/if}
{if $totalrecurringannually}{$totalrecurringannually} {$LANG.orderpaymenttermannually}, {/if}
{if $totalrecurringbiennially}{$totalrecurringbiennially} {$LANG.orderpaymenttermbiennially}, {/if}
{if $totalrecurringtriennially}{$totalrecurringtriennially} {$LANG.orderpaymenttermtriennially}, {/if}{/if}
</p>

<form method="post" action="{$smarty.server.PHP_SELF}?a=view" target="act_wnd" onsubmit="document.getElementById('loadinggraphic').style.display='block'">
<input type="hidden" name="validatepromo" value="true" />

<p align="center"><strong>{$LANG.orderpromotioncode}</strong> {if $promotioncode}{$promotioncode} - {$promotiondescription} <a href="{$smarty.server.PHP_SELF}?a=removepromo" target="act_wnd">{$LANG.orderdontusepromo}</a>{else}<input type="text" name="promocode" size="20" /> <input type="submit" value="{$LANG.orderpromovalidatebutton}" />{/if}</p>

</form>

<form method="post" action="{$smarty.server.PHP_SELF}?a=checkout" name="orderfrm">
<input type="hidden" name="submit" value="true" />

<div class="seperatorbar">{$LANG.yourdetails}</div>

{if !$loggedin}<p align="center"><strong>{$LANG.alreadyregistered}</strong> <a href="{$smarty.server.PHP_SELF}?a=login">{$LANG.clickheretologin}</a></p>{else}<br />{/if}

<table cellspacing="1" cellpadding="0" class="frame"><tr><td width="50%" valign="top">

<table width="100%" cellpadding="2">
<tr><td width="100" class="fieldarea">{$LANG.clientareafirstname}</td><td>{if $loggedin}{$clientsdetails.firstname}{else}<input type="text" name="firstname" style="width:80%;" value="{$clientsdetails.firstname}" />{/if}</td></tr>
<tr><td class="fieldarea">{$LANG.clientarealastname}</td><td>{if $loggedin}{$clientsdetails.lastname}{else}<input type="text" name="lastname" style="width:80%;" value="{$clientsdetails.lastname}" />{/if}</td></tr>
<tr><td class="fieldarea">{$LANG.clientareacompanyname}</td><td>{if $loggedin}{$clientsdetails.companyname}{else}<input type="text" name="companyname" style="width:80%;" value="{$clientsdetails.companyname}" />{/if}</td></tr>
<tr><td class="fieldarea"{if !$loggedin} style="height:21px;"{/if}><br /></td><td></td></tr>
<tr><td class="fieldarea">{$LANG.clientareaemail}</td><td>{if $loggedin}{$clientsdetails.email}{else}<input type="text" name="email" style="width:90%;" value="{$clientsdetails.email}" />{/if}</td></tr>
{if $loggedin}
<tr><td class="fieldarea"><br /></td><td></td></tr>
<tr><td class="fieldarea"><br /></td><td></td></tr>
{else}
<tr><td class="fieldarea">{$LANG.clientareapassword}</td><td><input type="password" name="password" id="newpw" size="20" /></td></tr>
<tr><td class="fieldarea">{$LANG.clientareaconfirmpassword}</td><td><input type="password" name="password2" size="20" /></td></tr>
{/if}
</table>

</td><td width="50%" valign="top">

<table width="100%" cellpadding="2">
<tr><td width="100" class="fieldarea">{$LANG.clientareaaddress1}</td><td>{if $loggedin}{$clientsdetails.address1}{else}<input type="text" name="address1" style="width:80%;" value="{$clientsdetails.address1}" />{/if}</td></tr>
<tr><td class="fieldarea">{$LANG.clientareaaddress2}</td><td>{if $loggedin}{$clientsdetails.address2}{else}<input type="text" name="address2" style="width:80%;" value="{$clientsdetails.address2}" />{/if}</td></tr>
<tr><td class="fieldarea">{$LANG.clientareacity}</td><td>{if $loggedin}{$clientsdetails.city}{else}<input type="text" name="city" style="width:80%;" value="{$clientsdetails.city}" />{/if}</td></tr>
<tr><td class="fieldarea">{$LANG.clientareastate}</td><td>{if $loggedin}{$clientsdetails.state}{else}<input type="text" name="state" style="width:80%;" value="{$clientsdetails.state}" />{/if}</td></tr>
<tr><td class="fieldarea">{$LANG.clientareapostcode}</td><td>{if $loggedin}{$clientsdetails.postcode}{else}<input type="text" name="postcode" size="15" value="{$clientsdetails.postcode}" />{/if}</td></tr>
<tr><td class="fieldarea">{$LANG.clientareacountry}</td><td>{if $loggedin}{$clientsdetails.country}{else}{$clientcountrydropdown}{/if}</td></tr>
<tr><td class="fieldarea">{$LANG.clientareaphonenumber}</td><td>{if $loggedin}{$clientsdetails.phonenumber}{else}<input type="text" name="phonenumber" size="20" value="{$clientsdetails.phonenumber}" />{/if}</td></tr>
</table>

</td></tr></table>

{if $customfields || $securityquestions}
<table cellspacing="1" cellpadding="0" class="frame"><tr><td>
<table width="100%" cellpadding="2">
{if $securityquestions && !$loggedin}
<tr><td width="200" class="fieldarea">{$LANG.clientareasecurityquestion}</td><td><select name="securityqid">
{foreach key=num item=question from=$securityquestions}
	<option value={$question.id}>{$question.question}</option>
{/foreach}
</select></td></tr>
<tr><td class="fieldarea">{$LANG.clientareasecurityanswer}</td><td><input type="password" name="securityqans" size="30"></td></tr>
{/if}
{foreach key=num item=customfield from=$customfields}
<tr><td width="200" class="fieldarea">{$customfield.name}</td><td>{$customfield.input} {$customfield.description}</td></tr>
{/foreach}
</table>
</td></tr></table>
{/if}

{if $taxenabled && !$loggedin}
<p align="center">{$LANG.carttaxupdateselections} <input type="submit" value="{$LANG.carttaxupdateselectionsupdate}" name="updateonly" /></p>
{else}
<br />
{/if}

{if $domainsinorder}
<div class="seperatorbar">{$LANG.domainregistrantinfo}</div>
<br />
{if $addcontact}
<input type="hidden" name="contact" value="addingnew" />
<table cellspacing="1" cellpadding="0" class="frame"><tr><td width="50%" valign="top">
<table width="100%" cellpadding="2">
<tr><td width="100" class="fieldarea">{$LANG.clientareafirstname}</td><td><input type="text" name="domaincontactfirstname" style="width:80%;" value="{$domaincontact.firstname}" /></td></tr>
<tr><td class="fieldarea">{$LANG.clientarealastname}</td><td><input type="text" name="domaincontactlastname" style="width:80%;" value="{$domaincontact.lastname}" /></td></tr>
<tr><td class="fieldarea">{$LANG.clientareacompanyname}</td><td><input type="text" name="domaincontactcompanyname" style="width:80%;" value="{$domaincontact.companyname}" /></td></tr>
<tr><td class="fieldarea" style="height:21px;"><br /></td><td></td></tr>
<tr><td class="fieldarea">{$LANG.clientareaemail}</td><td><input type="text" name="domaincontactemail" style="width:90%;" value="{$domaincontact.email}" /></td></tr>
<tr><td class="fieldarea">{$LANG.clientareaphonenumber}</td><td><input type="text" name="domaincontactphonenumber" size="20" value="{$domaincontact.phonenumber}" /></td></tr>
</table>
</td><td width="50%" valign="top">
<table width="100%" cellpadding="2">
<tr><td width="100" class="fieldarea">{$LANG.clientareaaddress1}</td><td><input type="text" name="domaincontactaddress1" style="width:80%;" value="{$domaincontact.address1}" /></td></tr>
<tr><td class="fieldarea">{$LANG.clientareaaddress2}</td><td><input type="text" name="domaincontactaddress2" style="width:80%;" value="{$domaincontact.address2}" /></td></tr>
<tr><td class="fieldarea">{$LANG.clientareacity}</td><td><input type="text" name="domaincontactcity" style="width:80%;" value="{$domaincontact.city}" /></td></tr>
<tr><td class="fieldarea">{$LANG.clientareastate}</td><td><input type="text" name="domaincontactstate" style="width:80%;" value="{$domaincontact.state}" /></td></tr>
<tr><td class="fieldarea">{$LANG.clientareapostcode}</td><td><input type="text" name="domaincontactpostcode" size="15" value="{$domaincontact.postcode}" /></td></tr>
<tr><td class="fieldarea">{$LANG.clientareacountry}</td><td>{$domaincontactcountrydropdown}</td></tr>
</table>
</td></tr></table>
{else}
<p>{$LANG.domainregistrantchoose}: <select name="contact">
<option value="">{$LANG.usedefaultcontact}</option>
{foreach key=num item=domaincontact from=$domaincontacts}
<option value="{$domaincontact.id}">{$domaincontact.name}</option>
{/foreach}
<option value="new">{$LANG.clientareanavaddcontact}...</option>
</select><br /></p>
{/if}
{/if}

<br />

<div class="seperatorbar">{$LANG.orderpaymentmethod}</div>
<p align="center">{foreach key=num item=gateway from=$gateways}<input type="radio" name="paymentmethod" value="{$gateway.sysname}" id="pgbtn{$num}"{if $selectedgateway eq $gateway.sysname} checked{/if} /><label for="pgbtn{$num}">{$gateway.name}</label> {/foreach}</p>

{if $shownotesfield}
<div class="seperatorbar">{$LANG.ordernotes}</div>
<p align="center"><textarea name="notes" rows="4" cols="100" onFocus="if(this.value=='{$LANG.ordernotesdescription}'){ldelim}this.value='';{rdelim}" onBlur="if (this.value==''){ldelim}this.value='{$LANG.ordernotesdescription}';{rdelim}">{$notes}</textarea></p>
{/if}

{if $accepttos}
<p align="center"><input type="checkbox" name="accepttos" id="accepttos" /> <label for="accepttos">{$LANG.ordertosagreement} <a href="{$tosurl}" target="_blank">{$LANG.ordertos}</a></label><p>
{/if}

<p align="center"><input type="button" value="{$LANG.orderstartover}" onclick="window.location='cart.php?a=startover'" /> <input type="submit" value="{$LANG.completeorder}"{if $cartitems==0} disabled{/if} onclick="this.value='{$LANG.pleasewait}'" /></p>

<p><img align="left" src="images/padlock.gif" border="0" vspace="5" alt="Secure Transaction" style="padding-right: 10px;" /> {$LANG.ordersecure} (<strong>{$ipaddress}</strong>) {$LANG.ordersecure2}</p>

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

	window.parent.document.getElementById("ordersummary").innerHTML = document.getElementById("ordersummarycontainer").innerHTML;
	window.parent.document.getElementById("loadinggraphic").style.display="none";
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
	if (window.parent.document.forms['domainconfig']) {
	    var els = window.parent.document.forms['domainconfig'].elements;
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