This module adds subscription features for Google Groups to Drupal.

The module works by sending an email to your Google Group's subscription address.
The user will still have to verify their address by clicking the link in Google's 
confirmation email.

Currently, it is designed to only work with a single Google Group for a site.

A fully themable block is provided with a subscription form. This could also easily 
be turned into just a link to the group by overriding the template file. This 
block is best used for anonymous users.

The abiltiy to put a checkbox on the registration form is also included with a
setting for the default value. Also has the ability to hide the registration 
checkbox to force user subscription.

Future features:
  - Users with permission will be able to have subscribe and unsubscibe via their 
    account edit page
  - Allow management of multiple Google Groups