from google.appengine.api import taskqueue

from base_handler import BaseHandler
from users_db import UsersDb
from message_db import MessageDb
from message_db import message_db_rootkey
from user_group_db import UserGroup


#
# Class: ComposeMessage
# ---------------------
# ComposeMessage manages the creation and sending of messages. 
# 
#
				
class ComposeMessage(BaseHandler):
	
	
	def get(self,path):
		if not self.user:
			self.error(400)
			return
		
		
		#
		# Implementation note: 
		# -------------------
		# The only time ComposeMessage is rendered with msgAuthor
		# parameters is on a redirect from the ViewMessage handler.
		# The ViewMessage handler extracts the post data, builds 
		# a query string from the post data, and redirects to 
		# the /newMsg URL
		# 
		if self.request.get('msgAuthor'): 
			self.render("composeMsg.html",\
				num_msgs=len(self.inbox),\
				num_sent_msgs=len(self.outbox),\
				recipient=self.request.get('msgAuthor'),\
				subject="RE: " + self.request.get('msgSubject')
				)
		else: 
			
			self.render("composeMsg.html",\
				num_msgs=len(self.inbox),\
				num_sent_msgs=len(self.outbox)
				)
		
	def post(self,path):
		if not self.user:
			self.error(400)
			return
		
		# retreive the field named "subject" and the field 
		# named "content" from the form submission
		msg_recipient = self.request.get("recipient")
		msg_subject = self.request.get("subject")
		msg_body = self.request.get("body")
		
		q_params = {}
		q_params['recipient'] = msg_recipient 
		q_params['subject'] = msg_subject
		q_params['body'] = msg_body
		q_params['user_key'] = self.user.key()
		
		
		# check if the message is a global broadcast
		if msg_recipient.lower() == "all": 
			
			# send to taskqueue to manage distribution 
			taskqueue.add( params=q_params )
			self.redirect("/")
					
		group_qry = UserGroup.db_by_name(msg_recipient)	
		if group_qry: 
			
			# create a new Message entity
			to_store = MessageDb(
							parent=message_db_rootkey(),\
							author=self.user.key().name(),\
							subject=msg_subject,\
							body=msg_body,\
							recipient_keys=group_qry.group_keys)
		
			to_store.put()
			
			for recipient_key in group_qry.group_keys:
				msg_file = UsersDb.get(recipient_key).msg_file
				msg_file.message_keys.append(to_store.key())
				msg_file.unread_keys.append(to_store.key())
				msg_file.put()

			self.user.msg_file.sent_keys.append(to_store.key())
			self.user.msg_file.put()
			
			self.redirect("/")
	
		
		# Query the database for the recipient
		recipient_entity = UsersDb.db_by_name(msg_recipient) 
		
		if recipient_entity:
			# create a new Message entity
			to_store = MessageDb(parent=message_db_rootkey(),\
							author=self.user.key().name(),\
							subject=msg_subject,\
							body=msg_body, 
							recipient_keys=[recipient_entity.key()]
							)
				
			# store the message object
			to_store.put()
			
			# retrieve the recipient's message file and
			# add the message to their message list
			# and unread message list
			msg_file = recipient_entity.msg_file
			msg_file.message_keys.append(to_store.key())
			msg_file.unread_keys.append(to_store.key())
			msg_file.put()
			
			# add the message to the user's sent message list
			self.user.msg_file.sent_keys.append(to_store.key())
			self.user.msg_file.put()
			
			self.redirect("/")
			
		else: 
			error = "That recipient doesn't exist"
			
			# pass the error message to the render fuction
			# the function then passes 'error' to the form
			self.render("composeMsg.html",\
						recipient=msg_recipient,\
						subject=msg_subject,\
						body=msg_body,\
						num_msgs=len(self.inbox),\
						num_sent_msgs=len(self.outbox),\
						fallback_error=error
						)
