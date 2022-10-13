#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TText.h>
#include <TF1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TPDF.h>
#include "/home/yusiang/personalLib/WeatherPattern.h"
#include "/home/yusiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"
#include "DSLData.h"

using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace DataConst;

//Rate vs Time for Channel
void RVTCAna() {

  vector<string> vRootFiles = RawRootList(DirOperate);
  TreeReader data(vRootFiles);
  
  ofstream out(Form("%sRVTRec.dat",DirOperate));
  //Data Variable
  Int_t    unixtime;//abs time of event [s]
  Int_t    tYear   ;
  Int_t    tMonth  ;
  Int_t    tDate   ;
  Int_t    tHour   ;
  Int_t    tMinute ;
  Int_t    tSecond ;
  Int_t    nHits   ;//total number for hit in a frame/event
  Int_t*   board   ;//the board ID which if the index nth hit
  Int_t*   channel ;//the board ID which if the index nth hit
  
  //take time and set anlyze Constant for boundry condition

  //data hist create
  TH1F *hMod  = Mod_DateLabel("hMod","",0,5,"Rate(s^{-1} )");
  TH2F *hMod2 = Mod_DateLabel("hMod2","",BinNumOnCha,0,BinNumOnCha,"General Channel ID");

  TH1F *hRT1D     = new TH1F("hRT1D","",BinOnTimeD,unixtimei,unixtimef);
  TH1F *hRT1DA[NumBD];
  for(int i0=0;i0<NumBD;i0++) hRT1DA[i0] = new TH1F(Form("hRT1D%d",BD[i0]),"",BinOnTime,unixtimei,unixtimef);

  TH1F *h24Bin    = new TH1F("h24Bin","",24,UT_SelectDay,UT_SelectDay+86400);
  TH2F *hRT2D     = new TH2F("hRT2D","",BinOnTimeD,FirDaySec,FinDaySec,BinNumOnCha,0,BinNumOnCha);
  cout<<"bins:\t"<<BinOnTime<<endl;

  TGraphErrors *TDayGrid = Mod_DateGrid(0.5, 100);
  //set anlyze Variable
  
  float maxcon  = 0, mincon = 100;
  bool useB = 1;
  //fundimental understand for the data on operattion time
  
  //Fill the data
  for (Long64_t ev = 0; ev <evs; ++ev) {
    data.GetEntry(ev);
    unixtime  = data.GetLong64("unixtime");
    //frame     = data.GetInt("frame");//event
    tYear     = data.GetInt("tYear");
    tMonth    = data.GetInt("tMonth");
    tDate     = data.GetInt("tDate");
    tHour     = data.GetInt("tHour");
    tMinute   = data.GetInt("tMinute");
    tSecond   = data.GetInt("tSecond");
    nHits     = data.GetInt("nHits");
    board     = data.GetPtrInt("board");
    channel   = data.GetPtrInt("channel");
    vector<int> ihits;
    int i1 = 0;
    for(int i1 = 0; i1<nHits ;++i1){
      for(int i2 = 0; i2<NumBD ;++i2){
        if(board[i1]==BD[i2]){
          hRT2D->Fill(unixtime,channel[i1]+i2*BinNumOnCha/NumBD,1/th2fbinwidth);
          break;
        }
      }
      
      hRT1D->Fill(unixtime,1/th2fbinwidth/double(BinNumOnCha));
      for(int i2 = 0; i2<NumBD ;++i2){
        if(board[i1]==BD[i2]){
          hRT1DA[i2]->Fill(unixtime,1/th2fbinwidth/double(16));
          break;
        }
      }
    }
  }
  //find max content
  
  
  float minSetUserB2D, maxSetUserB2D;
  FindYRangeTH2F(hRT2D,minSetUserB2D,maxSetUserB2D,1,0.1,2);
  
  
  
  int c3l=1920,c3w=1080;
  //set canvas
  TCanvas *c1 = new TCanvas("c1","",c3l,c3w);
  TCanvas *c2 = new TCanvas("c2","",c3l,c3w);
  TCanvas *c3 = new TCanvas("c3","",c3l,c3w);
  TCanvas *c4 = new TCanvas("c4","",c3l,c3w);
  
  //set model for TH1F and
  hMod2->GetYaxis()->SetTitleOffset(1);
  hMod2->GetZaxis()->SetTitleOffset(0.7);
  hMod2->GetZaxis()->SetTitleOffset(0.7);
  
  hRT2D->GetYaxis()->SetRangeUser(0,BinNumOnCha);
  hRT2D->GetZaxis()->SetRangeUser(minSetUserB2D,maxSetUserB2D);
  cout<<"m:M:\t"<<minSetUserB2D<<"\t"<<maxSetUserB2D<<endl;
  hRT2D->SetStats(0);
  hRT2D->GetZaxis()->SetTitle("Rate(s^{-1} )");
  hRT2D->GetZaxis()->SetTitleOffset(0.7);
  hRT2D->GetZaxis()->RotateTitle(true);


  
  TH1F *hModp = (TH1F*) hMod->Clone();
  for (int i1=0;i1<BinNumOnCha;i1+=4){
    hMod2->GetYaxis()->SetBinLabel(i1+1 ,Form("%d",i1));
  }
  //Draw RvT for Each channel
  
  c1->cd();

  TPDF *pdfAllChDis = new TPDF(Form("%sTi%05.fRVTVChs_%s.pdf",DirRes_RatVT,th2fbinwidth,TimeStr));

  hMod2->Draw("");
  hRT2D->Draw("colzsame");
  TDayGrid->Draw("esame");
  PDFCanPage(pdfAllChDis    ,c1 ,Form("GChID VS Rate VS Time"),1,BinNumOnCha+1);
  c1->SetGrid(0,1);
  c1->SetTicks(0,1);
  
  TH1F *hModchs = (TH1F*) hMod->Clone();
  hModchs->GetYaxis()->SetRangeUser(minSetUserB2D,maxSetUserB2D);
  for(int i0=0;i0<NumBD;i0++){
    for(int i1=0;i1<NumCh;i1++){
      TH1F *hRVTChTtmp = new TH1F("hRVTChTtmp","",BinOnTimeD,FirDaySec,FinDaySec);
      hModchs->SetTitle(Form("MTB:%d Ch:%02.f",BD[i0],i1*1.));
      for(int i2=0;i2<BinOnTimeD;i2++){
        hRVTChTtmp->SetBinContent(i2+1,hRT2D->GetBinContent(i2+1,i1+i0*NumCh+1));
      }
      hModchs->Draw("box");
      hRVTChTtmp->Draw("boxsame");
      TDayGrid->Draw("esame");
      PDFCanPage(pdfAllChDis, c1, Form("MTB:%d Ch:%02.f",BD[i0],i1*1.),i1+i0*NumCh+2,BinNumOnCha+1);
      delete hRVTChTtmp;
    }
  }
  PDFFinish(pdfAllChDis);
  
  
  c1->Close();
  //WeatherPattern(Form(), Double_t xlow, Double_t ylow, Double_t xup, Double_t yup, TCanvas *Canvas0,const char *option)

  float minSetUser4B = 100, maxSetUser4B = 0;
  for(int i0=0;i0<NumBD;i0++){
    
    FindYRangeTH1F(hRT1DA[i0],mincon,maxcon,1,0.1,2);
    if(maxSetUser4B<maxcon) maxSetUser4B = maxcon;
    if(minSetUser4B>mincon) minSetUser4B = mincon;
    // cout<<minSetUser4B<<"\t"<<maxSetUser4B<<endl;
    // cout<<mincon<<"\t"<<maxcon<<endl;
  }
  
  //Draw average Channel
  c3->cd();
  c3->SetGrid(0,1);
  c3->SetTicks(0,1);
  hModp->Draw();
  //hRT1D->Draw("hsame");
  for(int i0=0;i0<NumBD;i0++){
    hRT1DA[i0] ->Draw("hsame");
    hRT1DA[i0] ->SetLineColor(ColorArr9[i0]);
    hRT1DA[i0] ->SetLineWidth(2);
    
  }
  TDayGrid->Draw("esame");
  
  hModp->GetYaxis()->SetRangeUser(minSetUser4B,maxSetUser4B);
  hModp->SetTitle("");
  hModp->GetXaxis()->SetNdivisions(-105);
  hModp->GetYaxis()->SetNdivisions(506,false);
  TLegend *LhRT1D = new TLegend(0.3,0.8,0.9,0.9);
  LhRT1D->SetFillColor(0);
  LhRT1D->SetLineColor(1);
  LhRT1D->SetNColumns(NumBD);
  for(int i0=0;i0<NumBD;i0++) LhRT1D->AddEntry(hRT1DA[i0],Form("MTB%d",BD[i0]),"l");
  
  LhRT1D->Draw();
  

  c3->Print(Form("%sAllChRVTBox_%s.png",DirRes_RatVT,TimeStr));
  c3->Print(Form("%sAllChRVTBox_%s.pdf",DirRes_RatVT,TimeStr));
  c3->Close();
  
  //Draw the channel compare
  c2->cd();
  c2->SetGrid(0,1);
  c2->SetTicks(1,1);
  
  
  h24Bin->GetXaxis()->SetRangeUser(UT_SelectDay,UT_SelectDay+86400);
  h24Bin->GetXaxis()->SetNdivisions(-224);
  h24Bin->SetStats(0);
  h24Bin->GetXaxis()->SetTitle("Hour(UTC+8)");
  h24Bin->GetYaxis()->SetTitle("Rate(s^{-1})");
  h24Bin->GetYaxis()->SetTitleOffset(1.5);
  h24Bin->GetYaxis()->SetRangeUser(minSetUserB2D,maxSetUserB2D);
  h24Bin->SetTitle("Compare MTB 1,3,5,4 Channel 0");
  for (int i1=0;i1<24;i1++){
    h24Bin->GetXaxis()->SetBinLabel(i1+1,Form("%d",i1));
  }
  
  TPDF *pdfBDChCp = new TPDF(Form("%sTi%05.fRVTCChBox_%s.pdf",DirRes_RatVT,th2fbinwidth*1.,TimeStr));

  PDFCanPage(pdfBDChCp    ,c2 ,Form("Title"),1,NumCh);
  
  TH1F *h24Binp = (TH1F*) h24Bin->Clone();
  for(int i0=0;i0<NumCh;i0++){
    h24Binp->Draw();
    TH1F *h4BChCp[NumBD];
    for(int i1=0;i1<NumBD;i1++){
      h4BChCp[i1] = new TH1F(Form("h4BChCpB%d",BD[i1]),Form("h4BChCpB%d",BD[i1]),BinOnTimeD,FirDaySec,FinDaySec);
      for(int i2=0;i2<BinOnTimeD;i2++){
        h4BChCp[i1]->SetBinContent(i2+1, hRT2D->GetBinContent(i2+1,i0+i1*NumCh+1));

      }
      
      h4BChCp[i1]->SetLineColor(ColorArr9[i1]);
      h4BChCp[i1]->SetLineWidth(2);
      h4BChCp[i1]->Draw("hsame");
    }
    LhRT1D->Draw("same");
    // cout<<Form("Ch:%02.f",i0*1.)<<endl;
    h24Binp->SetTitle(Form("Ch:%02.f",i0*1.));
    PDFCanPage(pdfBDChCp, c2, Form("Ch:%02.f",i0*1.),i0+1,NumCh);
    for(int i1=0;i1<NumBD;i1++) delete h4BChCp[i1];
  }
  PDFFinish(pdfBDChCp);


  c2->Close();

  c4->cd();
  const int BinDayDisp = 48;
  TH1F   *h48BinSun  = new TH1F("h48BinSun","",BinDayDisp,0,86400);
  h48BinSun->SetStats(0);
  h48BinSun->GetXaxis()->SetNdivisions(-224);
  h48BinSun->GetXaxis()->SetTitle("Hour(UTC+8)");
  h48BinSun->GetYaxis()->SetTitle("Rate(s^{-1})");
  h48BinSun->GetYaxis()->SetTitleOffset(1.5);
  h48BinSun->GetYaxis()->SetRangeUser(minSetUserB2D,maxSetUserB2D);
  h48BinSun->SetTitle("Average Rate on sunny day");
  for(int i0=0;i0<BinDayDisp/2;i0++){
     h48BinSun->GetXaxis()->SetBinLabel(2*i0+1,Form("%02.f",i0*1.));
  }
  Float_t SelectData[Num_SelectDay][2][BinDayDisp];
  maxcon = 0, mincon = 100;
  for(int i0=0;i0<Num_SelectDay;i0++){
    for(int i1=0;i1<BinDayDisp;i1++){
      float contmp = hRT1D->GetBinContent(hRT1D->FindBin(SelectDaySec[i0]+th2fbinwidth*i1));
      SelectData[i0][0][i1] = th2fbinwidth*i1;
      SelectData[i0][1][i1] = contmp;
      if(maxcon<contmp) maxcon = contmp;
      if(mincon>contmp&&contmp>=0.1){
        mincon = contmp;
        cout<<mincon<<endl;
      }
      // out<<i0<<"\t"<<i1<<"\t"<<SelectDaySec[i0]<<"\t"<<hRT1D->FindBin(SelectDaySec[i0]+th2fbinwidth*i1)<<"\t"<<SelectData[i0][0][i1]<<"\t"<<SelectData[i0][1][i1]<<endl;
    }
  }
  
  float maxSetUser48 = MaxSetUserRatio(maxcon,1,1);
  float minSetUser48 = MinSetUserRatio(mincon,1.2,1);
  h48BinSun->GetYaxis()->SetRangeUser(minSetUser48,maxSetUser48);

  TGraph *gdaycp[Num_SelectDay];

  TLegend *Lgg = new TLegend(0.5,0.8,0.89,0.89);
  h48BinSun->Draw("Box");
  Lgg->Draw("same");
  for(int i0=0;i0<Num_SelectDay;i0++){
    gdaycp[i0] = new TGraph(BinDayDisp,SelectData[i0][0],SelectData[i0][1]);
    gdaycp[i0] ->SetMarkerColor(colorArr[i0]);
    gdaycp[i0]->SetMarkerSize(2);
    gdaycp[i0]->SetMarkerStyle(33);
    gdaycp[i0]->Draw("psame");
    
    unixTimeToHumanReadable(SelectDaySec[i0], tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    char str_MD[30] ={};
    sprintf(str_MD , "%s-%02.f",MonthName[tMonth],1.*tDate);
    Lgg->AddEntry(gdaycp[i0],str_MD,"p");

  }
  
  Lgg->SetFillColor(0);
  Lgg->SetLineColor(0);
  Lgg->SetNColumns(2);
  Lgg->SetFillColorAlpha (0, 0.00);
  Lgg->SetNColumns(5);
  
  c4->Print(Form("%sSunnyDateRVTCpBox_%s.png",DirRes_RatVSun,TimeStr));
  c4->Print(Form("%sSunnyDateRVTCpBox_%s.pdf",DirRes_RatVSun,TimeStr));
}