Started (and updated for some errors at 4.12) from

https://gist.github.com/arunk-s/c897bb9d75a6c98733d6, actually
https://github.com/arunk-s, look for "Instructions"

Which was taken from

https://stackoverflow.com/questions/15215865/netlink-sockets-in-c-using-the-3-x-linux-kernel?lq=1

Other useful links:

https://wiki.linuxfoundation.org/networking/generic_netlink_howto

1. make
1. insmod netlinkKernel.ko
1. execute netlinkUser

Currently testing under SLES15 with a 4.12 kernel (because that's what
was handy).

The goal is a Python3 program built around pyroute2 (package python3-pyroute2).
