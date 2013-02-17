watchpuppy
==========

Useful for testing server software that is supposed to run 24/7. In case it exits it'll just restart the specified program. To test if your software is still running you can tell watchpuppy to just check a certain tcp port your software will listen to every tick. You can also extend it in any way you like by using the hooks feature, you can for example execute a script to email you in case your software is restarted by watchpuppy.
