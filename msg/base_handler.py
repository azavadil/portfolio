import os
import webapp2
import jinja2
import json
import logging

from google.appengine.ext import db

from user_names_db import UserNames
from memcache_fn import cache_user
from password_fn import make_secure_val
from password_fn import check_secure_val


template_dir = os.path.join(os.path.dirname(__file__), 'templates')

jinja_env = jinja2.Environment(loader = jinja2.FileSystemLoader(template_dir),\
								autoescape = True, extensions=['jinja2.ext.autoescape'])
def render_str(template, **params):
	
	t = jinja_env.get_template(template)
	return t.render(params)

#
# Class: BaseHandler 
# ------------------
# BaseHandler is the main request handler that other 
# handlers inherit from. We put the convience methods 
# in BaseHandler so other handlers inherit the convience
# functions. 
# 
							
class BaseHandler(webapp2.RequestHandler):
    def write(self, *a, **kw):
		self.response.out.write(*a, **kw)
		
    def render_str(self, template, **params):
		params['user'] = self.user
		return render_str(template, **params)
	
    def render(self, template, **kw):
    	self.write(self.render_str(template, **kw))
	
    def set_secure_cookie(self, name, val):
		cookie_val = make_secure_val(val)
		self.response.headers.add_header('Set-Cookie','%s=%s; Path=/' % (name, cookie_val))
	
    def read_secure_cookie(self, name):
        """(str) -> str or Nonetype
            read_secure_cookie uses a python shortcut
            if expression 1 and expression 2 match, 
			the return value is expression 1
            if expression 1 and expression 2 doesn't match, 
			the return value is expression 2
			if expression 1 or expression 2 is False, 
			the return value is false
			
			check_secure_val takes the cookie value which 
			is in the format userID | hashed value and returns 
			the userID portion if the hashed value
			validates
			e.g. 1 | 2b1423ca5183a0ff98bda78157ac08df would return 1 
        """
        
    	cookie_val = self.request.cookies.get(name)
    	return cookie_val and check_secure_val(cookie_val)

    def handler_login(self, user):
		self.set_secure_cookie('user_name', str(user.key().name()))
		self.set_secure_cookie('trie_flag', 'True')

    def handler_logout(self):
		"""()->Nonetype
           handler_logout is implemented by setting the user_id value of the
           cookie to be blank
        """
		self.response.headers.add_header('Set-Cookie', 'trie_flag=; Path=/')
		self.response.headers.add_header('Set-Cookie', 'user_name=; Path=/')
		
    def initialize(self, *a, **kw):
		"""
           () -> Nonetype
           initialize check to see if the user is logged in/logged out
           Allows the app to display differently depending on whether
           the user is logged in. The framework calls webapp2 with
           every request triggering initialize with every request.
           Initialize checks for a user cookie, if a cookie exists,
           initialize checks the cookie and sets the cookie if the 
		   cookie is valid
        """
		webapp2.RequestHandler.initialize(self, *a, **kw)
		user_name = self.read_secure_cookie('user_name')		## return string value of user name 
		
		
		self.user = user_name and cache_user(user_name)
		if self.user:
			user_msg_file = self.user.msg_file
			self.inbox = sorted(db.get(user_msg_file.message_keys),\
				key=lambda x:x.created, reverse=True
				)
			self.outbox = sorted(db.get(user_msg_file.sent_keys),\
				key=lambda x:x.created, reverse =True
				)
		
		
		
		# 
		# Implementation note: 
		# -------------------
		# The app will only execute the conditional once
		# so the trie data is only sent once. 
		#
		self.triedata = None
		flag = self.read_secure_cookie('trie_flag')
		if flag == 'True': 
			qry = UserNames.all().get()
			if qry: 
				self.triedata = json.dumps(qry.user_name_list)
				self.set_secure_cookie('trie_flag', 'Done')
			
    def notfound(self):
		self.error(404)
		self.write('<h1>404: Note Found</h1> Sorry, my friend, but that page does not exist. ')					



