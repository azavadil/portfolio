import sys
sys.path.append( 'C:\Program Files (x86)\Google\google_appengine' )


import webapp2
import webtest
import unittest

from main_page import MainPage
from logout_page import LogoutPage
	   
class HandlerTest(unittest.TestCase):
	
	
	def setUp(self):
		# Create a WSGI application.
		app = webapp2.WSGIApplication([
			('/', MainPage), 
			('/logout', LogoutPage), 
			])
        # Wrap the app with WebTest TestApp
		self.testapp = webtest.TestApp(app)
	
	def test_LogoutPage_response(self):
		# test that non-logged in users get blocked
		response = self.testapp.get('/logout')
		self.assertEqual(response.status_int, 302)
	
	def test_LogoutPage_redirect(self):
		
		response = self.testapp.get('/logout')
		redirect = response.follow()
		self.assertEqual(redirect.status_int, 200)
	
