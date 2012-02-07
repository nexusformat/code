#!/usr/bin/env python

#run unit tests

import unittest

from nxs_napi_lowlevel import NAPILowLevelTestCase


suite1 = unittest.TestLoader().loadTestsFromTestCase(NAPILowLevelTestCase)

alltests = unittest.TestSuite([suite1])

unittest.TextTestRunner(verbosity=2).run(alltests)





