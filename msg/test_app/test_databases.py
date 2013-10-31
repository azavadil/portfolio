import sys
sys.path.append( 'C:\Program Files (x86)\Google\google_appengine' )
sys.path.append( 'G:\\msgApp\msg' )  # for Users_DB import

import unittest
from google.appengine.ext import testbed

# Databases split out as modules
import users_db  
import message_db
import user_group_db
import msgfile_db
import user_names_db

def GetEntityViaMemcache(entity_key):
	"""Get entity from memcache if available, from datastore if not."""
	entity = memcache.get(entity_key)
	if entity is not None:
		return entity
	entity = TestModel.get(entity_key)
	if entity is not None:
		memcache.set(entity_key, entity)
	return entity
  
class TestDbs(unittest.TestCase):

	def setUp(self):
		# First, create an instance of the Testbed class
		self.testbed = testbed.Testbed()
		# Then activate the testbed, which prepares the service stubs for use.
		self.testbed.activate()
		# Next, declare which service stubs you want to use.
		self.testbed.init_datastore_v3_stub()
		self.testbed.init_memcache_stub()

		test_user = users_db.UsersDb( 
			key_name='Laplace',
			pw_hash='pwd',
			parent=users_db.users_db_rootkey()
			)
		test_user.put()
		
		self.test_user = test_user
		recipient_key = test_user.key()
		
		fixture_msg = message_db.MessageDb( parent = message_db.message_db_rootkey(),\
									author = 'anthony',\
									subect = 'test',\
									body = 'this is a test',\
									recipient_keys = [recipient_key])
		fixture_msg.put()		
			
		self.fix_msg = fixture_msg
		
	def tearDown(self):
		self.testbed.deactivate()	
	
	# UsersDb tests
	def test_basic_db(self): 
		testUser = users_db.UsersDb(
			key_name='Laplace', 
			pw_hash = 'pwd'
			)
		testUser.put()
		
		query = users_db.UsersDb.all()
		result = query.fetch(2)
		self.assertEqual( result[0].key().name(), 'Laplace')
		
				
	def test_db_by_name(self): 
		test_user = users_db.UsersDb( 
			parent=users_db.users_db_rootkey(),\
			key_name='Laplace',\
			pw_hash='pwd'
			)
		test_user.put()
		
		result = users_db.UsersDb.db_by_name('Laplace')		
		self.assertEqual( result.key().name(), 'Laplace')
	
	
	def test_my_get_or_insert_new_user(self): 
		
		test_new_user = users_db.UsersDb.my_get_or_insert(
				'anthony', 
				pwd='pwd'
				) 
		self.assertEqual( True, test_new_user[1]) 
			
	def test_my_get_or_insert_existing_user(self): 
		
		test_existing_user = users_db.UsersDb.my_get_or_insert(
			'Laplace', 
			pw_hash='pwd'
			)
		self.assertEqual( False, test_existing_user[1] )
	
	def test_my_get_or_insert_key(self): 
		
		test_new_user, _ = users_db.UsersDb.my_get_or_insert(
				'anthony', 
				pwd='pwd'
				) 
				
	# message_db tests
	def test_message_db_by_id(self):
		
		recipient_key = self.test_user.key()
		
		test_msg = message_db.MessageDb( parent = message_db.message_db_rootkey(),\
									author = 'anthony',\
									subect = 'test',\
									body = 'this is a test',\
									recipient_keys = [recipient_key])
		test_msg.put()
		test_msg_id = test_msg.key().id()
		res = message_db.MessageDb.db_by_id(test_msg_id)
		self.assertEqual( res.author, 'anthony')
	
	
	# user_group_db tests
	def test_user_group_db_search_by_name(self): 
		
		# create a user
		test_key = self.test_user.key()
		
		test_group = user_group_db.UserGroup(
			parent=user_group_db.group_db_rootkey(),\
			key_name='cs_101',\
			group_keys=[test_key],\
			group_author=test_key
			)
		test_group.put()
		
		found = user_group_db.UserGroup.get_by_key_name('cs_101', 
			parent=user_group_db.group_db_rootkey()
			)
		self.assertEqual(found.key().name(), 'cs_101')
	
	# user_group_db tests
	def test_user_group_db_search_by_name(self): 
		
		# create a user
		test_key = self.test_user.key()
		
		test_group = user_group_db.UserGroup(
			parent=user_group_db.group_db_rootkey(),\
			key_name='cs_101',\
			group_keys=[test_key],\
			group_author=test_key
			)
		test_group.put()
		
		found = user_group_db.UserGroup.db_by_name('cs_101')
		self.assertEqual(found.key().name(), 'cs_101')
	
	
	def test_user_group_my_get_or_insert(self): 
		
		ent, created_user = user_group_db.UserGroup.my_get_or_insert(
			'cs_101', 
			group_keys=[self.test_user.key()], 
			group_author=self.test_user.key()
			)
		self.assertTrue( created_user )
		
		found = user_group_db.UserGroup.get_by_key_name(
			'cs_101', 
			parent=user_group_db.group_db_rootkey()
			)
		self.assertEqual( found.key(), ent.key() )
		
		
	
	def test_user_group_my_get_or_insert_exists(self): 
		
		# insert cs_101 group 
		ent = user_group_db.UserGroup( 
			key_name='cs_101', 
			parent=user_group_db.group_db_rootkey(), 
			group_keys=[self.test_user.key()], 
			group_author=self.test_user.key()
			)
		ent.put()
		
		ent, created_user = user_group_db.UserGroup.my_get_or_insert(
			'cs_101', 
			group_keys=[self.test_user.key()], 
			group_author=self.test_user.key()
			)
		self.assertFalse( created_user )
		
	
	def test_msgfile_db_put(self):
		
		test_msg = msgfile_db.MsgFile(
			parent = msgfile_db.msgfile_db_rootkey(),\
			message_keys = [self.fix_msg.key()], 
			unread_keys = [self.fix_msg.key()], 
			sent_keys = [self.fix_msg.key()]
			)
			
		test_msg.put()
		
		query = msgfile_db.MsgFile.all()
		results = query.fetch(2)
		
		self.assertEqual(len(results), 1)
		
	def test_msgfile_db_create(self): 
		
		test_msg = msgfile_db.MsgFile.create_msg_file()
		results = msgfile_db.MsgFile.all().fetch(2)
		self.assertEqual( len(results), 1)
		
		
	def test_user_names_db(self): 
		
		test_list = user_names_db.UserNames( user_name_list = ['anthony'] )
		test_list.put()
		
		results = user_names_db.UserNames.all().fetch(2)
		self.assertEqual(len(results), 1)

	def test_user_names_db_add_name(self): 
	
		user_names_db.UserNames.add_name('anthony')
		
		results = user_names_db.UserNames.all().fetch(2)
		self.assertEqual(len(results), 1)
		
		
if __name__ == '__main__':
    unittest.main()