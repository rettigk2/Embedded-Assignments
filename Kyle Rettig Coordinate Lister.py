#Code by Kyle Rettig
from math import acos, cos, sin, trunc, sqrt, floor
from typing import final

#Defining Arm Degree of Rotation
s1 = [] #first arm degree of rotation
for i  in range(0,91):
  if i%5==0: 
        s1.append(i) 

s2 = [] #second arm degree of rotation
for i  in range(9,91):
  if i%5==0: 
        s2.append(i)

#Defining Lengths of Arms
s1length = (10) #b first arm length
s2length = (4.5) #c second arm length

#Define all x,y coordinates of the first arm
x1 = []
y1 = []
for i in range(0,len(s1)):
    if s1length*cos(s1[i]) >= 0:
        x1.append(s1length*cos(s1[i])) #10 value array
    if s1length*sin(s1[i]) >= 0:
        y1.append(s1length*sin(s1[i])) #8 value array
 
#Define all x,y coordinates for the second arm
x2 = []
y2 = []
for i in range(0,len(s2)):
    if s2length*cos(s2[i]) >= 0:
        x2.append(s2length*cos(s2[i])) #8 value array
    if s2length*sin(s2[i]) >= 0:
        y2.append(s2length*sin(s2[i])) #7 value array


#Combined final coordinate locations
x3 = []
y3 = []

#Each array is a different max length, so theres a for loop for every size
for i in range(0,len(x1)):
    for s in range (0,len(x2)):
        for k in range (0,len(y2)):
            x3.append(x1[i]+x2[s])
            y3.append(y1[s]+y2[k])


#Remove Duplicates
xt = []
yt = [] 
for i in x3: 
    if i not in xt: 
        xt.append(i)
for i in y3: 
    if i not in yt: 
        yt.append(i) 
xt.sort()
yt.sort()

#Print All Coordinates
print("All Coordinate Pairs for 2 Sevro Arm:")
for i in range(0, len(xt)):
    for z in range(0, len(yt)):
        print((xt[i],yt[z]))