{if $langchange}<div class="language-wrapper">{$setlanguage}</div><br />{/if}
  </div>


<!--{$smarty.server.REQUEST_URI}-->
{if $smarty.server.REQUEST_URI|substr:0:18 eq "/cc/knowledgebase/" or $smarty.server.REQUEST_URI|substr:0:47 eq "/cc/knowledgebase.php?action=displayarticle&id="} {else}
  <div id="side_menu">
    <p class="header">{$LANG.quicknav}</p>
    <ul>
      <li><a href="http://firehost.com.au/"><img src="templates/{$template}/images/icons/support.gif" alt="{$LANG.globalsystemname}" width="16" height="16" border="0" class="absmiddle" /></a> <a href="index.php" title="{$LANG.globalsystemname}">{$LANG.globalsystemname}</a></li>
      <li><a href="clientarea.php"><img src="templates/{$template}/images/icons/clientarea.gif" alt="{$LANG.clientareatitle}" width="16" height="16" border="0" class="absmiddle" /></a> <a href="clientarea.php" title="{$LANG.clientareatitle}">{$LANG.clientareatitle}</a></li>
      <li><a href="announcements.php" title="{$LANG.announcementstitle}"><img src="templates/{$template}/images/icons/announcement.gif" alt="{$LANG.announcementstitle}" width="16" height="16" border="0" class="absmiddle" /></a> <a href="announcements.php" title="{$LANG.announcementstitle}">{$LANG.announcementstitle}</a></li>
      <li><a href="knowledgebase.php" title="{$LANG.knowledgebasetitle}"><img src="templates/{$template}/images/icons/knowledgebase.gif" alt="{$LANG.knowledgebasetitle}" width="16" height="16" border="0" class="absmiddle" /></a> <a href="knowledgebase.php" title="{$LANG.knowledgebasetitle}">{$LANG.knowledgebasetitle}</a></li>
      <li><a href="submitticket.php" title="{$LANG.supportticketssubmitticket}"><img src="templates/{$template}/images/icons/submit-ticket.gif" alt="{$LANG.supportticketssubmitticket}" width="16" height="16" border="0" class="absmiddle" /></a> <a href="submitticket.php" title="{$LANG.supportticketspagetitle}">{$LANG.supportticketssubmitticket}</a></li>
      <li><a href="cart.php" title="{$LANG.ordertitle}"><img src="templates/{$template}/images/icons/order.gif" alt="{$LANG.ordertitle}" width="16" height="16" border="0" class="absmiddle" /></a> <a href="cart.php" title="{$LANG.ordertitle}">{$LANG.ordertitle}</a></li>
    </ul>
{if $loggedin}
    <p class="header">{$LANG.accountinfo}</p>
<p><strong>{$clientsdetails.firstname} {$clientsdetails.lastname} {if $clientsdetails.companyname}({$clientsdetails.companyname}){/if}</strong><br />
{$clientsdetails.address1}, {$clientsdetails.address2}<br />
{$clientsdetails.city}, {$clientsdetails.state}, {$clientsdetails.postcode}<br />
{$clientsdetails.countryname}<br />
{$clientsdetails.email}<br /><br />
{if $addfundsenabled}<img src="templates/{$template}/images/icons/money.gif" alt="Add Funds" width="22" height="22" border="0" class="absmiddle" /> <a href="clientarea.php?action=addfunds">{$LANG.addfunds}</a>{/if}</p>
    <p class="header">{$LANG.accountstats}</p>
    <p>{$LANG.statsnumproducts}: <strong>{$clientsstats.productsnumactive}</strong> ({$clientsstats.productsnumtotal})<br />
{$LANG.statsnumdomains}: <strong>{$clientsstats.numactivedomains}</strong> ({$clientsstats.numdomains})<br />
{$LANG.statsnumtickets}: <strong>{$clientsstats.numtickets}</strong><br />
{$LANG.statsnumreferredsignups}: <strong>{$clientsstats.numaffiliatesignups}</strong><br />
{$LANG.statscreditbalance}: <strong>{$clientsstats.creditbalance}</strong><br />
{$LANG.statsdueinvoicesbalance}: <strong>{if $clientsstats.numdueinvoices>0}<span class="red">{/if}{$clientsstats.dueinvoicesbalance}{if $clientsstats.numdueinvoices>0}</span>{/if}</strong></p>
{else}
<form method="post" action="{$systemsslurl}dologin.php">
  <p class="header">{$LANG.clientlogin}</p>
  <p><strong>{$LANG.email}</strong><br />
    <input name="username" type="text" size="25" />
  </p>
  <p><strong>{$LANG.loginpassword}</strong><br />
    <input name="password" type="password" size="25" />
  </p>
  <p>
    <input type="checkbox" name="rememberme" />
    {$LANG.loginrememberme}</p>
  <p>
    <input type="submit" class="submitbutton" value="{$LANG.loginbutton}" />
  </p>
