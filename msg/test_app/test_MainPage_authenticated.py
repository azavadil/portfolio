import sys
sys.path.append( 'C:\Program Files (x86)\Google\google_appengine' )

import webapp2
import webtest
import unittest

from google.appengine.ext import db
from google.appengine.ext import testbed

from main_page import MainPage	   

from users_db import UsersDb
from msgfile_db import MsgFile


class HandlerTest(unittest.TestCase):
	
	
	def setUp(self):
		# Create a WSGI application.
		app = webapp2.WSGIApplication([
			('/', MainPage)
			])
        # Wrap the app with WebTest TestApp
		self.testapp = webtest.TestApp(app)
		
		# First, create an instance of the Testbed class
		self.testbed = testbed.Testbed()
		# Then activate the testbed, which prepares the service stubs for use.
		self.testbed.activate()		
		self.testbed.init_datastore_v3_stub()
		self.testbed.init_memcache_stub()	
		
		user_ent, _ = UsersDb.my_get_or_insert('anth', pwd='Foobar1')
		new_msg_file = MsgFile.create_msg_file()
		user_ent.msg_file = new_msg_file
		user_ent.put()
		self.user = user_ent
		
	
	def tearDown(self):
		self.testbed.deactivate()	

	# Test the MainPage handler
	def test_MainPage_no_authentication(self):
		response = self.testapp.get('/')
		self.assertEqual(response.status_int, 200)
		self.assertEqual(response.content_type, 'text/html')
	
	# Test the MainPage handler
	# We add a cookie generated using the make_secure_val function to simulate 
	# authentication 
	
	def test_MainPage_authenticated(self):
		response = self.testapp.get('/', headers={'Cookie':'user_name=anth|ce908054e59b3e4b38891dcd7feb93d5'})
		self.assertEqual(response.status_int, 200)
		self.assertEqual(response.content_type, 'text/html')
		contains_username = 'anth' in response.body
		self.assertTrue(contains_username)
	
	def testMainPage_login(self):
		response = self.testapp.post('/', {'username':'anth','pwd1':'Foobar1'})
		self.assertEqual(response.status_int, 200)
		self.assertEqual(response.content_type, 'text/html')
		contains_username = 'anth' in response.body
		self.assertTrue(contains_username)
	
	
	