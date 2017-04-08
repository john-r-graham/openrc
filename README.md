# OpenRC README

OpenRC is a dependency-based init system that works with the
system-provided init program, normally `/sbin/init`. Currently, it does
not have an init program of its own.

This is a private fork on which I'm working on some experimental new
features, currently

* Ability to accept command line arguments that alter (principally) the invocation of the service daemon.
* Some cleanup and enhancement related to the above.

Please see the [main openrc](https://github.com/OpenRC/openrc) project unless you're interested in these experimental changes.
