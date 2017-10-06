from numpy.linalg import inv
from numpy import dot,transpose
import numpy as np


from sklearn.linear_model import Ridge
from sklearn.preprocessing import PolynomialFeatures
from sklearn.pipeline import make_pipeline
poly_deg=3

x=[]
y=[]

x_poly=[]
y_poly=[]
delay_ans=[]
delay_ans_poly=[]
datarate=0.0
rss=0.0
angle=0.0
other=0.0

def cal():
     global x
     global y
     ans=[]
     ans=dot(inv(dot(transpose(x),x)),dot(transpose(x),y))
      
     global datarate
     global rss
     global other
     global angle
     datarate=ans[0][0]
     rss=ans[1][0]
     other=ans[2][0]
     angle=ans[3][0]

with open("80211ax.txt") as f:
    content = f.read()
ok=content.split("\n")
temp=[]
index=0
exe=[]
for i in ok:
   

    temp.append(i)
    index=index+1

    if index%10==0:
        exe.append(temp)
        temp=[]

model = make_pipeline(PolynomialFeatures(poly_deg), Ridge())
index=0
for i in exe :
    #print(datarate,rss,other,angle)
    #正負號轉換
    datarate=-1*abs(datarate)
    rss=-1*abs(rss)
    other=abs(other)
    angle=abs(angle)

    
    if index==0:
        for j in i:
            j_ok=j.split(" ")
            temp=[]
            temp.append(float(j_ok[0]))
            temp.append(float(j_ok[8]))
            temp.append(float(j_ok[7]))
            j_ok[6]=abs((float(j_ok[6])))
            while j_ok[6]>180:
                j_ok[6]=360-j_ok[6]
            temp.append(j_ok[6])
            x.append(temp)
            x_poly.append(temp)
            #print(x)
            
            temp=[]

            temp.append(float(j_ok[2]))
            y.append(temp)
            y_poly.append(temp)
            #print(y)
        cal()
        
        model.fit(x,y)
        #print(model.predict([[float(j_ok[0]),float(j_ok[8]),float(j_ok[7]),float(j_ok[6])]]))

       
        #print(datarate,rss,other,angle)
        
            

            
        
        
        pass
    else:
        pre_delay=[]
        poly=[]
        for j in i:
             
            j_ok=j.split(" ")
            j_ok[6]=abs((float(j_ok[6])))
            while j_ok[6]>180:
                j_ok[6]=360-j_ok[6]
            tmp=[]
            tmp.append(datarate*float(j_ok[0])+rss*float(j_ok[8])+other*float(j_ok[7])+angle*float(j_ok[6]))
            tmp.append(float(j_ok[2]))
            tmp.append(float(j_ok[0]))
            tmp.append(float(j_ok[8]))
            tmp.append(float(j_ok[7]))
            tmp.append(float(j_ok[6]))
            #print(tmp)
            pre_delay.append(tmp)
            tmp=[]
            tmp.append(model.predict([[float(j_ok[0]),float(j_ok[8]),float(j_ok[7]),float(j_ok[6])]]))
            tmp.append(float(j_ok[2]))
            tmp.append(float(j_ok[0]))
            tmp.append(float(j_ok[8]))
            tmp.append(float(j_ok[7]))
            tmp.append(float(j_ok[6]))
            poly.append(tmp)
        bibi=sorted(pre_delay)[0]
        bibi2=sorted(poly)[0]
        #liner reg
        temp=[]
        temp.append(bibi[2])
        temp.append(bibi[3])
        temp.append(bibi[4])
        temp.append(bibi[5])
        x.append(temp)
        temp=[]
        temp.append(bibi[1])
        delay_ans.append(bibi[1])
        #print(bibi[0])
        y.append(temp)
        cal()
        print(datarate,rss,other,angle)
        #poly
        temp=[]
        temp.append(bibi2[2])
        temp.append(bibi2[3])
        temp.append(bibi2[4])
        temp.append(bibi2[5])
        x_poly.append(temp)
        temp=[]
        temp.append(bibi2[1])
        delay_ans_poly.append(bibi2[1])
        #print(bibi[0])
        y_poly.append(temp)
        model.fit(x_poly,y_poly)



        #print(len(y))

         
        



        pass
        
    index=index+1
    pass
#print(delay_ans)
aall=0
for i in delay_ans:
    
    aall=i+aall

print("多元線性回歸總平均delay")
print(aall/len(delay_ans))



aall=0
for i in delay_ans_poly:
    
    aall=i+aall

print(str(poly_deg)+"項式回歸"+"總平均delay")
print(aall/len(delay_ans_poly))


