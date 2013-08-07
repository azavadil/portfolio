import os
import webapp2
import jinja2
import hmac
import hashlib
import re
import cgi
import random
import string
import urllib
import json
import logging
import time
import hashsecret

from google.appengine.ext import db
from google.appengine.api import memcache

template_dir = os.path.join(os.path.dirname(__file__), 'templates')
jinja_env = jinja2.Environment(loader = jinja2.FileSystemLoader(template_dir),\
								autoescape = True)

def render_str(template, **params):
	t = jinja_env.get_template(template)
	return t.render(params)


########## PASSWORD STORAGE ##########

## put the secret into another module and change to a unique 
## secret for your app

SECRET = hashsecret.getSecret()

def make_salt():
    return ''.join(random.choice(string.letters) for x in xrange(5))

def make_pw_hash(name, pw,salt = None):
	if not salt:
		salt = make_salt()
	h = hashlib.sha256(name+pw+salt).hexdigest()
	return '%s|%s' %(h, salt)

def valid_pw(name, pw, h):
    salt = h.split('|')[1]
    return h == make_pw_hash(name, pw, salt)
	
def hash_str(s):
    return hmac.new(SECRET,s).hexdigest()
	
def make_secure_val(s):
    """make secure value is used to generate outgoing keys
    to be sent and stored by the browser"""
    ##s is the string
    ##hash_str(s) the is hashed value of the string
    return '%s|%s' %(s, hash_str(s))

def check_secure_val(h):
    """(str) -> str or Nonetype
        check_secure_val take a string in the format
        {value} | {hashed value of (value + secret)}
        and returns the value if the hashing the value
        the secret matches the hash value component of the string
    """ 
    val = h.split('|')[0]
    if h == make_secure_val(val):
        return val
		
def gray_style(lst):
    for n, x in enumerate(lst):
	if n % 2 == 0:
		yield x, ''
	else:
		yield x, 'gray'
		
########## PASSWORD VERIFICATION ##########		

def escape_html(input_string):
    return cgi.escape(input_string,quote=True)

def valid_username(username):
    USER_RE = re.compile(r"^[a-zA-Z0-9_-]{3,20}$")
    if USER_RE.match(username):
		return True 
    return False

def valid_password(user_password):
    PASSWORD_RE = re.compile(r"^.{3,20}$")
    if PASSWORD_RE.match(user_password):
    	return True 
    return False
	
def valid_email_address(email_address):
    EMAIL_RE = re.compile(r"^[\S]+@[\S]+\.[\S]+$")
    if EMAIL_RE.match(email_address):
		return True 
    return False
	
########## PERMALINK FUNCTION ##########

##  the URLs are organized by date and title  
##  to create a unique identifier
##  Format is /year/day/title
##  e.g. /2013/150/lebron-wins-title


def get_id_from(input_string):
    
    regexp = r'[0-9]+$'
		
    return int(input_string[re.search(regexp,input_string).start():\
				re.search(regexp,input_string).end()])

def make_urlpath(input_string):

    input_string = input_string.replace(' ','-')
	
    ##we have to find at least one eligible character
    ##then we optionally find additional characters
    regexp = r'[a-zA-Z0-9_-]+'
    result = ''.join(re.findall(regexp,input_string))
    return result

def make_url_datepath(input_string):
    """(str) -> str
        make_url_datepath generates a string in the format
       /year/day/inputstring where days is a value between 1-366
       e.g. /2012/150/lebron is good"""
    year = time.localtime().tm_year
    day_of_year = time.localtime().tm_yday
    return '/' + str(year) + '/' + str(day_of_year) + '/' + input_string
		
			
					
########## DATABASE CLASSES ##########	
def users_DB_key(group = 'default'):
    return db.Key.from_path('users', group)	
    ##the users_DB_key fuction returns an empty object
    ##that we can use for organization

