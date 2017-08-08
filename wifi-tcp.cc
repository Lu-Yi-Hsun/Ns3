/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
  
  n*node-------ap------sta(直線移動)
 
*/

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h"
#include "ns3/netanim-module.h"
#include <math.h>

#define PI 3.14159265

#include "ns3/config-store-module.h"

NS_LOG_COMPONENT_DEFINE("wifi-tcp");

using namespace ns3;

Ptr<PacketSink> sink;     /* Pointer to the packet sink application */
uint64_t lastTotalRx = 0; /* The value of the last total received bytes */

double angle_ans;

uint32_t payloadSize; /* Transport layer payload size in bytes. */
uint64_t MaxBytes;

double ap_po_x;

double ap_po_y;
std::string ans_dataRate;
double ans_averageThroughput = 0;
double ans_delay = 0;
int ans_allbytes = 0;

//double distance = 50.0;
//設定 datarate
std::string dataRate;
//設定訊號強度
double rss = -98;
//控制ue的速度向量
double speed_x = 10;
double speed_y = 0;

//有多少 其他人也在使用這個wifiap
int other_node = 1;

//處理顯示Throughput
void CalculateThroughput()
{

  Time now = Simulator::Now();                                       /* Return the simulator's virtual time. */
  double cur = (sink->GetTotalRx() - lastTotalRx) * (double)8 / 1e5; /* Convert Application RX Packets to MBits. */
                                                                     // std::cout << now.GetSeconds () << "s: \t" << cur << " Mbit/s" << std::endl;
                                                                     // std::cout<<"my\t"<<sink->GetTotalRx()<<"all\t"<<MaxBytes<<"\n";
  lastTotalRx = sink->GetTotalRx();
  Simulator::Schedule(MilliSeconds(1), &CalculateThroughput);

  if (cur != 0)
  {

    double averageThroughput = ((sink->GetTotalRx() * 8) / (1e6 * now.GetSeconds()));

    ans_averageThroughput = averageThroughput;
    ans_dataRate = dataRate.substr(0, dataRate.length() - 4);
    ans_delay = now.GetSeconds() - 1.0001;
    ans_allbytes = sink->GetTotalRx();
    // std::cout <<dataRate.substr(0,dataRate.length()-4)<<" "<<averageThroughput<<" "<<now.GetSeconds ()-1.0001<<" "<<MaxBytes*8/1024/1024<<std::endl;
    Simulator::Schedule(MilliSeconds(1), &CalculateThroughput);
  }
}

std::string int2str(int &i)
{
  std::string s;
  std::stringstream ss(s);
  ss << i;

  return ss.str();
}

/*處理顯示位置與速度*/

//只能顯示隨機路線的位置與速度
//固定位置與速度部份能直接換算
static void
CourseChange(std::string foo, Ptr<const MobilityModel> mobility)
{
  Vector pos = mobility->GetPosition();
  Vector vel = mobility->GetVelocity();
  // uint64_t time_no=Simulator::Now ().GetNanoSeconds();
  //double pox=0.000000001*time_no*

  // std::cout<<time_no<<std::endl;
  if (vel.x != 0)
  {
    std::cout << Simulator::Now() << ", model=" << mobility << ", Position: x=" << pos.x << ", y=" << pos.y << "; Velocity: x=" << vel.x << ", y=" << vel.y << std::endl;
  }
}

