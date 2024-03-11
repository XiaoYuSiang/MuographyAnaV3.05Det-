#include <typeinfo>
#include <algorithm>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TText.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include "/data4/YuSiang/personalLib/Style/DBMTStyle.h"
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang01.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"
#include "DSLData.h"
#include "/data4/YuSiang/personalLib/RPU/DBMTThread/DBMTThread.h"

using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace DataConst;
vector<string>   vRootFiles;
vector<string>   vRotFileSepRun[MaxRun];
vector<Long64_t> vSizeSepRun;
vector<Long64_t> VNegUTRangeStart, VNegUTRangeFinal;
const string FNOEvNumForGap = Form("%sEvNumForGap.dat",DirOperate);
const string FNOEvDisplay   = Form("%sEvDisplay.dat",DirOperate);
const string FNOEvProblem   = Form("%sEvProblem.dat",DirOperate);

const int  CPUNum = cpuCores;
const int  ProcessNum = MaxRun;
// const int  ProcessNum = MaxRun;
const bool processShow   = false;
char RecreateOption = 'N';
int gapIndexNow;
int UTsfNow[2] = {0};
void EventAna(){
  cout<<"Please use the last version: EventAnaV4.C"<<endl;
}
void EventAnaV2(){
  cout<<"Please use the last version: EventAnaV4.C"<<endl;
}
void EventAnaV3(const int indexi=28, const int indexf=29, const int unixtimeini = unixtimei, const int unixtimefin = unixtimef, const char * runName="-NA-" ) {
  cout<<"Please use the last version: EventAnaV4.C"<<endl;
}

