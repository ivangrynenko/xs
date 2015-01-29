<?php


define("CLIENTAREA",true);

require("dbconnect.php");
require("includes/functions.php");
require("includes/clientareafunctions.php");
require_once(__DIR__ . '/modules/addons/duosecurity/library/duo_web.php');
require_once(__DIR__ . '/modules/addons/duosecurity/duosecurity.php');

// Getting Duo Security system variables
$variables = duosecurity_get_variables();

$pagetitle = $_LANG['clientareatitle'];
$breadcrumbnav = '<a href="index.php">'.$_LANG['globalsystemname'].'</a>';
$breadcrumbnav .= ' > <a href="services.php">Web and Design Services</a>'; 

initialiseClientArea($pagetitle,'',$breadcrumbnav);

$smartyvalues["two-factor-form"] = $form;
$smartyvalues["duo_whmcs_base_path"] = $variables->duo_whmcs_base_dir;
$smartyvalues["rh_clientareaaction"] = $_GET['action'];
$smartyvalues["rh_user"] = $_SESSION['uid'] ? duosecurity_get_user($_SESSION['uid']) : NULL;
$smartyvalues["rh_user_duo_enabled"] = "Your clientarea is currently secured with 
  two-factor authentication";
$smartyvalues["rh_user_duo_disabled"] = "
  <p>
    Your clientarea is currently not secured with two-factor authentication. 
  </p>
  <p>
    Anyone with the password could access your client area without you knowing it.
  </p>
  <p>
    By enabling the 'two factor authentication' you increase the security of your account
    by only allowing to login with a security code obtained via SMS, DuoSecurity app 
    for your smartphone or even a phone call.
  </p>
  <p>
    So whenever you are, setting Two Factor Authentication to Yes will ensure that 
    you and only you can access your account.
  </p>";

if ($smartyvalues["rh_user"]->duo_status == 1) {
  $smartyvalues["rh_user_duo_status"] = $smartyvalues["rh_user"]->duo_status;
}

switch ($_GET['action']) {
  case 'details':
  case 'creditcard':
  case 'contacts':
  case 'changepw':
  case 'changesq':
    $query = $_GET['action'];
    header("Location: clientarea.php?action=$query");
    break;
}


if ($_SESSION['uid']) {
  if ($_GET['action'] === '2factorauth' && isset($_POST['submit']) && count($_POST)) {
    // Saving submitted values into our custom tables
    $result = duosecurity_form_save($_POST);
    if ($result) {
      if ($_POST['twofactorauth'] === 'Yes') {
        if (function_exists('logActivity')) {
          logActivity('User id:' . $_SESSION['uid'] . ' has activated 2-factor authenticatin');
        }
        $smartyvalues['successful'] = 'Two factor authentication has been successfully activated';
        // Now we need to set DuoSecurity
        $duo = new Duo();
        $sig_request = $duo->signRequest($variables->duo_integration_key, $variables->duo_secret_key, $variables->duo_akey, $smartyvalues["rh_user"]->email);
        print duosecurity_print_output($variables, $sig_request);
        exit;
      }
      if ($_POST['twofactorauth'] === 'No') {
        if (function_exists('logActivity')) {
          logActivity('User id:' . $_SESSION['uid'] . ' has de-activated 2-factor authenticatin');
        }
        $smartyvalues['successful'] = 'Two factor authentication has been deactivated';
      }
    }
    // redirect to the first tab
    header('Location: rh-clientarea.php?action=2factorauth');
  }
}
else {
  header('Location: clientarea.php');
  header('Status: 403 Access denied"');
  exit;
}

$templatefile = "twofactorauth"; 

outputClientArea($templatefile);
?>