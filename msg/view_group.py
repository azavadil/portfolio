#
#
# Class: View Group
# -----------------
# ViewGroup manages the CRUD actions for user groups. 
# 				

from base_handler import BaseHandler

from user_group_db import UserGroup
from user_group_db import group_db_rootkey

from users_db import UsersDb

from memcache_fn import cache_user_group
from memcache_fn import cache_group
from validation_fn import valid_groupname


class ViewGroup(BaseHandler):
	def get(self):
		if not self.user:
			self.error(400)
			return
		
		groups_user_belongs_to = cache_user_group(self.user); 
			
		self.render("viewGroup.html",\
				groups=groups_user_belongs_to,\
				num_msgs=len(self.inbox),\
				num_sent_msgs=len(self.outbox))
	
	def post(self): 
		
		groups_user_belongs_to = cache_user_group(self.user)
		input_groupname = self.request.get("groupname")
		selected_action = self.request.get("selectedAction") 		
		
		
		error_msg = ""
		if not valid_groupname(input_groupname): 
			error_msg = "Please enter a valid groupname. Groupname must be lowercase/uppercase/digit and at least 1 underscore."
			self.render("viewGroup.html",\
						user_input_groupname=input_groupname,\
						groups=groups_user_belongs_to,\
						num_msgs=len(self.inbox),\
						num_sent_msgs=len(self.outbox),\
						error=error_msg)
			return
		
		#
		# Implementation note: 
		# --------------------
		# The programs checks for conflicts with both groupnames
		# and usernames. The program takes the 'To' field and looks
		# for a group or user that matches. Therefore, we must for 
		# unique names
		# 
			
		if selected_action == "makeGroup": 
			ent, group_created = UserGroup.my_get_or_insert(
				input_groupname.lower(), 
				group_keys=[self.user.key()], 
				group_author=self.user.key()
				)
				
			if not group_created: 
				error_msg = "That group already exists" 
				self.render("viewGroup.html",\
							user_input_groupname=input_groupname,\
							groups=groups_user_belongs_to,\
							num_msgs=len(self.inbox),\
							num_sent_msgs=len(self.outbox),\
							error=error_msg)
			else:
				cache_group(input_groupname.lower(), update=True)
				cache_user_group(self.user, update=True)
				self.redirect("/group")
		
		if selected_action == "joinGroup": 
			qry = cache_group(input_groupname)
			if not qry: 
				error_msg = "That group doesn't exist" 
				self.render("viewGroup.html",\
							user_input_groupname=input_groupname,\
							groups=groups_user_belongs_to,\
							num_msgs=len(self.inbox),\
							num_sent_msgs=len(self.outbox),\
							error=error_msg)
			else: 
				qry.group_keys.append(self.user.key())
				qry.put()
				cache_group(input_groupname, update=True)
				cache_user_group(self.user, update=True)
				self.redirect("/group")
		
		if selected_action == "leaveGroup": 
			qry = cache_group(input_groupname)
			if not qry: 
				error_msg = "That group doesn't exist" 
				self.render("viewGroup.html",\
							user_input_groupname = input_groupname,\
							groups = groups_user_belongs_to,\
							num_msgs = len(self.inbox),\
							num_sent_msgs = len(self.outbox),\
							error = error_msg)
			else: 
				if self.user.key() not in qry.group_keys: 
					error_msg = "You don't belong to that group" 
					self.render("viewGroup.html",\
								user_input_groupname = input_groupname,\
								groups = groups_user_belongs_to,\
								num_msgs = len(self.inbox),\
								num_sent_msgs = len(self.outbox),\
								error = error_msg)
				else:
					qry.group_keys.remove(self.user.key())
					qry.put()
					cache_group(input_groupname, update=True)
					cache_user_group(self.user, update=True)
					self.redirect("/group")
		
		if selected_action == "deleteGroup": 
			qry = cache_group(input_groupname)
			if not qry: 
				error_msg = "That group doesn't exist" 
				self.render("viewGroup.html",\
							user_input_groupname = input_groupname,\
							groups = groups_user_belongs_to,\
							num_msgs = len(self.inbox),\
							num_sent_msgs = len(self.outbox),\
							error = error_msg)
			#
			# Implementation note: 
			# --------------------
			# group_author is set as Reference property on the group. 
			# Therefore, qry.group_author dereferences a user entity. 
			# This may be a surprising result since we set group_author
			# to be self.user.key().
			#
			elif qry.group_author.key() != self.user.key(): 
				error_msg = "Only group author can delete group"
				self.render("viewGroup.html",\
							user_input_groupname = input_groupname,\
							groups = groups_user_belongs_to,\
							num_msgs = len(self.inbox),\
							num_sent_msgs = len(self.outbox),\
							error = error_msg)
			else: 
				## we have a problem here in that we need to update the cache for all members of the group
				qry.delete()
				cache_group(input_groupname, update = True)
				## [TODO: this needs to be tested]  
				for user_key in qry.group_keys: 
					user_entity = UsersDb.get(user_key)
					cache_user_group(user_entity, update=True)
				self.redirect("/group")
