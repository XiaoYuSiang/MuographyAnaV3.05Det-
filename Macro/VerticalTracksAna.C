//include
#include <iostream>
#include <vector>
#include <fstream>
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
#include "/home/yusiang/personalLib/Style/DBMTStyle.h"
#include "/home/yusiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"
#include "DSLData.h"
#include "LoadODectTable.h"


using namespace std;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace MuoAna_path_dir;
using namespace DataConst;

bool BLayCheck(const int BLay, int &iHit){
  if((BLay==9||BLay==11||BLay==13||BLay==15 )
    ||(BLay==6||BLay==7||BLay==14 )){
    iHit = 2;
    switch (BLay) { 
      case 15:
        iHit = 4;
        break; 
      case 7: case 11: case 13: case 14:
        iHit = 3;
        break; 
      case 6: case 9:
        iHit = 2;
        break; 
    }
    return true;
  }
  return false;
}


void VerticalTracksAna(const int indexi=28, const int indexf=29 ) {
  cout<<"Please use VerticalTracksAnaV2"<<endl;
  throw;
  defstyle();
  double eventGap[46]={};
  
  ofstream out(Form("%sSEVertical4v3.dat",DirOperate));
  
  map<int, scintillator> MScints = LoadODectTable();
  
  for(int i0=indexi;i0<indexf;i0++){
    eventGap[i0] = (1.0*eventGapTcnt[i0])/2560000000.;
    char RFNStr[200];
    sprintf(RFNStr,"%d_%s_nHTH%dL%d.root",eventGapTcnt[i0],TimeStr,TriggerS,TriggerL);
    
    TreeReader data(Form("%sGapT%s",DirOperate,RFNStr));
    cout<<Form("%sGapT%s",DirOperate,RFNStr)<<" is opened"<<endl;
        
    TFile *rotfile = new TFile(Form("%sTracksGT%s",DirOperate,RFNStr),"RECREATE");
    cout<<Form("%sTracksGT%s",DirOperate,RFNStr)<<" is opened"<<endl;

    //new Tree unit and odl tree unit
    
    Int_t      TrackIndex=0;
    Int_t      frame_   ;
    Int_t      EvIndex_ ;
    Long64_t   unixtime_;
    Int_t      tYear_, tMonth_, tDate_, tHour_, tMinute_, tSecond_ ;
    Int_t      nLayers_ ;
    Int_t      iHit_;
    Int_t      eventHit_;
    Int_t      BLay_;
    Int_t*     board_;
    Int_t*     channel_;
    Int_t*     pwidth_;
    Int_t*     pwidthScaleFactor_;
    Int_t*     pcnt_;
    Long64_t*  tcnt_;
    Double_t*  dtime_;
    vector<Int_t>    board;
    vector<Int_t>    channel;
    vector<Int_t>    iX, iY, iZ;
    vector<Int_t>    pwidth;
    vector<Int_t>    pwidthScaleFactor;
    vector<Int_t>    pcnt;
    vector<Long64_t> tcnt;
    vector<Double_t> dtime;
    //old Tree uint
    Int_t      nH, nH0, nH1, nH2, nH3  ;//total number for hit in a frame/event
    
    TTree *tree = new TTree("t","data from analyzing file");
    tree->Branch("TrackIndex" ,&TrackIndex   );
    tree->Branch("frame"      ,&frame_       );
    tree->Branch("EvIndex"    ,&EvIndex_     );
    tree->Branch("unixtime"   ,&unixtime_    );
    tree->Branch("tYear"      ,&tYear_       );
    tree->Branch("tMonth"     ,&tMonth_      );
    tree->Branch("tDate"      ,&tDate_       );
    tree->Branch("tHour"      ,&tHour_       );
    tree->Branch("tMinute"    ,&tMinute_     );
    tree->Branch("tSecond"    ,&tSecond_     );
    tree->Branch("nLayers"    ,&nLayers_     );
    // tree->Branch("pcnt",&pcnt);
    tree->Branch("BLay"       ,&BLay_        );
    tree->Branch("board"      ,&board        );
    tree->Branch("channel"    ,&channel      );
    tree->Branch("eventHit"   ,&eventHit_    );
    tree->Branch("iHit"       ,&iHit_       );
    tree->Branch("iX"         ,&iX          );
    tree->Branch("iY"         ,&iY          );
    tree->Branch("iZ"         ,&iZ          );
    tree->Branch("pwidth"     ,&pwidth       );
    tree->Branch("pwidthScaleFactor",&pwidthScaleFactor);
    tree->Branch("pcnt"       ,&pcnt        );
    tree->Branch("tcnt"       ,&tcnt        );
    tree->Branch("dtime"      ,&dtime       );
    
    //take time and set anlyze Constant for boundry condition
    Long64_t evsVT = data.GetEntriesFast();
    cout <<"total event:\t"<<evsVT<<endl;
    data.GetEntry(0);

    //data hist create
    TH1F *hMod         = new TH1F("hMod","",ndivise,FirDaySec,FinDaySec);
    TH1F *hModG        = new TH1F("hModG","",ndivise,FirDaySec,FinDaySec);
    TH1F *hN1XX4       = new TH1F("B=1XX4","",16,0,16);
    TH1F *hNX35X       = new TH1F("B=X35X","",16,0,16);
    TH1F *hN13X4       = new TH1F("B=13X4","",16,0,16);
    TH1F *hN1X54       = new TH1F("B=1X54","",16,0,16);
    TH1F *hNX354       = new TH1F("B=X354","",16,0,16);
    TH1F *hN135X       = new TH1F("B=135X","",16,0,16);
    TH1F *hN1354       = new TH1F("B=hN1354","",16,0,16);
    TH1F *hN1354One    = new TH1F("B=hN1354One","",16,0,16);
    TH2F *hNHit2D      = new TH2F("hNHit2D","",16,0,16,5,1,6);
    for(int i4=0;i4<16;i4++){
      hN1354One->Fill(i4);
    }
    int ct3=0 ,ct4 = 0;
    int N3 = 0, N2 = 0;
    //Fill the data
    for (Long64_t ev = 0; ev <evsVT; ++ev) {//evsVT; ++ev) {
      data.GetEntry(ev);
      frame_    = data.GetInt("frame");
      EvIndex_  = data.GetInt("EvIndex");
      unixtime_ = data.GetLong64("unixtime");
      if(ev%1000 == 0) cout<<Form("\r%.5f%%\t%d",(ev*100.)/(1.*evsVT),unixtime_)<<flush;
      tYear_    = data.GetInt("tYear");
      tMonth_   = data.GetInt("tMonth");
      tDate_    = data.GetInt("tDate");
      tHour_    = data.GetInt("tHour");
      tMinute_  = data.GetInt("tMinute");
      tSecond_  = data.GetInt("tSecond");
      nLayers_  = data.GetInt("nLayers");
      nH        = data.GetInt("nH");
      eventHit_ = nH;
      board_    = data.GetPtrInt("board");
      channel_  = data.GetPtrInt("channel");
      pwidth_   = data.GetPtrInt("pwidth");
      pwidthScaleFactor_   = data.GetPtrInt("pwidthScaleFactor");
      pcnt_     = data.GetPtrInt("pcnt");
      tcnt_     = data.GetPtrLong64("tcnt");
      dtime_    = data.GetPtrDouble("dtime");
      
      if(nH<2 ) continue;
      if(nH>62) continue;//@@
      Long64_t BoolnH = 0;
      short BoolnB = 0;
      for(int i2=0 ; i2<nH ; i2++){
        if ((BoolnH>>i2)&1) continue;
        int hitCHA = channel_[i2];
        int hitBDA = board_[i2];
        double hitdTC = dtime_[i2];
        int GID = BCIDCovGID(board_[i2], channel_[i2]);
        int BID = BDcheck(board_[i2]);
        board  .push_back(board_[i2]);
        channel.push_back(channel_[i2]);
        iX    .push_back(MScints[GID].iX);
        iY    .push_back(MScints[GID].iY);
        iZ    .push_back(MScints[GID].iZ);
        pwidth.push_back(pwidth_[i2]);
        pwidthScaleFactor.push_back(pwidthScaleFactor_[i2]);
        pcnt  .push_back(pcnt_[i2]);
        tcnt  .push_back(tcnt_[i2]);
        dtime .push_back(dtime_[i2]);
        BoolnH |= 1UL << i2;
        BoolnB |= 1UL << BID;
        for(int i3 = i2+1 ; i3<nH ; i3++){
          
          GID = BCIDCovGID(board_[i3], channel_[i3]);
          BID = BDcheck(board_[i3]);
          if ((BoolnH>>i3)&1) continue;
          if ((BoolnB>>BID)&1) continue;
          if ( channel_[i3]==hitCHA ){
            BoolnH |= 1UL << i3;
            BoolnB |= 1UL << BID;
            board   .push_back(board_[i3]);  
            channel .push_back(channel_[i3]);
            iX     .push_back(MScints[GID].iX);
            iY     .push_back(MScints[GID].iY);
            iZ     .push_back(MScints[GID].iZ);
            pcnt   .push_back(pcnt_[i3]);
            tcnt   .push_back(tcnt_[i3]);
            dtime  .push_back(dtime_[i3]);
            pwidth .push_back(pwidth_[i3]);
            pwidthScaleFactor.push_back(pwidthScaleFactor_[i3]);
            BLay_ = BoolnB;
          }
        }
        //for(int i2=0 ; i2<16 ; i2++){
        if((BLay_==9||BLay_==11||BLay_==13||BLay_==15 )
          ||(BLay_==6||BLay_==7||BLay_==14 )){
          TrackIndex ++;
          iHit_=2;
          hN1XX4->Fill(hitCHA);
          if(BLay_==15){
            hN1354->Fill(hitCHA);
            hN1X54->Fill(hitCHA);
            hN13X4->Fill(hitCHA);
            hN1XX4->Fill(hitCHA);
            hNX354->Fill(hitCHA);
            hN135X->Fill(hitCHA);
            hNX35X->Fill(hitCHA);
            iHit_=4;
          }else if(BLay_==13){
            hN13X4->Fill(hitCHA);
            hN1XX4->Fill(hitCHA);
            iHit_=3;
          }else if(BLay_==11){
            hN1X54->Fill(hitCHA);
            hN1XX4->Fill(hitCHA);
            iHit_=3;
          }else if(BLay_==7){
            hNX354->Fill(hitCHA);
            iHit_=3;
          }else if(BLay_==14){
            hN135X->Fill(hitCHA);
            iHit_=3;
          }else if(BLay_==6){
            hNX35X->Fill(hitCHA);
            iHit_=2;
          }else if(BLay_==9){
            hN1XX4->Fill(hitCHA);
            iHit_=2;
          }




          if(iHit_!=2&&false){//@@

            for(int i6=0;i6<iHit_;i6++) cout<<iZ[i6];
            cout<<"\n";
          }
          //cout<<"\n\nthe throw\n\n";
          //throw;
          
          tree->Fill();
          //cout<<"aaaaa"<<endl;
        }
        //}
        BoolnB =0;
        BLay_   =0;
        board.clear();
        channel.clear();
        pwidth.clear();
        pwidthScaleFactor.clear();
        pcnt.clear();
        iX.clear();
        iY.clear();
        iZ.clear();
        tcnt.clear();
        dtime.clear();
        iHit_ = 0;

      }
    }

    rotfile->Write();
    cout<<"\r"<<"Finished Ntuple write"<<endl;
    TCanvas *c1 = new TCanvas("c1","",1800,1200);
    c1->Divide(3,2);
    
    c1->cd(1);
    hN13X4->Draw("box");
    hN1354->Draw("boxsame");
    c1->cd(2);
    hN1X54->Draw("box");
    hN1354->Draw("boxsame");
    c1->cd(4);
    hN135X->Draw("box");
    hN1354->Draw("boxsame");
    c1->cd(5);
    hNX354->Draw("box");
    hN1354->Draw("boxsame");
    TH1F *hNX[7]={
      hN13X4,hN1X54,hN135X,hNX354
    };
    hN13X4->SetLineColor(1);
    hN1X54->SetLineColor(2);
    hN135X->SetLineColor(1);
    hNX354->SetLineColor(2);
    hN1354->SetLineColor(3);
    hN1354One->SetLineColor(1);
    hN1354One->SetLineStyle(2);
    hN1354One->GetXaxis()->SetTitle("Channel");
    hN1354One->GetYaxis()->SetTitle("Effciency");
    hN1354One->GetYaxis()->SetRangeUser(0,1.2);
    for(int i=0;i<4;i++){
      hNX[i]->GetXaxis()->SetTitle("Channel");
      hNX[i]->GetYaxis()->SetTitle("Number of Event");
      hNX[i]->GetYaxis()->SetRangeUser(0,700);
    }
    TH1F *hNEff3 = (TH1F*) hN1354->Clone();
    hNEff3 ->SetTitle("Efficiency: MTB3");
    hNEff3 ->Divide(hN1X54);
    TH1F *hNEff5 = (TH1F*) hN1354->Clone();
    hNEff5 ->SetTitle("Efficiency: MTB5");
    hNEff5 ->Divide(hN13X4);
    TH1F *hNEff4 = (TH1F*) hN1354->Clone();
    hNEff4 ->SetTitle("Efficiency: MTB3");
    hNEff4 ->Divide(hNX354);
    TH1F *hNEff1 = (TH1F*) hN1354->Clone();
    hNEff1 ->SetTitle("Efficiency: MTB5");
    hNEff1 ->Divide(hN135X);
    
    hNEff3 ->SetStats(0);
    hNEff5 ->SetStats(0);
    hNEff4 ->SetStats(0);
    hNEff1 ->SetStats(0);
    hNEff3->SetLineColor(1);
    hNEff5->SetLineColor(2);
    hNEff4->SetLineColor(1);
    hNEff1->SetLineColor(2);
    c1->cd(3);
    hN1354One->Draw("box");
    hNEff3->Draw("boxsame");
    hNEff5->Draw("boxsame");
    c1->cd(6);
    hN1354One->Draw("box");
    hNEff4->Draw("boxsame");
    hNEff1->Draw("boxsame");
    
    c1->Print(Form("%sBoardChannelEffective/EFF_2Track%d.png",DirResult,eventGapTcnt[i0]));
    c1->Print(Form("%sBoardChannelEffective/EFF_2Track%d.pdf",DirResult,eventGapTcnt[i0]));
    int num1XX4=0,num13X4=0,num1X54=0,num1354=0;  
    int numX35X=0,num135X=0,numX354=0;  
    for(int i2=0;i2<16;i2++){
      num1XX4+=hN1XX4->GetBinContent(i2+1);
      num13X4+=hN13X4->GetBinContent(i2+1);
      num1X54+=hN1X54->GetBinContent(i2+1);
      num1354+=hN1354->GetBinContent(i2+1);
      numX35X+=hNX35X->GetBinContent(i2+1);
      numX354+=hNX354->GetBinContent(i2+1);
      num135X+=hN135X->GetBinContent(i2+1);
    }
    cout<<eventGapTcnt[i0]<<"\t";
    cout<<num1XX4<<"\t";
    cout<<num13X4<<"\t";
    cout<<num1X54<<"\t";
    cout<<num1354<<"\t";
    cout<<numX35X<<"\t";
    cout<<numX354<<"\t";
    cout<<num135X<<"\n";
    
    rotfile->Close();
  

  }
}

