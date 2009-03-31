// $Id: README.txt,v 1.1 2009/03/24 10:33:55 mikeytown2 Exp $
CSS gzip module for Drupal 6
==================================

Installation
------------

 1) Copy the CSS gzip module to sites/all/modules.

 2) Enable it in admin/build/modules.

 3) Enable CSS Optimization in admin/settings/performance.
 
 4) Compress the CSS even more by enabling GZip compression.


Issues
------
Certian hosts do not like mutiple .htaccess files. To get around this issue you need to copy the text below into drupal's root .htaccess file, and enable the "GZip CSS: Do not generate .htaccess file" checkbox.
Add this Inside the <IfModule mod_rewrite.c> block, right before </IfModule> (add it to the bottom)

  #CSS GZIP
  <FilesMatch "\.(css.gz)$">
    AddEncoding x-gzip .gz
    ForceType text/css
  </FilesMatch>
  RewriteCond %{HTTP_USER_AGENT} !".*Safari.*"
  RewriteCond %{HTTP:Accept-encoding} gzip
  RewriteCond %{REQUEST_FILENAME}.gz -f
  RewriteRule ^(.*)\.css $1.css.gz [L,QSA]
  #End CSS GZIP