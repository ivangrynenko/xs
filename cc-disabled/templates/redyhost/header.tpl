<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head profile="http://www.w3.org/1999/xhtml/vocab">
<meta content="text/html; charset=utf-8" http-equiv="Content-Type" />
<link type="image/vnd.microsoft.icon" href="/favicon_1.ico" rel="shortcut icon"></link>
<meta content="WHMCS (http://whmcs.com)" name="Generator" />
<title>RedyHost Secure Cloud Hosting | Client area</title>
{if $systemurl}<base href="{$systemurl}" />{/if}
<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.5.2/jquery.min.js" type="text/javascript"></script>
<script type="text/javascript">
<!--//--><![CDATA[//><!--
window.jQuery || document.write("<script src='/sites/all/modules/jquery_update/replace/jquery/jquery.min.js'><script>")
//--><!]]>
</script>
<script src="/themes/firehost/js/firehost.js" type="text/javascript"></script>
<script src="/cc/templates/redyhost/whmcs.js" type="text/javascript"></script>

<style media="all" type="text/css">
	@import url("/sites/all/modules/jquery_update/replace/ui/themes/base/minified/jquery.ui.core.min.css");
	@import url("/sites/all/modules/jquery_update/replace/ui/themes/base/minified/jquery.ui.theme.min.css");
</style>
<style media="all" type="text/css">
@import url("/sites/all/modules/ctools/css/ctools.css");
@import url("/sites/all/modules/nice_menus/nice_menus.css");
@import url("/sites/all/modules/nice_menus/nice_menus_default.css");
@import url("/sites/all/modules/selectmenu/js/jquery.ui.selectmenu/jquery.ui.selectmenu.css");
@import url("/sites/all/modules/twitter_block/twitter_block.css");
</style>
<style media="all" type="text/css">
@import url("/themes/firehost/css/layout.css");
@import url("/themes/firehost/css/menu.css");
@import url("/themes/firehost/css/style.css");
</style>
<style media="print" type="text/css">
@import url("/themes/firehost/css/print.css");
</style>
<style media="all" type="text/css">@import url("/sites/default/files/fontyourface/font.css");
@import url("/sites/default/files/fontyourface/fontsquirrel/Aurulent-Sans-fontfacekit/stylesheet.css");
</style>
<link media="all" href="{if $smarty.server.HTTPS != ""}https://{else}http://{/if}fonts.googleapis.com/css?family=Architects+Daughter:regular&amp;subset=latin" rel="stylesheet" type="text/css"></link>
<style media="all" type="text/css">@import url("/sites/default/files/fontyourface/local_fonts/Serpentine-oblique-bold/stylesheet.css");</style>

<link rel="stylesheet" type="text/css" href="templates/{$template}/style.css"></link>
<link rel="stylesheet" type="text/css" href="templates/{$template}/css/style.css"></link>
<link rel="stylesheet" type="text/css" href="templates/{$template}/style-1.css"></link>
<script type="text/javascript" src="{if $smarty.server.HTTPS != ""}https://{else}http://{/if}www.redyhost.com.au/cc/templates/{$template}/js/whmcs.js"></script>
{if $livehelpjs}{$livehelpjs}{/if}
</head>
<body>
  <div id="page-wrapper">
    <div id="top_link">
    <ul>
      <li class="first odd"><a href="/cc/cart.php?a=view" class="user-link1">Your Cart</a></li>
      <li class="last"><a href="/cc/clientarea.php" class="user-link2">Customer Login</a></li>
    </ul>
  </div>
    <div class="clear"></div>
    <div id="page">
      <div id="header">
      <div id="logo">
        <h1><a id="logo-link" rel="home" title="Home" href="/">
            <img alt="Home" src="/sites/default/files/redyhost-logo-web_0.png" />
          </a>
        </h1>
