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
#include "/home/yusiang/personalLib/Style/DBMTStyle.h"

using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace MuoAna_path_dir;
using namespace DataConst;

void txtDefault(float px, float py, const char* content, double size, int rotateD, TCanvas *c1){
  c1->cd();
  TText *txt = new TText(px,py,content);
  txt->SetTextSize(size);
  txt->SetTextAlign(22);
  txt->SetTextFont(152);
  txt->SetTextAngle(rotateD);
  txt->Draw();
}


void  FitTrackResAna(const int indexGap=28){
  defstyle();
  char DirRoot[1][100] = {
    ""
  };
  sprintf(DirRoot[0],"%s",DirOperate);

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
    t->Draw("Tr_The/TMath::Pi()*180.:Tr_Phi/TMath::Pi()*180.>>TRS","","colz");
    // t->Draw("Tr_The/TMath::Pi()*180.:Tr_Phi/TMath::Pi()*180.","","colz");
    t->Draw("Tr_The/TMath::Pi()*180.:Tr_Phi/TMath::Pi()*180.>>TRSC","","colz");
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
    C->Print(Form("%sTryThetaVsPhiSP.pdf",DirSave));
    C->Print(Form("%sTryThetaVsPhiSP.png",DirSave));
    C->SetTheta(45);
    C->Print(Form("%sTryThetaVsPhiSP45.pdf",DirSave));
    C->Print(Form("%sTryThetaVsPhiSP45.png",DirSave));
    
    
    TH2F *TDDD = new TH2F("TDDD","#font[12]{Tracking result}",30,-0.15,0.15,30,-0.15,0.15);
    TDDD->SetStats(0);
    t->Draw("DyDz:DxDz>>TDDD","","colz");
    TDDD->GetXaxis()->SetTitle("dx/dz");
    TDDD->GetXaxis()->SetLabelSize(0.03);
    TDDD->GetYaxis()->SetTitle("dy/dz");
    TDDD->GetYaxis()->SetTitleOffset(1.4);
    TDDD->GetYaxis()->SetLabelSize(0.03);
    TDDD->GetZaxis()->SetTitle("Numbers of event");
    TDDD->GetZaxis()->SetTitleOffset(1.4);
    TDDD->GetZaxis()->RotateTitle(true);
    C->Print(Form("%sDxDzVSDyDz.pdf",DirSave));
    C->Print(Form("%sDxDzVSDyDz.png",DirSave));
    
    
    
  }
}