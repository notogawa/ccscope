=======
CCscope
=======

About
=====

This is a Continuous Cscope_ index updater.

Install
=======

CCscope depends on pficommon_. Install pficommon_ and type

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
Cscope_ index is automatically updated.
If you modify "cscope.files" itself,
"cscope.files" is automatically reloaded too.


.. _Cscope: http://cscope.sourceforge.net/
.. _pficommon: https://github.com/pfi/pficommon