<!--        <span class="moto">Secure Cloud Hosting</span>-->
      </div>
      <div class="menu">
        <h4 class="phone-number">1300 66 2 100</h4>
        <span id="live-chat">
          <a title="Chat Live Now" href="/cc/modules/livehelp/index.php" target="_blank" onclick="openLiveHelp(); return false">
            <img width="40px" height="40px" border="0" src="/themes/firehost/images/chat_blue.png" id="LiveHelpStatus" name="LiveHelpStatus" alt="Live Help" title="Chat with us Live Now" />
          </a>
        </span>
          <div class="region region-header">
          <div class="block block-nice-menus" id="block-nice-menus-1">


            <div class="content">
              <ul id="nice-menu-1" class="nice-menu nice-menu-down sf-js-enabled"><li class="menu-817 menuparent menu-path-vps-hosting first  odd"><a href="/vps-hosting">VPS Hosting</a><span class="menu-description"></span><ul style="display: none; visibility: hidden;"><li class="menu-818 menu-path-vps-hosting first  odd"><a title="VPS Plans overview" href="/vps-hosting">Virtual Servers</a><span class="menu-description">VPS Plans overview</span></li>
                <li class="menu-918 menu-path-vps-hosting-all-plans even  last"><a title="Compare our VPS plans" href="/vps-hosting/all-plans">Compare VPS Plans</a><span class="menu-description">Compare our VPS plans</span></li>
                </ul>
                </li>
                <li class="menu-588 menuparent menu-path-web-hosting even"><a href="/web-hosting">Hosting</a><span class="menu-description"></span><ul style="display: none; visibility: hidden;"><li class="menu-815 menu-path-web-hosting first  odd"><a title="Our Cloud Shared Hosting plans" href="/web-hosting">Shared Hosting</a><span class="menu-description">Our Cloud Shared Hosting plans</span></li>
                <li class="menu-813 menu-path-web-hosting-reseller even  last"><a title="Browse our reseller hosting plans" href="/web-hosting/reseller">Reseller Hosting</a><span class="menu-description">Browse our reseller hosting plans</span></li>
                </ul>
                </li>
                <li class="menu-795 menuparent menu-path-node-85 odd"><a href="/domains">Domains</a><span class="menu-description"></span><ul style="display: none; visibility: hidden;"><li class="menu-797 menu-path-node-89 first  odd"><a href="/domains/tld-name">about .NAME</a><span class="menu-description"></span></li>
                <li class="menu-800 menu-path-node-91 even "><a href="/domains/tld-asia">about .asia domains</a><span class="menu-description"></span></li>
                <li class="menu-799 menu-path-node-90 odd"><a href="/domains/tld-mobi">about .mobi domains</a><span class="menu-description"></span></li>
                <li class="menu-798 menu-path-node-87 even  last"><a href="/domains/tld-org">about .org domains</a><span class="menu-description"></span></li>
                </ul>
                </li>
                <li class="menu-977 menu-path-ssl-security-geotrust even "><a href="/ssl-security/geotrust">SSL Security</a><span class="menu-description"></span></li>
                <li class="menu-505 menuparent menu-path-node-2 odd"><a href="/support">Support</a><span class="menu-description"></span><ul style="display: none; visibility: hidden;"><li class="menu-978 menu-path-node-2 first  odd"><a title="contact support or get self-help" href="/support">Support Center</a><span class="menu-description">contact support or get self-help</span></li>
                <li class="menu-504 menu-path-knowledgebase even  last"><a title="Interactive knowledgebase" href="/knowledgebase">Knowledgebase</a><span class="menu-description">Interactive knowledgebase</span></li>
                </ul>
                </li>
                <li class="menu-583 menuparent menu-path-node-62 even"><a href="/about">About</a><span class="menu-description"></span><ul style="display: none; visibility: hidden;"><li class="menu-808 menu-path-node-99 first  odd"><a title="Why you'll love our web hosting services" href="/about/five-reasons-why-you-love-our-services">5 Reasons Why Choose Us</a><span class="menu-description">Why you'll love our web hosting services</span></li>
                <li class="menu-811 menu-path-node-103 even "><a title="All active discounts and specials" href="/about/discounts-and-specials">Discounts &amp; Specials</a><span class="menu-description">All active discounts and specials</span></li>
                <li class="menu-810 menu-path-node-101 odd"><a title="We migrate your accounts free!" href="/about/free-account-migration">Free Account Migration</a><span class="menu-description">We migrate your accounts free!</span></li>
                <li class="menu-807 menu-path-node-98 even "><a title="Check out where all our servers live!" href="/about/data-center">Our Data Center</a><span class="menu-description">Check out where all our servers live!</span></li>
                <li class="menu-809 menu-path-node-100 odd  last"><a title="RedyHost guarantees level of services. Read how." href="/about/guarantee">RedyHost Guarantees</a><span class="menu-description">RedyHost guarantees level of services. Read how.</span></li>
                </ul>
                </li>
                <li class="menu-958 menu-path-node-126 odd  last"><a href="/contact">Contact</a><span class="menu-description"></span></li>
                </ul>
                  </div>
                </div>
                  </div>
      </div>
      <div id="welcome_box">{if $loggedin}{$LANG.welcomeback}, <strong>{$loggedinuser.firstname}</strong>&nbsp;&nbsp;&nbsp;<img src="templates/{$template}/images/icons/details.gif" alt="{$LANG.clientareanavdetails}" width="16" height="16" border="0" class="absmiddle" /> <a href="clientarea.php?action=details" title="{$LANG.clientareanavdetails}"><strong>{$LANG.clientareanavdetails}</strong></a>&nbsp;&nbsp;&nbsp;<img src="templates/{$template}/images/icons/logout.gif" alt="{$LANG.logouttitle}" width="16" height="16" border="0" class="absmiddle" /> <a href="logout.php" title="Logout"><strong>{$LANG.logouttitle}</strong></a>{else}{$LANG.please} <a href="clientarea.php" title="{$LANG.loginbutton}"><strong>{$LANG.loginbutton}</strong></a> {$LANG.or} <a href="register.php" title="{$LANG.clientregistertitle}"><strong>{$LANG.clientregistertitle}</strong></a>{/if}</div>
 
      <div class="clear"></div>
    </div> <!-- #header-->
    <div class="clear"></div>
    <div id="white_bg_title">
      <p class="breadcrumb">{$breadcrumbnav}</p>
      <h1 id="page-title" class="title">{$pagetitle}</h1>
    </div>
    <div class="clear"></div>
    {if $loggedin}
      <div id="top_menu">
        <ul>
          <li><a href="clientarea.php" title="{$LANG.clientareanavhome}">{$LANG.clientareanavhome}</a></li>
          <li><a href="clientarea.php?action=details" title="{$LANG.clientareanavdetails}">{$LANG.clientareanavdetails}</a></li>
          <li><a href="clientarea.php?action=products" title="{$LANG.clientareanavservices}">{$LANG.clientareanavservices}</a></li>
          <li><a href="clientarea.php?action=domains" title="{$LANG.clientareanavdomains}">{$LANG.clientareanavdomains}</a></li>
          <li><a href="clientarea.php?action=quotes" title="{$LANG.quotestitle}">{$LANG.quotestitle}</a></li>
          <li><a href="clientarea.php?action=invoices" title="{$LANG.invoices}">{$LANG.invoices}</a></li>
          <li><a href="supporttickets.php" title="{$LANG.clientareanavsupporttickets}">{$LANG.clientareanavsupporttickets}</a></li>
          <li><a href="affiliates.php" title="{$LANG.affiliatestitle}">{$LANG.affiliatestitle}</a></li>
          <li><a href="clientarea.php?action=emails" title="{$LANG.clientareaemails}">{$LANG.clientareaemails}</a></li>
        </ul>
        <div class="clear"></div>
      </div>
    {/if}
    <div class="clear"></div>
    <div id="content-area">
      <div class="clearfix" id="main-wrapper">
        <div class="clearfix" id="main">
          <div class="column" id="content">
            <div class="section">
              <a id="main-content"></a>
              <div class="tabs"></div>
              <div class="region region-content">
                <div class="block block-system" id="block-system-main">
                  <div class="content">