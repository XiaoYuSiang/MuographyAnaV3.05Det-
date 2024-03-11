#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <algorithm>

#include "/data4/YuSiang/personalLib/FileIO/FileIO.h"
TString MacrosNames[100]={
  "AnaVariable.h",
  "CaConvertorV2.C",
  // "CombinationFacV2.C",
  "CombinationFacV3.C",
  "DataBaseLocate.C",
  "DataNameAnaV3.C",
  "dataSpaceDirShow.C",
  "DayEAnaV4.C",
  "DSLAnaV5.C",
  "EventAnaV3.C",
  "EventAnaV4.C",
  "EventAnaV5.C",
  // "FitTrackResAnaV2.C",
  "FitTrackResAnaV3.C",
  "G4CaConvertorV4.C",
  "GA4x4_2.csv",
  "GA8x8_2.csv",
  "GobelFormats.h",
  "GobelFunctions.h",
  "lineFit3D.h",
  "LoadODectTable.h",
  "HKPosAna.C",
  "HKProAna.C",
  "MuonElectronIDAna.C",
  "MuonElectronIDAnaV2.C",
  "MuonElectronIDAnaV3.C",
  "MuonElectronIDSub.C",
  "MuonElectronIDSubV2.C",
  "ODectImfAna.C",
  "ODectImfAnaV2.C",
  "PwidthAna.C",
  "PWidthPeakAna.C",
  "R4VTAna.C",
  "RunDataSortAnaV3.C",
  "RVHKAna.C",
  "RVTAna.C",
  "RVTCAna.C",
  // "SEAnaV2.C",
  // "SEAnaV3.C",
  "SEAnaV4.C",
  "SeqSkipAna.C",
  "STRealEvAna.C",
  "TrackAnaV2.C",
  "TrackAnaV3.C",
  "TrackAnaV4.C",
  "TrackAnaV5.C",
  "TrackAnaV6.C",
  "TrackAnaSub.C",
  "TrackAnaSubV2.C",
  "TrackAnaSubV3.C",
  "TrackAnaSubV5.C",
  "TrackInFoAna.C",
  "VerticalTracksAnaV3.C",
  "VerticalTracksAnaV4.C",
  "VerticalTracksSub.C",
};

void RenewMacros(const TString TargetPath="./Macro/Operate/", const TString MacrosPath="./Macro/" ){
  TString cmdline = "mkdir -p "+TargetPath;
  if(TargetPath == MacrosPath){
    cout<<"Error: TargetPath & MacrosPath can't be the came path!!!"<<endl;
    cout<<"Now the Enter TargetPath & MacrosPath are: "+TargetPath<<endl;
    cout<<"Program Stop at ./RenewMacros.h::RenewMacros(c.TString TargetPath, c.TString MacrosPath)"<<endl;
    throw;
  }
  if(MacrosPath[MacrosPath.Length()-1]!='/'){
    cout<<MacrosPath[MacrosPath.Length()-1]<<endl;
    cout<<"Error: Please enter format like this /Dir0/Dir1/MacrosPath/ or ./MacrosPath/"<<endl;
    cout<<"Now the Enter MacrosPath is: "+MacrosPath<<endl;
    cout<<"Program Stop at ./RenewMacros.h::RenewMacros(c.TString TargetPath, c.TString MacrosPath)"<<endl;
    throw;
  }
  if(TargetPath[TargetPath.Length()-1]!='/'){
    cout<<"Error: Please enter format like this /Dir0/Dir1/TargetPath/ or ./TargetPath/"<<endl;
    cout<<"Now the Enter TargetPath is: "+TargetPath<<endl;
    cout<<"Program Stop at ./RenewMacros.h::RenewMacros(c.TString TargetPath, c.TString MacrosPath)"<<endl;
    throw;
  }
  if (system(cmdline)!=0){
    system(cmdline);
    cout<<"Not found dir: "+TargetPath+", now located this dir path"<<endl;
  } 
  cout<<"Start the job: cp "+MacrosPath+" to dir: "+TargetPath<<endl;
  system("cp AnaVariable.h  "+TargetPath+"AnaVariable.h");
  for(int iNames=0; (MacrosNames[iNames] != ""); iNames++){
    cmdline = "cp "+MacrosPath+MacrosNames[iNames]+"  "+TargetPath+MacrosNames[iNames];
    system(cmdline);
  }
  cout<<"End the job:   cp "+MacrosPath+" to dir: "+TargetPath<<endl;
}

