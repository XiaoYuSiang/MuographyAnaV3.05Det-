//include
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TPDF.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TText.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TGraphErrors.h>
#include "/data4/YuSiang/personalLib/Style/DBMTStyle.h"
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"
#include "DSLData.h"
using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace DataConst;


//Statac Variable

//Rate vs Time for Channel
void SEAnaV2(const int indexGap=28) {
  FATAL("Error: Please use the last version: SEAnaV3.C");
}

TCanvas *SEVT(const int indexGap=28) {
  setTDRStyle();
  
  char FileRead[200];
  sprintf(FileRead,"%sETracksGT%d_%s_nHTH%dL%d.root",DirOperate,eventGapTcnt[indexGap],TimeStr,TriggerS,TriggerL);
  cout<<"opening....."<<FileRead<<endl;
  TFile *SelectRfile = TFile::Open(FileRead);
  TTree *tSE  = (TTree*) SelectRfile ->Get("t");
  string Path_report = Form("%sReport_SENumDayBin_%s.dat", DirOperate, TimeStr);
  ofstream report(Path_report.data());
  
  //Data Variable
  //Int_t    frame   ;//event
  Int_t unixtime, tYear, tMonth, tDate, tHour, tMinute, tSecond ;
  
  //take time and set anlyze Constant for boundry condition


  Int_t th2fbinwidthtmp = 3600*binHours;
  //Int_t ndivis    = int((FinDaySec-FirDaySec)/th2fbinwidthtmp)+1;
  const int ndivis = binHours>=24 ? ndivise/bindays : ndivise*24/binHours;
  cout<<"ndivise = "<<ndivise<<" ; ndivis = "<< ndivis <<endl;
  string str_TimeBin = binHours>=24 ? Form("%dday",bindays) : Form("%02.fhrs",binHours*1.);

  // cout<<"str_TimeBin:   "<<str_TimeBin<<endl;
  
  TH1F *hLpw[NumLY], *hBpw[NumBD];
  for(int iLy=0;iLy<NumLY;iLy++)
    hLpw[iLy] = new TH1F(Form("hLpw%d",iLy),"",int(binpw*0.6),0,int(binpw*0.6));
  for(int ibd=0;ibd<NumBD;ibd++)
    hBpw[ibd] = new TH1F(Form("MTB-%02.f",BD[ibd]*1.),Form("MTB-%02.f",BD[ibd]*1.),int(binpw*0.6),0,int(binpw*0.6));
  //@@seperate every board!
  TH1F *hERT3     = new TH1F("hERT3","",ndivis,FirDaySec,FinDaySec);
  TH1F *hERT4     = new TH1F("hERT4","",ndivis,FirDaySec,FinDaySec);
  TH1F *hCRvT     = new TH1F("hCRvT","",ndivis,FirDaySec,FinDaySec);//event 
  TH1F *hERTA     = new TH1F("hERTA","",ndivis,FirDaySec,FinDaySec);//event 

  cout<<"bins:\t"<<ndivis<<endl;
  float maxcon  = 0 , maxconA  = 0;

  char SE_timeSelect[80];
  sprintf(SE_timeSelect,"unixtime>%d&&unixtime<%d",FirDaySec,FinDaySec);

  for(int ily=0;ily<NumLY;ily++)
    tSE->Draw(Form("pwidth%d/pwidthScaleFactor>>hLpw%d",ily,ily),
      Form("%s||%s&&iZ==%d",SE_CaseA,SE_CaseB,ily),"");
  tSE->Draw("unixtime>>hERT4", Form("%s&&%s",SE_timeSelect,SE_CaseA),"");
  tSE->Draw("unixtime>>hERT3", Form("%s&&%s",SE_timeSelect,SE_CaseB),"");
  tSE->Draw("unixtime>>hERTA", Form("%s&&(%s||%s)",SE_timeSelect,SE_CaseA,SE_CaseB),"");
  
  vector <int> vXA, vY3, vY4, vYA;

  for(int i1 = 0; i1<ndivis+1 ;++i1){
    vXA.push_back(th2fbinwidthtmp*(i1)+FirDaySec);
    vY3.push_back(hERT3->GetBinContent(i1+1));
    vY4.push_back(hERT4->GetBinContent(i1+1));
    vYA.push_back(hERTA->GetBinContent(i1+1));
    unixTimeToHumanReadable(vXA[i1], tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    
    cout<<"ut:YYYYMMDD-HH:Y3:Y4:YA:    "<<vXA[i1]<<"\t";
    cout<<Form("%4.f%02.f%02.f-%02.f",tYear*1.,tMonth*1.,tDate*1.,tHour*1.)<<"\t";
    cout<<vY3[i1]<<"\t"<<vY4[i1]<<"\t"<<vYA[i1]<<endl;
  }

  for(int i1 = 0; i1<ndivis ;++i1)
    if(maxconA<vYA[i1]) maxconA = vYA[i1];
  
  cout<<"maxcontentALL:\t"<<maxconA<<endl;
  report<<"maxcontentALL:\t"<<maxconA<<endl;
  
  //set canvas

  TCanvas *c3 = EasyCanvas("c3","1k");
  //set model for TH1F and
  setTDRStyle();

  TH1F *hMod      = new TH1F("hMod","",vXA.size()-1,vXA[0],vXA[vXA.size()-1]);
  hMod->SetStats(0);
  hMod->GetXaxis()->SetTitle(Form("Date(%s/Bin)",str_TimeBin.data()));
  hMod->GetXaxis()->SetLabelSize(0.025);
  hMod->GetXaxis()->SetLabelOffset(hMod->GetXaxis()->GetLabelOffset()*0.5);
  hMod->SetStats(0);
  hMod->SetTitle(TimeStrTitle);
  hMod->GetYaxis()->SetTitle("Number of tracks");
  hMod->SetTitle("");//Form("Event VS time For Case A,B"));
  hMod->GetXaxis()->SetRangeUser(FirDaySec,FinDaySec+86400*2);
  hMod->GetXaxis()->SetTickLength(0);
  
  for (int i1=0;i1<=ndivis;i1++){
    int timeBinDay = FirDaySec + th2fbinwidthtmp*(i1);
    unixTimeToHumanReadable
      (timeBinDay, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    if(ruleDisPlayTStrOrNot( ndivise, tYear, tMonth, tDate, tHour)||i1==ndivise-1||i1==0)
    // if(tHour==0)
      hMod->GetXaxis()->SetBinLabel(i1+1 ,Form("%s-%02.f",MonthName[tMonth],1.*tDate));
  }

  
  int nhERT3 = hERT3->Integral(-1,-1);
  int nhERT4 = hERT4->Integral(-1,-1);
  int nhERTA = hERTA->Integral(-1,-1);
  cout<<"3H:  "<<nhERT3<<"  4H:  \t"<<nhERT4<<"  AH:  \t"<<nhERTA<<"\t"<<endl;

  TLegend *LERT = new TLegend(0.7,0.91,1,1);
  
  // TGraph *gA = new TGraph(ndivis,vXA.data(),vYA.data());  
  TH1F   *h3 = (TH1F*) hMod->Clone(); h3->SetName("h3");
  TH1F   *h4 = (TH1F*) hMod->Clone(); h4->SetName("h4");
  TH1F   *hModf = (TH1F*) hMod->Clone(); hModf->SetName("hModf");
  const int NumSECase = 2;
  TH1F   *hs[NumSECase] = {h3,h4};
  // TH1F   *h4 = new TH1F("h4","",ndivis,vXA[0],vXA[ndivis]);  
  cout<<"ndivis,XA[0]\t"<<ndivis<<"\t"<<vXA[0]<<"\t"<<vXA[ndivis]<<endl;
  for(int i=0;i<ndivis;i++){
    h3->SetBinContent(i+1,hERT3->GetBinContent(i+1));
    h4->SetBinContent(i+1,hERT4->GetBinContent(i+1));
  }
  
  maxcon = h3->GetMaximum()>h4->GetMaximum() ? h3->GetMaximum() : h4->GetMaximum();
  hMod->GetYaxis()->SetRangeUser(0,maxcon*1.2);
  hModf->GetYaxis()->SetRangeUser(0,1.2*float(maxcon)/float(th2fbinwidthtmp));
  
  for(int i=0;i<NumSECase;i++){
    hs[i]->SetBarWidth(0.8/float(NumSECase)*0.97);//0.39);
    hs[i]->SetBarOffset(0.1+0.8/float(NumSECase)*i);//0.11+0.4*i);
    hs[i]->SetMarkerStyle(33);
    hs[i]->SetLineColor(ColorArr9[i]);
    hs[i]->SetFillColor(ColorArr9[i]);
    hs[i]->SetMarkerSize(4-i);
  }
  
  c3->cd();
  hMod->Draw("");
  h3->Draw("barsame");
  h4->Draw("barsame");
    
  TLegend *Lg = new TLegend(0.34,0.85,0.98,0.95);
  Lg->SetFillColor(0);
  Lg->SetNColumns(2);
  // Lg->SetFillColorAlpha (0, 0.00);
  Lg->AddEntry(h4,Form("Case A%s:  %4d",SE_CaseA,nhERT4),"F");
  Lg->AddEntry(h3,Form("Case B%s:  %4d",SE_CaseB,nhERT3),"F");
  Lg->Draw();
 
  DrawPdfPng(c3,Form("%sSEVTTG_com%s",DirRes_LFirTra,str_TimeBin.data()));
  
  TPDF *pdfc1sc   = new TPDF(Form("%sSEResult_com%dday%s.pdf",DirRes_LFirTra,bindays,TimeStr));
  // TCanvas *c1s= new TCanvas("c1s","",1920,2160);
  PDFCanPage(pdfc1sc ,c3 ,Form("Title"));
  
  string cmdline = Form("mkdir -p %sPerDayAna/",DirRes_LFirTra);
  if (system(cmdline.data())!=0)  system(cmdline.data());
  
  c3->cd();  c3->Update();
  report << "report: Selection number of muon list per binday:"<<endl;
  report<<"--------------------------------------------------"<<endl;
  report<<"YYYYMMDD\tUT\tTZone\tNumCaseA\tNumCaseB"<<endl;
  report<<"--------------------------------------------------"<<endl;
  for(int i=0;i<ndivise;i++){
    TLegend *Lgp = new TLegend(0.34,0.85,0.98,0.95);
    Lgp->SetNColumns(2);
    int g4b = h4->GetBinContent(i+1), g3b = h3->GetBinContent(i+1);
    cout<<"g4b:g3b:   "<<g4b<<"\t"<<g3b<<endl;
    
    Lgp->AddEntry(h4,Form("Case A%s:  %4d",SE_CaseA,g4b),"F");
    Lgp->AddEntry(h3,Form("Case B%s:  %4d",SE_CaseB,g3b),"F");
    Lgp->Draw();
    
    int timeBinDay = FirDaySec + 86400*i;
    unixTimeToHumanReadable(timeBinDay, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    char str_MD[30] ={},str_MD1[40] ={};
    sprintf(str_MD , "%s-%02.f",MonthName[tMonth],1.*tDate);
    sprintf(str_MD1, "%04.f%02.f%02.f-%02.f",tYear*1.,tMonth*1.,1.*tDate,tHour*1.);

    int BinIndex[2] = {hMod->FindBin(timeBinDay+1),hMod->FindBin(timeBinDay+86399)};
    hMod->GetXaxis()->SetRangeUser(hMod->GetBinCenter(BinIndex[0]),hMod->GetBinCenter(BinIndex[1]));
    
    c3->Update();
    c3->Print(Form("%sPerDayAna/SE%dday%s.pdf",DirRes_LFirTra,bindays,str_MD1));
    cout<<(Form("%sPerDayAna/SE%dday%s.pdf",DirRes_LFirTra,bindays,str_MD1))<<endl;
    report<<str_MD1<<"\t"<<timeBinDay<<"\t"<<timeZone<<"\t"<<g4b<<"\t"<<g3b<<endl;
    if(i==ndivis-1) PDFCanPage(pdfc1sc,c3,str_MD,ndivis,ndivis);
    else PDFCanPage(pdfc1sc   ,c3 ,str_MD);
  }
  
  TH1F   *h3f = (TH1F*) h3->Clone(); h3f->SetName("h3f");
  TH1F   *h4f = (TH1F*) h4->Clone(); h4f->SetName("h4f");
  h3f->Scale(1./float(th2fbinwidthtmp));
  h4f->Scale(1./float(th2fbinwidthtmp));
  hModf->GetYaxis()->SetTitle("Select Event Frequency(HZ)");
  hModf->Draw();
  h3f->Draw("barsame");
  h4f->Draw("barsame"); 
  Lg->Draw();
  DrawPdfPng(c3,Form("%sSEFVTTG_com%s",DirRes_LFirTra,str_TimeBin.data()));

  report.close();
  system(Form("cp %s %s",Path_report.data(),DirRes_LFirTra));
  // c1s->Close();
  return c3;
}

TCanvas *SEVPW(const int indexGap=28) {
  setTDRStyle();
  
  char FileRead[200];
  sprintf(FileRead,"%sETracksGT%d_%s_nHTH%dL%d.root",DirOperate,eventGapTcnt[indexGap],TimeStr,TriggerS,TriggerL);
  TFile *SelectRfile;
  cout<<"opening....."<<FileRead<<endl;
  SelectRfile = TFile::Open(FileRead);
  TTree *tSE  = (TTree*) SelectRfile ->Get("t");
  
  if(24/binHours<1)
    if(24./float(binHours)-24/binHours > 0.0001)
      FATAL("ERROR: \n  Please use the factor \"F\" of 24 to be the AnaVariable.h: int binHours = F.\n    For example: F = 1,2,3,4,6,12,24");
  
  string str_TimeBin = binHours>=24 ? Form("%dday",bindays) : Form("%02.fhrs",binHours*1.);

  TH1F *hLpw[NumLY], *hBpw[NumBD];
  for(int iLy=0;iLy<NumLY;iLy++)
    hLpw[iLy] = new 
      TH1F(Form("hLpw%d",iLy),"",256,0,256);
  for(int ibd=0;ibd<NumBD;ibd++)
    hBpw[ibd] = new 
      TH1F(Form("MTB-%02.f",BD[ibd]*1.),Form("MTB-%02.f",BD[ibd]*1.),256,0,256);
  TCanvas *ctmp = new TCanvas("ctmp","",100,100); ctmp->cd();
  for(int ily=0;ily<NumLY;ily++)
    tSE->Draw(Form("pwidth/pwidthScaleFactor>>hLpw%d",ily),
      Form("(%s||%s)&&iZ==%d",SE_CaseA,SE_CaseB,ily),"");
  for(int ibd=0;ibd<NumBD;ibd++)
    tSE->Draw(Form("pwidth/pwidthScaleFactor>>MTB-%02.f",BD[ibd]*1.),
      Form("(%s||%s)&&board==%d",SE_CaseA,SE_CaseB,BD[ibd]),"");
  
  //set canvas
  TCanvas *c1 = new TCanvas("c1","",1920,1080);

  TLegend *LhLpw[4];
  //gStyle-> SetStatH(0.1); 
  c1->cd();
  c1->Divide(2,2);
  vector <int> numMax;

  for(int i0=0;i0<NumLY;i0++)
    numMax.push_back(hLpw[i0]->GetMaximum());
  
  int maxbin = *max_element(numMax.begin(), numMax.end());
  cout<<numMax.size()<<" "<<maxbin<<endl;
  for(int i0=0;i0<NumLY;i0++){
    c1->cd(i0+1);
    hLpw[i0]->GetXaxis()->SetTitle("PWidth(100ns) PS: scale normalized ");
    hLpw[i0]->GetYaxis()->SetTitle("Number of tracks");

    hLpw[i0]->SetLineColor(ColorArr9[i0]);
    hLpw[i0]->SetLineWidth(2);
    hLpw[i0]->GetXaxis()->SetRangeUser(0,50);
    hLpw[i0]->Draw("box");
    double Mx[2]={hLpw[i0]->GetMean()  , hLpw[i0]->GetMean() };
    double My[2]={ 0                  , maxbin*1.2         };
    double Ex[2]={hLpw[i0]->GetRMS ()  , hLpw[i0]->GetRMS()  };
    double Ey[2]={maxbin*1.2          , maxbin*1.2         };
    TGraphErrors *MeanMark= new TGraphErrors(2,Mx,My,Ex,Ey);
    TGraphErrors *MeanMarkAdd = new TGraphErrors();
    LhLpw[i0] = new TLegend(0.75,0.60,0.98,0.95);
    LhLpw[i0] -> AddEntry(hLpw[i0],Form("Layer: %d (iZ=%d)",i0,i0),"l");
    LhLpw[i0] -> AddEntry(MeanMark,Form("Mean: % 6.2f",hLpw[i0]->GetMean()),"E2");
    LhLpw[i0] -> AddEntry(MeanMarkAdd,Form("RMS : % 6.2f",hLpw[i0]->GetRMS ()),"F");
    MeanMark ->SetLineStyle(3);

    hLpw[i0]->Draw("h");
    LhLpw[i0]->Draw("same");
    MeanMark->Draw("e2same");
    MeanMark->Draw("Esame");
    MeanMark->SetFillColorAlpha(2,0.1);
    MeanMarkAdd->SetFillColorAlpha(2,0.1);
    MeanMarkAdd->SetLineColor(0);
  }

  double numMaxtmp=0;
  for(int i=0;i<NumLY;i++)
    if(numMax[i]>numMaxtmp) numMaxtmp = numMax[i];
  for(int i=0;i<NumLY;i++) 
    hLpw[i]->GetYaxis()->SetRangeUser(0,numMaxtmp*1.2);
  
  
  TCanvas *cbdpw  = EasyCanvas("cbdpw" ,1080,16,9,NumBX,NumBY);
  TPDF *pdfcbdpw  = new TPDF(Form("%sSEPwVBoard_%s.png",DirRes_LFirTra,TimeStr));
  PDFCanPage(pdfcbdpw    ,cbdpw ,Form("Title"),1,NumLY);
  TCanvas *cbdpwA = EasyCanvas("cbdpwA",1920,1,NumLY,1,NumLY);


  
  gStyle->SetOptStat(1);
  gStyle->SetStatY(.95);
  gStyle->SetStatX(.98);
  gStyle->SetStatW(0.25);
  gStyle->SetStatH(0.1);
  gStyle->SetOptTitle(0);
  
  for(int ily=0;ily<NumLY;ily++){
    
    for(int ibd=0;ibd<NumBD/NumLY;ibd++){
      
      cbdpw->cd(ibd%4+1);

      hBpw[ibd+NumLY*ily]->GetXaxis()->SetTitle("PWidth(100ns) PS: scale normalized ");
      hBpw[ibd+NumLY*ily]->GetYaxis()->SetTitle("Number of track Hits");
      hBpw[ibd+NumLY*ily]->Draw("hist");
    }
    cbdpwA->cd(NumLY-ily);
    cbdpw->DrawClonePad();
    
    TPad *PTitle = new TPad("PTitle","",0.05,0.235+(ily)*0.25,0.95,0.245+(ily)*0.25);
    TText *tTitle = new TText(0.5,0.5,Form("Layer: %d (iZ=%d)",ily,ily));
    tTitle->SetTextSize(1);
    tTitle->SetTextAlign(22);
    cbdpwA->cd();
    PTitle->Draw();
    PTitle->cd();
    tTitle->Draw("same");
    PDFCanPage(pdfcbdpw    ,cbdpw ,Form("Layer: %d (iZ=%d)",ily,ily),1+ily,NumLY);

  }
  //cbdpwA->Print(Form("%sSEPw.pdf",DirRes_LFirTra));
  cbdpwA->Print(Form("%sSEPwVBoard_%s.png",DirRes_LFirTra,TimeStr));
  
  setTDRStyle();
 
  DrawPdfPng(c1,Form("%sSEPWidth_%s",DirRes_LFirTra,str_TimeBin.data()));
  cbdpw->Close();
  ctmp->Close();
  return c1;
}
void CombineSEResult(TCanvas *cSEVT, TCanvas *cSEVPW){
  string str_TimeBin = binHours>=24 ? Form("%dday",bindays) : Form("%02.fhrs",binHours*1.);

  TCanvas *c2= EasyCanvas("c2",1920,16,18,1,2);
  DrawPadCloneOn(cSEVT, c2->cd(1));
  DrawPadCloneOn(cSEVPW, c2->cd(2));
  DrawPdfPng(c2,Form("%sSEResult_%s",DirRes_LFirTra,str_TimeBin.data()));
  c2->Close();
}
void SEAnaV3(const int indexGap=28) {
  TCanvas *cSEVT = (TCanvas*) SEVT(indexGap)->Clone();
  TCanvas *cSEVPW = (TCanvas*) SEVPW(indexGap)->Clone();
  CombineSEResult(cSEVT,cSEVPW);
}
/*

void SEAnaV3Back(const int indexGap=28) {
  setTDRStyle();
  
  char FileRead[200];
  sprintf(FileRead,"%sETracksGT%d_%s_nHTH%dL%d.root",DirOperate,eventGapTcnt[indexGap],TimeStr,TriggerS,TriggerL);
  TFile *SelectRfile;
  cout<<"opening....."<<FileRead<<endl;
  SelectRfile = TFile::Open(FileRead);
  TTree *tSE  = (TTree*) SelectRfile ->Get("t");

  ofstream out(Form("%sSEV2.dat",DirOperate));
  ofstream report(Form("%sReport_SENumDayBin_%s.dat",DirRes_LFirTra,TimeStr));
  
  
  //Data Variable
  //Int_t    frame   ;//event
  Int_t unixtime, tYear, tMonth, tDate, tHour, tMinute, tSecond ;
  
  //take time and set anlyze Constant for boundry condition


  Int_t th2fbinwidthtmp = 3600*binHours;
  //Int_t ndivis    = int((FinDaySec-FirDaySec)/th2fbinwidthtmp)+1;
  const int ndivis = binHours>=24 ? ndivise/bindays : ndivise*24/binHours;
  cout<<"ndivise = "<<ndivise<<" ; ndivis = "<< ndivis <<endl;
  string str_TimeBin = binHours>=24 ? Form("%dday",bindays) : Form("%02.fhrs",binHours*1.);

  // cout<<"str_TimeBin:   "<<str_TimeBin<<endl;
  
  TH1F *hLpw[NumLY], *hBpw[NumBD];
  for(int iLy=0;iLy<NumLY;iLy++)
    hLpw[iLy] = new TH1F(Form("hLpw%d",iLy),"",int(binpw*0.6),0,int(binpw*0.6));
  for(int ibd=0;ibd<NumBD;ibd++)
    hBpw[ibd] = new TH1F(Form("MTB-%02.f",BD[ibd]*1.),Form("MTB-%02.f",BD[ibd]*1.),int(binpw*0.6),0,int(binpw*0.6));
  //@@seperate every board!
  TH1F *hERT3     = new TH1F("hERT3","",ndivis,FirDaySec,FinDaySec);
  TH1F *hERT4     = new TH1F("hERT4","",ndivis,FirDaySec,FinDaySec);
  TH1F *hCRvT     = new TH1F("hCRvT","",ndivis,FirDaySec,FinDaySec);//event 
  TH1F *hERTA     = new TH1F("hERTA","",ndivis,FirDaySec,FinDaySec);//event 

  cout<<"bins:\t"<<ndivis<<endl;
  // throw;
  
  //set anlyze Variable


  float maxcon  = 0 , maxconA  = 0;


  // int evsSE = data.GetEntriesFast();
  
  char SE_timeSelect[80];
  sprintf(SE_timeSelect,"unixtime>%d&&unixtime<%d",FirDaySec,FinDaySec);

  for(int ily=0;ily<NumLY;ily++)
    tSE->Draw(Form("pwidth%d/pwidthScaleFactor>>hLpw%d",ily,ily),
      Form("%s||%s&&iZ==%d",SE_CaseA,SE_CaseB,ily),"");
  tSE->Draw("unixtime>>hERT4", Form("%s&&%s",SE_timeSelect,SE_CaseA),"");
  tSE->Draw("unixtime>>hERT3", Form("%s&&%s",SE_timeSelect,SE_CaseB),"");
  tSE->Draw("unixtime>>hERTA", Form("%s&&(%s||%s)",SE_timeSelect,SE_CaseA,SE_CaseB),"");
  
  for(int ibd=0;ibd<NumBD;ibd++)
    tSE->Draw(Form("pwidth/pwidthScaleFactor>>MTB-%02.f",BD[ibd]*1.),
      Form("(%s||%s)&&board==%d",SE_CaseA,SE_CaseB,BD[ibd]),"");
  

  int XA[ndivis+1], Y3[ndivis+1], Y4[ndivis+1], YA[ndivis+1];
  vector <int> vXA, vY3, vY4, vYA;

  for(int i1 = 0; i1<ndivis+1 ;++i1){
    vXA.push_back(th2fbinwidthtmp*(i1)+FirDaySec);
    vY3.push_back(hERT3->GetBinContent(i1+1));
    vY4.push_back(hERT4->GetBinContent(i1+1));
    vYA.push_back(hERTA->GetBinContent(i1+1));
    // XA[i1] = th2fbinwidthtmp*(i1)+FirDaySec;
    // Y3[i1] = hERT3->GetBinContent(i1+1);
    // Y4[i1] = hERT4->GetBinContent(i1+1);
    // YA[i1] = hERTA->GetBinContent(i1+1);
    unixTimeToHumanReadable(vXA[i1], tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    
    cout<<"ut:YYYYMMDD-HH:Y3:Y4:YA:    "<<vXA[i1]<<"\t"<<Form("%4.f%02.f%02.f-%02.f",tYear*1.,tMonth*1.,tDate*1.,tHour*1.)<<"\t"<<vY3[i1]<<"\t"<<vY4[i1]<<"\t"<<vYA[i1]<<endl;
  }
  // vXA.push_back(th2fbinwidthtmp*(ndivis+1)+FirDaySec);
  
  for(int i1 = 0; i1<ndivis ;++i1)
    if(maxconA<vYA[i1]) maxconA = vYA[i1];
  
  cout<<"maxcontentALL:\t"<<maxconA<<endl;
  report<<"maxcontentALL:\t"<<maxconA<<endl;
  
  //set canvas
  TCanvas *c1 = new TCanvas("c1","",1920,1080);
  TCanvas *c1s= new TCanvas("c1s","",1920,2160);
  TCanvas *c3 = new TCanvas("c3","",1920,1080);

  
  TLegend *LhLpw[4];
  //gStyle-> SetStatH(0.1); 
  c1->cd();
  c1->Divide(2,2);
  vector <int> numMax;

  for(int i0=0;i0<NumLY;i0++)
    numMax.push_back(hLpw[i0]->GetMaximum());
  
  int maxbin = *max_element(numMax.begin(), numMax.end());
  
  for(int i0=0;i0<NumLY;i0++){
    c1->cd(i0+1);
    hLpw[i0]->GetXaxis()->SetTitle("PWidth(100ns) PS: scale normalized ");
    hLpw[i0]->GetYaxis()->SetTitle("Number of tracks");


    hLpw[i0]->SetLineColor(ColorArr9[i0]);
    hLpw[i0]->SetLineWidth(2);
    hLpw[i0]->GetXaxis()->SetRangeUser(0,50);
    hLpw[i0]->Draw("box");
    double Mx[2]={hLpw[i0]->GetMean()  , hLpw[i0]->GetMean() };
    double My[2]={ 0                  , maxbin*1.2         };
    double Ex[2]={hLpw[i0]->GetRMS ()  , hLpw[i0]->GetRMS()  };
    double Ey[2]={maxbin*1.2          , maxbin*1.2         };
    TGraphErrors *MeanMark= new TGraphErrors(2,Mx,My,Ex,Ey);
    TGraphErrors *MeanMarkAdd = new TGraphErrors();
    LhLpw[i0] = new TLegend(0.75,0.60,0.98,0.95);
    LhLpw[i0] -> AddEntry(hLpw[i0],Form("Layer: %d (iZ=%d)",i0,i0),"l");
    LhLpw[i0] -> AddEntry(MeanMark,Form("Mean: % 6.2f",hLpw[i0]->GetMean()),"E2");
    LhLpw[i0] -> AddEntry(MeanMarkAdd,Form("RMS : % 6.2f",hLpw[i0]->GetRMS ()),"F");
    MeanMark ->SetLineStyle(3);

    hLpw[i0]->Draw("h");
    LhLpw[i0]->Draw("same");
    MeanMark->Draw("e2same");
    MeanMark->Draw("Esame");
    MeanMark->SetFillColorAlpha(2,0.1);
    MeanMarkAdd->SetFillColorAlpha(2,0.1);
    MeanMarkAdd->SetLineColor(0);
  }

  double numMaxtmp=0;
  for(int i=0;i<NumLY;i++){
    if(numMax[i]>numMaxtmp) numMaxtmp = numMax[i];
  } 
  numMaxtmp*=1.2;
  for(int i=0;i<NumLY;i++) hLpw[i]->GetYaxis()->SetRangeUser(0,numMaxtmp);
  
  
  TCanvas *cbdpw  = new TCanvas("cbdpw" ,"",1920,1920  );
  TPDF *pdfcbdpw  = new TPDF(Form("%sSEPwVBoard_%s.png",DirRes_LFirTra,TimeStr));
  PDFCanPage(pdfcbdpw    ,cbdpw ,Form("Title"),1,NumLY);
  cbdpw ->Divide(NumBX,NumBY);
  
  TCanvas *cbdpwA = new TCanvas("cbdpwA","",1920,1920*NumLY);
  cbdpwA->Divide(1,NumLY);


  
  gStyle->SetOptStat(1);
  gStyle->SetStatY(.95);
  gStyle->SetStatX(.98);
  gStyle->SetStatW(0.25);
  gStyle->SetStatH(0.1);
  gStyle->SetOptTitle(0);
  
  for(int ily=0;ily<NumLY;ily++){
    
    for(int ibd=0;ibd<NumBD/NumLY;ibd++){
      
      cbdpw->cd(ibd%4+1);

      hBpw[ibd+NumLY*ily]->GetXaxis()->SetTitle("PWidth(100ns) PS: scale normalized ");
      hBpw[ibd+NumLY*ily]->GetYaxis()->SetTitle("Number of track Hits");
      hBpw[ibd+NumLY*ily]->Draw("hist");
    }
    cbdpwA->cd(NumLY-ily);
    cbdpw->DrawClonePad();
    
    TPad *PTitle = new TPad("PTitle","",0.05,0.235+(ily)*0.25,0.95,0.245+(ily)*0.25);
    TText *tTitle = new TText(0.5,0.5,Form("Layer: %d (iZ=%d)",ily,ily));
    tTitle->SetTextSize(1);
    tTitle->SetTextAlign(22);
    cbdpwA->cd();
    PTitle->Draw();
    PTitle->cd();
    tTitle->Draw("same");
    PDFCanPage(pdfcbdpw    ,cbdpw ,Form("Layer: %d (iZ=%d)",ily,ily),1+ily,NumLY);

  }
  //cbdpwA->Print(Form("%sSEPw.pdf",DirRes_LFirTra));
  cbdpwA->Print(Form("%sSEPwVBoard_%s.png",DirRes_LFirTra,TimeStr));
  //LhLpw->AddEntry(hLpw4,Form("4point:\t%d",nhERT4),"lp");
  //set model for TH1F and
  setTDRStyle();
  // throw;

  TH1F *hMod      = new TH1F("hMod","",vXA.size()-1,vXA[0],vXA[vXA.size()-1]);
  hMod->SetStats(0);
  hMod->GetXaxis()->SetTitle("Date");
  hMod->GetXaxis()->SetLabelSize(0.025);
  
  for (int i1=0;i1<=ndivis;i1++){
    int timeBinDay = FirDaySec + th2fbinwidthtmp*(i1);
    unixTimeToHumanReadable
      (timeBinDay, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    
    if(ruleDisPlayTStrOrNot( ndivise, tYear, tMonth, tDate, tHour)||i1==ndivise||i1==0)
      hMod->GetXaxis()
        ->SetBinLabel(i1+1 ,Form("%s-%02.f",MonthName[tMonth],1.*tDate));
  }

  
  int nhERT3 = hERT3->Integral(-1,-1);
  int nhERT4 = hERT4->Integral(-1,-1);
  int nhERTA = hERTA->Integral(-1,-1);
  cout<<"3H:  "<<nhERT3<<"  4H:  \t"<<nhERT4<<"  AH:  \t"<<nhERTA<<"\t"<<endl;

  TLegend *LERT = new TLegend(0.7,0.91,1,1);
  
  TGraph *gA = new TGraph(ndivis,vXA.data(),vYA.data());  
  TH1F   *h3 = new TH1F("h3","",ndivis,vXA[0],vXA[ndivis]);  
  TH1F   *h4 = new TH1F("h4","",ndivis,vXA[0],vXA[ndivis]);  
  cout<<"ndivis,XA[0]\t"<<ndivis<<"\t"<<XA[0]<<"\t"<<XA[ndivis]<<endl;
  for(int i=0;i<ndivis;i++){
    h3->SetBinContent(i+1,hERT3->GetBinContent(i+1));
    h4->SetBinContent(i+1,hERT4->GetBinContent(i+1));
    
  }
  

  maxcon = 
    h3->GetMaximum()>h4->GetMaximum() ?
    h3->GetMaximum() : h4->GetMaximum();
  


  h3->SetBarWidth(0.39);
  h3->SetBarOffset(0.11+0.4*0);
  
  h4->SetBarWidth(0.39);
  h4->SetBarOffset(0.11+0.4*1);
  
  c3->cd();
  
  h3->SetMarkerStyle(33);
  h4->SetMarkerStyle(33);
  h3->SetLineColor(ColorArr9[0]);
  h4->SetLineColor(ColorArr9[1]);
  h3->SetFillColor(ColorArr9[0]);
  h4->SetFillColor(ColorArr9[1]);
  h3->SetMarkerSize(4);
  h4->SetMarkerSize(3);
  
  hMod->SetStats(0);
  hMod->SetTitle(TimeStrTitle);
  hMod->GetXaxis()->SetTitle("Date");
  hMod->GetYaxis()->SetTitle("Number of tracks");
  hMod->SetTitle("");//Form("Event VS time For Case A,B"));
  hMod->GetXaxis()->SetRangeUser(FirDaySec,FinDaySec+86400*2);
  hMod->GetYaxis()->SetRangeUser(0,maxcon*1.2);
  hMod->GetXaxis()->SetTickLength(0);
  
  
  
  hMod->Draw();
  // if(b3L4){
    h3->Draw("barsame");
    h4->Draw("barsame");
  // }else{
    // h4->Draw("barsame");
    // h3->Draw("barsame");
  // }
  TLegend *Lg = new TLegend(0.34,0.85,0.98,0.95);
  Lg->SetFillColor(0);
  Lg->SetNColumns(2);
  // Lg->SetFillColorAlpha (0, 0.00);
  Lg->AddEntry(h4,Form("Case A%s:  %4d",SE_CaseA,nhERT4),"F");
  Lg->AddEntry(h3,Form("Case B%s:  %4d",SE_CaseB,nhERT3),"F");
  Lg->Draw();
 // hMod->GetYaxis()->SetRangeUser(0,;
 
  DrawPdfPng(c1,Form("%sSEPWidth_%s",DirRes_LFirTra,str_TimeBin.data()));
  DrawPdfPng(c3,Form("%sSEVTTG_com%s",DirRes_LFirTra,str_TimeBin.data()));
  

  c1s->Divide(1,2);
  
  c1s->cd(1);
  for(int i=0;i<NumLY;i++) hLpw[i]->Scale(1./hLpw[i]->Integral(-1,-1));
  
  numMaxtmp=0;
  for(int i=0;i<NumLY;i++){
    if(hLpw[i]->GetMaximum()>numMaxtmp) numMaxtmp = hLpw[i]->GetMaximum();
  } 
  numMaxtmp*=1.2;
  for(int i=0;i<NumLY;i++) hLpw[i]->GetYaxis()->SetRangeUser(0,numMaxtmp);
  cout<<"numMaxtmp:  "<<numMaxtmp<<endl;
  hLpw[0]->Draw("h");
  for(int i=0;i<NumLY;i++){
    hLpw[i]->GetYaxis()->SetTitle("Ratio of Number of tracks");
    hLpw[i]->Draw("hsame");
    LhLpw[i]->Draw("same");
    LhLpw[i]->SetX1NDC(0.48+0.125*i);
    LhLpw[i]->SetY1NDC(0.60);
    LhLpw[i]->SetX2NDC(0.605+0.125*i);
    LhLpw[i]->SetY2NDC(0.95);
  }
  c1s->cd(2);
  
  hMod->Draw();
  hMod->GetXaxis()->SetRangeUser(XA[0],XA[ndivis]);
  // if(b3L4){
    h3->Draw("barsame");
    h4->Draw("barsame");
  // }else{
    // h4->Draw("barsame");
    // h3->Draw("barsame");
  // }
  c1s->cd(2)->SetTicks(0,0);
  //hMod->GetYaxis()->SetNdivisions(0,(maxconA+1));

  Lg->Draw();
  c1s->cd(2)->SetTicks(0,0);
  c1s->Print(Form("%sSEResult_com%dday.png",DirRes_LFirTra,bindays));
  c1s->Print(Form("%sSEResult_com%dday.pdf",DirRes_LFirTra,bindays));
    
  TPDF *pdfc1sc   = new TPDF(Form("%sSEResult_com%dday%s.pdf",DirRes_LFirTra,bindays,TimeStr));

  PDFCanPage(pdfc1sc ,c1s ,Form("Title"));
  // throw;
  char cmdline[180];
  sprintf(cmdline,"mkdir -p %sPerDayAna/",DirRes_LFirTra);
  if (system(cmdline)!=0)  system(cmdline);
  c3->cd();
  c3->Update();
  report << "report: Selection number of muon list per binday:"<<endl;
  report<<"--------------------------------------------------"<<endl;
  report<<"YYYYMMDD\tUT\tTZone\tNumCaseA\tNumCaseB"<<endl;
  report<<"--------------------------------------------------"<<endl;
  for(int i=0;i<ndivise;i++){
    TLegend *Lgp = new TLegend(0.34,0.85,0.98,0.95);
    Lgp->SetNColumns(2);
    int g4b = h4->GetBinContent(i+1), g3b = h3->GetBinContent(i+1);
    cout<<"g4b:g3b:   "<<g4b<<"\t"<<g3b<<endl;
    
    Lgp->AddEntry(h4,Form("Case A%s:  %4d",SE_CaseA,g4b),"F");
    Lgp->AddEntry(h3,Form("Case B%s:  %4d",SE_CaseB,g3b),"F");
    Lgp->Draw();
    
    int timeBinDay = FirDaySec + 86400*i;
    unixTimeToHumanReadable(timeBinDay, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    char str_MD[30] ={},str_MD1[40] ={};
    sprintf(str_MD , "%s-%02.f",MonthName[tMonth],1.*tDate);
    // cout<<tDate<<endl;
    sprintf(str_MD1, "%04.f%02.f%02.f-%02.f",tYear*1.,tMonth*1.,1.*tDate,tHour*1.);
    // cout<<str_MD1<<endl;
        unixTimeToHumanReadable
      (timeBinDay, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    
    // if(ruleDisPlayTStrOrNot( 1, tYear, tMonth, tDate, tHour))
      // hMod->GetXaxis()
        // ->SetBinLabel(i1 ,Form("%s-%02.f",MonthName[tMonth],1.*tDate));
    // hMod->GetXaxis()->SetBinLabel(i+1,str_MD);
    // hMod->GetXaxis()->SetRangeUser(
      // h4->GetBinCenter(i+1)-h4->GetBinWidth(i)/2.,h4->GetBinCenter(i+1)+h4->GetBinWidth(i)/2.);
    // h3->GetXaxis()->SetRangeUser(
      // h4->GetBinCenter(i+1)-h4->GetBinWidth(i)/2.,h4->GetBinCenter(i+1)+h4->GetBinWidth(i)/2.);
    // h4->GetXaxis()->SetRangeUser(
      // h4->GetBinCenter(i+1)-h4->GetBinWidth(i)/2.,h4->GetBinCenter(i+1)+h4->GetBinWidth(i)/2.);
    int BinIndex[2] = {hMod->FindBin(timeBinDay),hMod->FindBin(timeBinDay+86399)-1};
    hMod->GetXaxis()->SetRangeUser(
      hMod->GetBinCenter(5),hMod->GetBinCenter(8));
      
    c3->Update();
    c3->Print(Form("%sPerDayAna/SE%dday%s.pdf",DirRes_LFirTra,bindays,str_MD1));
    cout<<(Form("%sPerDayAna/SE%dday%s.pdf",DirRes_LFirTra,bindays,str_MD1))<<endl;
    report<<str_MD1<<"\t"<<timeBinDay<<"\t"<<timeZone<<"\t"<<g4b<<"\t"<<g3b<<endl;
    if(i==ndivis-1) PDFCanPage(pdfc1sc,c3,str_MD,ndivis,ndivis);
    else PDFCanPage(pdfc1sc   ,c3 ,str_MD);
  }
  

  
}
*/