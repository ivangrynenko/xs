<?php

// Setting statustext to Overdue for overdue invoices
ini_set('date.timezone', "Australia/Melbourne");
date_default_timezone_set("Australia/Melbourne");

$iso_due_date = $duedate;
$iso_date_array = explode("/", $iso_due_date);

$iso_due_date = $iso_date_array[2] . '-' . $iso_date_array[1] . '-' . $iso_date_array[0];

$pdf->SetXY(0,10);

# Logo
if (file_exists(ROOTDIR . '/images/RedyHOST-logo-web.png')) {
$logo_html = '<img src="' . ROOTDIR . '/images/RedyHOST-logo-web.png" style="width: 163px; height: 30px;"/>';
$pdf->writeHTML($logo_html, true, false, false, false, '');
}
elseif (file_exists(ROOTDIR . '/images/logo.jpg')) {
$pdf->Image(ROOTDIR . '/images/logo.jpg',20,25,75);
}
else {
$pdf->Image(ROOTDIR . '/images/placeholder.png',20,25,75);
}

# Invoice Status
$statustext = $_LANG['invoices'.strtolower($status)];

$dateTimeZone = new DateTimeZone("Australia/Melbourne");
$date = new DateTime("now", $dateTimeZone);
$due_date = $date::createFromFormat('Y-m-d', $iso_due_date, $dateTimeZone);

if (strtolower($statustext) == 'unpaid' && $due_date->getTimestamp() < ($date->getTimestamp() + 3600) ) {
$statustext = 'Overdue';
}

$pdf->SetFillColor(223,85,74);
$pdf->SetDrawColor(171,49,43);
if ($status=="Paid") {
$pdf->SetFillColor(151,223,74);
$pdf->SetDrawColor(110,192,70);
}elseif ($status=="Cancelled") {
$pdf->SetFillColor(200);
$pdf->SetDrawColor(140);
} elseif ($status=="Refunded") {
$pdf->SetFillColor(131,182,218);
$pdf->SetDrawColor(91,136,182);
} elseif ($status=="Collections") {
$pdf->SetFillColor(3,3,2);
$pdf->SetDrawColor(127);
}

//var_dump($statustext); exit;
if (strtolower($statustext) == 'unpaid') {
unset($statustext);
}

if (isset($statustext111)) {
$pdf->SetXY(0,0);
$pdf->SetFont('freesans','B',28);
$pdf->SetTextColor(255);
$pdf->SetLineWidth(0.75);
$pdf->StartTransform();
$pdf->Rotate(-35,100,225);
$pdf->Cell(210,18,strtoupper($statustext),'TB',0,'C','1');
$pdf->StopTransform();
$pdf->SetTextColor(0);
}

# Company Details
$pdf->SetXY(15,14);
$pdf->SetFont('freesans','',13);
$pdf->Cell(160,6,trim($companyaddress[0]),0,1,'R');
$pdf->SetFont('freesans','',9);
for ( $i = 1; $i <= ((count($companyaddress)>6) ? count($companyaddress) : 6); $i += 1) {
$pdf->Cell(160,4,trim($companyaddress[$i]),0,1,'R');
}

# Header Bar
$invoiceprefix = $_LANG["invoicenumber"];
/*
** This code should be uncommented for EU companies using the sequential invoice numbering so that when unpaid it is shown as a proforma invoice **
if ($status!="Paid") {
$invoiceprefix = $_LANG["proformainvoicenumber"];
}
*/
$pdf->SetFont('freesans','B',10);
$pdf->SetFillColor(255,255,255);
$pdf->Cell(0,6,$invoiceprefix.$invoicenum,0,1,'L','1');
$pdf->SetFont('freesans','',10);
$pdf->Cell(0,4,$_LANG["invoicesdatecreated"].': '.$datecreated.'',0,1,'L','1');
$pdf->Cell(0,4,$_LANG["invoicesdatedue"].': '.$duedate.'',0,1,'L','1');
$pdf->Ln(3);

$startpage = $pdf->GetPage();

