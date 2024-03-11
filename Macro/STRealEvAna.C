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
#include <sstream>
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
char histName[NumLY+1][15];

char DirRes_EffDetail[180]={""};
char DirRCheffD[200]={""}, DirOCheffD[200]={""};
char DirRCheffDetBXY[200]={""}, DirOCheffSep[200]={""};
char DirOCheffCNE[200]={""}, DirOCheffCN[200]={""}, DirOCheffCE[200]={""};
void SetDirOfEffs(){
  sprintf(DirRes_EffDetail,"%sDetail/",DirRes_ChEff);
  sprintf(DirRCheffD,"%sCheffData/",DirRes_ChEff);
  sprintf(DirOCheffD,"%sCheffData/",DirOperate);
  sprintf(DirRCheffDetBXY,"%sCheffBxBy/",DirRes_EffDetail);
  sprintf(DirOCheffSep,"%sCheffdata/",DirOperate);
  sprintf(DirOCheffCNE,"%sChNE/" ,DirRes_ChEff);
  sprintf(DirOCheffCN, "%sChNum/",DirRes_ChEff);
  sprintf(DirOCheffCE, "%sChEff/",DirRes_ChEff);
  locatedDirbyPath(DirRes_EffDetail);
  locatedDirbyPath(DirRCheffD);
  locatedDirbyPath(DirOCheffD);
  locatedDirbyPath(DirRCheffDetBXY);
  locatedDirbyPath(DirOCheffSep);
  locatedDirbyPath(DirOCheffCNE);
  locatedDirbyPath(DirOCheffCN);
  locatedDirbyPath(DirOCheffCE);
}

void SetHistName(){
  for(int il=0;il<NumLY;il++){
    stringstream iss;
    iss<<"H"<<NumLY-1<<"_";
    for(int ilx=0;ilx<NumLY;ilx++){
      if(il==ilx) iss<<"X";
      else iss<<ilx;
    }
    sprintf(histName[il],iss.str().data());
  }
  stringstream iss;
  iss<<"H"<<NumLY<<"_";
  for(int il=0;il<NumLY;il++) iss<<il;
  sprintf(histName[NumLY],iss.str().data());
  for(int il=0;il<=NumLY;il++)
    cout<<histName[il]<<endl;
}

