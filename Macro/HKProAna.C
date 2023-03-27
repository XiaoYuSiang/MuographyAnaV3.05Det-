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




void SortHKData(TGraphErrors *TGp, int bkt = breaktime){
  
  vector<Double_t> Vx, Vy, Vex, Vey, Vxtmp, Vytmp;
  Double_t binTtmp, tolx, toly, lXtmp, pointcount=0;
  
  //cout<<39<<endl;
  for(int iN = 0 ; iN<TGp->GetN() ; iN++){
    Double_t tx, ty, tex = TGp->GetErrorX(iN);
    TGp->GetPoint(iN, tx, ty);
    
    
    tolx += tx;
    toly += ty;
    
    pointcount++;
    binTtmp = tx - lXtmp;
       // cout<<50<<"    "<<binTtmp<<"\t"<<tolx<<"\t"<<toly<<endl;
    if(binTtmp>=bkt){
      
      lXtmp   = tx;
      tolx   /= pointcount;
      toly   /= pointcount;
      // cout<<binTtmp<<"\t"<<tolx<<"\t"<<toly<<endl;
      Vx.push_back(tolx);
      Vy.push_back(toly);
      Vex.push_back(bkt/2.);
      Vey.push_back(1);
      // cout<<61<<"    "<<Form("%10.f",tolx)<<"\t"<<toly<<"\t"<<bkt/2.<<"\t"<<1<<endl;
      tolx = 0;
      toly = 0;
      binTtmp = 0;
      pointcount = 0;
    }
    
  }//cout<<68<<endl;
  // delete TGp;
  TGp = new TGraphErrors(Vx.size(),Vx.data(),Vy.data(),Vex.data(),Vey.data());
//cout<<71<<endl;
}


void HKProAna() {
  cout<<"Please use the new version: HKProAnaV2"<<endl;
  throw;

   
}


void HKProAnaV2() {
  defstyle();

  ofstream outFT(Form("%sHK_%s_Tem.dat",DirOperate,TimeStr));
  ofstream outFD(Form("%sHK_%s_Dir.dat",DirOperate,TimeStr));
  ofstream outFA(Form("%sHK_%s_Acc.dat",DirOperate,TimeStr));
  ofstream outFH(Form("%sHK_%s_Hum.dat",DirOperate,TimeStr));
  
  
  TFile *outfile = new TFile(Form("%sHK_%s.root",DirOperate,TimeStr), "recreate");

  TTree *tHK = new TTree("tHK", "tree of House keeping data");

  char HKFileList[200];
  sprintf(HKFileList, "%sHKTxtNameAna.dat",DirOperate);
  ifstream infilename(HKFileList);
  
  HouseKeepingData HKD;
  HKD.SetHKTreeBranch(tHK);
  int boan,tn;
  char filename[100];
  for(int i=0;infilename>>filename;i++){
    ifstream in(filename);
    Long64_t Lut[NumBD]={};
    
    while(in.good()){
      string Tmp_str;
      getline(in, Tmp_str);
      std::istringstream iss(Tmp_str);
      // cout<<Tmp_str<<endl;
      int stmp;
      Long64_t UT;
      // cout<<79<<endl;
      iss>>stmp>>HKD.boardID>>stmp>>stmp>>stmp>>stmp>>stmp;
      iss>>HKD.TempCen>>HKD.Humidity;
      
      iss>>HKD.Temps1>>HKD.Temps2>>HKD.Temps3>>HKD.Temps4;
      iss>>HKD.degX>>HKD.degY>>HKD.degZ;
      iss>>HKD.accX>>HKD.accY>>HKD.accZ;
      iss>>HKD.unixtime;
      
      if((tn==HKD.unixtime&&boan==HKD.boardID)||HKD.TempCen<1||tn==HKD.unixtime<TriggerUTLeast) continue;
      boan=HKD.boardID; tn=HKD.unixtime;
      int bdtmp = BDcheck(boan);
      HKD.dunixtime = Lut[bdtmp]==0 ? 60 : tn-Lut[bdtmp];
      Lut[bdtmp] = HKD.unixtime;
      if(HKD.dunixtime==0) continue;
      HKD.HK_Tem_TxtOuput(outFT);
      HKD.HK_Deg_TxtOuput(outFD);
      HKD.HK_Acc_TxtOuput(outFA);
      HKD.HK_Hum_TxtOuput(outFH);
      tHK->Fill();
    }
    in.close();
  }
  outFT.close();
  outFD.close();
  outFA.close();
  outFH.close();
  cout<<"House keeping data is save to be:  "<<endl<<Form("%sHK_%s_*.dat",DirOperate,TimeStr)<<endl;
  outfile->Write();
  outfile->Close();
  cout<<"House keeping data is save to be:  "<<endl<<Form("%sHK_%s.root",DirOperate,TimeStr)<<endl;
}

