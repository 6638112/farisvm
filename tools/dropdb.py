import time

from pymongo import MongoClient
import sys

connect = MongoClient("127.0.0.1", 27017)
connect.drop_database('farisvm')

