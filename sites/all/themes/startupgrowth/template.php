<?php

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 *
 * @return
 *   A string containing the breadcrumb output.
 */
function startupgrowth_breadcrumb($variables) {

  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    $breadcrumb[] = drupal_get_title();
    return '<div>' . implode(' <span class="breadcrumb-separator"></span>', $breadcrumb) . '</div>';
  }
}

/**
 * Add classes to block.
 */
function startupgrowth_preprocess_block(&$variables) {

  $variables['title_attributes_array']['class'][] = 'title';
  $variables['classes_array'][] = 'clearfix';
}

/**
 * Override or insert variables into the html template.
 */
function startupgrowth_preprocess_html(&$variables) {
  if (empty($variables['page']['banner'])) {
    $variables['classes_array'][] = 'no-banner';
  }

  $color_scheme = theme_get_setting('color_scheme');

  if ($color_scheme != 'default') {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/style-' . $color_scheme . '.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-1' &&
    theme_get_setting('slogan_font_family') == 'slff-1' ||
    theme_get_setting('headings_font_family') == 'hff-1' ||
    theme_get_setting('paragraph_font_family') == 'pff-1'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/merriweather-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-2' ||
    theme_get_setting('slogan_font_family') == 'slff-2' ||
    theme_get_setting('headings_font_family') == 'hff-2' ||
    theme_get_setting('paragraph_font_family') == 'pff-2'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/sourcesanspro-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-3' ||
    theme_get_setting('slogan_font_family') == 'slff-3' ||
    theme_get_setting('headings_font_family') == 'hff-3' ||
    theme_get_setting('paragraph_font_family') == 'pff-3'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/ubuntu-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-4' ||
    theme_get_setting('slogan_font_family') == 'slff-4' ||
    theme_get_setting('headings_font_family') == 'hff-4' ||
    theme_get_setting('paragraph_font_family') == 'pff-4'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/ptsans-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-5' ||
    theme_get_setting('slogan_font_family') == 'slff-5' ||
    theme_get_setting('headings_font_family') == 'hff-5' ||
    theme_get_setting('paragraph_font_family') == 'pff-5'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/roboto-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-6' ||
    theme_get_setting('slogan_font_family') == 'slff-6' ||
    theme_get_setting('headings_font_family') == 'hff-6' ||
    theme_get_setting('paragraph_font_family') == 'pff-6'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/opensans-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-7' ||
    theme_get_setting('slogan_font_family') == 'slff-7' ||
    theme_get_setting('headings_font_family') == 'hff-7' ||
    theme_get_setting('paragraph_font_family') == 'pff-7'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/lato-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-8' ||
    theme_get_setting('slogan_font_family') == 'slff-8' ||
    theme_get_setting('headings_font_family') == 'hff-8' ||
    theme_get_setting('paragraph_font_family') == 'pff-8'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/roboto-condensed-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-9' ||
    theme_get_setting('slogan_font_family') == 'slff-9' ||
    theme_get_setting('headings_font_family') == 'hff-9' ||
    theme_get_setting('paragraph_font_family') == 'pff-9'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/exo-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-10' ||
    theme_get_setting('slogan_font_family') == 'slff-10' ||
    theme_get_setting('headings_font_family') == 'hff-10' ||
    theme_get_setting('paragraph_font_family') == 'pff-10'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/roboto-slab-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-11' ||
    theme_get_setting('slogan_font_family') == 'slff-11' ||
    theme_get_setting('headings_font_family') == 'hff-11' ||
    theme_get_setting('paragraph_font_family') == 'pff-11'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/raleway-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-12' ||
    theme_get_setting('slogan_font_family') == 'slff-12' ||
    theme_get_setting('headings_font_family') == 'hff-12' ||
    theme_get_setting('paragraph_font_family') == 'pff-12'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/josefin-sans-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-14' ||
    theme_get_setting('slogan_font_family') == 'slff-14' ||
    theme_get_setting('headings_font_family') == 'hff-14' ||
    theme_get_setting('paragraph_font_family') == 'pff-14'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/playfair-display-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-15' ||
    theme_get_setting('slogan_font_family') == 'slff-15' ||
    theme_get_setting('headings_font_family') == 'hff-15' ||
    theme_get_setting('paragraph_font_family') == 'pff-15'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/philosopher-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-16' ||
    theme_get_setting('slogan_font_family') == 'slff-16' ||
    theme_get_setting('headings_font_family') == 'hff-16' ||
    theme_get_setting('paragraph_font_family') == 'pff-16'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/cinzel-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-17' ||
    theme_get_setting('slogan_font_family') == 'slff-17' ||
    theme_get_setting('headings_font_family') == 'hff-17' ||
    theme_get_setting('paragraph_font_family') == 'pff-17'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/oswald-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-18' ||
    theme_get_setting('slogan_font_family') == 'slff-18' ||
    theme_get_setting('headings_font_family') == 'hff-18' ||
    theme_get_setting('paragraph_font_family') == 'pff-18'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/playfairdisplaysc-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-19' ||
    theme_get_setting('slogan_font_family') == 'slff-19' ||
    theme_get_setting('headings_font_family') == 'hff-19' ||
    theme_get_setting('paragraph_font_family') == 'pff-19'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/cabin-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-20' ||
    theme_get_setting('slogan_font_family') == 'slff-20' ||
    theme_get_setting('headings_font_family') == 'hff-20' ||
    theme_get_setting('paragraph_font_family') == 'pff-20'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/notosans-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-22' ||
    theme_get_setting('slogan_font_family') == 'slff-22' ||
    theme_get_setting('headings_font_family') == 'hff-22' ||
    theme_get_setting('paragraph_font_family') == 'pff-22'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/droidserif-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-23' ||
    theme_get_setting('slogan_font_family') == 'slff-23' ||
    theme_get_setting('headings_font_family') == 'hff-23' ||
    theme_get_setting('paragraph_font_family') == 'pff-23'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/ptserif-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-24' ||
    theme_get_setting('slogan_font_family') == 'slff-24' ||
    theme_get_setting('headings_font_family') == 'hff-24' ||
    theme_get_setting('paragraph_font_family') == 'pff-24'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/vollkorn-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-25' ||
    theme_get_setting('slogan_font_family') == 'slff-25' ||
    theme_get_setting('headings_font_family') == 'hff-25' ||
    theme_get_setting('paragraph_font_family') == 'pff-25'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/alegreya-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-26' ||
    theme_get_setting('slogan_font_family') == 'slff-26' ||
    theme_get_setting('headings_font_family') == 'hff-26' ||
    theme_get_setting('paragraph_font_family') == 'pff-26'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/notoserif-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-27' ||
    theme_get_setting('slogan_font_family') == 'slff-27' ||
    theme_get_setting('headings_font_family') == 'hff-27' ||
    theme_get_setting('paragraph_font_family') == 'pff-27'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/crimsontext-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-28' ||
    theme_get_setting('slogan_font_family') == 'slff-28' ||
    theme_get_setting('headings_font_family') == 'hff-28' ||
    theme_get_setting('paragraph_font_family') == 'pff-28'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/gentiumbookbasic-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-29' ||
    theme_get_setting('slogan_font_family') == 'slff-29' ||
    theme_get_setting('headings_font_family') == 'hff-29' ||
    theme_get_setting('paragraph_font_family') == 'pff-29'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/volkhov-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  if (theme_get_setting('sitename_font_family') == 'sff-31' ||
    theme_get_setting('slogan_font_family') == 'slff-31' ||
    theme_get_setting('headings_font_family') == 'hff-31' ||
    theme_get_setting('paragraph_font_family') == 'pff-31'
  ) {
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/alegreyasc-font.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  drupal_add_css(path_to_theme() . '/fonts/sourcecodepro-font.css', array(
    'group' => CSS_THEME,
    'type' => 'file'
  ));

  drupal_add_css(path_to_theme() . '/fonts/ptserif-blockquote-font.css', array(
    'group' => CSS_THEME,
    'type' => 'file'
  ));

  drupal_add_css('//maxcdn.bootstrapcdn.com/font-awesome/4.2.0/css/font-awesome.min.css', array('type' => 'external'));

  drupal_add_css(path_to_theme() . '/ie9.css', array(
    'group' => CSS_THEME,
    'browsers' => array('IE' => '(IE 9)&(!IEMobile)', '!IE' => FALSE),
    'preprocess' => FALSE
  ));

  /**
   * Add local.css file for CSS overrides.
   */
  drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/local.css', array(
    'group' => CSS_THEME,
    'type' => 'file'
  ));

  /**
   * Add Javascript for enable/disable Bootstrap 3 Javascript.
   */
  if (theme_get_setting('bootstrap_js_include', 'startupgrowth')) {
    drupal_add_js(drupal_get_path('theme', 'startupgrowth') . '/bootstrap/js/bootstrap.min.js');
  }
  //EOF:Javascript

  /**
   * Add Javascript for enable/disable scrollTop action.
   */
  if (theme_get_setting('scrolltop_display')) {

    drupal_add_js('jQuery(document).ready(function($) {
		$(window).scroll(function() {
			if($(this).scrollTop() != 0) {
				$("#toTop").addClass("show");
			} else {
				$("#toTop").removeClass("show");
			}
		});

		$("#toTop").click(function() {
			$("body,html").animate({scrollTop:0},800);
		});

		});',
      array('type' => 'inline', 'scope' => 'header'));
  }
  //EOF:Javascript

  /**
   * Add Javascript for responsive mobile menu
   */
  if (theme_get_setting('responsive_menu_state')) {

    if (theme_get_setting('responsive_menu_optgroups')) {
      drupal_add_js(drupal_get_path('theme', 'startupgrowth') . '/js/jquery.mobilemenu.js');
    }
    else {
      drupal_add_js(drupal_get_path('theme', 'startupgrowth') . '/js/jquery.mobilemenu-no-optgroups.js');
    }

    $responsive_menu_nested = theme_get_setting('responsive_menu_nested');
    $responsive_menu_switchwidth = (int) theme_get_setting('responsive_menu_switchwidth');
    $responsive_menu_topoptiontext = theme_get_setting('responsive_menu_topoptiontext');
    drupal_add_js(array('startupgrowth' => array('topoptiontext' => $responsive_menu_topoptiontext)), 'setting');
    $responsive_menu_nested = theme_get_setting('responsive_menu_nested');

    drupal_add_js('jQuery(document).ready(function($) {

		$("#main-navigation ul.main-menu, #main-navigation .content>ul.menu").mobileMenu({
			prependTo: "#main-navigation",
			combine: false,
			nested: ' . $responsive_menu_nested . ',
			switchWidth: ' . $responsive_menu_switchwidth . ',
            topOptionText: Drupal.settings.startupgrowth[\'topoptiontext\']
		});

		});',
      array('type' => 'inline', 'scope' => 'header'));
  }
  //EOF:Javascript

  /**
   * Add Javascript for enable/disable Isotope Javascript.
   */
  if (theme_get_setting('isotope_js_include', 'startupgrowth')) {

    drupal_add_js(drupal_get_path('theme', 'startupgrowth') . '/js/isotope/isotope.pkgd.js');

    drupal_add_js('
		jQuery(document).ready(function($) {

	    $(window).load(function() {

        $(".filters").fadeIn("slow");
        $(".filter-items").fadeIn("slow");
        var container = $(".filter-items"),
        filters= $(".filters");

        container.isotope({
            itemSelector: ".isotope-item",
            layoutMode : "masonry",
            transitionDuration: "0.6s",
            filter: "*"
        });

		$(".filters").prepend( "<li class=\"active\"><a href=\"#\" data-filter=\"*\">All</a></li>" );
        filters.find("a").click(function(){
            var $this = $(this);
            var selector = $this.attr("data-filter").replace(/\s+/g, "-");

            filters.find("li.active").removeClass("active");
            $this.parent().addClass("active");

            container.isotope({ filter: selector });
            return false;
        });


	    });

		});', array('type' => 'inline', 'scope' => 'footer', 'weight' => 2)
    );
  }
  //EOF:Javascript

  $fixed_header = theme_get_setting('fixed_header');
  if ($fixed_header) {

    /**
     * Add Javascript
     */
    drupal_add_js('jQuery(document).ready(function($) {

			var	headerHeight = $("#header").height();
			$(window).scroll(function() {
			if(($(this).scrollTop() > headerHeight) && ($(window).width() > 767)) {
				$("body").addClass("onscroll");
				$("body").css("paddingTop", (headerHeight)+"px");
				if( $(this).scrollTop() > headerHeight+40 ) {
				$("body").addClass("show");
				}
			} else {
				$("body").removeClass("onscroll");
				$("body").removeClass("show");
				$("body").css("paddingTop", (0)+"px");
				$("body.logged-in").css("paddingTop", (64)+"px");
			}
			});
		});',
      array('type' => 'inline', 'scope' => 'header'));
    //EOF:Javascript

  }

  $responsive_meanmenu = theme_get_setting('responsive_multilevelmenu_state');

  if ($responsive_meanmenu) {

    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/js/meanmenu/meanmenu.css');
    drupal_add_js(drupal_get_path('theme', 'startupgrowth') . '/js/meanmenu/jquery.meanmenu.min.js', array('preprocess' => FALSE));

    /**
     * Add Javascript
     */
    drupal_add_js('jQuery(document).ready(function($) {

			$("#main-navigation .sf-menu, #main-navigation .content>ul.menu, #main-navigation ul.main-menu").wrap("<div class=\'meanmenu-wrapper\'></div>");
			$("#main-navigation .meanmenu-wrapper").meanmenu({
				meanScreenWidth: "767",
				meanRemoveAttrs: true,
				meanMenuContainer: "#header-inside",
				meanMenuClose: ""
			});

		});',
      array('type' => 'inline', 'scope' => 'header'));
    //EOF:Javascript

  }

  $parallax_state = theme_get_setting('parallax_state');

  if ($parallax_state) {
    $variables['classes_array'][] = 'parallax-active';
    $parallax_opacity = (int) theme_get_setting('parallax_opacity') / 100;

    /**
     * Add Javascript
     */
    drupal_add_js('jQuery(document).ready(function($) {

			$(window).load(function() {
				$("#highlighted-bottom-transparent-bg").css("backgroundColor", "rgba(255,255,255,' . $parallax_opacity . ')");
			});

		});',
      array('type' => 'inline', 'scope' => 'header'));
    //EOF:Javascript

  }
}

