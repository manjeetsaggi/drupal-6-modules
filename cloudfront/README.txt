Description
==============================
This module integrates with imagecache to serve dynamic images from the Amazon CloudFront CDN. Dynamic images are placed in a queue and served immediately from the local filesystem. Once the queue is processed (via cron job) the images will automatically begin serving from CloudFront.

If you flush or modify a preset it will fall back to local hosting so that the changes are visible immediately. The new images will begin serving from CloudFront the next time the queue is processed.

This module requires no patches to either core or third party modules.

Here's how it works:
1) Install the module and override theme_imagecache in your theme to call cloudview_theme_imagecache
2) Create an imagecache preset and add the 'Send to CloudFront' action at the end.
3) The first time imagecache creates a dynamic image it will be placed in the cloudfront queue. Until the queue is processed the image will be served from the local filesystem.
4) A cron job loads cloudfront/processqueue and all queued images are sent to S3.
5) Once an image has been sent to S3 its image tags will be rewritten to serve from CloudFront

It is safe to point multiple presets at the same bucket. Images are stored on S3 with path <bucket>/preset_id_<#>/<imagefile>

The advantages of this system are:
1) Queuing system doesn't introduce latency for page views unlike sending the images in real time to s3.
2) You can fall back to serving images from the local filesystem at any time.
3) Images are only re-sent to s3 if they are regenerated by imagecache. This means you can make changes to or flush a preset and the new images will show up on the site immediately and start serving from CloudFront after the next queue processing.

Install
==============================
Here's an overview -- there will be more details below.
1) Place the entire cloudfront folder into your modules directory.
2) Add your AWS access key and secret key to settings.php (example below).
3) Enable the module from the Administer->Modules page.
4) Add the 'Send to CloudFront' action to the end of any imagecache presets
5) Override theme_imagecache in your theme and have it call cloudview_theme_imagecache
6) Set up a cron job to access <base_url>/cloudfront/processqueue

Settings.php:
------------------------------
You will need to add your aws keys to your settings.php file like so:
$conf = array(
  'cloudfront_access_key_id' => '<access key>',
  'cloudfront_secret_access_key' => '<secret access key>',
);

The 'Send to CloudFront' imagecache action options:
------------------------------
Bucket - any image processed with the preset will be sent to this bucket
Distribution URL - you will get this when you set up a distribution for cloudfront. This should be fully qualified like http://images.<yoursite>.com
Unique Name - This must be a name that is not used by any other instance of this action. 
This should be the last action in a preset.

Override theme_imagecache:
------------------------------
You can override the theme_imagecache function by placing a function called <theme_name>_imagecache in you theme.php file. Here's an example:
function garland_imagecache($namespace, $path, $alt = '', $title = '', $attributes = null) {
  if (module_exists('cloudfront')) {
    return cloudview_theme_imagecache($namespace, $path, $alt, $title, $attributes);
  }
  else {
    return theme_imagecache($namespace, $path, $alt = '', $title = '', $attributes = null);
  }
}

If you have already overridden theme_imagecache then take a look at _cloudview_theme_imagecache in cloudview.module to see how to integrate cloudview into your function.

Cron job to access <base_url>/cloudfront/processqueue
------------------------------
Images will not be sent to s3 (and not be served through cloudfront) until this url is accessed. You should set up a cron job to hit it often. It is safe to hit this as often as you want. If it is still processing a batch of files it will not start a second instance.

Warning
==============================
It is your responsibility to make sure this module functions correctly on your site. It is also your responsibility to monitor your site and AWS reports for any signs of malfunction. This module comes with no warranty and by installing it you agree to the conditions of the GNU General Public License Version 2.

lewisvance [at] gmail.com