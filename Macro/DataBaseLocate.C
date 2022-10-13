#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TF1.h>

using namespace std;
void locatedDirbyPath(const char *path_dir){
  char cmdline[180];
  sprintf(cmdline,"mkdir -p %s",path_dir);
  if (system(cmdline)!=0){
    // cout<<"Don't find dir path:  "<<path_dir<<", and create this directory"<<endl;
    system(cmdline);
  }
}


void DataBaseLocate( const char *path_dirRawData, const char *path_dirHkData, const char *path_dirAnaData, const char *path_dirMacros="./", const char *name_Raw="*_Mu" , const char *name_Hk="*_HK" ,const char OPT='P'){
  
  cout<<"Error!!! Please use the DataBaseLocate.C::DataBaseLocateV3(c.cdRD*,c.cdHK*,c.cdRT*,c.cdAD*,c.cdMa*,c.cNRD*,c.cNHk*,c.cOPT*) "<<endl;
  throw;
}

void DataBaseLocateV2( const char *path_dirRawData, const char *path_dirHkData, const char *path_dirAnaData, const char *path_dirMacros="./", const char *name_Raw="*_Mu" , const char *name_Hk="*_HK" ,const char OPT='P'){
  
  cout<<"Error!!! Please use the DataBaseLocate.C::DataBaseLocateV3(c.cdRD*,c.cdHK*,c.cdRN*,c.cdRT*,c.cdAD*,c.cdMa*,c.cNRD*,c.cNHk*,c.cNRN*,c.cOPT*) "<<endl;
  throw;
}

void DataBaseLocateV3(
  const char *path_dirRawData, const char *path_dirHkData, const char *path_dirRunData, const char *path_dirRTData, const char *path_dirAnaData, 
  const char *path_dirMacros="./", const char *name_Raw="*_Mu" , const char *name_Hk="*_HK" ,const char OPT='P')
{
  
  
  if(path_dirRawData[0]=='.'){
    cout<<endl;
    cout<<"\"Error\" Path of Raw data need to be \"Full\" Path!!!"<<endl;
    cout<<"      Now, the enter Raw data path is: "<<path_dirRawData<<endl;
    cout<<"      Please check the path of Raw dat !!!"<<path_dirRawData<<endl;
    cout<<endl;
    throw;
  }else if(path_dirHkData[0]=='.'){
    cout<<endl;
    cout<<"\"Error\" Path of HK data need to be \"Full\" Path!!!"<<endl;
    cout<<"      Now, the enter HK data path is: "<<path_dirHkData<<endl;
    cout<<"      Please check the path of HK dat !!!"<<path_dirHkData<<endl;
    cout<<endl;
    throw;
  }else if(path_dirRunData[0]=='.'){
    cout<<endl;
    cout<<"\"Error\" Path of Run data need to be \"Full\" Path!!!"<<endl;
    cout<<"      Now, the enter Run data path is: "<<path_dirRunData<<endl;
    cout<<"      Please check the path of Run dat !!!"<<path_dirRunData<<endl;
    cout<<endl;
    throw;
  }else if(path_dirRTData[0]=='.'){
    cout<<endl;
    cout<<"\"Error\" Path of Root data need to be \"Full\" Path!!!"<<endl;
    cout<<"      Now, the enter Root data path is: "<<path_dirRTData<<endl;
    cout<<"      Please check the path of Root dat !!!"<<path_dirRTData<<endl;
    cout<<endl;
    throw;
  }else if (path_dirAnaData[0]=='.'){
    cout<<endl;
    cout<<"\"Error\" Root path of data base need to be \"Full\" Path!!!"<<endl;
    cout<<"      Now, the enter data base path is: "<<path_dirAnaData<<endl;
    cout<<"      Please check the path of root !!!"<<path_dirAnaData<<endl;
    cout<<endl;
    throw;
  }
  
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
  
  char DirRes_HKVRat[170],DirRes_SeqSkip[170],DirRes_TcntGap[170];
  sprintf(DirRes_HKVRat  ,"%sHuoseKeepDatVsRate/",DirResult);
  sprintf(DirRes_SeqSkip ,"%sSequanceSkip/"      ,DirResult);
  sprintf(DirRes_TcntGap ,"%sTcntGap/"           ,DirResult);
  locatedDirbyPath(DirRes_HKVRat);
  locatedDirbyPath(DirRes_SeqSkip);
  locatedDirbyPath(DirRes_TcntGap);
  
  
  
  char name_Raw_txt [50], name_Raw_root[50], name_Hk_txt[50];
  sprintf (name_Raw_txt , "%s.txt" ,name_Raw);
  sprintf (name_Raw_root, "%s.root",name_Raw);
  sprintf (name_Hk_txt  , "%s.txt" ,name_Hk );
  cout<<"name_Raw_txt:   "<<name_Raw_txt<<endl;
  cout<<"name_Raw_root:  "<<name_Raw_root<<endl;
  cout<<"name_Hk_txt:  "  <<name_Hk_txt<<endl;
  
  
  
  
  
  
  
  
  
  
  
  
  
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
  outHfile<<"  /*  Name marker of raw datas  */"<<endl;
  outHfile<<Form("  const char %-16.16s[50] = \"%s\";","name_Raw_txt"   ,name_Raw_txt  )<<endl;
  outHfile<<Form("  const char %-16.16s[50] = \"%s\";","name_Raw_root"  ,name_Raw_root )<<endl;
  outHfile<<Form("  const char %-16.16s[50] = \"%s\";","name_Hk_txt"    ,name_Hk_txt    )<<endl;
  
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
    cout<<"  /*  Name marker of raw datas  */"<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","name_Raw_txt"  ,name_Raw_txt  )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","name_Raw_root" ,name_Raw_root )<<endl;
    cout<<Form("  c.char*    %-16.16s = \"%s","name_Hk_txt"   ,name_Hk_txt    )<<endl;
    cout<<"Finish to establish the Data base!!"<<endl;
    
  }else{
    cout<<"hint: Skip Print the Information of Directory are establish.(!='P')"<<endl;
  }

}

