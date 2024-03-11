//include
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
#include <TPaletteAxis.h>
#include <TText.h>
#include <TLatex.h>
#include "TStyle.h"
#include <TColor.h>
#include <TGaxis.h>
#include "TF2.h"
#include "TExec.h"
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "/data4/YuSiang/personalLib/Style/DBMTStyle.h"
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
void SetStyleDatEff()
{
  static const int colorSlice=20;
  static int       colors[colorSlice];
  // static Bool_t initialized = kFALSE;
    
  // if(!initialized){
    int colorR=0,colorG=0,colorB=0;
    for (int is=0; is<colorSlice-1; is++){
      colorG = 255./(1.*colorSlice)*(1.*(1+is));
      colorR = 255-colorG;
      colors[is] = TColor::GetColor(Form("#%02x%02x%02x",colorR,colorG,colorB));
      // cout<<colorR<<"\t"<<colorG<<"\t"<<colorB<<"\t"<<colors[is]<<"\t"<<Form("#%02x%02x%02x",colorR,colorG,colorB)<<endl;
    }
    colorR=128; colorG=128; colorB=255;
    colors[colorSlice-1] = TColor::GetColor(Form("#%02x%02x%02x",colorR,colorG,colorB));
      // cout<<colorR<<"\t"<<colorG<<"\t"<<colorB<<"\t"<<colors[colorSlice-1]<<"\t"<<Form("#%02x%02x%02x",colorR,colorG,colorB)<<endl;
    // initialized = kTRUE;
  // }
  gStyle->SetPalette(colorSlice,colors);


  gStyle->SetPadRightMargin(0.15);
 // gStyle->SetPadBottomMargin(0.15);
 
}

void TPadLabel(TPad* TPZL,TCanvas *c0){
  c0->cd();
  //delete TPZL;
  TPZL = new TPad("TPZL","",0.88,0,0.93,1);
  TPZL->SetFillStyle(4000); //will be transparent
  TPZL->SetFrameFillStyle(4000);
  TPZL->Draw();
  TPZL->cd();
  TPZL->Update();
  TLatex TPercent;
  TPercent.SetTextAlign(22);
  TPercent.SetTextSize(0.4);
  for(int i=0;i<10;i++) TPercent.DrawLatex(0.5,0.1+0.04*(2*i),Form("%d%%",5*(2*i+1)));
  TPercent.DrawLatex(0.5,0.88,Form(">99.9%%"));
  TPercent.Draw();
  TPZL->Update();
  c0->cd();
  c0->Update();
}


//channel vs rate combine
void DayEAna() {
  cout<<"Please use the new version: void DayEAnaV3()"<<endl;
}
//channel vs rate combine
void DayEAnaV1(){ DayEAna();}
void DayEAnaV2(){ DayEAna();}

void DayEAnaV3(){ DayEAna();}

