<?php
// $Id: rsvp-invitation-page.tpl.php,v 1.3.2.2 2009/03/04 20:09:36 ulf1 Exp $

/**
 * @file rsvp-invitation-page.tpl.php
 * Displays the rsvp invitation, replybox and guestlist.
 *
 * Available variables:
 *
 * $invitation:
 * $replybox:
 * $addguests:
 * $guestlist:
 * $image:
 * $icon_path:
 * $backgroundimage:

 *
 * The idea of this theme is to have the following setup
 * 
 * ------------------------------
 * |       $invitation          | 
 * ------------------------------
 * | $guestlist | $addguests    | 
 * |            | $replybox     |
 * ------------------------------ 
 *
 * @see template_preprocess_rsvp-invitation-page()
 */
?>

  <div class="rsvp_invitation_view rsvp_color_inner">
    <?php if ($invitation): ?>
      <div class="rsvp_invitation_view_top">
        <?php print $invitation ?>
      </div>
    <?php endif; ?>
    <div class="rsvp_invitation_view_bottom">
      <?php if ($guestlist): ?>
        <div class="rsvp_invitation_view_bottom_left">
          <?php print $guestlist ?>
        </div>
      <?php endif; ?>

      <div class="rsvp_invitation_view_bottom_right">
        <?php if ($addguests) : ?>
          <?php print $addguests ?>
        <?php endif; ?>
        <?php if ($replybox) : ?>
          <?php print $replybox ?>
        <?php endif; ?>
      </div>
    </div>
    <div style="clear: both">
    </div>
    
  </div>
        
  