void SepRotDataByRun(){
  for(int irun=0;irun<MaxRun;irun++){
    vSizeSepRun.push_back(0);
    cout<<"Run: "<<irun<<endl;
    for(int ifile=0;ifile<int(vRootFiles.size());ifile++){
      if(int(vRootFiles[ifile].find(Form("Run%d_",irun)))!=-1
        ||int(vRootFiles[ifile].find(Form("Run_%d_",irun)))!=-1
      ){
        vRotFileSepRun[irun].push_back(vRootFiles[ifile]);
        vSizeSepRun[irun] += FileSize(vRootFiles[ifile].data());
        cout<<"  "<<vRootFiles[ifile]<<endl;
      }
    }
    cout<<"File size: "<<vSizeSepRun[irun]<<endl;
  }
}
  
  
void EventAnaV4(
  const int iCore, const int iProc, const int index = 28, 
  const char * runName="-NA-") 
{//140
  const int unixtimeini = RunUts[iProc];
  const int unixtimefin = RunUtf[iProc];
  // ths[iCore]->Sleep(0,500000); //throw;
  double eventGap = eventGapTcnt[index]*1.;
  cout<<"EVG:\t"<<eventGap<<endl;
  
  if(iProc>=RunNums||vSizeSepRun[iProc]==0){
    cout<<"Hint: Run: "<< iProc
      << " is empty, without any data, skip this run."<<endl;
    return;
  }
  string rtfN = Form("%sGapT%.0f_%s_nHTH%dL%dCRun%d.root",
                  DirOperate,eventGap,TimeStr,TriggerS,TriggerL,iProc);
  cout<<"Checking the size of file: "<<rtfN.data()<<endl;
  Long64_t size = FileSize(rtfN.data());
  
  if(RecreateOption!='R'&&RecreateOption!='r'){
    Long64_t RunWorkTime = RunUtf[iProc] - RunUts[iProc];
    if (size>RunWorkTime*TimeSizeRate*TriggerEventAnaSkip){
      cout<<"The "<<rtfN
        <<" is exist\nNot going to recreate the file, and skip this program\n";
      return;
    }
    cout<<"L91: size is too small "<<size<<"<= "<<RunWorkTime*TimeSizeRate*TriggerEventAnaSkip<<endl;

  }

  // ths[iProc]->Sleep(1);

  stringstream iss, iss1, issEvP;

  ths[iProc]->Lock();
  // cout<<"L105, Lock the threads to create TreeReader of Core: "<<iCore<<endl;

  TreeReader data(vRotFileSepRun[iProc]);
  int evs = data.GetEntriesFast();
  ths[iProc]->UnLock();

  cout<<"evs is :" << evs<<endl;
  //Data Variable
  cout<<Form("Root file not find, and save: %s",rtfN.data())<<endl;
  Int_t Index =0;
  TFile *rotfile = new TFile(rtfN.data(),"RECREATE");//@@ by DSL
  cout<<Form("Root Save: %s",rtfN.data())<<endl;
  Int_t      EvIndex=0;
  Int_t      frame_;
  Int_t      frameLast = 0;
  Long64_t   unixtime_;
  Int_t      tYear;
  Int_t      tMonth;
  Int_t      tDate;
  Int_t      tHour;
  Int_t      tMinute;
  Int_t      tSecond;

  //PS:2560000000 Hz
  Int_t      nLayers;
  Int_t      nHits ;//total number for hit in a frame/event
  Int_t      nH  ;//total number for hit in a frame/event
  Int_t      nHk[NumLY];
  Double_t*  dtime_;
  Long64_t*  dtimeInt_;
  Int_t*     channel_;
  Int_t*     board_;
  Int_t*     pwidth_;
  Int_t*     pcnt_;
  Long64_t*  tcnt_;
  vector<Double_t>  dtime;
  vector<Long64_t>  dtimeInt;
  vector<Int_t>     channel;
  vector<Int_t>     board;
  vector<Int_t>     pwidth;
  vector<Int_t>     pwidthScaleFactor;
  vector<Int_t>     pcnt;
  vector<Long64_t>  tcnt;
  vector<Double_t>  dtcnt;
  Double_t   dTimeOfEv ;
  
  ths[iProc]->Lock();
  // cout<<"L148, Lock the threads to create TTree and TH1Fs of Core: "<<iCore<<endl;
  TTree *tree = new TTree("t","data from analyzing file");
  tree->Branch("frame",&frameLast);
  tree->Branch("EvIndex",&EvIndex);
  tree->Branch("unixtime",&unixtime_);
  tree->Branch("tYear",&tYear);
  tree->Branch("tMonth",&tMonth);
  tree->Branch("tDate",&tDate);
  tree->Branch("tHour",&tHour);
  tree->Branch("tMinute",&tMinute);
  tree->Branch("tSecond",&tSecond);
  tree->Branch("nH",&nH);
  for(int i=0; i<NumLY; i++) tree->Branch(Form("nH%d",i),&(nHk[i]));
  
  tree->Branch("nLayers",&nLayers);
  tree->Branch("channel",&channel);
  tree->Branch("board",&board);
  tree->Branch("pwidth",&pwidth);
  tree->Branch("pwidthScaleFactor",&pwidthScaleFactor);
  tree->Branch("tcnt",&tcnt);
  tree->Branch("pcnt",&pcnt);
  tree->Branch("dtcnt",&dtcnt);
  tree->Branch("dtime",&dtime);
  tree->Branch("dtimeInt",&dtimeInt);
  tree->Branch("dTimeOfEv",&dTimeOfEv);
  

  
  TH1F *hboard       = new TH1F("hboard ","",NumBD,0,NumBD);
  TH1F *hBoard       = new TH1F("hBoard ","",NumBD,0,NumBD);
  TH1F *hChannel     = new TH1F("hChannel ","",NumCh,0,NumCh);
    // cout<<"L182, UnLock the threads to create TTree and TH1Fs of Core: "<<iCore<<endl;
  ths[iProc]->UnLock();
  Index =0;
  double dtime0 = 0;
  dTimeOfEv = 0;
  int nHit = 0;
  cout<<"Start at: "<< 0<<endl;
  data.GetEntry(0);
  frameLast   = data.GetInt("frame");

  for (Long64_t ev = 0; ev < evs ; ++ev) {
    data.GetEntry(ev);
    frame_   = data.GetInt("frame");
    unixtime_= data.GetLong64("unixtime");//fabs time of event [s]
    if(!MCMode){
      if(InNegHourRange(unixtime_,VNegUTRangeStart,VNegUTRangeFinal)) continue;
      if(unixtime_<unixtimeini) continue;
      if(unixtime_>unixtimefin){
        cout<<"End at: "<< ev<<endl;
        cout<<"unixtime_>unixtimefin "<<unixtime_<<"\t"<< unixtimefin<<endl;
        cout<<"frame_ "<<frame_<<endl;
        break;
      }
    }
    if(ev%1000 == 0) cout<<Form("\r%.5f%%\t%d\t%d\t%d",(ev*100.)/(1.*evs),unixtime_,frame_,nHits)<<flush;
    nHits    = data.GetInt("nHits");
    dtime_   = data.GetPtrDouble("dtime");
    dtimeInt_= data.GetPtrLong64("dtimeInt");
    board_   = data.GetPtrInt("board");
    channel_ = data.GetPtrInt("channel");
    pwidth_  = data.GetPtrInt("pwidth");
    pcnt_    = data.GetPtrInt("pcnt");
    tcnt_    = data.GetPtrLong64("tcnt");
    

    if (dtime0 == 0) dtime0 = dtimeInt_[0];

     

    for( int i=0 ; i<nHits; i++){
      if(GammaCutMode) if(pwidth_[i]==0) continue;
      if(board_[i]>=17) continue;
      dtcnt.push_back(abs(dtimeInt_[i]-dtime0));
      if(((!MCMode)&&(nHit!=0&&dtcnt[nHit]>eventGap))||(MCMode&&frameLast!=frame_)){
        dtcnt.pop_back();
        tYear   = data.GetInt("tYear"  );
        tMonth  = data.GetInt("tMonth" );
        tDate   = data.GetInt("tDate"  );
        tHour   = data.GetInt("tHour"  );
        tMinute = data.GetInt("tMinute");
        tSecond = data.GetInt("tSecond");
        nH      = nHit;//total number for hit in a frame/event
        int NPartCut = NumBD/NumLY;
        int VnH[NumLY] = {0};
        for(int iPartCut = 0 ; iPartCut < NPartCut ; iPartCut++){
          for(int iLay = 0 ; iLay < NumLY ; iLay++){
            VnH[iLay] +=hBoard->GetBinContent(BDCheck(BD[iLay*NPartCut+iPartCut])+1);//total number for hit in a frame/event
          }
        }
        int Tmp_nH = 0;
        for(int iLay = 0 ; iLay < NumLY ; iLay++){
          nHk[iLay] = VnH[iLay];
          Tmp_nH += nHk[iLay];  
        } 
        
        if(nH!=Tmp_nH){
          cout<<EvIndex<<"\t"<<nH;
          for(int iLay = 0 ; iLay < NumLY ; iLay++) cout<<"\t"<<nHk[iLay];
          cout<<endl;
        }
        
        
        nLayers=0 ;//total number for hit in a frame/event
        for(int i1=0; i1<NumLY;i1++){//@@
          if (VnH[i1]!=0) nLayers++;
        }
        if( ((!MCMode)&&(TriggerS < nHit&&nHit < TriggerL)) || (MCMode) ){
          if(/*unixtime_>1600000000*/true){
            dTimeOfEv = abs(dtimeInt_[i]-dtime0);
            tree->Fill();
            for(int i7=0;i7<nH;i7++){
              char gid[20];
              sprintf(gid,"%010.f%d%02.f",tcnt[i7]*1.,board[i7],channel[i7]*1.);
              // iss1<<Index<<"\t"<<gid<<"\t"<<tcnt[i7]<<"\t"<<board[i7]<<"\t"<<channel[i7]<<"\t"<<pwidth[i7]<<endl;
            }
            Index++;
            EvIndex = Index;
          // cout<<"\tIndex\t"<<Index<<"\tnH\tbool \t "<<nHit<<"\t"<<( TriggerS < nHit&&nHit < TriggerL)<<endl;
          }
        }
        //cout<<"dTimeOfEv\t"<<dTimeOfEv<<"\tnHit:\t"<<nHit<<endl;
        board.clear();
        channel.clear();
        pwidth.clear();
        pwidthScaleFactor.clear();
        tcnt.clear();
        pcnt.clear();
        dtime.clear();
        dtimeInt.clear();
        dtcnt.clear();
        //cout<<fabs(dtime0-dtime_[i])<<endl;
        // cout<<"dTimeOfEv"<<dTimeOfEv<<endl;
        //dTimeOfEv = dtime_[i];
        nHit = 0;
        hBoard->Reset();
        hChannel->Reset();
        
        dtcnt.push_back(0);
        
        frameLast = frame_;
        //cout<<"EvIndex:\t"<<EvIndex<<endl;
      }
      board.push_back(board_[i]);
      channel.push_back(channel_[i]);
      pwidth.push_back(pwidth_[i]);
      tcnt.push_back(tcnt_[i]);
      pcnt.push_back(pcnt_[i]);
      pwidthScaleFactor.push_back(BDPwWei[BDcheck(board_[i])]);
      if (BDcheck(board_[i])==-1||CHcheck(channel_[i])==-1){
        issEvP<<unixtime_<<"\t"<<board_[i]<<"\t"<<channel_[i]<<"\t"<<pcnt_[i]<<"\t"<<tcnt_[i]<<endl;
        // cout<<unixtime_<<"\t"<<board_[i]<<"\t"<<channel_[i]<<"\t"<<pcnt_[i]<<"\t"<<tcnt_[i]<<endl;
      }
      dtime.push_back(dtime_[i]);
      dtimeInt.push_back(dtimeInt_[i]);
      
      hBoard  ->Fill(BDcheck(board_[i]));
      hboard  ->Fill(BDcheck(board_[i]));
      // cout<<board_[i]<<endl;
      hChannel->Fill(channel_[i]);
      nHit++;
      dtime0 = dtimeInt_[i];
    }
  }
  rotfile->Write();
  ths[iCore]->Lock();
  ofstream out   (FNOEvNumForGap.data(), ofstream::out | ofstream::app );
  // ofstream out1  (FNOEvDisplay.data()  , ofstream::out | ofstream::app );
  ofstream outEvP(FNOEvProblem.data()  , ofstream::out | ofstream::app );
  cout<<"\n"<<"Finished Ntuple write"<<endl;
  cout<<"gap:total event:\t"<<eventGap<<"\t"<<Index<<endl;
  iss<<Form("%11f",eventGap*1.E+9)<<"\t"<<eventGap<<"\t"<<runName<<"\t"<<unixtimeini<<"\t"<<unixtimefin<<"\t"<<Index<<endl;
  
  
  out<<iss.str();
  // out1<<iss1.str();
  outEvP<<issEvP.str();
  ths[iCore]->UnLock();

}

