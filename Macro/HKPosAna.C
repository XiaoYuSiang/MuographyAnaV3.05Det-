#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <vector>
#if defined (__MAKECINT__) 
#pragma link C++ class vector<Long64_t>+; 
#endif

#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>


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


void SetStyleTemPos()
{
  static const int colorSlice=20;
  static int       colors[colorSlice];
  for(int  i=0;i<3;i++){
    colors[0+i] = ColorArr9[0];
    colors[3+i] = ColorArr9[1];
    colors[7+i] = ColorArr9[2];
    colors[10+i] = ColorArr9[3];
    colors[13+i] = ColorArr9[4];
    colors[17+i] = ColorArr9[8];
  }
    
    colors[6] = ColorArr9[1];
    colors[16] = ColorArr9[4];
  
  gStyle->SetPalette(colorSlice,colors);

  gStyle->SetPadRightMargin(0.15);
}

void SensorPosShow(){
  SetStyleTemPos();
  TFile *_file0 = TFile::Open("/data4/YuSiang/TEST/Ana/DetIMF/SciPosDatV03_05.root");
  TTree *tPot = (TTree*) _file0 ->Get("tPot");

  TCanvas *c = new TCanvas("c","c",1080,1080);

  TH2F *HpXpY = new TH2F("HpXpY","Thermal sensor Position",80,0,400,80,0,400);
  tPot->Draw("pY:pX>>HpXpY","(TChID)*(BiZ==0)+1","colz");
  
  
 TGaxis *axis7 = new TGaxis(400,0,400,400,0,6,6,"g");
   axis7->SetName("axis7");
   axis7->SetLabelOffset(-0.08);
   
   axis7->CenterLabels();

   
   axis7->Draw();
   
  HpXpY->SetStats(0);
  HpXpY->SetMarkerSize(2);
  HpXpY->GetZaxis()->SetRangeUser(4,9);
  HpXpY->GetXaxis()->SetTitle("Position on X (mm)");
  HpXpY->GetYaxis()->SetTitle("Position on Y (mm)");
  HpXpY->GetZaxis()->SetTitle("Sensor ID on MTB");
  HpXpY->GetXaxis()->CenterTitle(true);
  HpXpY->GetYaxis()->CenterTitle(true);
  HpXpY->GetZaxis()->CenterTitle(true);
  HpXpY->GetZaxis()->CenterLabels(true);
  HpXpY->GetZaxis()->RotateTitle(true);
  HpXpY->GetXaxis()->SetNdivisions(8);
  HpXpY->GetYaxis()->SetNdivisions(8);
  HpXpY->GetZaxis()->SetNdivisions(0);
  HpXpY->GetXaxis()->SetTitleOffset(1.5);
  HpXpY->GetYaxis()->SetTitleOffset(1.5);
  HpXpY->GetZaxis()->SetTitleOffset(1.0);
  HpXpY->GetXaxis()->SetLabelSize(0.025);
  HpXpY->GetYaxis()->SetLabelSize(0.025);
  HpXpY->GetZaxis()->SetLabelSize(0.03);
  HpXpY->GetZaxis()->SetLabelOffset(999);
  HpXpY->GetXaxis()->SetTitleSize(0.03);
  HpXpY->GetYaxis()->SetTitleSize(0.03);
  
  gStyle->SetPadRightMargin(0.15);
  DrawPdfPng(c ,Form("%sTemVPosXYZ",DirIMFdat));
  c->Close();

}