void HKProAnaV3() {
  defstyle();

  ofstream outFT(Form("%sHK_%s_Tem.dat",DirOperate,TimeStr));
  ofstream outFH(Form("%sHK_%s_Hum.dat",DirOperate,TimeStr));
  ofstream outFP(Form("%sHK_%s_Pre.dat",DirOperate,TimeStr));
  ofstream outFD(Form("%sHK_%s_Dir.dat",DirOperate,TimeStr));
  ofstream outFA(Form("%sHK_%s_Acc.dat",DirOperate,TimeStr));
  ofstream outFM(Form("%sHK_%s_Mag.dat",DirOperate,TimeStr));
  
  
  TFile *outfile = new TFile(Form("%sHK_%s.root",DirOperate,TimeStr), "recreate");

  TTree *tHK = new TTree("tHK", "tree of House keeping data");

  char HKFileList[200];
  sprintf(HKFileList, "%sHKTxtNameAna.dat",DirOperate);
  ifstream infilename(HKFileList);
  
  vector<TString> filename;
  int Numfiles = 0;
  for(char fileNameTmp[200];infilename>>fileNameTmp;) filename.push_back(fileNameTmp);
  Numfiles = int (filename.size());
  infilename.close();
  HouseKeepingData HKD;
  HKD.SetHKTreeBranch(tHK);
  int boan,tn;
  for(int i=0;i<Numfiles; i++){
    cout<<Form("\r%5.f/%5.f",(i+1)*1.,Numfiles*1.)<<flush;
    ifstream in(filename[i].Data());
    ifstream intmp(filename[i].Data());
    string Tmp_str;
    getline(intmp, Tmp_str);
    std::istringstream iss(Tmp_str);
    int Num_Hkterm = 0;
    Long64_t stmp, cnts[4]={}, pcntOff;
    while(iss>>stmp){
      if(Num_Hkterm==3) cnts[0] = stmp;
      Num_Hkterm++;
    } 
    pcntOff = stmp-cnts[0];
    
    intmp.close();
    Long64_t Lut[NumBD]={};
    
    while(in.good()){
      getline(in, Tmp_str);
      std::istringstream iss(Tmp_str);
      // cout<<Tmp_str<<endl;
      Long64_t UT;
      if(Num_Hkterm<=21){
        iss>>stmp>>HKD.boardID>>stmp;
        iss>>cnts[0]>>cnts[1]>>cnts[2]>>cnts[3];
        iss>>HKD.TempCen>>HKD.Humidity;
        iss>>HKD.Temps1>>HKD.Temps2>>HKD.Temps3>>HKD.Temps4;
        iss>>HKD.degX>>HKD.degY>>HKD.degZ;
        iss>>HKD.accX>>HKD.accY>>HKD.accZ;
        iss>>HKD.unixtime;
        if((HKD.unixtime-cnts[0])!=pcntOff) HKD.unixtime = cnts[0]+pcntOff;
      }else{
        iss>>stmp>>HKD.boardID>>stmp;
        iss>>cnts[0]>>cnts[1]>>cnts[2]>>cnts[3];
        iss>>HKD.TempCen>>HKD.Humidity;
        iss>>HKD.Temps1>>HKD.Temps2>>HKD.Temps3>>HKD.Temps4;
        iss>>HKD.Pressure>>HKD.Altitude>>HKD.TempFPG;
        iss>>HKD.degX>>HKD.degY>>HKD.degZ;
        iss>>HKD.accX>>HKD.accY>>HKD.accZ;
        iss>>HKD.magX>>HKD.magY>>HKD.magZ;
        iss>>stmp>>stmp>>HKD.unixtime;
        if((HKD.unixtime-cnts[0])!=pcntOff) HKD.unixtime = cnts[0]+pcntOff;
      }
      
      if((tn==HKD.unixtime&&boan==HKD.boardID)||HKD.TempCen<1||tn==HKD.unixtime<TriggerUTLeast) continue;
      boan=HKD.boardID; tn=HKD.unixtime;
      int bdtmp = BDcheck(boan);
      HKD.dunixtime = Lut[bdtmp]==0 ? 60 : tn-Lut[bdtmp];
      Lut[bdtmp] = HKD.unixtime;
      if(HKD.dunixtime==0) continue;
      if(Num_Hkterm<=21){
        HKD.HK_Tem_TxtOuput(outFT);
        HKD.HK_Hum_TxtOuput(outFH);
        HKD.HK_Deg_TxtOuput(outFD);
        HKD.HK_Acc_TxtOuput(outFA);
      }else{
        HKD.HK_Pre_TxtOuput(outFP);
        HKD.HK_Mag_TxtOuput(outFM);
      }
      tHK->Fill();
    }
    in.close();
  }
  outFT.close();
  outFH.close();
  outFP.close();
  outFD.close();
  outFA.close();
  outFM.close();
  cout<<"\nHouse keeping data is save to be:  "<<endl<<Form("%sHK_%s_*.dat",DirOperate,TimeStr)<<endl;
  outfile->Write();
  outfile->Close();
  cout<<"House keeping data is save to be:  "<<endl<<outfile->GetName()<<endl;
}

