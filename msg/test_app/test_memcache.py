import sys
sys.path.append( 'G:\\msgApp\msg' )  

import unittest
from google.appengine.ext import testbed

# Databases split out as modules
import memcache_fn
import users_db
import user_group_db

  
class TestMemcache(unittest.TestCase):

	def setUp(self):
		# First, create an instance of the Testbed class
		self.testbed = testbed.Testbed()
		# Then activate the testbed, which prepares the service stubs for use.
		self.testbed.activate()
		# Next, declare which service stubs you want to use.
		self.testbed.init_datastore_v3_stub()
		self.testbed.init_memcache_stub()

		test_user = users_db.UsersDb.register('Laplace', 'pwd')
		test_user.put()
		self.test_user = test_user

		test_group = user_group_db.UserGroup(
					parent=user_group_db.group_db_rootkey(), 
					key_name='cs_101', 
					group_keys=[test_user.key()], 
					group_author=test_user.key()
					)
		test_group.put()
		self.test_group = test_group
			
		
	def tearDown(self):
	
		self.testbed.deactivate()	
		
	def test_cache_user(self):
	
		user_name = self.test_user.key().name()
		result = memcache_fn.cache_user( user_name )
		self.assertEquals( self.test_user.key(), result.key()) 
	
	def test_cache_user_group_search(self): 
		
		group_list = memcache_fn.cache_user_group(self.test_user)
		self.assertEquals( len(group_list), 1)

	def test_cache_user_group(self): 
		
		group_list = memcache_fn.cache_user_group(self.test_user)
		self.assertEqual( 'cs_101',group_list[0].key().name() )
		
	def test_cache_group(self):
		
		result = memcache_fn.cache_group(groupname='cs_101',\
										update=True
										)
										
		self.assertEqual( result.key().name(), self.test_group.key().name() )
		
		
		 
		
		
		
		
		
	
if __name__ == '__main__':
    unittest.main()
	