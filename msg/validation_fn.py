##
# Implementation note: 
# -------------------
# The functions in this section are user for validating 
# the format of user names, passwords, and groupnames. 
# User names must be composed of lowercase and uppercase
# letters and the digits 1-9. 
# 
# Passwords must have 1 uppercase letter, 1 lowercase letter
# and 1 digit. 
# 
# Groupnames must be composes of lowercase and uppercase letters,
# the digits 1-9, hyphens, and underscores. 
##

import cgi
import re

def escape_html(input_string):
    return cgi.escape(input_string,quote=True)

def valid_username(username):
    USER_RE = re.compile(r"^[a-zA-Z0-9]{4,20}$")
    if USER_RE.match(username):
		return True 
    return False

def valid_password(user_password):
	""" 
		require 1 uppercase, 1 lowercase, 1 digit, length of at least 6
		
		^                  		the start of the string
		(?=.*[a-z])        		use positive look ahead to see if at 
								least one lower case letter exists
		(?=.*[A-Z])        		use positive look ahead to see if at 
								least one upper case letter exists
		(?=.*\d)           		use positive look ahead to see if at 
								least one digit exists
		.+                 		gobble up the entire string
		$                  		the end of the string
	"""
	PASSWORD_RE = re.compile(r"^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)\w{6,20}$")
	if PASSWORD_RE.match(user_password):
		return True 
	return False
	
def valid_groupname(groupname): 
	GROUP_RE = re.compile(r"^(?=.*_)[a-zA-Z0-9_]{3,10}$")
	if GROUP_RE.match(groupname): 
		return True
	return False
	