/**
 * Override or insert variables into the html template.
 */
function startupgrowth_process_html(&$vars) {

  $classes = explode(' ', $vars['classes']);
  $classes[] = theme_get_setting('sitename_font_family');
  $classes[] = theme_get_setting('slogan_font_family');
  $classes[] = theme_get_setting('headings_font_family');
  $classes[] = theme_get_setting('paragraph_font_family');
  $classes[] = theme_get_setting('form_style');
  $vars['classes'] = trim(implode(' ', $classes));
}

/**
 * Preprocess variables for page template.
 */
function startupgrowth_preprocess_page(&$variables) {
  if (!empty($variables['node'])) {
    $numberOfImages = 0;
    if (!empty($variables['node']->field_image[LANGUAGE_NONE])) {
      $numberOfImages = count($variables['node']->field_image[LANGUAGE_NONE]);
    }

    if ($numberOfImages > 100) {
      drupal_add_js(drupal_get_path('theme', 'startupgrowth') . '/js/flexslider/rh.flexslider.js', array(
          'scope' => 'footer',
          'weight' => 4
        )
      );
    }
  }

  // Insert variables into the page template.
  if (isset($variables['node']) && $variables['node']->type != 'page') {
    if ($variables['page']['sidebar_first'] && $variables['page']['sidebar_second']) {
      $variables['main_grid_class'] = 'col-md-6';
      $variables['sidebar_grid_class'] = 'col-md-3';
    }
    elseif ($variables['page']['sidebar_first'] && !$variables['page']['sidebar_second']) {
      $variables['main_grid_class'] = 'col-md-8';
      $variables['sidebar_grid_class'] = 'col-md-4 fix-sidebar-first';
    }
    elseif (!$variables['page']['sidebar_first'] && $variables['page']['sidebar_second']) {
      $variables['main_grid_class'] = 'col-md-8';
      $variables['sidebar_grid_class'] = 'col-md-4 fix-sidebar-second';
    }
    else {
      $variables['main_grid_class'] = 'col-md-8 col-md-offset-2';
      $variables['sidebar_grid_class'] = '';
    }
  }
  else {
    if ($variables['page']['sidebar_first'] && $variables['page']['sidebar_second']) {
      $variables['main_grid_class'] = 'col-md-6';
      $variables['sidebar_grid_class'] = 'col-md-3';
    }
    elseif ($variables['page']['sidebar_first'] && !$variables['page']['sidebar_second']) {
      $variables['main_grid_class'] = 'col-md-8';
      $variables['sidebar_grid_class'] = 'col-md-4 fix-sidebar-first';
    }
    elseif (!$variables['page']['sidebar_first'] && $variables['page']['sidebar_second']) {
      $variables['main_grid_class'] = 'col-md-8';
      $variables['sidebar_grid_class'] = 'col-md-4 fix-sidebar-second';
    }
    else {
      $variables['main_grid_class'] = 'col-md-12';
      $variables['sidebar_grid_class'] = '';
    }
  }

  if ($variables['page']['highlighted_bottom_right'] && $variables['page']['highlighted_bottom_left']) {
    $variables['highlighted_bottom_left_grid_class'] = 'col-md-8';
    $variables['highlighted_bottom_right_grid_class'] = 'col-md-4';
  }
  elseif ($variables['page']['highlighted_bottom_right'] || $variables['page']['highlighted_bottom_left']) {
    $variables['highlighted_bottom_right_grid_class'] = 'col-md-12';
    $variables['highlighted_bottom_left_grid_class'] = 'col-md-12';
  }
}

