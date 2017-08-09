# Ns3
## 安裝教學
安裝穩定版本：

下載：https://github.com/Lu-Yi-Hsun/Ns3/tree/master/install/install-ns3.sh
執行安裝
```
sudo sh install-ns3.sh
```

安裝即時最新版本(才能使用802.11ax)：
```
git clone https://github.com/nsnam/ns-3-dev-git.git
./waf configure --enable-examples
./waf

```

## 使用教學
1.把要執行的程式放入/ns-allinone-3.26/ns-3.26/scratch

2.在/ns-allinone-3.26/ns-3.26目錄 以終端機模擬打入 ：sudo ./waf --run scratch/wifi-tcp 

3.如果要開啟影像模擬：sudo ./waf --run scratch/wifi-tcp --vis
## 模擬內容
### wifi-tcp.cc:可以設定頻寬,可以設定802.11n,802.11ac,802.11ax(這個只能在最新github上編譯的ns3才能實現),節點位置速度大小(可以固定速度向量或是隨機),設定其他node也在連線的數量,並計算throughput,delay,封包大小,跟理論值誤差
![wifi-tcp](image/wifi-tcp.gif)