</form>
  <p class="header">{$LANG.knowledgebasesearch}</p>
<form method="post" action="knowledgebase.php?action=search">
  <p>
    <input name="search" type="text" size="25" /><br />
    <select name="searchin">
      <option value="Knowledgebase">{$LANG.knowledgebasetitle}</option>
      <option value="Downloads">{$LANG.downloadstitle}</option>
    </select> 
    <input type="submit" value="{$LANG.go}" />
  </p>
</form>
{/if}
  </div>

{/if}
  <div class="clear"></div>
  
</div>


<div class="art-Footer">
    <div class="art-Footer-inner">
        <!--<a class="art-rss-tag-icon" href="/rss.xml"/>-->
        	<div class="art-Footer-text"><p><a href="/node/4">Contact Us</a> | <a href="/node/15">Terms & Conditions</a><br/>Copyright &copy; {$smarty.now|date_format:"%Y"} FIREHOST. Australia based cloud web hosting, cheap domain names & SSL certificates. All Rights Reserved.</p><p>Phone: 1300 66 2 100. FIREHOST is subdivision of TRUECMS Pty Ltd. ABN: 63 133 456 005</p>                </div>
    </div>
    <div class="art-Footer-background"/>
</div>
</div>



</div>
<div class="follow-us"><h5>Follow FireHost on </h5>
  <a href="http://www.facebook.com/profile.php?id=100000379924707&amp;ref=profile" target="_blank"><img src="/themes/firehost1/images/facebook.jpg" width="36" height="36" alt="facebook"></a>&nbsp;<a href="http://twitter.com/firehost_com_au" target="_blank"><img src="/themes/firehost1/images/twitter.jpg" width="36" height="36" alt="twitter"></a>
  <img class="geotrust" src="/images/GeoTrust_Partner_logo.jpg" alt="Firehost is GeoTrust Partner" width="125" height="60">
  

<a href="/cc/modules/livehelp/index.php" target="_blank" onclick="openLiveHelp(); return false"><img src="/cc/modules/livehelp/include/status.php" id="LiveHelpStatus" name="LiveHelpStatus" border="0" alt="Live Help" onload="statusImagesLiveHelp[statusImagesLiveHelp.length] = this;"/></a>


  </div>
<div id="bottom-wrapper-0">
    <div id="bottom-wrapper-1">
            <div id="banner33">
<div id="block-block-9" class="art-Block clear-block block block-block">
    <div class="art-Block-tl">
    <div class="art-Block-tr">
    <div class="art-Block-bl">
    <div class="art-Block-br">
    <div class="art-Block-tc">
    <div class="art-Block-bc">
    <div class="art-Block-cl">
    <div class="art-Block-cr">
    <div class="art-Block-cc">
    <div class="art-Block-body">

	<div class="art-BlockHeader">
		    <div class="art-header-tag-icon">
		        <div class="t">
                			<h2 class="subject">Business Web Hosting</h2>