/**
 * Implements hook_preprocess_maintenance_page().
 */
function startupgrowth_preprocess_maintenance_page(&$variables) {

  $color_scheme = theme_get_setting('color_scheme');
  if ($color_scheme != 'default') {
    $color_scheme = $color_scheme == 'grey' ? 'gray' : $color_scheme;
    drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/style-' . $color_scheme . '.css', array(
      'group' => CSS_THEME,
      'type' => 'file'
    ));
  }

  drupal_add_css(drupal_get_path('theme', 'startupgrowth') . '/fonts/lato-font.css', array(
    'group' => CSS_THEME,
    'type' => 'file'
  ));
}

function startupgrowth_page_alter($page) {

  $mobileoptimized = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'MobileOptimized',
      'content' => 'width'
    )
  );
  $handheldfriendly = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'HandheldFriendly',
      'content' => 'true'
    )
  );
  $viewport = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1'
    )
  );
  drupal_add_html_head($mobileoptimized, 'MobileOptimized');
  drupal_add_html_head($handheldfriendly, 'HandheldFriendly');
  drupal_add_html_head($viewport, 'viewport');
}

function startupgrowth_form_alter(&$form, &$form_state, $form_id) {

  if ($form_id == 'search_block_form') {
    unset($form['search_block_form']['#title']);
    $form['search_block_form']['#title_display'] = 'invisible';
    $form_default = t('Enter terms then hit Search...');
    $form['search_block_form']['#default_value'] = $form_default;

    $form['actions']['submit']['#attributes']['value'][] = '';

    $form['search_block_form']['#attributes'] = array(
      'onblur' => "if (this.value == '') {this.value = '{$form_default}';}",
      'onfocus' => "if (this.value == '{$form_default}') {this.value = '';}"
    );
  }
}

?>
