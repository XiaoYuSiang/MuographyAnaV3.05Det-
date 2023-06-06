#include <typeinfo>
#include <algorithm>
#include <TText.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH2F.h>
#include <TTree.h>
#include <TStyle.h>
#include <TMath.h>
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"
#include "DSLData.h"
#include "/data4/YuSiang/personalLib/Style/DBMTStyle.h"

using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace MuoAna_path_dir;
using namespace DataConst;

void txtDefault(float px, float py, const char* content, double size, int rotateD, TCanvas *c1, int txtfont=152){
  c1->cd();
  TText *txt = new TText(px,py,content);
  txt->SetTextSize(size);
  txt->SetTextAlign(22);
  txt->SetTextFont(txtfont);
  txt->SetTextAngle(rotateD);
  txt->Draw();
}
TF1 GetAngularFunction(const char opt = 'C'){
  TF1 AngularWeight;
  if(opt == 'C') AngularWeight = TF1("AngularWeight","pow(cos(75./180.*3.1415926-atan(x)),2)",0,1.57);
  else if(opt == 'T') AngularWeight = TF1("AngularWeight","pow(tan(15./180.*3.1415926+atan(x)),1.762)",0,1.57);
  return AngularWeight;
}



TH2F *SetTDDDModel(
  const char *Name = "TDDM",
  const char *Title = "#font[12]{Tracking result direction distribution}",
  const int binNFX = binNumFluxX,
  const int binNFY = binNumFluxY
){
  TH2F *TDDDM = new TH2F(Name,Title,binNFX,-1.400,1.400,binNFY,-1.400,1.400);
  //TDDDM->SetStats(0);
  TDDDM->GetXaxis()->SetTitle("dx/dz");
  TDDDM->GetXaxis()->SetLabelSize(0.03);
  TDDDM->GetYaxis()->SetTitle("dy/dz");
  TDDDM->GetYaxis()->SetTitleOffset(1.4);
  TDDDM->GetYaxis()->SetLabelSize(0.03);
  TDDDM->GetZaxis()->SetTitle("Numbers of event");
  TDDDM->GetZaxis()->SetTitleOffset(1.4);
  TDDDM->GetZaxis()->RotateTitle(true);
  TDDDM->GetZaxis()->SetLabelSize(0.03);
  char version[8] = {};
  DetVerCheck(version,detVer);
  if(strcmp(version,"V03_05")==0){ 
    TDDDM->GetXaxis()->SetRangeUser(V305AngRang[0][TriggerTrNLayer][0],V305AngRang[0][TriggerTrNLayer][1]);
    TDDDM->GetYaxis()->SetRangeUser(V305AngRang[1][TriggerTrNLayer][0],V305AngRang[1][TriggerTrNLayer][1]);
  }else{
    cout<<"Error: Version content yo be addition!!!  FitTrackResAna.C:SetTDDDModel~L30~L60"<<endl;
    cout<<"Now, version: "<< version<<endl;
    throw;
  }
  return TDDDM;
}

