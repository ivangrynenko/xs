<p>{$LANG.passwordreminderintrotext}</p>
{if $errormessage}
<div class="errorbox">{$errormessage}</div>
{/if}
<form method="post" action="passwordreminder.php">
  {if !$question}
  <input type="hidden" name="action" value="validate">
  <p align="center">{$LANG.loginemail}:
    <input type="text" name="email" size="50" value="{$email}">
  </p>
  <p align="center">
    <input type="submit" value="{$LANG.passwordremindervalidate}">
  </p>
  {else}
  <input type="hidden" name="action" value="send" />
  <input type="hidden" name="email" value="{$email}" />
  <p align="center">{$question}
    <input type="text" name="answer" size="50">
  </p>
  <p align="center">
    <input type="submit" value="{$LANG.passwordremindersendbutton}">
  </p>
  {/if}
</form><br />