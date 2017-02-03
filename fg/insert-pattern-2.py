# apt-get install python-pymongo
#!/usr/bin/env python
import time

#from pymongo import Connection
from pymongo import MongoClient
import sys

connect = MongoClient("127.0.0.1", 27017)
db = connect["farisvm"]
callers = db["url"]

argvs = sys.argv  
argc = len(argvs) 

f = open(argvs[1])

line = f.readline() 

while line:

    try:
        tmp = line.split("apk:")
        #print tmp[1].rstrip()

        callers.insert( { 'url' : tmp[1].rstrip(), 'referer' : 'http://referer.com/' } )    
        #callers.insert( { 'pattern' : line, 'label' : 'file1.txt' } )    
        
    except:
        pass
        

    line = f.readline() 


