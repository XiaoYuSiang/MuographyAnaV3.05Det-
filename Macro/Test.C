#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <algorithm>

string fileName[150] = {
  "/data4/YuSiang/DAXIFIX/rootfile1/20220830_Run0_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220831_Run0_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220901_Run0_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220902_Run0_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220903_Run0_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220904_Run0_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220905_Run0_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220906_Run0_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220907_Run0_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220907_Run1_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220907_Run1_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220907_Run1_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220907_Run1_3_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220907_Run1_4_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220908_Run1_4_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220908_Run1_5_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220908_Run1_6_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220909_Run1_6_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220910_Run1_6_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220911_Run1_6_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220912_Run1_6_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220912_Run1_7_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220912_Run1_8_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220912_Run1_9_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220913_Run1_9_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220914_Run1_9_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220915_Run1_9_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220916_Run1_9_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220917_Run2_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220918_Run2_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220919_Run2_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220920_Run2_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220921_Run2_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220921_Run3_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220922_Run3_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220922_Run4_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220923_Run4_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220924_Run4_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220925_Run4_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220926_Run4_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220927_Run4_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220928_Run5_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220929_Run5_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20220930_Run6_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221001_Run6_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221002_Run6_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221003_Run6_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221003_Run6_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221003_Run7_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221004_Run7_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221005_Run7_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221005_Run7_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221006_Run7_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221007_Run7_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221008_Run7_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221009_Run7_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221010_Run7_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221011_Run7_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221012_Run7_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221012_Run8_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221013_Run8_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221014_Run8_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221015_Run8_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221016_Run8_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221017_Run8_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221018_Run8_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221019_Run8_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221019_Run8_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221019_Run8_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221020_Run8_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221021_Run8_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221022_Run8_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221023_Run8_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221024_Run8_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221025_Run8_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221026_Run8_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221027_Run8_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221027_Run8_3_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221027_Run8_4_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221028_Run8_4_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221029_Run8_4_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221030_Run8_4_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221031_Run8_4_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221101_Run8_4_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221102_Run8_4_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221103_Run8_4_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221103_Run9_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221104_Run9_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221105_Run9_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221106_Run9_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221107_Run9_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221108_Run9_0_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221108_Run9_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221109_Run9_1_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221109_Run9_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221110_Run9_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221111_Run9_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221112_Run9_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221113_Run9_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221114_Run9_2_Mu.root",
  "/data4/YuSiang/DAXIFIX/rootfile1/20221115_Run9_2_Mu.root"
};



void Test(){
  int fileNum = int(fileName.size());
  ofstream fileSave(Form("FrameLoss.dat"), ofstream::out | ofstream::app );
  fileSave<<"#FileName\tFrameS\tFrameF\tFrameNum\tDFrame\tEffciency"<<endl;

  for(int ifile = 0;ifile<fileNum; ifile++){
    TFile *_file0 = new TFile::Open(fileName[ifile]);
    fileSave<<fileName[ifile]<<"\t";

    int   frameS = 0, frameF = 0, FrameN = 0, FrameD = 0, frameT =0;
    float FrameE = 0;
    t->SetBranchAddress("frame",&frameT);
    FrameN = t->GetEntries();
    
    t->GetEntry(0);
    frameS = frameT;
    fileSave<<frameS<<"\t";
    
    t->GetEntry(FrameN-1);
    frameF = frameT;
    fileSave<<frameF<<"\t";
    
    fileSave<<FrameN<<"\t";
    
    FrameD = frameF - frameS;
    fileSave<<FrameD<<"\t";
    
    FrameE = (FrameN*1.)/(FrameD*1.);
    fileSave<<FrameE<<"\n";
    

  }
  
}