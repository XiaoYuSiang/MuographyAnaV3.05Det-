#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <algorithm>

TString MacrosNames[50]={
  "CaConvertor.C",
  "DataBaseLocate.C",
  "DataNameAna.C",
  "dataSpaceDirShow.C",
  "DayEAnaV3.C",
  "DSLAnaV2.C",
  "EventAna.C",
  "FitTrackResAna.C",
  "GobelFormats.h",
  "GobelFunctions.h",
  "lineFit3D.h",
  "LoadODectTable.h",
  "HKProAna.C",
  "MuonElectronID.C",
  "ODectImfAna.C",
  "PwidthAna.C",
  "PWidthPeakAna.C",
  "R4VTAna.C",
  "RunDataSortAna.C",
  "RVHKAna.C",
  "RVTAna.C",
  "RVTCAna.C",
  "SEAnaV2.C",
  "SeqSkipAna.C",
  "STRealEvAna.C",
  "TrackAna.C",
  "VerticalTracksAna.C",
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