/*
void SetTracksTree(TTree *tree){
  
  
}
*/

void rootFileSave(const char *RFNStr,const bool testMode){
  
  map<int, scintillator> MScints = LoadODectTable();
  TreeReader data(Form("%sGapT%s",DirOperate,RFNStr));
  cout<<Form("%sGapT%s",DirOperate,RFNStr)<<" is opened"<<endl;
  
  TFile *rotfile = new TFile(Form("%sTracksGT%s",DirOperate,RFNStr),"RECREATE");
  cout<<Form("%sTracksGT%s",DirOperate,RFNStr)<<" is created"<<endl;
  
  //new Tree unit and odl tree unit
  
  Int_t      TrackIndex=0;
  Int_t      frame_   ;
  Int_t      EvIndex_ ;
  Long64_t   unixtime_;
  Int_t      tYear_, tMonth_, tDate_, tHour_, tMinute_, tSecond_ ;
  Int_t      nLayers_ ;
  Int_t      iHit_;
  Int_t      eventHit_;
  Int_t      BLay_;
  Int_t*     board_;
  Int_t*     channel_;
  Int_t*     pwidth_;
  Int_t*     pwidthScaleFactor_;
  Int_t*     pcnt_;
  Long64_t*  tcnt_;
  Double_t*  dtime_;
  vector<Int_t>    board;
  vector<Int_t>    channel;
  vector<Int_t>    iX , iY , iZ ;
  vector<Int_t>    BiX, BiY, BiZ;
  vector<Int_t>    pwidth;
  vector<Int_t>    pwidthScaleFactor;
  vector<Int_t>    pcnt;
  vector<Long64_t> tcnt;
  vector<Double_t> dtime;
  //old Tree uint
  Int_t      nH, nH0, nH1, nH2, nH3  ;//total number for hit in a frame/event
  
  TTree *tree = new TTree("t","data from analyzing file");
  tree->Branch("TrackIndex" ,&TrackIndex   );
  tree->Branch("frame"      ,&frame_       );
  tree->Branch("EvIndex"    ,&EvIndex_     );
  tree->Branch("unixtime"   ,&unixtime_    );
  tree->Branch("tYear"      ,&tYear_       );
  tree->Branch("tMonth"     ,&tMonth_      );
  tree->Branch("tDate"      ,&tDate_       );
  tree->Branch("tHour"      ,&tHour_       );
  tree->Branch("tMinute"    ,&tMinute_     );
  tree->Branch("tSecond"    ,&tSecond_     );
  tree->Branch("nLayers"    ,&nLayers_     );
  // tree->Branch("pcnt",&pcnt);
  tree->Branch("BLay"       ,&BLay_        );
  tree->Branch("board"      ,&board        );
  tree->Branch("channel"    ,&channel      );
  tree->Branch("eventHit"   ,&eventHit_    );
  tree->Branch("iHit"       ,&iHit_       );
  tree->Branch("iX"         ,&iX          );
  tree->Branch("iY"         ,&iY          );
  tree->Branch("iZ"         ,&iZ          );
  tree->Branch("BiX"         ,&BiX          );
  tree->Branch("BiY"         ,&BiY          );
  tree->Branch("BiZ"         ,&BiZ          );
  tree->Branch("pwidth"     ,&pwidth      );
  tree->Branch("pwidthScaleFactor",&pwidthScaleFactor);
  tree->Branch("pcnt"       ,&pcnt        );
  tree->Branch("tcnt"       ,&tcnt        );
  tree->Branch("dtime"      ,&dtime       );
  
  //take time and set anlyze Constant for boundry condition
  Long64_t evsVT = data.GetEntriesFast();
  cout <<"total event:\t"<<evsVT<<endl;
  data.GetEntry(0);

  //data hist create
  

  if(testMode) evsVT *= 0.01;
  //Fill the data
  for (Long64_t ev = 0; ev <evsVT*0.1; ++ev) {//evsVT; ++ev) {
    data.GetEntry(ev);
    frame_    = data.GetInt("frame");
    EvIndex_  = data.GetInt("EvIndex");
    unixtime_ = data.GetLong64("unixtime");
    if(ev%1000 == 0) cout<<Form("\r%.5f%%\t%d",(ev*100.)/(1.*evsVT),unixtime_)<<flush;
    tYear_    = data.GetInt("tYear");
    tMonth_   = data.GetInt("tMonth");
    tDate_    = data.GetInt("tDate");
    tHour_    = data.GetInt("tHour");
    tMinute_  = data.GetInt("tMinute");
    tSecond_  = data.GetInt("tSecond");
    nLayers_  = data.GetInt("nLayers");
    nH        = data.GetInt("nH");
    eventHit_ = nH;
    board_    = data.GetPtrInt("board");
    channel_  = data.GetPtrInt("channel");
    pwidth_   = data.GetPtrInt("pwidth");
    pwidthScaleFactor_   = data.GetPtrInt("pwidthScaleFactor");
    pcnt_     = data.GetPtrInt("pcnt");
    tcnt_     = data.GetPtrLong64("tcnt");
    dtime_    = data.GetPtrDouble("dtime");
    
    if(nH<2 ) continue;
    if(nH>62) continue;//@@
    Long64_t BoolnH = 0;
    short BoolnL = 0;
    for(int i2=0 ; i2<nH ; i2++){
      if ((BoolnH>>i2)&1) continue;
      int hitCHA = channel_[i2];
      int hitBDA = board_[i2];
      double hitdTC = dtime_[i2];
      int GID = BCIDCovGID(board_[i2], channel_[i2]);
      int LID = MScints[GID].iZ;
      board  .push_back(board_[i2]);
      channel.push_back(channel_[i2]);
      iX    .push_back(MScints[GID].iX);
      iY    .push_back(MScints[GID].iY);
      iZ    .push_back(LID);
      BiX    .push_back(MScints[GID].BiX);
      BiY    .push_back(MScints[GID].BiY);
      BiZ    .push_back(MScints[GID].BiZ);
      pwidth.push_back(pwidth_[i2]);
      pwidthScaleFactor.push_back(pwidthScaleFactor_[i2]);
      pcnt  .push_back(pcnt_[i2]);
      tcnt  .push_back(tcnt_[i2]);
      dtime .push_back(dtime_[i2]);
      BoolnH |= 1UL << i2;
      BoolnL |= 1UL << LID;
      for(int i3 = i2+1 ; i3<nH ; i3++){
        GID = BCIDCovGID(board_[i3], channel_[i3]);
        LID = MScints[GID].iZ;
        if ((BoolnH>>i3)&1) continue;
        if ((BoolnL>>LID)&1) continue;
        if ( channel_[i3]==hitCHA ){
          BoolnH |= 1UL << i3;
          BoolnL |= 1UL << LID;
          board   .push_back(board_[i3]);  
          channel .push_back(channel_[i3]);
          iX     .push_back(MScints[GID].iX);
          iY     .push_back(MScints[GID].iY);
          iZ     .push_back(MScints[GID].iZ);
          BiX    .push_back(MScints[GID].BiX);
          BiY    .push_back(MScints[GID].BiY);
          BiZ    .push_back(MScints[GID].BiZ);
          // cout<<"iZ:GZ\t"<<iZ[iZ.size()-1]<<"\t"<<MScints[GID].iZ<<endl;
          pcnt   .push_back(pcnt_[i3]);
          tcnt   .push_back(tcnt_[i3]);
          dtime  .push_back(dtime_[i3]);
          pwidth .push_back(pwidth_[i3]);
          pwidthScaleFactor.push_back(pwidthScaleFactor_[i3]);
          BLay_ = BoolnL;
        }
      }

      if( BLayCheck(BLay_, iHit_ ) ){//return BLay and iHit

        TrackIndex ++;
        tree->Fill();
      }
      BoolnL =0;
      BLay_   =0;
      board.clear();
      channel.clear();
      pwidth.clear();
      pwidthScaleFactor.clear();
      pcnt.clear();
      iX.clear();
      iY.clear();
      iZ.clear();
      BiX.clear();
      BiY.clear();
      BiZ.clear();
      tcnt.clear();
      dtime.clear();
      iHit_ = 0;

    }
  }

  rotfile->Write();
  cout<<"\r"<<"Finished Ntuple write"<<endl;
  rotfile->Close();
  
}

