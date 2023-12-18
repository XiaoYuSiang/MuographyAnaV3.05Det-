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
#include <TFile.h>
#include <TTree.h>

const int  Num_ModVCase = 2;
const char str_ModVer[Num_ModVCase][10] = {"easy","gdml"};
const int  Num_PartCase = 2;
const char str_Patiko[Num_PartCase][10] = {"mu","ele"};
const char str_Patiko_title[Num_PartCase][10] = {"#mu^{-}","e^{-}"};
const int  Num_leadCase = 4;
const int  PBSet[Num_leadCase] = {0,1,11,1111};
const char str_PBSet[Num_leadCase][10] = {"0000","0001","0011","1111"};
const int  Num_MTBCase = 1;
const int  MTBSt[Num_MTBCase] = {11110};
const char str_MTBSet[Num_MTBCase][10] = {"11110"};
const int  Num_Selects = 6;
const int  SELay[Num_Selects] = {0,1,2,3,4,5};
const char str_SELays[Num_Selects][10] = {"BemTake","LesLayS","LarHitS","LarNH01","LarNH23","PosCodS"};
string caseTitle[Num_Selects] = {
  "EventAna.C","TrackAna.C:Lay>=4","TrackAna.C:nH<7",
  "EventAna.C:nH(0,1)<3","TrackAna.C:nH(2,3)==1","TrackAna.C:PosCode"
};

class MCCaseSet{
public:
  int ModVer, Patiko, PBSets, MTBSet;
  double Energy, Triger;
  int  BemTake, LesLayS, LarHitS, LarNH01, LarNH23, PosCodS;
  int  NScater, NShower;
  char SModVer[10], SPatiko[10], SPBSets[10], SMTBSet[10];
  MCCaseSet(){
    ModVer=0; Patiko=0; PBSets=0; MTBSet=0;
    Energy=0; Triger=0;
    BemTake=0; LesLayS=0; LarHitS=0; LarNH01=0; LarNH23=0; PosCodS=0;
    NScater=1; NShower=1;
  };
  ~MCCaseSet(){};
  void GetAttributeStrs(){
    sprintf(SModVer,"%s",str_ModVer[ModVer]);
    sprintf(SPatiko,"%s",str_Patiko_title[Patiko]);
    sprintf(SPBSets,"%s",str_PBSet[PBSets]);
    sprintf(SMTBSet,"%s",str_MTBSet[MTBSet]);
  }
  string Show(){
    string tempshowtext =
      Form("%s\t%s\t%s\t%s\t%.0E\t%.0f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
        str_ModVer[ModVer], str_Patiko[Patiko], str_PBSet[PBSets], str_MTBSet[MTBSet],
        Energy, Triger,
        BemTake, LesLayS, LarHitS, LarNH01, LarNH23, PosCodS,
        NScater, NShower
      );
    cout<<tempshowtext<<endl;
    return tempshowtext;
  }
  string ShowHeadLine(){
    return 
      "#ModVer\tPatiko\tPBSets\tMTBSet\tEnergy\tTriger\tBemTake\tLesLayS\tLarHitS\tLarNH01\tLarNH23\tPosCodSNScater\tNShower\n";
  }
  void SetCaseModeBranchInTTree(TTree *Obj_TTree){
    Obj_TTree->Branch("ModVer",&ModVer,"Model Version/I");
    Obj_TTree->Branch("SModVer",&SModVer,"String of Model Version/C");
    Obj_TTree->Branch("Patiko",&Patiko,"Particle Name/I");
    Obj_TTree->Branch("SPatiko",&SPatiko,"String of Particle Name/C");
    Obj_TTree->Branch("PBSets",&PBSets,"Lead Plate Setup/I");
    Obj_TTree->Branch("SPBSets",&SPBSets,"String of Lead Plate Setup/C");
    Obj_TTree->Branch("MTBSet",&MTBSet,"MTBLayersSet/I");
    Obj_TTree->Branch("SMTBSet",&SMTBSet,"String of MTBLayersSet/C");
    Obj_TTree->Branch("Energy",&Energy,"Energy Of Paeticle Beam(MeV)/D");
    Obj_TTree->Branch("Triger",&Triger,"Trigger value Of Selection/D");
  }
  void SetPassNumBranchInTTree(TTree *Obj_TTree){
    Obj_TTree->Branch("BemTake",&BemTake,"Beam Were Take by detector(nH>1)/I");
    Obj_TTree->Branch("LesLayS",&LesLayS,"NLayers > 3/I");
    Obj_TTree->Branch("LarHitS",&LarHitS,"nHit < 7/I");
    Obj_TTree->Branch("LarNH01",&LarNH01,"nH0&nH1 <3/I");
    Obj_TTree->Branch("LarNH23",&LarNH23,"nH2&nH3 <2/I");
    Obj_TTree->Branch("PosCodS",&PosCodS,"Passing PosCode Selection/I");
  }
  void SetScaShowBranchInTTree(TTree *Obj_TTree){
    // Obj_TTree->Branch("LarNH23",&LarNH23,"nH2&nH3 <2/I");
    // Obj_TTree->Branch("PosCodS",&PosCodS,"Passing PosCode Selection/I");
    // Obj_TTree->Branch("NScater",&NScater,"Number of Scattering case/I");
    // Obj_TTree->Branch("NShower",&NShower,"Number of Shower case/I");
  }
};