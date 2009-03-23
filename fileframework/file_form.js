// $Id: file_form.js,v 1.1 2009/02/12 10:23:06 miglius Exp $

Drupal.behaviors.file = function() {
  $('input[@type=file]').change(function() {
    if (this.value != '' && $('#edit-title').val() == '') {
      var title = this.value.match(/([^\/\\]+)$/)[1];
      $('#edit-title').val(title);
    }
  });
};