class user_DB(db.Model):
    ##required = True, will raise an exception if we try to create 
    ##content without a title
    user_name = db.StringProperty(required = True)
    pw_hash = db.StringProperty(required = True)
    user_email = db.StringProperty(required = False)
    frnt_author = db.BooleanProperty(required = False)
    ##auto_now_add sets created to be the current time
    created = db.DateTimeProperty(auto_now_add = True)
    last_modified = db.DateTimeProperty(auto_now = True)
	
    @classmethod
    def db_by_id(cls, uid):
    	return user_DB.get_by_id(uid,users_DB_key())

    @classmethod
    def db_by_name(cls, name):
    	u = user_DB.all().filter('user_name =', name).get()
    	return u
		
		
    @classmethod
    def register(cls, name, pw, email = None):
	
		## replace these values with actual values 
		frontpage_authors = ['alan','bob','carol']

		frnt_author = False
		if name in frontpage_authors:
			frnt_author = True
		
		current_pw_hash = make_pw_hash(name, pw)
		
		return user_DB(parent = users_DB_key(), \
            				user_name = name, \
							pw_hash = current_pw_hash, \
							user_email = email, \
            				frnt_author = frnt_author)

    @classmethod
    def db_login(cls, name, pw):
		u = cls.db_by_name(name)
		if u and valid_pw(name, pw, u.pw_hash):
			return u, ''
		elif u:
			return u, "Username and password don't match"
		else:
			return None, "Invalid login"

########## CONTENT DATABASE ##########
	
def insider_key(name = 'default'):
    return db.Key.from_path('insider',name)
		
class insiderContent(db.Model):
    ##required = True, will raise an exception if we try to create 
    ##content without a title
	title = db.StringProperty(required = True)
	insiderPost = db.TextProperty(required = True)
	author = db.StringProperty(required = True)
	front_page = db.BooleanProperty(required = True)
	url_path = db.StringProperty(required = True)
	##auto_now_add sets created to be the current time
	created = db.DateTimeProperty(auto_now_add = True)
	last_modified = db.DateTimeProperty(auto_now = True)	
	
	def render(self, b_summarize = None):
		self._render_text = self.insiderPost.replace('\n','<br>')
		return render_str("formatted_post.html", page = self, summarize_text = b_summarize)
    	
    ##doesn't need an instance of the class
    ##e.g. can be called on insiderContent 
	@staticmethod
	def parent_key(path):
		return db.Key.from_path(path,'pages')
		
    ## doesn't run on an instance of the class
    ## e.g. can be called on insiderContent
	## get posts by URL path
	
	@classmethod
	def by_path(cls,path):
		q = cls.all()
		q.ancestor(cls.parent_key(path))
		q.order("-created")
		return q
    
	## get posts by ID 
	
	@classmethod
	def by_id(cls,page_id,path):
		return cls.get_by_id(page_id,cls.parent_key(path))
    		
########## COMMENT DATABASE ##########
	
class postComment(db.Model):
    ##required = True, will raise an exception if we try to create 
    ##content without a title
    cmt_title = db.StringProperty(required = False)
    cmt_comment = db.TextProperty(required = True)
    cmt_author = db.StringProperty(required = True)
    cmt_url_path = db.StringProperty(required = True)  ##NTD:need to check for uniqueness
    ##auto_now_add sets created to be the current time
    created = db.DateTimeProperty(auto_now_add = True)
    last_modified = db.DateTimeProperty(auto_now = True)
    	
    def render(self, b_summarize = None):
    	self._render_text = self.cmt_comment.replace('\n','<br>')
    	return render_str("formatted_comment.html", page = self)
	
    @staticmethod
    def parent_key(path):
    	return db.Key.from_path(path,"comments")
		
    @classmethod
    def by_path(cls,path):
    	q = cls.all()
    	q.ancestor(cls.parent_key(path))
    	q.order("-created")
    	return q


########## CACHING FUNCTIONS ##########		
		
