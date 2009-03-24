if (Drupal.jsEnabled) {
	$(document).ready(function() {
		init_pickrs();
	});
	$(document).ajaxComplete(function() {
    init_pickrs();
  });
}

var init_pickrs = function() {
	if($(Drupal.settings.flickrpickr.radioclass).get(0)
	&& $(Drupal.settings.flickrpickr.imageclass).get(0)
	&& $(Drupal.settings.flickrpickr.imglinkclass).get(0)) { 
		$(Drupal.settings.flickrpickr.imglinkclass).click(function() {
			var id = $(this).attr('id');
			var selector = 'input#edit-' + id;
			$(selector).attr('checked', 'checked');
			return false;
		});
	}
}