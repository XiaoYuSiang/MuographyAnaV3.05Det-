//include
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

//channel vs rate combine

void TrackAna(const int indexi=28, const int indexf=29, const double trigger = TriggerTrFitSigSizeX,const bool testMode=false ) {
  defstyle();
  double eventGap[46]={};
  char version[8] = {};
  DetVerCheck(version,detVer);
  
  char cmdline[250], DirTfitG[200];
  
  sprintf(DirTfitG,"%sTrackingDetail/",DirRes_LFirTra);
  sprintf(cmdline,"mkdir -p %s",DirTfitG);
  if (system(cmdline)!=0) system(cmdline);
  //Printf("Files found: %i", (int) files.size());
  //ofstream out(Form("%sSEVertical4v3.dat",DirOperate));
  
  string EffFiles = string(Form("%sEDetCh%s_Till_Run*.root",DirIMFdat,version));
  vector<string> vStrRunTillData = find_files(EffFiles.data());
  const int totalRunNum = int(vStrRunTillData.size());
  
  map<string, GeometricAreaData> MSGAD;
  LoadPosCodeTable(MSGAD);
    
  ofstream ott("/home/yusiang/G4DataFindR2/lEbhgrsabhvrs.txt", ofstream::out | ofstream::app );
  int O0=0,O1=0,O2=0,O3=0,O4=0,O5=0;
  
  for(int i0=indexi;i0<indexf;i0++){
    eventGap[i0] = (1.0*eventGapTcnt[i0])/2560000000.;
    char RFNStr[200];
    sprintf(RFNStr,"%d_%s_nHTH%dL%d.root",eventGapTcnt[i0],TimeStr,TriggerS,TriggerL);
    cout<<Form("%sGapT%s",DirOperate,RFNStr)<<" is opening"<<endl;
    TreeReader data(Form("%sGapT%s",DirOperate,RFNStr));
        
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
    
    float      Tr_The   ,  Tr_Phi;
    float      DxDz     ,  DyDz;
    float      Tr_ZX_Err,  Tr_ZY_Err;
    char       RelCode[30];
    float      DxDzPos, DyDzPos;
    float      AreaFac;
    int        NumbFac;
    bool       CodeEmp;
    
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
    vector<Int_t>    pwidth, pwidthk[NumLY];//pwidth0,pwidth1,pwidth2,pwidth3;
    vector<Int_t>    pwidthScaleFactor;
    vector<Int_t>    pcnt;
    vector<Long64_t> tcnt;
    vector<Double_t> dtime;
    vector<Double_t> ErrLs;
    
    //old Tree uint
    Int_t      nH ,nHk[NumLY];
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
    
    //take time and set anlyze Constant for boundry condition
    Long64_t evsTr = data.GetEntriesFast(), lastev = 0;
    cout <<"total event:\t"<<evsTr<<endl;
    if(testMode&&evsTr>3000) evsTr = evsTr*0.01;
    //Fill the data
    
    // for(int ir = 0; ir < 10 ; ir ++){
    for(int ir = 0; ir < totalRunNum ; ir ++){ 
      if(RunID[ir] == -1) continue;

      RunData      runDatatmp;
      map<int, scintillator> MScints = LoadEDectTable(vStrRunTillData[ir].data());
      TFile *ScinEfile = TFile::Open(vStrRunTillData[ir].data());
      TTree *tERPos = (TTree*) ScinEfile ->Get("tERPos");
      tERPos->SetBranchAddress("StartUT"     ,&runDatatmp.StartUT );
      tERPos->SetBranchAddress("EndUT"       ,&runDatatmp.EndUT   );
      tERPos->GetEntry(0);
      cout<<"StartUT : EndUT  "<<runDatatmp.StartUT<<"  "<<runDatatmp.EndUT<<endl;
      int rnum = 0; 
      for (Long64_t ev = lastev; ev <evsTr; ++ev) {//evsTr; ++ev) {
      // for (Long64_t ev = lastev; ev <10; ++ev) {//evsTr; ++ev) {
        data.GetEntry(ev);
        unixtime_ = data.GetLong64("unixtime");
        if(ev%1000 == 0) 
          cout<<Form("\r%.5f%%",(ev*100.)/(1.*evsTr))
              <<Form("   %d",unixtime_)
              <<Form("   %d",rnum)<<flush;
        
        if(!MCMode){
          if(unixtime_<runDatatmp.StartUT) continue;
          if(unixtime_>runDatatmp.EndUT){
            lastev = ev;
            break;
          } 
        }       
        O0++;
        nLayers_  = data.GetInt("nLayers");
        // if(nLayers_<TriggerTrNLayer) continue;
        if(nLayers_<4) continue;
        // if(nLayers_<2) continue;
        // if(nLayers_<trigger) continue;
        O1++;
        nH        = data.GetInt("nH");
        if( nH > 6 ) continue;
        O2++;
        // if(nH<TriggerTraNHitS) continue;
        // if(nH<4) continue;
        // if(nH<trigger) continue;
        // cout<<nHk[0]<<"\t"<<nHk[1]<<"\t"<<nHk[2]<<"\t"<<nHk[3]<<"\n";
        nHk[0]       = data.GetInt("nH0");
        nHk[1]       = data.GetInt("nH1");
        // nHk[2]       = data.GetInt("nH2");
        // nHk[3]       = data.GetInt("nH3");
        // if( nHk[3] != 1 || nHk[2] != 1 ) continue;
        if( nHk[1] > 2 ) continue;
        if( nHk[0] > 2 ) continue;
        O3++;
        // if(nH>TriggerTraNHitL) continue;
        // if(nH>8) continue;
        // if(nH>trigger) continue;
        // nHk[0]       = data.GetInt("nH0");
        // nHk[1]       = data.GetInt("nH1");
        // if( nHk[1] > 2 || nHk[0] > 2 ) continue;
        nHk[2]       = data.GetInt("nH2");
        nHk[3]       = data.GetInt("nH3");
        // if( nHk[1] != 1 ) continue;
        // if( nHk[0] != 1 ) continue;
        // if( nHk[3] > 2 ) continue;
        // if( nHk[2] > 2 ) continue;
        if( nHk[3] != 1 ) continue;
        if( nHk[2] != 1 ) continue;
        // cout<<nHk[0]<<"\t"<<nHk[1]<<"\t"<<nHk[2]<<"\t"<<nHk[3]<<"\n";
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
        //@@ Obtain 3,5 layer track
        // const int ProbableComb = int(GIDOnLayk[0].size()*GIDOnLayk[1].size()*GIDOnLayk[2].size()*GIDOnLayk[3].size());
        vector<line3D*> vTrack3D;
        vector<int*> vGIDCase, viHitCase;
        int GIDCase[NumLY], iHitCase[NumLY];
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
        float BestErr = TriggerTrFitSigSizeX*10;
        int BestLineIndex = -1;
        for(int iCase=0;iCase<int(vTrack3D.size());iCase++){
          float Error =
            sqrt(
              pow(vTrack3D[iCase]->Sigzx,2)
             +pow(vTrack3D[iCase]->Sigzy,2)
            );
          ErrLs.push_back(Error);
          if(Error<BestErr){
            BestErr = Error;
            BestLineIndex = iCase;
          }
        }
        
        // cout<<265<<endl;
        frame_    = data.GetInt("frame");// cout<<frame_<<"\t";
        for(int ilay=0; ilay<NumLY;ilay++){
          GID = vGIDCase[BestLineIndex][ilay];
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
        if((Track3D->Sigzx<trigger)&&(Track3D->Sigzy<trigger)){
        // if(Track3D->Sigzx<TriggerTrFitSigSizeX||1){ 
          // if(Track3D->Sigzy<TriggerTrFitSigSizeY||1){ 
          TrackIndex++;
          // if((Track3D->Sigzx>TriggerTrFitSigSizeX/1.414||Track3D->Sigzy>TriggerTrFitSigSizeY/1.414)&&nLayers_==4)
            // Track3D->RunGraph(Form("%sTrackID%08.f.png",DirTfitG,TrackIndex*1.));
          
          

          // if(TrackIndex>50) throw;
          Tr_ZX_Err = Track3D->Sigzx;
          Tr_ZY_Err = Track3D->Sigzy;
          // cout<<Form("\n%10.2e%10.2e\n",Track3D->Sigzx,Track3D->Sigzy);
          // sleep(0.1);
          Tr_The = Track3D->Lfit3D->Dthe;
          Tr_Phi = Track3D->Lfit3D->Dphi;
          DxDz = Track3D->rzx;
          DyDz = Track3D->rzy;
          /*
          if ((abs(DyDz)>0.025&&abs(DyDz)<0.035)&&(abs(DxDz)>0.025&&abs(DxDz)<0.035))
            Track3D->RunGraph(Form("%sTrackID%08.f.png",DirTfitG,TrackIndex*1.));
          
          
          if (TrackIndex == 74915)
            Track3D->RunGraph(Form("%sTrackID%08.f.png",DirTfitG,TrackIndex*1.));
          */
          // Track3D->RunGraph(Form("%sTrackID%08.f.png",DirTfitG,TrackIndex*1.));

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
          // cout<<endl<<pos[0][0]<<pos[1][0]<<pos[2][0]<<pos[3][0]<<pos[0][1]<<pos[1][1]<<pos[2][1]<<pos[3][1]<<endl;
          
          sprintf(RelCode,"%s", (getRelCode(pos,NumLY)).data());
          DxDzPos = MSGAD[RelCode].DXZ;
          DyDzPos = MSGAD[RelCode].DYZ;
          AreaFac = MSGAD[RelCode].AreaFactor;
          NumbFac = MSGAD[RelCode].NumbFactor;
          CodeEmp = MSGAD[RelCode].Empty; if(!CodeEmp) O5++; 
          /* Load Frame Imformation */
          frame_    = data.GetInt("frame");
          EvIndex_  = data.GetInt("EvIndex");
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
          if(frame_==259109) cout<<endl<<viHitCase[BestLineIndex][0]<<"\t"<<viHitCase[BestLineIndex][1]<<"\t"<<viHitCase[BestLineIndex][2]<<"\t"<<viHitCase[BestLineIndex][3]<<"\n\n";

          for(int iHit = 0; iHit<nH ; iHit++){
            bool iHitAlive = false;
            for(int ilay=0 ; ilay<nLayers_ ; ilay++){
              if(GIDOfiHit[iHit] == vGIDCase[BestLineIndex][ilay]) iHitAlive = true;
            }
            if(iHitAlive) continue;
            GID = BCIDCovGID(board_[iHit], channel_[iHit]);
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
          
          // cout<<"HR: "<<tHour_<<endl;
          tree->Fill();
          
          pwidth.clear();
          for(int iLay=0; iLay<NumLY; iLay++) pwidthk[iLay].clear();
          pwidthScaleFactor.clear();
          pcnt.clear();
          tcnt.clear();
          dtime.clear();
          Effch.clear();
          EffWei = 1.;
          rnum++;
        }
        
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
        
      } cout<<"\rFinish Tracking of Events "<<endl;
      cout<<"Number of track in run "<<ir<<" :  "<<rnum<<endl;
    }
    ott<<DirOperate<<"\t"<<trigger<<"\t"<<O0<<"\t"<<O1<<"\t"<<O2<<"\t"<<O3<<"\t"<<O4<<"\t"<<O5<<endl;
    rotfile->Write();
    
    
  
  }
}


void  FitTrackInfoAna(const int indexGap=28){
  defstyle();
  char DirRoot[1][100] = {
    ""
  };
  sprintf(DirRoot[0],"%s",DirOperate);

  char DirSave[100] ;
  sprintf(DirSave,"%sFitTrackInfo/",DirRes_LFirTra);
  char cmdline[150];
  sprintf(cmdline,"mkdir -p %s",DirSave);
  if (system(cmdline)!=0) system(cmdline);
  
  char FileSave[200];
  sprintf(FileSave,"%sETracksGT%d_%s_nHTH%dL%d.root",DirOperate,eventGapTcnt[indexGap],TimeStr,TriggerS,TriggerL);

  
  TFile *_file0 = TFile::Open(FileSave);
  cout<<"Tracking result File: "<<FileSave<<" is Opening:  "<<endl;
  TTree *t  = (TTree*) _file0 ->Get("t");
  
  TCanvas *C = new TCanvas("Canvas","",1920,1080);
  C->cd();
  C->SetLogy(1);
  TH1F *hTrErrXZ = new TH1F("hTrErrXZ","Fitting Err(1-#sigma) on X-Z, Y-Z planes",500,0,100);
  TH1F *hTrErrYZ = new TH1F("hTrErrYZ","Fitting Err(1-#sigma) on X-Z, Y-Z planes",500,0,100);
  hTrErrXZ->SetLineColor(1);
  hTrErrYZ->SetLineColor(2);
  hTrErrXZ->SetStats(0);
  hTrErrYZ->SetStats(0);
  TLegend *Lerrzxy = new TLegend(0.6,0.65,.9,.9);
  Lerrzxy->SetFillColor(0);
  Lerrzxy->SetLineColor(1);
  t->Draw("Tr_ZX_Err>>hTrErrXZ","Tr_ZX_Err<100","box");
  t->Draw("Tr_ZY_Err>>hTrErrYZ","Tr_ZY_Err<100","boxsame");
  
  Lerrzxy->AddEntry(hTrErrXZ,Form("Track Err on Z-X plane"),"l");
  Lerrzxy->AddEntry(hTrErrYZ,Form("Track Err on Z-Y plane"),"l");
  Lerrzxy->AddEntry(hTrErrXZ,Form("Entries:%10.0f",hTrErrXZ->GetEntries()),"h");
  Lerrzxy->Draw("same");
  
  DrawPdfPng(C,Form("%sFittingErrZXZY",DirSave));
  
  C->cd();
  C->SetLogy(0);
  TH2F *hTrDXYZ = new TH2F("hTrDXYZ","Angular distribution",201,-1.400,1.400,201,-1.400,1.400);
  hTrDXYZ->GetXaxis()->SetRangeUser(-0.75,0.75);
  hTrDXYZ->GetYaxis()->SetRangeUser(-0.75,0.75);
  t->Draw("abs(DyDz):abs(DxDz)>>hTrDXYZ","nLayers==2&&nH==3","colz");
  DrawPdfPng(C,Form("%sFittingNL2NH3",DirSave));

  

}