##  cache_allpost is applied when we cache multiple posts
##  (e.g. when the front page is generated or we generate
##   a user page) 

	
def cache_allpost(front_val = "", update = False):
	""" (str, bool) -> str 
		param front_val: string that's used set construct database keys
        param update: specifies whether the cache should be overwritten
	"""

	key = 'top'+front_val
	if front_val == "":
		db_frnt_property = True
	else: 
		db_frnt_property = False
		##keys have to be strings
		logging.error("cache allpost %s" %key)
	frontpage_res = memcache.get(key)
	if frontpage_res is None or update:
		logging.error("cache_allpost - DB QUERY")
		frontpage_res = insiderContent.all().filter("front_page =", db_frnt_property).order("-created").fetch(8)	
		memcache.set(key, frontpage_res)
	return frontpage_res


##  cache_singlepost is applied to cache a single post

def cache_singlepost(key_val,update = False):
    """(str) -> str or Nonetype"""

    cache_key = str(key_val)
    ##keys have to be strings
    ##the key is a string of the path
    singlepost_res = memcache.get(cache_key)
    if singlepost_res is None or update:
		logging.error("cache_singlePost - DB SINGLEPOST QUERY")
		singlepost_res = insiderContent.by_path(key_val).get()
		
		##return None if db is empty
		if not singlepost_res:
			return None
		memcache.set(cache_key,singlepost_res)
    return singlepost_res	
	
def cache_comments(key_val,update = False):
    ##key_val is the url_path
	cache_key = "cmt_" + str(key_val)
	comment_res = memcache.get(cache_key)
	if comment_res is None or update: 
		comment_res = list(postComment.by_path(key_val))
	if not comment_res:
		return None
	memcache.set(cache_key,comment_res)
	return comment_res


########## REQUEST HANDLERS ##########
	
##  baseHandler is the main request handler that
##  other handlers inherit from. We put the convience  
##  methods in baseHandler
		
########## GENERAL PURPOSE HANDLER ##########							
class BaseHandler(webapp2.RequestHandler):
    def write(self, *a, **kw):
		self.response.out.write(*a, **kw)
		
    def render_str(self, template, **params):
		params['user'] = self.user
		params['gray_style'] = gray_style
		return render_str(template, **params)
	
    def render(self, template, **kw):
    	self.write(self.render_str(template, **kw))
	
    def set_secure_cookie(self, name, val):
		cookie_val = make_secure_val(val)
		self.response.headers.add_header('Set-Cookie','%s=%s; Path=/' % (name, cookie_val))
	
    def read_secure_cookie(self, name):
        """(str) -> str or Nonetype
            read_secure_cookie uses a python shortcut
            if expression 1 and expression 2 match, the return value is expression 1
            if expression 1 and expression 2 don't match, the return value is expression 2
			if expression 1 or expression 2 is False, the return value is false
        """
        
    	cookie_val = self.request.cookies.get(name)
    	return cookie_val and check_secure_val(cookie_val)

    def handler_login(self, user):
    	self.set_secure_cookie('user_id', str(user.key().id()))

    def handler_logout(self):
		"""()->Nonetype
           handler_logout is implemented by setting the user_id value of the
           cookie to be blank
        """
		self.response.headers.add_header('Set-Cookie', 'user_id=; Path=/')

    def initialize(self, *a, **kw):
		"""
           () -> Nonetype
           initialize check to see if the user is logged in/logged out
           Allows the app to display differently depending on whether
           the user is logged in. The framework calls webapp2 with
           every request triggering initialize with every request.
           Initialize checks for a user cookie, if a cookie exists,
           initialize checks the cookie and sets the cookie if the cookie is valid
        """
		webapp2.RequestHandler.initialize(self, *a, **kw)
		uid = self.read_secure_cookie('user_id')
		self.user = uid and user_DB.db_by_id(int(uid))
		
    def get_prior_url(self):
		"""
			get_prior_url is used in Signup to redirect the user back to 
			whatever page they initiated the signup from. 
			Convience that returns the user to the page they were on when 
			they initiated the signup process
			
			referer: the referer is sent as part of every http request and 
			is the page that generated the http request (i.e. the page the user
			is coming from)
		"""
		return self.request.headers.get('referer','/')
	
    def get_prior_url_set_next_url(self):
		"""
			get_prior_url is used in SignupPage and LoginPage. We extract the hidden field 
			prior_url from the form and use the result to redirect the user back to whatever
			page they were on when they initiated the signup/login process
		"""
			
		next_url = str(self.request.get('prior_url'))
		logging.error("baseHandlers - get_prior_url_set_next_url - next_url = %s" %next_url)
		
		## check that we have a url and the url is not the login page (wrt login page, 
		## if the user came from the login page we don't want to redirect back to the login 
		## page when we finish the signup/login process
		if not next_url or next_url.startswith('/login'):
			next_url = '/'
		## strip off trailing '/'
		if len(next_url) > 1 and next_url[-1] == '/':
			next_url = next_url[:-1]
		return next_url
	
    def notfound(self):
		self.error(404)
		self.write('<h1>404: Note Found</h1> Sorry, my friend, but that page does not exist. ')					
			
