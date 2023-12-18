#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <algorithm>

#include "/data4/YuSiang/personalLib/FileIO/FileIO.h"
TString MacrosNames[50]={
  "CaConvertorV2.C",
  "CombinationFacV2.C",
  "DataBaseLocate.C",
  "DataNameAnaV2.C",
  "DataNameAnaV3.C",
  "dataSpaceDirShow.C",
  "DayEAnaV4.C",
  "DSLAnaV5.C",
  "EventAnaV3.C",
  "FitTrackResAnaV2.C",
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
  "MuonElectronIDSub.C",
  "ODectImfAna.C",
  "PwidthAna.C",
  "PWidthPeakAna.C",
  "R4VTAna.C",
  "RunDataSortAnaV3.C",
  "RVHKAna.C",
  "RVTAna.C",
  "RVTCAna.C",
  "SEAnaV2.C",
  "SeqSkipAna.C",
  "STRealEvAna.C",
  "TrackAnaV2.C",
  "TrackAnaV3.C",
  "TrackAnaV4.C",
  "TrackAnaV5.C",
  "TrackAnaSub.C",
  "TrackAnaSubV5.C",
  "VerticalTracksAnaV3.C",
  "VerticalTracksSub.C",
};

void TargetDirLocate(const TString TargetPath="./Macro/Operate/", const TString MacrosPath="./Macro/" ){
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
}

void RenewMacros(const TString TargetPath="./Macro/Operate/", const TString MacrosPath="./Macro/" ){
  TString cmdline;
  cout<<"Start the job: cp "+MacrosPath+" to dir: "+TargetPath<<endl;
  for(int iNames=0; (MacrosNames[iNames] != ""); iNames++){
    cmdline = "cp "+MacrosPath+MacrosNames[iNames]+"  "+TargetPath+MacrosNames[iNames];
    system(cmdline);
  }
  cout<<"End the job:   cp "+MacrosPath+" to dir: "+TargetPath<<endl;
}

void AnaVarSynthx(const char *vertmp, const string PBSET="1111", const TString TargetPath="./Macro/Operate/", const TString MacrosPath="./Macro/"){
  
  cout<<"Start the job:   synthesis AnaVariable.h with detector setting"<<endl;
  cout<<"To be: "<<Form("%sAnaVariable.h",MacrosPath.Data())<<endl;
  string AnaFile_Pre = "./AnaVariable.h";
  string PBSeFile = Form("./PBInduceSelection/PB%s.h",PBSET.data());
  string AnaFile_Sec = Form("%sAnaVariablePB.h",TargetPath.Data());
  string ODetFile;
  string AnaFile_Fin = Form("%sAnaVariable.h",TargetPath.Data());
  
  cout<<"Synthx:\n  "<<AnaFile_Pre<<"\n +"<<PBSeFile<<"\n ="<<AnaFile_Sec<<endl<<endl;
  AppendHeadFile(AnaFile_Pre.data(),PBSeFile.data(), AnaFile_Sec.data());
  cout<<"Finish the job:   synthesis AnaVariablePre.h with PB indeced selection setting"<<endl;

  cout<<"Choosing the version of detector"<<endl;
  if(strcmp(vertmp,"V2.00")==0)
    ODetFile = "./DetInforSet/DetSetV0200.h";
  else if(strcmp(vertmp,"V3.00")==0)
    ODetFile = "./DetInforSet/DetSetV0300.h";
  else if(strcmp(vertmp,"V3.05")==0)
    ODetFile = "./DetInforSet/DetSetV0305.h";
  else if(strcmp(vertmp,"V3.06")==0)
    ODetFile = "./DetInforSet/DetSetV0306.h";
  else if(strcmp(vertmp,"User")==0)
    ODetFile = "./DetInforSet/DetSetUserSet.h";
  else{
    cout<<"Error: The temp version set of vertmp: "<<vertmp<<endl;
    cout<<"Isn't record in data base"<<endl;
    cout<<"Please enter the true verion name:\n";
    cout<<"Hint: V2.00,V3.00,V3.05,User"<<endl;
    throw;
  }
  cout<<"detVer is: "<<vertmp<<endl;
  cout<<"Synthx:\n  "<<AnaFile_Sec<<"\n +"<<ODetFile<<"\n ="<<AnaFile_Fin<<endl<<endl;
  AppendHeadFile(AnaFile_Sec.data(),ODetFile.data(), AnaFile_Fin.data());
  cout<<"Finish the job:   synthesis AnaVariable.h with detector setting"<<endl;
  // system(Form("rm %s",AnaFile_Sec.data()));
  // throw;
}
void RenewMacrosV3(const char *vertmp, const string PBSET="1111", const TString TargetPath="./Macro/Operate/", const TString MacrosPath="./Macro/"){
  TargetDirLocate(TargetPath,MacrosPath);
  
  AnaVarSynthx( vertmp, PBSET, TargetPath, MacrosPath);
  
  RenewMacros(TargetPath,MacrosPath);
}