void TempVPosDataSort(){
  defstyle();
  map<int, tempSensor> MTSens = LoadODectTable_TSensor();
  
  char Path_file_HK[200];
  sprintf(Path_file_HK,"%sHK_%s.root",DirOperate,TimeStr);
  cout<<"Opening the file:  "<<Path_file_HK<<endl;
  TFile *rotfileHK = TFile::Open(Form("%s",Path_file_HK));
  TTree *tHK = (TTree*) rotfileHK ->Get("tHK");
  HouseKeepingData HKD;
  HKD.SetHKTreeBranchAddress(tHK);
  
  TFile *outfile = new TFile(Form("%sPosHK_%s.root",DirOperate,TimeStr), "recreate");
  TTree *tTP = new TTree("tTP", "tree of House keeping data and position for temperature");
  TTree *tHP = new TTree("tHP", "tree of House keeping data and position for Humidity");
  HouseKeepingData HKDTmp;
  tempSensor       TSTmp;
  int   GeneralID;
  
  
  tTP->Branch("unixtime"  , &HKDTmp.unixtime);
  tTP->Branch("boardID"   , &HKDTmp.boardID );
  tTP->Branch("TChID"     , &TSTmp.TChID    );
  tTP->Branch("Temp"      , &HKDTmp.TempCen );
  tTP->Branch("BiX"       , &TSTmp.BiX      );
  tTP->Branch("BiY"       , &TSTmp.BiY      );
  tTP->Branch("BiZ"       , &TSTmp.BiZ      );
  tTP->Branch("pX"        , &TSTmp.pX       );
  tTP->Branch("pY"        , &TSTmp.pY       );
  tTP->Branch("pZ"        , &TSTmp.pZ       );
  tTP->Branch("face"      , &TSTmp.face     );
  
  tHP->Branch("unixtime"  , &HKDTmp.unixtime);
  tHP->Branch("boardID"   , &HKDTmp.boardID );
  tHP->Branch("HChID"     , &TSTmp.TChID    );
  tHP->Branch("Humi"      , &HKDTmp.Humidity);
  tHP->Branch("BiX"       , &TSTmp.BiX      );
  tHP->Branch("BiY"       , &TSTmp.BiY      );
  tHP->Branch("BiZ"       , &TSTmp.BiZ      );
  tHP->Branch("pX"        , &TSTmp.pX       );
  tHP->Branch("pY"        , &TSTmp.pY       );
  tHP->Branch("pZ"        , &TSTmp.pZ       );
  tHP->Branch("face"      , &TSTmp.face     );

  for(int ihk = 0 ; ihk < int(tHK->GetEntriesFast());ihk++){
    tHK->GetEntry(ihk);
    HKDTmp   = HKD;
    for(int itch = 0; itch<=5; itch++){
      TSTmp.TChID = itch;
      GeneralID   = TSTmp.TChID + 6*(HKD.boardID-1);
      TSTmp = tempSensor(MTSens[GeneralID]);
      switch(itch){
        case 0:
          HKDTmp.TempCen = HKD.TempFPG;
          break;
        case 1:
          HKDTmp.TempCen = HKD.Temps1;
          break;
        case 2:
          HKDTmp.TempCen = HKD.Temps2;
          break;
        case 3:
          HKDTmp.TempCen = HKD.Temps3;
          break;
        case 4:
          HKDTmp.TempCen = HKD.Temps4;
          break;
        case 5:
          HKDTmp.TempCen = HKD.TempCen;
          break;
      }
      if(HKDTmp.TempCen == -1) continue;
      tTP->Fill();
    }
    TSTmp.TChID = 0;
    tHP->Fill();
  }
  outfile->Write();
  cout<<outfile->GetName()<<" have created."<<endl;
  outfile->Close();
}


