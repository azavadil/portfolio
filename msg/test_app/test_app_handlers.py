import sys
sys.path.append( 'C:\Program Files (x86)\Google\google_appengine' )


import webapp2
import webtest
import unittest

from main_page import MainPage	   
from signup_page import Register 
from sent_page import SentPage
from login_for_testing import LoginForTesting
from compose_message import ComposeMessage
from view_message import ViewMessage

from google.appengine.ext import db
from google.appengine.ext import testbed

from users_db import UsersDb
from msgfile_db import MsgFile
from message_db import MessageDb
from message_db import message_db_rootkey
	   
class HandlerTest(unittest.TestCase):
	
	
	def setUp(self):
		# Create a WSGI application.
		app = webapp2.WSGIApplication([
			('/', MainPage), 
			('/signup', Register), 
			('/sent', SentPage),
			('/testlogin', LoginForTesting),
			('/compose(.*)', ComposeMessage),
			(r'(/(?:[a-zA-Z0-9_-]+)*)', ViewMessage)
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
					
	def test_ViewMessage_no_login(self):
		# test that non-logged in users get blocked
		response = self.testapp.get('/1', status=400)
		self.assertEqual(response.status_int, 400)
	
	def test_ViewMessage_login_bad_address(self):
		# test that urls for message ids that don't exist return 404
		response = self.testapp.get('/1', headers={'Cookie':'user_name=anth|ce908054e59b3e4b38891dcd7feb93d5'}, status=404)
		self.assertEqual(response.status_int, 404)
		
	def test_ViewMessage_login_good_address(self):
		
		#
		# Implementation note:
		# --------------------
		# we have to put a message in the database so we can test the 
		# MessageView handler. The message view only renders when there
		# is an actual message. 
		
		
		to_store = MessageDb(parent=message_db_rootkey(),\
							author=self.user.key().name(),\
							subject='test_subject',\
							body='test_body', 
							recipient_keys=[self.user.key()]
							)
		to_store.put()
		
		
		# test that urls for message ids that don't exist return 404
		response = self.testapp.get('/' + str(to_store.key().id()), headers={'Cookie':'user_name=anth|ce908054e59b3e4b38891dcd7feb93d5'})
		self.assertEqual(response.status_int, 200)
		
	def test_ViewMessage_login_good_address_malicious_user(self):
		
		#
		# Implementation note:
		# --------------------
		# here we test for a malicious user that is attempting to access
		# a message they are not authorized to view. Users can only views
		# messages where they are in the recipient list or they are the 
		# sender  
		
		user_ent, _ = UsersDb.my_get_or_insert('Laplace',pwd='pwd') 
		user_ent.put()
		
		to_store = MessageDb(parent=message_db_rootkey(),\
							author=user_ent.key().name(),\
							subject='test_subject',\
							body='test_body', 
							recipient_keys=[user_ent.key()]
							)
		to_store.put()
		
		
		# test that urls for message ids that don't exist return 404
		response = self.testapp.get('/' + str(to_store.key().id()), 
			headers={'Cookie':'user_name=anth|ce908054e59b3e4b38891dcd7feb93d5'},
			status=400)
		self.assertEqual(response.status_int, 400)
	
	def test_ViewMessage_login_non_interger_address(self):
		
		#
		# Implementation note:
		# --------------------
		# the handler uses the path as an integer. Test that our 
		# defensive programming guards against an error 
		
		
		
		# test that urls for message ids that don't exist return 404
		response = self.testapp.get('/a', 
			headers={'Cookie':'user_name=anth|ce908054e59b3e4b38891dcd7feb93d5'},
			status=404)
		self.assertEqual(response.status_int, 404)
	