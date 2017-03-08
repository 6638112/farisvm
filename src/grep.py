# python 1.py tools/libxl/libxl.c libxl 372

import sys
import commands

argvs = sys.argv  
argc = len(argvs) 

f = open(argvs[1])

line = f.readline() 

while line:
    #print line.strip()

    cmdstr = "grep " + line.strip() + " apkurl.txt"
    print cmdstr
    check = commands.getoutput("cmdstr")

    line = f.readline() 
