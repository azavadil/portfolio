Implementation notes
===================

Specification
-------------
Build an internal messaging system for site-wide announcements, for instructors to send messages to a group, and for one-to-one communication between users.  Design and implement as many of the following features.

1. Allow sending of messages between individual users, identified by the unique key of their record in the system.  Messages are in markdown format, and auto-complete of recipients would be a bonus.
2. Allow a user to view their inbox, read messages, (automatically) mark messages as read, and delete messages.
3. Allow sending a broadcast message to all users.  Keep in mind that there could be millions of users.
4. Allow sending a message to a group of users.  Groups can be large (over 100,000 users) and are stored by having each user record list all the groups it’s a member of.  Group membership varies over time and a message should be received only by the users who were members of the destination group at the time the message was sent.

Application outline
-------------------

There's a live instance of the application at www.umessaging.appspot.com

In implementing a messaging system I thought about how I would build a messaging system without a computer. If I was providing a secure service where users could call in to get messages and leave messages for other users I would need the following: 

1. A list of users and their passwords ("User File")
2. A list of the messages IDs for each user ("Msg List") indicating which messages the user should receive 
3. A file of the messages, each with a unique ID ("Message File")
4. A list of groups, each group listing the members of their group. ("Group List") 

With the above tools, one can envision how the service would work. When a user calls in, the service validates their identity with the User File. The service pulls up the Msg List (for that user) and finds all messages addressed to the user. When a user leaves a message for a specific recipient, the message is added to the central Message File with a unique ID and the ID gets add to the recipient's Msg List. For group and global distributions, the message ID is added to each group member's Msg List.  

One advantage of the system is only one copy of each message is retained avoiding duplication. Global and group distributions are easy as they only require adding the message ID to the message file of the specified users.   

I used the above inventory to guide my implementation. 

Application logic
-----------------
The application logic is contained in the file [main.py](https://github.com/azavadil/msgApp/blob/master/main.py). Following the outline above, when a user arrives at the site the application checks to see if the user is logged in. If not, the user has the option of signing into their account or creating a new account. When the user logs in a secure cookie is set which can subsequently be used to validate their identity. Upon login, an inbox of the user's ten most recent messages is displayed. The user can click on any of the listed messages listed in the inbox to view the message content. 

The user also has the option to send messages to other users on the system. When a user sends a message, two things happen. The message is stored to the Message database and the message ID (i.e. the message's datastore key) is added to the recipient/recipients message lists.

This structure efficiently addresses some of the challenges posed by the coding exercise. For example, sending a global broadcast of the message results in a single message being saved to the datastore and the message ID (i.e. the message's datastore key) being added to each user's message list. Similarly, when a message is sent to a group, the message is saved to the datastore and the message ID is added to the message list of current group members. This ensures that the message is received only by users who are members of the group at the time the message was sent. 


Database Models
---------------
The database models have the following relationship to the outline items. 

####User File => user_DB model: store user names, passwords, and link to a list of the user's messages.
@property user_name:		a unique string representing the user's name  
@property pw_hash:	 		a string representing the salted, hashed value of the user's password  
@property msg_file: 		a pointer to the database entity which tracks the user's messages  
    
The user_DB model has a standard user authentication system. Passwords are salted and hashed before being saved to the database. Each user also has a list of messages. The list of messages is modeled as a one-to-one relationship with the user. The one-to-one relationship is implemented by adding a ReferenceProperty to the user datastore entity. When a new user is created a blank message file is created and a link between the user and the message file is set.  

####Msg List => MsgFile model: list of messages for a single user
@property messageKeys: 		list of messages (pointers) addressed to user  
@property unreadKeys: 		list of messages (pointers) addressed to user that user has not read  
@property sentKeys: 		list of messages(pointers) sent by user  

####Message File => Message: represents a message sent on the system
@property author:  			a string representing the name of the message author  
@property authorID:  		an integer representing the unique ID of the message author  
@property subject:  		a string representing the message subject  
@property body: 	 		text representing the message body  
@property recipientKeys:  	a list of keys representing authorized recipients of the message  

####Group List => UserGroup: represents a group and the members of a group
@property groupname:		a string representing the name of the group  
@property groupKeys: 		a list of keys (i.e. pointers) representing the members of a group  
@property groupAuthor: 		a key (i.e. pointer) for the author of the group  

####Implementation note: 
The recipientKeys property isn't necessary to make the program work. However, the list of recipientKeys is maintained to guard against malicious users. Before sending the HTTP response the application validates that the logged in user is an authorized recipient of the message (i.e. recipientKeys contains the user's key). Without this safeguard a malicious user could fabricate message IDs and submit a request for a message they are not authorized to receive. 

####Implementation note: 
Global and group distributions are straightforward. The recipient 'all' sends the message globally. A  group name as recipient sends the message to the members of the group. 

Memcache
--------
The application uses memcache to minimize reads from the datastore. User entities, group entities, and the list of groups a user belongs to are cached. For now, I chose not to cache the inbox because it wasn't clear if there would be a net benefit. Caching the inbox / outbox requires a read from the database each time a user receives a message. Depending on user behavior, this could result in more database reads than not caching the inbox. For example, if the user only logs on once a day, then 20 messages might be received between logins. Instead of 20 database reads, there's only 1 database read. However, if the user checks their inbox more frequently than messages are received, then there would be a benefit to caching the inbox.

Taskqueue for global distrubtions
---------------------------------
As the system can potentially have millions of users, to avoid blocking the application the global distribution (i.e. distribution to ‘all’) is handled by a taskqueue. When a message is addressed to ‘all’, the message parameters are passed to the task queue which completes the actions of storing the message and adding the message key to each user’s message file.

Autocompletion:
---------------
My initial approach was to build a trie on the client side to generate autocompletions. The motivation for that implementation was that it would be fast. With the trie in the browswer's memory, instead of querying the server and waiting for a response, the brower would be able to provide instant autocompletion options as the user typed.  

I initially thought I could build a trie instance on the server side and then transmit the trie to the client. My thinking was I would reduce the amount of data to be sent to the client by encoding the user names as a trie. In order to avoid blocking the application, I set up a task queue to manage retrieving the trie from the datastore, unpickling the stored data, putting the new name into the trie, pickling the updated trie, and putting the updated trie back in the datastore. Unfortunately, it was only after I had gone partway down that path that I realized that I wouldn't be able to reconstitute a trie instance in the browser. 

Having been blocked building the trie on the server side, I switched to a naive implementation where the program transmits the entire list of names to the client. The broswer then builds an instance of a trie. To minimize space, we take advantage of that fact that user names are limited to a 62 character alphabet (uppercase/lowercase letters and the digits 0-9). The trie code can be [found here](https://github.com/azavadil/msgApp/blob/master/static/js/Trie.js). There are two problems with this implementation.  First, the bandwidth required to transmit the entire list of names. In addition, the browser can only store the list of names in memory, not the trie instance. That means that the trie has to be rebuilt each time we leave and come back to the message composition page.

Knowing what I know now, there are two approaches to autocomplete that I'd explore for a production version.  

1. A succinct data structure instance that could be built on the server side and transmitted to the client. 
2. Maintain the trie (or similar structure) on the server side and set up a URL to response to autocomplete queries.  



 


  
