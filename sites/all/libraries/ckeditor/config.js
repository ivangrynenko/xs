/**
 * @license Copyright (c) 2003-2014, CKSource - Frederico Knabben. All rights reserved.
 * For licensing, see LICENSE.md or http://ckeditor.com/license
 */

CKEDITOR.editorConfig = function( config ) {
	// Define changes to default configuration here. For example:
	// config.language = 'fr';
	// config.uiColor = '#AADC6E';
  config.extraPlugins = 'pbckcode';
  // ADVANCED CONTENT FILTER (ACF)
  //     // ACF protects your CKEditor instance of adding unofficial tags
  //         // however it strips out the pre tag of pbckcode plugin
  //             // add this rule to enable it, useful when you want to re edit a post
  //                 // Only needed on v1.1.x and v1.2.0
  config.allowedContent= 'pre[*]{*}(*)';
  config.pbckcode = {
    cls : '',

    highlighter : 'PRETTIFY',

    modes : [ ['HTML', 'html'], ['CSS', 'css'], ['PHP', 'php'], ['JS', 'javascript'] ],

    theme : 'textmate',

    tab_size : '2',

    js : "http://cdn.jsdelivr.net//ace/1.1.4/noconflict/"
  };
};
