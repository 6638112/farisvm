import time
import sys
import commands

argvs = sys.argv  
argc = len(argvs) 

f = open(argvs[1])
filename = argvs[2]

line = f.readline() 

while line:
    tmp = line.strip("||")
    searchstr = tmp.rstrip().strip("^")

    print "grep " + searchstr
    cmdstr = "grep " + searchstr + " " + filename
    check = commands.getoutput(cmdstr)
    
    line = f.readline() 


