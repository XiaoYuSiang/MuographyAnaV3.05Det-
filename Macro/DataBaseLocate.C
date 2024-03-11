#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TF1.h>
#include "./SettingJSONIO.h"

using namespace std;
const string StrPathErrorCase[5] = { "Raw data", "HK data", "Run data", "Root data", "data base"};
const string StrNameErrorCase[5] = { "Raw data", "HK data"};

void locatedDirbyPath(const char *path_dir){
  char cmdline[180];
  sprintf(cmdline,"mkdir -p %s",path_dir);
  if (system(cmdline)!=0){
    // cout<<"Don't find dir path:  "<<path_dir<<", and create this directory"<<endl;
    system(cmdline);
  }
}
void CheckCorrection( JSONFileIOSet &PMS ){
  
  string paths[5] = {
    string(PMS.path_Raw),     string(PMS.path_Hk),     string(PMS.path_Run), 
    string(PMS.path_Rot),     string(PMS.path_Mac)
  };
  string names[5] = {
    string(PMS.name_MC),     string(PMS.name_Hk)
  };
  bool NotEndWithDiv = false, nameEndWithExt = false;
  for(int ipath=0; ipath<5;ipath++){
    if(paths[ipath][0]=='.'){
      cout<<endl;
      cout<<"\"Error\" Path of "<<StrPathErrorCase[ipath]<<" need to be !!!\"Absolute\" Path!!!"<<endl;
      cout<<"      Now, the enter "<<StrPathErrorCase[ipath]<<" path is: "<<paths[ipath]<<endl;
      cout<<"      Please check the path of <<StrPathErrorCase[ipath]<< !!!"<<endl;
      cout<<endl;
      throw;
    }
    if(paths[ipath][paths[ipath].size()-1] != '/'){
      cout<<endl;
      cout<<"\"Warning\" Path of "<<StrPathErrorCase[ipath]<<" need to be end with '/'!!!"<<endl;
      cout<<"      Now, the enter "<<StrPathErrorCase[ipath]<<" path is: "<<paths[ipath]<<endl;
      cout<<"      Please check the path of "<<StrPathErrorCase[ipath]<<" and end with '/' !!!"<<endl;
      cout<<"\"Warning\" Atomatic add the end of path by '/'."<<endl;
      paths[ipath]+="/";
      cout<<endl;
      NotEndWithDiv = true;
    }
  }
  for(int iname=0;iname<2;iname++){
    if(int(names[iname].find(".txt"))!=-1){
      cout<<endl;
      cout<<"\"Warning\" Name of "<<StrNameErrorCase[iname]<<" needn't end with \".txt\"!!!"<<endl;
      cout<<"      Now, the enter "<<StrNameErrorCase[iname]<<" name is: "<<names[iname]<<endl;
      cout<<"      Please check the name of "<<StrNameErrorCase[iname]<<" and delete \".txt\" !!!"<<endl;
      cout<<"\"Warning\" Atomatic remove the ext name of name."<<endl;
      names[iname]=names[iname].substr(0,names[iname].size()-4);
      cout<<endl;
      nameEndWithExt = true;
    }
  }
  if(NotEndWithDiv){
    sprintf(PMS.path_Raw,"%s",paths[0].data());
    sprintf(PMS.path_Hk ,"%s",paths[1].data());
    sprintf(PMS.path_Run,"%s",paths[2].data());
    sprintf(PMS.path_Rot,"%s",paths[3].data());
    sprintf(PMS.path_Mac,"%s",paths[4].data());
  }
  if(nameEndWithExt){
    if(PMS.MCMode) sprintf(PMS.name_MC,"%s" , names[0].data());
    else            sprintf(PMS.name_Raw,"%s", names[0].data());
    sprintf(PMS.name_Hk,"%s" , names[1].data());
  }
  if(NotEndWithDiv||nameEndWithExt){
    cout<<"\"Warning\" Now, restart to located the databace with correct paths and names!!"<<endl;
    // DataBaseLocateV4( PMS, OPT);
  }
}