void FitTrackResAna(
  const int indexGap=28,
  const int binNFX = binNumFluxX,
  const int binNFY = binNumFluxY,
  const char opt    = 'C',
  const bool Rotate = false
){
  defstyle();
  
  char version[8] = {};
  DetVerCheck(version,detVer);
  cout<<"Now, detector Ver is:  "<<version<<endl;
  /*Set Sci size and layer ID*/
  
  char DirRoot[1][100] = {
    ""
  };
  sprintf(DirRoot[0],"%s",DirOperate);
  
  char GraphEndStamp[100];
  sprintf(GraphEndStamp,"%sBinX%03.fY%03.f",TimeStr,binNFX*1.,binNFY*1.);
  
  
  char saveInformation[200];
  sprintf(saveInformation,"GT%d_%s_nHTH%dL%d_BX%03.fBY%03.f",eventGapTcnt[indexGap],TimeStr,TriggerS,TriggerL,binNFX*1.,binNFY*1.);

  double WorkTimeN = WorkTime - GetTotNegTime(DirOperate, unixtimei, unixtimef);
//561000*2
  char TriLesDxDzADyDz[100],TriLesDxDzADyDzPos[100], TriTimeRange[100];
  sprintf(TriTimeRange," ");
  // sprintf(TriTimeRange,"&&unixtime>1672200000&&unixtime<1673322000");
  if(Rotate){
    sprintf(TriLesDxDzADyDz,"(DxDz>%f)&&(DyDz>%f)%s",TriggerLessDyDz,TriggerLessDxDz,TriTimeRange);
    sprintf(TriLesDxDzADyDzPos,"(DxDzPos>%f)&&(DyDzPos>%f)%s",TriggerLessDyDz,TriggerLessDxDz,TriTimeRange);
  }else{
    sprintf(TriLesDxDzADyDz,"(DxDz>%f)&&(DyDz>%f)%s",TriggerLessDxDz,TriggerLessDyDz,TriTimeRange);
    sprintf(TriLesDxDzADyDzPos,"(DxDzPos>%f)&&(DyDzPos>%f)%s",TriggerLessDxDz,TriggerLessDyDz,TriTimeRange);
  }
    
  cout<<TriLesDxDzADyDz<<endl;
  cout<<TriLesDxDzADyDzPos<<endl;
  
  TF1 AF = GetAngularFunction(opt);
  char AngTerm[20]={""};
  if(opt=='C'){
    sprintf(AngTerm,"cos^{2}(#theta)");
  }else if(opt=='T'){
    sprintf(AngTerm,"tan^{1.762}(90#circ-#theta)");
  }else{
    cout<<"Error: FitTrackRes.C: L~118: opt!='C'|'T'"<<endl;
    throw;
  }

  for(int iF=0;iF<1;iF++){
    char DirSave[100] ;
    sprintf(DirSave,"%s",DirRes_LFirTra);
    
    char FileSave[200];
    sprintf(FileSave,"%sETracksGT%d_%s_nHTH%dL%d.root",DirOperate,eventGapTcnt[indexGap],TimeStr,TriggerS,TriggerL);

    
    TFile *_file0 = TFile::Open(FileSave);
    cout<<"Tracking result File: "<<FileSave<<" is Opening:  "<<endl;
    TTree *t  = (TTree*) _file0 ->Get("t");
    
    FastStyleStats(0.2,1);
    TCanvas *C = new TCanvas("Canvas","",0,0,1000,1000);
    C->SetRightMargin(0.18); 

    int ndP = 180; /* Number of d Theta */
    TH2F *TRS = new TH2F("TRS","#font[12]{Tracking result}",ndP,0,360,25,0,25);
    TH2F *TRSC= new TH2F("TRSC","#font[12]{Center Bar}"    ,ndP,0,360,25,0,25);
    t->Draw("Tr_The/TMath::Pi()*180.:Tr_Phi/TMath::Pi()*180.>>TRS","","colz");//unixtime>1668700000||unixtime<1668400000
    // t->Draw("Tr_The/TMath::Pi()*180.:Tr_Phi/TMath::Pi()*180.","","colz");
    t->Draw("Tr_The/TMath::Pi()*180.:Tr_Phi/TMath::Pi()*180.>>TRSC","","colz");//@@unixtime>1668700000||unixtime<1668400000
    int NullBin = 0;
    for(int iphi=0;iphi<TRS->GetNbinsX();iphi++){
      for(int ithe=0;ithe<TRS->GetNbinsY();ithe++){
        if(iF==0&&TRS->GetBinContent(iphi+1,ithe+1)<5){ //@@ adjust small number in low static
          TRS->SetBinContent(iphi+1,ithe+1,-1);
          TRSC->SetBinContent(iphi+1,ithe+1,-1);
          NullBin++;
        }
        else if(iF==1&&TRS->GetBinContent(iphi+1,ithe+1)<100){ //@@ adjust small number in low static
          TRS->SetBinContent(iphi+1,ithe+1,-1);
          TRSC->SetBinContent(iphi+1,ithe+1,-1);
          NullBin++;
        }
      }
    }
    TRS->SetEntries(TRS->GetEntries()-NullBin);
    // TRS->SetEntries(TRS->GetEntries()-TRS->GetNbinsX()*TRS->GetNbinsY());
    for(int i=0;i<ndP;i++)TRSC->SetBinContent(i+1,1,TRSC->GetBinContent(1,1));
    TRS->GetYaxis()->SetTitle("#font[12]{Detector Inner Zenith angle #font[152]{q} (deg)}");
    TRS->GetXaxis()->SetTitle("#font[12]{Detector Inner Azimuth #font[152]{f} (deg)}");
    TRS->GetZaxis()->SetTitle("#font[12]{Event Numbers}");
    TRS->GetXaxis()->SetRangeUser(0,360);
    TRS->GetYaxis()->SetRangeUser(0,25);
    TRS->GetZaxis()->RotateTitle(true);
    TRS->GetXaxis()->SetTitleOffset(1.15);
    TRS->GetYaxis()->SetTitleOffset(1.15);
    TRS->GetZaxis()->SetTitleOffset(1.6);
    TRS->GetZaxis()->SetLabelSize(0.027);
    TRS->SetMinimum(0);
    TRS->SetStats(0);
    C->cd();
    TRS->Draw("colz");
    C->Print(Form("%s0_degThetaVsPhiColz.pdf",DirSave));
    TRS->Draw("p");
    C->Print(Form("%s0_degThetaVsPhiP.pdf",DirSave));
    TRS->Draw("box");
    C->Print(Form("%s0_degThetaVsPhiBox.pdf",DirSave));

    C->Close();
    delete C;
    C = new TCanvas("Canvas","",0,0,1050*8,1000*8);
    C->SetRightMargin(0.15); 
    C->SetPhi(0);
    C->SetTheta(90);
    C->SetLogz();
    TRS->Draw("Lego2 polz");
    TRSC->Draw("Lego2 pol same");
    TRS->SetStats(true);
    for(int i=0;i<8;i++){
      txtDefault(
        +.62*cos(TMath::Pi()/4.*i),
        +.62*sin(TMath::Pi()/4.*i),
        Form("f = %dp/4",i),
        .025,
        -90 + 180./4.*i,
        C
      );
    }
    txtDefault( -0.03, 0.02, "q = 0", .015, +.0, C);
    for(int i=1;i<6;i++){
      txtDefault(
        +.0,
        +.58/5.*i,
        Form("q = %dp/36",i),//5/144
        .015,
        +.0,
        C
      );
      
    }
    TRS->SetLineColor(9);
    TRS->SetLineWidth(1);
    TRSC->SetLineColorAlpha(0,0.);
    TRSC->SetLineWidth(0.5);
    // DrawPdfPng(C,Form("%sTryThetaVsPhiSP",DirSave));
    // DrawPdfPng(C,Form("%sTryThetaVsPhiSP",DirRes_FluxCal));
    C->SetTheta(45);
    // DrawPdfPng(C,Form("%sTryThetaVsPhiSP45",DirSave));
    // DrawPdfPng(C,Form("%sTryThetaVsPhiSP45",DirRes_FluxCal));

    
    FastStyleStats(1,1);
    gStyle->SetStatH(0.066);
    gStyle->SetStatW(0.150);
    
    TFile *FHistTDDD = new TFile(Form("%sTDDDs%s%c.root",DirOperate,saveInformation,opt), "RECREATE");

    
    TH2F *TDDD = SetTDDDModel("TDDD","#font[12]{Tracking result direction distribution}",binNFX,binNFY);
    TH2F *TDDDNorEff = SetTDDDModel("TDDDNorEff","#font[12]{Normalize by Efficiency}",binNFX,binNFY);
    TH2F *TDDDNorEffGeo = SetTDDDModel("TDDDNorEffGeo","#font[12]{Normalize by Eff.& Geometry Factor}",binNFX,binNFY);
    TH2F *TDDDNorEffGeoThe = SetTDDDModel(Form("TDDDNorEffGeoThe"),"#font[12]{Normalize by Eff.& G.Fac & Angular Factor}",binNFX,binNFY);
    TH2F *TDDDNorEffGeoTheTim = SetTDDDModel(Form("TDDDNorEffGeoTheTim"),"#font[12]{Normalize by Eff.& G.Fac & A.Fac& WorkTime}",binNFX,binNFY);

    if(Rotate) t->Draw("DxDz:-DyDz>>TDDD",Form("%s",TriLesDxDzADyDz),"colz");  
    else t->Draw("DyDz:DxDz>>TDDD",Form("%s",TriLesDxDzADyDz),"colz");  
    for(int ixbin=1; ixbin<=binNFX; ixbin++){
      for(int iybin=1; iybin<=binNFY; iybin++){
        float value = (TDDD->GetBinContent(ixbin,iybin));
        if(!MCMode) if(value < TriggerNegLowSEComb&&NegLowSta) TDDD->SetBinContent(ixbin,iybin,0);
      }
    }
    TDDD->Draw("colz");
    txtDefault(0., 0.28, Form("Statistic: %.0f",TDDD->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    // DrawPdfPng(C,Form("%sDxDzVSDyDz%s",DirSave,GraphEndStamp));
    // DrawPdfPng(C,Form("%sDxDzVSDyDz%s",DirRes_FluxCal,GraphEndStamp));
    
    if(Rotate) t->Draw("DxDz:-DyDz>>TDDDNorEff",Form("EffWei&&%s",TriLesDxDzADyDz),"colz");
    else t->Draw("DyDz:DxDz>>TDDDNorEff",Form("EffWei&&%s",TriLesDxDzADyDz),"colz");
    for(int ixbin=1; ixbin<=binNFX; ixbin++){
      for(int iybin=1; iybin<=binNFY; iybin++){
        float value = (TDDDNorEff->GetBinContent(ixbin,iybin));
        if(!MCMode) if(value<TriggerNegLowSEComb&&NegLowSta) value = 0;
        TDDDNorEff->SetBinContent(ixbin,iybin,value);
      }
    }
    txtDefault(0., 0.28, Form("Statistic: %.0f",TDDDNorEff->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    // DrawPdfPng(C,Form("%sDxDzVSDyDzNorEff%s",DirSave,GraphEndStamp));
    // DrawPdfPng(C,Form("%sDxDzVSDyDzNorEff%s",DirRes_FluxCal,GraphEndStamp));

    TFile *ScaleFacFile = new TFile(Form("%sOAngComFacBX%03.fBY%03.f.root",DirIMFdat,binNFX*1.,binNFY*1.),"READ");
    
    cout<<ScaleFacFile->GetName()<<" is opening!"<<endl;
    TH2F *hCombinationFac  = (TH2F*) ScaleFacFile ->Get("hCombinationFac");
    if(hCombinationFac->GetNbinsX()!=binNFX||hCombinationFac->GetNbinsY()!=binNFY)
      FATAL("binX or binY of hCombinationFac != binNumFlux X or Y");
    
    for(int ixbin=1; ixbin<=binNFX; ixbin++){
      for(int iybin=1; iybin<=binNFY; iybin++){
        float value = (TDDDNorEff->GetBinContent(ixbin,iybin));
        float wei   = (hCombinationFac->GetBinContent(ixbin,iybin));
        if(value == 0 || wei == 0) continue;
        TDDDNorEffGeo->SetBinContent(ixbin,iybin,value/wei);

      }
    }
    
    TDDDNorEffGeo->Draw("colz");
    txtDefault(0., 0.28, Form("Statistic: %.0f",TDDDNorEffGeo->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    // DrawPdfPng(C,Form("%sDxDzVSDyDzNorEffGeo%s",DirRes_FluxCal,GraphEndStamp));

    C->SetLogz(0);
    TDDDNorEffGeoThe->GetZaxis()->SetTitle(Form("Number/GeoAreaFactor/%s",AngTerm));
    TDDDNorEffGeoThe->GetZaxis()->SetRangeUser(0,3000);
    for(int ixbin=1; ixbin<=binNFX; ixbin++){
      for(int iybin=1; iybin<=binNFY; iybin++){
        float value = (TDDDNorEffGeo->GetBinContent(ixbin,iybin));
        // float wei   = pow(cos(75./180.*3.14-atan(TDDDNorEffGeo->GetYaxis()->GetBinCenter(iybin))),2);
        float wei   = AF(TDDDNorEffGeo->GetYaxis()->GetBinCenter(iybin));
        if(value == 0 || wei == 0) continue;
        TDDDNorEffGeoThe->SetBinContent(ixbin,iybin,value/wei);
        // cout<<value<<"\t"<<TDDDNorEffGeoThe->GetYaxis()->GetBinCenter(iybin)<<"\t";
        // cout<<atan(TDDDNorEffGeoThe->GetYaxis()->GetBinCenter(iybin))<<"\t";
        // cout<<cos(atan(TDDDNorEffGeoThe->GetYaxis()->GetBinCenter(iybin)))<<"\t";
        // cout<<pow(cos(atan(TDDDNorEffGeoThe->GetYaxis()->GetBinCenter(iybin))),2)<<endl;
      }
    }
    
    TDDDNorEffGeoThe->Draw("colz");
    txtDefault(0., 0.28, Form("Statistic: %.0f",TDDDNorEffGeoThe->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    // DrawPdfPng(C,Form("%sDxDzVSDyDzNorEffGeoThe%s",DirRes_FluxCal,GraphEndStamp));

    TDDDNorEffGeoTheTim->GetZaxis()->SetTitle(Form("Number/Eff./GeoAreaFactor/%s/Worktime",AngTerm));
    TDDDNorEffGeoTheTim->Add(TDDDNorEffGeoThe);
    TDDDNorEffGeoTheTim->Scale(1./WorkTimeN);
    TDDDNorEffGeoTheTim->GetZaxis()->SetRangeUser(0.0000,0.00950);
    TDDDNorEffGeoTheTim->Draw("colz");
    txtDefault(0., 0.28, Form("Statistic: %f",TDDDNorEffGeoTheTim->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    // DrawPdfPng(C,Form("%sDxDzVSDyDzNorEffGeoTheTim%s",DirRes_FluxCal,GraphEndStamp));

    FHistTDDD->WriteObject(TDDD               , "TDDD"                );
    FHistTDDD->WriteObject(TDDDNorEff         , "TDDDNorEff"          );
    FHistTDDD->WriteObject(TDDDNorEffGeo      , "TDDDNorEffGeo"       );
    FHistTDDD->WriteObject(TDDDNorEffGeoThe   , Form("TDDDNorEffGeoThe")   );
    FHistTDDD->WriteObject(TDDDNorEffGeoTheTim, Form("TDDDNorEffGeoTheTim"));
    cout<<FHistTDDD->GetName()<<" have created!"<<endl;
    FHistTDDD->Close();
    
    
    C->SetLogz();
    TFile *FHistTDDDP = new TFile(Form("%sTDDDPs%s%c.root",DirOperate,saveInformation,opt), "RECREATE");

    TH2F *TDDDP = SetTDDDModel("TDDDP","#font[12]{Tracking result direction distribution}",binNFX,binNFY);
    TH2F *TDDDPNorEff = SetTDDDModel("TDDDPNorEff","#font[12]{Normalize by Efficiency}",binNFX,binNFY);
    TH2F *TDDDPNorEffGeo = SetTDDDModel("TDDDPNorEffGeo","#font[12]{Normalize by Eff.& Geometry Factor}",binNFX,binNFY);
    TH2F *TDDDPNorEffGeoThe = SetTDDDModel(Form("TDDDPNorEffGeoThe"),"#font[12]{Normalize by Eff.& G.Fac & Angular Factor}",binNFX,binNFY);
    TH2F *TDDDPNorEffGeoTheTim = SetTDDDModel(Form("TDDDPNorEffGeoTheTim"),"#font[12]{Normalize by Eff.& G.Fac & A.Fac& WorkTime}",binNFX,binNFY);
    
    
    if(Rotate) t->Draw("DxDzPos:-DyDzPos>>TDDDP",Form("(CodeEmp==0)&&%s",TriLesDxDzADyDzPos),"colz");
    else t->Draw("DyDzPos:DxDzPos>>TDDDP",Form("(CodeEmp==0)&&%s",TriLesDxDzADyDzPos),"colz");
    for(int ixbin=1; ixbin<=binNFX; ixbin++){
      for(int iybin=1; iybin<=binNFY; iybin++){
        float value = (TDDDP->GetBinContent(ixbin,iybin));
        if(!MCMode) if(value < TriggerNegLowSEComb&&NegLowSta) TDDDP->SetBinContent(ixbin,iybin,0);
      }
    }
    txtDefault(0., 0.28, Form("Statistic: %.0f",TDDDP->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    DrawPdfPng(C,Form("%sDxDzVSDyDzP%s",DirSave,GraphEndStamp));
    DrawPdfPng(C,Form("%sDxDzVSDyDzP%s",DirRes_FluxCal,GraphEndStamp));

    
    TDDDPNorEff->GetZaxis()->SetTitle("Numbers of event/Effciency");
    if(Rotate) t->Draw("DxDzPos:-DyDzPos>>TDDDPNorEff",Form("(CodeEmp==0&&%s)*EffWei",TriLesDxDzADyDzPos),"colz");
    else t->Draw("DyDzPos:DxDzPos>>TDDDPNorEff",Form("(CodeEmp==0&&%s)*EffWei",TriLesDxDzADyDzPos),"colz");
    for(int ixbin=1; ixbin<=binNFX; ixbin++){
      for(int iybin=1; iybin<=binNFY; iybin++){
        float value = (TDDDPNorEff->GetBinContent(ixbin,iybin));
        if(!MCMode) if(value < TriggerNegLowSEComb&&NegLowSta) TDDDPNorEff->SetBinContent(ixbin,iybin,0);
      }
    }
    txtDefault(0., 0.28, Form("Statistic: %.0f",TDDDPNorEff->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    DrawPdfPng(C,Form("%sDxDzVSDyDzPNorEff%s",DirSave,GraphEndStamp));
    DrawPdfPng(C,Form("%sDxDzVSDyDzPNorEff%s",DirRes_FluxCal,GraphEndStamp));
    
    
    for(int ixbin=1; ixbin<=binNFX; ixbin++){
      for(int iybin=1; iybin<=binNFY; iybin++){
        float value = (TDDDPNorEff->GetBinContent(ixbin,iybin));
        float wei   = (hCombinationFac->GetBinContent(ixbin,iybin));
        if(value == 0 || wei == 0) continue;
        TDDDPNorEffGeo->SetBinContent(ixbin,iybin,value/wei);
      }
    }
    TDDDPNorEffGeo->Draw("colz");
    txtDefault(0., 0.28, Form("Statistic: %.0f",TDDDPNorEffGeo->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    DrawPdfPng(C,Form("%sDxDzVSDyDzPNorEffGeo%s",DirRes_FluxCal,GraphEndStamp));
    
    
    C->SetLogz(0);
    TDDDPNorEffGeoThe->GetZaxis()->SetTitle(Form("Number/GeoAreaFactor/%s",AngTerm));
    //TDDDPNorEffGeoThe->GetZaxis()->SetRangeUser(0,3000);
    for(int ixbin=1; ixbin<=binNFX; ixbin++){
      for(int iybin=1; iybin<=binNFY; iybin++){
        float value = (TDDDPNorEffGeo->GetBinContent(ixbin,iybin));
        // float wei   = pow(cos(75./180.*3.14-atan(TDDDPNorEffGeo->GetYaxis()->GetBinCenter(iybin))),2);
        float wei   = AF(TDDDPNorEffGeo->GetYaxis()->GetBinCenter(iybin));
        if(value == 0 || wei == 0) continue;
        TDDDPNorEffGeoThe->SetBinContent(ixbin,iybin,value/wei);
      }
    }
    TDDDPNorEffGeoThe->GetZaxis()->SetLabelSize(0.02);
    TDDDPNorEffGeoThe->Draw("colz");
    txtDefault(0., 0.28, Form("Statistic: %.0f",TDDDPNorEffGeoThe->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    DrawPdfPng(C,Form("%sDxDzVSDyDzPNorEffGeoThe%c%s",DirRes_FluxCal,opt,GraphEndStamp));
    
    TDDDPNorEffGeoTheTim->GetZaxis()->SetTitle(Form("Number/Eff./GeoAreaFactor%s/Worktime",AngTerm));
    
    TDDDPNorEffGeoTheTim->Add(TDDDPNorEffGeoThe);
    TDDDPNorEffGeoTheTim->Scale(1./WorkTimeN);
    TDDDPNorEffGeoTheTim->GetZaxis()->SetRangeUser(0.0000,0.00950);
    TDDDPNorEffGeoTheTim->GetZaxis()->SetLabelSize(0.020);
    TDDDPNorEffGeoTheTim->Draw("colz");
    txtDefault(0., 0.28, Form("Statistic: %f",TDDDPNorEffGeoTheTim->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    DrawPdfPng(C,Form("%sDxDzVSDyDzPNorEffGeoThe%cTim%s",DirRes_FluxCal,opt,GraphEndStamp));
    
    FHistTDDDP->WriteObject(TDDDP               , "TDDDP"                );
    FHistTDDDP->WriteObject(TDDDPNorEff         , "TDDDPNorEff"          );
    FHistTDDDP->WriteObject(TDDDPNorEffGeo      , "TDDDPNorEffGeo"       );
    FHistTDDDP->WriteObject(TDDDPNorEffGeoThe   , Form("TDDDPNorEffGeoThe")   );
    FHistTDDDP->WriteObject(TDDDPNorEffGeoTheTim, Form("TDDDPNorEffGeoTheTim"));
    cout<<FHistTDDDP->GetName()<<" have created!"<<endl;
    FHistTDDDP->Close();
    
    C->Close();
    
  }
}

void  FitTrackResAnaV2( const int indexGap=28, const bool Rotate=false){
  
  // FitTrackResAna(indexGap,281,281,Rotate);
  FitTrackResAna(indexGap,93,93,'C',Rotate); cout<<416<<endl;
  FitTrackResAna(indexGap,93,93,'T',Rotate); cout<<417<<endl;
  // FitTrackResAna(indexGap,281,281,'C',Rotate); cout<<418<<endl;
  // FitTrackResAna(indexGap,281,281,'T',Rotate); cout<<419<<endl;
  if(binNumFluxX!=93&&binNumFluxX!=281&&binNumFluxY!=93&&binNumFluxY!=281){ cout<<420<<endl;
    FitTrackResAna(indexGap,binNumFluxX,binNumFluxY,'C',Rotate); cout<<421<<endl;
    FitTrackResAna(indexGap,binNumFluxX,binNumFluxY,'T',Rotate); cout<<422<<endl;
  }
  
}