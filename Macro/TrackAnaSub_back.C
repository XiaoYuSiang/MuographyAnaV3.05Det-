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

void TrackAnaSub(const string InputGapTFile, const string Tmp_ETrFile_hash, const string evhash, const string OsName, const int ir, const int ev_Fir, const int ev_Fin,const double trigger) {
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
  for(int i=0; i<NumLY; i++) tree->Branch(Form("pwidth%d",i),&(pwidthk[i]));
  
  tree->Branch("pwidthScaleFactor",&pwidthScaleFactor);
  tree->Branch("pcnt",&pcnt);
  tree->Branch("tcnt",&tcnt);
  tree->Branch("dtime",&dtime);
  tree->Branch("EffWei",&EffWei);
  tree->Branch("Effch",&Effch);
  
  for (Long64_t ev = ev_Fir; ev <ev_Fin; ++ev) {
    data.GetEntry(ev);
    unixtime_ = data.GetLong64("unixtime");
    if(ev%1000 == 0) 
      cout<<Form("\r%.5f%%",(ev*100.)/(1.*ev_Fin))
          <<Form("   %d",unixtime_)
          <<flush;
    O0++;
    
    nLayers_  = data.GetInt("nLayers");
    if(nLayers_<4) continue;
    O1++;
    
    nH        = data.GetInt("nH");
    if( nH > 6 ) continue;
    O2++;
    
    nHk[0]       = data.GetInt("nH0");
    nHk[1]       = data.GetInt("nH1");
    if( nHk[1] > 2 ) continue;
    if( nHk[0] > 2 ) continue;
    O3++;
    
    nHk[2]       = data.GetInt("nH2");
    nHk[3]       = data.GetInt("nH3");
    if( nHk[3] != 1 ) continue;
    if( nHk[2] != 1 ) continue;
    O4++;
    
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
    if((Tr_ZX_Err<trigger)&&(Tr_ZY_Err<trigger)){
      TrackIndex++;
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
      
      sprintf(RelCode,"%s", (getRelCode(pos,NumLY)).data());
      DxDzPos = MSGAD[RelCode].DXZ;
      DyDzPos = MSGAD[RelCode].DYZ;
      AreaFac = MSGAD[RelCode].AreaFactor;
      NumbFac = MSGAD[RelCode].NumbFactor;
      CodeEmp = MSGAD[RelCode].Empty; if(!CodeEmp) O5++; 
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
      
      tree->Fill();
      
      pwidth.clear();
      for(int iLay=0; iLay<NumLY; iLay++) pwidthk[iLay].clear();
      pwidthScaleFactor.clear();
      pcnt.clear();
      tcnt.clear();
      dtime.clear();
      Effch.clear();
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
    Effch.clear();
    EffWei = 1.;
    ErrLs.clear();
    delete Track3D;
  }
  ofstream outOs(OsName.data());
  outOs<<O0<<"\t"<<O1<<"\t"<<O2<<"\t"<<O3<<"\t"<<O4<<"\t"<<O5<<"\t"<<TrackIndex;
  outOs.close();
  rotfile->Write();
  rotfile->Close();
}


void TrackAnaSubHAdd(const string TargetETrFile, const vector<string> vChipETrFiles, vector<int> StaTrackIndex) {
  TreeReader data(vChipETrFiles);

  TChain *t = new TChain("t");
  for(int i=0;i<int(vChipETrFiles.size());i++){
    t->Add(vChipETrFiles[i].data());
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
  
  Int_t     *board_, *channel_, *gid_;
  Int_t     *iX_, *iY_, *iZ_;
  double    *pX_, *pY_, *pZ_;
  Int_t     *pwidth_, *pwidthk_[NumLY];
  Int_t     *pwidthScaleFactor_;
  Int_t     *pcnt_;
  Long64_t  *tcnt_;
  Double_t  *dtime_, *ErrLs_;
  Double_t  *Effch_;
  
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
  //total number for hit in a frame/event for ni is i board

  TTree *tree = new TTree("t","ETrack data tree");
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
  for(int i=0; i<NumLY; i++) tree->Branch(Form("pwidth%d",i),&(pwidthk[i]));
  tree->Branch("pwidthScaleFactor",&pwidthScaleFactor);
  tree->Branch("pcnt",&pcnt);
  tree->Branch("tcnt",&tcnt);
  tree->Branch("dtime",&dtime);
  tree->Branch("ErrLs",&ErrLs);
  tree->Branch("Effch",&Effch);
  
  int evss = t->GetEntries();
  int IndexOfChip = -1, TillIndex = 0;
  
  for (Long64_t ev = 0; ev <evss; ++ev) {
    data.GetEntry(ev);
    TrackIndex = data.GetInt("TrackIndex");
    if(ev!=0&&TrackIndex==1){
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
  }
  rotfile->Write();
  rotfile->Close();
}