void SetTH2FXVTMod(TH2F *TH2FObj, const char *Title){
  TH2FObj -> GetZaxis()->SetTitle("times");
  TH2FObj -> GetZaxis()->SetLabelSize(0.02);
  TH2FObj -> GetZaxis()->SetTitleOffset(0.9);
  TH2FObj -> GetZaxis()->SetNdivisions(-005);
  TH2FObj -> SetTitle(Title); 
}



void HKAnaV2() {
  defstyle();
  char Path_file_HK[200];
  sprintf(Path_file_HK,"%sHK_%s.root",DirOperate,TimeStr);
  cout<<"Opening the file:  "<<Path_file_HK<<endl;
  TFile *rotfileHK = TFile::Open(Form("%s",Path_file_HK));
  TTree *tHK = (TTree*) rotfileHK ->Get("tHK");
  HouseKeepingData HKD;
  HKD.SetHKTreeBranchAddress(tHK);

  int Bins_breaktT = int ((FinDaySec-FirDaySec)/breaktime);

  TH2F *hHVT [NumBD], *hPVT [NumBD], * hAVT[NumBD];
  TH2F *hTFVT[NumBD], *hTCVT[NumBD], *hTNVT[4][NumBD];
  TH2F *hDeVT[3][NumBD], *hAcVT[3][NumBD], *hMaVT[3][NumBD];
  TGraphErrors *TDayGrid = Mod_DateGrid(50,200);
  
  TCanvas *ctmp = new TCanvas("ctmp","",100,100);
  ctmp->cd();
  for(int i0=0;i0<NumBD;i0++){ 
    for(int i1=0;i1<4;i1++){
      hTNVT[i1][i0] = new TH2F(Form("hT%dVT%d",i1+1,BD[i0]),"",Bins_breaktT,FirDaySec,FinDaySec,70,0,70); 
      tHK->Draw(Form("Temps%d:unixtime>>%s",i1+1,Form("hT%dVT%d",i1+1,BD[i0])),Form("boardID==%d",BD[i0]),"colz");
      SetTH2FXVTMod(hTNVT[i1][i0],Form("Time Vs Temperature on MTB%02.f(sensor T%d)",BD[i0]*1.,i1+1));
      hTNVT[i1][i0] -> GetZaxis()->SetTitle("Temperature(#circC)");
    }
    hTCVT[i0] = new TH2F(Form("hTCVT%d",BD[i0]) ,"",Bins_breaktT,FirDaySec,FinDaySec, 70,10, 70);
    hTFVT[i0] = new TH2F(Form("hTFVT%d",BD[i0]) ,"",Bins_breaktT,FirDaySec,FinDaySec, 70,10, 70);
    hHVT [i0] = new TH2F(Form("hHVT%d",BD[i0])  ,"",Bins_breaktT,FirDaySec,FinDaySec,100, 0,100);
    tHK->Draw(Form("TempCen:unixtime>>%s",Form("hTCVT%d",BD[i0])),Form("boardID==%d",BD[i0]),"colz");
    tHK->Draw(Form("TempFPG:unixtime>>%s",Form("hTFVT%d",BD[i0])),Form("boardID==%d",BD[i0]),"colz");
    tHK->Draw(Form("Humidity:unixtime>>%s",Form("hHVT%d",BD[i0])),Form("boardID==%d",BD[i0]),"colz");
    SetTH2FXVTMod(hTCVT[i0],Form("Time Vs Temperature on MTB%02.f(center T sensor)",BD[i0]*1.));
    SetTH2FXVTMod(hTFVT[i0],Form("Time Vs Temperature on MTB%02.f(FPGA T sensor)",BD[i0]*1.));
    SetTH2FXVTMod(hHVT [i0],Form("Time Vs Humidity on MTB%02.f(center H sensor)",BD[i0]*1.));
    hTCVT[i0] -> GetZaxis()->SetTitle("Temperature(#circC)");
    hTFVT[i0] -> GetZaxis()->SetTitle("Temperature(#circC)");
    hHVT [i0] -> GetZaxis()->SetTitle("Humidity(RH%)");
  }
  ctmp->Close();
  TCanvas *cH = new TCanvas("cH","",1920,1080);
  TCanvas *c = new TCanvas("c","",1920,1080*1.5);
  
  char cmdline[250];
  char DirTbds[50] = "TemBoardDetail/";
  char DirHbds[50] = "HumBoardDetail/";
  char DirPbds[50] = "PreBoardDetail/";
  char DirDbds[50] = "DegBoardDetail/";
  char DirAbds[50] = "AccBoardDetail/";
  char DirMbds[50] = "MagBoardDetail/";
  sprintf(cmdline,"mkdir -p %s%s",DirRes_HousKep,DirTbds);
  if (system(cmdline)!=0) system(cmdline);
  sprintf(cmdline,"mkdir -p %s%s",DirRes_HousKep,DirHbds);
  if (system(cmdline)!=0) system(cmdline);
  sprintf(cmdline,"mkdir -p %s%s",DirRes_HousKep,DirPbds);
  if (system(cmdline)!=0) system(cmdline);
  sprintf(cmdline,"mkdir -p %s%s",DirRes_HousKep,DirDbds);
  if (system(cmdline)!=0) system(cmdline);
  sprintf(cmdline,"mkdir -p %s%s",DirRes_HousKep,DirAbds);
  if (system(cmdline)!=0) system(cmdline);
  sprintf(cmdline,"mkdir -p %s%s",DirRes_HousKep,DirMbds);
  if (system(cmdline)!=0) system(cmdline);
  

  TPDF *pdfHKRec = new TPDF(Form("%sTi%05.fHKVTColz%s.pdf",DirRes_HousKep,th2fbinwidth,TimeStr));
  PDFCanPage(pdfHKRec    ,c ,Form("Title"),1,NumBD*2);

  for(int i0=0;i0<NumBD;i0++){ 
  
    
    cH->cd();
    hHVT[i0]->Draw("colz");
    TDayGrid->Draw("esame");
    DrawPdfPng(cH ,Form("%s%sHumVTColzB%02.f%s",DirRes_HousKep,DirHbds,BD[i0]*1.,TimeStr));
    PDFCanPage(pdfHKRec ,cH ,Form("Humidity Vs Time for Board %02.f",BD[i0]*1.),i0*2+1,NumBD*2);
    cH->Clear();
    
    c->Divide(2,3);
    TH2F *hTVT[6] = {hTFVT[i0],hTCVT[i0],hTNVT[0][i0],hTNVT[1][i0],hTNVT[2][i0],hTNVT[3][i0]};
    
    for(int i=0; i<=5; i++){
      c->cd(i+1);
      hTVT[i]->Draw("colz");
      TDayGrid->Draw("esame");
    }
    
    DrawPdfPng(c,Form("%s%sTemVTColzB%02.f%s",DirRes_HousKep,DirTbds,BD[i0]*1.,TimeStr));
    
    PDFCanPage(pdfHKRec    ,c ,Form("Temperature Vs Time for Board %02.f",BD[i0]*1.),i0*2+2,NumBD*2);
    delete *hTVT;
    c->Clear();
  }
  
  
  delete c;
  delete pdfHKRec;

  delete *hTCVT;
  delete *hHVT;
  
  throw;
}