# Clients Details
$pdf->SetFillColor(255,255,255);
$pdf->SetFont('freesans','',10);
$pdf->SetX(35);
if ($clientsdetails["companyname"]) {
$pdf->SetX(35);
$pdf->Cell(0,4,$clientsdetails["companyname"],0,1,'L');
$pdf->SetX(35);
$pdf->Cell(0,4,$clientsdetails["firstname"]." ".$clientsdetails["lastname"],0,1,'L');
}
else {
$pdf->SetX(35);
$pdf->Cell(0,4,$clientsdetails["firstname"]." ".$clientsdetails["lastname"],0,1,'L');
}

$pdf->SetX(35);
$pdf->SetFont('freesans','B',10);
$pdf->Cell(0,4,$clientsdetails["address1"],0,1,'L');

if ($clientsdetails["address2"]) {
$pdf->SetX(35);
$pdf->SetFont('freesans','B',10);
$pdf->Cell(0,4,$clientsdetails["address2"],0,1,'L');
}

$pdf->SetX(35);
$pdf->SetFont('freesans','B',10);
$pdf->Cell(0,4,strtoupper($clientsdetails["city"]) . ", " . strtoupper($clientsdetails["state"]) . ", ".$clientsdetails["postcode"],0,1,'L');

$pdf->SetFont('freesans','',10);
$pdf->SetX(35);
$pdf->Cell(0,4,$clientsdetails["country"],0,1,'L');
if ($customfields) {
$pdf->Ln();
foreach ($customfields AS $customfield) {
$pdf->SetX(45);
$pdf->Cell(0,4,$customfield['fieldname'].': '.$customfield['value'],0,1,'L');
}
}

# How to pay your invoice
if ($statustext != 'Paid') {
$pdf->SetXY(130,62);
$pdf->SetFont('freesans','B',10);
$pdf->Cell(160, 0,"How to pay your invoice",0,1,'L');
$pdf->SetXY(130,70);
$pdf->SetFont('freesans','',9);
$pdf->Cell(300,0,"Our bank details for Bank transfer/EFT are:",0,1,'L');
$pdf->SetXY(130,74);
$pdf->Cell(300,0,"Account name: TRUECMS PTY LTD",0,1,'L');
$pdf->SetXY(130,78);
$pdf->Cell(300,0,"Account number: 338633",0,1,'L');
$pdf->SetXY(130,82);
$pdf->Cell(300,0,"BSB: 033181",0,1,'L');
$pdf->SetXY(130,86);
$pdf->Cell(300,0,"Bank name: Westpac",0,1,'L');
$pdf->SetXY(130,90);
$pdf->Cell(300,0,"To pay via Credit Card or PayPal",0,1,'L');
$pdf->SetXY(130,94);
$pdf->Cell(300,0,"please login to your clientarea",0,1,'L');
$pdf->SetXY(130,98);
$pdf->Cell(300,0,"http://www.redyhost.com.au/myredyhost",0,1,'L');
$pdf->SetXY(0,102);
}

if ($statustext == 'Overdue') {
$pdf->Ln(4);
$pdf->SetFillColor(215,90,90);
// $pdf->SetDrawColor(0,0,0);
$pdf->SetTextColor(255,255,255);
$pdf->SetFont('freesans','B',16);
$pdf->Cell(0,6,'REMINDER - OVERDUE ACCOUNT',0,1,'C','1');
$pdf->SetFillColor(0);
$pdf->SetTextColor(0,0,0);
$pdf->SetFont('freesans','',9);
$pdf->Ln(1);
$overdueHTML = '<p> &nbsp;Our records indicate that payment of the Balance amount below is overdue.<br/>
  &nbsp; To avoid any interruptions in delivering your service please pay the invoice Balance amount now!<br/>
  &nbsp; If you paid your account on or
  after ' . date("d/m/y", time() ) . ', thank you and please disregard this notice.</p>
<p>&nbsp; If you are unable to pay this amount, please contact RedyHost™ for alternative payment options.</p>
<hr/>';

$pdf->writeHTML($overdueHTML, true, false, false, false, '');
}

$pdf->Ln(4);

