import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), 'lib'))

import markdown
import urllib
import logging
from collections import OrderedDict

from base_handler import BaseHandler
from message_db import MessageDb





#
# Class: ViewMessage
# ------------------
# ViewMessage manages the display of a single message
#
				
class ViewMessage(BaseHandler):
	def get(self, path):
		if not self.user:
			self.error(400)
			return
		
		# 
		# Implementation note: 
		# --------------------
		# we're using the key as a url. The app extracts
		# the URL (which is actually a key) and uses the
		# key to retrieve the message from the database. 
		# use path[1:] to strip off the leading "/"
		#
		# Originally there was no parent key and the key == id. 
		# That allowed code Message.get(db.Key(path[1:])) where 
		# the function db.Key() converted a string to a key. 
		# When there is a parent component to the path the 
		# key != ID so
		# 
		# Defensive programming: validate we don't have a
		# string integer before calling int() on the path
		#  
		#
		
		logging.warning(path)
		if not path[1:].isdigit() or path is None: 
			self.notfound()
			return
		
		msg = MessageDb.db_by_id(int(path[1:]))
		
		#
		# Implementation note: defend against a garbage URL
		# --------------------------------------------------
		# If the ID doesn't return a message return not found
        #  		
		# 
		if not msg:
			self.notfound()
			return
		
		# 
		# Impmlementation note: defend against malicious users
		# ----------------------------------------------------
		# Validate that the user that's logged in is either
		# the recipient or the author of the message. If not, 
		# fail silently. Don't give the user any more information
		# 
		#
		
		if self.user.key() not in msg.recipient_keys and self.user.key().name() != msg.author: 
			self.error(400)
			return 
		
		if msg.key() in self.user.msg_file.unread_keys: 
			self.user.msg_file.unread_keys.remove(msg.key()) 
			self.user.msg_file.put() 
		
		# TODO: escape html
		self.render("viewMsg.html",\
					message_HTML=markdown.markdown(msg.body),\
					message=msg,\
					num_msgs=len(self.inbox),\
					num_sent_msgs=len(self.outbox),\
					user= self.user)
	
	
	
	#
	# Implementation note: 
	# -------------------
	# The app posts to the ViewMessage handlers when either
	# the 'Reply' or 'Delete' button is clicked. When the 
	# 'Reply' button is clicked, we extract the values for 
	# the message author and subject, build a query string, 
	# and redirect to /newMsg with the query string allowing 
	# the app to fill in the recipient and subject of the new 
	# message 
	#
	
	def post(self, path): 
		
		selected_action = self.request.get('selectedAction')
		msg_author = self.request.get('msgAuthor')
		msg_subject = self.request.get('msgSubject')
		
	
		msg = MessageDb.db_by_id(int(path[1:]))
		
		if selected_action == "reply":
		
			qs_params = OrderedDict([("msgAuthor",msg_author),("msgSubject", msg_subject)])
			self.redirect("/newMsg?" + urllib.urlencode(qs_params))
		
		if selected_action == "delete": 
			if msg.key() in self.user.msg_file.message_keys: 
				self.user.msg_file.message_keys.remove(msg.key())
			if msg.key() in self.user.msg_file.unread_keys: 
				self.user.msg_file.unread_keys.remove(msg.key()) 
			self.user.msg_file.put()
			
			self.redirect("/") 