void STRealEvAna(const int indexi=28){
  cout<<"Please use the last new version STRealEvAnaV2"<<endl;
}
TH1F  *hMod, *hMod2;
void SethMod(){
  hMod = new TH1F("hMod"     ,"",NumCh,0,NumCh);
  hMod->GetYaxis()->SetTitle("Event Number");
  hMod->GetXaxis()->SetTitle("Channel ID");
  hMod->GetXaxis()->SetTitleSize(0.05);
  hMod->GetYaxis()->SetTitleSize(0.05);
  hMod->GetXaxis()->SetTitleOffset(1.2);
  hMod->GetYaxis()->SetTitleOffset(1.1);
  hMod->SetStats(0);
  hMod2 = (TH1F*) hMod->Clone();
  hMod2->SetName("hMod2");
  for(int ichs=0;ichs<NumCh;ichs++){
    hMod2->Fill(ichs);
    hMod->GetXaxis()->SetBinLabel(ichs+1,Form("ch.%d",ichs));
    hMod2->GetXaxis()->SetBinLabel(ichs+1,Form("ch.%d",ichs));
  }
}
void STRealEvAnaV2(const int indexi=28,const bool sepRun = false){
  defstyle();
  gStyle->SetPadRightMargin(0.02);
  map<int, scintillator> MScints = LoadODectTable();
  SetHistName();

  char NoiseStr[20] = {};
  char RFNStr[200] = {};
  sprintf(NoiseStr,"_NT%dNF%d",TriggerNHitOfEffB3,TriggerNHitOfEffB4);

  if(sepRun){
    sprintf(RFNStr,"%d_%s_nHTH%dL%dCRun",eventGapTcnt[indexi],TimeStr,TriggerS,TriggerL);
    SetDirOfEffs();
  }
  else
    sprintf(RFNStr,"%d_%s_nHTH%dL%d",eventGapTcnt[indexi],TimeStr,TriggerS,TriggerL);
  
  SethMod();
  
  TH1F  *H3_X123    = new TH1F(histName[0],"",NumCh,0,NumCh);
  TH1F  *H3_0X23    = new TH1F(histName[1],"",NumCh,0,NumCh);
  TH1F  *H3_01X3    = new TH1F(histName[2],"",NumCh,0,NumCh);
  TH1F  *H3_012X    = new TH1F(histName[3],"",NumCh,0,NumCh);
  TH1F  *H4_0123    = new TH1F(histName[4],"",NumCh,0,NumCh);
  TH1F  *H3_0123[NumLY  ]={H3_X123,H3_0X23,H3_01X3,H3_012X};
  TH1F  *HN_AALL[NumLY+1]={H3_X123,H3_0X23,H3_01X3,H3_012X,H4_0123};
  for(int iLy=0;iLy<=NumLY;iLy++)
    HN_AALL[iLy]->Sumw2();
  
  for(int irun=0; irun<(sepRun?RunNums:1); irun++){
  // for(int irun=0; irun<(sepRun?2:1); irun++){
    int tmpRID = RunID[irun];
    char RunStr[200]; sprintf(RunStr,sepRun ?Form("_Run%02.f",tmpRID*1.):"");
    string TrackData = 
      sepRun ? Form("%sTracksGT%s%d.root",DirOperate,RFNStr,tmpRID)
              : Form("%sTracksGT%s.root",DirOperate,RFNStr);
    char RFNT[200]; sprintf(RFNT,TrackData.data());
    if(FileSize(TrackData.data())<=0){
      cout<<"Skip this null run, run: "<<tmpRID<<endl;
      continue;
    } 
    cout<<"Root file: "<<Form("%s",RFNT)<<" is opening!"<<endl;
    TChain *t = new TChain("t");
    t->Add(Form("%s",RFNT));
    
    ofstream out(Form("%sCheff%s_%s%s.txt",DirRCheffD,NoiseStr,TimeStr,RunStr));
    ofstream out1(Form("%sCheff%s_%s%s.csv",DirRCheffD,NoiseStr,TimeStr,RunStr));
    ofstream out2(Form("%sCheffL%d%s_%s%s.csv",DirOCheffD,int(TriggerChanEff*100),NoiseStr,TimeStr,RunStr));
    out<<"ch\t=\"1111\"\t=\"0111\"\t=\"1011\"\t=\"1101\"\t=\"1110\"\tEff.L0\tEff.L1\tEff.L2\tEff.L3"<<endl;
    out2<<"#BID	CID	GID	Eff%	Sta	UCL%	UCH%"<<endl;

    TLegend *pwdVSddt = new TLegend(0.15,.76,.96,0.9);
    setLegendDefault(pwdVSddt,5);
    for(int iLy=0;iLy<NumLY;iLy++) 
      pwdVSddt->AddEntry(H3_0123[iLy],Form("nH==3 no iZ = %d",iLy),"FEl");
    pwdVSddt->AddEntry(H4_0123,"nH==4 all Pass","E1X0p");
    
    TLegend *pwdVSddt1 = new TLegend(0.11,.77,.96,0.895);
    pwdVSddt1->SetNColumns(4);
    pwdVSddt1->SetLineColor(0);
    
    // setLegendDefault(pwdVSddt1,4);
    for(int iLy=0;iLy<NumLY;iLy++) 
      pwdVSddt1->AddEntry(H3_0123[iLy],Form("Efficiency of iZ = %d",iLy),"FEl");
    

    
    hMod2->GetYaxis()->SetRangeUser(0.5,1.1);
    hMod2->GetYaxis()->SetTitle("Channel Efficiency");
    hMod2->SetLineStyle(2);
    
    TCanvas *cEVEFAll = new TCanvas("cEVEFAll","",1920*NumBX,2160*NumBY);
    TCanvas *cNUMSAll = new TCanvas("cNUMSAll","",1920*NumBX,1080*NumBY);
    TCanvas *cEVESAll = new TCanvas("cEVESAll","",1920*NumBX,1080*NumBY);
    cEVEFAll ->Divide(NumBX,NumBY);
    cNUMSAll ->Divide(NumBX,NumBY);
    cEVESAll ->Divide(NumBX,NumBY);
    
    TH1F  *H0123EMR[NumBX][NumBY][NumLY];
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
          H3_0123[iLy]->SetLineColor(ColorArr17[iLy+4]);
          H3_0123[iLy]->SetBarWidth((1./(NumLY+1))*0.9);
          H3_0123[iLy]->SetBarOffset((0.5/(NumLY+1))*1.1+(1./(NumLY+1))*iLy);
          H3_0123[iLy]->SetLineWidth(2);
          H3_0123[iLy]->SetMarkerStyle(0);
          // H3_0123[iLy]->Scale();
          
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
          TGEFFErr[iLy] ->SetLineColor(ColorArr17[iLy+4]);
          TGEFFErr[iLy] ->SetLineStyle(1);
          TGEFFErr[iLy] ->SetMarkerStyle(104);
          TGEFFErr[iLy] ->SetMarkerColor(ColorArr17[iLy+4]);

        }
        
        H4_0123 ->SetMarkerStyle(4);
        H4_0123 ->SetMarkerSize(0.5);
        H4_0123 ->SetLineColor(1);
        // H4_0123->Scale(1/4.);
        
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
        TH1F  *HS0123[NumLY]={H3_X123E,H3_0X23E,H3_01X3E,H3_012XE};
        
        
        int Statistic[NumLY][NumCh]={0};

        for(int ichs=0;ichs<NumCh;ichs++){
          
          out<<ibx<<"\t"<<iby<<"\t"<<"ch"<<ichs<<"\t";
          out<<H4_0123->GetBinContent(ichs+1)<<"\t";
          out<<H3_X123->GetBinContent(ichs+1)<<"\t";
          out<<H3_0X23->GetBinContent(ichs+1)<<"\t";
          out<<H3_01X3->GetBinContent(ichs+1)<<"\t";
          out<<H3_012X->GetBinContent(ichs+1)<<"\t";
          out<<H4_0123E1->GetBinContent(ichs+1)<<"\t";
          out<<H4_0123E3->GetBinContent(ichs+1)<<"\t";
          out<<H4_0123E5->GetBinContent(ichs+1)<<"\t";
          if(ichs==NumCh-1&&ibx==NumBX-1&&iby==NumBY-1) out<<H4_0123E4->GetBinContent(ichs+1);
          else out<<H4_0123E4->GetBinContent(ichs+1)<<endl;
          for(int iLy=0;iLy<NumLY;iLy++) 
            Statistic[iLy][ichs] = HS0123[iLy]->GetBinContent(ichs+1);
        }


        cevent ->cd(1);
        cTMP->DrawClonePad();
        
        cNUMSAll->cd(1+ibx+2-NumBX*iby);
        cTMP    ->DrawClonePad();
        
        cTMP->cd();
        hMod2->Draw("l");
        pwdVSddt1->Draw();
        
        //TGraphErrors *EFFErr[4]={};
        float AVGEffOfLayer[4]={};
        for(int iLy=0;iLy<NumLY;iLy++){
          for(int ichs=0;ichs<NumCh;ichs++)
            AVGEffOfLayer[iLy]+=H0123E[iLy]->GetBinContent(ichs+1);;
          AVGEffOfLayer[iLy]/=NumCh;
          cout<<"Mean Eff. Of Layer "<<iLy<<" : "<<AVGEffOfLayer[iLy]<<endl;
        }
        for(int iLy=0;iLy<NumLY;iLy++){
          double  x[NumCh]={},   y[NumCh]={};
          double ex[NumCh]={}, eyl[NumCh]={}, eyh[NumCh]={};
          for(int ichs=0;ichs<NumCh;ichs++){
            x[ichs]  = ichs+(1./(NumLY+1))*(1+iLy);
            y[ichs]  = H0123E[iLy]->GetBinContent(ichs+1);
            int bidtmp = LayArr[ibx][iby]+NumBX*NumBY*iLy;
            eyl[ichs]= H0123E[iLy]->GetBinError(ichs+1);
            eyh[ichs]= H0123E[iLy]->GetBinError(ichs+1);
            if      (y[ichs]-eyl[ichs]<=0) eyl[ichs]=y[ichs];
            else if(y[ichs]+eyh[ichs]>=1) eyh[ichs]=1-y[ichs];
            out1<<BD[iLy]<<"\t"<<x[ichs]<<"\t"<<y[ichs]<<"\t"<<eyl[ichs]<<"\t"<<eyh[ichs]<<"\n";
            if(AVGEffOfLayer[iLy]>TriggerChanEff){
              
              out2<<bidtmp<<"\t"<<ichs<<"\t"<<BCIDCovGID(bidtmp,ichs)<<"\t"<<y[ichs]<<"\t"<<Statistic[iLy][ichs]<<"\t"<<eyl[ichs]<<"\t"<<eyh[ichs]<<"\n";
              MScints[BCIDCovGID(BD[iLy],ichs)].Efficiency = y[ichs];
              MScints[BCIDCovGID(BD[iLy],ichs)].EffSta     = Statistic[iLy][ichs];
            }
              
            // cout<<x[ichs]<<"  "<<H0123E[iLy]->GetBinContent(ichs+1)<<"  "<<ex[ichs]<<"  "<<H0123E[iLy]->GetBinError(ichs+1)<<"  "<<endl;
          }
          //cout<<H0123E[iLy]->GetBinContent(1);
          H0123E[iLy]->Draw("histbarSame");
          TGraphAsymmErrors *EFFErrAsy = new TGraphAsymmErrors(NumCh,x,y,ex,ex,eyl,eyh);
          EFFErrAsy->Draw("esame");
          EFFErrAsy->SetLineColor(ColorArr17[iLy+NumLY]);
          EFFErrAsy->SetLineStyle(1);
          EFFErrAsy->SetMarkerStyle(104);
          EFFErrAsy->SetMarkerColor(ColorArr17[iLy]);
          H0123E[iLy]->SetFillColor(ColorArr17[iLy]);
          H0123E[iLy]->SetBarWidth((1./(NumLY+1))*0.9);
          H0123E[iLy]->SetBarOffset((0.5/(NumLY+1))*1.1+(1./(NumLY+1))*iLy);
          H0123E[iLy]->SetLineColor(ColorArr17[iLy+NumLY]);
        }
        
        for(int iLay=0;iLay<NumLY;iLay++)
          H0123EMR[ibx][iby][iLay] =  (TH1F*) H0123E[iLay]->Clone();
        hMod2->Draw("lsame");

        DrawPdfPng(cevent,Form("%sCheff%s_%sBiX%dBiY%d%s",DirRCheffDetBXY,NoiseStr,TimeStr,ibx,iby,RunStr));
        
        cevent ->cd(2);
        cTMP->DrawClonePad();
        
        cEVESAll->cd(1+ibx+2-NumBX*iby);
        cTMP    ->DrawClonePad();
        
        cEVEFAll->cd(1+ibx+2-NumBX*iby);
        cevent->DrawClonePad();
        
        cTMP->Close();
        cevent->Close();
        delete H3_X123E, H3_0X23E, H3_01X3E, H3_012XE, H4_0123E1;
        delete H4_0123E3, H4_0123E5, H4_0123E4, H4_0123E;
      }
    }
    out.close();
    out1.close();
    out2.close();
    system(Form("cp %sCheff%s_%s%s.txt  %s",DirRCheffD,NoiseStr,TimeStr,RunStr,DirOCheffSep));
    system(Form("cp %sCheff%s_%s%s.csv  %s",DirRCheffD,NoiseStr,TimeStr,RunStr,DirOCheffSep));
    cout<<(Form("%sCheff%s_%s%s.csv/.txt has been freated",DirRCheffD,NoiseStr,TimeStr,RunStr,DirOCheffSep))<<endl;
    
    DrawPdfPng(cEVEFAll,Form("%sChNE%s_%s%s",DirOCheffCNE,NoiseStr,TimeStr,RunStr));
    DrawPdfPng(cNUMSAll,Form("%sChNum%s_%s%s",DirOCheffCN,NoiseStr,TimeStr,RunStr));
    DrawPdfPng(cEVESAll,Form("%sChEff%s_%s%s",DirOCheffCE,NoiseStr,TimeStr,RunStr));
    
    // TH2F *h2MathEffRepair[NumLY];
  /*
    TH1F *h1MathEffRepair[NumBX][NumBY][NumLY];
        
    for(int ibx=0; ibx<NumBX; ibx++){
      for(int iby=0; iby<NumBY; iby++){
        for(int ily=0; ily<NumLY; ily++){
          h1MathEffRepair[ibx][iby][ily] 
            = new TH1F(Form(Form("h1MathRep%dX%dY%d",ibx,iby,ily)),"",NumCh,0,NumCh);
        }
      }
    }
    for(int igid=0;igid<=NumBD*NumCh;igid++){//NumBD!=if BD0>1@@
      int iX  = MScints[igid].iX, iY = MScints[igid].iY, iZ = MScints[igid].iZ ;
      int BiX = MScints[igid].BiX, BiY = MScints[igid].BiY, BiZ = MScints[igid].BiZ;
      // h2MathEffRepair[iLay] = new TH2F(Form("h2MathRep%d",iLay),"",NumBX*NumnX,0,NumBX*NumnX,NumBY*NumnY,0,NumBY*NumnY);
      if(BiZ==1||BiZ==2){
        h1MathEffRepair[BiX][BiY][BiZ]->SetBinContent( MScints[igid].channelID+1, 0.9975);
      }else if((iX==0||iX==NumBX*NumnX-1)&&(iY==0||iY==NumBY*NumnY-1)){
        h1MathEffRepair[BiX][BiY][BiZ]->SetBinContent( MScints[igid].channelID+1, .58*0.9975);
        // h1MathEffRepair[BiX][BiY][BiZ]->SetBinError(CIDOfGID(igid)+1,BiZ%(NumLY-1)==0?.00106+0.0045:0+0.0045);
        // h2MathEffRepair[iLay]->SetBinContent(iX+1,iY+1,iLy%(NumLY-1)==0?.58000:1.00000);
        // h2MathEffRepair[iLay]->SetBinError(iX+1,iY+1,iLy%(NumLY-1)==0?.00106:0);
      }else if(iX==0||iX==NumBX*NumnX-1||iY==0||iY==NumBY*NumnY-1){
        h1MathEffRepair[BiX][BiY][BiZ]->SetBinContent( MScints[igid].channelID+1, .70*0.9975);
        // h1MathEffRepair[BiX][BiY][BiZ]->SetBinError(CIDOfGID(igid)+1,BiZ%(NumLY-1)==0?.00133+0.0045:0+0.0045);
        // h2MathEffRepair[iLay]->SetBinContent(iX+1,iY+1,iLy%(NumLY-1)==0?.70200:1.00000);
        // h2MathEffRepair[iLay]->SetBinError(iX+1,iY+1,iZiLyNumLY-1)==0?.00133:0);
      }else{
        h1MathEffRepair[BiX][BiY][BiZ]->SetBinContent( MScints[igid].channelID+1, .93911*0.9975);
        // h1MathEffRepair[BiX][BiY][BiZ]->SetBinContent(
          // CIDOfGID(igid)+1,
          // h1MathEffRepair[BiX][BiY][BiZ]->GetBinContent(CIDOfGID(igid)+1)
            // *(BiZ%(NumLY-1)==0?1/.93911/0.9975:1.00000)
        // );
        // h1MathEffRepair[BiX][BiY][BiZ]->SetBinError(CIDOfGID(igid)+1,BiZ%(NumLY-1)==0?.00190+0.0045:0+0.0045);
        // h2MathEffRepair[iLay]->SetBinContent(iX+1,iY+1,iLy%(NumLY-1)==0?.93911:1.00000);
        // h2MathEffRepair[iLay]->SetBinError(iX+1,iY+1,iZiLyNumLY-1)==0?.00190:0);
      }
      // cout<<"iX,iY,BiX,BiY,BiZ,ich\t"<<iX<<" "<<iY<<" "<<BiX<<" "<<BiY<<" "<<BiZ<<" "<<MScints[igid].channelID+1<<" "<<H0123EMR[BiX][BiY][BiZ]->GetBinContent(MScints[igid].channelID+1)<<" / "<<h1MathEffRepair[BiX][BiY][BiZ]->GetBinContent(MScints[igid].channelID+1)<<" = "<<H0123EMR[BiX][BiY][BiZ]->GetBinContent(MScints[igid].channelID+1)/h1MathEffRepair[BiX][BiY][BiZ]->GetBinContent(MScints[igid].channelID+1)<<endl;

    }
    TCanvas *cEMRSAll = new TCanvas("cEMRSAll","",1920*NumBX,1080*NumBY);
    cEMRSAll ->Divide(NumBX,NumBY);

    for(int ibx=0; ibx<NumBX; ibx++){
      for(int iby=0; iby<NumBY; iby++){
        cEMRSAll->cd(ibx+iby*NumBX+1);
        
        for(int ily=0; ily<NumLY; ily++){
          // for(int ich=0;ich<NumCh;ich++)
            // cout<<"ibx,iby,ily,ich\t"<<ibx<<" "<<iby<<" "<<ily<<" "<<ich+1<<" "<<H0123EMR[ibx][iby][ily]->GetBinContent(ich+1)<<" / "<<h1MathEffRepair[ibx][iby][ily]->GetBinContent(ich+1)<<" = "<<H0123EMR[ibx][iby][ily]->GetBinContent(ich+1)/h1MathEffRepair[ibx][iby][ily]->GetBinContent(ich+1)<<endl;
          H0123EMR[ibx][iby][ily]->Divide(h1MathEffRepair[ibx][iby][ily]);
        }
        for(int ily=0; ily<NumLY; ily++){
          for(int ich=0;ich<NumCh;ich++){
            if(H0123EMR[ibx][iby][ily]->GetBinContent(ich+1)>=1)
              H0123EMR[ibx][iby][ily]->SetBinContent(ich+1,1);
          }
          if(ily==0) hMod2->Draw("");
          
          H0123EMR[ibx][iby][ily]->Draw(Form("bar%s","same"));
          H0123EMR[ibx][iby][ily]->SetStats(0);
          H0123EMR[ibx][iby][ily]->GetYaxis()->SetRangeUser(.85,1.00);
          if(ily==NumLY-1) pwdVSddt1->Draw();
        }
      }
    }
    cEMRSAll->Print("/data4/YuSiang/Muography_Math_Study/Test.png");
    //@@ Add the Odet->Edet
    */
    cEVEFAll->Close();     cNUMSAll->Close();     cEVESAll->Close();
    H3_X123->Reset();
    H3_0X23->Reset();
    H3_01X3->Reset();
    H3_012X->Reset();
    H4_0123->Reset();
    //, H3_X123E, H3_0X23E, H3_01X3E, H3_012XE;
    // delete H4_0123E1, H4_0123E3, H4_0123E5, H4_0123E4, H4_0123E;
  }
}

