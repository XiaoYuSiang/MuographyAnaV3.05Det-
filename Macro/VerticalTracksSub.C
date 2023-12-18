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
// #include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
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

void VerticalTracksSub(const char *RFNStr,const bool testMode, const int ChipIndex=0){
  map<int, scintillator> MScints = LoadODectTable();
  
  string InputData = Form("%sGapT%s.root",DirOperate,RFNStr);
  string TrackData = Form("%sTracksGT%s_t%02.f.root",DirOperate,RFNStr,ChipIndex*1.);
  TreeReader data(InputData.data());
  cout<<InputData.data()<<" is opened"<<endl;
  
  Long64_t evsVT = data.GetEntriesFast();
  if(testMode) evsVT *= 0.01;
  int evChipNum = evsVT/cpuCores;
  int evRangeMin = ChipIndex*evChipNum;
  int evRangeMax = ChipIndex==cpuCores-1 ? evsVT :(ChipIndex+1)*evChipNum;

  cout <<"total event:\t"<<evsVT<<endl 
       <<"  Event Chip number:\t"<<evChipNum<<endl
       <<"    Event Min ID:\t"<<evRangeMin<<endl
       <<"    Event Max ID:\t"<<evRangeMax<<endl;
  
  
  TFile *rotfile = new TFile(TrackData.data(),"RECREATE");
  cout<<TrackData.data()<<" is created"<<endl;
  
  //new Tree unit and odl tree unit
  VertTrackData VTD(0);

  //old Tree uint
  
  TTree *tree = new TTree("t","data from analyzing file");
  VTD.SetTreeBranch(tree);
  
  // data.GetEntry(0);

  //Fill the data
  for (Long64_t ev = evRangeMin; ev <evRangeMax; ++ev) {//evsVT; ++ev) {
    VTD.GetTreeReaderTGap(data, ev);
    if(ev%1000 == 0)
      cout<<Form("\r%.5f%%\t%d",((ev-evRangeMin)*100.)/(1.*evChipNum),VTD.unixtime)<<flush;
    // cout<<endl<<88<<endl;
    if(VTD.eventHit<2 ) continue;
    if(VTD.eventHit>62) continue;
    Long64_t BoolnH = 0;
    short BoolnL = 0;
    for(int i2=0 ; i2<VTD.eventHit ; i2++){
      if ((BoolnH>>i2)&1) continue;
      int hitCHA = VTD.channel_[i2];
      int hitBDA = VTD.board_[i2];
      // cout<<hitBDA<<"\t"<<hitCHA<<endl;
      double hitdTC = VTD.dtime_[i2];
      int GID = BCIDCovGID(VTD.board_[i2], VTD.channel_[i2]);
      int LID = MScints[GID].iZ;
      VTD.board  .push_back(VTD.board_[i2]);
      VTD.channel.push_back(VTD.channel_[i2]);
      VTD.iX     .push_back(MScints[GID].iX);
      VTD.iY     .push_back(MScints[GID].iY);
      VTD.iZ     .push_back(LID);
      VTD.BiX    .push_back(MScints[GID].BiX);
      VTD.BiY    .push_back(MScints[GID].BiY);
      VTD.BiZ    .push_back(MScints[GID].BiZ);
      VTD.pwidth.push_back(VTD.pwidth_[i2]);
      VTD.pwidthScaleFactor.push_back(VTD.pwidthScaleFactor_[i2]);
      VTD.pcnt  .push_back(VTD.pcnt_[i2]);
      VTD.tcnt  .push_back(VTD.tcnt_[i2]);
      VTD.dtime .push_back(VTD.dtime_[i2]);
      BoolnH |= 1UL << i2;
      BoolnL |= 1UL << LID;
      for(int i3 = i2+1 ; i3<VTD.eventHit ; i3++){
        GID = BCIDCovGID(VTD.board_[i3], VTD.channel_[i3]);
        LID = MScints[GID].iZ;
        if ((BoolnH>>i3)&1) continue;
        if ((BoolnL>>LID)&1) continue;
        if ( VTD.channel_[i3]==hitCHA ){
          BoolnH |= 1UL << i3;
          BoolnL |= 1UL << LID;
          VTD.board   .push_back(VTD.board_[i3]);  
          VTD.channel .push_back(VTD.channel_[i3]);
          VTD.iX     .push_back(MScints[GID].iX);
          VTD.iY     .push_back(MScints[GID].iY);
          VTD.iZ     .push_back(MScints[GID].iZ);
          VTD.BiX    .push_back(MScints[GID].BiX);
          VTD.BiY    .push_back(MScints[GID].BiY);
          VTD.BiZ    .push_back(MScints[GID].BiZ);

          VTD.pcnt   .push_back(VTD.pcnt_[i3]);
          VTD.tcnt   .push_back(VTD.tcnt_[i3]);
          VTD.dtime  .push_back(VTD.dtime_[i3]);
          VTD.pwidth .push_back(VTD.pwidth_[i3]);
          VTD.pwidthScaleFactor.push_back(VTD.pwidthScaleFactor_[i3]);
          VTD.BLay = BoolnL;
        }
      }

      if( BLayCheck(VTD.BLay, VTD.iHit ) ){//return BLay and iHit

        VTD.TrackIndex ++;
        tree->Fill();
      }
      BoolnL =0;
      VTD.Clear();

    }
  }

  rotfile->Write();
  cout<<"\r"<<"Finished Ntuple write"<<endl;
  rotfile->Close();
  
}

