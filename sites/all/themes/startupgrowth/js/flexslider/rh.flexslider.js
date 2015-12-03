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
      // store the slider in a local variable
      var $window = $(window),
        flexslider;

      // tiny helper function to add breakpoints
      function getGridSize() {
        return (window.innerWidth < 768) ? 2 : 4;
      }

      $(window).load(function () {

        $("#service-slider").fadeIn("slow");
        $("#service-slider-carousel").fadeIn("slow");

        // The slider being synced must be initialized first
        $("#service-slider-carousel").flexslider({
          animation: "slide",
          controlNav: false,
          animationLoop: false,
          slideshow: false,
          itemWidth: 172.5,
          itemMargin: 20,
          prevText: "",
          nextText: "",
          asNavFor: "#service-slider",
          minItems: getGridSize(), // use function to pull in initial value
          maxItems: getGridSize(), // use function to pull in initial value
          start: function (slider) {
            flexslider = slider;
          }
        });

        $("#service-slider").flexslider({
          useCSS: false,
          animation: "slide",
          controlNav: false,
          directionNav: false,
          animationLoop: false,
          slideshow: false,
          sync: "#service-slider-carousel"
        });

      });

      // check grid size on resize event
      $window.resize(function () {
        var gridSize = getGridSize();
        flexslider.vars.minItems = gridSize;
        flexslider.vars.maxItems = gridSize;
      });
    }
  };
}(jQuery));