void RenewMacrosV2(const char *vertmp, const TString TargetPath="./Macro/Operate/", const TString MacrosPath="./Macro/" ){
  cout<<"Start the job:   synthesis AnaVariable.h with detector setting"<<endl;
  cout<<"To be: "<<Form("%sAnaVariable.h",MacrosPath.Data())<<endl;
  // ofstream ofsAnaVabh();
  // ifstream ifsAnaVabh("./AnaVariable.h");
  // string linetmp;
  // while (getline(ifsAnaVabh, linetmp)) ofsAnaVabh << linetmp << '\n';
  // ifsAnaVabh.close();
  
  cout<<"Choosing the version of detector"<<endl;
  if(strcmp(vertmp,"V2.00")==0){
    AppendHeadFile("./AnaVariable.h","./DetInforSet/DetSetV0200.h", Form("%sAnaVariable.h",MacrosPath.Data()));
    // ifstream ifsDetSet("./DetInforSet/DetSetV0200.dat");
    // while (getline(ifsDetSet, linetmp)) ofsAnaVabh << linetmp << '\n';
    // ifsDetSet.close(); 
    cout<<"detVer is: "<<vertmp<<endl;
  }else if(strcmp(vertmp,"V2.03")==0){
    AppendHeadFile("./AnaVariable.h","./DetInforSet/DetSetV0203.h", Form("%sAnaVariable.h",MacrosPath.Data()));
    // ifstream ifsDetSet("./DetInforSet/DetSetV0200.dat");
    // while (getline(ifsDetSet, linetmp)) ofsAnaVabh << linetmp << '\n';
    // ifsDetSet.close(); 
    cout<<"detVer is: "<<vertmp<<endl;
  }else if(strcmp(vertmp,"V3.00")==0){
    AppendHeadFile("./AnaVariable.h","./DetInforSet/DetSetV0300.h", Form("%sAnaVariable.h",MacrosPath.Data()));
    // ifstream ifsDetSet("./DetInforSet/DetSetV0200.dat");
    // while (getline(ifsDetSet, linetmp)) ofsAnaVabh << linetmp << '\n';
    // ifsDetSet.close(); 
    cout<<"detVer is: "<<vertmp<<endl;
  }else if(strcmp(vertmp,"V3.05")==0){
    AppendHeadFile("./AnaVariable.h","./DetInforSet/DetSetV0305.h", Form("%sAnaVariable.h",MacrosPath.Data()));
    // ifstream ifsDetSet("./DetInforSet/DetSetV0200.dat");
    // while (getline(ifsDetSet, linetmp)) ofsAnaVabh << linetmp << '\n';
    // ifsDetSet.close(); 
    cout<<"detVer is: "<<vertmp<<endl;
  }else if(strcmp(vertmp,"V3.06")==0){
    AppendHeadFile("./AnaVariable.h","./DetInforSet/DetSetV0306.h", Form("%sAnaVariable.h",MacrosPath.Data()));
    // ifstream ifsDetSet("./DetInforSet/DetSetV0200.dat");
    // while (getline(ifsDetSet, linetmp)) ofsAnaVabh << linetmp << '\n';
    // ifsDetSet.close(); 
    cout<<"detVer is: "<<vertmp<<endl;
  }else if(strcmp(vertmp,"User")==0){
    AppendHeadFile("./AnaVariable.h","./DetInforSet/DetSetUserSet.h", Form("%sAnaVariable.h",MacrosPath.Data()));
    // ifstream ifsDetSet("./DetInforSet/DetSetV0200.dat");
    // while (getline(ifsDetSet, linetmp)) ofsAnaVabh << linetmp << '\n';
    // ifsDetSet.close(); 
    cout<<"detVer is: "<<vertmp<<endl;
  }else{
    cout<<"Error: The temp version set of vertmp: "<<vertmp<<endl;
    cout<<"Isn't record in data base"<<endl;
    cout<<"Please enter the true verion name:\n";
    cout<<"Hint: V2.00,V2.03,V3.00,V3.05,V3.06,User"<<endl;
    throw;
  }
  // ofsAnaVabh<<"\n}"; 
  // ofsAnaVabh.close();
  cout<<"Finish the job:   synthesis AnaVariable.h with detector setting"<<endl;
  
  RenewMacros(TargetPath,MacrosPath);
}
