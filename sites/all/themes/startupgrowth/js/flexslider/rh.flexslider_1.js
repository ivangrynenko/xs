/**
 * @file
 * RH OnePage Scroll plugin.
 **/

/*global jQuery, Drupal*/

(function ($) {
  'use strict';

  /**
   * Initialize dashboard events.
   */
  Drupal.behaviors.rhFlexSlider = {
    attach: function (context, settings) {
      jQuery(document).ready(function ($) {
        $(window).load(function () {
          $(".image-popup").magnificPopup({
            type: "image",
            removalDelay: 300,
            mainClass: "mfp-fade",
            gallery: {
              enabled: true // set to true to enable gallery
            }
          });
        });
      });
    }
  };
}(jQuery));