void *process(void *arg){
  /*Don't change*/
  int iCore, iProc; SetProcStart(arg, iCore, iProc);
  // cout<<304<<"\t"<<iCore<<"\t"<<iProc<<endl;
  /*Adjust this funtion*/
  EventAnaV4(iCore, iProc, gapIndexNow, Form("Run%d",iProc));//, const char * runName="-NA-" ) {//140

  /*Don't change*/
  SetProcFinish(arg, iCore, iProc);
}


void EventAnaControl(
  const int indexi=28, const int indexf=29, const char OPT = 'N'
) {//140
  defstyle();
  vRootFiles = RawRootList(DirOperate);
  SepRotDataByRun();
  GetNegHourRange(VNegUTRangeStart, VNegUTRangeFinal, DirOperate);
  RecreateOption = OPT;
  system(Form("rm %s",FNOEvDisplay.data()));
  system(Form("rm %s",FNOEvProblem.data()));
  ofstream out   (FNOEvNumForGap.data(), ofstream::out | ofstream::app );
  // ofstream out1  (FNOEvDisplay.data()  , ofstream::out | ofstream::app );
  ofstream outEvP(FNOEvProblem.data());
  outEvP<<"#unixtime_\tbid\tcid\tpcnt\tcnt"<<endl;

  for(int i0=indexi;i0<indexf;i0++){
    gapIndexNow = i0;
    ContralThread();
  }
}
void EventGapAna() {
  string file = Form(FNOEvNumForGap.data());

  ifstream in(file.data());
  cout<<file<<endl;
  double evg  = 0;
  long   evgp = 0;
  int    num = 0;
  int    UTi = 0, UTf = 0;
  char   runName[50];
  TGraphErrors *gEGVN = new TGraphErrors();
  TGraphErrors *gEGVNSelect = new TGraphErrors();
  TH1F *hMod = new TH1F("hMod","",3000,0,3000);
  TCanvas *c1 = new TCanvas("c1","",1920,1080);
  c1->cd();
  c1->SetLogx();
  for(; in>>evgp>>evg>>runName>>UTi>>UTf>>num ;){
    cout<<evg<<"\t"<<evgp<<"\t"<<num<<"\n";
    gEGVN->SetPoint(gEGVN->GetN(), evg, num);
    gEGVN->SetPointError(gEGVN->GetN()-1, 1, sqrt(num));
    if(evg == 140){
      gEGVNSelect->SetPoint(gEGVNSelect->GetN(), evg, num);
      gEGVNSelect->SetPointError(gEGVNSelect->GetN()-1, 1, sqrt(num));
    }
  }
  hMod->Draw("");
  gEGVN->Draw("pe");
  gEGVNSelect->Draw("pe");
  gEGVN->SetMarkerStyle(8);
  gEGVN->SetMarkerColor(ColorArr9[0]);
  gEGVN->SetMarkerSize(2.5);
  gEGVNSelect->SetMarkerStyle(8);
  gEGVNSelect->SetMarkerColor(ColorArr9[1]);
  gEGVNSelect->SetMarkerSize(2.5);
  hMod->SetStats(0);
  hMod->GetXaxis()->SetTitleOffset(1.3);
  hMod->GetXaxis()->SetTitle("time window(tcnt)");
  hMod->GetYaxis()->SetTitle("Event Number");
  // hMod->GetYaxis()->SetRangeUser(0,num*1.2);
  
  TGaxis *AEGVN = new TGaxis(1,num*1.2, 3000, num*1.2,0.390625*1,0.390625*3000,510,"-G");
                              (-4.5,0.2,5.5,0.2,0.001,10000,510,"G");
  AEGVN->SetTitle("time window(ns)");
  AEGVN->SetLabelSize(0.025); 
  AEGVN->Draw("same");
  
  TLegend *LEGVN = new TLegend(0.6,0.3,0.8,0.50);
  LEGVN->AddEntry(gEGVN      ,Form("Data"),"pel");
  LEGVN->AddEntry(gEGVNSelect,Form("Select"),"pel");
  LEGVN->SetNColumns(2);
  LEGVN->Draw();
  
  
  c1->Print(Form("%sEvNumForGap.pdf",DirRes_TcntGap));

}
  