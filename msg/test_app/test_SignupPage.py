import sys
sys.path.append( 'C:\Program Files (x86)\Google\google_appengine' )


import webapp2
import webtest
import unittest

from main_page import MainPage	   
from signup_page import Register 

from google.appengine.ext import db
from google.appengine.ext import testbed


class HandlerTest(unittest.TestCase):
	
	
	def setUp(self):
		# Create a WSGI application.
		app = webapp2.WSGIApplication([
			('/', MainPage), 
			('/signup', Register)
			])
        # Wrap the app with WebTest TestApp
		self.testapp = webtest.TestApp(app)
		
		# First, create an instance of the Testbed class
		self.testbed = testbed.Testbed()
		# Then activate the testbed, which prepares the service stubs for use.
		self.testbed.activate()		
		self.testbed.init_datastore_v3_stub()
		self.testbed.init_memcache_stub()		
	
	def tearDown(self):
		self.testbed.deactivate()	


	
	# Test the SignupPage handler
	def test_SignupPage(self):
		response = self.testapp.get('/signup')
		self.assertEqual(response.status_int, 200)
	
	def test_SignupPage_post_valid(self): 
		response = self.testapp.post('/signup', {'username':'anth','pwd1':'Star88','pwd2':'Star88'})
		redirect = response.follow()
		self.assertEqual(redirect.status_int, 200)
		contains_username = 'anth' in redirect.normal_body
		self.assertTrue(contains_username)
	
	def test_SignupPage_post_invalid(self): 
		response = self.testapp.post('/signup', {'username':'ant','pwd1':'Star88','pwd2':'Star88'})
		self.assertEqual(response.status_int, 200)
		contains_error = "not a valid username" in response.normal_body
		self.assertTrue(contains_error); 