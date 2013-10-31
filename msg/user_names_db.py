from google.appengine.ext import db

def usernames_db_rootkey(group = 'default'):
	""" 
		group_DB_rootkey takes a string and returns a key. 
		The returned key is used as the parent key for the entire 
		Message class. For this class a parent key isn't strictly 
		necessary except to ensure consistency. 
	"""
	return db.Key.from_path('UserNames', group)		
		
#
# Class: UserNames
# ----------------
# UserNames is used strictly to maintain a list of user names. 
# The only purpose of the user names class is to provide fast 
# access to a complete list of user names which is used to 
# build the client side trie
#

class UserNames(db.Model): 


	user_name_list = db.ListProperty(str, required = True)
	
	@classmethod
	def add_name(cls, name): 
		qry = UserNames.all().get()
		if not qry: 
			newEntity = UserNames(parent = usernames_db_rootkey(), user_name_list = [name])
			newEntity.put()
		else: 
			qry.user_name_list.append(name)
			qry.put()
	
