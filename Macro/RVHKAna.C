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

//Statac Variable





//Rate vs Time for Channel
void RVHKAna(const bool*OperMode) {
  bool PointTextMode = OperMode[6];
  int ChannelID = 0;
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
  TH1F *hMod      = new TH1F("hMod","",ndivise+1,FirDaySec,FinDaySec);
  TH1F *hRT1D     = new TH1F("hRT1D","",BinOnTimeD,unixtimei,unixtimef);
  TH1F *hRT1DA[NumBD];
  for(int i0=0;i0<NumBD;i0++) hRT1DA[i0] = new TH1F(Form("hRT1D%d",BD[i0]),"",BinOnTime,unixtimei,unixtimef);

  
  float DGx[ndivise]={},DGy[ndivise]={},DGex[ndivise]={},DGey[ndivise]={};
  for(int i0=0;i0<ndivise;i0++) {
    DGx[i0] = hMod->GetBinCenter(i0+1)+hMod->GetBinWidth(i0+1)/2.;
    DGy[i0] = 0.5;  DGey[i0] = 100;  DGex[i0] = 0;
    cout<<DGx[i0]<<"\t"<<DGy[i0]<<"\t"<<DGex[i0]<<"\t"<<DGey[i0]<<"\n";
  }
  TGraphErrors *TDayGrid = new TGraphErrors(ndivise,DGx,DGy,DGex,DGey);
  TDayGrid->SetLineStyle(3);
  //set anlyze Variable
  
  
  //Fill the data
  for (Long64_t ev = 0; ev <evs; ++ev) {
    data.GetEntry(ev);
    unixtime  = data.GetLong64("unixtime");
    nHits     = data.GetInt("nHits");
    board     = data.GetPtrInt("board");

    for(int i1 = 0; i1<nHits ;++i1){
      hRT1D->Fill(unixtime,1/th2fbinwidth/double(BinNumOnCha));
      for(int i2 = 0; i2<NumBD ;++i2){
        if(board[i1]==BD[i2]){
          hRT1DA[i2]->Fill(unixtime,1/th2fbinwidth/double(16));
          break;
        }
      }
    }
  }
  
  float maxcon  = 0, mincon = 100;
  
  for(int i0=0;i0<NumBD;i0++){
    for(int i1 =0;i1<BinOnTime;i1++){
      
      float contmp = hRT1DA[i0]->GetBinContent(i1+1);
      if(maxcon<contmp) maxcon = contmp;
      if(mincon>contmp&&contmp!=0) mincon = contmp;
    }
  }
  
  float maxSetUser4B = MaxSetUserRatio(maxcon,1.2,1);
  float minSetUser4B = MinSetUserRatio(mincon,1.1,1);
  
  
  hMod->SetStats(0);
  hMod->GetXaxis()->SetTitle("Date");
  hMod->GetYaxis()->SetTitle("Rate(s^{-1} )");
  hMod->GetXaxis()->SetTitleOffset(1.5);
  hMod->GetYaxis()->SetTitleOffset(1);
  hMod->GetXaxis()->SetRangeUser(FirDaySec,FinDaySec);
  hMod->GetXaxis()->SetTickLength(0);
  hMod->GetYaxis()->SetRangeUser(minSetUser4B,maxSetUser4B);
  hMod->SetTitle("");
  hMod->GetXaxis()->SetNdivisions(-105);
  hMod->GetYaxis()->SetNdivisions(506,false);

  for (int i3=0;i3<(Dayf-Dayi+2);i3++){
    unixtime = hMod->GetXaxis()->GetBinCenter(1+i3)-hMod->GetXaxis()->GetBinWidth(1+i3)/2.;
    unixTimeToHumanReadable(unixtime, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    char str_MD[30] ={};
    sprintf(str_MD , "%s-%02.f",MonthName[tMonth],1.*tDate);
    if(tDate==1||tDate==15||i3==0||i3==Dayf-Dayi+1||(Dayf-Dayi+2)<32){
      hMod->GetXaxis()->SetBinLabel(1+i3 ,str_MD);
    }
    cout<<"i3:    "<<i3<<"    "<<unixtime<<"    "<<str_MD<<endl;
  }
  

  TCanvas *c1 = new TCanvas("c1","",1920,1920);
  TCanvas *c2 = new TCanvas("c2","",1920,1920);
  TCanvas *c3 = new TCanvas("c3","",1920,1920);
  TCanvas *c4 = new TCanvas("c4","",1920,1920);


  
  
  ifstream inTT[NumBD]={}, inHT[NumBD]={};
  for(int i0=0;i0<NumBD;i0++){
    inTT[i0].open(Form("%sTempVTB%d.dat",DirOperate,BD[i0]));
    inHT[i0].open(Form("%sHumiVTB%d.dat",DirOperate,BD[i0]));
  }

  TH2F *hTR[4];
  TH2F *hHR[4];
  TH2F *hHTMod = new TH2F("hHTMod","",100,0,100,maxSetUser4B*50,minSetUser4B,maxSetUser4B);
  hHTMod->SetStats(0);
  hHTMod->GetYaxis()->SetTitle("Rate(s^{-1} )");
  hHTMod->GetYaxis()->SetTitleOffset(1.5); 
  hHTMod->GetYaxis()->SetLabelSize(0.025); 
  hHTMod->GetYaxis()->SetRangeUser(minSetUser4B,maxSetUser4B);
  
  TH2F *hTRMod = (TH2F*) hHTMod->Clone();
  TH2F *hHRMod = (TH2F*) hHTMod->Clone();
  
  hTRMod->GetXaxis()->SetTitle("Temperature(#circC )");
  hTRMod->GetXaxis()->SetRangeUser(0,50);
  hHRMod->GetXaxis()->SetTitle("Humidity(RH%)");
  hHRMod->GetXaxis()->SetRangeUser(0,100);


  TPDF *pdfHKVR = new TPDF(Form("%sTi%05.fHKVRColz%s.pdf",DirRes_HKVRat,th2fbinwidth,TimeStr));
  PDFCanPage(pdfHKVR    ,c1 ,Form("Title"),1,NumBD*2+1);
  
  c2->cd();

  
  int binID;
  float Rval_HorT;
  for(int i0=0;i0<NumBD;i0++){
    hTRMod->SetTitle(Form("Temperature VS Rate for MTB%02.f",BD[i0]*1.));

    hHRMod->SetTitle(Form("Humidity VS Rate for MTB%02.f",BD[i0]*1.));
    hTR[i0] = new TH2F(Form("hTR%d",BD[i0]),"",50,0,50,maxSetUser4B*50,minSetUser4B,maxSetUser4B);
    hHR[i0] = new TH2F(Form("hHR%d",BD[i0]),"",50,0,100,maxSetUser4B*50,minSetUser4B,maxSetUser4B);
    float tmp_UT=0,tmp_tem=0,tmp_hum=0,value_tem=0,value_hum=0;
    
    for(;inTT[i0]>>tmp_UT;){
      inTT[i0]>>tYear>>tMonth>>tDate>>tHour>>tMinute>>tSecond>>tmp_tem>>value_tem;
      binID = hRT1DA[i0]->FindBin(tmp_UT);
      Rval_HorT = hRT1DA[i0]->GetBinContent(binID);
      // cout<<binID<<endl;
      if(Rval_HorT<=0.05) continue;//prevent the start and end point
      hTR[i0]->Fill(tmp_tem,Rval_HorT,value_tem);
    }
    for(;inHT[i0]>>tmp_UT;){
      inHT[i0]>>tYear>>tMonth>>tDate>>tHour>>tMinute>>tSecond>>tmp_hum>>value_hum;
      binID = hRT1DA[i0]->FindBin(tmp_UT);
      Rval_HorT = hRT1DA[i0]->GetBinContent(binID);
      if(Rval_HorT<=0.05) continue;//prevent the start and end point
      hHR[i0]->Fill(tmp_hum,Rval_HorT,value_hum);
      // cout<<binID<<"  "<<tmp_hum<<"  "<<Rval_HorT<<"  "<<value_tem<<endl;
    }
    
    c1->cd();
    hTR[i0]->GetZaxis()->SetRangeUser(0,400);
    hTR[i0]->GetZaxis()->SetTitleOffset(1.2); 
    hTR[i0]->GetZaxis()->SetLabelSize(0.025); 
    hTR[i0]->GetZaxis()->SetTitle(Form("Data Number(1 count = %4.fsec)",th2fbinwidth*1.)); 
    hTR[i0]->SetMarkerStyle(20); 
    hTR[i0]->SetMarkerColor(ColorArr9[i0]); 
    hTR[i0]->SetMarkerSize(.5); 

    hTRMod->Draw("boxz");
    hTR[i0]->Draw("colzsame");

    c1->Print(Form("%sTemVRColzB%02.f%s.png",DirRes_HKVRat,BD[i0]*1.,TimeStr));
    PDFCanPage(pdfHKVR    ,c1 ,Form("Temp. Vs Rate MTB%02.f",BD[i0]*1.),2*i0+1,NumBD*2+1);
    
    hTRMod->Draw("boxz");
    hTR[i0]->Draw("textsame");

    c1->Print(Form("%sTemVRPEB%02.f%s.png",DirRes_HKVRat,BD[i0]*1.,TimeStr));
    
    
    hHR[i0]->GetZaxis()->SetRangeUser(0,400);
    hHR[i0]->GetZaxis()->SetTitleOffset(1.2); 
    hHR[i0]->GetZaxis()->SetLabelSize(0.025); 
    hHR[i0]->GetZaxis()->SetTitle(Form("Data Number(1 count = %4.fsec)",th2fbinwidth*1.)); 
    hHR[i0]->SetMarkerStyle(22); 
    hHR[i0]->SetMarkerColor(ColorArr9[i0+NumBD]); 
    hHR[i0]->SetMarkerSize(.5); 
    
    hHRMod->Draw("boxz");
    hHR[i0]->Draw("colzsame");

    c1->Print(Form("%sHumVRColzB%02.f%s.png",DirRes_HKVRat,BD[i0]*1.,TimeStr));
    PDFCanPage(pdfHKVR    ,c1 ,Form("Humi. Vs Rate MTB%02.f",BD[i0]*1.),2*i0+2,NumBD*2+1);
    
    hHRMod->Draw("boxz");
    hHR[i0]->Draw("textsame");

    c1->Print(Form("%sHumVRPEB%02.f%s.png",DirRes_HKVRat,BD[i0]*1.,TimeStr));
    

    c2->cd();
    hHR[i0]->Draw("colsame");
    hTR[i0]->Draw("colsame");
    
  }
  

  hTRMod->SetTitle(Form("Temperature VS Rate for All"));
  c3->cd();
  hTRMod->Draw("box");
  cout<<"Hint: if don't want to wait draw \"point\" for All graph, set bool \"PointTextMode\" to be false !"<<endl;
  TLegend *LTA = new TLegend(0.1,0.75,0.9,0.9);
  LTA->SetNColumns(4);
  for(int i0=0;i0<NumBD;i0++){
    if(PointTextMode) hTR[i0]->Draw("psame");
    else              hTR[i0]->Draw("textsame");
    TH2F *hTRLG = (TH2F*) hTR[i0]->Clone();
    hTRLG ->SetMarkerSize(4); 
    LTA->AddEntry(hTRLG,Form("#scale[1.0]{MTB.%02.f}",BD[i0]*1.),"p");
  }
  LTA->Draw();
  c3->Print(Form("%sHVRAllColzAll%s.png",DirRes_HKVRat,TimeStr));
  PDFCanPage(pdfHKVR    ,c3 ,Form("Temp. Vs Rate For all"),NumBD*2+1,NumBD*2+2);
  
  
  
  hHRMod->SetTitle(Form("Humidity VS Rate for All"));
  c4->cd();
  hHRMod->Draw("box");
  TLegend *LHA = new TLegend(0.1,0.75,0.9,0.9);
  LHA->SetNColumns(4);
  for(int i0=0;i0<NumBD;i0++){
    if(PointTextMode) hHR[i0]->Draw("psame");
    else              hHR[i0]->Draw("textsame");
    TH2F *hHRLG = (TH2F*) hHR[i0]->Clone();
    hHRLG ->SetMarkerSize(4); 
    LHA->AddEntry(hHRLG,Form("#scale[1.0]{MTB.%02.f}",BD[i0]*1.),"p");
  }
  LHA->Draw();
  c4->Print(Form("%sHVRAllColzAll%s.png",DirRes_HKVRat,TimeStr));
  PDFCanPage(pdfHKVR    ,c4 ,Form("Humi. Vs Rate For all"),NumBD*2+2,NumBD*2+3);
  
  
  TF1 *fTR[NumBD],*fHR[NumBD];
  for(int i0=0;i0<NumBD;i0++){
    fTR[i0] = new TF1(Form("fTR%02.f",BD[i0]*1.),Line2Dfun,0,50,2);
    fHR[i0] = new TF1(Form("fHR%02.f",BD[i0]*1.),Line2Dfun,0,100,2);
    hTR[i0]->Fit(fTR[i0]);
    hHR[i0]->Fit(fHR[i0]);
    fTR[i0]->SetLineColor(ColorArr9[i0]);
    fHR[i0]->SetLineColor(ColorArr9[i0+NumBD]);
    hTR[i0]->SetLineColor(ColorArr9[i0]);
    hHR[i0]->SetLineColor(ColorArr9[i0+NumBD]);
    LTA->AddEntry(fTR[i0],Form("#scale[1.0]{MTB%02.f Fit}",BD[i0]*1.),"l");
    LHA->AddEntry(fHR[i0],Form("#scale[1.0]{MTB%02.f Fit}",BD[i0]*1.),"l");
  }
  
  for(int i0=0;i0<NumBD;i0++){
    LTA->AddEntry(fTR[i0],Form("#scale[1.0]{R=%.5fT+%.3f}",fTR[i0]->GetParameter(0),fTR[i0]->GetParameter(1)),"");
    LHA->AddEntry(fHR[i0],Form("#scale[1.0]{R=%.5fH+%.3f}",fHR[i0]->GetParameter(0),fHR[i0]->GetParameter(1)),"");
  }
  
  c3->cd();
  hTRMod->Draw("box");
  for(int i0=0;i0<NumBD;i0++){
    if(PointTextMode) hTR[i0]->Draw("psame");
    else              hTR[i0]->Draw("textsame");
    fTR[i0]->Draw("lsame");
  }
  LTA->Draw("same");
  
  PDFCanPage(pdfHKVR    ,c3 ,Form("Temp. Vs Rate fit for all"),NumBD*2+3,NumBD*2+4);
  c3->Print(Form("%sTVRPFitAll%s.png",DirRes_HKVRat,TimeStr));
  
  c4->cd();
  hHRMod->Draw("box");
  for(int i0=0;i0<NumBD;i0++){
    if(PointTextMode) hHR[i0]->Draw("psame");
    else              hHR[i0]->Draw("textsame");
    fHR[i0]->Draw("lsame");
  }
  LHA->Draw("same");
 
  PDFCanPage(pdfHKVR    ,c4 ,Form("Humi. Vs Rate fit for all"),NumBD*2+4,NumBD*2+4);
  c4->Print(Form("%sHVRPFitAll%s.png",DirRes_HKVRat,TimeStr));
  
}
