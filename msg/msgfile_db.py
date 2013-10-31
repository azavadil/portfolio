from google.appengine.ext import db


def msgfile_db_rootkey(group = 'default'):
	""" 
		group_DB_rootkey takes a string and returns a key. 
		The returned key is used as the parent key for the entire 
		Message class. For this class a parent key isn't strictly 
		necessary except to ensure consistency. 
	"""
	
	return db.Key.from_path('MsgFile', group)
		
#
# Class: MsgFile
# --------------
# The MsgFile class models a one-to-one relationship with 
# a user. Each user has a message file that is estalished 
# when a user registered for the application. The relationship
# is established by storing the MsgFile key as ReferenceProperty
# on the UsersDb entity for that user. 
#

class MsgFile(db.Model):


	message_keys = db.ListProperty(db.Key, required = True, indexed = False)
	unread_keys = db.ListProperty(db.Key, required = True, indexed = False)
	sent_keys = db.ListProperty(db.Key, required = True, indexed = False)
	
	@classmethod
	def create_msg_file(cls): 
		msg_file = MsgFile(parent = msgfile_db_rootkey())
		msg_file.put()
		return msg_file
