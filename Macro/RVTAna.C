 #include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TF1.h>
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


//channel vs rate combine
void RVTAna() {
  vector<string> vRootFiles = RawRootList(DirOperate);
  TreeReader data(vRootFiles);
  ofstream out(Form("%sRVT.dat",DirOperate));
  ofstream report(Form("%sReport_RVChError_%s.dat",DirRes_RatVCh,TimeStr));

  //take time
  TH1F *hBT  = new TH1F("hBT","",BinNumOnCha,0,BinNumOnCha);
  data.GetEntry(0);

  //Float_t th2fbinwidth = 60.*30.;

  TH2F *hRT   = new TH2F("hRT","",BinOnTimeD,FirDaySec,FinDaySec,BinNumOnCha,0,BinNumOnCha);
  TH2F *hMod2 = Mod_DateLabel("hMod2","",BinNumOnCha,0,BinNumOnCha,"General Channel ID","Rate(s^{-1} )");
  TGraphErrors *TDayGrid = Mod_DateGrid(0, BinNumOnCha+1);


  float maxcon = 0,maxconRt=0;
  float mincon = 100,minconRt=100;
  //evs = 1600;
  
  Int_t    unixtime   ;
  Int_t    tYear      ;
  Int_t    tMonth     ;
  Int_t    tDate      ;
  Int_t    tHour      ;
  Int_t    tMinute    ;
  Int_t    tSecond    ;



  for (Long64_t ev = 0; ev <evs; ++ev) {//evs; ++ev) {
    data.GetEntry(ev);
          unixtime  = data.GetLong64("unixtime");
    Int_t    nHits     = data.GetInt("nHits");
    Int_t*   board     = data.GetPtrInt("board");
    Int_t*   channel   = data.GetPtrInt("channel");

    for(int i1 = 0; i1<nHits ;++i1){
      int ibd = BDcheck(board[i1]), ich = channel[i1];
      hBT->Fill(ich+ibd*NumCh,RateWei);
      hRT->Fill(unixtime,ich+ibd*NumCh,1./th2fbinwidth);
    }
  }
  
  
  float minSetUser, maxSetUser;
  float minSetUserRT, maxSetUserRT;
  FindYRangeTH1F(hBT,minSetUser,maxSetUser,1,0.1,2);
  FindYRangeTH2F(hRT,minSetUserRT,maxSetUserRT,1,0.1,2);
  
  
  int CSizeX = BinNumOnCha*30, CSizeY = BinNumOnCha*16.875; //64ch = 1920:1080
  TCanvas *c1 = new TCanvas("c1","",CSizeX,CSizeY);
  TH1F *hMod  = new TH1F("hMod","",BinNumOnCha,0,BinNumOnCha);

  c1->cd();

  hMod->SetStats(0);
  hMod->SetTitle("");
  hMod->GetXaxis()->SetTitle("General ChID");
  hMod->GetYaxis()->SetTitle("rate(s^{-1} )");
  // hMod->GetXaxis()->SetRangeUser(-1,65);
  hMod->GetYaxis()->SetRangeUser(minSetUser,maxSetUser);//maxcon*1.2);
  hMod->GetYaxis()->SetTitleOffset(1.4);
  hMod->GetYaxis()->SetNdivisions(5);
  hBT->SetLineColor(4);
  hBT->SetLineWidth(2);

  hMod->Draw();
  hBT->Draw("hsame");
  
  c1->cd();

  TF1 *f16=new TF1("f16","x",-0.5,NumCh-0.5);
  TGaxis *AxisID[NumBD];
  
  report << "report: unusual Rate of Channel list report:"<<endl;
  report<<"--------------------------------------------------"<<endl;
  report<<"BDID\tMean\t1.5RMS"<<endl;
  report<<"GChID\tBDID\tChID\tLorH\tRate"<<endl;
  report<<"--------------------------------------------------"<<endl;

  TLegend *L[NumBD];
  for(int i1=0;i1<NumBD;i1++){
    float RateMean = 0,RateRMS = 0;
    for(int i2=i1*NumCh;i2<NumCh+i1*NumCh;i2++){
      RateMean += hBT->GetBinContent(i2+1);
    }
    RateMean /= NumCh;
    for(int i2=i1*NumCh;i2<NumCh+i1*NumCh;i2++){
      RateRMS += pow(hBT->GetBinContent(i2+1)-RateMean,2);
    }
    RateRMS /= NumCh;
    RateRMS = sqrt(RateRMS);
    report<<BD[i1]<<"\t"<<RateMean<<"\t"<<RateRMS*1.5<<endl;
    vector<float> vxerrH,vyerrH,vxerrL,vyerrL;
    for(int i2=i1*NumCh;i2<NumCh+i1*NumCh;i2++){
      if( hBT->GetBinContent(i2+1) > RateMean+RateRMS*Trigger_RateRMS ){
        vxerrH .push_back( hBT->GetBinCenter(i2+1));
        vyerrH .push_back( hBT->GetBinContent(i2+1));
        report<<i2<<"\t"<<BD[i1]<<"\t"<<i2%NumCh<<"\t"<<"H"<<"\t"<<hBT->GetBinContent(i2+1)<<endl;
      }
      else if( hBT->GetBinContent(i2+1) < RateMean-RateRMS*Trigger_RateRMS ){
        vxerrL .push_back( hBT->GetBinCenter(i2+1));
        vyerrL .push_back( hBT->GetBinContent(i2+1));
        report<<i2<<"\t"<<BD[i1]<<"\t"<<i2%NumCh<<"\t"<<"L"<<"\t"<<hBT->GetBinContent(i2+1)<<endl;
      }
    }
    
    report<<"--------------------------------------------------"<<endl;
    int iHErr = vxerrH.size(),iLErr = vxerrL.size();
    float xerrH[iHErr],yerrH[iHErr],xerrL[iLErr],yerrL[iLErr];
    for(int i2=0;i2<iHErr;i2++) {
      xerrH[i2] = vxerrH[i2];
      yerrH[i2] = vyerrH[i2];
    }
    for(int i2=0;i2<iLErr;i2++) {
      xerrL[i2] = vxerrL[i2];
      yerrL[i2] = vyerrL[i2];
    }
    if(iHErr!=0){
      TGraph *ErrPH = new TGraph(iHErr,xerrH,yerrH);
      ErrPH->SetMarkerStyle(22);
      ErrPH->SetMarkerColor(2);
      ErrPH->SetMarkerSize(3);
      ErrPH->Draw("psame");
    } 
    if(iLErr!=0){
      TGraph *ErrPL = new TGraph(iLErr,xerrL,yerrL);
      ErrPL->SetMarkerStyle(23);
      ErrPL->SetMarkerColor(4);
      ErrPL->SetMarkerSize(3);
      ErrPL->Draw("psame");
    }
    
    //@@TLATEX
    L[i1] = new TLegend(0.10+0.80/(NumBD*1.)*i1,0.82,0.10+0.80/(NumBD*1.)*(1+i1),0.9);
    //L[i1]->SetNColumns(4);
    L[i1]->SetLineColorAlpha (0, 0.00);
    L[i1]->SetFillColorAlpha (0, 0.00);
    L[i1]->AddEntry("hBT",Form("MTB-%d (GChID: %02.f~%02.f)",BD[i1],i1*NumCh*1.,(i1+1)*NumCh*1.-1),"F");
    L[i1]->AddEntry("hBT",Form("    Mean: %.2f  RMS: %.2f",RateMean,RateRMS),"h");
    L[i1]->Draw();
  }
  for(int i1=0;i1<NumBD;i1++){
    AxisID[i1] = new TGaxis(0+i1*NumCh        ,maxSetUser/1.1
                           ,NumCh+i1*NumCh    ,maxSetUser/1.1
                           ,"f16",20,"-");
    AxisID[i1]->SetTitle("ChID");
    AxisID[i1]->SetTitleSize(0.02/((1.*NumBD)/4.));
    AxisID[i1]->SetLabelSize(0.02/((1.*NumBD)/4.));
    AxisID[i1]->SetTitleOffset(1.6);
    //AxisID[i1]->SetLabelOffset(0.04/((1.*NumBD)/4.));
    AxisID[i1]->Draw();
  }
  
  int Bx[NumBD+2][2]={};
  Bx[NumBD+1][0] = 0; Bx[NumBD+1][1] = 10;
  TGraph *BVL[NumBD+1];
  for(int i1=1;i1<NumBD;i1++){
    Bx[i1][0] = NumCh*i1;  Bx[i1][1] = NumCh*i1;
    BVL[i1] = new TGraph(2,Bx[i1],Bx[NumBD+1]);
    BVL[i1]->SetLineStyle(2);
    BVL[i1]->SetLineWidth(2);
    BVL[i1]->Draw("samel");
  }
  
  c1->Print(Form("%sRVChBox_%s.png",DirRes_RatVCh,TimeStr));
  c1->Print(Form("%sRVChBox_%s.pdf",DirRes_RatVCh,TimeStr));

  c1->Close();
  //c1->Close();
  
  TCanvas *c2 = new TCanvas("c2","",CSizeX*2,CSizeY*2);
  c2->cd();
  hRT->SetStats(0);
  hRT->GetXaxis()->SetTitle("Date");
  hRT->GetYaxis()->SetTitle("General ChID");
  hRT->GetXaxis()->SetTitleOffset(1.5);
  hRT->GetYaxis()->SetTitleOffset(1.3);
  hRT->GetZaxis()->SetLabelOffset(0.003);
  hRT->GetXaxis()->SetLabelOffset(0.002);
  hRT->GetZaxis()->SetTitleSize(0.04);
  hRT->GetYaxis()->SetTitleSize(0.04);
  hRT->GetZaxis()->SetTitle("Rate(s^{-1})");
  hRT->GetZaxis()->SetLabelSize(0.025);
  hRT->GetXaxis()->SetLabelSize(0.03);
  hRT->GetYaxis()->SetLabelSize(0.035);
  hRT->GetXaxis()->SetNdivisions(-ndivise+2);
  hRT->GetXaxis()->SetRangeUser(Dayi,Dayf);
  hRT->GetZaxis()->SetRangeUser(0,maxSetUserRT);
  hRT->GetXaxis()->CenterTitle(true);
  hRT->GetYaxis()->CenterTitle(true);
  hRT->GetZaxis()->CenterTitle(true);
  cout<<"maxSetUser"<<maxSetUser<<endl;
  hRT->GetZaxis()->SetNdivisions(-005);
  hRT->GetZaxis()->SetTitleOffset(0.6);
  hRT->SetLineColor(1);
  
  for (int i3=0;i3<BinNumOnCha;i3+=NumBD){
    hRT->GetYaxis()->SetBinLabel(i3+1 ,Form("%d",i3));
  }
  cout<<"The during days is:  "<<ndivise+1<<endl;
  out<<"The during days is:  "<<ndivise+1<<endl;
  
  TH2F *hRTLego = (TH2F*) hRT->Clone();
  hRT->GetZaxis()->RotateTitle(true);
  int rotateRate = 6;
  for (int i3=0;i3<(ndivise+1);i3++){
    unixtime = hRT->GetXaxis()->GetBinCenter(1+86400/th2fbinwidth*i3);
    unixTimeToHumanReadable(unixtime, tYear, tMonth, tDate, tHour, tMinute, tSecond,timeZone);
    char str_MD[30] ={};
    sprintf(str_MD , "%s-%02.f",MonthName[tMonth],1.*tDate);
    if(tDate==1||tDate==15||i3==0||i3==ndivise||(ndivise+1)<32){
      if(i3 == ndivise){
             hRT    ->GetXaxis()->SetBinLabel(86400/th2fbinwidth*i3 ,str_MD);
             hRTLego->GetXaxis()->SetBinLabel(86400/th2fbinwidth*i3/rotateRate ,str_MD);
      }else{
        hRT    ->GetXaxis()->SetBinLabel(1+86400/th2fbinwidth*i3 ,str_MD);
        hRTLego->GetXaxis()->SetBinLabel(1+86400/th2fbinwidth*i3/rotateRate ,str_MD);
      }
    }
    cout<<"i3:    "<<i3<<"    "<<unixtime<<"    "<<str_MD<<endl;
  }
  hMod2->Draw("box");
  hRT->Draw("colzsame");
  TDayGrid->Draw("esame");
  c2->Print(Form("%sRVTAllBcolz_%s.png",DirRes_RatVT,TimeStr));
  c2->Print(Form("%sRVTAllBcolz_%s.pdf",DirRes_RatVT,TimeStr));



  hRTLego->Draw("lego2");
  hRTLego->GetZaxis()->SetTitleOffset(1.2);
  hRTLego->GetZaxis()->SetRangeUser(0,maxSetUserRT);
  c2->Print(Form("%sRVTAllBLego2_%s.png",DirRes_RatVT,TimeStr));
  c2->Print(Form("%sRVTAllBLego2_%s.pdf",DirRes_RatVT,TimeStr));

  out.close();


}