void DataBaseLocate( const char *path_dirRawData, const char *path_dirHkData, const char *path_dirAnaData, const char *path_dirMacros="./", const char *name_Raw="*_Mu" , const char *name_Hk="*_HK" ,const char OPT='P'){
  
  cout<<"Error!!! Please use the DataBaseLocate.C::DataBaseLocateV4(c.cdRD*,c.cdHK*,c.cdRT*,c.cdAD*,c.cdMa*,c.cNRD*,c.cNHk*,c.cOPT*) "<<endl;
  throw;
}

void DataBaseLocateV2( const char *path_dirRawData, const char *path_dirHkData, const char *path_dirAnaData, const char *path_dirMacros="./", const char *name_Raw="*_Mu" , const char *name_Hk="*_HK" ,const char OPT='P'){
  
  cout<<"Error!!! Please use the DataBaseLocate.C::DataBaseLocateV4(c.cdRD*,c.cdHK*,c.cdRN*,c.cdRT*,c.cdAD*,c.cdMa*,c.cNRD*,c.cNHk*,c.cNRN*,c.cOPT*) "<<endl;
  throw;
}

void DataBaseLocateV3(
  const char *path_dirRawData, const char *path_dirHkData, const char *path_dirRunData, const char *path_dirRTData, const char *path_dirAnaData, 
  const char *path_dirMacros="./", const char *name_Raw="*_Mu" , const char *name_Hk="*_HK" ,const char OPT='P')
{
  cout<<"Error!!! Please use the DataBaseLocate.C::DataBaseLocateV4(c.cdRD*,c.cdHK*,c.cdRN*,c.cdRT*,c.cdAD*,c.cdMa*,c.cNRD*,c.cNHk*,c.cNRN*,c.cOPT*) "<<endl;
  throw;
}

