=======
CCscope
=======

About
=====

This is a Continuous Cscope index updater.

Install
=======

::

  $ ./configure
  $ make
  $ sudo make install

Usage
=====

At first, create a target source list "cscope.files". ::

  example)
  $ cd path_to_project_dir
  $ find . -name "*.[ch]pp" | sort > cscope.files

Next, create an index updating script "cscope.sh". ::

  example)
  $ emacs ccsope.sh
  +#!/bin/sh
  +cscope -b -q

Run. ::

  $ ccscope

If you modify any file in "cscope.files",
cscope index is automatically updated.
If you modify "cscope.files" itself,
"cscope.files" is automatically reloaded too.
