//include
#include <iterator>
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <map>
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

int GidFindiHit(const int *vgid, const int gid, const int nH){
  for(int i=0;i<nH;i++) if(vgid[i]==gid) return i;
  cout<<"Error Not find the gid "<<gid<<" in gid array:\n {";
  cout<<vgid[0];
  for(int i=1;i<nH;i++) cout<<","<<vgid[i];
  cout<<"}"<<endl;
  throw;
}

void TrackAnaSubV5(
  const string InputGapTFile, 
  const string Tmp_ETrFile_hash, 
  const string evhash, 
  const string OsName,
  const int ir, 
  const int ev_Fir,
  const int ev_Fin,
  const double trigger,
  const bool   PassMode = 1
) {
  cout<<"  The 1st/end eventID: "<<ev_Fir<<" "<<ev_Fin+1<<endl;
  char version[8] = {};
  DetVerCheck(version,detVer);
  string EffFiles = string(Form("%sEDetCh%s_Till_Run*.root",DirIMFdat,version));
  vector<string> vStrRunTillData = find_files(EffFiles.data());
  const int totalRunNum = int(vStrRunTillData.size());
  
  map<string, GeometricAreaData> MSGAD;
  LoadPosCodeTable(MSGAD);
  
  map<int, scintillator> MScints = LoadEDectTable(vStrRunTillData[ir].data());

  
  
  int O0=0,O1=0,O2=0,O3=0,O4=0,O5=0;
  
  TreeReader data(InputGapTFile.data());
  cout<<"Opening: "<<InputGapTFile<<endl;
  TFile *rotfile = new TFile(Tmp_ETrFile_hash.data(),"RECREATE");
  cout<<Tmp_ETrFile_hash<<" is RECREATE."<<endl;

  //Data Variable
  //new Tree unit and tree unit
  
  Int_t      TrackIndex=0;
  Int_t      frame_, EvIndex_ ;
  Long64_t   unixtime_;
  Int_t      tYear_, tMonth_, tDate_, tHour_, tMinute_, tSecond_ ;
  Int_t      nLayers_ ;
  Int_t      nH ,nHk[NumLY];
  
  float      Tr_The   ,  Tr_Phi;
  float      DxDz     ,  DyDz;
  float      Tr_ZX_Err,  Tr_ZY_Err;
  char       RelCode[30];
  float      DxDzPos, DyDzPos;
  float      AreaFac;
  int        NumbFac;
  double     RMS;
  bool       CodeEmp;
  
  Int_t     *board_,    *channel_;
  Int_t      GID;

  Int_t     *pwidth_, *pwidthScaleFactor_, *pcnt_;
  Long64_t  *tcnt_;
  Double_t  *dtime_;
  
  vector<Int_t>    board, channel,gid;
  vector<Int_t>    iX, iY, iZ;
  vector<Double_t> pX, pY, pZ;
  vector<Int_t>    pwidth, pwidthk[NumLY];//pwidth0,pwidth1,pwidth2,pwidth3;
  vector<Int_t>    pwidthScaleFactor;
  vector<Int_t>    pcnt;
  vector<Long64_t> tcnt;
  vector<Double_t> dtime;
  vector<Double_t> ErrLs;
  vector<Double_t> DisP;
  //old Tree uint
  Double_t EffWei = 1.;
  vector<Double_t>  Effch;
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
  for(int i=0; i<NumLY; i++) tree->Branch(Form("nH%d",i),&(nHk[i]));
  tree->Branch("Tr_The",&Tr_The);
  tree->Branch("DxDz"  ,&DxDz); 
  tree->Branch("Tr_Phi",&Tr_Phi);
  tree->Branch("DyDz"  ,&DyDz);  
  tree->Branch("Tr_ZX_Err",&Tr_ZX_Err);
  tree->Branch("Tr_ZY_Err",&Tr_ZY_Err);
  tree->Branch("RelCode"  ,&RelCode ,"RelCode/C");
  tree->Branch("DxDzPos"  ,&DxDzPos );
  tree->Branch("DyDzPos"  ,&DyDzPos );
  tree->Branch("AreaFac"  ,&AreaFac );
  tree->Branch("NumbFac"  ,&NumbFac );
  tree->Branch("CodeEmp"  ,&CodeEmp );
  tree->Branch("RMS"      ,&RMS     );
  
  tree->Branch("board",&board);
  tree->Branch("channel",&channel);
  tree->Branch("gid",&gid);
  tree->Branch("iX",&iX);
  tree->Branch("iY",&iY);
  tree->Branch("iZ",&iZ);
  tree->Branch("pX",&pX);
  tree->Branch("pY",&pY);
  tree->Branch("pZ",&pZ);
  tree->Branch("ErrLs",&ErrLs);
  tree->Branch("pwidth",&pwidth);
  tree->Branch("DisP",&DisP);
  for(int i=0; i<NumLY; i++) tree->Branch(Form("pwidth%d",i),&(pwidthk[i]));
  
  tree->Branch("pwidthScaleFactor",&pwidthScaleFactor);
  tree->Branch("pcnt",&pcnt);
  tree->Branch("tcnt",&tcnt);
  tree->Branch("dtime",&dtime);
  tree->Branch("EffWei",&EffWei);
  tree->Branch("Effch",&Effch);
  
  int Os[6]={0};
  int
    ONPass = 1, OShowN = 1, OShowF = 1, OShowB = 1,
    OPosNE = 1, OPosEm = 1,
    OScaDx = 1, OScaDy = 1, 
    OShRMs = 1, OShRMS = 1;
  TTree *tOs = new TTree("tOs","ETrack data Os tree");
  tOs->Branch("EvIndex",&EvIndex_,"Event Index of MC/I");
  tOs->Branch("O0",&Os[0],"All Event to select track/I");
  tOs->Branch("O0",&Os[0],"All Event to select track/I");
  tOs->Branch("O1",&Os[1],"nLayers==4/I");
  tOs->Branch("O2",&Os[2],"nH>10/I");
  tOs->Branch("O3",&Os[3],"nH2>3&&nH3>3/I");
  tOs->Branch("O4",&Os[4],"nH0>3&&nH1>3/I");
  tOs->Branch("O5",&Os[5],"!CodeEmp/I");
  tOs->Branch("ONPass",&ONPass,"Order No Pass through 4 lays/I");
  tOs->Branch("OShowN",&OShowN,"Order Shower all nH too much/I");
  tOs->Branch("OShowF",&OShowF,"Order Shower at Front 2 lays/I");
  tOs->Branch("OShowB",&OShowB,"Order Shower at Back  2 lays/I");
  tOs->Branch("OPosNE",&OPosNE,"Order Best Line Pass PosCode/I");
  tOs->Branch("OPosEm",&OPosEm,"Order Best Line PosCod Empty/I");
  tOs->Branch("OScaDx",&OScaDx,"Order B.L. Scattering On  XZ/I");
  tOs->Branch("OScaDy",&OScaDy,"Order B.L. Scattering On  YZ/I");
  tOs->Branch("OShRMs",&OShRMs,"Order B.L. Shower -small RMS/I");
  tOs->Branch("OShRMS",&OShRMS,"Order B.L. Shower -large RMS/I");
  
  cout<<"  The 1st/end eventID: "<<ev_Fir<<" "<<ev_Fin+1<<endl;
  int Num_1over1k = (ev_Fin-ev_Fir)/1000;
  for (Long64_t ev = ev_Fir; ev <ev_Fin; ++ev) {
    ONPass = 0; OShowN = 0; OShowF = 0; OShowB = 0; 
    OPosNE = 0; OPosEm = 0; OScaDx = 0; OScaDy = 0;
    OShRMS = 0, OShRMs = 0;
    for(int i=0;i<6;i++) Os[i]=0;
    data.GetEntry(ev);
    unixtime_ = data.GetLong64("unixtime");
    EvIndex_  = data.GetInt("EvIndex");
    // cout<<ev<<"\t"<<EvIndex_<<endl;
    // cout<<O0<<"\t"<<O1<<"\t"<<O2<<"\t"<<O3<<"\t"<<O4<<"\t"<<O5<<endl;
    if(ev%Num_1over1k == 0) 
      cout<<Form("\r%.3f%%",(ev*100.)/(1.*ev_Fin-ev_Fir))
          <<Form("   %d",unixtime_)
          <<flush;
    O0++; Os[0]=1;

    nLayers_  = data.GetInt("nLayers");
    nH        = data.GetInt("nH");
    nHk[0]       = data.GetInt("nH0");
    nHk[1]       = data.GetInt("nH1");
    nHk[2]       = data.GetInt("nH2");
    nHk[3]       = data.GetInt("nH3");
    ONPass = nLayers_!=4;
    OShowN = !(nH <= Trigger_nH[1]);
    OShowF = !(nHk[0] <= Trigger_nH0[1] && nHk[1] <= Trigger_nH1[1]);
    OShowB = !(nHk[2] <= Trigger_nH2[1] && nHk[3] <= Trigger_nH3[1]);
    if(!ONPass){
      O1++;
      if( !OShowN ) O2++;
      if( !OShowF ) O3++;
      if( !OShowB ) O4++;
    } 
    Os[1]=!ONPass;
    Os[2]=!ONPass&&!OShowN;
    Os[3]=!ONPass&&!OShowF;
    Os[4]=!ONPass&&!OShowB;
    bool First4LaySelect = Os[1]&&Os[2]&&Os[3]&&Os[4];
    if(!First4LaySelect){
      tOs->Fill();
      continue;
    } 
      
    board_    = data.GetPtrInt("board");
    channel_  = data.GetPtrInt("channel");
    
    vector<int> GIDOnLayk[NumLY], GIDOfiHit;
    for(int iHit=0 ; iHit<nH ; iHit++){
      GID = BCIDCovGID(board_[iHit], channel_[iHit]);
      GIDOfiHit.push_back(GID);
      for(int ilay=0; ilay<NumLY;ilay++) 
        if(MScints[GID].iZ == ilay) 
          GIDOnLayk[ilay].push_back(GID);
    }
    frame_    = data.GetInt("frame");// cout<<frame_<<"\t";
    //@@ Obtain 3,5 layer track
    // const int ProbableComb = int(GIDOnLayk[0].size()*GIDOnLayk[1].size()*GIDOnLayk[2].size()*GIDOnLayk[3].size());
    vector<line3D*> vTrack3D;
    vector<std::vector<int> > vGIDCase, viHitCase;
    vector<int> GIDCase, iHitCase;
    for(int ilay=0 ; ilay<NumLY ; ilay++){
      GIDCase.push_back(0); 
      iHitCase.push_back(0); 
    }
    for(int iHit0 = 0; iHit0<int(GIDOnLayk[0].size()); iHit0++){
      GIDCase[0] = GIDOnLayk[0][iHit0]; 
      iHitCase[0] = iHit0;
      for(int iHit1 = 0; iHit1<int(GIDOnLayk[1].size()); iHit1++){
        GIDCase[1] = GIDOnLayk[1][iHit1];
        iHitCase[1] = iHit1;
        for(int iHit2 = 0; iHit2<int(GIDOnLayk[2].size()); iHit2++){
          GIDCase[2] = GIDOnLayk[2][iHit2];
          iHitCase[2] = iHit2;
          for(int iHit3 = 0; iHit3<int(GIDOnLayk[3].size()); iHit3++){
            GIDCase[3] = GIDOnLayk[3][iHit3];
            iHitCase[3] = iHit3;
            vector<double> vtmpP[3];
            for(int ilay=0 ; ilay<NumLY ; ilay++){
              GID = GIDCase[ilay];
              vtmpP[0].push_back(MScints[GID].pX);
              vtmpP[1].push_back(MScints[GID].pY);
              vtmpP[2].push_back(MScints[GID].pZ);
            }
           
            line3D *Track3Dtmp = new line3D();
            Track3Dtmp->line3Dfit( vtmpP[0], vtmpP[1], vtmpP[2] );
            vTrack3D.push_back(Track3Dtmp);
            vGIDCase.push_back(GIDCase);
            viHitCase.push_back(iHitCase);
          }
        }
      }
    }
    float BestErr = 1.E+8;
    int BestLineIndex = -1; 
    // if(frame_ == 151417) cout<<endl<<endl<<endl<<"223 "<<int(vTrack3D.size())<<endl;
    for(int iCase=0;iCase<int(vTrack3D.size());iCase++){
      float Error =
        sqrt(
          pow(vTrack3D[iCase]->Sigzx,2)
         +pow(vTrack3D[iCase]->Sigzy,2)
        );
      if(Error<BestErr){
        BestErr = Error;
        BestLineIndex = iCase;
        ErrLs.insert(ErrLs.begin(),BestErr);
      }else
        ErrLs.push_back(Error);
    }
    // cout<<271<<"BestLineIndex: "<<BestErr<<"\t"<<BestLineIndex<<endl;
    for(int ilay=0; ilay<NumLY;ilay++){
      GID = vGIDCase[BestLineIndex][ilay];
      gid.push_back(GID);
      board.push_back(BDOfGID(GID));
      channel.push_back(CIDOfGID(GID));//cout<<GID<<"\t"<<board[board.size()-1]<<"\t";
      iX.push_back(MScints[GID].iX);
      iY.push_back(MScints[GID].iY);
      iZ.push_back(MScints[GID].iZ);
      pX.push_back(MScints[GID].pX);
      pY.push_back(MScints[GID].pY);
      pZ.push_back(MScints[GID].pZ);
    }
    line3D *Track3D = new line3D() ;
    Track3D->line3Dfit( pX, pY, pZ ); 
    // cout<< sqrt(pow(Track3D->Sigzx,2)+pow(Track3D->Sigzy,2))<<endl;
    // if(TrackIndex<1) Track3D->RunGraph(Form("%sEvID%08.f.png",DirTfitG,ev*1.));
    // if((Track3D->Sigzx<TriggerTrFitSigSizeX)&&(Track3D->Sigzy<TriggerTrFitSigSizeY)){O4++;
    Tr_ZX_Err = Track3D->Sigzx;
    Tr_ZY_Err = Track3D->Sigzy;
      EvIndex_  = data.GetInt("EvIndex");
      // if(EvIndex_==56968){
        // cout<<endl<<endl<<endl;
        // cout<<trigger<<"\t"<<Tr_ZX_Err<<"\t"<<Tr_ZY_Err<<endl;
        // cout<<"\t\t"<<(Tr_ZX_Err<trigger)<<"\t"<<(Tr_ZY_Err<trigger)<<endl;
        // cout<<Tr_ZX_Err<<"\t"<<Tr_ZY_Err<<endl;
        // cout<<endl<<endl;
      // } 
    if((Tr_ZX_Err<trigger)&&(Tr_ZY_Err<trigger)||PassMode){
      
      if(PassMode) TrackIndex = EvIndex_;
      else TrackIndex++;
      // if((Track3D->Sigzx>TriggerTrFitSigSizeX/1.414||Track3D->Sigzy>TriggerTrFitSigSizeY/1.414)&&nLayers_==4)
        // Track3D->RunGraph(Form("%sTrackID%08.f.png",DirTfitG,TrackIndex*1.));
      // if(EvIndex_==56968) cout<<"pass 311"<<endl;

      // if(TrackIndex>50) throw;
      // cout<<Form("\n%10.2e%10.2e\n",Track3D->Sigzx,Track3D->Sigzy);
      // sleep(0.1);
      Tr_The = Track3D->Lfit3D->Dthe;
      Tr_Phi = Track3D->Lfit3D->Dphi;
      DxDz = Track3D->rzx;
      DyDz = Track3D->rzy;

      int pos[NumLY][2]={};
      

      for(int ily=0;ily<NumLY;ily++){
        pos[iZ[ily]][0] = iX[ily];
        pos[iZ[ily]][1] = iY[ily];
      }
      // cout<<endl<<pos[0][0]<<pos[1][0]<<pos[2][0]<<pos[3][0]<<pos[0][1]<<pos[1][1]<<pos[2][1]<<pos[3][1]<<endl;
      for(int ily=1;ily<NumLY;ily++){
        pos[ily][0] = pos[ily][0]-pos[0][0];
        pos[ily][1] = pos[ily][1]-pos[0][1];
      }
      pos[0][0] = 0;  pos[0][1] = 0;
      // FATAL("326");
      sprintf(RelCode,"%s", (getRelCode(pos,NumLY)).data());
      DxDzPos = MSGAD[RelCode].DXZ;
      DyDzPos = MSGAD[RelCode].DYZ;
      AreaFac = MSGAD[RelCode].AreaFactor;
      NumbFac = MSGAD[RelCode].NumbFactor;
      CodeEmp = MSGAD[RelCode].Empty;
      OPosNE = !CodeEmp; 
      OPosEm = CodeEmp;
      if(OPosNE){ O5++; Os[5]=1; };
      /* Load Frame Imformation */
      frame_    = data.GetInt("frame");
      tYear_    = data.GetInt("tYear");
      tMonth_   = data.GetInt("tMonth");
      tDate_    = data.GetInt("tDate");
      tHour_    = data.GetInt("tHour");
      tMinute_  = data.GetInt("tMinute");
      tSecond_  = data.GetInt("tSecond");
      
      for(int iLay=0; iLay<NumLY; iLay++) 
        nHk[iLay]= data.GetInt(Form("nH%d",iLay));
      
      /* Load Hits Imformation */
      board_    = data.GetPtrInt("board");
      channel_  = data.GetPtrInt("channel");
      pwidth_   = data.GetPtrInt("pwidth");
      pwidthScaleFactor_   = data.GetPtrInt("pwidthScaleFactor");
      pcnt_     = data.GetPtrInt("pcnt");
      tcnt_     = data.GetPtrLong64("tcnt");
      dtime_    = data.GetPtrDouble("dtime");
      // if(EvIndex_==56968) cout<<"pass 362"<<endl;
      
      for(int ilay=0 ; ilay<nLayers_ ; ilay++){
        int indHit = viHitCase[BestLineIndex][ilay];
        pwidth.push_back(pwidth_[indHit]);
        GID = vGIDCase[BestLineIndex][ilay];
        for(int iLay=0; iLay<NumLY; iLay++) pwidthk[iLay].push_back(-1);
        for(int iLay=0; iLay<NumLY; iLay++){
          if(MScints[GID].iZ==iLay){
            pwidthk[iLay][indHit]=pwidth_[indHit];
            break;
          }
        }
        pwidthScaleFactor.push_back(pwidthScaleFactor_[ilay]);
        pcnt.push_back(pcnt_[ilay]);
        tcnt.push_back(tcnt_[ilay]);
        dtime.push_back(dtime_[ilay]);
        Effch.push_back(MScints[GID].Efficiency);
        EffWei/=MScints[GID].Efficiency;
      }
      
      for(int iHit = 0; iHit<nH ; iHit++){
        bool iHitAlive = false;
        for(int ilay=0 ; ilay<nLayers_ ; ilay++){
          if(GIDOfiHit[iHit] == vGIDCase[BestLineIndex][ilay]) iHitAlive = true;
        }
        if(iHitAlive) continue;
        GID = BCIDCovGID(board_[iHit], channel_[iHit]);
        gid.push_back(GID);
        board.push_back(BDOfGID(GID));
        channel.push_back(CIDOfGID(GID));//cout<<GID<<"\t"<<board[board.size()-1]<<"\t";
        iX.push_back(MScints[GID].iX);
        iY.push_back(MScints[GID].iY);
        iZ.push_back(MScints[GID].iZ);
        pX.push_back(MScints[GID].pX);
        pY.push_back(MScints[GID].pY);
        pZ.push_back(MScints[GID].pZ);
        pwidth.push_back(pwidth_[iHit]);
        pwidthScaleFactor.push_back(pwidthScaleFactor_[iHit]);
        pcnt.push_back(pcnt_[iHit]);
        tcnt.push_back(tcnt_[iHit]);
        dtime.push_back(dtime_[iHit]);
        Effch.push_back(MScints[GID].Efficiency);
      }
      
      RMS = Track3D->RMSOfPointsAndBL( pX, pY, pZ);
      
      // OScaDx = Tr_ZX_Err>12&&abs(DxDz)>0.041; //@@ Set Trigger
      // OScaDy = Tr_ZY_Err>12&&abs(DyDz)>0.041;
      OScaDx = abs(DxDz)>0.041; //@@ Set Trigger
      OScaDy = abs(DyDz)>0.041;
      OShRMS = RMS>20&&nH>4;
      OShRMs = RMS<=20&&nH>4;
      
      double p0[3] = {Track3D->dzx,Track3D->dzy,Track3D->dzz};
      double Rl[3] = {DxDz,DyDz,1};
      // if(RMS>100){
        // cout<<"RMS:  "<<RMS<<endl;
        // cout<<"p0\t"<<p0[0]<<"	"<<p0[1]<<"	"<<p0[2]<<endl;
        // cout<<"Rl\t"<<Rl[0]<<"	"<<Rl[1]<<"	"<<Rl[2]<<endl;
      // }
      // double RMSPrime = 0;
      for(int iHit = 0; iHit<nH ; iHit++){
        double p1[3] = {pX[iHit],pY[iHit],pZ[iHit]};
        double DisOfLineAndPlane = distOfPlanePoint( p0, Rl, p1);
        DisP.push_back(DisOfLineAndPlane);
        // if(RMS>100){
          // RMSPrime+=pow(DisOfLineAndPlane,2);
          // cout<<"p1\t"<<p1[0]<<"	"<<p1[1]<<"	"<<p1[2]<<"\t"<<DisOfLineAndPlane<<endl;
        // }
      }
      // if(RMS>100) cout<<"RMSPrime: "<<sqrt(RMSPrime/(nH*1.))<<endl;
      tree->Fill();
      // tOs->Fill();
      pwidth.clear();
      for(int iLay=0; iLay<NumLY; iLay++) pwidthk[iLay].clear();
      pwidthScaleFactor.clear();
      pcnt.clear();
      tcnt.clear();
      dtime.clear();
      Effch.clear();
      DisP.clear();
      EffWei = 1.;
    }
    gid.clear();
    board.clear();
    channel.clear();
    iX.clear();
    iY.clear();
    iZ.clear();
    pX.clear();
    pY.clear();
    pZ.clear();
    DisP.clear();
    Effch.clear();
    EffWei = 1.;
    ErrLs.clear();
    delete Track3D;
    tOs->Fill();
  }
  
  // tOs->Fill();
  ofstream outOs(OsName.data());
  outOs<<O0<<"\t"<<O1<<"\t"<<O2<<"\t"<<O3<<"\t"<<O4<<"\t"<<O5<<"\t"<<TrackIndex;
  cout<<endl<<O0<<"\t"<<O1<<"\t"<<O2<<"\t"<<O3<<"\t"<<O4<<"\t"<<O5<<"\t"<<TrackIndex<<endl;
  outOs.close();
  rotfile->Write();
  rotfile->Close();
}


