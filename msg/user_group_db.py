from google.appengine.ext import db

#
# Function: group_db_rootkey
# --------------------------
# Generates default key to serve as parent key for 
# the UserGroup entity model 
#



def group_db_rootkey(group = 'default'):
	
	""" 
		group_db_rootkey returns a default parent key. 
		parent keys are used to organize all UserGroups 
		entities into a single entity group. 
	"""
	
	return db.Key.from_path('UserGroup', group)

	
class UserGroup(db.Model):
	
	group_keys = db.ListProperty(db.Key, required = True)
	group_author = db.ReferenceProperty(required = True, indexed = False)
    
	@classmethod
	def db_by_name(cls, name):
		return UserGroup.get_by_key_name(name,parent=group_db_rootkey())
	
	
	@classmethod
	def my_get_or_insert(cls, key_name, **kwargs):
		""" (str, dict) -> (UserGroup entity, bool) 
		
		Attempts to get then entity of the model's kind with 
		the given name. If it exists, the entity is returned. 
		If the name doesn't exist, a new entity with the given 
		kind, name, and parameters in kwds is created, stored, 
		and returned
		
		Args: 
		    username: string that's used as database key_name
			kwds: keyword arguments used to populate the database 
				  entity
		
		Returns: 
			A tuple of the UsersDb entity and true if a new entity 
			was create, and false otherwise. 
		"""
		
		default_parent = group_db_rootkey()
	
		def _tx():
			entity = cls.get_by_key_name(key_name, parent=default_parent)
			if entity:
				return entity, False
			entity = cls(key_name=key_name, parent=default_parent, **kwargs)
			entity.put()
			return entity, True
		return db.run_in_transaction(_tx)
		
	