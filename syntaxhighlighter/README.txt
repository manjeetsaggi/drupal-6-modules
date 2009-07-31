REQUIREMENTS
-------------

Download the latest Syntax highlighter javascript library from http://alexgorbatchev.com/wiki/SyntaxHighlighter,
extract the content and place it inside any of the following directories or sub-directory of any name therein(*):

  -  sites/all/libraries      (multisite install can share the same library install here)
  -  site-file-directory-path (usually sites/<site_domain>/files but can be changed through 'admin/settings/file-system'
  -  inside the syntaxhighlighter module directory
  
Examples:

sites/all/libraries/js/syntaxhighlighter_2.0.320
site-file-directory-path

(*) The directory name 'src' is not allowed. Do not place syntaxhighlighter library inside there.


NOTES
-----

You must use the Full HTML format because the HTML filter modifies the class attribute.