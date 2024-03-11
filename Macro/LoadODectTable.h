#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TTree.h>
#include <TGaxis.h>

#include "GobelFormats.h"



using namespace std;
using namespace DBMMath;
using namespace MuoAna_path_dir;
using namespace MuographGobelFuns;
using namespace MuographGobelForm;



map<int, scintillator> LoadODectTable(const char QMode='q', const char* ReadMode="root"){

  char Tmp_ReadMode = FileTypeCheck(ReadMode);
  
  char version[8] = {};
  DetVerCheck(version,detVer);  
  
  char Path_file[100];
  sprintf(Path_file,"%sSciPosDat%s",DirIMFdat,version);
  
  map<int, scintillator> MScints;
  
  if(QMode!='q')  cout<<"mode is: \'"<<Tmp_ReadMode<<"\'\n";
  
  switch(Tmp_ReadMode){
    case 'R':
      {
        char Path_file[100];
        sprintf(Path_file,"%sSciPosDat%s",DirIMFdat,version);
        TFile *_file0 = TFile::Open(Form("%s.root",Path_file));
        scintillator Scints;
        TTree *tPos = (TTree*) _file0 ->Get("tPos");
        Scints.SetSciTreeBranchAddress(tPos,0);

        for(Long64_t i=0;i<tPos->GetEntries();i++){
          tPos->GetEntry(i);
          if(QMode!='q') Scints.Sci_CoutPrint();
          MScints[i] = Scints;
        }
        return MScints;
      }
    case 'T':
      {
        ifstream intxt(Form("%s.txt",Path_file));
        scintillator Scints;
        cout<<"Not finish!!!"<<endl;//@@
        throw;
      }

      return MScints;
    case 'C':
      {
        ifstream incsv(Form("%s.csv",Path_file));
        scintillator Scints;
        cout<<"Not finish!!!"<<endl;//@@
        throw;
      }

      return MScints;
    default:
      cout<<"ERROR: Mode of ReadDetIMF(char *M) is Wrong!"<<endl;
      cout<<"Now, the Mode value is:  \""<<ReadMode<<"\"\n";
      cout<<"Please enter:"<<endl;
      for(int i=0;i<6;i++) cout<<VModeRootOption[i]<<"\t";
      cout<<"For .root files"<<endl;
      for(int i=0;i<6;i++) cout<<VModeTxtOption[i]<<"\t";
      cout<<"For .txt files"<<endl;
      for(int i=0;i<6;i++) cout<<VModeCsvOption[i]<<"\t";
      cout<<"For .csv files"<<endl;
      throw;
  }
}

map<int, tempSensor> LoadODectTable_TSensor(const char QMode='q', const char* ReadMode="root"){

  char Tmp_ReadMode = FileTypeCheck(ReadMode);
  
  char version[8] = {};
  DetVerCheck(version,detVer);  
  
  char Path_file[100];
  sprintf(Path_file,"%sSciPosDat%s",DirIMFdat,version);
  
  map<int, tempSensor> MTSens;
  
  if(QMode!='q')  cout<<"mode is: \'"<<Tmp_ReadMode<<"\'\n";
  
  switch(Tmp_ReadMode){
    case 'R':
      {
        char Path_file[100];
        sprintf(Path_file,"%sSciPosDat%s",DirIMFdat,version);
        TFile *_file0 = TFile::Open(Form("%s.root",Path_file));
        tempSensor TSen;
        TTree *tPot = (TTree*) _file0 ->Get("tPot");
        TSen.SetTSTreeBranchAddress(tPot);

        for(Long64_t i=0;i<tPot->GetEntries();i++){
          tPot->GetEntry(i);
          //if(QMode!='q') TSen.Sci_CoutPrint();
          MTSens[i] = TSen;
        }
        return MTSens;
      }
    case 'T':
      {
        ifstream intxt(Form("%s.txt",Path_file));
        tempSensor TSen;
        cout<<"Not finish!!!"<<endl;//@@
        throw;
      }

      return MTSens;
    case 'C':
      {
        ifstream incsv(Form("%s.csv",Path_file));
        tempSensor Scints;
        cout<<"Not finish!!!"<<endl;//@@
        throw;
      }

      return MTSens;
    default:
      cout<<"ERROR: Mode of ReadDetIMF(char *M) is Wrong!"<<endl;
      cout<<"Now, the Mode value is:  \""<<ReadMode<<"\"\n";
      cout<<"Please enter:"<<endl;
      for(int i=0;i<6;i++) cout<<VModeRootOption[i]<<"\t";
      cout<<"For .root files"<<endl;
      for(int i=0;i<6;i++) cout<<VModeTxtOption[i]<<"\t";
      cout<<"For .txt files"<<endl;
      for(int i=0;i<6;i++) cout<<VModeCsvOption[i]<<"\t";
      cout<<"For .csv files"<<endl;
      throw;
  }
}

