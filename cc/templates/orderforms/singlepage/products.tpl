<link rel="stylesheet" type="text/css" href="templates/orderforms/singlepage/style.css" />

{if !$loggedin && $currencies}
<form method="post" action="cart.php?gid={$smarty.get.gid}">
<p align="right">{$LANG.choosecurrency}: <select name="currency" onchange="submit()">{foreach from=$currencies item=curr}
<option value="{$curr.id}"{if $curr.id eq $currency.id} selected{/if}>{$curr.code}</option>
{/foreach}</select> <input type="submit" value="{$LANG.go}" /></p>
</form>
{/if}

<div class="seperatorbar">{$LANG.ordercategories}</div>

<p>{foreach key=num item=productgroup from=$productgroups}<input type="radio" name="gid" value="{$productgroup.gid}" onclick="window.location='{$smarty.server.PHP_SELF}?gid={$productgroup.gid}'"{if $smarty.get.gid eq $productgroup.gid} checked{else}{if $num eq "0"} checked{/if}{/if} /> {$productgroup.name}{/foreach}
{if $loggedin}<input type="radio" onclick="window.location='{$smarty.server.PHP_SELF}?gid=addons'"{if $smarty.get.gid eq "addons"} checked{/if} /> {$LANG.cartproductaddons}{/if}
{if $registerdomainenabled}<input type="radio" onclick="window.location='{$smarty.server.PHP_SELF}?a=add&domain=register'" /> {$LANG.registerdomain}{/if}
{if $transferdomainenabled}<input type="radio" onclick="window.location='{$smarty.server.PHP_SELF}?a=add&domain=transfer'" /> {$LANG.transferdomain}{/if}</p>

<form method="post" action="{$smarty.server.PHP_SELF}?a=add" name="productchoose" target="act_wnd" onsubmit="document.getElementById('loadinggraphic').style.display='block'">

<div class="seperatorbar">{$LANG.orderproduct}</div>

<p>
{foreach key=num item=product from=$products}
<input type="radio" name="pid" value="{$product.pid}" id="product{$product.pid}"{if $product.qty eq "0"} disabled{/if} /> <label for="product{$product.pid}">{$product.name}</label> - {$product.description}<br />
{/foreach}
</p>

<p align="center"><input type="submit" value="{$LANG.ordercontinuebutton}" /></p>

</form>

<div id="domainchoose"></div>
<div id="productconfig"></div>
<div id="domainconfig"></div>
<div id="ordersummary"></div>

<div id="loadinggraphic" align="center" style="display:none;"><img src="images/loading.gif" /></div>

<iframe src="images/logo.jpg" id="act_wnd" name="act_wnd" style="display:none;"></iframe>

{php}
if (isset($_SESSION["cart"])) {
    unset($_SESSION["cart"]);
}
{/php}