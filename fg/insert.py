# apt-get install python-pymongo
#!/usr/bin/env python
import time

#from pymongo import Connection
from pymongo import MongoClient
import sys

connect = MongoClient("127.0.0.1", 27017)
db = connect["url"]
callers = db["url"]

callers.insert( { '_id' : 'https://www.google.com/', 'referer' : 'http://referer.com/' } )
callers.insert( { '_id' : 'http://example.com/index.html', 'referer' : 'http://referer.com/' } )
callers.insert( { '_id' : 'http://example.com/index.swf', 'referer' : 'http://referer.com/' } )

