Module Terms Of Use

This module was written as sample code for a "How to write a module" tutorial. 
You can find that tutorial at http://11heavens.com/adding-a-checkbox-to-the-register-form-in-Drupal-6

Instructions:

1. Move or copy or extract the 'terms_of_use' folder to sites/all/modules.

2. Enable the module 'Terms of Use' on the page admin/build/modules.

3. Create a Terms of Use page at node/add/page. Do not promote the node. TAKE NOTE OF THE NODE ID.

4. Go to admin/settings/terms_of_use and TYPE THE NODE ID in the field "Node id where Terms of Use are published".

5. Save your module configuration.

6. Clear your Drupal cache at admin/settings/performance by clicking 'Clear cached data'.

7. Log out and access the registeration page at user/register. 

It will now be required for anyone wishing to sign in to check the 'I agree with these terms.' checkbox.