Nooliberry
==========

Nooliberry platform for contiki OS

To integrate the nooliberry in your contiki-tree, copy/paste the 
contiki-platform/nooliberry folder inside your $CONTIKI_HOME/platform
folder.

To test your setup, you should be able to compile contiki examples 
for Nooliberry:

<pre>
  cd $CONTIKI_HOME/examples/hello-world
  make TARGET=nooliberry
</pre>