# Invoice Items
$tblhtml = '
<table width="100%" bgcolor="#ccc" cellspacing="1" cellpadding="2" border="0">
  <tr height="30" bgcolor="#efefef" style="font-weight:bold;text-align:center;">
    <td width="80%">Your Services & Add-Ons '.$_LANG['invoicesdescription'].'</td>
    <td width="20%">'.$_LANG['quotelinetotal'].'</td>
  </tr>
  ';
  foreach ($invoiceitems AS $item) {
  $tblhtml .= '
  <tr bgcolor="#fff">
    <td align="left">'.nl2br($item['description']).'<br/></td>
    <td align="center">'.$item['amount'].'</td>
  </tr>
  ';
  }
  $tblhtml .= '
  <tr height="30" bgcolor="#efefef" style="font-weight:bold;">
    <td align="right">'.$_LANG['invoicessubtotal'].'</td>
    <td align="center">'.$subtotal.'</td>
  </tr>
  ';
  if ($taxname) $tblhtml .= '
  <tr height="30" bgcolor="#efefef" style="font-weight:bold;">
    <td align="right">'.$taxrate.'% '.$taxname.'</td>
    <td align="center">'.$tax.'</td>
  </tr>
  ';
  if ($taxname2) $tblhtml .= '
  <tr height="30" bgcolor="#efefef" style="font-weight:bold;">
    <td align="right">'.$taxrate2.'% '.$taxname2.'</td>
    <td align="center">'.$tax2.'</td>
  </tr>
  ';
  $tblhtml .= '
  <tr height="30" bgcolor="#efefef" style="font-weight:bold;">
    <td align="right">'.$_LANG['invoicescredit'].'</td>
    <td align="center">'.$credit.'</td>
  </tr>
  <tr height="30" bgcolor="#efefef" style="font-weight:bold;">
    <td align="right">'.$_LANG['invoicestotal'].'</td>
    <td align="center">'.$total.'</td>
  </tr>
</table>';

$pdf->writeHTML($tblhtml, true, false, false, false, '');

$pdf->Ln(5);

# Transactions
$pdf->SetFont('freesans','B',12);
$pdf->Cell(0,4,$_LANG["invoicestransactions"],0,1);

$pdf->Ln(5);

$pdf->SetFont('freesans','',9);

$tblhtml = '
<table width="100%" bgcolor="#ccc" cellspacing="1" cellpadding="2" border="0">
  <tr height="30" bgcolor="#efefef" style="font-weight:bold;text-align:center;">
    <td width="25%">'.$_LANG['invoicestransdate'].'</td>
    <td width="25%">'.$_LANG['invoicestransgateway'].'</td>
    <td width="30%">'.$_LANG['invoicestransid'].'</td>
    <td width="20%">'.$_LANG['invoicestransamount'].'</td>
  </tr>
  ';

  if (!count($transactions)) {
  $tblhtml .= '
  <tr bgcolor="#fff">
    <td colspan="4" align="center">'.$_LANG['invoicestransnonefound'].'</td>
  </tr>
  ';
  } else {
  foreach ($transactions AS $trans) {
  $tblhtml .= '
  <tr bgcolor="#fff">
    <td align="center">'.$trans['date'].'</td>
    <td align="center">'.$trans['gateway'].'</td>
    <td align="center">'.$trans['transid'].'</td>
    <td align="center">'.$trans['amount'].'</td>
  </tr>
  ';
  }
  }
  $tblhtml .= '
  <tr height="30" bgcolor="#efefef" style="font-weight:bold;">
    <td colspan="3" align="right">'.$_LANG['invoicesbalance'].'</td>
    <td align="center">'.$balance.'</td>
  </tr>
</table>';

$pdf->writeHTML($tblhtml, true, false, false, false, '');

# Notes
if ($notes) {
$pdf->Ln(5);
$pdf->SetFont('freesans','',8);
$pdf->MultiCell(170,5,$_LANG["invoicesnotes"].": $notes");
}

# Generation Date
$pdf->SetFont('freesans','',8);
$pdf->Ln(5);
$pdf->Cell(180,4,$_LANG['invoicepdfgenerated'].' '.getTodaysDate(1),'','','C');

?>