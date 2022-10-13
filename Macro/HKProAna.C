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
void HKProAna() {

  ofstream outF(Form("%sHKdat.dat",DirOperate));

  char HKFileList[200];
  sprintf(HKFileList, "%sHKTxtNameAna.dat",DirOperate);
  ifstream infilename(HKFileList);

  int boan,tn,t0;
  char filename[100];
  for(int i=0;infilename>>filename;i++){
    ifstream in(filename);
    int a,boa,s1, ti1,ti2,    ti3,    ti4, Tc,  h, T1, T2, T3, t5, d1, d2, d3, a1, a2, a3,t;
   // EX 43	1	60	480	479	9863045	9863043	27	-53	26	26	26	25	360	-1	-95	-0	-1	-0	1616041866
    in>>a>>boa>>s1>>ti1>>ti2>>ti3>>ti4>>Tc>>h>>T1>>T2>>T3>>t5>>d1>>d2>>d3>>a1>>a2>>a3>>t;
    outF<<boa<<"\t"<<t<<"\t"<<Tc<<"\t"<<T1<<"\t"<<T2<<"\t"<<T3<<"\t"<<t5<<"\t"<<h<<endl;
    boan=boa; tn=t;
    for(;in>>a>>boa>>s1>>ti1>>ti2>>ti3>>ti4>>Tc>>h>>T1>>T2>>T3>>t5>>d1>>d2>>d3>>a1>>a2>>a3>>t;){
      if(boan==boa||tn==t||Tc<11) continue;
      outF<<boa<<"\t"<<t<<"\t"<<Tc<<"\t"<<T1<<"\t"<<T2<<"\t"<<T3<<"\t"<<t5<<"\t"<<h<<endl;
      boan=boa; tn=t;
    }
  }
  cout<<"House keeping data is save to be:  "<<endl<<Form("    %sHKdat.dat",DirOperate)<<endl;
  outF.close();
   
}





