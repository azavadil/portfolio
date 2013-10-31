#
# Implementation note: 
# --------------------
# task queue that handles global distributions
#

import webapp2
import logging 

from google.appengine.ext import db

from users_db import UsersDb
from message_db import MessageDb
from message_db import message_db_rootkey

class AllManager(webapp2.RequestHandler): 
	
	def post(self): 
		
		logging.warning('task queue triggered')
		recipients = db.Query(UsersDb)
		recipient_keys = db.Query(UsersDb, keys_only=True)
			
		user_key = self.request.get('user_key')
		curr_user = UsersDb.get(user_key)
			
		to_store = MessageDb(
			parent=message_db_rootkey(),\
			author=curr_user.key().name(),\
			subject=self.request.get('subject'),\
			body=self.request.get('body'),\
			recipient_keys=list(recipient_keys)
			)
		to_store.put()
		
		for recipient in recipients: 
			curr_file = recipient.msg_file
			curr_file.message_keys.append(to_store.key())
			curr_file.unread_keys.append(to_store.key())
			curr_file.put()

		# add the message to the user's sent message list
		curr_user.msg_file.sent_keys.append(to_store.key())
		curr_user.msg_file.put()
					
