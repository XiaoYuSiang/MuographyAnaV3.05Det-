#include "TTree.h"
#include "TFile.h"
#include <TH1F.h>

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <vector>
#if defined (__MAKECINT__) 
#pragma link C++ class vector<Long64_t>+; 
#endif
#include "path_dir.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"

using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;

// C++ program for the above approach

// Unix time is in seconds and
// Humar Readable Format:
// DATE:MONTH:YEAR:HOUR:MINUTES:SECONDS,
// Start of unix time:01 Jan 1970, 00:00:00
//#include <bits/stdc++.h>

// Function to convert unix time to
// Human readable format

struct Tmpdata {
  Int_t     bd;
  Int_t     ch;
  Int_t     se;
  double    dt;
  Long64_t  dtI;
  Int_t     pc;
  Long64_t  tc;
  Int_t     pw;
};

struct Timdata {
  Int_t     fr;
  Int_t     nH;
  Long64_t  ut;
};

bool mycompare(Tmpdata  s1, Tmpdata  s2){
   return s1.dt < s2.dt;
}


void CaConvertor(TString filename="/data4/YuSiang/DaXi/20220907_Run1_0_Mu.txt", TString ofile="/data4/YuSiang/DaXi/Ana2/rootfile/20220907_Run1_0_Mu.root") {

  ifstream infile;
  infile.open(filename);
  
  Int_t            frame_= 0 ;
  Long64_t         unixtime_;
  Int_t            tYear_;
  Int_t            tMonth_;
  Int_t            tDate_;
  Int_t            tHour_;
  Int_t            tMinute_;
  Int_t            tSecond_;
  Int_t            nHits_;
  
  
  vector<Int_t>      board_;
  vector<Int_t>      channel_;
  vector<Int_t>      seq_;
  vector<Int_t>      pcnt_;
  vector<Long64_t>   tcnt_;
  vector<double>     dtime_;
  vector<Long64_t>   dtimeInt_;
  vector<Int_t>      pwidth_;

  TFile *f = new TFile(ofile, "recreate");

  TTree *t = new TTree("t", "frame data");
  t->Branch("frame",    &frame_);     
  t->Branch("unixtime", &unixtime_);
  t->Branch("tYear",    &tYear_);
  t->Branch("tMonth",   &tMonth_);
  t->Branch("tDate",    &tDate_);
  t->Branch("tHour",    &tHour_);
  t->Branch("tMinute",  &tMinute_);
  t->Branch("tSecond",  &tSecond_);
  t->Branch("nHits",    &nHits_);
  t->Branch("board",    &board_);
  t->Branch("channel",  &channel_);
  t->Branch("seq",      &seq_);
  t->Branch("dtime",    &dtime_);
  t->Branch("dtimeInt", &dtimeInt_);
  t->Branch("pcnt",     &pcnt_);
  t->Branch("tcnt",     &tcnt_);
  t->Branch("pwidth",   &pwidth_);     

  nHits_ = 0;

  std::string line;
  Long64_t tcntB1 = 0,pcntB1 = 0;
  vector<struct Tmpdata> data1;
  vector<struct Timdata> time1;
  int nHits1 = 0, nHits2 = 0;

  while (std::getline(infile, line)) {

    std::string s1, s2;
    Int_t frame, frame0;
    Long64_t unixtime = 0, unixtime0 =0;

    Int_t a=0, board=0, channel=0, seq=0, pcnt=0, pwidth=0;
    Long64_t tcnt,dtimeInt;
    double dtime;

    std::istringstream iss(line);
    size_t pos = line.find("Frame");

    if (pos != string::npos) { // the line containing "#Frame"

      if ((iss >> s1 >> frame >> s2 >> unixtime)) { 
        //if (frame_ >= 28588) continue;
        nHits1 = nHits2;
        nHits2 = 0;
        
        Timdata Tim_tmp = {frame,nHits2,unixtime};
        time1.push_back(Tim_tmp);
        if(time1.size()>1){
          time1[time1.size()-2].nH = nHits1;
        }
      } else {
        cout<<"Warning : a bad line!! "<<line<<endl;
      }

    } else {
      //if (frame_ >= 28588) continue;
      if ((iss >> a >> board >> channel >> seq >> dtime >> pcnt >> tcnt >> pwidth)) {
        if(board==1||pcntB1==0){
          tcntB1 = tcnt;
          pcntB1 = pcnt;
        }else if(pcnt<pcntB1){
          pcnt = pcntB1;
          if(tcnt>2.56E+9) {
            pcnt++;
            tcnt-=2.56E+9;
            cout<<"CaConV.C: Auto adjust: pcnt lager than 2.56E+9."<<endl;
          }else if(tcntB1>2.E+9&&tcnt<1.E+9){
            pcntB1++;
            pcnt = pcntB1;
            cout<<"CaConV.C: Auto adjust: pcnt carry."<<endl;
          }
          
          if(tcnt>2.56E+9)cout<<pcnt<<"/"<<pcntB1<<" "<<Form("%.11f",dtime)<<"Warning : disconnect over 1 second = 2.56E+9 pcnt"<<"\n";
          //cout<<pcnt<<"/"<<pcntB1<<" "<<Form("%.11f",dtime)<<"\n";
        }
        dtime = pcnt*1.+(1.*tcnt)/2.56E+9;
        dtimeInt = pcnt*2.56E+9+tcnt;
        //cout<<pcnt<<" "<<tcnt<<" "<<dtimeInt<<" ";
        Tmpdata dataT = {board,channel,seq,dtime,dtimeInt,pcnt,tcnt,pwidth};
        data1.push_back(dataT);
        //cout<<dataT.dtI<<endl;
        nHits2++;
        //cout<<nHits_<<" "<<a<<" "<<board<<" "<<channel<<" "<<seq<<" "<<dtime<<" "<<pcnt<<" "<<tcnt<<" "<<pwidth<<endl;
      } else {
        cout<<"Warning : a bad line!! "<<line<<endl;
      }
      
    }
    
  }
  time1[time1.size()-1].nH = nHits2;
  sort(data1.begin(), data1.end(), mycompare);
  cout<<"size:  "<<int(data1.size());


  int totalfr = int(time1.size());
  cout<<"\tNframe:  "<<totalfr<<endl;
  int iHits=0;
  cout<<time1[0].fr<<"\t"<<time1[0].nH<<"\t"<<time1[0].ut<<"\n";
  for(int ifr=0;ifr<totalfr;ifr++){
    frame_    = time1[ifr].fr;
    nHits_    = time1[ifr].nH;
    unixtime_ = time1[ifr].ut;
    unixTimeToHumanReadable(unixtime_,
        tYear_, tMonth_, tDate_, tHour_, tMinute_, tSecond_,timeZone);
    for(int i1=0;i1<nHits_;i1++){
      board_   .push_back(data1[iHits].bd);
      channel_ .push_back(data1[iHits].ch);
      seq_     .push_back(data1[iHits].se);
      dtime_   .push_back(data1[iHits].dt);
      dtimeInt_.push_back(data1[iHits].dtI);
      pcnt_    .push_back(data1[iHits].pc);
      tcnt_    .push_back(data1[iHits].tc);
      pwidth_  .push_back(data1[iHits].pw);
      iHits++;
      //cout<<dtimeInt_[i1]<<endl;
    }
    if(nHits_!=0) t->Fill();

    board_.clear();
    channel_.clear();
    seq_.clear();
    dtime_.clear();
    dtimeInt_.clear();
    pcnt_.clear();
    tcnt_.clear();
    pwidth_.clear();
    
  }


  //cout<<s1<<" "<<frame<<" "<<s2<<" "<<unixtime<<endl;

  // t->Fill();
  f->Write();
  f->Close();
  
}

