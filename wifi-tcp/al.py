print("start")
with open("bo.txt") as f:
    content = f.read()

ok=content.split("\n")

run=0
delay=[]
point=[]
best_delay=[]
lost=[]
for str in ok:
    ok_co=str.split(" ")
    if ok_co[0].isdigit():
        delay.append(ok_co[2])
        run=run+1
        #print(ok_co[0])
    if run==10:
        temp=1000.0
        los=0
        for de in delay:
            if float(de)<temp:
                temp=float(de)
            if los<float(de):
                los=float(de)
        best_delay.append(temp)
        lost.append(los)
        delay=[]
        run=0

print(best_delay)
print("\n\n")
print(lost)

bobo_point=[]

bobo_point_det=[]
print("！累加數字！")
print("node \t datarate \t rss \t angle")
for str in ok:
    ok_co=str.split(" ")
    if ok_co[0].isdigit():
        #算分數
        sub=[]
        score=0
        print("*******各個參數值********")
        print(ok_co[7]+"\t"+ok_co[0]+"\t"+ok_co[8]+"\t"+ok_co[6])
        print("*分數累加*")
        #for node
        if int(ok_co[7])<40:
            score=score+0.3
        elif int(ok_co[7])>=40 and int(ok_co[7])<=100:
            score=score+0.2
        else:
            score=score+0.1 
        
         
        print(score,end='\t')
        #datarate
        if int(ok_co[0])>=10 and int(ok_co[0])<40:
            score=score+0.1
             
        elif int(ok_co[0])>=40 and int(ok_co[0])<70:
            score=score+0.2
        elif int(ok_co[0])>=70:
            score=score+0.3

        
        print(score,end='\t')
        # 10~40m 0.1
        # 40~70m 0.2
        #70~100m 0.3
        
        
        #rss
        #0 ~ -20 0.3
        #-20~-40 0.2
        #-40~-60 0.1
        if int(ok_co[8])<=0 and int(ok_co[8])>-20:
            score=score+0.3
        elif int(ok_co[8])<=-20 and int(ok_co[8])>-40:
            score=score+0.2
        elif int(ok_co[8])<=-40:
            score=score+0.1 
         
        print(score,end='\t')
        ok_co[6]=abs(int(ok_co[6]))
        while ok_co[6]>180:
            ok_co[6]=360-ok_co[6]
        if ok_co[6]<=180 and ok_co[6]>90:
            score=score+0.1
        elif ok_co[6]<=90 and ok_co[6]>60:
            score=score+0.2
        elif ok_co[6]<=60 and ok_co[6]>=0:
            score=score+0.3
         
        print(score,end='\n')
        #score=int(ok_co[8])
        #delay.append(ok_co[2])
        
        sub.append(score)
        sub.append(ok_co[2])
        bobo_point_det.append(sub)
        
        
        
        
        run=run+1
        #print(ok_co[0])
    if run==10:
        temp_best=-100000
        delay_best=100000;
        for str in bobo_point_det:
            if temp_best<str[0]:
                temp_best=str[0]
                delay_best=str[1]
        bobo_point.append(delay_best)
        bobo_point_det=[]
        run=0
        pass
print("\n\n")
print(bobo_point)

count=0.0
index=0
locount=0.0
bo_av=0
best_av=0
bad_av=0
for i in bobo_point :
    bo_av=float(i)+bo_av
    best_av=best_av+float(best_delay[index])
    bad_av=bad_av+float(lost[index])
    count=count+float(i)-float(best_delay[index])
    locount=locount+float(lost[index])-float(i)
    index=index+1
    pass

print("伯育演算法平均")
print (bo_av/1)

print("最好平均")
print(best_av/1)

print("最差平均")
print(bad_av/1)

print("伯育跟最好誤差")
print(count/1)
#print("\n")

print("伯育跟最差誤差")
print(locount/1)