void wifi(int argc, char *argv[])
{

  std::string animFile = "my_wifi.xml";

  /* Application layer datarate. */
  std::string tcpVariant = "ns3::TcpNewReno"; /* TCP variant type. */

  /*這裡有介紹  http://mcsindex.com/  */
  std::string phyRate = "VhtMcs9"; /* Physical layer bitrate. */
  double simulationTime = 10;      /* Simulation time in seconds. */
                                   // bool pcapTracing = false;                          /* PCAP Tracing is enabled or not. */

  /* Command line argument parser setup. 這裡不重要 是控制如果要用terminel傳參數用的 我沒用到放著以後要修改能用 */
  CommandLine cmd;
  cmd.AddValue("payloadSize", "Payload size in bytes", payloadSize);
  cmd.AddValue("dataRate", "Application data ate", dataRate);
  cmd.AddValue("tcpVariant", "Transport protocol to use: TcpTahoe, TcpReno, TcpNewReno, TcpWestwood, TcpWestwoodPlus ", tcpVariant);
  cmd.AddValue("phyRate", "Physical layer bitrate", phyRate);
  cmd.AddValue("simulationTime", "Simulation time in seconds", simulationTime);
  // cmd.AddValue ("pcap", "Enable/disable PCAP Tracing", pcapTracing);
  cmd.Parse(argc, argv);
  //Config::Set("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelWidth", UintegerValue (20));
  /* No fragmentation and no RTS/CTS */
  Config::SetDefault("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue("999999"));
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue("999999"));

  /* Configure TCP Options */
  Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(payloadSize));

  WifiMacHelper wifiMac;
  WifiHelper wifiHelper;
  //底下設定wifi版本 80211ac 80211n 80211ax

  //設定給80211n用
  //wifiHelper.SetStandard (WIFI_PHY_STANDARD_80211n_5GHZ);

  // 設定給80211ac用
  //wifiHelper.SetStandard (WIFI_PHY_STANDARD_80211ac);

  //!80211ax!!!!!!
  wifiHelper.SetStandard(WIFI_PHY_STANDARD_80211ax_5GHZ);

  /* Set up Legacy Channel */
  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss("ns3::FriisPropagationLossModel", "Frequency", DoubleValue(5e9));
  wifiChannel.AddPropagationLoss("ns3::FixedRssLossModel", "Rss", DoubleValue(rss));
  /* wifi硬體設定Setup Physical Layer */
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default();
  wifiPhy.SetChannel(wifiChannel.Create());
  wifiPhy.Set("TxPowerStart", DoubleValue(10.0));
  wifiPhy.Set("TxPowerEnd", DoubleValue(10.0));
  wifiPhy.Set("TxPowerLevels", UintegerValue(1));
  wifiPhy.Set("TxGain", DoubleValue(0));
  wifiPhy.Set("RxGain", DoubleValue(0));
  wifiPhy.Set("RxNoiseFigure", DoubleValue(10));
  wifiPhy.Set("CcaMode1Threshold", DoubleValue(-79));
  wifiPhy.Set("EnergyDetectionThreshold", DoubleValue(-79 + 3));
  wifiPhy.SetErrorRateModel("ns3::YansErrorRateModel");
  //for wifi 設定給80211n用
  //wifiHelper.SetRemoteStationManager ("ns3::ConstantRateWifiManager","DataMode", StringValue (phyRate),"ControlMode", StringValue ("VhtMcs9"));
  //for wifi 設定給80211ac用
  wifiHelper.SetRemoteStationManager("ns3::ConstantRateWifiManager", "DataMode", StringValue("VhtMcs9"), "ControlMode", StringValue("VhtMcs0"));

  /*產生節點*/
  NodeContainer networkNodes;
  networkNodes.Create(2);
  Ptr<Node> apWifiNode = networkNodes.Get(0);
  Ptr<Node> staWifiNode = networkNodes.Get(1);

  NodeContainer other_ue;
  other_ue.Create(other_node);

  /* Configure AP（把wifi設備放入節點） */
  Ssid ssid = Ssid("network");
  wifiMac.SetType("ns3::ApWifiMac",
                  "Ssid", SsidValue(ssid));

  NetDeviceContainer apDevice;
  apDevice = wifiHelper.Install(wifiPhy, wifiMac, apWifiNode);

  /* Configure STA */
  wifiMac.SetType("ns3::StaWifiMac",
                  "Ssid", SsidValue(ssid));

  NetDeviceContainer staDevices;
  staDevices = wifiHelper.Install(wifiPhy, wifiMac, staWifiNode);

  NetDeviceContainer otherDevices;
  otherDevices = wifiHelper.Install(wifiPhy, wifiMac, other_ue);

  /* Mobility model(設定各個節點移動模型) */

  //走直線

  /*
    直線
 MobilityHelper mobility;
 mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                  "X", StringValue ("10.0"),
                                  "Y", StringValue ("100.0"),
                                  "Rho", StringValue ("ns3::UniformRandomVariable[Min=0|Max=30]"));
mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
 mobility.Install (apWifiNode);
 apWifiNode->GetObject<ConstantVelocityMobilityModel> ()->SetVelocity (Vector (22, 0, 0));
*/

  /*隨機
  mobility2.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                  "X", StringValue ("7.5"),
                                  "Y", StringValue ("7.5"),
                                  "Rho", StringValue ("ns3::UniformRandomVariable[Min=0|Max=30]"));
  
  mobility2.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                              "Mode", StringValue ("Time"),
                              "Time", StringValue ("1s"),
                              "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"),
                              "Bounds", StringValue ("0|200|0|200"));*/

  //設定使用者的wifi(ue) 直線進行

  MobilityHelper mobility;

  mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                "MinX", DoubleValue(0),
                                "MinY", DoubleValue(0),
                                "DeltaX", DoubleValue(5.0),
                                "DeltaY", DoubleValue(10.0),
                                "GridWidth", UintegerValue(3),
                                "LayoutType", StringValue("RowFirst"));
  mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");

  mobility.Install(staWifiNode);

  //apWifiNode->GetObject<MobilityModel> ()->SetPosition (Vector (222, 222, 0));

  //這裡設定固定速度 行走向量 Vector (speed_x, 0, 0) (速度,x向量,y向量)
  staWifiNode->GetObject<ConstantVelocityMobilityModel>()->SetVelocity(Vector(speed_x, speed_y, 0));

  //底下功能 為靜止
  //wifi ap

  MobilityHelper mobility2;
  mobility2.SetPositionAllocator("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue(ap_po_x),
                                 "MinY", DoubleValue(ap_po_y),
                                 "DeltaX", DoubleValue(5.0),
                                 "DeltaY", DoubleValue(10.0),
                                 "GridWidth", UintegerValue(3),
                                 "LayoutType", StringValue("RowFirst"));
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility2.Install(apWifiNode);

  //other_ue 靜止不動
  //
  MobilityHelper mobility3;
  mobility3.SetPositionAllocator("ns3::RandomDiscPositionAllocator",
                                 "X", StringValue(std::to_string(ap_po_x)),
                                 "Y", StringValue(std::to_string(ap_po_x)),
                                 "Rho", StringValue("ns3::UniformRandomVariable[Min=0|Max=30]"));
  /* mobility3.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                              "Mode", StringValue ("Time"),
                              "Time", StringValue ("1s"),
                              "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"),
                              "Bounds", StringValue ("0|200|0|200"));*/

  mobility3.Install(other_ue);

  /* Internet stack */
  InternetStackHelper stack;
  stack.Install(networkNodes);

  InternetStackHelper stack2;
  stack2.Install(other_ue);

  //只是拿來設定ip
  Ipv4AddressHelper address;
  address.SetBase("10.0.0.0", "255.255.255.0");
  Ipv4InterfaceContainer apInterface;
  apInterface = address.Assign(apDevice);
  Ipv4InterfaceContainer staInterface;
  staInterface = address.Assign(staDevices);
  Ipv4InterfaceContainer otherInterface;
  otherInterface = address.Assign(otherDevices);

  /* Populate routing table */
  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  /*
  設定發送端跟接收端是誰
  */
  //這裡是ap 接收端
  /* Install TCP Receiver on the access point */
  PacketSinkHelper sinkHelper("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), 9));
  ApplicationContainer sinkApp = sinkHelper.Install(apWifiNode);
  sink = StaticCast<PacketSink>(sinkApp.Get(0));

  /* Install TCP/UDP Transmitter on the station 發送端 */
  //這裡是我測量的ue

  OnOffHelper server("ns3::TcpSocketFactory", (InetSocketAddress(apInterface.GetAddress(0), 9)));
  server.SetAttribute("PacketSize", UintegerValue(payloadSize));
  server.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
  server.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  server.SetAttribute("DataRate", DataRateValue(DataRate(dataRate)));
  server.SetAttribute("MaxBytes", UintegerValue(MaxBytes));
  ApplicationContainer serverApp = server.Install(staWifiNode);

  //這裡說在其他ue 建立一個應用程式 對ap發送封包
  OnOffHelper otherserver("ns3::TcpSocketFactory", (InetSocketAddress(apInterface.GetAddress(0), 9)));
  otherserver.SetAttribute("PacketSize", UintegerValue(payloadSize));
  otherserver.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
  otherserver.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  otherserver.SetAttribute("DataRate", DataRateValue(DataRate(dataRate)));
  otherserver.SetAttribute("MaxBytes", UintegerValue(MaxBytes));
  ApplicationContainer serverApp_other = server.Install(other_ue);

  /* Start Applications 開啟應用程式的時間 */
  sinkApp.Start(Seconds(0.0));
  serverApp.Start(Seconds(1.0));
  serverApp_other.Start(Seconds(1.0));
  Simulator::Schedule(Seconds(1.0001), &CalculateThroughput);

  /* Enable Traces */

  // wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO);
  // wifiPhy.EnablePcap ("AccessPoint", apDevice);
  //wifiPhy.EnablePcap ("Station", staDevices);

  Config::Connect("/NodeList/*/$ns3::MobilityModel/CourseChange",
                  MakeCallback(&CourseChange));
  /*啟動追蹤檔案 可以用netanim開啟 缺點運算速度慢 因為需要寫入檔案
 AnimationInterface anim (animFile);
  anim.EnablePacketMetadata (); // Optional
  anim.EnableIpv4L3ProtocolCounters (Seconds (0), Seconds (10)); // Optional
  anim.SetMaxPktsPerTraceFile(999999999);//這裡如果設定太小會有警告錯誤
*/

  /* Start Simulation */
  Simulator::Stop(Seconds(simulationTime + 1));
  Simulator::Run();
  Simulator::Destroy();

  std::cout << ans_dataRate << " " << ans_averageThroughput << " " << ans_delay << " " << ans_allbytes << " " << MaxBytes << " " << (double)ans_delay - (double)ans_allbytes * 8 / std::stod(ans_dataRate) / 1024 / 1024 << " " << angle_ans << " " << other_node << " " << rss << " " << speed_x << " " << speed_y << std::endl;

  //std::cout <<"*************************************"<<std::endl;
}

