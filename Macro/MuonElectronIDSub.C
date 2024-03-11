#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TText.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <THStack.h>
#include "/data4/YuSiang/personalLib/Style/DBMTStyle.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"
#include "DSLData.h"
#include "LoadODectTable.h"

#if defined (__MAKECINT__) 
  #pragma link C++ class vector<Long64_t>+; 
#endif

using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace DataConst;

void MEIdentify(const int nL, const int nH, bool &MID, bool &EID, bool &NID, short &tight){
  if(nL==4){
    if(nH==4){
      MID =true;
      tight =5;
    }else if(nH==5){
      MID =true;
      tight =4;
    }else if(nH==6){
      MID =true;
      tight =3;
    }else if(nH==7){
      MID =true;
      tight =2;
    }
    if(nH>4){
      EID =true;
      tight =2;
      if(tight==0) tight =1;
    }
    
  }else if(nL==3){
    if(nH==3){
      MID =true;
      tight =4;
    }else if(nH==4){
      MID =true;
      tight =3;
    }else if(nH==5){
      MID =true;
      tight =2;
    }
    if(nH>3){
      EID =true;
      if(tight==0) tight =1;
    }
  }
  /*else if(nL==1){
    if(nH==4){
      MID =true;
      tight =4;
    }else if(nH==3){
      MID =true;
      tight =3;
    }else if(nH==2){
      MID =true;
      tight =2;
    }
    if(nH>4){
      EID =true;
      if(tight==0) tight =1;
    }
  }*/
  else{
    NID=true;
  }
}
void MEIdentify(MuEleIDData &MEIDData){
  MEIdentify(
    MEIDData.nLayers, MEIDData.nH, MEIDData.MuonID,
    MEIDData.ElectronID, MEIDData.NoiseID, MEIDData.IDtight
  );
}

void MuonElectronIDSub(const char *RFNStr,const char *TestMarker, const int ChipIndex=0){
  map<int, scintillator> MScints = LoadODectTable();

  vector<Long64_t> VNegUTRangeStart, VNegUTRangeFinal;
  GetNegHourRange(VNegUTRangeStart, VNegUTRangeFinal, DirOperate);
  char DirResultPwVCase[150]={};//save path graph of Pwidth Rate
  sprintf(DirResultPwVCase,Form("%s%sBoardPwVCase/",DirResult,TestMarker));
  if (system(Form("mkdir -p %s",DirResultPwVCase))!=0) system(Form("mkdir -p %s",DirResultPwVCase));
  
  string str_InputData = Form("%sGapT%s.root",DirOperate,RFNStr);
  string str_MEIDData  = Form("%s%sEventsGT%s_t%02.f.root"
    , DirOperate,TestMarker,RFNStr,ChipIndex*1.);
  cout<<"ID the event in the str_InputData:  "<<str_InputData<<endl;
  cout<<"To be str_MEIDData:  "<<str_MEIDData<<endl;
  TreeReader data(str_InputData.data());
  cout<<str_InputData.data()<<" is opened"<<endl;
  
  Long64_t evsVT = data.GetEntriesFast();
  if(strcmp(TestMarker,"TST_")==0) evsVT *= 0.01;
  int evChipNum = evsVT/cpuCores;
  int evRangeMin = ChipIndex*evChipNum;
  int evRangeMax = ChipIndex==cpuCores-1 ? evsVT :(ChipIndex+1)*evChipNum;

  TFile *rotfile = new TFile(str_MEIDData.data(),"RECREATE");
  cout<<str_MEIDData.data()<<" is recreated"<<endl;
  
  cout <<"total event:\t"<<evsVT<<endl 
       <<"  Event Chip number:\t"<<evChipNum<<endl
       <<"    Event Min ID:\t"<<evRangeMin<<endl
       <<"    Event Max ID:\t"<<evRangeMax<<endl;
  
  MuEleIDData MEIDData(0);
  TTree *tree = new TTree("t","data from analyzing file");
  MEIDData.SetTreeBranch(tree);
  int iCore = ChipIndex;
  tree->Branch("iCore" ,&iCore);
  int Index = 0;
  for (Long64_t ev = evRangeMin; ev <evRangeMax; ++ev) {//evsID; ++ev) {
    MEIDData.GetTreeReaderTGap(data,ev);
    if(InNegHourRange(MEIDData.unixtime,VNegUTRangeStart,VNegUTRangeFinal)) continue;
    if(ev%1000 == 0) cout<<Form("\r%.5f%%",(ev*100.)/(1.*evChipNum))<<flush;
    int nH = MEIDData.nH;
    MEIdentify( MEIDData );
    for(int i2=0 ; i2<nH ; i2++){
      int GID = BCIDCovGID(MEIDData.board_[i2], MEIDData.channel_[i2]);
      MEIDData.board   .push_back(MEIDData.board_[i2]); 
      MEIDData.channel .push_back(MEIDData.channel_[i2]);
      MEIDData.iX      .push_back(MScints[GID].iX); 
      MEIDData.iY      .push_back(MScints[GID].iY); 
      MEIDData.iZ      .push_back(MScints[GID].iZ);
      MEIDData.pcnt    .push_back(MEIDData.pcnt_[i2]);
      MEIDData.tcnt    .push_back(MEIDData.tcnt_[i2]);
      MEIDData.dtime   .push_back(MEIDData.dtime_[i2]);
      MEIDData.dtimeInt.push_back(MEIDData.dtimeInt_[i2]);
      MEIDData.pwidth  .push_back(MEIDData.pwidth_[i2]);
      MEIDData.pwidthScaleFactor
                       .push_back(MEIDData.pwidthScaleFactor_[i2]);
      
    }
    tree->Fill();
    Index++;
    MEIDData.Clear();
    MEIDData.TrackIndex = Index;
  }
  rotfile->Write();
  cout<<"\n"<<"Finished Ntuple write"<<endl;
  rotfile->Close();
}