########## FRONT PAGE ##########	
class MainPage(BaseHandler):
    def get(self):
    	##[NTD: topUserPosts once voting is implemented]
    	userPosts = cache_allpost(front_val = "readers")   
    	frontPosts = cache_allpost()
			
    	self.render("front.html", frontPosts = frontPosts, userPosts = userPosts)

########## DISCRETE PAGE ##########					
class DiscretePost(BaseHandler):
    def get(self, path):
		
		readerComments = cache_comments(path) 
		logging.error('discretePost -get - readerComments = %s'%readerComments)
		
		##find the content who's url_path is the same as current url path
		##singlepost will return None if there's nothing in the DB
		logging.error("discretePost - get - path =  %s" %type(path))
		singlePost = cache_singlepost(path)
		if not singlePost:
			self.error(404)
			return
				
		self.render("discretePost.html", singlePost = singlePost, path = path, readerComments = readerComments)		

    def post(self, path):
		if not self.user:
			self.error(400)
			return
			
		input_title = self.request.get("title")
		input_comment = self.request.get("comment")
		author = self.user.user_name
		
		logging.error('discretePost - post - input comment = %s' %input_comment)
		if input_comment and self.user:
			logging.error('discretePost - post')
			cmt_to_store = postComment(cmt_title = input_title, 
            				cmt_comment = input_comment, 
            				cmt_author = author, 
            				cmt_url_path = path, 
            				parent = postComment.parent_key(path))
			cmt_to_store.put()
			cache_comments(path,True)
			self.redirect(path)
		else:
			self.redirect(path)
			self.redirect(path)
		
########## USERPOST TABLE ##########
class UserpostSummary(BaseHandler):
    def get(self):
    	userPosts = insiderContent.all().filter('front_page =', False).order('-created')  ###[NTD: need to filter by front_page = True]
    	logging.error('userpostSummary - userPosts = %s'%userPosts)			
    	self.render("userposts.html", posts = userPosts)

########## YOURPOST TABLE ##########
class YourpostSummary(BaseHandler):
	"""() -> Nonetype
        renders a table of the posts by a specified user
    """
	def get(self):
		if not self.user:
			self.error(400)
			return 
		yourPosts = insiderContent.all().filter('author =', self.user.user_name).order('-created')
		logging.error('yourpostSummary - get - yourPosts = %s'%yourPosts)
		self.render("yourposts.html", posts = yourPosts)
	