void DataBaseLocateV4( JSONFileIOSet &PMS ,const char OPT='P'){
  CheckCorrection( PMS );
  char path_dirRawData[200]; sprintf(path_dirRawData,"%s",PMS.path_Raw);
  char path_dirHkData [200]; sprintf(path_dirHkData ,"%s",PMS.path_Hk );
  char path_dirRunData[200]; sprintf(path_dirRunData,"%s",PMS.path_Run);
  char path_dirRTData [200]; sprintf(path_dirRTData ,"%s",PMS.path_Rot);
  char path_dirAnaData[200]; sprintf(path_dirAnaData,"%s",PMS.path_OpR);
  char path_dirMacros [200]; sprintf(path_dirMacros ,"%s",PMS.path_Mac);
  char name_Raw        [200]; sprintf(name_Raw        ,"%s",PMS.MCMode ? PMS.name_MC : PMS.name_Raw);
  char name_Hk         [200]; sprintf(name_Hk         ,"%s",PMS.name_Hk );
  
  char DirMacros[150];
  sprintf(DirMacros ,"%s"               ,path_dirMacros);
  char DirRunfile[150];
  sprintf(DirRunfile,"%s"               ,path_dirRunData);
  char DirRunTillfile[150];
  sprintf(DirRunTillfile,"%sTillData/"  ,path_dirRunData);
  locatedDirbyPath(DirRunTillfile);
  char DirRotfile[150];
  sprintf(DirRotfile,"%s"               ,path_dirRTData);
  locatedDirbyPath(DirRotfile);
  char DirIMFdat[150]={}, DirOperate[150]={}, DirResult [150]={}, DirHousKep[150]={};
  sprintf(DirIMFdat ,"%sDetIMF/"       ,path_dirAnaData);
  sprintf(DirOperate,"%sOperateResule/",path_dirAnaData);
  sprintf(DirResult ,"%sResult/"       ,path_dirAnaData);
  sprintf(DirHousKep ,"%sHouseKeep/"   ,path_dirAnaData);
  locatedDirbyPath(DirIMFdat);
  // locatedDirbyPath(DirRotfile);
  locatedDirbyPath(DirOperate);
  locatedDirbyPath(DirResult);
  locatedDirbyPath(DirHousKep);
  
  char DirOperEV[170],      DirOperEVGT[170],      DirOperEVGTST[170],    DirOperEVGTLF[170];
    /*Raw data to Event,  Select by nH and Gap,  straight line tracks,  line fitting tracking*/
  sprintf(DirOperEV     ,"%sRawEventDat/"  ,DirOperate);
  sprintf(DirOperEVGT   ,"%sEventGapDat/"  ,DirOperate);
  sprintf(DirOperEVGTST ,"%sEvGTrackDat/"  ,DirOperate);
  sprintf(DirOperEVGTLF ,"%sEvGTLFitDat/"  ,DirOperate);
  locatedDirbyPath(DirOperEV);
  locatedDirbyPath(DirOperEVGT);
  locatedDirbyPath(DirOperEVGTST);
  locatedDirbyPath(DirOperEVGTLF);
  
  char DirRes_ChEff[170], DirRes_ChPw[170], DirRes_ChRt[170], DirRes_PwVCase[170], DirRes_LFirTra[170];
    /*Raw data to Event,  Select by nH and Gap,  straight line tracks,  line fitting tracking*/
  sprintf(DirRes_ChEff  ,"%sBoardChannelEffective/"  ,DirResult);
  sprintf(DirRes_ChPw   ,"%sBoardGPwNor/"         ,DirResult);
  sprintf(DirRes_ChRt   ,"%sBoardGRtNor/"         ,DirResult);
  sprintf(DirRes_PwVCase,"%sBoardPwVCase/"           ,DirResult);
  sprintf(DirRes_LFirTra,"%sTracking_Result/"        ,DirResult);
  locatedDirbyPath(DirRes_ChEff);
  locatedDirbyPath(DirRes_ChPw);
  locatedDirbyPath(DirRes_ChRt);
  locatedDirbyPath(DirRes_PwVCase);
  locatedDirbyPath(DirRes_LFirTra);

  
  char DirRes_RatVT[170], DirRes_PwidVT[170], DirRes_HousKep[170], DirRes_OpEff[170];
  sprintf(DirRes_RatVT  ,"%sRateVsTime/"        ,DirResult);
  sprintf(DirRes_PwidVT ,"%sPwidthVsTime/"      ,DirResult);
  sprintf(DirRes_HousKep,"%sHouseKeeping/"      ,DirResult);
  sprintf(DirRes_OpEff  ,"%sOperateVsDate/"     ,DirResult);
  locatedDirbyPath(DirRes_RatVT);
  locatedDirbyPath(DirRes_PwidVT);
  locatedDirbyPath(DirRes_HousKep);
  locatedDirbyPath(DirRes_OpEff);
  
  char DirRes_TempVT[170], DirRes_HumiVT[170], DirRes_DayEff[170],DirRes_RatVCh[170],DirRes_RatVSun[170];
  sprintf(DirRes_TempVT  ,"%sTemperatureVsTime/" ,DirResult);
  sprintf(DirRes_HumiVT  ,"%sHumidityVsTime/"    ,DirResult);
  sprintf(DirRes_DayEff  ,"%sDayEff/"            ,DirResult);
  sprintf(DirRes_RatVCh  ,"%sRateVsChannel/"     ,DirResult);
  sprintf(DirRes_RatVSun ,"%sRateVsSunnyDay/"    ,DirResult);
  locatedDirbyPath(DirRes_TempVT);
  locatedDirbyPath(DirRes_HumiVT);
  locatedDirbyPath(DirRes_DayEff);
  locatedDirbyPath(DirRes_RatVCh);
  locatedDirbyPath(DirRes_RatVSun);
  
  char DirRes_HKVRat[170],DirRes_SeqSkip[170],DirRes_TcntGap[170],DirRes_FluxCal[170];
  sprintf(DirRes_HKVRat  ,"%sHuoseKeepDatVsRate/",DirResult);
  sprintf(DirRes_SeqSkip ,"%sSequanceSkip/"      ,DirResult);
  sprintf(DirRes_TcntGap ,"%sTcntGap/"           ,DirResult);
  sprintf(DirRes_FluxCal ,"%sFluxCalculate/"     ,DirResult);
  locatedDirbyPath(DirRes_HKVRat);
  locatedDirbyPath(DirRes_SeqSkip);
  locatedDirbyPath(DirRes_TcntGap);
  locatedDirbyPath(DirRes_FluxCal);
  
  
  
  char name_Raw_txt [50], name_Raw_root[50], name_RMC_root[50], name_Hk_txt[50];
  sprintf (name_Raw_txt , "%s.txt" ,name_Raw);
  sprintf (name_Raw_root, "%s.root",name_Raw);
  sprintf (name_RMC_root, "%s.root",name_Raw);
  sprintf (name_Hk_txt  , "%s.txt" ,name_Hk );
  cout<<198<<endl;
  cout<<"name_Raw_txt:   "<<name_Raw_txt<<endl;
  cout<<"name_Raw_root:  "<<name_Raw_root<<endl;
  cout<<"name_RMC_root:  "<<name_RMC_root<<endl;
  cout<<"name_Hk_txt:  "  <<name_Hk_txt<<endl;
  
  
  
  
  
  
  
  
  
  
  
  
  cout<<"Located the path_dir.h @ "<<path_dirMacros<<endl;
  /* Locate Dir Path data HeadFile */
  ofstream outHfile(Form("%spath_dir.h",path_dirMacros));
  outHfile<<"#include <iostream>"<<endl;
  outHfile<<"#include <vector>"<<endl;
  outHfile<<"#include <fstream>"<<endl;
  outHfile<<"#include <typeinfo>"<<endl;
  outHfile<<"#include <algorithm>"<<endl;
  outHfile<<""<<endl;
  outHfile<<"namespace MuoAna_path_dir{"<<endl;
  outHfile<<"  /*  Path of Macros!!!( *.C or *.h ) */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirMacros"     ,path_dirMacros)<<endl;
  outHfile<<"  /*  Path of Raw Datas (*_Mu.txt) */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRawData"    ,path_dirRawData)<<endl;
  outHfile<<"  /*  Path of Hk Datas (*_HK.txt) */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirHkData"     ,path_dirHkData)<<endl;
  outHfile<<"  /*  Path of Run Datas (Setup_*.txt) */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRunData"    ,DirRunfile)<<endl;
  outHfile<<"  /*  Path of Run Till_Datas (Setup_Till_*.R/T/C) */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRunTillfile",DirRunTillfile)<<endl;
  outHfile<<"  /*  Path of Root Datas (*_Mu.root) */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRTData"     ,DirRotfile)<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRotfile"    ,DirRotfile)<<endl;
  outHfile<<"  /*  Root path of Save Datas  */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirAnaPath"    ,path_dirAnaData)<<endl;
  outHfile<<"  /*  Path of All type Dir in root path  */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirIMFdat"     ,DirIMFdat     )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirOperate"    ,DirOperate    )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirResult"     ,DirResult     )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirHousKep"    ,DirHousKep    )<<endl;
  outHfile<<"  /*  Path of Operation result and data  */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirOperEV"     ,DirOperEV     )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirOperEVGT"   ,DirOperEVGT   )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirOperEVGTST" ,DirOperEVGTST )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirOperEVGTLF" ,DirOperEVGTLF )<<endl;
  outHfile<<"  /*  Path of Result and data  */"<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_ChEff"  ,DirRes_ChEff  )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_ChRt"   ,DirRes_ChRt )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_ChPw"   ,DirRes_ChPw )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_PwVCase",DirRes_PwVCase)<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_LFirTra",DirRes_LFirTra)<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_RatVT"  ,DirRes_RatVT  )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_PwidVT" ,DirRes_PwidVT )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_HousKep",DirRes_HousKep)<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_OpEff"  ,DirRes_OpEff  )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_TempVT" ,DirRes_TempVT )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_HumiVT" ,DirRes_HumiVT )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_DayEff" ,DirRes_DayEff )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_RatVCh" ,DirRes_RatVCh )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_RatVSun",DirRes_RatVSun)<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_HKVRat" ,DirRes_HKVRat )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_SeqSkip",DirRes_SeqSkip )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_TcntGap",DirRes_TcntGap )<<endl;
  outHfile<<Form("  const char %-16.16s[180] = \"%s\";","DirRes_FluxCal",DirRes_FluxCal )<<endl;
  outHfile<<"  /*  Name marker of raw datas  */"<<endl;
  outHfile<<Form("  const char %-16.16s[50] = \"%s\";","name_Raw_txt"   ,name_Raw_txt  )<<endl;
  outHfile<<Form("  const char %-16.16s[50] = \"%s\";","name_Raw_root"  ,name_Raw_root )<<endl;
  outHfile<<Form("  const char %-16.16s[50] = \"%s\";","name_RMC_root"  ,name_RMC_root )<<endl;
  outHfile<<Form("  const char %-16.16s[50] = \"%s\";","name_Hk_txt"    ,name_Hk_txt    )<<endl;
  cout<<Form("  const char %-16.16s[50] = \"%s\";","name_Raw_txt"   ,name_Raw_txt  )<<endl;
  cout<<Form("  const char %-16.16s[50] = \"%s\";","name_Raw_root"  ,name_Raw_root )<<endl;
  cout<<Form("  const char %-16.16s[50] = \"%s\";","name_RMC_root"  ,name_RMC_root )<<endl;
  cout<<Form("  const char %-16.16s[50] = \"%s\";","name_Hk_txt"    ,name_Hk_txt    )<<endl;
  
  outHfile<<"};"<<endl;
  outHfile.close();

  if(OPT=='P'){
  
    cout<<"Establishing the Data base!!"<<endl;
    cout<<"  /*  Path of Raw Datas (*_Mu.txt) */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRawData"    ,path_dirRawData)<<endl;
    cout<<"  /*  Path of Hk Datas (*_HK.txt) */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirHkData"     ,path_dirHkData)<<endl;
    cout<<"  /*  Path of Run Datas (Setup_*.txt) */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRunData"    ,DirRunfile)<<endl;
    cout<<"  /*  Path of Run Till_Datas (Setup_Till_*.R/T/C) */"<<endl;
    cout<<Form("  c char*    %-16.16s = \"%s","DirRunTillfile",DirRunTillfile)<<endl;
    cout<<"  /*  Path of Root Datas (*_Mu.root) */"<<endl;
    cout<<Form("  c char*    %-16.16s = \"%s","DirRTData"     ,DirRotfile)<<endl;
    cout<<Form("  c char*    %-16.16s = \"%s","DirRotfile"    ,DirRotfile)<<endl;
    cout<<"  /*  Root path of Save Datas  */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirAnaPath"    ,path_dirAnaData)<<endl;
    cout<<"  /*  Path of All type Dir in root path  */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirIMFdat"     ,DirIMFdat     )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirOperate"    ,DirOperate    )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirResult"     ,DirResult     )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirHousKep"    ,DirHousKep    )<<endl;
    cout<<"  /*  Path of Operation result and data  */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirOperEV"     ,DirOperEV     )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirOperEVGT"   ,DirOperEVGT   )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirOperEVGTST" ,DirOperEVGTST )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirOperEVGTLF" ,DirOperEVGTLF )<<endl;
    cout<<"  /*  Path of Result and data  */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_ChEff"  ,DirRes_ChEff  )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_ChRt"   ,DirRes_ChRt   )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_ChPw"   ,DirRes_ChPw   )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_PwVCase",DirRes_PwVCase)<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_LFirTra",DirRes_LFirTra)<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_RatVT"  ,DirRes_RatVT  )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_PwidVT" ,DirRes_PwidVT )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_HousKep",DirRes_HousKep)<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_OpEff"  ,DirRes_OpEff  )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_TempVT" ,DirRes_TempVT )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_HumiVT" ,DirRes_HumiVT )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_DayEff" ,DirRes_DayEff )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_RatVCh" ,DirRes_RatVCh )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_RatVSun",DirRes_RatVSun)<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_HKVRat" ,DirRes_HKVRat )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_SeqSkip",DirRes_SeqSkip)<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_TcntGap",DirRes_TcntGap)<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","DirRes_FluxCal",DirRes_FluxCal)<<endl;
    cout<<"  /*  Name marker of raw datas  */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","name_Raw_txt"  ,name_Raw_txt  )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","name_Raw_root" ,name_Raw_root )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","name_RMC_root" ,name_RMC_root )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","name_Hk_txt"   ,name_Hk_txt    )<<endl;
    cout<<"Finish to establish the Data base!!"<<endl;
    
  }else{
    cout<<"hint: Skip Print the Information of Directory are establish.(!='P')"<<endl;
  }

}

