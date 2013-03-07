watchpuppy
==========

[![Build Status](https://travis-ci.org/schoentoon/watchpuppy.png)](https://travis-ci.org/schoentoon/watchpuppy)

Useful for testing server software that is supposed to run 24/7. In case it exits it'll just restart the specified program. To test if your software is still running you can tell watchpuppy to just check a certain tcp port your software will listen to every tick. You can also extend it in any way you like by using the hooks feature, you can for example execute a script to email you in case your software is restarted by watchpuppy.

Notes:

In case you're using the tcp port check please keep in mind that watchpuppy will simply connect and disconnect again without reading or writing any data. Be sure to handle this correctly in your own software. I recommend to (in case of one thread per socket) not create the thread in case the socket is instantly closed.

Currently both the -e and the -H flags will not look in your PATH so you'll have to supply the full path to your binary, or simply do something like $ watchpuppy -e `which <program>`. This might get 'fixed' in the future..