int main(int argc, char *argv[])
{

  /*******************************控制的變數參考***************************/
  //設定訊號強度
  rss = 100; //屬於設定全部wifi的訊號 設定太小會無法執行因為訊號太小 接收不到   主要應影響可以傳輸的範圍 訊號設定超級高 傳輸距離能操過wifi的 100m
  //控制要測量的ue的速度向量
  speed_x = 0;
  speed_y = 11;
  other_node = 200;   //有多少其他人也在使用這個wifiap
  payloadSize = 1024; //設定每一個封包多大
  int package = 100;  //設定多少個封包 注意封包設定太大會有模擬器跑完卻還沒送完問題所以要再增大模擬器時間
  MaxBytes = payloadSize * package;
  //設定datarate (所有node包含ue的連接點datarate)
  dataRate = "54Mbps";
  /*計算移動角度跟ue-ap角度 的夾角*/
  int angle; //ue移動方向的角度
  angle = atan(speed_y / speed_x) * 180 / PI;
  int param; //ap-ue向量的角度
  param = 90.0;
  int range = 10;            //ap距離ue的距離
  angle_ans = angle - param; //行走方向-ap角度等於 方向跟ap夾角 用途是 可以測量ue是否靠近ap的方向前進 夾角為0代表朝向ap 夾角為180代表直接逆向該ap
  //設定ap位置 以ue為中心
  ap_po_x = range * cos(param * PI / 180.0);
  ap_po_y = range * sin(param * PI / 180.0);
  //方向角度減去ap位置角度

  //輸出的格式
  std::cout << "DataRate(Mbps) AverageThroughput(Mbit/s) Delay(sec) APgetDATA(Bytes) MaxBytes deviation-sec(s) angle(角度夾角) other_node（此ap有多少其他連線）rss(dbm) ue-x方向速度(m/s) ue-y方向速度(m/s)\n";
  wifi(argc, argv);
  return 0;
  //底下是我藉由上面參數做的實驗 實驗室 隨機ue速度向量 隨機other_node 隨機rss 隨機datarate 隨機 ap位置所產生的數據
  //亂數
  unsigned seed;
  seed = (unsigned)time(NULL); // 取得時間序列

  srand(seed);

  for (int i = 0; i < 100; i++)
  {
    speed_x = rand() % 20;
    speed_y = rand() % 20;

    other_node = rand() % 100;
    range = rand() % 90 + 10;
    param = rand() % 360;
    rss = -1 * rand() % 60;
    int temp = rand() % 100 + 10;
    dataRate = int2str(temp) + "Mbps";
    angle_ans = angle - param;

    ap_po_x = range * cos(param * PI / 180.0);
    ap_po_y = range * sin(param * PI / 180.0);

    wifi(argc, argv);
  }

  return 0;
}
