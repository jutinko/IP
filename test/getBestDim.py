#!/usr/bin/python

import sys
import decimal

lines = [line.rstrip('\n') for line in open(sys.argv[1])]

min = sys.float_info.max
bestDim = None
#bestPoint = None
for line in lines:
    if("K" in line):
        print("%s, %f" %(bestDim, min))
        min = sys.float_info.max
        print line
    else:
        dim, _, time = line.split(",", 2)
        if(min > float(time)):
            min = float(time)
            #bestPoint = point
            bestDim = dim
print("%s,%f" %(bestDim, min))
