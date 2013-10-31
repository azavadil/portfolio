from base_handler import BaseHandler
from users_db import UsersDb

#
# Class: MainPage
# ---------------
# MainPage handles the landing page and the main user page. 
# 
# 	
class MainPage(BaseHandler):
	def get(self):
	
		#
		# Implementation note: 
		# -------------------
		# There are three cases. The control flow is such that
		# self.triedata will be populated with data only one time.
		# If the user is not logged in, we render the summaryPanel 
		# without data. 
		# 
		# If the user is logged in we check for trie data (only 
		# occurs once) and include the trie data. 		
		#
		
		if not self.user: 
			self.render("summaryPanel.html")
		elif self.triedata:
			self.render("summaryPanel.html",\
						num_msgs=len(self.inbox),\
						num_sent_msgs=len(self.outbox),\
						msgs=self.inbox[:10],\
						user=self.user,\
						page_num='0',\
						data=self.triedata)
		else: 
			self.render("summaryPanel.html",\
						num_msgs=len(self.inbox),\
						num_sent_msgs=len(self.outbox),\
						msgs=self.inbox[:10],\
						page_num='0',\
						user=self.user)

	#
	# Implementation note: 
	# -------------------
	# The front page receives a post request when an 
	# existing user logs in. The application doesn't 
	# have a separate login URL. Rather, the signin 
	# panel is on the front page and collapses once 
	# the user has logged in. 
	# 
	# The front page can also receive a post request
	# when the user wants to navigate to newer/older
	# messages. It's easy to distinguish between the 
	# two cases as only a logged in user can navigate
	# messages
	#  
	
	def post(self):

		if not self.user: 
	
			input_username = self.request.get('username')
			input_password = self.request.get('password')
		
			#
			# Implementation note: 
			# --------------------
			# db_login returns the user id and the empty string 
			# if the password validates, the user and the msg 
			# "Username and password don't match" if the user 
			# was found but the password doesn't validate, and 
			# "Invalid login" otherwise
			#
			user, pw_msg = UsersDb.db_login(input_username,input_password)
			
			if user and pw_msg == '': 
				self.handler_login(user)
				self.redirect("/")
			else:
				self.render('base.html',\
						name_provided = input_username,\
						password_error = pw_msg) 
		else: 
			#
			# Impementation note: 
			# -------------------
			# Defensive programming, before calling int() 
			# validate we don't get none [test required] 
			# 
			page_num = self.request.get('hiddenPageNum'); 
			if not page_num.isdigit() or page_num is None: 
				page_num = 0
			else: 
				page_num = int(self.request.get('hiddenPageNum'))
			
			selected_action = self.request.get('selectedAction')
			
			if selected_action == 'Older': 
				if (page_num + 1) * 10 < len(self.inbox): 
					page_num += 1 
			else:  				# selected action is 'Newer' 
				if (page_num - 1) >= 0: 
					page_num -= 1
			start_index = page_num * 10 
			end_index = start_index + 10
			
			
			self.render("summaryPanel.html",\
						num_msgs=len(self.inbox),\
						num_sent_msgs=len(self.outbox),\
						msgs=self.inbox[start_index:end_index],\
						user=self.user,\
						page_num=str(page_num)
						)
