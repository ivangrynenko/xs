(function ($) {
  "use strict";

  /**
   * Bootstrap tooltip initialisation.
   */

  Drupal.behaviors.xstooltip = {
    attach: function (context, settings) {
      $('[data-toggle="tooltip"]').tooltip();
    }
  }

  Drupal.behaviors.xspopover = {
    attach: function (context, settings) {
      $('[data-toggle="popover"]').popover();
    }
  }
}(jQuery));
