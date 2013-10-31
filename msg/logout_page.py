#
# Class: LogoutPage
# -----------------
# LogoutPage manages user logout. Simple class
# that calls the handler_logout() method (defined 
# in the BaseHanlder class and redirects to the 
# home page 
#

from base_handler import BaseHandler
 					
class LogoutPage(BaseHandler):
	
    def get(self):
		self.handler_logout()
		self.redirect("/")