########## NEWPOST PAGE ##########				
class NewPost(BaseHandler):
	def get(self):
		if not self.user:
			self.error(400)
			return
		logging.error("newPost - get - self.user = %s" %self.user)
		self.render("newpost.html")
		
	def post(self):
		if not self.user:
			self.error(400)
			return
		
		##retreive the field named "title" and the field named "content"
		##from the form submission
		user_title = self.request.get("title")
		user_input = self.request.get("content")
		path_title = make_urlpath(user_title)
		path_title = make_url_datepath(path_title)
		
		valid_title, title_error_msg = self.validateTitle(path_title)
		
		if user_title and user_input and valid_title:
			##create a new insiderContent DB entry
			author = self.user.user_name
			front_page = self.user.frnt_author
			logging.error('newPost - get - front_page = %s'%front_page)
			to_store = insiderContent(title = user_title,
					insiderPost = user_input,
					author = author,
					front_page = front_page,
					url_path = path_title,
					parent = insiderContent.parent_key(path_title))
		
			
			 
			
			##store the new blog object
			to_store.put()
		
			##only cache the relevant section. If it's a
			##frontpage writer, we need to cache the frontpage
			##if it's a reader, we need to cache the reader
			if front_page:
				cache_allpost(update = True)
			else:
				cache_allpost("readers",True)
			##cache the permalink page for the post
			cache_singlepost(path_title,True)
			##redirect to a permalink page, pass the id
			self.redirect(path_title)
			
		else: 
			error = "we need both subject and content!"
			
			if not valid_title: 
				error = title_error_msg
			##pass the error message to the render fuction
			##the function then passes 'error' to the form
			self.render("newpost.html",outTitle=user_title, outCon=user_input, error=error)

	def validateTitle(self, path): 
		""" check that we don't have a url path conflict 
		    we could get a conflict if we have 2 posts with 
			the same title on the same day 
		"""
		content = insiderContent.by_path(path).get()
		logging.error('Newpost - post - validateTitle - content = %s'%content)
		
		if content: 
			msg = "That title already exists" 
			return False, msg
		else: 
			return True, ""
			
########## EDITPOST PAGE ##########				
class EditPost(BaseHandler):
	
	def get(self, path):
		if not self.user:
			self.error(400)
			return
		
		logging.error('editPost - get - path %s' %path)
		singlePost = insiderContent.by_path(path).get()
		self.render("editpost.html", singlePost = singlePost)
	
	def post(self,path):
		if not self.user:
			self.error(400)
			return
		
		logging.error('editPost - post - path = %s' %path)
		##retreive the field named "title" and the field named "content"
		##from the form submission
		user_title = self.request.get("title")
		user_input = self.request.get("content")
		db_key = self.request.get("db_id")
		path_title = path
				
		if user_title and user_input:
		##create a new insiderContent DB entry
			
			##db_by_name() returns a .get() instance
			author = self.user.user_name
			front_page = self.user.frnt_author
			logging.error('editPost - post - front_page = %s'%front_page)
			to_store = insiderContent(key = db_key,
                        	   title = user_title,
                            	   insiderPost = user_input,
                            	   author = author,
                            	   front_page = front_page,
                            	   url_path = path_title, 
                            	   parent = insiderContent.parent_key(path_title))
		
			##store the new blog object
			to_store.put()
	
			##see newpost comment
			if front_page:
				cache_allpost(update = True)
			else:
				cache_allpost("readers",True)
				##get the unique id for the post
			cache_singlepost(path_title,True)
			##redirect to a permalink page, pass the id
			self.redirect(path_title)
		else:
			error = "we need both subject and content!"
			##pass the error message to the render fuction
			##the function then passes 'error' to the form
			self.render("newpost.html",outTitle=user_title, outCon=user_input, error=error)
			
