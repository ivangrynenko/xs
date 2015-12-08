/**
 * @file
 * Drupal behaviors for xenserver module.
 */

/*global Drupal, window, jQuery*/
(function ($) {
  "use strict";

  Drupal.behaviors.xs_tooltip = {
    attach: function (context, settings) {
      $('[data-toggle="tooltip"]').tooltip();
    }
  };

})(jQuery);
