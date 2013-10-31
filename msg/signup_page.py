from users_db import UsersDb
from msgfile_db import MsgFile
from user_names_db import UserNames
from base_handler import BaseHandler
from validation_fn import valid_username
from validation_fn import valid_password


##
# Class: SignupPage
# -----------------
# SignupPage manages the creation of a new user
#
##						
class SignupPage(BaseHandler):
	
	def get(self):
		if self.user:
			self.error(400)
			return
			
		self.render("signupPage.html", isSignupPage = True)
	
	def post(self):
		
		# check if the user is logged in. If the user is logged in then we 
		# shouldn't be on this page 
		if self.user:
			self.error(400)
			return
	
		# store what the user entered
		self.input_username = self.request.get('username')
		self.input_password = self.request.get('pwd1')
		self.password_verify = self.request.get('pwd2')
		
		params = dict(name_provided = self.input_username)

		error_msg = "" 
		have_error = False
		
		# test for valid user_name
		if not valid_username(self.input_username):
			error_msg += "That's not a valid username"
			have_error = True

		if not valid_password(self.input_password):
			error_msg += "That wasn't a valid password" if error_msg == "" else ", that isn't a valid password" 
			have_error = True
		
		if self.input_password != self.password_verify:
			error_msg += "Passwords don't match" if error_msg == "" else ", passwords don't match" 
			have_error = True
	
		if have_error:
			params["fallback_error"] = error_msg
			self.render('signupPage.html',**params)	
			##set cookie, redirect to welcome page	
		else: 
			##set cookie, redirect to welcome page	
			self.done()
		
	def done(self,*a,**kw):
		"""not implemented in signupPage. Overwriten in Register below"""
		raise NotImplementedError

##
# Class: Register
# ---------------
# Register extends the signupPage class to include an 
# additional check of whether a user exists before adding 
# a user to the database. Register is implemented by 
# inheriting from signupPage and overwriting the done() 
# method. 
#
			
class Register(SignupPage):

	
	def done(self):
		
		#
		# Implementation note: 
		# --------------------
		# If the user already exists, then my_get_or_insert
		# returns a tuple with false as the second value
		# TODO: [test required]
		 
		user_entity, user_created = UsersDb.my_get_or_insert(
			self.input_username, 
			pwd=self.input_password
			)
		
		if not user_created: 
			self.render('signupPage.html',
				fallback_error='User already exists'
				)	
			
		new_msg_file = MsgFile.create_msg_file()
		user_entity.msg_file = new_msg_file
		user_entity.put()
		
					
		UserNames.add_name(user_entity.key().name())
		
		self.handler_login(user_entity)
		# TODO: cache_user(user.key().id())
		self.redirect("/")