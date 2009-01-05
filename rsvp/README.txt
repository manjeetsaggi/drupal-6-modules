This module lets users invite people to a node and maintains a list of
people who will be attending.

Requires either the event or the date module to be installed.

Installation
------------

1) Copy the rsvp directory into your Drupal modules directory.

2) Go to Administer >> Site Building >> Modules and 

   2.1 enable the RSVP module.

   2.2 enable either the 'RSVP date connector' or the 'RSVP event connector' module. Do not enable both at the same time.
   If you decide to switch from one connector to the other you have to use the uninstall option to uninstall the current connector before  
   you enable the other one.
   Be careful. Uninstalling a connector will remove all existing invitation and RSVP records. Disabling and Enabling is fine though and will not remove
   existing invitations.


Configuration
-------------
1) Enable permissions appropriate to your site.
     The rsvp module provides the following permissions:
	   "administer rsvp"      - full access
     "maintain rsvp"        - access to all invitations, but no permissions to modify the general rsvp settings.
	   "rsvp on events"       - create invitations on nodes.
	   "rsvp on own events"   - create invitations on own nodes.
	   "rsvp newsletters subscribers"  - permission to add attendees based on existing newsletters (module simplenews is required).
	   "rsvp system users"    - permission to add users based on available roles.

2) The basic difference between the event and the date module is that the date module supports reocurring events, and the creation of a Datetime or Date field 
is mandatory.

2.1)If you use the event Module:
From the RSVP perspective there is no other configuration required. You are set to create RSVPs for existing events.


2.2)If you use the date Module:
2.2.1) Create Field
   First you have to add a custom field of type "Datetime" or "Date" to all the content types that you want to utilize as potential events in the future. 
   The RSVP module can manage different "event enabled" content types, but each type needs at least one Datetime or Date field.
2.2.2) Create association(s)
   If the content types are prepared, you need to setup the content type / field associations for each content type that you want to use for RSVP's. You can do that under 
   Administrator >> Site configuration >> RSVP settings >> Assocications >> Add association. The user requires "administer rsvp" permissions to modify associations. 
   Select the content type you added a Datetime or Date field for and press the "add" link. On the following page select the field that you want to utilize. 
   If the field selection box is empty, you did not setup a proper field inside the content type.

3) Disable Wysiwig editor for the "People you like to invite" / "Add Attendees" textarea which
   is part of the manage attendees form when creating a RSVP. The Wysiwig editor should be disabled by default(Tested with FcK).

Manage invitations (Admins and Maintainers)
-------------------------------------
All invitations of all users can be found at 
   Adminstrator >> Content management >> RSVP management
   
Create invitations
------------------
Go to the node you want to create an invitation for and click on the Invitation tab.
  If the Invitation tab is not available, 
    - verify that this content type is enabled in the rsvp settings.
    - verify that the user has the right permissions ("rsvp on events" or "rsvp on own events")

Modify invitations
------------------
Automatic startdate adjustment is not supported yet. 
If the startdate of an event/node changes, the startdate of the related invitation needs to be changed
as well.




   
TODO/Features for upcoming 2.0 release:
- Tighter Integration to event nodes. Why do we need an extra Invitation page at all??
- http://drupal.org/node/293409 : Block on the side of eventnodes with the attendees
- http://drupal.org/node/344328 : notify admin of RSVP changes
- http://drupal.org/node/331423 : Can RSVP and remove RSVP be exposed as Drupal Actions, so other modules like Flag could trigger this action?
  possible actions : create Invitation, add Invitees, add Invitees(simplenews), add Invitees(roles), 
                     delete Invitation, send Message, send Invitation, remove Invitee

- http://drupal.org/node/180153 : Add invitations to "MyCalendar" 

- Add icon to send message to a specific Invitee (if allowed for user/invitees)
- Add icon to send invitation to a specific Invitee
- preview option when writing an rsvp
- automatic update of invitations startdate when startdate in node changes.
- Allow parallel usage of event connector and date connector
- allow parameters for views
- I would like to see a "reply/response" panel like in evite. Any Volunteers ?? Who can help here.
- Adding toolbox feature like in Evite:
   
   HOST TOOL BOX
    * › Edit Guest(s) Replies
    * › Send a Message to Guest(s)
    * › Remove Guest(s)
    * › Email Me when Guest(s) Reply
   OTHER OPTIONS
    * › Send a free Evite eCard
    * › Print Invitation
    * › Mobile Alerts Phone
    * › Change Organizer
    * › Export Guest List
    * › Cancel Event
    * › Copy this Event
    * › Add to my Outlook Calendar
        


