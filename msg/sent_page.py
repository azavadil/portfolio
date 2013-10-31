from base_handler import BaseHandler
import logging

#
# Class: SentPage
# ---------------
# SentPage manages displaying the user's outbox
#		
		
class SentPage(BaseHandler):
	def get(self):
		
		if not self.user: 
			self.error(400)
			return 
		else:
			self.render("summaryPanel.html",\
						num_msgs=len(self.inbox),\
						num_sent_msgs=len(self.outbox),\
						msgs=self.outbox,\
						user=self.user,\
						page_num= '0'
						)
	def post(self):

		
				
		if not self.user:
			self.error(400)
			return 
		
		else: 
			page_num = int(self.request.get('hiddenPageNum'))
			selected_action = self.request.get('selectedAction')
			
			
			if selected_action == 'Older': 
				if (page_num + 1) * 10 < len(self.outbox): 
					page_num += 1 
			else:  				# selected action is 'Newer' 
				if (page_num - 1) >= 0: 
					page_num -= 1
			start_index = page_num * 10 
			end_index = start_index + 10
			
			
			self.render("summaryPanel.html",\
						num_msgs=len(self.inbox),\
						num_sent_msgs=len(self.outbox),\
						msgs=self.outbox[start_index:end_index],\
						user=self.user,\
						page_num=str(page_num)
						)
			