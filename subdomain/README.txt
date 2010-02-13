===========================================================
OVERVIEW
===========================================================
Subdomain automatically creates subdomains and assigns nodes
to those subdomains based on several administrator chosen criteria.

Currently, Subdomain supports 4 modes:

 Node Author:
  EXAMPLE: A user named "Sayuko" and her content would be located at:
  http://sayuko.example.com

 Node Content Type:
  EXAMPLE: Nodes with a content type of "News" would have URLs like
  http://news.example.com/drupal-is-the-greatest-web-tool-ever

 Organic Groups:
  EXAMPLE: A group named "Pizza lovers" & content would be located at:
  http://pizza-lowers.example.com

 Taxonomy:
  EXAMPLE: A term named "Seattle" & associated content would be located at:
  http://seattle.example.com

===========================================================
UPGRADE
===========================================================
Upgrading from any of the previous 6.x versions (i.e 6.x-1.5 or below):

Subdomain no longer needs the custom functions in settings.php
Please review your settings.php and remove the following two
functions if present (NOTE: If you've made customizations to
these functions that you still need, install the url_alter
module and migrate your customizations to a custom module
using the hooks provided by url_alter.

  function custom_url_rewrite_outbound(&$path, &$options, $original_path) {

    // Used by the Subdomain module to generate URLs with subdomains
    if (module_exists('subdomain')) {
      subdomain_url_rewrite_outbound($path, $options); 
    }
  }

  function custom_url_rewrite_inbound(&$result, $path, $path_language) {

    // Used by the Subdomain module to correctly interpret URLs with subdomains
    if (module_exists('subdomain')) {
      subdomain_url_rewrite_inbound($result, $path, $path_language); 
    }
  }  



===========================================================
INSTALL
===========================================================

STEP 1: Edit settings.php & set $cookie_domain to your site domain. e.g.:
 $cookie_domain = "example.com";

STEP 2: Enable wildcard DNS on your DNS hosting provider (e.g. *.example.com)

STEP 3: Configure wildcard virtual hosts. For apache or lighttpd,
 see below. For other web servers, consult their documentation.

STEP 4: Enable & configure Subdomain: URL aliases -> Subdomain settings

STEP 5: Configure Pathauto (URL aliases -> Automated alias settings):
 1) Go to "Punctuation Settings" and set "Tilde ~:" to "No action"
 2) Place [subdomain] at the *start* of all paths whose content you
    want placed on a subdomain


===========================================================
CONFIGURING APACHE
===========================================================
Setting up Apache varies depending on your host and server
environment and as such is beyond the scope of this document.

If your using Shared Hosting, you'll likely need to ask your
Host to set wildcard subdomains up for you. 

If you're on a VPS or dedicated server the basic steps are:

1. enable mod_rewrite
2. configure wildcard virtual hosts. 

The key for #2 is the "ServerAlias" directive. You can find 
tutorials and documention all over the internet. An example 
VirtualHost declaration might look something like this:

NameVirtualHost *:80
<VirtualHost *:80>
    ServerAdmin webmaster@example.com
    DocumentRoot /var/www/html
    ServerName example.com
    ServerAlias *.example.com

    <Directory "/var/www/html">
      AllowOverride All
    </Directory>
</VirtualHost>


===========================================================
CONFIGURING LIGHTTPD
===========================================================
STEP 1: Enable wildcard DNS on your DNS hosting provider
 (e.g. *.example.com)

STEP 2: Edit your lighttpd.conf file, enable the evhost module, 
and append the following, modifying directories as appropriate:

$HTTP["host"] =~ "([^.]+)\.([^.]+)$" {
        evhost.path-pattern = "/var/www/%2.%1/html/"
}

# Apply the following drupal rewrite rules to subdomain URLs
# moving subdomain to the 1st argument in the path
# e.g. bob.example.com news.mysite.net 
$HTTP["host"] =~ "^([^.]+)\.([^.]+)\.([^.]+)$" {
url.rewrite-final = (
  # More than one argument
  "^/([^.?]*)\?(.*)$" => "/index.php?q=_%1/$1&$2",
  # No arguments
  "^/([^.?]*)$" => "/index.php?q=_%1/$1",
 )
}


===========================================================
CREDITS
===========================================================
Authored and maintained by Stein Setvik <setvik AT gmail DOT com>