# run with
#
#    env PYTHONPATH=.libs python nxpython_test.py
#
# as  nxpython.py  needs to load  libnxpython.so
#
import nxpython
h = nxpython.nx_open("nxpython_test.nxs", nxpython.NXACC_CREATE)
nxpython.nx_close(h)
