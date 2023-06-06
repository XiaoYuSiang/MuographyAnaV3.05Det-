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
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "path_dir.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"

using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;

//channel vs rate combine
struct Tmpdata {
  Int_t     gd;
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

int G4CaConvertorV3(
  const char vRootFiles[200]= "/data4/AndyLu/to_cw/4_4_4_starightbeam_merge.root",
  const char ofilePath [200]= "/home/yusiang/G4/4_4_4HorizontaBeamTest/Data/",
  const char ofileName [200]= "4_4_4HorizontalBeamTest.root"
) {
  
  const char *SavePathName = Form("%s%s",ofilePath,ofileName);

  Long64_t SizeOfRootFile = FileSize(SavePathName);
  if( SizeOfRootFile > 25000 ){
    cout<<"Find the Target .root: "<<SavePathName<<endl;
    cout<<"Skip to convert: "<<SavePathName<<endl;
    return 1;
  } 
  
  
  char version[8] = {};
  DetVerCheck(version,detVer);
  cout<<"Now, detector Ver is:  "<<version<<endl;
  int detVerID = 0;
  if(strcmp(version,"V03_00")==0) detVerID = 0;
  else if(strcmp(version,"V03_05")==0) detVerID = 1;
  system(Form("mkdir -p %s",ofilePath));
  
  int eventGap = G4eventGap;
  ifstream infile;
  cout<<"G4.Conv.C opening: "<<vRootFiles<<endl;
  infile.open(vRootFiles);
  TreeReader data(vRootFiles,"mu_detector");

  Int_t            frame_= 0 ;
  Long64_t         unixtime_;
  Int_t            tYear_;
  Int_t            tMonth_;
  Int_t            tDate_;
  Int_t            tHour_;
  Int_t            tMinute_;
  Int_t            tSecond_;
  Int_t            nHits_;
  
  Int_t            gid;
  Int_t            board;
  Int_t            channel;
  Int_t            pwidth;
  
  double            hit_time;
  
  vector<Int_t>      gid_;
  vector<Int_t>      board_;
  vector<Int_t>      channel_;
  vector<Int_t>      seq_;
  vector<Int_t>      pcnt_;
  vector<Long64_t>   tcnt_;
  vector<double>     dtime_;
  vector<Long64_t>   dtimeInt_;
  vector<Int_t>      pwidth_;


  TFile *f = new TFile(SavePathName, "recreate");

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
  t->Branch("gid",      &gid_);
  t->Branch("board",    &board_);
  t->Branch("channel",  &channel_);
  t->Branch("seq",      &seq_);
  t->Branch("dtime",    &dtime_);
  t->Branch("dtimeInt", &dtimeInt_);
  t->Branch("pcnt",     &pcnt_);
  t->Branch("tcnt",     &tcnt_);
  t->Branch("pwidth",   &pwidth_);     

  nHits_ = 0;
  vector<struct Tmpdata> data1;
  vector<struct Timdata> time1;
  int evs = data.GetEntriesFast();
  Int_t frame=0, frame0=0;
  data.GetEntry(0);
  frame    = data.GetInt("eID");
  frame0 = frame;
  for (Long64_t ev = 0; ev < evs ; ++ev) {//@@@
    data.GetEntry(ev);
    
    frame    = data.GetInt("eID");
    gid      = G4GIDToGID[detVerID][int(data.GetDouble("channel"))];
    BCIDOfGID(gid, board, channel);
    board = BD[board];
    // cout<<gid<<"\t"<<board<<"\t"<<channel<<"\n";
    pwidth   = data.GetDouble("Edep")*G4EdeptoPWit;
    hit_time = int(data.GetDouble("hit_time")*2.56);
    int      pcnt = (frame/G4FramIn1Sec);
    Long64_t tcnt = Long64_t(2.56E+9*double(frame%G4FramIn1Sec)/double(G4FramIn1Sec)+hit_time);
    // cout<< hit_time<<"\t"<<frame%G4FramIn1Sec<<"\t"<<2.56E+9*double(frame%G4FramIn1Sec)<<"\t"<<2.56E+9*double(frame%G4FramIn1Sec)/double(G4FramIn1Sec)<<"\t"<<tcnt<<endl;
    Long64_t dtimeInt = pcnt*2.56E+9+tcnt;
    double   dtime = pcnt*1.+(1.*tcnt)/2.56E+9;
    int      seq = 1;//MC default == 1
    nHits_++;
    
    unixtime_ = G4TheFirstUT+pcnt;
    if(ev%1000 == 0) cout<<Form("\r%.5f%%\t%d",(ev*100.)/(1.*evs),unixtime_)<<flush;

    gid_     .push_back(gid);
    board_   .push_back(board);
    channel_ .push_back(channel);
    seq_     .push_back(seq);
    dtime_   .push_back(dtime);
    dtimeInt_.push_back(dtimeInt);
    pcnt_    .push_back(pcnt);
    tcnt_    .push_back(tcnt);
    pwidth_  .push_back(pwidth);
    
    if(frame!=frame0){
      frame_= frame;
      unixTimeToHumanReadable(unixtime_,
          tYear_, tMonth_, tDate_, tHour_, tMinute_, tSecond_,timeZone);

      if(nHits_!=0) t->Fill();
      gid_.clear();
      board_.clear();
      channel_.clear();
      seq_.clear();
      dtime_.clear();
      dtimeInt_.clear();
      pcnt_.clear();
      tcnt_.clear();
      pwidth_.clear();
      
      nHits_=0;
      frame0 = frame;  
    }
    
  }
  cout<<"File: "<<SavePathName<<" have been create!"<<endl;
  f->Write();
  // f->Close();
  
  cout<<"size=effEIDNum:  "<<evs;
  cout<<"Finish convert the G4 raw data:  "<<ofilePath<<endl;
  cout<<"The G4 MonteCarlo data save as:  "<<ofilePath<<ofileName<<endl;
  return 1;
}


// void CaConvertor(
  // const char vRootFiles[200]= "/data4/AndyLu/to_cw/4_4_4_starightbeam_merge.root",
  // const char ofilePath [200]= "/home/yusiang/G4/4_4_4HorizontaBeamTest/Data/",
  // const char ofileName [200]= "4_4_4HorizontalBeamTest.root"
// ) {
    // cout<<"Error: Please use the last version CaConvertor: CaConvertorV2"<<endl;
  // throw;
// }
// void G4CaConvertor(
  // const char vRootFiles[200]= "/data4/AndyLu/to_cw/4_4_4_starightbeam_merge.root",
  // const char ofilePath [200]= "/home/yusiang/G4/4_4_4HorizontaBeamTest/Data/",
  // const char ofileName [200]= "4_4_4HorizontalBeamTest.root"
// ) {
  // cout<<"Error: Please use the last version G4CaConvertor: G4CaConvertorV2"<<endl;
  // throw;
// }
