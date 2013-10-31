import logging

from users_db import UsersDb
from user_group_db import UserGroup
from user_group_db import group_db_rootkey

from google.appengine.api import memcache

#  
# Function: cache_user 
# --------------------
# is used for our user tracking system
#  (e.g. when the front page is generated or we generate
#   a user page) 

	
def cache_user(user_name, update = False):
	""" (str, bool) -> UsersDb entity 
		
		Retrieves UserDb entity from memcache if available, from
		datastore if not. 
		
		Args: 
		    user_id: string that's used as database key
			update: specifies whether the cache should be overwritten
		
		Returns: 
			A UsersDb entity
	"""

	user_result = memcache.get(user_name)
	if user_result is None or update:
		user_result = UsersDb.db_by_name(str(user_name))	
		memcache.set(user_name, user_result)
	return user_result

def cache_user_group(user, update = False): 
	""" (UserDb entity, bool) -> Group entities
		
		Retrieves a user's group list from memcache if available, from
		datastore if not. 
		
		Args: 
			user: string that's used as database key
			update: specifies whether the cache should be overwritten
		
		Returns: 
			list of UserGroup entities
	"""
	
	user_group_key = "group_" + str(user.key().name())
	list_of_users_groups = memcache.get(user_group_key)
	if list_of_users_groups is None or update: 
		list_of_users_groups =\
			UserGroup.all().ancestor(group_db_rootkey())\
			.filter("group_keys =",user.key()).fetch(10)
		memcache.set(user_group_key, list_of_users_groups)
	return list_of_users_groups
		 
def cache_group(groupname, update = False): 
	""" (str, bool) -> Group entity
		
		Retrieves a groupname from memcache if available, from
		datastore if not. 
		
		Args: 
			group_name: string that's used as database key
			update: specifies whether the cache should be overwritten
		
		Returns: 
			UserGroup entity
		
	"""
	
	group_result = memcache.get(groupname)
	if group_result is None or update: 
		group_result = UserGroup.get_by_key_name(
			groupname.lower(), 
			parent=group_db_rootkey()
			)
			
		memcache.set(groupname, group_result)
	return group_result