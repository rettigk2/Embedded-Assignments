#Code by Kyle Rettig
from math import acos, cos, sin, trunc, sqrt, floor, asin
from typing import final
import matplotlib.pyplot as plt

#Defining Arm Degree of Rotation
A1 = [] #first arm degree of rotation
for i  in range(0,91):
  if i%5==0: 
        A1.append(i*0.0174533) 

A2 = [] #second arm degree of rotation
for i  in range(9,91):
  if i%5==0: 
        A2.append(i*0.0174533)
        
#Defining Lengths of Arms
len1 = (10) #b first arm length
len2 = (4.5) #c second arm length

x = []
y = []
for z in range(0,len(A1)):
    for i in range(0,len(A2)):
        hyp = sqrt((len1*len1)+(len2*len2)-(2*len1*len2*cos(A2[i])))
        D2 = acos(((len1*len1)-(len2*len2)+(hyp*hyp))/(2*hyp*len1))
        D1 = A1[z] - D2
        x.append(hyp*cos(D1))
        y.append(hyp*sin(D1))
        print((x[i]) , (y[i]))

plt.plot(x,y)
plt.show()
        