void TempVPosAna(){
  defstyle();
    
  TFile *PTHfile = new TFile(Form("%sPosHK_%s.root",DirOperate,TimeStr));
  TTree *tTP = (TTree*) PTHfile ->Get("tTP");
  TTree *tHP = (TTree*) PTHfile ->Get("tHP");
    
  char cmdline[250], DirTHVP[200];
  
  sprintf(DirTHVP,"%sTHVPosDetail/",DirRes_HousKep);
  sprintf(cmdline,"mkdir -p %s",DirTHVP);
  if (system(cmdline)!=0) system(cmdline);
    
  TCanvas *cA = new TCanvas("cA","",2*1920,2*1080);
  cA->Divide(1,2);
  cA->SetGrid(1,1);
  
  TH2F *HTpXpY, *HTFpXpY, *HTCpXpY, *HHpXpYm, *HHpXpY;
  HTpXpY = new TH2F(Form("HTpXpY") ,"", 4*4,0,400*4, 4,0,400);
  HTFpXpY= new TH2F(Form("HTFpXpY"),"",20*4,0,400*4,20,0,400);
  HTCpXpY= new TH2F(Form("HTCpXpY"),"", 4*4+1,0-50,400*4+50,5,0-50,400+50);
  HHpXpYm= new TH2F(Form("HHpXpYm"),"", 4*4,0,400*4, 4,0,400);
  HHpXpY = new TH2F(Form("HHpXpY") ,"", 4*4+1,0-50,400*4+50,5,0-50,400+50);
  TH2F *HAll[5] = {HTpXpY,HTFpXpY,HTCpXpY,HHpXpYm,HHpXpY};
  //for(int ibd=0; ibd<4; ibd++){
  for(int i=0; i<5; i++){
    HAll[i]->SetStats(0);
    HAll[i]->GetXaxis()->SetTitle("Position on X + iZ*400(mm)");
    HAll[i]->GetYaxis()->SetTitle("Position on Y (mm)");
    HAll[i]->GetXaxis()->CenterTitle(true);
    HAll[i]->GetYaxis()->CenterTitle(true);
    HAll[i]->GetZaxis()->CenterTitle(true);
    HAll[i]->GetZaxis()->RotateTitle(true);
    HAll[i]->GetXaxis()->SetNdivisions(16);
    HAll[i]->GetYaxis()->SetNdivisions(4);
    HAll[i]->GetXaxis()->SetTitleOffset(2.0);
    HAll[i]->GetYaxis()->SetTitleOffset(1.5);
    HAll[i]->GetZaxis()->SetTitleOffset(0.5);
    HAll[i]->GetXaxis()->SetLabelSize(0.025);
    HAll[i]->GetYaxis()->SetLabelSize(0.025);
    HAll[i]->GetZaxis()->SetLabelSize(0.03);
    HAll[i]->GetXaxis()->SetTitleSize(0.03);
    HAll[i]->GetYaxis()->SetTitleSize(0.03);
    HAll[i]->GetXaxis()->SetRangeUser(0,400*4);
    HAll[i]->GetYaxis()->SetRangeUser(0,400);
    
  }
  HTpXpY ->GetZaxis()->SetRangeUser(10,65);
  HTpXpY ->GetZaxis()->SetTitle("Temperature(#circC)");
  HTCpXpY->GetZaxis()->SetRangeUser(10,65);
  HTCpXpY->GetZaxis()->SetTitle("Temperature(#circC)");
  HTFpXpY->GetZaxis()->SetRangeUser(10,65);
  HTFpXpY->GetZaxis()->SetTitle("Temperature(#circC)");
  HHpXpYm->GetZaxis()->SetRangeUser(0,100);
  HHpXpYm->GetZaxis()->SetTitle("Humidity(RH%)");
  HHpXpY ->GetZaxis()->SetRangeUser(0,100);
  HHpXpY ->GetZaxis()->SetTitle("Humidity(RH%)");
  
      
  TH2F *HMod = new TH2F(Form("HMod") ,"", 4*4,0,400*4, 4,0,400);
  TH2F *HModB = new TH2F(Form("HModB") ,"", 8,1,400*4, 2,0,400);
  TH2F *HModBT= new TH2F(Form("HModBT"),"", 8,1,400*4, 2,0,400);
  for(int ix = 0; ix<16; ix++) HMod->GetXaxis()->SetBinLabel(ix+1, Form(" "));
  for(int iy = 0; iy< 4; iy++) HMod->GetYaxis()->SetBinLabel(iy+1, Form(" "));
  HMod->SetStats(0);
  HMod->GetXaxis()->SetTitleColor(4);
  HMod->GetYaxis()->SetTitleColor(4);
  HMod->GetXaxis()->SetLabelColor(4);
  HMod->GetYaxis()->SetLabelColor(4);
  
  for(int iz = 0; iz<4; iz++){
    for(int ixb = 0; ixb<2; ixb++){
      HModB->GetXaxis()->SetBinLabel(ixb+2*iz+1, Form("iX = %d, iZ = %d",ixb,iz));
      HModB->Fill((ixb+2*iz+.5)*200,100);
      HModB->Fill((ixb+2*iz+.5)*200,300);
      for(int iyb = 0; iyb<2; iyb++) HModBT->Fill((ixb+2*iz+.5)*200,100+200*iyb,2*(1-iyb)+ixb+iz*4+1);
    }
  } 
  HModB->GetYaxis()->SetBinLabel(1, Form("iY = 0"));
  HModB->GetYaxis()->SetBinLabel(2, Form("iY = 1"));
  HModB->SetStats(0);
  HModB->GetXaxis()->SetTitleColor(2);
  HModB->GetXaxis()->SetLabelColor(2);
  HModB->GetXaxis()->SetLabelOffset(0.02);
  HModB->GetYaxis()->SetLabelOffset(0.015);
  HModB->GetYaxis()->SetTitleColor(2);
  HModB->GetYaxis()->SetLabelColor(2);
  HModB->SetMarkerColor(2);
  HModB->SetLineColor(2);
  HModB->SetLineStyle(2);
  HModB->GetZaxis()->SetRangeUser(0,1);
  
  HModBT->SetMarkerSize(4);
      
  Long64_t unixtime, unixtimetmp, dunixtime;
  int tYear_, tMonth_, tDate_, tHour_, tMinute_, tSecond_;
  int boardID, TChID, HChID;
  int Temp, Humi;
  int BiX, BiY, BiZ;
  float pX, pY, pZ;
  bool face;
  
  tTP->SetBranchAddress("unixtime"  , &unixtime );
  tTP->SetBranchAddress("TChID"     , &TChID    );
  tTP->SetBranchAddress("BiZ"       , &BiZ      );
  tTP->SetBranchAddress("Temp"      , &Temp     );
  tTP->SetBranchAddress("pX"        , &pX       );
  tTP->SetBranchAddress("pY"        , &pY       );
  tTP->SetBranchAddress("pZ"        , &pZ       );
  
  tHP->SetBranchAddress("unixtime"  , &unixtime );
  tHP->SetBranchAddress("BiZ"       , &BiZ      );
  tHP->SetBranchAddress("Humi"      , &Humi     );
  tHP->SetBranchAddress("pX"        , &pX       );
  tHP->SetBranchAddress("pY"        , &pY       );
  tHP->SetBranchAddress("pZ"        , &pZ       );
  
  tHP->GetEntry(0);
  unixtimetmp = unixtime;
  dunixtime = unixtimetmp-unixtime;
  //cout<<tHP->GetEntriesFast()<<"  "<<unixtime<<"  "<<unixtimetmp<<endl;
  tTP->GetEntry(1);
  //cout<<tTP->GetEntriesFast()<<"  "<<unixtime<<"  "<<unixtimetmp<<endl;
  int  totNum = int(tHP->GetEntriesFast());
  int  pngInd = 0, iNslast = 0;
  bool istakingdata = true;
  for(int i = totNum*0.95; i< totNum; i++){
    tHP->GetEntry(i);
    dunixtime = unixtime-unixtimetmp;
    if(istakingdata){
      if(dunixtime==0){
        continue;
      }else{
        unixTimeToHumanReadable(unixtimetmp, tYear_, tMonth_, tDate_, tHour_, tMinute_, tSecond_,timeZone);
        char UtimeStr[150];
        char UtimeStrN[150];
        sprintf(UtimeStr,"%04d-%02d-%02d %02d:%02d:%02d(UTC+%02d)",
            tYear_, tMonth_, tDate_, tHour_, tMinute_, tSecond_,timeZone);
        sprintf(UtimeStrN,"%04d_%02d_%02d_%02d:%02d:%02d_UTC%02d",
            tYear_, tMonth_, tDate_, tHour_, tMinute_, tSecond_,timeZone);
        cout<<UtimeStr<<endl;
        unixtimetmp = unixtime ;

        cA->cd(1);
        HTpXpY->SetTitle(Form("Temperature @%s",UtimeStr));
        tTP->Draw("pY:pX+400*BiZ>>HTpXpY" ,Form("Temp*((unixtime==%d)&&(TChID>0&&TChID<5))",unixtimetmp),"colz");
        tTP->Draw("pY:pX+400*BiZ>>HTFpXpY",Form("Temp*((unixtime==%d)&&TChID==0)",unixtimetmp),"colzsame");
        tTP->Draw("pY:pX+400*BiZ>>HTCpXpY",Form("Temp*((unixtime==%d)&&TChID==5)",unixtimetmp),"colzsame");
        
        TPad pad1("pad1","",0,0,1,1);
        SetTransparentPadcd(pad1);
        HMod->Draw("col");

        TPad padB1("padB1","",0,0,1,1);
        SetTransparentPadcd(padB1);
        HModB->Draw("box");
        HModBT->Draw("textsame");
        
        cA->cd(2);
        HHpXpYm->SetTitle(Form("Humidity @%s",UtimeStr));
        HHpXpYm->Draw("colz");
        tHP->Draw("pY:pX+400*BiZ>>HHpXpY", Form("Humi*(unixtime==%d)",unixtimetmp),"colzsame");
        
        TPad pad2("pad2","",0,0,1,1);
        SetTransparentPadcd(pad2);
        HMod->Draw("col");
        
        TPad padB2("padB2","",0,0,1,1);
        SetTransparentPadcd(padB2);
        HModB->Draw("box");
        HModBT->Draw("textsame");

        cA->Print(Form("%sTHVPos%s.png",DirTHVP,UtimeStrN));      //if(i>10) throw;
        pngInd++;
        HTpXpY ->Reset();
        HTCpXpY->Reset();
        HHpXpY ->Reset();
        
        cout<<Form("\r%.6f",i*1./(1.*totNum)*100)<<flush;
        istakingdata = false;
        //throw;
      }
    }
    else if(dunixtime/TGapTHVPos<=0) continue;
    else{
      istakingdata = true;
      unixtimetmp = unixtime;
      i--;
    }
  }
  //cA->Print(Form("/data4/YuSiang/TEST/Ana/HouseKeep/test.gif++"));
    
  
}

void HKPosAna(){
  SensorPosShow();
  TempVPosDataSort();
  TempVPosAna();
}