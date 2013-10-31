#!/usr/bin/python
import optparse
import sys
#sys.path.append( "G:\msgApp\msg\test_app" )

# Install the Python unittest2 package before you run this script.
import unittest2

USAGE = """%prog SDK_PATH TEST_PATH
Run unit tests for App Engine apps.

SDK_PATH    Path to the SDK installation
TEST_PATH   Path to package containing test modules"""


def main(sdk_path, test_path):
    sys.path.insert(0, sdk_path)
    import dev_appserver
    dev_appserver.fix_sys_path()
    suite = unittest2.loader.TestLoader().discover(test_path)
    unittest2.TextTestRunner(verbosity=2).run(suite)


if __name__ == '__main__':
    SDK_PATH = 'C:\Program Files (x86)\Google\google_appengine'
    TEST_PATH = 'G:\msgApp\msg'
    main(SDK_PATH, TEST_PATH)