import sys
sys.path.append( 'C:\Program Files (x86)\Google\google_appengine' )


import webapp2
import webtest
import unittest

from google.appengine.ext import db
from google.appengine.ext import testbed

from view_group import ViewGroup

from users_db import UsersDb
from msgfile_db import MsgFile
from message_db import MessageDb
from message_db import message_db_rootkey
	   
class HandlerTest(unittest.TestCase):
	
	
	def setUp(self):
		# Create a WSGI application.
		app = webapp2.WSGIApplication([
			('/group', ViewGroup), 
			])
        # Wrap the app with WebTest TestApp
		self.testapp = webtest.TestApp(app)
		
		# First, create an instance of the Testbed class
		self.testbed = testbed.Testbed()
		# Then activate the testbed, which prepares the service stubs for use.
		self.testbed.activate()		
		self.testbed.init_datastore_v3_stub()
		self.testbed.init_memcache_stub()
		
		user_ent, _ = UsersDb.my_get_or_insert('anth', pwd='foobar')
		new_msg_file = MsgFile.create_msg_file()
		user_ent.msg_file = new_msg_file
		user_ent.put()
		self.user = user_ent
			
	
	def tearDown(self):
		self.testbed.deactivate()	

	
	def test_ViewGroup_no_login(self):
		# test that non-logged in users get blocked
		response = self.testapp.get('/group', status=400)
		self.assertEqual(response.status_int, 400)
	
	def test_SentPage_login(self):
		# test that non-logged in users 
		response = self.testapp.get('/group', headers={'Cookie':'user_name=anth|ce908054e59b3e4b38891dcd7feb93d5'})
		self.assertEqual(response.status_int, 200)
	