<?php
  //$Id: comments_display.tpl.php,v 1.3 2009/06/11 14:05:43 prajwala Exp $
?>
<?php  $fb = facebook_client(); ?> 
 <div class = "fb_stream_post_comments">
    <?php  
       foreach ($comments as $comment) {
         $user_info = $fb->api_client->users_getInfo($comment['fromid'], 'name,pic_square,profile_url');
         $user_info = $user_info[0];
    ?>
      <div class = "fb_post_comment <?php echo $string = str_replace (" ", "",$comment['id']) ?> ">
         <div class = "fb_comment_user_picture"><img src = "<?php echo $user_info[pic_square]; ?>" />
         </div>
         <div class = "fb_comment_message">
            <a href = "<?php echo $user_info[profile_url]; ?> "><?php echo $user_info['name'] ?> </a>
	 <span class = "fb_comment_user_message"><?php echo str_parse_url(check_plain($comment['text'])); ?> </span>
      <?php $time = time_duration(time() - $comment['time']).' ago'; ?>
            <div class = "fb_comment_created"><?php echo $time ?>
            </div>
      </div>
      <div style = "clear:both;"></div>
     </div>
    <?php  } ?>
 </div>
 

