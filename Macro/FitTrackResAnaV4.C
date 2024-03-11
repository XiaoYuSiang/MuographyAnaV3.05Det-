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
TF1 GetAngularFunction(const float AngOff = 75.4, const char opt = 'C'){
  TF1 AngularWeight;
  if(opt == 'C') AngularWeight = TF1("AngularWeight",Form("pow(cos(%f/180.*3.1415926-atan(x)),2)",AngOff),0,1.57);
  else if(opt == 'T') AngularWeight = TF1("AngularWeight",Form("pow(tan((90.-%f)/180.*3.1415926+atan(x)),1.762)",AngOff),0,1.57);
  return AngularWeight;
}


void FitTrackResAna(
  const int indexGap=28,
  const int binNFX = 281,
  const int binNFY = 281,
  const char opt    = 'C',
  const bool Rotate = false
){
  FATAL("Please use the last Ver. : FitTrackResAnaV3(c.i. indexGap, c.d. binWidth, c.c opt, c.b Rotate)");
}
void FitTrackResAnaV2(
  const int indexGap=28,
  const int binNFX = 281,
  const int binNFY = 281,
  const char opt    = 'C',
  const bool Rotate = false
){
  FATAL("Please use the last Ver. : FitTrackResAnaV3(c.i. indexGap, c.d. binWidth, c.c opt, c.b Rotate)");
}
void FitTrackResAnaV3(
  const int indexGap=28,
  const double binWidth=0.01,
  const char opt    = 'C',
  const bool Rotate = false
){
  const char DxDzVar[8] = "DxDz";
  const char DyDzVar[8] = "DyDz";
  const char PHOrNot[2] = "";
  
  defstyle();
  // cout<<"V3-63" <<endl; throw;
  char version[8] = {};
  DetVerCheck(version,detVer);
  cout<<"Now, detector Ver is:  "<<version<<endl;
  /*Set Sci size and layer ID*/
  
  char DirRoot[1][100] = {""};
  sprintf(DirRoot[0],"%s",DirOperate);
  
  vector <float> vRxyBin = GetFluxRangeBins(FluxBinDXYDZRange,binWidth,true);
  int     NumBins = vRxyBin.size()-1;

  int binNFX = NumBins, binNFY = NumBins;
  
  char GraphEndStamp[100];
  sprintf(GraphEndStamp,"%sBinXY%03.f",TimeStr,NumBins*1.);
  
  
  char saveInformation[200];
  sprintf(saveInformation,"GT%d_%s_nHTH%dL%d_BXY%03.f",eventGapTcnt[indexGap],TimeStr,TriggerS,TriggerL,NumBins*1.);

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
  
  TF1 AF = GetAngularFunction(0, opt);
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
    C = new TCanvas("Canvas","",0,0,1050*4,1000*4);
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

    C->SetGrid(1,1);
    FastStyleStats(1,1);
    gStyle->SetStatH(0.066);
    gStyle->SetStatW(0.150);
    

    TFile *ScaleFacFile = new TFile(Form("%sOAngComFacRADBXY%03.f.root",DirIMFdat,NumBins*1.),"READ");
    
    cout<<ScaleFacFile->GetName()<<" is opening!"<<endl;
    TH2F *hCombinationFac  = (TH2F*) ScaleFacFile ->Get("hCombinationFac");
    if(hCombinationFac->GetNbinsX()!=binNFX||hCombinationFac->GetNbinsY()!=binNFY)
      FATAL("binX or binY of hCombinationFac != binNumFlux X or Y");
    
    
    
    C->SetLogz();
    TFile *FHistTDDDP = new TFile(Form("%sTDDDPs%s%c.root",DirOperate,saveInformation,opt), "RECREATE");

    TH2F *TDDDP = SetTDDDModelRAD("TDDDP","#font[12]{Tracking result direction distribution}",binWidth);
    TH2F *TDDDPNorEff = SetTDDDModelRAD("TDDDPNorEff","#font[12]{Normalize by Efficiency}",binWidth);
    TH2F *TDDDPNorEffGeo = SetTDDDModelRAD("TDDDPNorEffGeo","#font[12]{Normalize by Eff.& Geometry Factor}",binWidth);
    TH2F *TDDDPNorEffGeoThe = SetTDDDModelRAD(Form("TDDDPNorEffGeoThe"),"#font[12]{Normalize by Eff.& G.Fac & Angular Factor}",binWidth);
    TH2F *TDDDPNorEffGeoTheTim = SetTDDDModelRAD(Form("TDDDPNorEffGeoTheTim"),"#font[12]{Normalize by Eff.& G.Fac & A.Fac& WorkTime}",binWidth);
    
    if(Rotate) t->Draw(
        Form("atan(%s):-atan(%s)>>TDDD%s",DxDzVar,DyDzVar,PHOrNot),
        Form("(CodeEmp==0)&&%s",TriLesDxDzADyDzPos),
        "colz"
      );
    else t->Draw(
        Form("atan(%s):atan(%s)>>TDDD%s",DyDzVar,DxDzVar,PHOrNot),
        Form("(CodeEmp==0)&&%s",TriLesDxDzADyDzPos),
        "colz"
      );

    for(int ixbin=1; ixbin<=binNFX; ixbin++){
      for(int iybin=1; iybin<=binNFY; iybin++){
        float value = (TDDDP->GetBinContent(ixbin,iybin));
        if(!MCMode) if(value < TriggerNegLowSEComb&&NegLowSta) TDDDP->SetBinContent(ixbin,iybin,0);
      }
    }
    // txtDefault(0., 0.28, Form("Statistic: %.0f",TDDDP->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    DrawPdfPng(C,Form("%sDxDzVSDyDz%s%s",DirSave,PHOrNot,GraphEndStamp));
    DrawPdfPng(C,Form("%sSADxDzVSDyDz%s%s",DirRes_FluxCal,PHOrNot,GraphEndStamp));

    
    TDDDPNorEff->GetZaxis()->SetTitle("Numbers of event/Effciency");
    if(Rotate) t->Draw(Form("atan(%s):-atan(%s)>>TDDD%sNorEff",DxDzVar,DyDzVar,PHOrNot),Form("(CodeEmp==0&&%s)*EffWei",TriLesDxDzADyDzPos),"colz");
    else t->Draw(Form("atan(%s):atan(%s)>>TDDD%sNorEff",DyDzVar,DxDzVar,PHOrNot),Form("(CodeEmp==0&&%s)*EffWei",TriLesDxDzADyDzPos),"colz");
    for(int ixbin=1; ixbin<=binNFX; ixbin++){
      for(int iybin=1; iybin<=binNFY; iybin++){
        float value = (TDDDPNorEff->GetBinContent(ixbin,iybin));
        if(!MCMode) if(value < TriggerNegLowSEComb&&NegLowSta) TDDDPNorEff->SetBinContent(ixbin,iybin,0);
      }
    }
    // txtDefault(0., 0.28, Form("Statistic: %.0f",TDDDPNorEff->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    DrawPdfPng(C,Form("%sDxDzVSDyDz%sNorEff%s",DirSave,PHOrNot,GraphEndStamp));
    DrawPdfPng(C,Form("%sSBDxDzVSDyDz%sNorEff%s",DirRes_FluxCal,PHOrNot,GraphEndStamp));
    
    
    for(int ixbin=1; ixbin<=binNFX; ixbin++){
      for(int iybin=1; iybin<=binNFY; iybin++){
        float value = (TDDDPNorEff->GetBinContent(ixbin,iybin));
        float wei   = (hCombinationFac->GetBinContent(ixbin,iybin));
        if(value == 0 || wei == 0) continue;
        TDDDPNorEffGeo->SetBinContent(ixbin,iybin,value/wei);
      }
    }
    TDDDPNorEffGeo->Draw("colz");
    // txtDefault(0., 0.28, Form("Statistic: %.0f",TDDDPNorEffGeo->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    DrawPdfPng(C,Form("%sSCDxDzVSDyDz%sNorEffGeo%s",DirRes_FluxCal,PHOrNot,GraphEndStamp));
    
    
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
    // txtDefault(0., 0.28, Form("Statistic: %.0f",TDDDPNorEffGeoThe->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    DrawPdfPng(C,Form("%sSDDxDzVSDyDz%sNorEffGeoThe%c%s",DirRes_FluxCal,PHOrNot,opt,GraphEndStamp));
    
    TDDDPNorEffGeoTheTim->GetZaxis()->SetTitle(Form("Number/Eff./GeoAreaFactor/%s/Worktime",AngTerm));
    
    TDDDPNorEffGeoTheTim->Add(TDDDPNorEffGeoThe);
    TDDDPNorEffGeoTheTim->Scale(1./WorkTimeN);
    TDDDPNorEffGeoTheTim->GetZaxis()->SetRangeUser(0.0,TriggerMaxFlux);
    TDDDPNorEffGeoTheTim->GetZaxis()->SetLabelSize(0.020);
    TDDDPNorEffGeoTheTim->Draw("colz");
    // txtDefault(0., 0.28, Form("Statistic: %f",TDDDPNorEffGeoTheTim->Integral(-1,-1,-1,-1)), 0.03, 0, C, 1);
    DrawPdfPng(C,Form("%sSEDxDzVSDyDz%sNorEffGeoThe%cTim%s",DirRes_FluxCal,PHOrNot,opt,GraphEndStamp));
    
    
    vector<float>vRxyBinMrad;
    for(int i=0; i < int(vRxyBin.size());i++)
      vRxyBinMrad.push_back(vRxyBin[i]*1000);
    TH2F *h2 = new TH2F("h2","",vRxyBinMrad.size()-1,vRxyBinMrad.data(),vRxyBinMrad.size()-1,vRxyBinMrad.data());
    for(int i=0;i<int(vRxyBinMrad.size());i++){
      cout<<vRxyBin[i]<<"\t"<<vRxyBinMrad[i]<<endl;
    }
    for(int ix=0;ix<int(vRxyBinMrad.size()-1);ix++){
      for(int iy=0;iy<int(vRxyBinMrad.size()-1);iy++){
        h2->SetBinContent(ix+1,iy+1,TDDDPNorEffGeoTheTim->GetBinContent(ix+1,iy+1));
      }
    }
    h2->SetStats(0);
    h2->GetXaxis()->SetTitle("Vertical direction(mrad)");
    h2->GetXaxis()->SetLabelSize(0.04);
    h2->GetYaxis()->SetLabelSize(0.04);
    h2->GetYaxis()->SetTitle("Horizontal direction(mrad)");
    h2->GetYaxis()->SetTitleOffset(1.4);
    h2->GetZaxis()->SetTitle(Form("#Phi_{#mu}=Number/Eff./GeoAreaFactor/%s/Worktime",AngTerm));
    h2->GetZaxis()->SetTitleOffset(1.4);
    h2->GetZaxis()->RotateTitle(true);
    h2->GetXaxis()->CenterTitle(true);
    h2->GetYaxis()->CenterTitle(true);
    h2->GetZaxis()->CenterTitle(true);
    // h2->GetXaxis()->LabelsOption("m");
    // h2->GetYaxis()->LabelsOption("m");
    h2->GetZaxis()->SetLabelSize(0.02);
    h2->GetZaxis()->SetRangeUser(0.0,TriggerMaxFlux);
    h2->GetXaxis()->SetRangeUser(-260,260);
    h2->GetYaxis()->SetRangeUser(-260,260);
    h2->GetXaxis()->SetNdivisions(-20213);
    h2->GetYaxis()->SetNdivisions(-20213);
    h2->GetXaxis()->SetTickSize(1);
    h2->GetYaxis()->SetTickSize(1);
    h2->GetXaxis()->SetTickLength(0.05);
    h2->GetYaxis()->SetTickLength(0.05);
    for(int i=1;i<=int(vRxyBinMrad.size())-1;i++){
      h2->GetXaxis()->SetBinLabel(i,Form("%.0f",h2->GetXaxis()->GetBinCenter(i)*1.));
      h2->GetYaxis()->SetBinLabel(i,Form("%.0f",h2->GetYaxis()->GetBinCenter(i)*1.));
      cout<<Form("%f",h2->GetYaxis()->GetBinCenter(i)*1.)<<endl;
    }
    // h2->GetZaxis()->SetRangeUser(0,TriggerMaxFlux);
    h2->Draw("colz");
    DrawPdfPng(C,Form("%sSEDxDzVSDyDz%sNorEffGeoThe%cTim%sMRAD",DirRes_FluxCal,PHOrNot,opt,GraphEndStamp));
    
    
    FHistTDDDP->WriteObject(TDDDP                    , Form("TDDD%s",PHOrNot) );
    FHistTDDDP->WriteObject(TDDDPNorEff             , Form("TDDDPNorEff%s",PHOrNot) );
    FHistTDDDP->WriteObject(TDDDPNorEffGeo         , Form("TDDDPNorEffGeo%s",PHOrNot) );
    FHistTDDDP->WriteObject(TDDDPNorEffGeoThe      , Form("TDDDPNorEffGeoThe%s",PHOrNot) );
    FHistTDDDP->WriteObject(TDDDPNorEffGeoTheTim  , Form("TDDDPNorEffGeoTheTim%s",PHOrNot) );
    
    cout<<FHistTDDDP->GetName()<<" have created!"<<endl;
    FHistTDDDP->Close();
    
    C->Close();
    FATAL("386"); throw;
    
  }
}

