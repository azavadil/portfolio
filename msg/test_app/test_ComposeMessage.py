import sys
sys.path.append( 'C:\Program Files (x86)\Google\google_appengine' )


import webapp2
import webtest
import unittest

from compose_message import ComposeMessage
from main_page import MainPage

from google.appengine.ext import db
from google.appengine.ext import testbed

from users_db import UsersDb
from msgfile_db import MsgFile
from message_db import MessageDb
from message_db import message_db_rootkey
from user_group_db import UserGroup
	   
	   
	   
class HandlerTest(unittest.TestCase):
	
	
	def setUp(self):
		# Create a WSGI application.
		app = webapp2.WSGIApplication([
			('/', MainPage), 
			('/compose(.*)', ComposeMessage)
			])
        # Wrap the app with WebTest TestApp
		self.testapp = webtest.TestApp(app)
		
		# First, create an instance of the Testbed class
		self.testbed = testbed.Testbed()
		# Then activate the testbed, which prepares the service stubs for use.
		self.testbed.activate()		
		self.testbed.init_datastore_v3_stub()
		self.testbed.init_memcache_stub()
		self.testbed.init_taskqueue_stub()
		
		user_ent, _ = UsersDb.my_get_or_insert('anth', pwd='foobar1')
		new_msg_file = MsgFile.create_msg_file()
		user_ent.msg_file = new_msg_file
		user_ent.put()
		self.user = user_ent
			
		user_ent, _ = UsersDb.my_get_or_insert('bond', pwd='foobar2')
		new_msg_file = MsgFile.create_msg_file()
		user_ent.msg_file = new_msg_file
		user_ent.put()

		UserGroup.my_get_or_insert('cs_101',
			group_keys=[self.user.key()],
			group_author=self.user.key()
			)
		
	def tearDown(self):
		self.testbed.deactivate()	
		
	def test_ComposeMessage_no_login(self):
		# test that non-logged in users get blocked
		response = self.testapp.get('/compose', status=400)
		self.assertEqual(response.status_int, 400)
	
	def test_ComposeMessage_login(self):
		# test that non-logged in users 
		response = self.testapp.get('/compose', headers={'Cookie':'user_name=anth|ce908054e59b3e4b38891dcd7feb93d5'})
		self.assertEqual(response.status_int, 200)
	
	def test_ComposeMessage_post_valid_user(self):
		# test that non-logged in users 
		response = self.testapp.post('/compose', 
			{'recipient':'bond', 
			'subject':'hello', 
			'body':'test message'}, 
			headers={'Cookie':'user_name=anth|ce908054e59b3e4b38891dcd7feb93d5'}
			)
		redirect = response.follow()
		self.assertEqual(redirect.status_int, 200)
		q = MessageDb.all()
		q.filter("author =", "anth")
		msg = q.get()
		self.assertEqual("hello",msg.subject)
		self.assertEqual("test message", msg.body)
		
	def test_ComposeMessage_post_invalid_user(self):
		# test that non-logged in users 
		response = self.testapp.post('/compose', 
			{'recipient':'fake_name', 
			'subject':'hello', 
			'message':'test message'}, 
			headers={'Cookie':'user_name=anth|ce908054e59b3e4b38891dcd7feb93d5'}
			)
		self.assertEqual(response.status_int, 200)
		contains_error = "That recipient" in response.body
		self.assertTrue(contains_error)
	
	def test_ComposeMessage_post_all(self):
		# test that non-logged in users 
		response = self.testapp.post('/compose', 
			{'recipient':'all', 
			'subject':'hello', 
			'body':'test message'}, 
			headers={'Cookie':'user_name=anth|ce908054e59b3e4b38891dcd7feb93d5'}
			)
		redirect = response.follow()
		self.assertEqual(redirect.status_int, 200)
	
	def test_ComposeMessage_post_group(self):
		# test that non-logged in users 
		response = self.testapp.post('/compose', 
			{'recipient':'cs_101', 
			'subject':'hello', 
			'body':'test message'}, 
			headers={'Cookie':'user_name=anth|ce908054e59b3e4b38891dcd7feb93d5'}
			)
		redirect = response.follow()
		self.assertEqual(redirect.status_int, 200)
		
	
	