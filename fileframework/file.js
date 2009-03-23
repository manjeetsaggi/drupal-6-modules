// $Id: file.js,v 1.17 2009/02/12 10:23:06 miglius Exp $

Drupal.behaviors.file = function() {
  $('span.file.with-menu span.label .highlight').unbind('click').click(function(event) {
    $(this).parent('.label').toggleClass('active');
    $(this).parent('.label').find('ul').toggle('fast');
  });
  $(document).click(function(event) {
    $('span.file.with-menu .label').each(function() {
      if ($(event.target).parents('.label')[0] != this) {
        $(this).removeClass('active');
        $(this).find('ul').hide();
      }
    });
  });
};

