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

        if ($.fn.cssOriginal != undefined) {
          $.fn.css = $.fn.cssOriginal;
        }

        var api = $(".fullwidthbanner").revolution({
          delay: 15000,
          startheight: 500,
          startwidth: 1140,
          onHoverStop: "on",
          stopAfterLoops: 1
        });

        api.bind("revolution.slide.onloaded", function (e, data) {
          jQuery(".tparrows.default").css("display", "block");
          jQuery(".tp-bullets").css("display", "block");
          jQuery(".tp-bannertimer").css("display", "block");
        });

      });
    }
  };
}(jQuery));
