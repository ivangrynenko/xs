<div id="slideshow" class="fullwidthbanner-container main-slider hidden-xs">
  <?php $rs_effect = theme_get_setting('rs_slideshow_full_effect'); ?>
  <div class="fullwidthbanner">
    <ul>
      <?php foreach ($rows as $id => $row) { ?>

        <?php $view = views_get_current_view();
        $nid = $view->result[$id]->nid;
        $node = node_load($nid);
        $lang = 'und';
        $nodeurl = url('node/' . $node->nid);

        $image = image_style_url('slideshow', $node->field_teaser_image[$lang][0]['uri']);
        $title = $node->field_teaser_image[$lang][0]['title'];
        $alt = $node->field_teaser_image[$lang][0]['alt'];

        if ($node->type == 'mt_slideshow_entry') { ?>
          <?php if ($node->field_slideshow_entry_path) { ?>
            <?php $path = $node->field_slideshow_entry_path[$lang][0]['value']; ?>
            <li data-link="<?php print url($path); ?>" data-transition="<?php print $rs_effect ?>" data-masterspeed="400">
          <?php }
          else { ?>
            <li data-transition="<?php print $rs_effect ?>" data-masterspeed="400">
          <?php } ?>

          <img data-bgposition="center top" src="<?php print $image; ?>" title="<?php print $title; ?>" alt="<?php print $alt; ?>"/>

          <div class="tp-caption title-teaser-text sft fadeout"
               data-x="left"
               data-y="bottom"
               data-speed="8000"
               data-start="800"
               data-voffset="-30"
               data-easing="Power0.easeIn">
            <div class="title">
              <?php $title = $node->title; ?>
              <?php if ($node->field_slideshow_entry_path) { ?>
                <?php $path = $node->field_slideshow_entry_path[$lang][0]['value']; ?>
                <a href="<?php print url($path); ?>"><?php print $title; ?></a>
              <?php }
              else { ?>
                <?php print $title; ?>
              <?php } ?>
            </div>
            <?php if ($node->field_teaser_text): ?>
              <?php $field_teaser_text = $node->field_teaser_text[$lang][0]['value']; ?>
              <p><?php print $field_teaser_text; ?></p>
            <?php endif; ?>
          </div>

          <div class="tp-caption caption-link sft fadeout"
               data-x="right"
               data-y="bottom"
               data-speed="800"
               data-start="800"
               data-voffset="-30"
               data-easing="Power0.easeIn">
            <?php if ($node->field_slideshow_entry_path): ?>
              <?php $path = $node->field_slideshow_entry_path[$lang][0]['value']; ?>
              <a href="<?php print url($path); ?>" class="more"><i class="fa fa-info-circle"></i> <?php print t('View details'); ?>
              </a>
            <?php endif; ?>
          </div>
          </li>
        <?php }
        else { ?>
          <li data-transition="<?php print $rs_effect ?>" data-link="<?php print $nodeurl ?>" data-masterspeed="400">
            <img data-bgposition="center top" src="<?php print $image; ?>" title="<?php print $title; ?>" alt="<?php print $alt; ?>"/>
            <?php print $row; ?>
          </li>
        <?php } ?>

      <?php } ?>
    </ul>
    <div class="tp-bannertimer tp-bottom"></div>
  </div>
</div>
