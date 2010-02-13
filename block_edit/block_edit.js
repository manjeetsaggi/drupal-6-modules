// $Id: block_edit.js,v 1.1.2.13 2009/12/30 12:22:51 psynaptic Exp $

Drupal.behaviors.block_edit = function (context) {
  $('div.block, div.node').mouseover(function() {
    $(this).find('.node-edit-link, .block-edit-link').css('display', 'block');
  });

  $('div.block, div.node').mouseout(function() {
    $(this).find('.node-edit-link, .block-edit-link').css('display', 'none');
  });
};
