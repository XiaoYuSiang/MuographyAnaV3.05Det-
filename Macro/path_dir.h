#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>

namespace MuoAna_path_dir{
  /*  Path of Macros!!!( *.C or *.h ) */
  const char DirMacros       [180] = "./Macro1/";
  /*  Path of Raw Datas (*_Mu.txt) */
  const char DirRawData      [180] = "/data4/YuSiang/DaXi/";
  /*  Path of Hk Datas (*_HK.txt) */
  const char DirHkData       [180] = "/data4/YuSiang/DaXi/HKData/";
  /*  Path of Run Datas (Setup_*.txt) */
  const char DirRunData      [180] = "/data4/YuSiang/DaXi/OdetData/";
  /*  Path of Run Till_Datas (Setup_Till_*.R/T/C) */
  const char DirRunTillfile  [180] = "/data4/YuSiang/DaXi/OdetData/TillData/";
  /*  Path of Root Datas (*_Mu.root) */
  const char DirRTData       [180] = "/data4/YuSiang/DaXi/rootfile/";
  const char DirRotfile      [180] = "/data4/YuSiang/DaXi/rootfile/";
  /*  Root path of Save Datas  */
  const char DirAnaPath      [180] = "/data4/YuSiang/DaXi/Ana/";
  /*  Path of All type Dir in root path  */
  const char DirIMFdat       [180] = "/data4/YuSiang/DaXi/Ana/DetIMF/";
  const char DirOperate      [180] = "/data4/YuSiang/DaXi/Ana/OperateResule/";
  const char DirResult       [180] = "/data4/YuSiang/DaXi/Ana/Result/";
  const char DirHousKep      [180] = "/data4/YuSiang/DaXi/Ana/HouseKeep/";
  /*  Path of Operation result and data  */
  const char DirOperEV       [180] = "/data4/YuSiang/DaXi/Ana/OperateResule/RawEventDat/";
  const char DirOperEVGT     [180] = "/data4/YuSiang/DaXi/Ana/OperateResule/EventGapDat/";
  const char DirOperEVGTST   [180] = "/data4/YuSiang/DaXi/Ana/OperateResule/EvGTrackDat/";
  const char DirOperEVGTLF   [180] = "/data4/YuSiang/DaXi/Ana/OperateResule/EvGTLFitDat/";
  /*  Path of Result and data  */
  const char DirRes_ChEff    [180] = "/data4/YuSiang/DaXi/Ana/Result/BoardChannelEffective/";
  const char DirRes_ChRt     [180] = "/data4/YuSiang/DaXi/Ana/Result/BoardGRtNor/";
  const char DirRes_ChPw     [180] = "/data4/YuSiang/DaXi/Ana/Result/BoardGPwNor/";
  const char DirRes_PwVCase  [180] = "/data4/YuSiang/DaXi/Ana/Result/BoardPwVCase/";
  const char DirRes_LFirTra  [180] = "/data4/YuSiang/DaXi/Ana/Result/Tracking_Result/";
  const char DirRes_RatVT    [180] = "/data4/YuSiang/DaXi/Ana/Result/RateVsTime/";
  const char DirRes_PwidVT   [180] = "/data4/YuSiang/DaXi/Ana/Result/PwidthVsTime/";
  const char DirRes_HousKep  [180] = "/data4/YuSiang/DaXi/Ana/Result/HouseKeeping/";
  const char DirRes_OpEff    [180] = "/data4/YuSiang/DaXi/Ana/Result/OperateVsDate/";
  const char DirRes_TempVT   [180] = "/data4/YuSiang/DaXi/Ana/Result/TemperatureVsTime/";
  const char DirRes_HumiVT   [180] = "/data4/YuSiang/DaXi/Ana/Result/HumidityVsTime/";
  const char DirRes_DayEff   [180] = "/data4/YuSiang/DaXi/Ana/Result/DayEff/";
  const char DirRes_RatVCh   [180] = "/data4/YuSiang/DaXi/Ana/Result/RateVsChannel/";
  const char DirRes_RatVSun  [180] = "/data4/YuSiang/DaXi/Ana/Result/RateVsSunnyDay/";
  const char DirRes_HKVRat   [180] = "/data4/YuSiang/DaXi/Ana/Result/HuoseKeepDatVsRate/";
  const char DirRes_SeqSkip  [180] = "/data4/YuSiang/DaXi/Ana/Result/SequanceSkip/";
  const char DirRes_TcntGap  [180] = "/data4/YuSiang/DaXi/Ana/Result/TcntGap/";
  /*  Name marker of raw datas  */
  const char name_Raw_txt    [50] = "*_Mu.txt";
  const char name_Raw_root   [50] = "*_Mu.root";
  const char name_Hk_txt     [50] = "*_HK.txt";
};
