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

void CaConvertorV2(
  const char vRootFiles[200]= "/data4/AndyLu/to_cw/4_4_4_starightbeam_merge.root",
  const char ofilePath [200]= "/home/yusiang/G4/4_4_4HorizontaBeamTest/Data/",
  const char ofileName [200]= "4_4_4HorizontalBeamTest.root"
) {
  
  char version[8] = {};
  DetVerCheck(version,detVer);
  cout<<"Now, detector Ver is:  "<<version<<endl;
  int detVerID = 0;
  if(strcmp(version,"V03_00")==0) detVerID = 0;
  else if(strcmp(version,"V03_05")==0) detVerID = 1;
  system(Form("mkdir -p %s",ofilePath));
  
  int eventGap = G4eventGap;
  ifstream infile;

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
  
  vector<Int_t>      gid_;
  vector<Int_t>      board_;
  vector<Int_t>      channel_;
  vector<Int_t>      seq_;
  vector<Int_t>      pcnt_;
  vector<Long64_t>   tcnt_;
  vector<double>     dtime_;
  vector<Long64_t>   dtimeInt_;
  vector<Int_t>      pwidth_;

  const char *SavePathName = Form("%s%s",DirRotfile,ofileName);

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
    pwidth   = data.GetDouble("Edep")*G4EdeptoPWit;
    
    board   = int(gid/NumCh)+1;
    int chtmp = (int(gid%NumCh)/4+(int(gid%NumCh)%4)*4);
    channel = chtmp;

    int      pcnt = (frame/G4FramIn1Sec);
    Long64_t tcnt = (rand()%eventGap);
    Long64_t dtimeInt = pcnt*2.56E+9+tcnt;
    double   dtime = pcnt*1.+(1.*tcnt)/2.56E+9;
    int      seq = 1;//MC default == 1
    nHits_++;
    
    unixtime_ = G4TheFirstUT+pcnt;
    if(ev%1000 == 0) cout<<Form("\r%.5f%%\t%d",(ev*100.)/(1.*evs),unixtime_)<<flush;
    
    Tmpdata dataT = {gid,board,channel,seq,dtime,dtimeInt,pcnt,tcnt,pwidth};
    data1.push_back(dataT);
    
    if(frame!=frame0){
      Timdata Tim_tmp = {frame,nHits_,unixtime_};
      time1.push_back(Tim_tmp);
      if(time1.size()>1){
        time1[time1.size()-2].nH = nHits_;
      }
      nHits_=0;
      frame0 = frame;  
    }
  }
  
  time1[time1.size()-1].nH = nHits_;
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
      gid_     .push_back(data1[iHits].gd);
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

    gid_.clear();
    board_.clear();
    channel_.clear();
    seq_.clear();
    dtime_.clear();
    dtimeInt_.clear();
    pcnt_.clear();
    tcnt_.clear();
    pwidth_.clear();
    
  }

  f->Write();
  f->Close();
  
}


void G4CaConvertorV2(
  const char vRootFiles[200]= "/data4/AndyLu/to_cw/4_4_4_starightbeam_merge.root",
  const char ofilePath [200]= "/home/yusiang/G4/4_4_4HorizontaBeamTest/Data/",
  const char ofileName [200]= "4_4_4HorizontalBeamTest.root"
) {
  
  const char *SavePathName = Form("%s%s",ofilePath,ofileName);

  Long64_t SizeOfRootFile = FileSize(SavePathName);
  if( SizeOfRootFile > 25000 ){
    cout<<"Find the Target .root: "<<SavePathName<<endl;
  }else{
    CaConvertorV2( vRootFiles , ofilePath , ofileName);
    cout<<"Finish convert the G4 raw data:  "<<ofilePath<<endl;
    cout<<"The G4 MonteCarlo data save as:  "<<ofilePath<<ofileName<<endl;
  }
  
}


void CaConvertor(
  const char vRootFiles[200]= "/data4/AndyLu/to_cw/4_4_4_starightbeam_merge.root",
  const char ofilePath [200]= "/home/yusiang/G4/4_4_4HorizontaBeamTest/Data/",
  const char ofileName [200]= "4_4_4HorizontalBeamTest.root"
) {
    cout<<"Error: Please use the last version CaConvertor: CaConvertorV2"<<endl;
  throw;
}
void G4CaConvertor(
  const char vRootFiles[200]= "/data4/AndyLu/to_cw/4_4_4_starightbeam_merge.root",
  const char ofilePath [200]= "/home/yusiang/G4/4_4_4HorizontaBeamTest/Data/",
  const char ofileName [200]= "4_4_4HorizontalBeamTest.root"
) {
  cout<<"Error: Please use the last version G4CaConvertor: G4CaConvertorV2"<<endl;
  throw;
}
