from numpy.linalg import inv
from numpy import dot,transpose
import numpy as np
import statsmodels.api as sm
def reg_m(y, x):
   
    results = sm.OLS(y, x).fit()
    return results
x=[]
y=[]
#for st
x_s=[]
y_s=[]
x_t1=[]
x_t2=[]
x_t3=[]
x_t4=[]
with open("n_noother") as f:
    
    content = f.read()

content_ok=content.split("\n")

for i in content_ok:
    ok=i.split(" ")
    if  ok[0]=="":
        break

    xt=[]
    yt=[]
   # xt.append(1)
    xt.append(float(ok[0]))
    xt.append(float(ok[8]))
    #xt.append(float(ok[7]))
    ok[6]=abs(float(ok[6]))
    while ok[6]>180:
        ok[6]=360-ok[6]
    xt.append(float(ok[6]))
   # print(xt)
    x_t1.append(float(ok[0]))
    x_t2.append(float(ok[8]))
    x_t3.append(float(ok[7]))
    x_t4.append(float(ok[6]))
    
    x.append(xt)
    y_s.append(float(ok[2]))
    yt.append(float(ok[2]))
    y.append(yt)

x_s.append(x_t1)
x_s.append(x_t2)
x_s.append(x_t3)
x_s.append(x_t4)   
print(x)
print(y)

print("DataRate(Mbps) rss(dbm)  other_node  angle(角度夾角)")

print( dot(inv(dot(transpose(x),x)),dot(transpose(x),y)))


print (reg_m(y, x).summary())

