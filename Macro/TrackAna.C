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
#include "/data4/YuSiang/personalLib/Style/DBMTStyle.h"
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"
#include "DSLData.h"
#include "LoadODectTable.h"

using namespace std;
using namespace DBMMath;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace MuoAna_path_dir;
using namespace DataConst;

//channel vs rate combine

void TrackAna(const int indexi=28, const int indexf=29, const bool testMode=false ) {
  defstyle();
  double eventGap[46]={};
  map<int, scintillator> MScints = LoadODectTable();
  //Printf("Files found: %i", (int) files.size());
  //ofstream out(Form("%sSEVertical4v3.dat",DirOperate));
  for(int i0=indexi;i0<indexf;i0++){
    eventGap[i0] = (1.0*eventGapTcnt[i0])/2560000000.;
    char RFNStr[200];
    sprintf(RFNStr,"%d_%s_nHTH%dL%d.root",eventGapTcnt[i0],TimeStr,TriggerS,TriggerL);
    TreeReader data(Form("%sGapT%s",DirOperate,RFNStr));
    cout<<Form("%sGapT%s",DirOperate,RFNStr)<<" is opened"<<endl;
        
    TFile *rotfile = new TFile(Form("%sETracksGT%s", DirOperate, RFNStr),"RECREATE");
    cout<<Form("%sETracksGT%s", DirOperate, RFNStr)<<" is opened"<<endl;

    //Data Variable
    //new Tree unit and tree unit
    
    Int_t      TrackIndex=0;
    Int_t      frame_   ;
    Int_t      EvIndex_ ;
    Long64_t   unixtime_;
    Int_t      tYear_, tMonth_, tDate_, tHour_, tMinute_, tSecond_ ;
    Int_t      nLayers_ ;
    
    float      Tr_The,     Tr_Phi;
    float      DxDz  ,     DyDz  ;
    // float      Tr_The_Err, Tr_Phi_Err;
    Int_t     *board_,    *channel_;
    int GID;

    Int_t*     pwidth_;
    Int_t*     pwidthScaleFactor_;
    Int_t*     pcnt_;
    Long64_t*  tcnt_;
                  
    Double_t*        dtime_;
    
    vector<Int_t>    board;
    vector<Int_t>    channel;
    vector<Int_t>    iX;
    vector<Int_t>    iY;
    vector<Int_t>    iZ;
    vector<Double_t> pX;
    vector<Double_t> pY;
    vector<Double_t> pZ;
    vector<Int_t>    pwidth,pwidth0,pwidth1,pwidth2,pwidth3;
    vector<Int_t>    pwidthScaleFactor;
    vector<Int_t>    pcnt;
    vector<Long64_t> tcnt;
    vector<Double_t> dtime;
    
    //old Tree uint
    Int_t      nH ,nH0 ,nH1 ,nH2 ,nH3  ;
    //total number for hit in a frame/event for ni is i board
    
    TTree *tree = new TTree("t","data from analyzing file");
    tree->Branch("TrackIndex",&TrackIndex);
    tree->Branch("frame",&frame_);
    tree->Branch("EvIndex",&EvIndex_);
    tree->Branch("unixtime",&unixtime_);
    tree->Branch("tYear",&tYear_);
    tree->Branch("tMonth",&tMonth_);
    tree->Branch("tDate",&tDate_);
    tree->Branch("tHour",&tHour_);
    tree->Branch("tMinute",&tMinute_);
    tree->Branch("tSecond",&tSecond_);
    tree->Branch("nLayers",&nLayers_);
    tree->Branch("nH",&nH);
    tree->Branch("nH0",&nH0);
    tree->Branch("nH1",&nH1);
    tree->Branch("nH2",&nH2);
    tree->Branch("nH3",&nH3);
    tree->Branch("Tr_The",&Tr_The);     //@@
    tree->Branch("DxDz"  ,&DxDz);     //@@
    tree->Branch("Tr_Phi",&Tr_Phi);     //@@
    tree->Branch("DyDz"  ,&DyDz);     //@@
    // tree->Branch("Tr_The_Err",&Tr_The_Err);     //@@
    // tree->Branch("Tr_Phi_Err",&Tr_Phi_Err);     //@@
    tree->Branch("board",&board);
    tree->Branch("channel",&channel);
    tree->Branch("iX",&iX);
    tree->Branch("iY",&iY);
    tree->Branch("iZ",&iZ);
    tree->Branch("pX",&pX);
    tree->Branch("pY",&pY);
    tree->Branch("pZ",&pZ);
    tree->Branch("pwidth",&pwidth);
    tree->Branch("pwidth0",&pwidth0);
    tree->Branch("pwidth1",&pwidth1);
    tree->Branch("pwidth2",&pwidth2);
    tree->Branch("pwidth3",&pwidth3);
    tree->Branch("pwidthScaleFactor",&pwidthScaleFactor);
    tree->Branch("pcnt",&pcnt);
    tree->Branch("tcnt",&tcnt);
    tree->Branch("dtime",&dtime);
    
    //take time and set anlyze Constant for boundry condition
    Long64_t evsTr = data.GetEntriesFast();
    cout <<"total event:\t"<<evsTr<<endl;
    data.GetEntry(0);
    if(testMode) evsTr = evsTr*0.01;
    //Fill the data
    for (Long64_t ev = 0; ev <evsTr; ++ev) {//evsTr; ++ev) {
      data.GetEntry(ev);
      if(ev%1000 == 0) cout<<Form("\r%.5f%%",(ev*100.)/(1.*evsTr))<<flush;
      nH        = data.GetInt("nH");
      if(nH<3) continue;
      if(nH>9) continue;
      nLayers_  = data.GetInt("nLayers");
      if(nLayers_<3) continue;
      
      board_    = data.GetPtrInt("board");
      channel_  = data.GetPtrInt("channel");
      for(int iHit=0 ; iHit<nH ; iHit++){
        GID = BCIDCovGID(board_[iHit], channel_[iHit]);
        iX.push_back(MScints[GID].iX);  //if(board[iHit] ==hitBDA) continue;
        iY.push_back(MScints[GID].iY);  //if(board[iHit] ==hitBDA) continue;
        iZ.push_back(MScints[GID].iZ);
        board.push_back(board_[iHit]);
        channel.push_back(channel_[iHit]);
        pX.push_back(MScints[GID].pX);
        pY.push_back(MScints[GID].pY);
        pZ.push_back(MScints[GID].pZ);
      }
      line3D *Track3D = new line3D() ;
      Track3D->line3Dfit( pX, pY, pZ );
      if((Track3D->Sigzx<1.25)&&(Track3D->Sigzy<1.25)){

        // Tr_The_Err = Track3D->ErrThe;
        // Tr_Phi_Err = Track3D->ErrPhi;
        // cout<<Form("\n%10.2e%10.2e\n",Track3D->Sigzx,Track3D->Sigzy);
        // sleep(0.1);
        Tr_The = Track3D->Lfit3D->Dthe;
        Tr_Phi = Track3D->Lfit3D->Dphi;
        DxDz = Track3D->rzx;
        DyDz = Track3D->rzy;
        
        /* Load Frame Imformation */
        frame_    = data.GetInt("frame");
        EvIndex_  = data.GetInt("EvIndex");
        unixtime_ = data.GetLong64("unixtime");
        tYear_    = data.GetInt("tYear");
        tMonth_   = data.GetInt("tMonth");
        tDate_    = data.GetInt("tDate");
        tHour_    = data.GetInt("tHour");
        tMinute_  = data.GetInt("tMinute");
        tSecond_  = data.GetInt("tSecond");
        
        nH0       = data.GetInt("nH0");
        nH1       = data.GetInt("nH1");
        nH2       = data.GetInt("nH2");
        nH3       = data.GetInt("nH3");
        
        /* Load Hits Imformation */
        board_    = data.GetPtrInt("board");
        channel_  = data.GetPtrInt("channel");
        pwidth_   = data.GetPtrInt("pwidth");
        pwidthScaleFactor_   = data.GetPtrInt("pwidthScaleFactor");
        pcnt_     = data.GetPtrInt("pcnt");
        tcnt_     = data.GetPtrLong64("tcnt");
        dtime_    = data.GetPtrDouble("dtime");
        
        for(int iHit=0 ; iHit<nH ; iHit++){
          pwidth.push_back(pwidth_[iHit]);
          GID = BCIDCovGID(board_[iHit], channel_[iHit]);
          pwidth0.push_back(-1);
          pwidth1.push_back(-1);
          pwidth2.push_back(-1);
          pwidth3.push_back(-1);
          
          if     (MScints[GID].iZ==0) pwidth0[iHit]=pwidth_[iHit];
          else if(MScints[GID].iZ==1) pwidth1[iHit]=pwidth_[iHit];
          else if(MScints[GID].iZ==2) pwidth2[iHit]=pwidth_[iHit];
          else if(MScints[GID].iZ==3) pwidth3[iHit]=pwidth_[iHit];
          pwidthScaleFactor.push_back(pwidthScaleFactor_[iHit]);
          pcnt.push_back(pcnt_[iHit]);
          tcnt.push_back(tcnt_[iHit]);
          dtime.push_back(dtime_[iHit]);
        }
        // cout<<"HR: "<<tHour_<<endl;
        tree->Fill();

        pwidth.clear();
        pwidth0.clear();
        pwidth1.clear();
        pwidth2.clear();
        pwidth3.clear();
        pwidthScaleFactor.clear();
        pcnt.clear();
        tcnt.clear();
        dtime.clear();
      }
      
      board.clear();
      channel.clear();
      iX.clear();
      iY.clear();
      iZ.clear();
      pX.clear();
      pY.clear();
      pZ.clear();
      
      delete Track3D;

    } cout<<"\rFinish Tracking of Events"<<endl;

    rotfile->Write();
    
    
  

  }
}


