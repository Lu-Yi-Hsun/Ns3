# Ns3
## 安裝教學（）
！[install]（https://github.com/Lu-Yi-Hsun/Ns3/tree/master/install/LTE模擬器-NS3之簡介,環境設置與模擬範例.pptx）
## 使用教學
1.把想要模擬的檔案放入ns3的scratch
2.在scratch往上一層目錄開啟終端機
3.以終端機模擬打入 ：sudo ./waf --run scratch/wifi-tcp 
4.如果要開啟影像模擬：sudo ./waf --run scratch/wifi-tcp --vis
## 模擬內容
### wifi-tcp.cc:可以設定頻寬,節點位置速度大小(可以固定速度向量或是隨機),並計算throughput,delay,封包大小,跟理論值誤差
![wifi-tcp](image/wifi-tcp.gif)
