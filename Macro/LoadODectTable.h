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
        
        tPos->SetBranchAddress("GeneralID"   ,&Scints.GeneralID);
        tPos->SetBranchAddress("name"        ,&Scints.name     );
        tPos->SetBranchAddress("boardID"     ,&Scints.boardID  );
        tPos->SetBranchAddress("channelID"   ,&Scints.channelID);
        tPos->SetBranchAddress("iX"          ,&Scints.iX       );
        tPos->SetBranchAddress("iY"          ,&Scints.iY       );
        tPos->SetBranchAddress("iZ"          ,&Scints.iZ       );
        tPos->SetBranchAddress("BiX"         ,&Scints.BiX      );
        tPos->SetBranchAddress("BiY"         ,&Scints.BiY      );
        tPos->SetBranchAddress("BiZ"         ,&Scints.BiZ      );
        tPos->SetBranchAddress("pX"          ,&Scints.pX       );
        tPos->SetBranchAddress("pY"          ,&Scints.pY       );
        tPos->SetBranchAddress("pZ"          ,&Scints.pZ       );
        tPos->SetBranchAddress("face"        ,&Scints.face     );
        tPos->SetBranchAddress("PwWei"       ,&Scints.PwWei    );

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