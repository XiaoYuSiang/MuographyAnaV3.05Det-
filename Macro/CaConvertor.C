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

bool pcntCheck(int board, Long64_t &pcntM, Long64_t &tcntM, Long64_t &tcnt, int &pcnt, int &bad1, int &bad2){
  if(board==MotherBoard||pcntM==0){
    tcntM = tcnt;
    pcntM = pcnt;
  }else if(pcnt<pcntM){
    pcnt = pcntM;
    if(tcnt>2.56E+9) {
      pcnt++;
      tcnt-=2.56E+9;
      //cout<<"CaConV.C: Auto adjust: pcnt lager than 2.56E+9."<<endl;
      bad1++;
    }else if(tcntM>2.E+9&&tcnt<1.E+9){
      pcntM++;
      pcnt = pcntM;
      //cout<<"CaConV.C: Auto adjust: pcnt carry."<<endl;
      bad2++;
    }
    
    //if(tcnt>2.56E+9) cout<<pcnt<<"/"<<pcntM<<" "<<Form("%.11f",dtime)<<"Warning : disconnect over 1 second = 2.56E+9 pcnt"<<"\n";
    //cout<<pcnt<<"/"<<pcntM<<" "<<Form("%.11f",dtime)<<"\n";
  }
}

void CaConvertor(
  TString filename="/data4/YuSiang/DaXi/20220907_Run1_0_Mu.txt", 
  TString ofile="/data4/YuSiang/DaXi/Ana2/rootfile/20220907_Run1_0_Mu.root",
  int FrameNumEsti = 10000
) {

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
  Long64_t tpcntM = 0,pcntM = 0,Indextmp=0, unixtime0 =0, pcntOff = 0;
  vector<struct Tmpdata> data1,data2;
  vector<struct Timdata> time1;
  int nHits1 = 0, nHits2 = 0;
  size_t data1Size = 0, time1Size = 0;
  int badline[4] = {0};
  while (std::getline(infile, line)) {

    std::string s1, s2;
    Int_t frame, frame0;
    Long64_t unixtime = 0;

    Int_t a=0, board=0, channel=0, seq=0, pcnt=0, pwidth=0;
    Long64_t tcnt,dtimeInt;
    double dtime;
    std::istringstream iss(line);
    size_t pos = line.find("Frame");
    
    if (pos != string::npos) { // the line containing "#Frame"

      if ((iss >> s1 >> frame >> s2 >> unixtime)) { 
        //if (frame_ >= 28588) continue;
        nHits1 = nHits2;
        
        Timdata Tim_tmp = {frame, 0 ,unixtime};
        time1.push_back(Tim_tmp);
        time1Size ++;
        if(time1.size()>1) time1[time1.size()-2].nH = nHits1;
        if(time1.size()>2){
          // cout<<time1[0].nH<<" "<<time1[1].nH<<" "<<time1[2].nH<<"\n";
          sort(data1.begin(), data1.end(), mycompare);
          frame_    = time1[0].fr;
          nHits_    = time1[0].nH;
          unixtime_ = time1[0].ut;
          unixTimeToHumanReadable(unixtime_,
              tYear_, tMonth_, tDate_, tHour_, tMinute_, tSecond_,timeZone);
          
          // cout<<time1Size<<" "<< time1.size()<<" "<< data1.size()<<" "<< data2.size() <<endl;
          // cout<<0<<"~"<<nHits_<<endl;
          for(int iHit=0;iHit<nHits_;iHit++){
            board_   .push_back(data1[iHit].bd );
            channel_ .push_back(data1[iHit].ch );
            seq_     .push_back(data1[iHit].se );
            dtime_   .push_back(data1[iHit].dt );
            dtimeInt_.push_back(data1[iHit].dtI);
            pcnt_    .push_back(data1[iHit].pc );
            tcnt_    .push_back(data1[iHit].tc );
            pwidth_  .push_back(data1[iHit].pw );
          }   
          // cout<<nHits_<<"~"<<nHits_+time1[1].nH<<endl;

          for(int iHit=nHits_;iHit<nHits_+time1[1].nH;iHit++){
            Tmpdata dataT = {
              data1[iHit].bd,data1[iHit].ch,data1[iHit].se,data1[iHit].dt,
              data1[iHit].dtI,data1[iHit].pc,data1[iHit].tc,data1[iHit].pw
            };
            data2.push_back(dataT);
          }
          time1.erase(time1.begin());
          data1 = data2;
          // cout<<time1Size<<" "<< time1.size()<<" "<< data1.size()<<" "<< data2.size() <<endl;
          // throw;
          if(nHits_!=0) t->Fill();
          // if(time1Size % (FrameNumEsti/100) == 0) 
          if(time1Size % 100 == 0) 
            cout<<"\r" <<frame_ <<"\t"<<(100.*time1Size)/(1.*FrameNumEsti)<<"%"<<flush;

          board_.clear();
          channel_.clear();
          seq_.clear();
          dtime_.clear();
          dtimeInt_.clear();
          pcnt_.clear();
          tcnt_.clear();
          pwidth_.clear();
          data2.clear();
        }
        nHits2 = 0;
      } else {
        //cout<<"Warning : a bad line!! "<<line<<endl;
        badline[0]++;
      }
      Indextmp = 0;
    } else {
      //if (frame_ >= 28588) continue;
      if ((iss >> a >> board >> channel >> seq >> dtime >> pcnt >> tcnt >> pwidth)) {
        if(Indextmp == 0){
          unixtime0 = unixtime;
          pcntOff = unixtime0 - pcnt;
          Indextmp++;
        }

        pcntCheck(board,pcntM,tpcntM,tcnt,pcnt,badline[1],badline[2]);
        dtime = pcnt*1.+(1.*tcnt)/2.56E+9;
        dtimeInt = pcnt*2.56E+9+tcnt;
        if(nHits2==0 && unixtime - pcnt != pcntOff){
          //cout<<"CaConV.C: Auto adjust: UT - pcnt != pcntOff.\t"<<unixtime<<"\t"<<pcnt<<"\t"<<pcntOff<<endl;
          badline[3]++;
          unixtime = pcnt + pcntOff;
        } 
        //cout<<pcnt<<" "<<tcnt<<" "<<dtimeInt<<" ";
        Tmpdata dataT = {board,channel,seq,dtime,dtimeInt,pcnt,tcnt,pwidth};
        data1.push_back(dataT);
        //cout<<dataT.dtI<<endl;
        nHits2++;
        data1Size++;
        //cout<<nHits_<<" "<<a<<" "<<board<<" "<<channel<<" "<<seq<<" "<<dtime<<" "<<pcnt<<" "<<tcnt<<" "<<pwidth<<endl;
      } else {
        badline[0]++;
        //cout<<"Warning : a bad line!! "<<line<<endl;
      }
      
    }
    
  }
  
  time1[1].nH = nHits2;
  sort(data1.begin(), data1.end(), mycompare);
  //cout<<431<<int(time1.size())<<endl;
  for(int i = 0 ; i<int(time1.size());i++){
    
    frame_    = time1[i].fr;
    nHits_    = time1[i].nH;
    unixtime_ = time1[i].ut;
    unixTimeToHumanReadable(unixtime_,
      tYear_, tMonth_, tDate_, tHour_, tMinute_, tSecond_,timeZone);    
    for(int iHit=time1[i].nH*i;iHit<time1[0].nH+time1[1].nH;iHit++){
      board_   .push_back(data1[iHit].bd );
      channel_ .push_back(data1[iHit].ch );
      seq_     .push_back(data1[iHit].se );
      dtime_   .push_back(data1[iHit].dt );
      dtimeInt_.push_back(data1[iHit].dtI);
      pcnt_    .push_back(data1[iHit].pc );
      tcnt_    .push_back(data1[iHit].tc );
      pwidth_  .push_back(data1[iHit].pw );
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
    data2.clear();
  }
  cout<<"\nBadLine: [linebad] : [pc>2.56E9] : [pcM>2E9&pc<1E9] : [UT-pc!=pcOff]"<<endl;
  cout<<"         "<<badline[0]<<":"<<badline[1]<<":"<<badline[2]<<":"<<badline[3]<<endl;
  cout<<"\nFinish reading and convert.\n";
  
  
  cout<<"Hits number of File:\t:  "<<data1Size;

  cout<<"\tNframe:  "<<time1Size<<endl;

  //cout<<time1[0].fr<<"\t"<<time1[0].nH<<"\t"<<time1[0].ut<<"\n";
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
            Long64_t SizeOfRootFile = FileSize(path_fileroot);
            if( SizeOfRootFile > 20971520 && (OPT!='r'||OPT!='R') ){
              //file size > 20 MB && not on "Rewrite" mode
              cout<<"Find the Target .root: "<<path_filetxt<<endl;
              break;
            }
            
            // cout<<"string: root path:  "<<path_fileroot<<endl;
            cout<<"Converting : "<<path_filetxt<<endl;
            Long64_t SizeOfTxtFile = FileSize(path_filetxt);
            Long64_t FrameNumEsti = ceil(SizeOfTxtFile / 6324.);
            cout<<"  File size: "<<SizeOfTxtFile<<" Bytes, estimate to be: "<< FrameNumEsti<<" Frames"<<endl;
            if(SizeOfTxtFile> 2147483648) {
              cout<<"Warning: File Larger than 2GB! use the ProCaConvertor()"<<endl;
            
            }
            cout<<"CaConvertor\\("<<path_filetxt<<","<<path_fileroot<<","<<FrameNumEsti<<"\\)"<<endl;
              CaConvertor(path_filetxt,path_fileroot,FrameNumEsti);
            //else CaConvertor(path_filetxt,path_fileroot,FrameNumEsti);
            cout<<"root file  : "<<path_fileroot<<endl;
            break;
          }
        }
        
        break;
      }
      
    }
    // cout<<name[Fnum]<<endl;
    
  }
  
}