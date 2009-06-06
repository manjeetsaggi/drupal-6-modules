// $Id: README.txt,v 1.1.2.1 2008/10/28 13:11:01 arto Exp $

Killfile for Drupal
===================
Provides soft-deletion/trash bin functionality for Drupal. Content is
"killfiled" instead of deleted outright, and can be retained for a
configurable span of time before being permanently purged. All this is
opaque to end-users as all killfiled content is pervasively hidden from
them, but administrators can access and restore killfiled content when
necessary.

An index of recently killfiled content is also made available as an RSS feed
at killfile.rdf (this functionality requires the RDF module), allowing
content deletions to be propagated from one Drupal instance to another that
subscribes to its content (requires FeedAPI on the subscribing side in order
to process the content deletions). This particular functionality is intended
for use cases such as multi-instance scenarios based on the publish/
subscribe architectural model. See INSTALL.txt for information on other
modules that Killfile interoperates with.

For more information on this project please refer to:

  <http://drupal.org/project/killfile>


BUG REPORTS
-----------
Post bug reports and feature requests to the issue tracking system at:

  <http://drupal.org/node/add/project-issue/killfile>


CREDITS
-------
Developed and maintained by Arto Bendiken <http://bendiken.net/>
Sponsored by MakaluMedia Group <http://www.makalumedia.com/>
Sponsored by M.C. Dean, Inc. <http://www.mcdean.com/>
Sponsored by SPAWAR <http://www.spawar.navy.mil/>
