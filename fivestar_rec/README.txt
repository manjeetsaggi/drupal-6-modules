README file for Fivestar Recommender

This module, once enabled, provides 4 blocks:
1) Users who voted similarly as you
2) Recommended nodes from users who voted similarly as you
3) Other nodes voted similar to this one
4) You might also interest in these nodes based on your previous votes

The first 2 blocks are computed using the User-to-user algorithm. The first
block returns a list of users who have made similar votes as the current user.
The second block then recommends a list of nodes to the current user that were
highly voted by those users from the first block. For detailsof this algorithm,
please refer to: http://portal.acm.org/citation.cfm?doid=192844.192905

The other 2 blocks are computed using the Item-to-item algorithm. Block (3)
returns a list of nodes in the sense of "users who liked this node also liked".
It's different from block (2) because block (2) is personalized towards the
current user, where as block (3) is not personalized. Block (4) returns a list
of nodes similar to the nodes you voted highly. The "item-to-item" algorithm is
also used by Amazon. Please refer to http://portal.acm.org/citation.cfm?id=642471
for more details.

This module requires Recommender API at http://drupal.org/project/recommender.
After installation/configuration, please go to admin/settings/recommender and
generate the recommendations, then enable the 4 blocks as needed.

For more information, please go to the project homepage at:
http://drupal.org/project/fivestar_rec