void DayEAnaV4(const int SPD = 24) {
  defstyle();
  //Printf("Files found: %i", (int) files.size());
  vector<string> vRootFiles = RawRootList(DirOperate);
  
  // for(int i=0;i<vRootFiles.size();i++) cout<<vRootFiles[i]<<endl;
  TreeReader data(vRootFiles);
  
  ofstream out(Form("%sDayEffAna.txt",DirOperate));
  //Data Variable
  //Int_t    frame   ;//event
  Int_t    unixtime;//abs time of event [s]
  Int_t    nHits   ;
  Int_t*   board   ;
  Int_t*   channel ;
  Int_t    tYear   ;
  Int_t    tMonth  ;
  Int_t    tDate   ;
  Int_t    tHour   ;
  Int_t    tMinute ;
  Int_t    tSecond ;
  //take time and set anlyze Constant for boundry condition

  int nd10min = (ndivise)*24*6, ndayChip = (ndivise)*SPD;
  float BinHours = 24./SPD;

  cout<<"nd10min:  "<<nd10min<<endl;
  //data hist create
  TH2F *htmp  = new TH2F("htmp","",nd10min,FirDaySec,FinDaySec,NumBD,0,NumBD);//10min
  TH2F *htmpc = new TH2F("htmpc","",nd10min,FirDaySec,FinDaySec,NumBD*NumCh,0,NumBD*NumCh);//10min
  TH2F *hModRT = Mod_DateLabel("hModRT","",NumBD*NumCh,0, NumBD*NumCh,"Readout channel(GID)","Data collection Rate(Hz)");
  TH2F *hModBT = Mod_DateLabel("hModBT","",NumBD      ,0, NumBD      ,"Readout board"  ,"Data collection efficiency");
  TH2F *hModCT = Mod_DateLabel("hModCT","",NumBD*NumCh,0, NumBD*NumCh,"Readout channel","Data collection efficiency");
  TH2F *hBT  = new TH2F("hBT","",ndayChip,FirDaySec,FinDaySec,NumBD      ,0,NumBD      );//10min 
  TH2F *hCT  = new TH2F("hCT","",ndayChip,FirDaySec,FinDaySec,NumBD*NumCh,0,NumBD*NumCh);//10min
  TH2F *hRT  = new TH2F("hRT","",nd10min,FirDaySec,FinDaySec,NumBD*NumCh,0,NumBD*NumCh);//10min
  //TH2F *hBT  = new TH2F("hBT","",Dayf-Dayi+4,Dayi-1,Dayf+3,4,0,4);//10min
  int t1=0;
  int oiev=0, iev=0, XBins=0;
  
  for(int iev = 0; iev <evs; iev++){
    data.GetEntry(iev);
    if(iev%1000 == 0) cout<<Form("\r%.5f%%\t%d",(iev*100.)/(1.*evs))<<flush;
    t1        = data.GetLong64("unixtime");
    board     = data.GetPtrInt("board");
    channel   = data.GetPtrInt("channel");
    nHits     = data.GetInt("nHits");
    //XBins     = (t1-FirDaySec)/600.;
    // if(iev%10000==0) cout<<XBins<<endl;
    for(int iHit = 0; iHit<nHits ; iHit++){
      // if(17<=board[iHit]) continue;
      htmp->Fill(t1,BDCheck(board[iHit]));
      htmpc->Fill(t1,(BDCheck(board[iHit]))*NumCh+channel[iHit]);
    }
    
  }
  
  cout<<"unixtimei:    "<<unixtimei<<endl;
  cout<<"unixtimef:    "<<unixtimef<<endl;
  const float BinNum10mPerBin = 86400./(1.*SPD)/600.;
  
  for(int iBin = 0; iBin<nd10min ; iBin++){
    t1 = htmp->GetXaxis()->GetBinCenter(iBin+1);
    // cout<<t1<<endl;
    //cout<<iBin/144<<endl;
    for(int ibd = 0; ibd<NumBD ; ibd++){
      if (htmp->GetBinContent(iBin+1,1+ibd)!=0) hBT->Fill(t1,ibd,1/BinNum10mPerBin);
      for(int ic = 0; ic<NumCh;ic++){
        if (htmpc->GetBinContent(iBin+1,1+ic+ibd*NumCh)!=0)  hCT->Fill(t1,ic+ibd*NumCh,1/BinNum10mPerBin);
        hRT->SetBinContent(iBin+1,1+ic+ibd*NumCh, htmpc->GetBinContent(iBin+1,1+ic+ibd*NumCh)/600.);
      }//@@ 161
    }
  }
  // throw;
  for(int ichip=1;ichip<=ndayChip;ichip++){
    for(int ibd = 0; ibd<NumBD ; ibd++){
      if(hBT->GetBinContent(ichip,1+ibd)>TriggerEffOver) {
        hBT->SetBinContent(ichip,1+ibd,1);
      }else 
        // cout<<ichip<<"\t"<<hBT->GetBinContent(ichip,1+ibd,1)<<endl;
      for(int ic = 0; ic<NumCh;ic++){
        if(hCT->GetBinContent(ichip,1+ic+ibd*NumCh)>TriggerEffOver) hCT->SetBinContent(ichip,1+ic+ibd*NumCh,1);
      }
    }
  }
  if (BinHours<=1){
    if(neglectHourRange[1]>neglectHourRange[0]){
      printf("Warning!!! Programs is working on neglectHourRange Mode.\n");
      printf("Warning!!! If tHour in range (%d,%d) would be neglect!!\n",neglectHourRange[0],neglectHourRange[1]);
      printf("Note: Adjust AnaVariable.h neglectHourRange[2]={-1,-1} can close the mode\n");
      // throw;
      ofstream outPR(Form("%sDayEffProblemRange.dat",DirOperate));
      
      for(int ichip=1;ichip<=ndayChip;ichip++){
        int tHr = UTtotHour(hBT->GetXaxis()->GetBinCenter(ichip),timeZone);
        if( tHr<neglectHourRange[0]||tHr>neglectHourRange[1]) continue;
        double UTST = 0, UTFT = 0;
        for(int ich=0;ich<NumBD*NumCh;ich++){
        /* for(int ic = 0; ic<NumCh;ic++){ */
          float Tmp_Eff = hCT->GetBinContent(ichip,1+ich);
          float Tmp_Fre = hRT->GetBinContent(ichip,1+ich);
          if( Tmp_Eff < TriggerNegEffInHour || Tmp_Fre > TriggerRatOver ){
            double
              UTS = hBT->GetXaxis()->GetBinCenter(ichip+1)-hBT->GetXaxis()->GetBinWidth(ichip+1)/2,
              UTF = hBT->GetXaxis()->GetBinCenter(ichip+1)+hBT->GetXaxis()->GetBinWidth(ichip+1)/2;
            if(UTST == UTS && UTFT == UTF) break;
            outPR<<Form("%.0f\t%.0f\n",UTS,UTF);
            cout <<Form("%.0f\t%.0f\t%f/%f\n",UTS,UTF,Tmp_Eff,TriggerNegEffInHour);
            UTST = UTS; UTFT = UTF;
            break;
          }
        // }
          
        }
      }
      cout<<"Finished creat: "<< Form("%sDayEffProblemRange.dat",DirOperate)<<endl;
      outPR.close();
    }
  }
  TCanvas *c0 = new TCanvas("c0","c0",1920,1080);
  c0->cd();
  htmpc->Draw("colz");
  // htmpc->GetZaxis()->SetRangeUser(0,10);
  c0->Print(Form("%shtmpc%scom.png",DirRes_DayEff,TimeStr));
  c0->Print(Form("%shtmpc%scom.pdf",DirRes_DayEff,TimeStr));
  
  
  hRT->GetZaxis()->SetTitle(hModRT->GetZaxis()->GetTitle());
  hModRT->Draw("colz");
  hRT->Draw("colzsame");
  DrawPdfPng(c0,Form("%stest_hrt_SPD%02.f_%scom",DirRes_DayEff,SPD*1.,TimeStr));
  
  SetStyleDatEff();
  TCanvas *c1 = new TCanvas("c1","c1",1920,1080);
  TCanvas *c2 = new TCanvas("c2","c2",1920,1080);
  
  //channel label
  for(int ibd = 0; ibd<NumBD ; ibd++){
    hModBT->GetYaxis()->SetBinLabel(ibd+1 ,Form("MTB%02.f",BD[ibd]*1.));
    for(int ic=0;ic<NumCh;ic++){
      hModCT->GetYaxis()->SetBinLabel(1+ic+ibd*NumCh ,Form("MTB%02.f-%02.f",BD[ibd]*1.,ic*1.));
    }
  }
  //TExec *ex2 = new TExec("ex2","SetStyleDatEff();");
  //ex2->Draw();

  hBT   ->SetStats(0);
  hBT   ->GetZaxis()->SetTitle(Form("Data collection efficiency(#frac{working time}{%.2f(Hour)})",BinHours));
  hModBT->GetXaxis()->SetTitle(Form("Date (%dBin/day=%.2fHr/Bin)",SPD,BinHours));
  hModBT->SetTitle(Form("All MTBs"));
  hModBT->GetYaxis()->SetTitleSize(0.05);
  hBT   ->GetZaxis()->SetTitleSize(0.04);
  hModBT->GetXaxis()->SetTitleOffset(1.5);
  hModBT->GetYaxis()->SetTitleOffset(1.0);
  hBT   ->GetZaxis()->SetTitleOffset(1.2);
  hBT   ->GetZaxis()->CenterTitle(true);
  hBT   ->GetZaxis()->RotateTitle(true);
  
  hModBT->GetXaxis()->SetLabelSize(0.025);
  hBT   ->GetZaxis()->SetLabelSize(0.025);
  hModBT->GetXaxis()->SetLabelOffset(0.004);
  hBT   ->GetZaxis()->SetLabelOffset(1);
  
  hModBT->GetXaxis()->SetRangeUser(Dayi,Dayf);
  hBT   ->GetZaxis()->SetRangeUser(0.05,1.05);
  // hModBT->GetXaxis()->SetNdivisions(ndivise);
  hBT   ->GetZaxis()->SetNdivisions(-10);

  // hBT->GetZaxis()->SetBinLabel(20,">99.75");
  
  hCT   ->SetStats(0);
  hCT   ->SetTitle("");
  hCT   ->GetZaxis()->SetTitle(Form("Data collection efficiency(#frac{working time}{%.2f(Hour)})",BinHours));
  hModCT->GetXaxis()->SetTitle(Form("Date (%dBin/day=%.2fHr/Bin)",SPD,BinHours));
  
  hModCT->GetYaxis()->SetTitleSize(0.04);
  hCT   ->GetZaxis()->SetTitleSize(0.04);
  hModCT->GetXaxis()->SetTitleOffset(1.5);
  hModCT->GetYaxis()->SetTitleOffset(1.3);
  hCT   ->GetZaxis()->SetTitleOffset(1.2);
  hCT   ->GetZaxis()->CenterTitle(true);
  hCT   ->GetZaxis()->RotateTitle(true);
  
  hModCT->GetXaxis()->SetLabelSize(0.025);
  hModCT->GetYaxis()->SetLabelSize(0.025);
  hCT   ->GetZaxis()->SetLabelSize(0.025);
  hModCT->GetXaxis()->SetLabelOffset(0.004);
  hCT   ->GetZaxis()->SetLabelOffset(1);
  
  hModCT->GetXaxis()->SetRangeUser(Dayi,Dayf);
  hCT   ->GetZaxis()->SetRangeUser(0.05,1.05);
  // hModCT->GetXaxis()->SetNdivisions(ndivise);
  hCT   ->GetZaxis()->SetNdivisions(-10);
  
  

  //L->Draw();

  TPaletteAxis *palette;
   
  c1->cd();
  hModBT->Draw("col");
  hBT   ->Draw("colzsame");
  c1->Update();
  palette = (TPaletteAxis*) hBT->GetListOfFunctions()->FindObject("palette");
  palette->SetX1NDC(0.855);
  palette->SetX2NDC(0.88);
  palette->SetY1NDC(0.10);
  
  
  TPad *pad1 = new TPad("pad1","",0,0,1,1);
  pad1->SetFillStyle(4000); //will be transparent
  pad1->SetFrameFillStyle(4000);
  
  pad1->SetGrid(0,1);
  pad1->SetTicks(1,1);
  pad1->Draw();
  pad1->cd();
  hModBT->Draw("col");

  TPad *TPZL = new TPad();
  TPadLabel( TPZL, c1);
  
  
  DrawPdfPng(c1,Form("%sDayEffBoardcolz_SPD%02.f_%scom",DirRes_DayEff,SPD*1.,TimeStr));
  
  delete pad1;
  

  TCanvas *c3 = new TCanvas("c3","c3",1920*NumBX,1080*NumBY);
  c3->Divide(NumBX,NumBY);

  for(int iLayDiv = 0; iLayDiv*4<NumBD; iLayDiv++ ){
    TH2F *htmpModCT = (TH2F*) hModCT->Clone();
    htmpModCT->SetName("htmpModCT");
    htmpModCT->GetYaxis()->SetRangeUser(iLayDiv*4*NumCh,(iLayDiv+1)*4*NumCh);
    htmpModCT->SetTitle(Form("MTB: %02.f    %02.f    %02.f    %02.f    (iZ = %d)"
                      ,BD[iLayDiv*4+0]*1.,BD[iLayDiv*4+1]*1.,BD[iLayDiv*4+2]*1.,BD[iLayDiv*4+3]*1.,iLayDiv));
    
    c2->cd();
    htmpModCT->Draw("col");
    hCT ->Draw("colzsame");
    htmpModCT->Draw("colsame");

    c2->Update();
    palette = (TPaletteAxis*) hCT->GetListOfFunctions()->FindObject("palette");
    palette->SetX1NDC(0.855);
    palette->SetX2NDC(0.88);
    palette->SetY1NDC(0.10);

    pad1 = new TPad("pad1","",0,0,1,1);
    pad1->SetFillStyle(4000); //will be transparent
    pad1->SetFrameFillStyle(4000);
    pad1->SetGrid(0,1);
    pad1->SetTicks(1,1);
    pad1->Draw();
    pad1->cd();
    htmpModCT->Draw("col");

    TPadLabel( TPZL, c2);
    DrawPdfPng(c2,Form("%sDayEffChanncolz_SPD%02.f_%scom_Part%02.f",DirRes_DayEff,SPD*1.,TimeStr,iLayDiv*1.));
    
    c3->cd(iLayDiv+1);
    c2->DrawClonePad();

    delete htmpModCT;
  }
  DrawPdfPng(c3,Form("%sDayEffChanncolz_SPD%02.f_%scom",DirRes_DayEff,SPD*1.,TimeStr));
  
  
  
  out.close();
  htmp->Delete();
  htmpc->Delete();
  hModBT->Delete();
  hModCT->Delete();
  hBT->Delete();
  hCT->Delete();
  // delete htmp, htmpc, hModBT, hModCT, hBT, hCT;
  c0->Close();
  c1->Close();
  c2->Close();
  c3->Close();
  delete c0, c1, c2, c3;
  delete TPZL;
}

void DayEAnaV5(){
  DayEAnaV4(); //Execute SPD = 24
  DayEAnaV4(SlicePerDay); //Execute SPD = SlicePerDay
} 