void HKAna() {
  ofstream outT[NumBD]={}, outH[NumBD]={};
  for(int i0=0;i0<NumBD;i0++){
    outT[i0].open(Form("%sTempVTB%02.f.dat",DirOperate,BD[i0]*1.));
    outH[i0].open(Form("%sHumiVTB%02.f.dat",DirOperate,BD[i0]*1.));
  }
  
  ifstream in(Form("%sHKdat.dat",DirOperate));
  TH1F *hXVTMod[2];
  TH2F *hTVT[NumBD], *hHVT[NumBD];
  TGraphErrors *TDayGrid = Mod_DateGrid(50,200);
  
  hXVTMod[0] = Mod_DateLabel(Form("hMXVT%d",0),"",10,60,"Temperature(#circC)");
  hXVTMod[1] = Mod_DateLabel(Form("hMXVT%d",1),"",10,110,"Humidity(RH%)");
    
    
  for(int i0 = 0; i0<NumBD ; i0++){
    hTVT[i0] = new TH2F(Form("hTVT%d",BD[i0]),"",BinOnTimeD,FirDaySec,FinDaySec,50,0,50);
    hHVT[i0] = new TH2F(Form("hHVT%d",BD[i0]),"",BinOnTimeD,FirDaySec,FinDaySec,100,0,100);
  }
  
  //TH2F *hTVHS  = new TH2F("hTVHS","",BinOnTime,t0,tn,100,0,100);
  int boa,UT,Tc,Hc,Ti[4];
  for(;in>>boa>>UT>>Tc;){
    for(int i1 = 0; i1<4 ; i1++) in>>Ti[i1];
    in>>Hc;
    for(int i1 = 0; i1<NumBD ; i1++){
      if(boa==BD[i1]){
        hTVT[i1]->Fill(UT,Tc);
        hHVT[i1]->Fill(UT,Hc<0 ? Hc+127:Hc);
      }
    }
  }
  int tYear, tMonth, tDate, tHour, tMinute, tSecond;
  for(int i0=0;i0<NumBD;i0++){
    for(int i1=0;i1<BinOnTimeD;i1++){
      int tout = FirDaySec+i1*th2fbinwidth;
      float tempout = 0, humiout = 0;
      int binnumtot = 0;
      for(int i2=0;i2<50;i2++){
        int numbin = hTVT[i0]->GetBinContent(i1+1,i2+1);
        tempout   += numbin*i2;
        binnumtot += numbin;
        numbin =  hHVT[i0]->GetBinContent(i1+1,2*i2+1);
        humiout   += numbin*(2*i2);
        numbin =  hHVT[i0]->GetBinContent(i1+1,2*i2+2);
        humiout   += numbin*(2*i2+1);
      }
      if(binnumtot == 0) continue;
      tempout /= (binnumtot*1.);
      humiout /= (binnumtot*1.);
      
      unixTimeToHumanReadable(tout, tYear, tMonth, tDate, tHour, tMinute, tSecond, timeZone);
      outT[i0]<<tout<<"\t"<<tYear<<"\t"<<tMonth<<"\t"<<tDate<<"\t";
      outT[i0]<<tHour<<"\t"<<tMinute<<"\t"<<tSecond<<"\t"<<tempout<<"\t";
      outT[i0]<<binnumtot<<"\n";
      outH[i0]<<tout<<"\t"<<tYear<<"\t"<<tMonth<<"\t"<<tDate<<"\t";
      outH[i0]<<tHour<<"\t"<<tMinute<<"\t"<<tSecond<<"\t"<<humiout<<"\n";
      outH[i0]<<binnumtot<<"\n";
      
    }
    
  }
  in.close();
  
  cout<< "Save Thermal data Vs Time of different board in:" <<endl;
  for(int i0=0;i0<NumBD;i0++){ 
    cout<< Form("%sTempVTB%02.f.dat",DirOperate,BD[i0]*1.) <<endl;
    cout<< Form("%sHumiVTB%02.f.dat",DirOperate,BD[i0]*1.) <<endl;
    outT[i0].close();
    outH[i0].close();
  }
  
  
  TCanvas *c = new TCanvas("c","HK tables",1920,1080);
  
  for(int i0=0;i0<NumBD;i0++){ 
    hTVT[i0]->GetZaxis()->SetTitle("times");
    hTVT[i0]->GetZaxis()->SetTitleOffset(0.6);
    hTVT[i0]->GetZaxis()->SetNdivisions(-005);
    hTVT[i0]->GetZaxis()->SetRangeUser(0,95);
    
    hHVT[i0]->GetZaxis()->SetTitle("times");
    hHVT[i0]->GetZaxis()->SetTitleOffset(0.6);
    hHVT[i0]->GetZaxis()->SetNdivisions(-005);
  }
  
  
  c->cd();
  TPDF *pdfHKRec = new TPDF(Form("%sTi%05.fHKVTColz%s.pdf",DirRes_HousKep,th2fbinwidth,TimeStr));
  PDFCanPage(pdfHKRec    ,c ,Form("Title"),1,NumBD*2);

  for(int i0=0;i0<NumBD;i0++){ 
    hXVTMod[0]->SetTitle(Form("Time Vs Temperature on MTB%02.f(center T sensor)",BD[i0]*1.));
    hXVTMod[0]->Draw("box");
    TDayGrid->Draw("esame");
    hHVT[i0]->Draw("colzsame");
    c->Print(Form("%sHumVTColzB%02.f%s.pdf",DirRes_HousKep,BD[i0]*1.,TimeStr));
    c->Print(Form("%sHumVTColzB%02.f%s.png",DirRes_HousKep,BD[i0]*1.,TimeStr));
    PDFCanPage(pdfHKRec    ,c ,Form("Humidity Vs Time for Board %02.f",BD[i0]*1.),i0*2+1,NumBD*2);
    
    hXVTMod[1]->SetTitle(Form("Time Vs Humidity on MTB%02.f(center H sensor)",BD[i0]*1.));
    hXVTMod[1]->Draw("box");
    TDayGrid->Draw("esame");
    hTVT[i0]->Draw("colzsame");
    c->Print(Form("%sTemVTColzB%02.f%s.pdf",DirRes_HousKep,BD[i0]*1.,TimeStr));
    c->Print(Form("%sTemVTColzB%02.f%s.png",DirRes_HousKep,BD[i0]*1.,TimeStr));
    PDFCanPage(pdfHKRec    ,c ,Form("Temperature Vs Time for Board %02.f",BD[i0]*1.),i0*2+2,NumBD*2);
  }
  delete c;
  delete pdfHKRec;
  delete hXVTMod[0];
  delete hXVTMod[1];
  for(int i0=0;i0<NumBD;i0++){ 
    delete hTVT[i0];
    delete hHVT[i0];
  }
}



