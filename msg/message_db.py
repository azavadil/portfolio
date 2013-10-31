import sys
sys.path.append( 'G:\\msgApp\\msg' )

from google.appengine.ext import db


##
# Function: message_DB_rootkey 
# ----------------------------
# Generate a default parent key
##
	
def message_db_rootkey(group = 'default'):
	""" 
		message_DB_rootkey returns a default parent key. 
		parent keys are used to organize all Messages 
		into a single entity group. 
	"""
	return db.Key.from_path('Message', group)
		
class MessageDb(db.Model):
    ##required = True, will raise an exception if we try to create 
    ##content without a title
	author = db.StringProperty(required = True)
	subject = db.StringProperty(required = False)
	body = db.TextProperty(required = False, indexed = False)
	recipient_keys = db.ListProperty(db.Key, required = True, indexed = False) 
	##auto_now_add sets created to be the current time
	created = db.DateTimeProperty(auto_now_add = True)
	
	def render(self, b_summarize = None):
		self._render_text = self.body.replace('\n','<br>')
		return render_str("formattedMsg.html",\
							page = self,\
							summarize_text = b_summarize)
    	
	@classmethod 
	def db_by_id(cls, msgID):
		return MessageDb.get_by_id(msgID, message_db_rootkey())
