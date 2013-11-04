import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), 'lib'))

from base_handler import BaseHandler
from main_page import MainPage
from signup_page import Register
from sent_page import SentPage
from compose_message import ComposeMessage
from view_message import ViewMessage
from view_group import ViewGroup
from logout_page import LogoutPage
from all_manager import AllManager

import webapp2

		
#
# Implementation note: 
# -------------------
# anything that is in paratheses gets passed in to 
# the handler the regular expression matches ()		
#

MSGKEY_RE = r'(/(?:[a-zA-Z0-9_-]+)*)'

NEWMSG_RE = r'/newMsg(.*)'

app = webapp2.WSGIApplication([('/', MainPage),
								( NEWMSG_RE, ComposeMessage),
								('/group', ViewGroup), 
								('/signup', Register),
								('/sent', SentPage), 
								('/logout',LogoutPage),
								( MSGKEY_RE, ViewMessage),
								( '/_ah/queue/default', AllManager), 
								],debug = True)
