function check_all_tld(oForm, cbName, checked) {
  for (var i=0; i < oForm[cbName].length; i++) oForm[cbName][i].checked = checked;
}