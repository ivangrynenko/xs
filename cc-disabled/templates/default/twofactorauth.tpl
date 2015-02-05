{include file="$template/pageheader.tpl" title="Two factor authentication"}

{include file="$template/clientareadetailslinks.tpl"}

{if $successful}
<div class="alert-message success">
    <p>{$successful}</p>
</div>
{/if}

{if $errormessage}
<div class="alert-message error">
    <p class="bold">{$LANG.clientareaerrors}</p>
    <ul>
        {$errormessage}
    </ul>
</div>
{/if}

{if $rh_user_duo_enabled && $rh_user_duo_status}
  <p class="duo green">{$rh_user_duo_enabled}</p>
{/if}

{if $rh_user_duo_disabled && !$rh_user_duo_status}
  <p class="duo red">{$rh_user_duo_disabled}</p>
{/if}

<form method="post" action="{$smarty.server.PHP_SELF}?action=2factorauth">

<fieldset class="onecol">

  <div class="clearfix">
      <label for="currentans">Enable Two-Factor Authentication</label>
    <div class="input">
      <select name="twofactorauth" id="twofactorauth">
        <option{if !$rh_user_duo_status} selected="selected"{/if}>No</option>
        <option{if $rh_user_duo_status} selected="selected"{/if}>Yes</option>
      </select>
    </div>
  </div>

</fieldset>

<div class="actions">
  <input class="btn primary" type="submit" name="submit" value="{$LANG.clientareasavechanges}" />
</div>
</form>
<hr />

<div>
  <p>Protecting your client area with two-factor authentication is the best way to protect against today's online threats.</p>
  <p><img src="{$duo_whmcs_base_path}modules/addons/duosecurity/how-it-works.png" /></p>
  <h3>How it Works for Everyone</h3>
  <p>
    The Duo Mobile smartphone application is free and available on all major 
    smartphone platforms, and lets users easily generate passcodes without 
    the cost and hassle of hardware tokens. iPhone and Android users can use 
    Duo Push which "pushes" login or transaction details to the phone, allowing 
    for immediate, one-tap approval. <a href="http://www.duosecurity.com/duo-push">Watch the Duo Push demo</a></p>
  <p>
    Older devices like cellphones and landlines are also fully supported. 
    Duo can send passcodes via text message, or place a phone call — users 
    just press a button on their keypad to authenticate.
  </p>
  <p><img src="{$duo_whmcs_base_path}modules/addons/duosecurity/methods.png" /></p>
</div>