void TrackAnaSubHAdd(const string TargetETrFile, const vector<string> vChipETrFiles, const vector<int> StaTrackIndex) {
  // cout<<&StaTrackIndex<<endl;
  // cout<<StaTrackIndex[7]<<endl;
  // throw;
  TreeReader data(vChipETrFiles);
  
  TChain *t   = new TChain("t"  );
  TChain *tOsI = new TChain("tOs");

  for(int i=0;i<int(vChipETrFiles.size());i++){
    Long64_t fsize = FileSize(vChipETrFiles[i].data());
    if(fsize<8133){
      cout<<"size of: "<<vChipETrFiles[i]<<"is too small, no data"<<endl;
      continue;
    }
    t->Add(vChipETrFiles[i].data());
    tOsI->Add(vChipETrFiles[i].data());
    cout<<"Opening chip of ETrack files: "<<vChipETrFiles[i].data()<<endl;
  }
  
  TFile *rotfile = new TFile(TargetETrFile.data(),"RECREATE");
  cout<<TargetETrFile<<" is RECREATE."<<endl;

  //Data Variable
  //new Tree unit and tree unit
  Int_t      TrackIndex;
  Int_t      frame, EvIndex;
  Long64_t   unixtime;
  Int_t      tYear, tMonth, tDate, tHour, tMinute, tSecond ;
  Int_t      nLayers ;
  Int_t      nH ,nHk[NumLY];
  
  float      Tr_The   ,  Tr_Phi;
  float      DxDz     ,  DyDz;
  float      Tr_ZX_Err,  Tr_ZY_Err;
  char       RelCode[40], *RelCode_;
  float      DxDzPos, DyDzPos;
  float      AreaFac;
  int        NumbFac;
  bool       CodeEmp;
  Double_t   EffWei = 1.;
  Double_t   RMS = 0;
  Int_t     *board_, *channel_, *gid_;
  Int_t     *iX_, *iY_, *iZ_;
  double    *pX_, *pY_, *pZ_;
  Int_t     *pwidth_, *pwidthk_[NumLY];
  Int_t     *pwidthScaleFactor_;
  Int_t     *pcnt_;
  Long64_t  *tcnt_;
  Double_t  *dtime_, *ErrLs_;
  Double_t  *Effch_, *DisP_;
  
  vector<Int_t>    board, channel,gid;
  vector<Int_t>    iX, iY, iZ;
  vector<double> pX, pY, pZ;
  vector<Int_t>    pwidth, pwidthk[NumLY];//pwidth0,pwidth1,pwidth2,pwidth3;
  vector<Int_t>    pwidthScaleFactor;
  vector<Int_t>    pcnt;
  vector<Long64_t> tcnt;
  vector<double>  dtime;
  vector<double>  ErrLs;
  vector<double>  Effch;
  vector<double>  DisP;
  //total number for hit in a frame/event for ni is i board

  TTree *tree = new TTree("t","ETrack data tree");
  TTree *tOs = new TTree("tOs","ETrack data Os tree");
  tree->Branch("TrackIndex",&TrackIndex);
  tree->Branch("frame",&frame);
  tree->Branch("EvIndex",&EvIndex);
  tree->Branch("unixtime",&unixtime);
  tree->Branch("tYear",&tYear);
  tree->Branch("tMonth",&tMonth);
  tree->Branch("tDate",&tDate);
  tree->Branch("tHour",&tHour);
  tree->Branch("tMinute",&tMinute);
  tree->Branch("tSecond",&tSecond);
  tree->Branch("nLayers",&nLayers);
  tree->Branch("nH",&nH);
  for(int i=0; i<NumLY; i++) tree->Branch(Form("nH%d",i),&(nHk[i]));
  tree->Branch("Tr_The",&Tr_The);
  tree->Branch("DxDz"  ,&DxDz); 
  tree->Branch("Tr_Phi",&Tr_Phi);
  tree->Branch("DyDz"  ,&DyDz);  
  tree->Branch("Tr_ZX_Err",&Tr_ZX_Err);
  tree->Branch("Tr_ZY_Err",&Tr_ZY_Err);
  tree->Branch("RelCode"  ,&RelCode ,"RelCode/C");
  tree->Branch("DxDzPos"  ,&DxDzPos );
  tree->Branch("DyDzPos"  ,&DyDzPos );
  tree->Branch("AreaFac"  ,&AreaFac );
  tree->Branch("NumbFac"  ,&NumbFac );
  tree->Branch("CodeEmp"  ,&CodeEmp );
  tree->Branch("RMS"      ,&RMS     );
  tree->Branch("EffWei"   ,&EffWei  );

  tree->Branch("board",&board);
  tree->Branch("channel",&channel);
  tree->Branch("gid",&gid);
  tree->Branch("iX",&iX);
  tree->Branch("iY",&iY);
  tree->Branch("iZ",&iZ);
  tree->Branch("pX",&pX);
  tree->Branch("pY",&pY);
  tree->Branch("pZ",&pZ);
  tree->Branch("pwidth",&pwidth);
  tree->Branch("DisP",&DisP);
  for(int i=0; i<NumLY; i++) tree->Branch(Form("pwidth%d",i),&(pwidthk[i]));
  tree->Branch("pwidthScaleFactor",&pwidthScaleFactor);
  tree->Branch("pcnt",&pcnt);
  tree->Branch("tcnt",&tcnt);
  tree->Branch("dtime",&dtime);
  tree->Branch("ErrLs",&ErrLs);
  tree->Branch("Effch",&Effch);
  
  int evss = t->GetEntries();
  int IndexOfChip = 0, TillIndex = 0;
  int ONPass=0,OShowN=0,OShowF=0,OShowB=0,OPosNE=0,OFitEr=0;
  int OScaDx=0,OScaDy=0,OScDxy=0,OShRMS=0,OShRMs=0,OPosEm=0;
  // cout<<583<<endl;
  // cout<<int(StaTrackIndex.size())<<endl;
  // for(int i=0;i<int(StaTrackIndex.size());i++) cout<<StaTrackIndex[i]<<endl;
  // throw;
  for (Long64_t ev = 0; ev <evss; ++ev) {
    data.GetEntry(ev);
    TrackIndex = data.GetInt("TrackIndex");
    if(ev!=0&&TrackIndex==0){
      IndexOfChip++;
      TillIndex += StaTrackIndex[IndexOfChip];
    }
    // if(ev%100==0) cout<<515<<" "<<IndexOfChip<<" "<<TrackIndex<<" "<<TillIndex<<endl;
    TrackIndex += TillIndex;
    
    frame      = data.GetInt   ("frame"   );
    if(ev%1000 == 0) 
      cout<<Form("\r%.5f%%",(ev*100.)/(1.*evss)) <<flush;
    EvIndex    = data.GetInt   ("EvIndex" );
    unixtime   = data.GetLong64("unixtime");
    tYear      = data.GetInt   ("tYear"   );
    tMonth     = data.GetInt   ("tMonth"  );
    tDate      = data.GetInt   ("tDate"   );
    tHour      = data.GetInt   ("tHour"   );
    tMinute    = data.GetInt   ("tMinute" );
    tSecond    = data.GetInt   ("tSecond" );
    nLayers    = data.GetInt   ("nLayers" );
    nH         = data.GetInt   ("nH"      );
    for(int ily=0; ily<NumLY; ily++) 
      nHk[ily]= data.GetInt   (Form("nH%d",ily));
    
    Tr_The =    data.GetFloat ("Tr_The"   );
    DxDz = data.GetFloat      ("DxDz"     ); 
    Tr_Phi = data.GetFloat    ("Tr_Phi"   );
    DyDz = data.GetFloat      ("DyDz"     );  
    Tr_ZX_Err = data.GetFloat ("Tr_ZX_Err");
    Tr_ZY_Err = data.GetFloat ("Tr_ZY_Err");
    
    t->GetEntry(ev);
    t->SetBranchAddress("RelCode" ,&RelCode);
    DxDzPos = data.GetFloat   ("DxDzPos"  );
    DyDzPos = data.GetFloat   ("DyDzPos"  );
    AreaFac = data.GetFloat   ("AreaFac"  );
    NumbFac = data.GetInt     ("NumbFac"  );
    CodeEmp = data.GetBool    ("CodeEmp"  );
    RMS    = data.GetDouble   ("RMS"      );
    EffWei = data.GetDouble   ("EffWei"   );
    
  
    board_    = data.GetPtrInt("board");
    channel_  = data.GetPtrInt("channel");
    gid_      = data.GetPtrInt("gid");
    iX_      = data.GetPtrInt("iX");
    iY_      = data.GetPtrInt("iY");
    iZ_      = data.GetPtrInt("iZ");
    pX_      = data.GetPtrDouble("pX");
    pY_      = data.GetPtrDouble("pY");
    pZ_      = data.GetPtrDouble("pZ");
    pwidth_      = data.GetPtrInt("pwidth");
    for(int ily=0; ily<NumLY; ily++) 
    pwidthk_[ily] = data.GetPtrInt(Form("pwidth%d",ily));
    pwidthScaleFactor_ = data.GetPtrInt("pwidthScaleFactor");
    pcnt_     = data.GetPtrInt("pcnt");
    tcnt_     = data.GetPtrLong64("tcnt");
    dtime_    = data.GetPtrDouble("dtime");
    ErrLs_    = data.GetPtrDouble("ErrLs");
    Effch_    = data.GetPtrDouble("Effch");
    DisP_     = data.GetPtrDouble("DisP");
    for(int iHit=0; iHit<nH;iHit++){
      board   .push_back(board_[iHit]);
      channel .push_back(channel_[iHit]);
      gid     .push_back(gid_[iHit]);
      iX      .push_back(iX_[iHit]);
      iY      .push_back(iY_[iHit]);
      iZ      .push_back(iZ_[iHit]);
      pX      .push_back(pX_[iHit]);
      pY      .push_back(pY_[iHit]);
      pZ      .push_back(pZ_[iHit]);
      pwidth  .push_back(pwidth_[iHit]);
      for(int ily=0; ily<NumLY; ily++) 
        pwidthk[ily] .push_back(pwidthk_[ily][iHit]);
      pwidthScaleFactor  .push_back(pwidthScaleFactor_[iHit]);
      pcnt     .push_back(pcnt_[iHit]);
      tcnt     .push_back(tcnt_[iHit]);
      dtime    .push_back(dtime_[iHit]);
      ErrLs    .push_back(ErrLs_[iHit]);
      Effch    .push_back(Effch_[iHit]);
      DisP     .push_back(DisP_[iHit]);
    }
    
    tree->Fill();
      
    pwidth.clear();
    for(int iLay=0; iLay<NumLY; iLay++) pwidthk[iLay].clear();
    pwidthScaleFactor.clear();
    pcnt.clear();
    tcnt.clear();
    dtime.clear();
    Effch.clear();
    EffWei = 1.;
    gid.clear();
    board.clear();
    channel.clear();
    iX.clear();
    iY.clear();
    iZ.clear();
    pX.clear();
    pY.clear();
    pZ.clear();
    Effch.clear();
    ErrLs.clear();
    DisP .clear();
  }
  int vOs[6]={0},vOSum[6]={0};
  
  tOsI->SetBranchAddress("EvIndex",&EvIndex);
  for(int i1=0;i1<6;i1++)
    tOsI->SetBranchAddress(Form("O%d",i1),&vOSum[i1]);
  tOsI->SetBranchAddress("ONPass",&ONPass);
  tOsI->SetBranchAddress("OShowN",&OShowN);
  tOsI->SetBranchAddress("OShowF",&OShowF);
  tOsI->SetBranchAddress("OShowB",&OShowB);
  tOsI->SetBranchAddress("OPosNE",&OPosNE);
  tOsI->SetBranchAddress("OPosEm",&OPosEm);
  tOsI->SetBranchAddress("OScaDx",&OScaDx);
  tOsI->SetBranchAddress("OScaDy",&OScaDy);
  tOsI->SetBranchAddress("OShRMS",&OShRMS);
  tOsI->SetBranchAddress("OShRMs",&OShRMs);

  tOs->Branch("EvIndex",&EvIndex,"Event Index of MC/I");
  tOs->Branch("O0",&vOSum[0],"All Event to select track/I");
  tOs->Branch("O1",&vOSum[1],"nLayers==4/I");
  tOs->Branch("O2",&vOSum[2],"nH>10/I");
  tOs->Branch("O3",&vOSum[3],"nH2>3&&nH3>3/I");
  tOs->Branch("O4",&vOSum[4],"nH0>3&&nH1>3/I");
  tOs->Branch("O5",&vOSum[5],"!CodeEmp/I");
  tOs->Branch("ONPass",&ONPass,"Order No Pass through 4 lays/I"); 
  tOs->Branch("OShowN",&OShowN,"Order Shower all nH too much/I"); 
  tOs->Branch("OShowF",&OShowF,"Order Shower at Front 2 lays/I"); 
  tOs->Branch("OShowB",&OShowB,"Order Shower at Back  2 lays/I"); 
  tOs->Branch("OPosNE",&OPosNE,"Order Best Line Pass PosCode/I"); 
  tOs->Branch("OPosEm",&OPosEm,"Order Best Line PosCod Empty/I");
  tOs->Branch("OScaDx",&OScaDx,"Order B.L. Scattering On  XZ/I");
  tOs->Branch("OScaDy",&OScaDy,"Order B.L. Scattering On  YZ/I");
  tOs->Branch("OShRMS",&OShRMS,"Order B.L. Shower -large RMS/I");
  tOs->Branch("OShRMs",&OShRMs,"Order B.L. Shower -small RMS/I");
  
  for(int i=0;i<int(tOsI->GetEntries());i++){
    tOsI->GetEntry(i);
    tOs->Fill();
  }

  rotfile->Write();
  rotfile->Close();
}