void HKTGAna(){
  
  Int_t    unixtime;//abs time of event [s]
  int tYear, tMonth, tDate, tHour, tMinute, tSecond;
  
  TH1F *hXVTMod[2];
  
  hXVTMod[0] = Mod_DateLabel(Form("hMXVT%d",0),"",10,60,"Temperature(#circC)");
  hXVTMod[1] = Mod_DateLabel(Form("hMXVT%d",1),"",10,100,"Humidity(RH%)");
  
  float DGx[ndivise]={},DGy[ndivise]={},DGex[ndivise]={},DGey[ndivise]={};
  for(int i0=0;i0<ndivise;i0++) {
    DGx[i0] = hXVTMod[0]->GetBinCenter(i0+1)+hXVTMod[0]->GetBinWidth(i0+1)/2.;
    DGy[i0] = 60;  DGey[i0] = 1000;  DGex[i0] = 0;
    // cout<<DGx[i0]<<"\t"<<DGy[i0]<<"\t"<<DGex[i0]<<"\t"<<DGey[i0]<<"\n";
  }
  TGraphErrors *TDayGrid = Mod_DateGrid(50,200);
  
  
  ifstream inTT[NumBD]={}, inHT[NumBD]={};
  for(int i0=0;i0<NumBD;i0++){
    inTT[i0].open(Form("%sTempVTB%02.f.dat",DirOperate,BD[i0]*1.));
    inHT[i0].open(Form("%sHumiVTB%02.f.dat",DirOperate,BD[i0]*1.));
  }
  
  TCanvas *c = new TCanvas("c","",1920,1080);
  TCanvas *cAT = new TCanvas("cAT","",1920,1080);
  TCanvas *cAH = new TCanvas("cAH","",1920,1080);
  c->cd();

  TPDF *pdfHKRec = new TPDF(Form("%sTi%05.fHKVTTG%s.pdf",DirRes_HousKep,th2fbinwidth,TimeStr));
  TPDF *pdfTRec = new TPDF(Form("%sTi%05.fTemVTTG%s.pdf",DirRes_TempVT,th2fbinwidth,TimeStr));
  TPDF *pdfHRec = new TPDF(Form("%sTi%05.fHumVTTG%s.pdf",DirRes_HumiVT,th2fbinwidth,TimeStr));
  PDFCanPage(pdfHKRec    ,c ,Form("Title"),1,NumBD*3);
  TGraph *gTVT[4], *gHVT[4],*gTVTLG[4], *gHVTLG[4];
  
  
  
  
  for(int i0=0;i0<NumBD;i0++){
    hXVTMod[0]->SetTitle(Form("Time Vs Tenperature on MTB%02.f(center T sensor)",BD[i0]*1.));
    hXVTMod[1]->SetTitle(Form("Time Vs Humidity on MTB%02.f(center H sensor)",BD[i0]*1.));
    
    
    float tmp_UT=0,tmp_tem=0,tmp_hum=0,value_tem,value_hum;
    int nT[4] ={}, nH[4]={};
    vector<float> tin, hin;
    vector<float> temin, humin; 
    for(;inTT[i0]>>tmp_UT;nT[i0]++){
      inTT[i0]>>tYear>>tMonth>>tDate>>tHour>>tMinute>>tSecond>>tmp_tem>>value_tem;
      tin.push_back(tmp_UT*1.);
      temin.push_back(tmp_tem*1.);
    }
    float tinArr[tin.size()];
    float teminArr[tin.size()];
    for(int i4=0;i4<nT[i0];i4++){
      tinArr[i4]=tin[i4];
      teminArr[i4]=temin[i4];
      //cout<<tin[i4]<<"\t"<<temin[i4];
    }
    
    for(;inHT[i0]>>tmp_UT;nH[i0]++){
      inHT[i0]>>tYear>>tMonth>>tDate>>tHour>>tMinute>>tSecond>>tmp_hum>>value_hum;
      hin.push_back(tmp_UT*1.);
      humin.push_back(tmp_hum*1.);
    }
    float hinArr[hin.size()];
    float huminArr[hin.size()];
    for(int i4=0;i4<nH[i0];i4++){
      hinArr[i4]=hin[i4];
      huminArr[i4]=humin[i4];
      //cout<<tin[i4]<<"\t"<<temin[i4];
    }
    
    gTVT[i0]= new TGraph(tin.size(),tinArr,teminArr);
    gHVT[i0]= new TGraph(hin.size(),hinArr,huminArr);
    
    gTVT[i0]->SetLineColor(ColorArr9[i0]);
    gHVT[i0]->SetLineColor(ColorArr9[i0+NumBD]);
    gTVT[i0]->SetMarkerStyle(20);
    gHVT[i0]->SetMarkerStyle(5);
    gTVT[i0]->SetMarkerColor(ColorArr9[i0]);
    gHVT[i0]->SetMarkerColor(ColorArr9[i0+NumBD]);
    
    gTVTLG[i0]= (TGraph*) gTVT[i0]->Clone();
    gHVTLG[i0]= (TGraph*) gHVT[i0]->Clone();
    gTVTLG[i0]->SetMarkerSize(4);
    gHVTLG[i0]->SetMarkerSize(4);
    
    c->cd();
    hXVTMod[0]->Draw("box");
    TDayGrid->Draw("pesame");
    gTVT[i0]->Draw("psame");
    c->Print(Form("%sTemVTTGB%02.f%s.pdf",DirRes_TempVT,BD[i0]*1.,TimeStr));
    c->Print(Form("%sTemVTTGB%02.f%s.png",DirRes_TempVT,BD[i0]*1.,TimeStr));
    PDFCanPage(pdfHKRec    ,c ,Form("Temperature Vs Time for Board %02.f",BD[i0]*1.),i0*2+1,NumBD*2+3);
    PDFCanPage(pdfTRec    ,c ,Form("Temperature Vs Time for Board %02.f",BD[i0]*1.),i0+1,NumBD);
    
    hXVTMod[1]->Draw("box");
    TDayGrid->Draw("pesame");
    gHVT[i0]->Draw("psame");
    c->Print(Form("%sHumVTTGB%02.f%s.pdf",DirRes_HumiVT,BD[i0]*1.,TimeStr));
    c->Print(Form("%sHumVTTGB%02.f%s.png",DirRes_HumiVT,BD[i0]*1.,TimeStr));
    PDFCanPage(pdfHKRec    ,c ,Form("Humidity Vs Time for Board %02.f",BD[i0]*1.),i0*2+2,NumBD*2+3);
    PDFCanPage(pdfHRec    ,c ,Form("Humidity Vs Time for Board %02.f",BD[i0]*1.),i0+1,NumBD);
    
    
    inTT[i0].close();
    inHT[i0].close();
  }
  TLegend *LHK = new TLegend(0.3,0.8,0.9,0.9);
  LHK->SetFillColor(0);
  LHK->SetLineColor(1);
  LHK->SetNColumns(NumBD);
  TLegend *LTT = (TLegend*) LHK->Clone();
  TLegend *LHT = (TLegend*) LHK->Clone();

  cAT->cd();
  hXVTMod[0]->Draw("box");
  TDayGrid->Draw("pesame");
  for(int i0=0;i0<NumBD;i0++){
    gTVT[i0]->Draw("psame");
    
    LHK->AddEntry(gTVTLG[i0],Form("Temp.MTB%02.f",BD[i0]*1.),"p");
    LTT->AddEntry(gTVTLG[i0],Form("Temp.MTB%02.f",BD[i0]*1.),"p");
  }
  LTT->Draw();
  PDFCanPage(pdfHKRec    ,cAT ,Form("Temperature Vs Time for All Board"),NumBD*2+1,NumBD*2+3);
  
  cAH->cd();
  hXVTMod[1]->Draw("box");
  TDayGrid->Draw("pesame");
  for(int i0=0;i0<NumBD;i0++){
    gHVT[i0]->Draw("psame");
    LHK->AddEntry(gHVTLG[i0],Form("Humi.MTB%02.f",BD[i0]*1.),"p");
    LHT->AddEntry(gHVTLG[i0],Form("Humi.MTB%02.f",BD[i0]*1.),"p");
  } 
  LHT->Draw();
  PDFCanPage(pdfHKRec    ,cAH ,Form("Humidity Vs Time for All Board"),NumBD*2+2,NumBD*2+3);

  
  
  hXVTMod[0]->Draw("box");
  TDayGrid->Draw("pesame");

  LHK->Draw("same");
  for(int i0=0;i0<NumBD;i0++){
    gTVT[i0]->Draw("psame");
    for(int i1=0; i1<gHVT[i0]->GetN(); i1++){
      double x,y;
      gHVT[i0]->GetPoint(i1,x,y);
      gHVT[i0]->SetPoint(i1,x,y/2.);
    }
    gHVT[i0]->Draw("psame");

  } 
  
  TGaxis *ARHT = new TGaxis(cAT->GetUxmax(),cAT->GetUymin(), cAT->GetUxmax(), 50,cAT->GetUymin()*2.,100,110,"+L");//@@反了??????
  ARHT->SetTitle("Humidity(RH%)");
  ARHT->SetLabelSize(0.025); 
  ARHT->Draw("same");
  
  
  PDFCanPage(pdfHKRec    ,cAH ,Form("HK Vs Time for All Board"),NumBD*2+3,NumBD*2+3);
  
  
  
  
  
  
  
  
  
  
}