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
  Int_t    unixtime;
  Int_t    tYear   ;
  Int_t    tMonth  ;
  Int_t    tDate   ;
  Int_t    tHour   ;
  Int_t    tMinute ;
  Int_t    tSecond ;
  
  //take time and set anlyze Constant for boundry condition


  Int_t th2fbinwidthtmp = 86400*bindays;
  //Int_t ndivise    = int((FinDaySec-FirDaySec)/th2fbinwidthtmp)+1;
  cout<<"ndivise:"<<ndivise<<endl;

  
  
  TH1F *hLpw[NumLY], *hBpw[NumBD];
  for(int iLy=0;iLy<NumLY;iLy++)
    hLpw[iLy] = new TH1F(Form("hLpw%d",iLy),"",int(binpw*0.6),0,int(binpw*0.6));
  for(int ibd=0;ibd<NumBD;ibd++)
    hBpw[ibd] = new TH1F(Form("MTB-%02.f",BD[ibd]*1.),Form("MTB-%02.f",BD[ibd]*1.),int(binpw*0.6),0,int(binpw*0.6));
  //@@seperate every board!
  TH1F *hERT3     = new TH1F("hERT3","",ndivise,FirDaySec,FinDaySec);
  TH1F *hERT4     = new TH1F("hERT4","",ndivise,FirDaySec,FinDaySec);
  TH1F *hCRvT     = new TH1F("hCRvT","",ndivise,FirDaySec,FinDaySec);//event 
  TH1F *hERTA     = new TH1F("hERTA","",ndivise,FirDaySec,FinDaySec);//event 

  cout<<"bins:\t"<<ndivise<<endl;
  
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
  

  int XA[ndivise+1];
  int Y3[ndivise+1],Y4[ndivise+1],YA[ndivise+1];

  for(int i1 = 0; i1<ndivise+1 ;++i1){
    XA[i1] = th2fbinwidthtmp*(i1)+FirDaySec;
    Y3[i1] = hERT3->GetBinContent(i1+1);
    Y4[i1] = hERT4->GetBinContent(i1+1);
    YA[i1] = hERTA->GetBinContent(i1+1);
    unixTimeToHumanReadable(XA[i1], tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    
    cout<<"ut:YYYYMMDD-HH:Y3:Y4:YA:    "<<XA[i1]<<"\t"<<Form("%4.f%02.f%02.f-%02.f",tYear*1.,tMonth*1.,tDate*1.,tHour*1.)<<"\t"<<Y3[i1]<<"\t"<<Y4[i1]<<"\t"<<YA[i1]<<endl;
  }

  for(int i1 = 0; i1<ndivise ;++i1){
    if(maxconA<YA[i1]) maxconA = YA[i1];
  }

  cout<<"maxcontentALL:\t"<<maxconA<<endl;
  out<<"maxcontentALL:\t"<<maxconA<<endl;
  
  //set canvas
  TCanvas *c1 = new TCanvas("c1","",1920,1080);
  TCanvas *c1s= new TCanvas("c1s","",1920,2160);

  TCanvas *c3 = new TCanvas("c3","",1920,1080);

  
  TLegend *LhLpw[4];
  //gStyle-> SetStatH(0.1); 
  c1->cd();
  c1->Divide(2,2);
  vector <int> numMax;

  for(int i0=0;i0<NumLY;i0++){
    numMax.push_back(hLpw[i0]->GetMaximum());
  }


  int maxbin = *max_element(numMax.begin(), numMax.end());
  for(int i0=0;i0<4;i0++){
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
  for(int i=0;i<4;i++){
    if(numMax[i]>numMaxtmp) numMaxtmp = numMax[i];
  } 
  numMaxtmp*=1.2;
  for(int i=0;i<4;i++) hLpw[i]->GetYaxis()->SetRangeUser(0,numMaxtmp);
  
  
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

  TH1F *hMod      = new TH1F("hMod","",ndivise,XA[0],XA[ndivise]);
  hMod->SetStats(0);
  hMod->GetXaxis()->SetTitle("Date");
  hMod->GetXaxis()->SetLabelSize(0.025);
  

  
  for (int i1=0;i1<ndivise;i1++){
    int timeBinDay = FirDaySec + 86400*i1;
    unixTimeToHumanReadable(timeBinDay, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    if(tDate==1||tDate==15||i1==0||i1==ndivise||ndivise<32){
      hMod->GetXaxis()->SetBinLabel((i1+1) ,Form("%s-%02.f",MonthName[tMonth],1.*tDate));
    }
  }

  
  int nhERT3 = hERT3->Integral(-1,-1);
  int nhERT4 = hERT4->Integral(-1,-1);
  int nhERTA = hERTA->Integral(-1,-1);
  cout<<"3H:  "<<nhERT3<<"  4H:  \t"<<nhERT4<<"  AH:  \t"<<nhERTA<<"\t"<<endl;

  TLegend *LERT = new TLegend(0.7,0.91,1,1);
  
 // TGraph *g3 = new TGraph(ndivise,XA,Y3);  
  // TGraph *g4 = new TGraph(ndivise,XA,Y4); 
  TGraph *gA = new TGraph(ndivise,XA,YA);  
  TH1F   *g3 = new TH1F("g3","",ndivise,XA[0],XA[ndivise]);  
  TH1F   *g4 = new TH1F("g4","",ndivise,XA[0],XA[ndivise]);  
  cout<<"ndivise,XA[0]\t"<<ndivise<<"\t"<<XA[0]<<"\t"<<XA[ndivise]<<endl;
  for(int i=0;i<ndivise;i++){
    g3->SetBinContent(i+1,hERT3->GetBinContent(i+1));
    g4->SetBinContent(i+1,hERT4->GetBinContent(i+1));
    
  }
  

  //float maxcon  = 0;
  int maxg3 = g3->GetMaximum();
  int maxg4 = g4->GetMaximum();
  bool b3L4 = maxg3>maxg4;
  maxcon = maxg4;
  
  if(b3L4){
    maxcon = maxg3;
  }
  


  g3->SetBarWidth(0.39);
  g3->SetBarOffset(0.11+0.4*0);
  
  g4->SetBarWidth(0.39);
  g4->SetBarOffset(0.11+0.4*1);
  
  c3->cd();
  
  g3->SetMarkerStyle(33);
  g4->SetMarkerStyle(33);
  g3->SetLineColor(ColorArr9[0]);
  g4->SetLineColor(ColorArr9[1]);
  g3->SetFillColor(ColorArr9[0]);
  g4->SetFillColor(ColorArr9[1]);
  g3->SetMarkerSize(4);
  g4->SetMarkerSize(3);
  
  hMod->SetStats(0);
  hMod->SetTitle(TimeStrTitle);
  hMod->GetXaxis()->SetTitle("Date");
  hMod->GetYaxis()->SetTitle("Number of tracks");
  hMod->SetTitle("");//Form("Event VS time For Case A,B"));
  hMod->GetXaxis()->SetRangeUser(FirDaySec,FinDaySec+86400);
  hMod->GetYaxis()->SetRangeUser(0,maxcon*1.2);
  hMod->GetXaxis()->SetTickLength(0);
  
  
  
  hMod->Draw();
  if(b3L4){
    g3->Draw("barsame");
    g4->Draw("barsame");
  }else{
    g4->Draw("barsame");
    g3->Draw("barsame");
  }
  TLegend *Lg = new TLegend(0.34,0.85,0.98,0.95);
  Lg->SetFillColor(0);
  Lg->SetNColumns(2);
  // Lg->SetFillColorAlpha (0, 0.00);
  Lg->AddEntry(g4,Form("Case A%s:  %4d",SE_CaseA,nhERT4),"F");
  Lg->AddEntry(g3,Form("Case B%s:  %4d",SE_CaseB,nhERT3),"F");
  Lg->Draw();
 // hMod->GetYaxis()->SetRangeUser(0,;
  c1->Print(Form("%sSEPWidth_%dday.pdf",DirRes_LFirTra,bindays));
  c1->Print(Form("%sSEPWidth_%dday.png",DirRes_LFirTra,bindays));
  c3->Print(Form("%sSEVTTG_com%dday.pdf",DirRes_LFirTra,bindays));
  c3->Print(Form("%sSEVTTG_com%dday.png",DirRes_LFirTra,bindays));
  
  

  c1s->Divide(1,2);
  
  c1s->cd(1);
  for(int i=0;i<4;i++) hLpw[i]->Scale(1./hLpw[i]->Integral(-1,-1));
  
  numMaxtmp=0;
  for(int i=0;i<4;i++){
    if(hLpw[i]->GetMaximum()>numMaxtmp) numMaxtmp = hLpw[i]->GetMaximum();
  } 
  numMaxtmp*=1.2;
  for(int i=0;i<4;i++) hLpw[i]->GetYaxis()->SetRangeUser(0,numMaxtmp);
  cout<<"numMaxtmp:  "<<numMaxtmp<<endl;
  hLpw[0]->Draw("h");
  for(int i=0;i<4;i++){
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
  hMod->GetXaxis()->SetRangeUser(XA[0],XA[ndivise]);
  if(b3L4){
    g3->Draw("barsame");
    g4->Draw("barsame");
  }else{
    g4->Draw("barsame");
    g3->Draw("barsame");
  }
  c1s->cd(2)->SetTicks(0,0);
  //hMod->GetYaxis()->SetNdivisions(0,(maxconA+1));

  Lg->Draw();
  c1s->cd(2)->SetTicks(0,0);
  c1s->Print(Form("%sSEResult_com%dday.png",DirRes_LFirTra,bindays));
  c1s->Print(Form("%sSEResult_com%dday.pdf",DirRes_LFirTra,bindays));
    
  TPDF *pdfc1sc   = new TPDF(Form("%sSEResult_com%dday%s.pdf",DirRes_LFirTra,bindays,TimeStr));

  PDFCanPage(pdfc1sc    ,c1s ,Form("Title"));
  
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
    int g4b = g4->GetBinContent(i+1), g3b = g3->GetBinContent(i+1);
    cout<<"g4b:g3b:   "<<g4b<<"\t"<<g3b<<endl;
    
    Lgp->AddEntry(g4,Form("Case A%s:  %4d",SE_CaseA,g4b),"F");
    Lgp->AddEntry(g3,Form("Case B%s:  %4d",SE_CaseB,g3b),"F");
    Lgp->Draw();
    
    int timeBinDay = FirDaySec + 86400*i;
    unixTimeToHumanReadable(timeBinDay, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    char str_MD[30] ={},str_MD1[40] ={};
    sprintf(str_MD , "%s-%02.f",MonthName[tMonth],1.*tDate);
    // cout<<tDate<<endl;
    sprintf(str_MD1, "%04.f%02.f%02.f",tYear*1.,tMonth*1.,1.*tDate);
    // cout<<str_MD1<<endl;
    // hMod->GetXaxis()->SetBinLabel(i+1,str_MD);
    hMod->GetXaxis()->SetRangeUser(
      g4->GetBinCenter(i+1)-g4->GetBinWidth(i)/2.,g4->GetBinCenter(i+1)+g4->GetBinWidth(i)/2.);
    g3->GetXaxis()->SetRangeUser(
      g4->GetBinCenter(i+1)-g4->GetBinWidth(i)/2.,g4->GetBinCenter(i+1)+g4->GetBinWidth(i)/2.);
    g4->GetXaxis()->SetRangeUser(
      g4->GetBinCenter(i+1)-g4->GetBinWidth(i)/2.,g4->GetBinCenter(i+1)+g4->GetBinWidth(i)/2.);
    
    c3->Print(Form("%sPerDayAna/SE%dday%s.pdf",DirRes_LFirTra,bindays,str_MD1));
    cout<<(Form("%sPerDayAna/SE%dday%s.pdf",DirRes_LFirTra,bindays,str_MD1))<<endl;
    report<<str_MD1<<"\t"<<timeBinDay<<"\t"<<timeZone<<"\t"<<g4b<<"\t"<<g3b<<endl;
    if(i==ndivise-1) PDFCanPage(pdfc1sc,c3,str_MD,ndivise,ndivise);
    else PDFCanPage(pdfc1sc   ,c3 ,str_MD);
  }
  

  
}