void MuEleIDSubHAdd(const char *RFNStr,const char *TestMarker, const string MEIDFile){
  TFile *rotfile = new TFile(MEIDFile.data(),"RECREATE");
  ofstream outLogs(Form("%sLog_%sEventsGT%s.dat",DirOperate,TestMarker,RFNStr));
  outLogs<<MEIDFile.data()<<" is created"<<endl;
  TTree *tree = new TTree("t","data from analyzing file");
  MuEleIDData MEIDData(0);
  MEIDData.SetTreeBranch(tree);
  int lastTrIndex = 0;
  vector<string> vTmpSubFilesStrs;
  for(int iCore = 0; iCore<cpuCores; iCore++){
    outLogs<<Form("\nProcess of index/Total Files: %02.f/%02.f\n",iCore*1.+1.,cpuCores*1.);

    string MEIDDataSub = Form("%s%sEventsGT%s_t%02.f.root",
      DirOperate,TestMarker,RFNStr,iCore*1.);
      
    vTmpSubFilesStrs.push_back(MEIDDataSub);
    TreeReader data(MEIDDataSub.data());
    int NumTrInChip = data.GetEntriesFast();
    for (Long64_t ev = 0; ev <NumTrInChip; ++ev) {
      if(ev%1000 == 0) outLogs<<Form("\r%.5f%%",(ev*100.)/(1.*NumTrInChip))<<flush;
      MEIDData.GetTreeReaderMEID(data, ev);
      MEIDData.TReaderPtrToVec();
      MEIDData.TrackIndex += lastTrIndex;
      tree->Fill();
      MEIDData.Clear();
    }
    lastTrIndex += NumTrInChip;
  }
  rotfile->Write();
  rotfile->Close();
  outLogs<<"\n"<<"Finished Ntuple write"<<endl;
  for(int i = 0; i<int(vTmpSubFilesStrs.size()); i++){
    system(Form("rm %s",vTmpSubFilesStrs[i].data()));
  }
}

void MuEleIDSubHAdd_easy(const char *RFNStr,const char *TestMarker, const string MEIDFile){
  string MEIDDataSubs = Form("%s%sEventsGT%s_t*.root",DirOperate,TestMarker,RFNStr);
  string haddcmdline = "hadd -f "+MEIDFile+" "+MEIDDataSubs;
  string RMcmdline = "rm "+MEIDDataSubs;
  system(haddcmdline.data());
  system(RMcmdline.data());
}

void MuEleIDMutiThread(const char *RFNStr,const char *TestMarker){
  bool RealHADD = false;
  string MEIDFileName = Form("%stmp_MEID_mutiBash.sh", DirOperate);
  ofstream MutiCoreBash(MEIDFileName.data());
  cout<<"Create muti process bash file: "<<MEIDFileName.data()<<endl;
  
  for(int iCore = 0; iCore<cpuCores; iCore++){
    int timeDelay = iCore==0?0:7;
    MutiCoreBash<<"sleep "<<timeDelay<<" &&root -l -b "
      <<DirMacros<<"MuonElectronIDSub.C+"<<"\\\("
        <<"\\\""<<RFNStr<<"\\\""<<","
        <<"\\\""<<TestMarker<<"\\\""<<","
        <<iCore
      <<"\\"<<") &\n";
  }
  MutiCoreBash<<"wait\n";
  MutiCoreBash<<endl<<"echo Finish all tracking by muti process."<<endl;
  MutiCoreBash.close();

  system(Form("sh %s",MEIDFileName.data()));
  // FATAL("264");
  // cout<<"\nFinish all process & rm: "<<MEIDFileName.data()<<endl;
  // system(Form("rm %s",MEIDFileName.data()));

  string MEIDFile = Form("%s%sEventsGT%s.root",DirOperate,TestMarker,RFNStr);
  cout<<"Merge the files to be: "<<MEIDFile.data()<<endl;
  cout<<"Save the process log file: "
    <<Form("%sLog_%sEventsGT%s.dat",DirOperate,TestMarker,RFNStr)<<endl;
  
  if(RealHADD) MuEleIDSubHAdd(RFNStr,TestMarker,MEIDFile);
  else MuEleIDSubHAdd_easy(RFNStr,TestMarker,MEIDFile);
  cout<<"Finish the files marge and rm the tmp datas\n";
}
