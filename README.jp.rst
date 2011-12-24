=======
CCscope
=======

About
=====

Cscope_ インデックスの更新対象を監視し自動継続更新を行います．

Install
=======

pficommon_ に依存しているので，
最初に pficommon_ をインストールし，
以下のようにタイプしてください．

::

  $ ./configure
  $ make
  $ sudo make install

Usage
=====

インデックス対象リスト"cscope.files"を作成します． ::

  example)
  $ cd path_to_project_dir
  $ find . -name "*.[ch]pp" | sort > cscope.files

次に，インデックス更新用スクリプト"cscope.sh"を用意します． ::

  example)
  $ emacs ccsope.sh
  +#!/bin/sh
  +cscope -b -q

CCscopeを実行します． ::

  $ ccscope

"cscope.files"にリストアップされているファイルの変更を監視するようになり，
変更に応じて Cscope_ インデックスも自動的に更新されるようになります．
また，"cscope.files"自体の更新も監視しており，
リストが更新された場合も監視対象は自動でリロードされます．


.. _Cscope: http://cscope.sourceforge.net/
.. _pficommon: https://github.com/pfi/pficommon
