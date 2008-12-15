<?php
// $Id: rsvp.views.inc,v 1.1 2008/11/30 21:12:07 owahab Exp $

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
  
	$data = array(
      'rsvp' => array(
        'table' => array(
          'group' => t('RSVP'),
    
          'join' => array(
            'node' => array(
              'left_field' => 'nid',
              'field' => 'nid',
              'type' => 'INNER',
            ),
          ),
        ),
        'total_rsvps' => array(
          'title' => t(' Total RSVPs'),
          'help' => t('Display total RSVPs for the node.'),
          'field' => array(
            'handler' => 'rsvp_handler_field_rsvp_total_rsvps',
            'click sortable' => true,
          ),
//          'sort' => array(
//            'handler' => 'views_rsvp_sort_handler_total_rsvp',
//          'help' => t('Sort by the total RSVPs on a node'),
//        ),
        
        ),  
    
      ),
	
	  'rsvp_invite' => array(
        'table' => array(
          'group' => t('RSVP'),
	      'base' => array(
	        'field' => 'rid',
            'title' => t('RSVP'),
            'help' => t('Displaying rsvp'),
	      ),
	
          'join' => array(
            'node' => array(
              'left_table' => 'rsvp',
	          'left_field' => 'rid',
              'field' => 'rid',
            ),
            'rsvp' => array(
              'left_field' => 'rid',
              'field' => 'rid',
            ),
          ),
        ),
        'total_invites' => array(
          'title' => t('Total Invites'),
          'help' => t('Display total invites for all RSVPs on the node.'),
          'field' => array(
            'handler' => 'rsvp_handler_field_rsvp_invite_total_invites',
            'click sortable' => true,
          ),
/*          'sort' => array(
            'help' => t('Sort by the total invites in all RSVPs on a node'),
          ),
*/
        ),    
      
      ),
	
	
	
	  
	  
    );
	
    return $data;
	
}

function rsvp_views_handlers() {
  return array(
    'handlers' => array(
      'rsvp_handler_field_rsvp_total_rsvps' => array(
        'parent' => 'views_handler_field',
      ),
      'rsvp_handler_field_rsvp_invite_total_invites' => array(
        'parent' => 'views_handler_field',
      ),
/*      'rsvp_handler_field_rsvp_invite_total_invites' => array(
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