########## SIGNUP PAGE ##########						
class SignupPage(BaseHandler):
	
	def get(self):
		if self.user:
			self.error(400)
			return
			
		## establish prior url so use gets returned to whatever page they were on 
		## when they complete signup
		prior_url = self.get_prior_url()
		## we fill in prior_url as a hidden field in the signup for 
		## when the form is posted we extract prior_url and redirect the user
		## to the URL they came from
		self.render("signup.html", prior_url = prior_url)
	
	def post(self):
		##hold what the user entered
		if self.user:
			self.error(400)
			return

		have_error = False
		
		##extract prior_url. Used to send user back to page they were on
		self.next_url = self.get_prior_url_set_next_url()
		logging.error('signupPage - get - self.next_url = %s'%self.next_url)
	
		self.input_username = self.request.get('username')
		self.input_password = self.request.get('password')
		self.password_verify = self.request.get('verify')
		self.input_email_address = self.request.get('email')    
	
		params = dict(name_provided = self.input_username, \
			email_provided = self.input_email_address)
		
		##test for validity
		##test for valid user_name
		if not valid_username(self.input_username):
			params['name_error'] = "That's not a valid username"
			have_error = True
		
		if not valid_password(self.input_password):
			params['password_error'] = "That wasn't a valid password"
			have_error = True
		elif self.input_password != self.password_verify:
			params['password_error'] = "Passwords don't match"
		
		if not (valid_email_address(self.input_email_address) or self.input_email_address == ''):
			params['email_error'] = "That's not a valid email"
			have_error = True
	
		if have_error:
			self.render('signup.html',**params)	
			##set cookie, redirect to welcome page	
		else:
			self.done()
		
	def done(self,*a,**kw):
		"""not implemented in signupPage. Overwriten in Register below"""
		raise NotImplementedError

## Register's purpose is to extend the signupPage class to include an additional check 
## of whether a user exists before adding a user to the database. Register is implemented
## by inheriting from signupPage and overwriting the done() method. 
			
class Register(SignupPage):

    def done(self):
    ##make sure the user doesn't already exist
    ##username in self.username is a field in the signup page. 
		user = user_DB.db_by_name(self.input_username)
		if user:
			msg = 'That user already exists.'
			self.render('signup.html', name_error = msg)
		else:
			user = user_DB.register(self.input_username, self.input_password, self.input_email_address)
			user.put()
			
			self.handler_login(user)
			self.redirect(self.next_url)
			
########## LOGIN PAGE ##########
class LoginPage(BaseHandler):
	
    def get(self):
    	if self.user:
            self.error(400)
            return
		
        prior_url = self.get_prior_url()
        self.render("login.html",prior_url = prior_url)
	
    def post(self):
        if self.user:
            self.error(400)
            return
		
	next_url = self.get_prior_url_set_next_url()
		
	##hold what the user entered
	input_username = self.request.get('username')
	input_password = self.request.get('password')
				
	##check the password
	user, pw_msg = user_DB.db_login(input_username,input_password)
	if user and pw_msg == '': 
            self.handler_login(user)
            self.redirect(next_url)
        elif user:
			
            msg = 'Invalid login'
            self.render('login.html',general_error = msg, password_error = pw_msg)
	else:
		msg = 'Username not found'
		self.render('login.html',general_error = msg) 
		
########## LOGOUT PAGE ##########					
class LogoutPage(BaseHandler):
	
    def get(self):
		next_url = self.get_prior_url()
		logging.error('logoutPage - get - next_url %s' %next_url)
		self.handler_logout()
		##send user to the page they came from
		self.redirect(next_url)


########## DELETE POST ##########
class DeletePost(BaseHandler): 
	
	def get(self, path):
		if not self.user:
			self.error(400)
			return
		
			
		logging.error('deletePost - get - path %s'%path)
		markedForDeletion = insiderContent.by_path(path).get()
		insiderContent.delete(markedForDeletion); 		
		self.redirect("/edit")
		
##anything that is in paratheses gets passed in to the handler
##the regular expression matches ()		

PAGE_RE = r'(/(?:[a-zA-Z0-9_-]+/?)*)'

app = webapp2.WSGIApplication([('/', MainPage),
								('/newpost', NewPost),
								('/userposts', UserpostSummary),
								('/edit', YourpostSummary),
								('/signup', Register),
								('/login', LoginPage),
								('/logout',LogoutPage),
								('/edit' + PAGE_RE, EditPost),
								('/delete' + PAGE_RE, DeletePost), 
								( PAGE_RE, DiscretePost),
								],debug = True)