void  FitTrackResAnaAll( const int indexGap=28, const bool Rotate=false){

  string RfileName = Form("%stmp_mutiBash_fluxDir.sh", DirOperate);

  ofstream MutiCoreBash(RfileName.data());
  int ibatch = 0;
  int timeDelay;
  for(int i=0; i<FluxBinDXYDZCases;i++){
    string cmdlineA = "sleep ";
    string cmdlineB = 
      string(Form(" &&root -l -b %sFitTrackResAnaV3.C+\\\(%d,%f,\\\'",DirMacros,indexGap,FluxBinDXYDZWidth[i]));
    string cmdlineC = string(Form("\\\',%o\\) &\n",Rotate));
    
    timeDelay = ibatch==0?0:7; ibatch++;
    MutiCoreBash<<cmdlineA<<timeDelay<<cmdlineB<<"C"<<cmdlineC<<endl;
    timeDelay = ibatch==0?0:7; ibatch++;
    // timeDelay = ibatch==0?0:ibatch++; ibatch++;
    MutiCoreBash<<cmdlineA<<timeDelay<<cmdlineB<<"T"<<cmdlineC<<endl;
    if(ibatch==cpuCores-1) ibatch = 0;
    // FitTrackResAnaV2(indexGap,FluxBinDXYDZWidth[i],'C',Rotate); 
    // FitTrackResAnaV2(indexGap,FluxBinDXYDZWidth[i],'T',Rotate); 
    
  }
  MutiCoreBash<<"wait\n";
  MutiCoreBash<<"echo Finish all tracking by muti process."<<endl;
  MutiCoreBash.close();
  
  system(Form("sh %s",RfileName.data()));
  cout<<"\nFinish all process & rm: "<<RfileName.data()<<endl;
}