</div>
		    </div>
		</div>
	<div class="art-BlockContent content">
	    <div class="art-BlockContent-body">

		<ul>
<li><a href="/cc/cart.php?a=add&amp;pid=8">Free for 90 days!</a></li>
<li><a href="/cc/cart.php?a=add&amp;pid=3">Fire1 (email only)</a></li>
<li><a href="/cc/cart.php?a=add&amp;pid=1">Fire3 (Business Starter)</a></li>
<li><a href="/cc/cart.php?a=add&amp;pid=7">Fire5 (Business)</a></li>
<li><a href="/cc/cart.php?a=add&amp;pid=2">Fire7 (Enterprise)</a>+ free domain</li>
<li><a href="/cc/cart.php?a=add&amp;pid=4">Fire9 (Enterprise Plus)</a>+ free domain</li>
<li><a href="/web-hosting">Compare hosting packages</a></li>
</ul>

	    </div>
	</div>


    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
</div>
</div>

<div id="banner34"><div id="block-block-10" class="art-Block clear-block block block-block">
    <div class="art-Block-tl">
    <div class="art-Block-tr">
    <div class="art-Block-bl">
    <div class="art-Block-br">
    <div class="art-Block-tc">
    <div class="art-Block-bc">
    <div class="art-Block-cl">
    <div class="art-Block-cr">
    <div class="art-Block-cc">
    <div class="art-Block-body">

	<div class="art-BlockHeader">
		    <div class="art-header-tag-icon">
		        <div class="t">
                			<h2 class="subject">Domain Names</h2>
</div>
		    </div>
		</div>
	<div class="art-BlockContent content">
	    <div class="art-BlockContent-body">

		<ul>
<li>.com.au - AU $39.95 (2 years)</li>
<li>.org.au - AU $25.95 (2 years)</li>
<li>.net.au - AU $39.95 (2 years)</li>
<li>.com - AU $19.95 (1 year)</li>
<li>.net - AU $19.95 (1 year)</li>
<li>.org - AU $19.95 (1 year)</li>
<li><a href="/cc/domainchecker.php?search=bulk">More domain names</a></li>
</ul>

	    </div>
	</div>


    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
</div>
</div>

<div id="banner35"><div id="block-block-11" class="art-Block clear-block block block-block">
    <div class="art-Block-tl">
    <div class="art-Block-tr">
    <div class="art-Block-bl">
    <div class="art-Block-br">
    <div class="art-Block-tc">
    <div class="art-Block-bc">
    <div class="art-Block-cl">
    <div class="art-Block-cr">
    <div class="art-Block-cc">
    <div class="art-Block-body">

	<div class="art-BlockHeader">
		    <div class="art-header-tag-icon">
		        <div class="t">
                			<h2 class="subject">Support Links</h2>
</div>
		    </div>
		</div>
	<div class="art-BlockContent content">
	    <div class="art-BlockContent-body">

		<ul>
<li><a href="/cc/knowledgebase.php">Search knowledgebase</a></li>
<li><a href="/cc/knowledgebase/14/Setting-up-email-client">How To Setup Email</a></li>
<li><a href="/cc/knowledgebase/1/Using-cPanel">How to use cPanel</a></li>
<li><a href="/cc/submitticket.php?step=2&amp;deptid=1">Contact Web Hosting Support</a></li>
<li><a href="/cc/submitticket.php?step=2&amp;deptid=4">Contact Domain Names Support</a></li>
<li><a href="/cc/submitticket.php?step=2&amp;deptid=2">Pre-sales contact us</a></li>
<li><a href="/news">Service Updates Notice</a></li>
</ul>

	    </div>
	</div>


    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
    </div>
</div>
</div>    </div>
    </div>

<script src="{if $smarty.server.HTTPS != ""}https://{else}http://{/if}www.google-analytics.com/urchin.js" type="text/javascript"> 
</script>
<script type="text/javascript"> 
_uacct = "UA-1170526-16";
urchinTracker();
</script>



</body>
</html>