void HKTGAnaV2(){
  defstyle();
  
  map<int, tempSensor> MTSens = LoadODectTable_TSensor();
  char Path_file_HK[200];
  sprintf(Path_file_HK,"%sHK_%s.root",DirOperate,TimeStr);
  cout<<"Opening the file:  "<<Path_file_HK<<endl;
  TFile *rotfileHK = TFile::Open(Form("%s",Path_file_HK));
  TTree *tHK = (TTree*) rotfileHK ->Get("tHK");
  HouseKeepingData HKD;
  HKD.SetHKTreeBranchAddress(tHK);

  Int_t    unixtime;//abs time of event [s]
  int tYear, tMonth, tDate, tHour, tMinute, tSecond;
  
  TH1F *hXVTMod[2];
  
  hXVTMod[0] = Mod_DateLabel(Form("hMXVT%d",0),"",10,70,"Temperature(#circC)");
  hXVTMod[1] = Mod_DateLabel(Form("hMXVT%d",1),"",10,100,"Humidity(RH%)");

  float DGx[ndivise]={},DGy[ndivise]={},DGex[ndivise]={},DGey[ndivise]={};
  for(int i0=0;i0<ndivise;i0++) {
    DGx[i0] = hXVTMod[0]->GetBinCenter(i0+1)+hXVTMod[0]->GetBinWidth(i0+1)/2.;
    DGy[i0] = 60;  DGey[i0] = 1000;  DGex[i0] = 0;
  }
  TGraphErrors *TDayGrid = Mod_DateGrid(50,200);

  
  TCanvas *c = new TCanvas("c","",1920,1080);
  TCanvas *cAT = new TCanvas("cAT","",1920,1080);
  TCanvas *cAH = new TCanvas("cAH","",1920,1080);
  c->cd();

  // TPDF *pdfHKRec = new TPDF(Form("%sTi%05.fHKVTTG%s.pdf",DirRes_HousKep,th2fbinwidth,TimeStr));
  TPDF *pdfTRec = new TPDF(Form("%sTi%05.fTemVTTG%s.pdf",DirRes_TempVT,th2fbinwidth,TimeStr));
  TPDF *pdfHRec = new TPDF(Form("%sTi%05.fHumVTTG%s.pdf",DirRes_HumiVT,th2fbinwidth,TimeStr));
  // PDFCanPage(pdfHKRec    ,c ,Form("Title"),1,NumBD*3);
  TGraphErrors *gTVT[5][NumBD], *gHVT[NumBD],*gTVTLG[5][NumBD], *gHVTLG[NumBD];
  
  
  //cout<<470<<endl;
  
  for(int i0=0;i0<NumBD;i0++){
    hXVTMod[0]->SetTitle(Form("Time Vs Tenperature on MTB%02.f(center T sensor)",BD[i0]*1.));
    hXVTMod[1]->SetTitle(Form("Time Vs Humidity on MTB%02.f(center H sensor)",BD[i0]*1.));
    
    int nT[4] ={}, nH[4]={};
    vector<Double_t> tin, dtin;
    vector<Double_t> temin[5], humin, THErr; cout<<tHK->GetEntriesFast()<<endl;
    for(int i = 0;i<int(tHK->GetEntriesFast());i++){
      tHK->GetEntry(i);
      if(HKD.boardID!= BD[i0]) continue;//@@
      tin     .push_back(HKD.unixtime);
      dtin    .push_back(HKD.dunixtime);
      THErr   .push_back(1);
      temin[0].push_back(HKD.Temps1);
      temin[1].push_back(HKD.Temps2);
      temin[2].push_back(HKD.Temps3);
      temin[3].push_back(HKD.Temps4);
      temin[4].push_back(HKD.TempCen);
      humin   .push_back(HKD.Humidity);
      // cout<<tin[i]<<temin[4][i]<<endl;
    }
    int dataNum = int(tin.size()); cout<<dataNum<<endl;
    //gTVT[is][i0]->Integral(-1,-1);
  //cout<<495<<endl;
    
    for(int is = 0;is<5;is++){
      gTVT[is][i0] = new TGraphErrors(dataNum,tin.data(),temin[is].data(),dtin.data(),THErr.data());
      cout<<"In: "<<gTVT[is][i0]->GetN()<<"\t";
      SortHKData(gTVT[is][i0]);
      cout<<"Out: "<<gTVT[is][i0]->GetN()<<"\n";
      gTVT[is][i0]->SetLineColor(ColorArr17[i0]);
      gTVT[is][i0]->SetMarkerStyle(20);
      gTVT[is][i0]->SetLineColor(ColorArr9[is]);
      // gTVTLG[is][i0]= (TGraphErrors*) gTVT[is][i0]->Clone();
      // gTVTLG[is][i0]->SetMarkerSize(4);
    } 
  //cout<<508<<endl;
    // gTVTLG[4][i0]->SetLineColor(ColorArr17[i0]);
    gHVT[i0]= new TGraphErrors(dataNum,tin.data(),   humin.data(),dtin.data(),THErr.data());
    SortHKData(gHVT[i0]);
  //cout<<512<<endl;
    
    gHVT[i0]->SetLineColor(ColorArr17[i0]);
    gHVT[i0]->SetMarkerStyle(5);
    gHVT[i0]->SetMarkerColor(ColorArr17[i0]);
     //cout<<517<<endl;
    gHVTLG[i0]= (TGraphErrors*) gHVT[i0]->Clone();
    gHVTLG[i0]->SetMarkerSize(4);
    
    c->cd();
    hXVTMod[0]->Draw("box");
    TDayGrid->Draw("pesame");
    for(int is = 0; is<5 ; is++) gTVT[is][i0]->Draw("esame");
    DrawPdfPng(c ,Form("%sTemVTTGB%02.f%s",DirRes_TempVT,BD[i0]*1.,TimeStr));

    // PDFCanPage(pdfHKRec    ,c ,Form("Temperature Vs Time for Board %02.f",BD[i0]*1.),i0*2+1,NumBD*2+3);
    PDFCanPage(pdfTRec    ,c ,Form("Temperature Vs Time for Board %02.f",BD[i0]*1.),i0+1,NumBD);
     //cout<<529<<endl;
    hXVTMod[1]->Draw("box");
    TDayGrid->Draw("pesame");
    gHVT[i0]->Draw("pesame");
    DrawPdfPng(c ,Form("%sHumVTTGB%02.f%s",DirRes_HumiVT,BD[i0]*1.,TimeStr));
    // PDFCanPage(pdfHKRec    ,c ,Form("Humidity Vs Time for Board %02.f",BD[i0]*1.),i0*2+2,NumBD*2+3);
    PDFCanPage(pdfHRec    ,c ,Form("Humidity Vs Time for Board %02.f",BD[i0]*1.),i0+1,NumBD);
     ////cout<<537<<endl;
    
  }
  TLegend *LHK = new TLegend(0.3,0.8,0.9,0.9);
  LHK->SetFillColor(0);
  LHK->SetLineColor(1);
  LHK->SetNColumns(NumBD);
  TLegend *LTT = (TLegend*) LHK->Clone();
  TLegend *LHT = (TLegend*) LHK->Clone();
 ////cout<<546<<endl;
  cAT->cd();
  hXVTMod[0]->Draw("box");
  TDayGrid->Draw("pesame");
  for(int i0=0;i0<NumBD;i0++){
    for(int is = 0; is<5 ; is++){
      gTVT[is][i0]->Draw("psame");
      // LTT->AddEntry(gTVTLG[is][i0],Form("Temp.MTB%02.f",BD[i0]*1.),"p");
    } 
  } //cout<<555<<endl;
  LTT->Draw();
  // PDFCanPage(pdfHKRec    ,cAT ,Form("Temperature Vs Time for All Board"),NumBD*2+1,NumBD*2+3);
   //cout<<558<<endl;
  cAH->cd();
  hXVTMod[1]->Draw("box");
  TDayGrid->Draw("pesame");
  for(int i0=0;i0<NumBD;i0++){
    gHVT[i0]->Draw("psame");
    LHK->AddEntry(gHVTLG[i0],Form("Humi.MTB%02.f",BD[i0]*1.),"p");
    LHT->AddEntry(gHVTLG[i0],Form("Humi.MTB%02.f",BD[i0]*1.),"p");
  } 
  LHT->Draw();
  // PDFCanPage(pdfHKRec    ,cAH ,Form("Humidity Vs Time for All Board"),NumBD*2+2,NumBD*2+3);
   //cout<<571<<endl;
  hXVTMod[0]->Draw("box");
  TDayGrid->Draw("pesame");

  LHK->Draw("same");
  for(int i0=0;i0<NumBD;i0++){
    // gTVTLG[4][i0]->Draw("psame");
    for(int i1=0; i1<gHVT[i0]->GetN(); i1++){
      double x,y;
      gHVT[i0]->GetPoint(i1,x,y);
      gHVT[i0]->SetPoint(i1,x,y/2.);
    }
    gHVT[i0]->Draw("psame");

  }  //cout<<585<<endl;
  
  TGaxis *ARHT = new TGaxis(cAT->GetUxmax(),cAT->GetUymin(), cAT->GetUxmax(), 50,cAT->GetUymin()*2.,100,110,"+L");//@@?ä???????
  ARHT->SetTitle("Humidity(RH%)");
  ARHT->SetLabelSize(0.025); 
  ARHT->Draw("same");
    // PDFCanPage(pdfHKRec    ,cAH ,Form("HK Vs Time for All Board"),NumBD*2+3,NumBD*2+3);
  
}
