// $Id: facebook_stream.js,v 1.1 2009/06/06 05:22:59 prajwala Exp $
/*jsl:option explicit*/
/*jsl:declare window*/
/*jsl:declare document*/
/*jsl:declare $*/
/*jsl:declare Drupal*/
/*jsl:declare FB*/
/*jsl:declare setTimeout*/

// got the permissions success or failure from the facebook
// just reloads the page after getting the permissions
function got_permissions(value){
    if (value) {
	window.location=document.location.href;
    }		
}

// prompt the user for the specified permission
function prompt_for_permissions(permission) {
    FB.Connect.showPermissionDialog(permission,got_permissions);
}

Drupal.behaviors.fbstream = function(){
    $('#post_dialog').dialog({
	autoOpen: false,
	width: 415,
	buttons: {
	    "Ok": function() { 
		$(this).dialog("close"); 
	    }
	}
    });
    // the display of fbstream block full or short
    if (!Drupal.settings.fbstream_dispaly) {
	// if display is short enable events for the more link
	$('.more_link a').click(function(){
	    var ele = $(this);
	    var post_id = ele.attr('post_id');
	    var moreInfoEle = $('.more_info[post_id="'+post_id+'"]');
	    if (moreInfoEle.css('display') == 'none') {
		moreInfoEle.show('slow');
		ele.text('Hide more info');
	    }
	    else{
		moreInfoEle.hide('slow');
		ele.text('more');
	    }
	    return false;
	});
    }

    // assign events for the comments link
    $('.time_comments_likes .comments a').click(function(){
	var ele = $(this);
	var post_id = ele.attr('post_id');
	$('#post_dialog').dialog('option', 'title', 'Comments of post');
	$('#post_dialog').html('<p>Loading comments please wait ...</p>');
	$('#post_dialog').dialog('open');

	$.post(Drupal.settings.basePath+'?q=fbstream/get/post/comments',
	       {'post_id':post_id},function(data){
		   $('#post_dialog').html('<p>'+data+'</p>');
	       });
	
	return false;
    });

    // assign events for the .likes link
    $('.time_comments_likes .likes a').click(function(){
	var ele = $(this);
	var post_id = ele.attr('post_id');
	$('#post_dialog').dialog('option', 'title', 'Friends who like this post');
	$('#post_dialog').html('<p>Loading likes information please wait ...</p>');
	$('#post_dialog').dialog('open');

	$.post(Drupal.settings.basePath+'?q=fbstream/get/post/likes',
	       {'post_id':post_id},function(data){
		   $('#post_dialog').html('<p>'+data+'</p>');
	       });
	
	return false;
    });

    // update the stream data for based on the updation time
    if (typeof(Drupal.settings.fbstream_update_time) != 'undefined') {
	var time = Drupal.settings.fbstream_update_time*60*1000;
	setTimeout('fbstream_update()',time);
    }
};

function fbstream_update(){
    $.get(Drupal.settings.basePath+'?q=fbstream/get',function(data){
	$('.fbstream_short_container').replaceWith(data);
	Drupal.behaviors.fbstream();
    });
}

