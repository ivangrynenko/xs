(function ($) {
$(document).ready(function() {

  $('#content_left h1').each(function() {
    if($(this).text() == 'Shopping Cart') {
    $('#content_left').addClass('shopping-cart');
    }
  });

  $('#content_left .cartbox').each(function(index) {
    $(this).addClass('item-'+index);
  });

  $('#content_left form h3').each(function() {
    if($(this).text() == 'Product/Service') {
      $(this).next('.cartbox').addClass('product-title');
    }

    if($(this).text() == 'Additional Required Information') {
      $(this).next('.cartbox').addClass('additional-required-information');
    }
  });

   $('#content_left .cartbox').each(function() {
    var firehost_productTitle = $(this).children('strong').text();
    if(firehost_productTitle.search(/geotrust /i) !=-1) {
      $('#content_left .cartbox').addClass("geotrust-product");
    }
  });
});

function check_all_tld(oForm, cbName, checked) {
  for (var i=0; i < oForm[cbName].length; i++) oForm[cbName][i].checked = checked;
}
});