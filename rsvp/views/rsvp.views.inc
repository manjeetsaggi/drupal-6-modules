<?php
// $Id: rsvp.views.inc,v 1.1.2.5 2009/03/09 21:51:54 ulf1 Exp $

/**
 * @module rsvp
 * @package rsvp - A drupal module developed for civicspace - a distribution of drupal.
 * @description Provides view support for rsvp.
 * @author crunchywelch (welch@advomatic.com)
 * @author Omar Abdel-Wahab (owahab@gmail.com)
 * @author Ulf Schenk (ulf@schenkunlimited.net)
 *
 */

/** 
* Declare rsvp tables to views.
* 
* @return array of tables.
 */
function rsvp_views_data() {

  $data['rsvp']['table']['group'] = t('Invitation');
  
  // Advertise this table as a possible base table
  $data['rsvp']['table']['base'] = array(
    'field' => 'rid',
    'title' => t('Invitations'),
    'help' => t('Displaying invitations'),
  );
  
  $data['rsvp']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['node']['table']['join']['rsvp'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  $data['content']['table']['join']['rsvp'] = array(
    'left_table' => 'node_revisions',
    'left_field' => 'vid',
    'field' => 'vid',
  );
  
  
  $data['rsvp']['rid'] = array(
    'title' => t('Invitation Id'),
    'help' => t('The id of the invitation.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
      'make_link' => TRUE, 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by invitation.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help' => t('Invitation ids that guests belong to.'),
      'label' => t('Invitation Id'),
    ),
  );
  
  $data['rsvp']['uid'] = array(
    'title' => t('Organizer Id'),
    'help' => t('The user Id of the organizer who created the invitation.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('uid'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by user Id.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help' => t('Ids of users that created invitations.'),
      'label' => t('Organizer Id'),
    ),
  );
  
  $data['rsvp']['nid'] = array(
    'title' => t('Node Id'),
    'help' => t('The node/event Id that the invitation is based on.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('nid'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by node/event Id.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help' => t('Node/event ids that invitations have been created for.'),
      'label' => t('Node/Event Id'),
    ),
  );

  $data['rsvp']['name'] = array(
    'title' => t('Title'),
    'click sortable' => TRUE,
    'field' => array(
      'handler' => 'rsvp_views_handler_field_rsvp_name',
      'help' => t('The subject(title) of the invitation'),
    ),
  );
  
  $data['rsvp']['timestamp'] = array(
    'title' => t('Last Update time.'),
    'help' => t('Last time when invitation has been updated.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
      'help' => t('Date when invitation has been updated.'),
      'label' => t('Update time'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
      'help' => t('Sort by time of last update.'),
    ),
  );

  $data['rsvp']['startdate'] = array(
    'title' => t('Event date'),
    'help' => t('Date of invitation.'),
    'field' => array(
      'handler' => 'rsvp_views_handler_field_rsvp_startdate',
    ),
    'filter' => array(
      'handler' => 'rsvp_views_handler_filter_rsvp_startdate',
      'help' => t('Filter on startdate of invitation.'),
      'label' => t('Startdate'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
      'help' => t('Sort by time of last update.'),
    ),
  );
  
  $data['rsvp']['total_rsvps'] = array(
    'title' => t(' Total Invitations'),
    'help' => t('Display total number of Invitations for the node.'),
    'field' => array(
      'handler' => 'rsvp_views_handler_field_rsvp_total_rsvps',
      'click sortable' => true,
    ),
//          'sort' => array(
//            'handler' => 'views_rsvp_sort_handler_total_rsvp',
//          'help' => t('Sort by the total RSVPs on a node'),
//        ),
        
  );  
  
  
  
  
  $data['rsvp_invite']['table']['group'] = t('Invitations');

  $data['rsvp_invite']['table']['join']['rsvp'] = array(
    'left_field' => 'rid',
    'field' => 'rid',
  );
  
  $data['rsvp_invite']['table']['join']['node'] = array(
    'left_table' => 'rsvp',
    'left_field' => 'rid',
    'field' => 'rid',
  );
/*
  $data['rsvp_invite']['rid'] = array(
    'title' => t('Invitation Id'),
    'help' => t('The invitation this guest belongs to.'),
    //'relationship' => array(
    //  'base' => 'rsvp',
    //  'field' => 'rid',
    //  'handler' => 'views_handler_relationship',
    //  'label' => t('Invitation'),
    //),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
      'help' => t('Sort by invitation.'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'help' => t('Invitation ids that guests belong to.'),
      'label' => t('Invitation Id'),
    ),
  );
*/  
  $data['rsvp_invite']['uid'] = array(
    'title' => t('User'),
    'help' => t('The user who has been invited.'),
  );
  
  $data['rsvp_invite']['email'] = array(
    'title' => t('Email Address'),
    'click sortable' => TRUE,
    'field' => array(
      'handler' => 'views_handler_field',
      'help' => t('Email address of a user (authenticated or anonymous) who received an invitation.') .' <strong>'. t('WARNING: only expose this data in a view that is restricted to users whom you can trust with such sensitive information.') .'</strong>',
    ),
  );
  
  $data['rsvp_invite']['rsvp_user_invited'] = array(
    'title' => t('Current User invited'),
    'real field' => 'uid',
    'filter' => array(
      'handler' => 'views_handler_filter_user_current',
      'help' => t('This allows you to filter by whether or not the logged in user has been invited to an event.'),
    ),
  );

  //SELECT DISTINCT n.* FROM node n LEFT JOIN rsvp r ON n.nid = r.nid LEFT JOIN rsvp_invite ri ON r.rid = ri.rid
  //WHERE ri.uid = 1 AND ri.response = 'no';
  $data['rsvp_invite']['rsvp_user_response'] = array(
    'title' => t('Response'),
    'real field' => 'response',
    'field' => array(
      'handler' => 'views_handler_field',
      'help' => t('Response of the user'),
    ),
    'filter' => array(
      'handler' => 'rsvp_views_handler_filter_user_response',
      'help' => t('This allows you to filter by the response the logged in user has replied to an event/node.'),
      'label' => t('Response:'),
    ),
  );
    
  $data['rsvp_invite']['total_invites'] = array(
    'title' => t('Total number of guests'),
    'help' => t('Display total number of guests for all invitations on the node.'),
    'field' => array(
      'handler' => 'rsvp_views_handler_field_rsvp_invite_total_invites',
      'click sortable' => true,
    ),
/*          'sort' => array(
            'help' => t('Sort by the total invites in all RSVPs on a node'),
          ),
*/
  );    
  
  
  return $data;
	
}

function rsvp_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'rsvp') .'/views',
    ),
    'handlers' => array(
      'rsvp_views_handler_filter_user_response' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'rsvp_views_handler_filter_rsvp_startdate' => array(
        'parent' => 'views_handler_filter_numeric',
      ),
      'rsvp_views_handler_field_rsvp_name' => array(
        'parent' => 'views_handler_field',
      ),
      'rsvp_views_handler_field_rsvp_startdate' => array(
        'parent' => 'views_handler_field_date',
      ),
      'rsvp_views_handler_field_rsvp_total_rsvps' => array(
        'parent' => 'views_handler_field',
      ),
      
/*      'rsvp_handler_field_rsvp_invite_total_invites' => array(
        'parent' => 'views_handler_field',
      ),
      'rsvp_handler_field_rsvp_invite_total_invites' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
*/    ),
  );
}



///////////////////

/*function views_rsvp_sort_handler_total_rsvp($action, &$query, $sortinfo, $sort) {
  $query->orderby[] = "rsvp.rsvp_total_rsvps $sort[sortorder]";
}
*/