void MuoCaConvertor(const char OPT=' '){
  char path_txts[180], path_filetxt[180];
  sprintf(path_txts,"%sDataTxtNameAna.dat",DirOperate);
  ifstream intxtfile(path_txts);

  int Fnum=0;
  for(; intxtfile>>path_filetxt;Fnum++){
    char path_fileroot[180], name_fileroot[180];
    for(int ic = 0 ; ic<180 ; ic++){
      if(path_filetxt[ic]-DirRawData[ic]!=0){
        for(int icst = ic ; icst<180 ; icst++){
          if(path_filetxt[icst]=='.'){
            //cout<<"pos "<<ic<<" in string"<<path_filetxt<<endl;
            for(int icrt = ic;icrt<180;icrt++){
              if(icrt<icst) name_fileroot[icrt-ic] = path_filetxt[icrt];
              if(icrt>=icst) name_fileroot[icrt-ic] = 0;
              // cout<<path_filetxt[icrt];
            }
            // cout<<"string: root path:  "<<path_fileroot<<endl;
            sprintf(path_fileroot,"%s%s.root",DirRotfile,name_fileroot);
            Long64_t SizeOfTxtFile = FileSize(path_fileroot);

            if( SizeOfTxtFile > 20971520 && (OPT!='r'||OPT!='R') ){
              //file size > 20 MB && not on "Rewrite" mode
              cout<<"Find the Target .root: "<<path_filetxt<<endl;
              break;
            }
            // cout<<"string: root path:  "<<path_fileroot<<endl;
            cout<<"Converting : "<<path_filetxt<<endl;
            CaConvertor(path_filetxt,path_fileroot);
            cout<<"root file  : "<<path_fileroot<<endl;
            break;
          }
        }
        
        break;
      }
      
    }
    // cout<<name[Fnum]<<endl;
    
  }
  
  // sprintf(path_target,"%s%s",DirRotfile,NameChara);
  // cout<<"finding raw .root:  "<<NameChara<<" file under dir:  "<<DirRotfile<<endl;
  // cout<<Fnum<<endl;
  // char name_Save[230][30];
  // char name_Read[230][30];
  // gROOT->LoadMacro("CaConvertor.C+");
  
  // for(int i= 0; i<Fnum; i++){
    // sprintf(name_Read[i], "%s%s.txt",path,name[i]);
    // sprintf(name_Save[i], "%s%s.root",spath,name[i]);
    // cout<<"Read txt file:  "<<name_Read[i]<<endl;
    // 
    // cout<<"Save root file: "<<name_Save[i]<<endl;
  // }
}