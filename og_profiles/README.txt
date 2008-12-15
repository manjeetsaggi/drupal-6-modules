INTO
--------------------------------------------------

This module is based on the core Profiles module except each field is restrict to a group.

Group admins can define fields that the user can fill in. Group admins can choose whether the user should fill this in when they join they group and change the fields using the Manage Membership link.

This will create another member list which takes the profile fields set to show up on member lists.

TODO:
1. Test the module more
2. Write better documentation for the module
- Alot of the documentation in the code is left over from the core module so I need to go through and change this
3. Finish integration with og
At the moment one line needs to be changed in og because of the way it gets fields in the manage form.
I will submit a bug report for this soon
4. Get a demo site up or at least some screenshots

INSTALL
--------------------------------------------------

At the moment one change should be made to og.module.
If you don't change this line you will get an error and the manage membership might not be saved
In og_manage_form_submit you should change the line:
  $passed_values = $form_state['values'];
with:
  $passed_values = $form['#parameters'][1]['post'];

This is so og_profiles can take the fields out that it uses whereas, as far as my knowledge goes I cannot get to $form_state['values'] in og_profiles
A fix for this would be very much appreciated!