void VerticalTracksAnaV2( const bool*OperMode, const int indexi=28, const int indexf=29 ) {
  bool testMode     = OperMode[0];
  bool rootFileMode = OperMode[4];
  defstyle();
  double eventGap[46]={};
  
  ofstream out(Form("%sSEVertical4v3.dat",DirOperate));
  
  
  for(int i0=indexi;i0<indexf;i0++){
    eventGap[i0] = (1.0*eventGapTcnt[i0])/2560000000.;
    char RFNStr[200];
    sprintf(RFNStr,"%d_%s_nHTH%dL%d.root",eventGapTcnt[i0],TimeStr,TriggerS,TriggerL);
    
    if(rootFileMode) rootFileSave( RFNStr, testMode);
    
    
    TFile *rotfile_0 = TFile::Open(Form("%sTracksGT%s",DirOperate,RFNStr));
    TTree *tf = (TTree*) rotfile_0 ->Get("t");
    cout<<"Opening the root file: "<<rotfile_0->GetName()<<endl;
    
    TH1F *hMod         = new TH1F("hMod"    ,"",ndivise,FirDaySec,FinDaySec);
    TH1F *hModG        = new TH1F("hModG"   ,"",ndivise,FirDaySec,FinDaySec);
    TH1F *hN0XX3       = new TH1F("hN0XX3"  ,"",NumChLys,0,NumChLys);
    TH1F *hNX12X       = new TH1F("hNX12X"  ,"",NumChLys,0,NumChLys);
    TH1F *hN01X3       = new TH1F("hN01X3"  ,"",NumChLys,0,NumChLys);
    TH1F *hN0X23       = new TH1F("hN0X23"  ,"",NumChLys,0,NumChLys);
    TH1F *hNX123       = new TH1F("hNX123"  ,"",NumChLys,0,NumChLys);
    TH1F *hN012X       = new TH1F("hN012X"  ,"",NumChLys,0,NumChLys);
    TH1F *hN0123       = new TH1F("hN0123"  ,"",NumChLys,0,NumChLys);
    TH1F *hEfEq1       = new TH1F("hEfEq1"  ,"",NumChLys,0,NumChLys);
    TH2F *hNHit2D      = new TH2F("hNHit2D" ,"",NumChLys,0,NumChLys,NumLY,0,NumLY);
    for(int ichlys = 0;ichlys<NumChLys;ichlys++){
      hEfEq1->Fill(ichlys);
    }
    
    TCanvas *ctmp = new TCanvas("ctmp","",100,100);
    ctmp->cd();
    char inputForm[30];
    sprintf(inputForm,"iX+iY*%d",NumnX*NumBX);
    tf->Draw(Form("%s>>hN0XX3",inputForm),"BLay==15||BLay==13||BLay==11||BLay==9","box");
    tf->Draw(Form("%s>>hNX12X",inputForm),"BLay==15||BLay==6"                    ,"box");
    tf->Draw(Form("%s>>hN01X3",inputForm),"BLay==15||BLay==13"                   ,"box");
    tf->Draw(Form("%s>>hN0X23",inputForm),"BLay==15||BLay==11"                   ,"box");
    tf->Draw(Form("%s>>hNX123",inputForm),"BLay==15||BLay==7"                    ,"box");
    tf->Draw(Form("%s>>hN012X",inputForm),"BLay==15||BLay==14"                   ,"box");
    tf->Draw(Form("%s>>hN0123",inputForm),"BLay==15"                             ,"box");
    ctmp->Close();
    
    TCanvas *c1 = new TCanvas("c1","",1800,1200);
    c1->Divide(3,2);
    
    c1->cd(1);
    hN01X3->Draw("box");
    hN0123->Draw("boxsame");
    c1->cd(2);
    hN0X23->Draw("box");
    hN0123->Draw("boxsame");
    c1->cd(4);
    hN012X->Draw("box");
    hN0123->Draw("boxsame");
    c1->cd(5);
    hNX123->Draw("box");
    hN0123->Draw("boxsame");
    TH1F *hNX[7]={
      hNX123,hN0X23,hN01X3,hN012X,
    };
    

    int NExtrem[2]={1000000000,0};
    for(int iL=0;iL<NumLY;iL++){

      if(NExtrem[0] > hNX[iL]->GetMinimum()) NExtrem[0] =hNX[iL]->GetMinimum();
      if(NExtrem[1] < hNX[iL]->GetMaximum()) NExtrem[1] =hNX[iL]->GetMaximum();
    }
    cout<<"NExtrem [0]:[1]\t"<<NExtrem[0]<<"\t"<<NExtrem[1]<<endl;
    
    hN0123->SetLineColor(3);
    hEfEq1->SetLineColor(1);
    hEfEq1->SetLineStyle(2);
    hEfEq1->GetXaxis()->SetTitle("Channel(iX + iY*NumnX*NumBX)");
    hEfEq1->GetXaxis()->CenterTitle(true);
    hEfEq1->GetYaxis()->SetTitle("Effciency");
    hEfEq1->GetYaxis()->SetRangeUser(0,1.2);
    hEfEq1->GetXaxis()->SetLabelSize(.035);
    hEfEq1->GetYaxis()->SetLabelSize(.035);
    hEfEq1->GetXaxis()->SetTitleOffset(1.6);
    hEfEq1->GetYaxis()->SetTitleOffset(1.6);
    hEfEq1->SetStats(0);
    
    
    for(int iL=0;iL<NumLY;iL++){
      hNX[iL]->GetXaxis()->SetTitle("Channel(iX + iY*NumnX*NumBX)");
      hNX[iL]->GetXaxis()->CenterTitle(true);
      hNX[iL]->GetYaxis()->SetTitle("Number of Event");
      hNX[iL]->GetXaxis()->SetLabelSize(.035);
      hNX[iL]->GetYaxis()->SetLabelSize(.035);
      hNX[iL]->GetXaxis()->SetTitleOffset(1.6);
      hNX[iL]->GetYaxis()->SetTitleOffset(1.6);
      hNX[iL]->GetYaxis()->SetRangeUser(NExtrem[0],NExtrem[1]*1.2);
      hNX[iL]->SetLineColor(2-iL/2);
    }
    
    // TH1F *hEffL0, *hEffL1, *hEffL2, *hEffL3;
    TH1F *hEff[7]={};
    for(int iL=0;iL<NumLY;iL++){
      hEff[iL] = (TH1F*) hN0123->Clone();
      hEff[iL]-> SetName(Form("hEffL%d",iL));
      hEff[iL]-> SetTitle(Form("Efficiency: Lay %d(iZ = %d)",iL,iL));
      hEff[iL]-> Divide(hNX[iL]);
      hEff[iL]->SetStats(0);
      hEff[iL]->SetLineColor(iL/2+1);
    }

    c1->cd(3);
    hEfEq1->Draw("box");
    hEff[1]->Draw("boxsame");
    hEff[2]->Draw("boxsame");
    c1->cd(6);
    hEfEq1->Draw("box");
    hEff[0]->Draw("boxsame");
    hEff[3]->Draw("boxsame");
    
    DrawPdfPng(c1, Form("%sEFF_2TrackGT%d_%s",DirRes_ChEff,eventGapTcnt[i0],TimeStr));

    int num0XX3=0,num01X3=0,num0X23=0,num0123=0;  
    int numX12X=0,numX123=0,num012X=0;  
    for(int ichlys = 1;ichlys<=NumChLys;ichlys++){
      num0XX3+=hN0XX3->GetBinContent(ichlys);
      num01X3+=hN01X3->GetBinContent(ichlys);
      num0X23+=hN0X23->GetBinContent(ichlys);
      num0123+=hN0123->GetBinContent(ichlys);
      numX12X+=hNX12X->GetBinContent(ichlys);
      numX123+=hNX123->GetBinContent(ichlys);
      num012X+=hN012X->GetBinContent(ichlys);
    }
    cout<<eventGapTcnt[i0]<<"\t";
    cout<<num0XX3<<"\t";
    cout<<num01X3<<"\t";
    cout<<num0X23<<"\t";
    cout<<num0123<<"\t";
    cout<<numX12X<<"\t";
    cout<<numX123<<"\t";
    cout<<num012X<<"\n";
    
    
    char seCondition[50] = "BLay==15||BLay==13||BLay==11||BLay==9";
    
    TH1F *hETExtrem = new TH1F("hETExtrem","",NumChLys,0,NumChLys);
    int ETExtrem[2]={1000000000,0};
    for(int iL=0;iL<NumLY;iL++){
      tf->Draw(Form("%s>>hETExtrem",inputForm),Form("%s&&iZ==%d",seCondition,iL),"box");
      if(ETExtrem[0] > hETExtrem->GetMinimum()) ETExtrem[0] =hETExtrem->GetMinimum();
      if(ETExtrem[1] < hETExtrem->GetMaximum()) ETExtrem[1] =hETExtrem->GetMaximum();
    }
    cout<<"ETExtrem [0]:[1]\t"<<ETExtrem[0]<<"\t"<<ETExtrem[1]<<endl;
    
    
    TCanvas *cTrackLy = new TCanvas("cTrackLy","",360*NumLY,360);
    cTrackLy->Divide(NumLY,1);
    TH2F *hETrack[NumLY]={};
    for(int iL=0;iL<NumLY;iL++){
      hETrack[iL] = new TH2F(Form("hETrackL%d",iL),"",NumnX*NumBX,0,NumnX*NumBX,NumnY*NumBY,0,NumnY*NumBY);
      hETrack[iL]-> SetTitle(Form("Fire position of EFF-used track at iZ = %d",iL));
      setTitle(hETrack[iL],"iX","iY","");
      hETrack[iL]->GetXaxis()->CenterTitle(true);
      hETrack[iL]->GetYaxis()->CenterTitle(true);
      hETrack[iL]->GetXaxis()->CenterLabels(true);
      hETrack[iL]->GetYaxis()->CenterLabels(true);
      hETrack[iL]->GetXaxis()->SetNdivisions(8);
      hETrack[iL]->GetYaxis()->SetNdivisions(8);
      hETrack[iL]->GetXaxis()->SetTitleOffset(1.0);
      hETrack[iL]->GetYaxis()->SetTitleOffset(1.0);
      hETrack[iL]->GetZaxis()->SetTitleOffset(0);
      hETrack[iL]->GetXaxis()->SetLabelSize(0.05);
      hETrack[iL]->GetYaxis()->SetLabelSize(0.05);
      hETrack[iL]->GetZaxis()->SetLabelSize(0.04);
      hETrack[iL]->GetXaxis()->SetTitleSize(0.05);
      hETrack[iL]->GetYaxis()->SetTitleSize(0.05);
      hETrack[iL]->GetZaxis()->SetRangeUser(ETExtrem[0] , ETExtrem[1]);
      hETrack[iL]->SetStats(0);
      cTrackLy->cd(1+iL);
      tf->Draw(Form("iY:iX>>hETrackL%d",iL),Form("%s&&iZ==%d",seCondition,iL),"colz");
    }
    
    
    
    DrawPdfPng(cTrackLy, Form("%sFirePositionGT%d_%s",DirRes_ChEff,eventGapTcnt[i0],TimeStr));
    
    cTrackLy->Close();
    
    delete hETExtrem, *hETrack;
  }
}
