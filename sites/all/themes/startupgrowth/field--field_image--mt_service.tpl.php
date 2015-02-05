<?php if (!$label_hidden) : ?>
  <div class="field-label"<?php print $title_attributes; ?>><?php print $label ?>:&nbsp;</div>
<?php endif; ?>

<?php
// Reduce number of images in teaser view mode to single image
if ($element['#view_mode'] == 'teaser') : ?>
  <div class="field-item field-type-image even"<?php print $item_attributes[0]; ?>><?php print render($items[0]); ?></div>
  <?php return; endif; ?>

<?php $node = $element['#object'];?>

<?php $numberOfImages = 0;
if (!empty($node->field_image[LANGUAGE_NONE])) {
  $numberOfImages = count($node->field_image[LANGUAGE_NONE]);
} ?>

<!--<div class="images-container clearfix">-->
<?php if ($numberOfImages > 100) { ?>

  <!-- #service-slider -->
  <div id="service-slider" class="flexslider">
    <ul class="slides">
      <?php $i = 0;
      foreach ($node->field_image[LANGUAGE_NONE] as $key => $file) {
        $i++; ?>
        <li>
          <img src="<?php print image_style_url('large', $node->field_image[LANGUAGE_NONE][$key]['uri']); ?>" alt="<?php print $node->field_image[LANGUAGE_NONE][$key]['alt']; ?>" title="<?php print $node->field_image[LANGUAGE_NONE][$key]['title']; ?>"/>

          <?php if ($node->field_image[LANGUAGE_NONE][$key]['title'] || $node->field_image[LANGUAGE_NONE][$key]['alt']) : ?>
                            <div class="image-caption hidden-xs">
                                <?php if ($node->field_image[LANGUAGE_NONE][$key]['title']) :?>
                                <h4><?php print $node->field_image[LANGUAGE_NONE][$key]['title']; ?></h4>
                                <?php endif; ?>
                                <?php if ($node->field_image[LANGUAGE_NONE][$key]['alt']) :?>
                                <p><?php print $node->field_image[LANGUAGE_NONE][$key]['alt']; ?></p>
                                <?php endif; ?>
                            </div>
          <?php endif; ?>
        </li>
      <?php } ?>
    </ul>
  </div>
  <!-- EOF:#service-slider -->

  <!-- #service-slider-carousel -->
  <div id="service-slider-carousel" class="flexslider">
    <ul class="slides">
      <?php $i = 0;
      foreach ($node->field_image[LANGUAGE_NONE] as $key => $file) {
        $i++; ?>
        <li>
          <img src="<?php print image_style_url('medium', $node->field_image[LANGUAGE_NONE][$key]['uri']); ?>" alt="<?php print $node->field_image[LANGUAGE_NONE][$key]['alt']; ?>" title="<?php print $node->field_image[LANGUAGE_NONE][$key]['title']; ?>"/>
        </li>
      <?php } ?>
    </ul>
  </div>
  <!-- EOF:#service-slider-carousel -->

<?php }
elseif ($numberOfImages == 2) { ?>

  <div class="image-preview">

    <img src="<?php print image_style_url('large', $node->field_image[LANGUAGE_NONE][1]['uri']); ?>" alt="<?php print $node->field_image[LANGUAGE_NONE][1]['alt']; ?>" title="<?php print $node->field_image[LANGUAGE_NONE][1]['title']; ?>"/>

    <?php if ($node->field_image[LANGUAGE_NONE][1]['title'] || $node->field_image[LANGUAGE_NONE][1]['alt']) : ?>
      <div class="image-caption hidden-xs">
        <?php if ($node->field_image[LANGUAGE_NONE][1]['title']) : ?>
          <h4><?php print $node->field_image[LANGUAGE_NONE][1]['title']; ?></h4>
        <?php endif; ?>
        <?php if ($node->field_image[LANGUAGE_NONE][1]['alt']) : ?>
          <p><?php print $node->field_image[LANGUAGE_NONE][1]['alt']; ?></p>
        <?php endif; ?>
      </div>
    <?php endif; ?>

  </div>

<?php } ?>
