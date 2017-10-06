from numpy.linalg import inv
from numpy import dot,transpose
#[x,b] 第一項固定為1就是為了求baise
x=[[101.0, -26.0, 58.0, 85.0], [80.0, -33.0, 28.0, 98.0], [51.0, -53.0, 69.0, 63.0], [43.0, -34.0, 11.0, 15.0], [66.0, -46.0, 76.0, 85.0], [17.0, 0.0, 0.0, 165.0], [98.0, -6.0, 77.0, 123.0], [13.0, -12.0, 81.0, 90.0], [90.0, -25.0, 36.0, 54.0], [94.0, -28.0, 36.0, 158.0]]
y=[[8.034], [2.598], [6.108], [1.164], [6.186], [0.09], [6.263], [6.92], [3.162], [3.193]]

x2=[[101.0, -26.0, 58.0, 85.0], [80.0, -33.0, 28.0, 98.0], [51.0, -53.0, 69.0, 63.0], [43.0, -34.0, 11.0, 15.0], [66.0, -46.0, 76.0, 85.0], [17.0, 0.0, 0.0, 165.0], [98.0, -6.0, 77.0, 123.0], [13.0, -12.0, 81.0, 90.0], [90.0, -25.0, 36.0, 54.0], [94.0, -28.0, 36.0, 158.0], [58.0, -44.0, 0.0, 23.0], [22.0, -19.0, 3.0, 52.0], [15.0, -10.0, 11.0, 66.0], [14.0, -24.0, 28.0, 73.0], [81.0, -25.0, 2.0, 143.0], [55.0, -50.0, 0.0, 118.0], [14.0, -33.0, 5.0, 61.0], [13.0, -34.0, 8.0, 163.0], [31.0, -17.0, 16.0, 163.0], [44.0, -47.0, 1.0, 105.0], [64.0, -2.0, 6.0, 33.0]]
y2=[[8.034], [2.598], [6.108], [1.164], [6.186], [0.09], [6.263], [6.92], [3.162], [3.193], [0.42058706266187018], [0.27906671574125436], [0.87018330800520172], [2.2136233938817211], [0.5906249741960653], [0.08923927848730262], [0.27129138959335819], [0.21676824578635595], [1.1698621749815641], [0.096235096742720128], [1.1420208265664364]]
print(   dot(inv(dot(transpose(x),x)),dot(transpose(x),y)))

print(   dot(inv(dot(transpose(x2),x2)),dot(transpose(x2),y2)))
