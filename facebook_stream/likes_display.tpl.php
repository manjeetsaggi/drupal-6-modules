<?php
  $fb = facebook_client(); 
?>
  <ul class = "post_liked_friends">
<?php
  
  foreach ($friends as $friend) {
    $user_info = $fb->api_client->users_getInfo($friend, 'name, pic_square, profile_url');
    $user_info = $user_info[0];
?>
    <li>
      <span class = "square"><img src = "<?php echo $user_info['pic_square'];?>" /></span>
      <strong><a href = "<?php echo $user_info['profile_url']; ?>"><?php echo $user_info['name']; ?></a></strong>
    </li>
<?php
  }
?>
</ul>