map<int, scintillator> LoadEDectTable(const char* EffFile , const char QMode='q'){
  
  map<int, scintillator> MScints;
  
  TFile *_file0 = TFile::Open(EffFile); 
  scintillator Scints; 
  TTree *tERPos = (TTree*) _file0 ->Get("tERPos"); cout<<"M"<<161<<" "<<EffFile<<endl;
  Scints.SetSciTreeBranchAddress(tERPos); 
  
  for(Long64_t i=0;i<tERPos->GetEntries();i++){
    tERPos->GetEntry(i);
    if(QMode!='q') Scints.Sci_CoutPrint();
    MScints[i] = Scints;
  }
  delete tERPos;
  _file0->Close();
  return MScints;

}

inline string getRelCode(int pos[][2],int layerN){
  string res;
  for(int i =1;i<layerN;i++){
    string x=Form("%d",(pos[i][0]-pos[0][0]));
    string y=Form("%d",(pos[i][1]-pos[0][1]));
    if(i!=1)
      res+="_";
    res=res+x+","+y;
  }
  return res;
}

void LoadPosCodeTable(map<int, GeometricAreaData> &MapGAD){
  ifstream in(Form("%sGA8x8_2.csv",DirMacros));
  string Tmp_str;
  getline(in, Tmp_str);
  GeometricAreaData GAD; // cout<<55<<"  "<<MapGAD.size()<<endl;
  while(in.good()){
    getline(in, Tmp_str);// cout<<Tmp_str<<endl;
    istringstream iss(Tmp_str);
    iss>>GAD.poscode;
    iss>>GAD.Vux>>GAD.Vuy>>GAD.Vuz;
    iss>>GAD.Vwx>>GAD.Vwy>>GAD.Vwz;
    iss>>GAD.AreaFactor;
    iss>>GAD.NumbFactor;
    iss>>GAD.DXZ>>GAD.DYZ;
    iss>>GAD.SymFactor;
    GAD.Empty = false;
    MapGAD[MapGAD.size()] = GAD;
  }
  //cout<<67<<"  "<<MapGAD.size()<<endl;
}
void LoadPosCodeTable(map<string, GeometricAreaData> &MapGAD){
  ifstream in(Form("%sGA8x8_2.csv",DirMacros));
  string Tmp_str;
  getline(in, Tmp_str);
  GeometricAreaData GAD;
  while(in.good()){
    getline(in, Tmp_str);// cout<<Tmp_str<<endl;
    istringstream iss(Tmp_str);
    iss>>GAD.poscode;
    iss>>GAD.Vux>>GAD.Vuy>>GAD.Vuz;
    iss>>GAD.Vwx>>GAD.Vwy>>GAD.Vwz;
    iss>>GAD.AreaFactor;
    iss>>GAD.NumbFactor;
    iss>>GAD.DXZ>>GAD.DYZ;
    iss>>GAD.SymFactor;
    GAD.Empty = false;
    MapGAD[GAD.poscode] = GAD;
  }
  // cout<<86<<"  "<<MapGAD.size()<<endl;
}

/*
void FindPos(Experiment *TmpEx, const char *SiteName){
  for(int iCaseName=0;iCaseName<4;iCaseName++){
    if(strcmp(SiteName,TmpEx->Sites[iCaseName])==0){
      TmpEx->Latitude  = Latitudes [iCaseName];
      TmpEx->Longitude = Longitudes[iCaseName];
      break;
    }
  }
}*/