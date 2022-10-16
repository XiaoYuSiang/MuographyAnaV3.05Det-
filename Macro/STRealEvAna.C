#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TText.h>
#include <TFile.h>
#include <TPad.h>
#include <TTree.h>
#include <TColor.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include "/data4/YuSiang/personalLib/Style/DBMTStyle.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"
#include "DSLData.h"
#include "LoadODectTable.h"

using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace DataConst;

void STRealEvAna(const int indexi=28){
  // setTDRStyle();
  defstyle();
  gStyle->SetPadRightMargin(0.02);
  map<int, scintillator> MScints = LoadODectTable();
  
  char NoiseStr[20] = {};
  sprintf(NoiseStr,"_NT%dNF%d",TriggerNHitOfEffB3,TriggerNHitOfEffB4);
  char RFNT[200];
  sprintf(RFNT,"%sTracksGT%d_%s_nHTH%dL%d.root",DirOperate,eventGapTcnt[indexi],TimeStr,TriggerS,TriggerL);
  cout<<"Root file: "<<Form("%s",RFNT)<<" is opening!"<<endl;
  TChain *t = new TChain("t");
  t->Add(Form("%s",RFNT));
  
  ofstream out(Form("%sCheff%s_%s.txt",DirRes_ChEff,NoiseStr,TimeStr));
  ofstream out1(Form("%sCheff%s_%s.csv",DirRes_ChEff,NoiseStr,TimeStr));
  out<<"ch\t=\"1111\"\t=\"0111\"\t=\"1011\"\t=\"1101\"\t=\"1110\"\tEff.L0\tEff.L1\tEff.L2\tEff.L3"<<endl;
  
  char DirRes_EffDetail[180], cmdline[180];
  sprintf(DirRes_EffDetail,"%sDetail/",DirRes_ChEff);
  sprintf(cmdline,"mkdir -p %s",DirRes_EffDetail);
  if (system(cmdline)!=0) system(cmdline);
  
  char histName[NumLY+1][15]={
    "H3_X123", "H3_0X23", "H3_01X3", "H3_012X", "H4_0123"
  };
  
  
  
  
  TH1F  *hMod       = new TH1F("hMod"     ,"",NumCh,0,NumCh);
  TH1F  *H3_X123    = new TH1F(histName[0],"",NumCh,0,NumCh);
  TH1F  *H3_0X23    = new TH1F(histName[1],"",NumCh,0,NumCh);
  TH1F  *H3_01X3    = new TH1F(histName[2],"",NumCh,0,NumCh);
  TH1F  *H3_012X    = new TH1F(histName[3],"",NumCh,0,NumCh);
  TH1F  *H4_0123    = new TH1F(histName[4],"",NumCh,0,NumCh);
  TH1F  *H3_0123[NumLY  ]={H3_X123,H3_0X23,H3_01X3,H3_012X};
  TH1F  *HN_AALL[NumLY+1]={H3_X123,H3_0X23,H3_01X3,H3_012X,H4_0123};
  
  TLegend *pwdVSddt = new TLegend(0.15,.76,.96,0.9);
  setLegendDefault(pwdVSddt,5);
  for(int iLy=0;iLy<NumLY;iLy++) 
    pwdVSddt->AddEntry(H3_0123[iLy],Form("nH==3 no iZ = %d",iLy),"FEl");
  pwdVSddt->AddEntry(H4_0123,"nH==4 all Pass","E1X0p");
  
  TLegend *pwdVSddt1 = new TLegend(0.23,.76,.93,0.9);
  setLegendDefault(pwdVSddt1,4);
  for(int iLy=0;iLy<NumLY;iLy++) 
    pwdVSddt1->AddEntry(H3_0123[iLy],Form("Efficiency of iZ = %d",iLy),"FEl");
  
  hMod->GetYaxis()->SetTitle("Event Number");
  hMod->GetXaxis()->SetTitle("Channel ID");
  hMod->GetXaxis()->SetTitleSize(0.05);
  hMod->GetYaxis()->SetTitleSize(0.05);
  hMod->GetXaxis()->SetTitleOffset(1.2);
  hMod->GetYaxis()->SetTitleOffset(1.1);
  hMod->SetStats(0);  

  TH1F *hMod2 = (TH1F*) hMod->Clone();
  
  for(int ichs=0;ichs<NumCh;ichs++){
    hMod2->Fill(ichs);
    hMod->GetXaxis()->SetBinLabel(ichs+1,Form("ch.%d",ichs));
    hMod2->GetXaxis()->SetBinLabel(ichs+1,Form("ch.%d",ichs));
  }
  
  hMod2->GetYaxis()->SetRangeUser(0.5,1.1);
  hMod2->GetYaxis()->SetTitle("Channel Efficiency");
  hMod2->SetLineStyle(2);
  
  TCanvas *cEVEFAll = new TCanvas("cEVEFAll","",1920*NumBX,2160*NumBY);
  TCanvas *cNUMSAll = new TCanvas("cNUMSAll","",1920*NumBX,1080*NumBY);
  TCanvas *cEVESAll = new TCanvas("cEVESAll","",1920*NumBX,1080*NumBY);
  cEVEFAll ->Divide(NumBX,NumBY);
  cNUMSAll ->Divide(NumBX,NumBY);
  cEVESAll ->Divide(NumBX,NumBY);
  
  
  for(int ibx=0; ibx<NumBX; ibx++){
    for(int iby=0; iby<NumBY; iby++){
      char layImfor[40];
      sprintf(layImfor,"-%dN+%d (BiX,BiY) = (%d,%d)",NumBX*NumBY,LayArr[ibx][iby],ibx,iby);
      hMod->SetTitle(Form("Event number of column%s",layImfor));
      hMod2->SetTitle(Form("Efficiency of column%s",layImfor));
      
      
      
      TCanvas *cTMP = new TCanvas("cTMP","",1920,1920);
      TCanvas *cevent = new TCanvas("cevent","",1920,2160);
      cevent ->Divide(1,2);
      cTMP->cd();
      
      hMod->Draw();
      pwdVSddt->Draw();
      // cout<<TriggerNHitOfEff<<endl;
      // throw;
      char inputForm[30]; int selectBLay[NumLY+1]={14,13,11,7,15};
      sprintf(inputForm,"channel");
      for(int icase=0;icase<NumLY;icase++) {
        t->Draw( Form("%s>>%s",inputForm,histName[icase]),
           Form("BLay==%d && eventHit<=%d && BiX==%d&&BiY==%d", selectBLay[icase],TriggerNHitOfEffB3+3,ibx,iby), "histbarSame");
           
        cout<<H3_0123[icase]->GetName()<<": "<<H3_0123[icase]->Integral(-1,-1)<<endl;
      }
      // throw;
      t->Draw( Form("%s>>%s",inputForm,histName[NumLY]),
         Form("BLay==%d && eventHit<=%d",// && BiX==%d&&BiY==%d",
         selectBLay[NumLY],TriggerNHitOfEffB4+4,ibx,iby),  "e1x0pSame");
      cout<<H4_0123->GetName()<<": "<<H4_0123->Integral(-1,-1)<<endl;

      TGraphErrors *TGEFFErr[NumLY];
      // throw;
      for(int iLy=0;iLy<NumLY;iLy++){
        H3_0123[iLy]->SetFillColor(ColorArr17[iLy]);
        H3_0123[iLy]->SetLineColor(ColorArr17[iLy]);
        H3_0123[iLy]->SetBarWidth((1./(NumLY+1))*0.9);
        H3_0123[iLy]->SetBarOffset((0.5/(NumLY+1))*1.1+(1./(NumLY+1))*iLy);
        H3_0123[iLy]->SetLineWidth(2);
        H3_0123[iLy]->SetMarkerStyle(0);
        // H3_0123[iLy]->Scale();
        H3_0123[iLy]->Sumw2();
        double x[NumCh]={}, y[NumCh]={}, ex[NumCh]={}, ey[NumCh]={};
        // cout<<histName[iLy]<<endl;
        for(int ichs=0;ichs<NumCh;ichs++){
          x[ichs]=  ichs+(1./(NumLY+1))*(iLy+1);
          
          y[ichs]=  H3_0123[iLy]->GetBinContent(ichs+1);
          ey[ichs]= H3_0123[iLy]->GetBinError(ichs+1);
          // cout<<x[ichs]<<"  "<<H3_0123[iLy]->GetBinContent(ichs+1)<<"  "<<ex[ichs]<<"  "<<H3_0123[iLy]->GetBinError(ichs+1)<<"  "<<endl;
        }

        TGEFFErr[iLy] = new TGraphErrors(NumCh,x,y,ex,ey);
        TGEFFErr[iLy] ->Draw("esame");
        TGEFFErr[iLy] ->SetLineColor(ColorArr17[iLy]);
        TGEFFErr[iLy] ->SetLineStyle(1);
        TGEFFErr[iLy] ->SetMarkerStyle(104);
        TGEFFErr[iLy] ->SetMarkerColor(ColorArr17[iLy]);

      }
      
      H4_0123 ->SetMarkerStyle(4);
      H4_0123 ->SetMarkerSize(0.5);
      H4_0123 ->SetLineColor(1);
      // H4_0123->Scale(1/4.);
      H4_0123->Sumw2();
      int NMax = 0, NMin = 1.E+9, tmpExt;
      for(int icase=0;icase<NumLY+1;icase++){
        tmpExt = HN_AALL[icase]->GetMaximum();
        if(tmpExt>NMax) NMax = tmpExt;
        tmpExt = HN_AALL[icase]->GetMinimum();
        if(tmpExt<NMin) NMin = tmpExt;
      } 
      hMod->GetYaxis()->SetRangeUser(NMin,NMax*1.2);

      TH1F *H3_X123E  = (TH1F*) H3_X123->Clone();
      TH1F *H3_0X23E  = (TH1F*) H3_0X23->Clone();
      TH1F *H3_01X3E  = (TH1F*) H3_01X3->Clone();
      TH1F *H3_012XE  = (TH1F*) H3_012X->Clone();
      TH1F *H4_0123E1 = (TH1F*) H4_0123->Clone();
      TH1F *H4_0123E3 = (TH1F*) H4_0123->Clone();
      TH1F *H4_0123E5 = (TH1F*) H4_0123->Clone();
      TH1F *H4_0123E4 = (TH1F*) H4_0123->Clone();
      TH1F *H4_0123E  = (TH1F*) H4_0123->Clone();
      
      H3_X123E->Add(H4_0123E);
      H3_0X23E->Add(H4_0123E);
      H3_01X3E->Add(H4_0123E);
      H3_012XE->Add(H4_0123E);
      H4_0123E1->Divide(H3_X123E);
      H4_0123E3->Divide(H3_0X23E);
      H4_0123E5->Divide(H3_01X3E);
      H4_0123E4->Divide(H3_012XE);

      TH1F  *H0123E[NumLY]={H4_0123E1,H4_0123E3,H4_0123E5,H4_0123E4};


      for(int ichs=0;ichs<NumCh;ichs++){
        out<<"ch"<<ichs<<"\t";
        out<<H4_0123->GetBinContent(ichs+1)<<"\t";
        out<<H3_X123->GetBinContent(ichs+1)<<"\t";
        out<<H3_0X23->GetBinContent(ichs+1)<<"\t";
        out<<H3_01X3->GetBinContent(ichs+1)<<"\t";
        out<<H3_012X->GetBinContent(ichs+1)<<"\t";
        out<<H4_0123E1->GetBinContent(ichs+1)<<"\t";
        out<<H4_0123E3->GetBinContent(ichs+1)<<"\t";
        out<<H4_0123E5->GetBinContent(ichs+1)<<"\t";
        if(ichs==NumCh-1) out<<H4_0123E4->GetBinContent(ichs+1);
        else out<<H4_0123E4->GetBinContent(ichs+1)<<endl;
      }


      cevent ->cd(1);
      cTMP->DrawClonePad();
      
      cNUMSAll->cd(1+ibx+2-NumBX*iby);
      cTMP    ->DrawClonePad();
      
      cTMP->cd();
      hMod2->Draw("l");
      pwdVSddt1->Draw();
      
      //TGraphErrors *EFFErr[4]={};
      for(int iLy=0;iLy<NumLY;iLy++){
        double  x[NumCh]={},   y[NumCh]={};
        double ex[NumCh]={}, eyl[NumCh]={}, eyh[NumCh]={};
        for(int ichs=0;ichs<NumCh;ichs++){
          x[ichs]  = ichs+(1./(NumLY+1))*(1+iLy);
          y[ichs]  = H0123E[iLy]->GetBinContent(ichs+1);
          eyl[ichs]= H0123E[iLy]->GetBinError(ichs+1);
          eyh[ichs]= H0123E[iLy]->GetBinError(ichs+1);
          if      (y[ichs]-eyl[ichs]<=0) eyl[ichs]=y[ichs];
          else if(y[ichs]+eyh[ichs]>=1) eyh[ichs]=1-y[ichs];
          out1<<BD[iLy]<<"\t"<<x[ichs]<<"\t"<<y[ichs]<<"\t"<<eyl[ichs]<<"\t"<<eyh[ichs]<<"\n";
          // cout<<x[ichs]<<"  "<<H0123E[iLy]->GetBinContent(ichs+1)<<"  "<<ex[ichs]<<"  "<<H0123E[iLy]->GetBinError(ichs+1)<<"  "<<endl;
        }
        //cout<<H0123E[iLy]->GetBinContent(1);
        H0123E[iLy]->Draw("histbarSame");
        TGraphAsymmErrors *EFFErrAsy = new TGraphAsymmErrors(NumCh,x,y,ex,ex,eyl,eyh);
        EFFErrAsy->Draw("esame");
        EFFErrAsy->SetLineColor(ColorArr17[iLy]);
        EFFErrAsy->SetLineStyle(1);
        EFFErrAsy->SetMarkerStyle(104);
        EFFErrAsy->SetMarkerColor(ColorArr17[iLy]);
        H0123E[iLy]->SetFillColor(ColorArr17[iLy]);
        H0123E[iLy]->SetBarWidth((1./(NumLY+1))*0.9);
        H0123E[iLy]->SetBarOffset((0.5/(NumLY+1))*1.1+(1./(NumLY+1))*iLy);
        H0123E[iLy]->SetLineColor(ColorArr17[iLy]);
      }
      hMod2->Draw("lsame");

      DrawPdfPng(cevent,Form("%sCheff%s_%sBiX%dBiY%d",DirRes_EffDetail,NoiseStr,TimeStr,ibx,iby));

      out.close();
      out1.close();
      
      cevent ->cd(2);
      cTMP->DrawClonePad();
      
      cEVESAll->cd(1+ibx+2-NumBX*iby);
      cTMP    ->DrawClonePad();
      
      cEVEFAll->cd(1+ibx+2-NumBX*iby);
      cevent->DrawClonePad();
      
      cTMP->Close();
      cevent->Close();
    }
  }
  system(Form("cp %sCheff%s_%s.txt  %sCheff%s_%s.txt",DirRes_ChEff,NoiseStr,TimeStr,DirOperate,NoiseStr,TimeStr));
  system(Form("cp %sCheff%s_%s.csv  %sCheff%s_%s.csv",DirRes_ChEff,NoiseStr,TimeStr,DirOperate,NoiseStr,TimeStr));
  cout<<(Form("%sCheff%s_%s.csv/.txt has been freated",DirRes_ChEff,NoiseStr,TimeStr,DirOperate))<<endl;
  
  DrawPdfPng(cEVEFAll,Form("%sChNE%s_%s",DirRes_ChEff,NoiseStr,TimeStr));
  DrawPdfPng(cNUMSAll,Form("%sChNum%s_%s",DirRes_ChEff,NoiseStr,TimeStr));
  DrawPdfPng(cEVESAll,Form("%sChEff%s_%s",DirRes_ChEff,NoiseStr,TimeStr));
  

}