void VerticalTracksSubHAdd(const char *RFNStr, const string VTAFile){
  TFile *rotfile = new TFile(VTAFile.data(),"RECREATE");
  cout<<VTAFile.data()<<" is created"<<endl;
  TTree *tree = new TTree("t","data from analyzing file");
  VertTrackData VTD(0);
  VTD.SetTreeBranch(tree);
  int lastTrIndex = 0;
  vector<string> vTmpSubFilesStrs;
  for(int iCore = 0; iCore<cpuCores; iCore++){
    string TrackDataSub = Form("%sTracksGT%s_t%02.f.root",DirOperate,RFNStr,iCore*1.);
    vTmpSubFilesStrs.push_back(TrackDataSub);
    TreeReader data(TrackDataSub.data());
    int NumTrInChip = data.GetEntriesFast();
    for (Long64_t ev = 0; ev <NumTrInChip; ++ev) {
      VTD.GetTreeReaderVTrack(data, ev);
      VTD.TReaderPtrToVec();
      VTD.TrackIndex += lastTrIndex;
      tree->Fill();
      VTD.Clear();
    }
    lastTrIndex += NumTrInChip;
  }
  rotfile->Write();
  rotfile->Close();
  cout<<"\n"<<"Finished Ntuple write"<<endl;
  for(int i = 0; i<int(vTmpSubFilesStrs.size()); i++){
    system(Form("rm %s",vTmpSubFilesStrs[i].data()));
  }
}
void VerticalTracksSubControl(const char *RFNStr,const bool testMode){
  string VTFileName = Form("%stmp_VTA_mutiBash.sh", DirOperate);
  ofstream MutiCoreBash(VTFileName.data());
  cout<<"Create muti process bash file: "<<VTFileName.data()<<endl;
  
  for(int iCore = 0; iCore<cpuCores; iCore++){
    int timeDelay = iCore==0?0:7;
    // MutiCoreBash<<VerticalTracksSub( RFNStr, testMode, iCore);
    MutiCoreBash<<"sleep "<<timeDelay<<" &&root -l -b "
      <<DirMacros<<"VerticalTracksSub.C+"
        <<"\\\("
          <<"\\\""<<RFNStr<<"\\\""<<","
          <<testMode<<","
          <<iCore
        <<"\\"<<") &\n";
  }
  MutiCoreBash<<"wait\n";
  MutiCoreBash<<endl<<"echo Finish all tracking by muti process."<<endl;
  MutiCoreBash.close();

  system(Form("sh %s",VTFileName.data()));
  // FATAL("264");
  // cout<<"\nFinish all process & rm: "<<VTFileName.data()<<endl;
  // system(Form("rm %s",VTFileName.data()));

  string VTAFile = Form("%sTracksGT%s.root",DirOperate,RFNStr);
  cout<<"Merge the files to be: "<<VTAFile.data()<<endl;
  
  VerticalTracksSubHAdd(RFNStr,VTAFile);
  cout<<"Finish the files marge and rm the tmp datas\n";

}