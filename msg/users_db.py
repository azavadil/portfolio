#
# Implementation note: 
# -------------------
# The app uses 5 database models. 
#
# UsersDb: 		models a single user. Used for managing 
#				a secure user login system
# Messages: 	models a single message
# UserGroup: 	models a group of users. 
# MsgFile: 		models a relationship between a user and the 	
#				user's messages. Each MsgFile belongs to one user
# UserNames: 	models a list of all the users. Used to rapidly provide
#				a name list for transmital to client to build the 
#				an autocompletion trie
#


import sys
sys.path.append( 'G:\\msgApp\\msg' )

from google.appengine.ext import db
from password_fn import make_pw_hash
from password_fn import valid_pw


def users_db_rootkey(group = 'default'):
	""" 	
		user_DB_rootkey returns a default parent key for 
		the user_DB class. Parent keys are used to organize 
		all user_DB entities into a single entity group. 
		The parent key is in the form kind/key_name 
		(e.g. UsersDb/'default').  
		Child keys are in the format kind/parent/ID 
		(e.g. UsersDb/'default'/XXXXXX)
		
		There's an equivalent syntax UsersDb(key_name=group) 
	"""
	return db.Key.from_path('UsersDb', group)	
    
#
# class: UsersDb
# --------------
# The UsersDb model models a single user.  
#
	
class UsersDb(db.Model):
	pw_hash = db.StringProperty(required = True, indexed = False)
	msg_file = db.ReferenceProperty(required = False, indexed = False)
	##auto_now_add sets created to be the current time
	created = db.DateTimeProperty(auto_now_add = True)
	last_modified = db.DateTimeProperty(auto_now = True)
	
	@classmethod
	def db_by_id(cls, uid):
		return UsersDb.get_by_id(uid,users_db_rootkey())

	@classmethod
	def db_by_name(cls, name):
		
		return UsersDb.get_by_key_name(name,parent=users_db_rootkey())
		
	@classmethod   
	def register(cls, name, pw):
		current_pw_hash = make_pw_hash(name, pw)
		
		return UsersDb(
			parent=users_db_rootkey(),\
            key_name=name,\
			pw_hash=current_pw_hash)
									
	@classmethod
	def db_login(cls, name, pw):
		u = cls.db_by_name(name)	
		if u and valid_pw(name, pw, u.pw_hash):
			return u, ''
		elif u:
			return u, "Username and password don't match"
		else:	
			return None, "Invalid login"
	
	@classmethod
	def my_get_or_insert(cls, key_name, **kwargs):
		""" (str, dict) -> (UsersDb entity, bool) 
		
		Attempts to get then entity of the model's kind with 
		the given name. If it exists, the entity. If the name
		doesn't exist, a new entity with the given kind, name, 
		and parameters in kwds is created, stored, and returned
		
		Args: 
		    username: string that's used as database key_name
			kwds: keyword arguments used to populate the database 
				  entity
		
		Returns: 
			A tuple of the UsersDb entity and true if a new entity 
			was create, and false otherwise. 
		"""
		default_parent = users_db_rootkey()
	
		def _tx():
			entity = cls.get_by_key_name(key_name, parent=default_parent)
			if entity:
				return entity, False
			kwargs['pw_hash'] = make_pw_hash(key_name, kwargs['pwd'])
			entity = cls(key_name=key_name, parent=default_parent, **kwargs)
			entity.put()
			return entity, True
		return db.run_in_transaction(_tx)