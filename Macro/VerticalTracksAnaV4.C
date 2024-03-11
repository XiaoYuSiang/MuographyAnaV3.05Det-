#include "VerticalTracksSub.C"

void VerticalTracksAna( const int indexi=28, const int indexf=29 ) {
  FATAL("Please use version: VerticalTracksAnaV4");
}
void VerticalTracksAnaV2( const bool*OperMode, const int indexi=28, const int indexf=29 ) {
  FATAL("Please use version: VerticalTracksAnaV4");
}

void VerticalTracksAnaV3( const bool*OperMode, const int indexi=28, const int indexf=29 ) {
  FATAL("Please use version: VerticalTracksAnaV4");
}
void VerticalTracksAnaV4( const bool*OperMode, const int indexi=28, const int indexf=29, const bool sepRun = false ) {
  bool testMode     = OperMode[0];
  bool rootFileMode = OperMode[4];
  defstyle();
  double eventGap[46]={};
  
  ofstream out(Form("%sSEVertical4v3.dat",DirOperate));
  string DirEFF2T, DirFiPo;
  if(sepRun){
    DirEFF2T = Form("%sEFF_2TrackGT/",DirRes_ChEff);
    locatedDirbyPath(DirEFF2T.data());
    DirFiPo = Form("%sFirePositionGT/",DirRes_ChEff);
    locatedDirbyPath(DirFiPo.data());
  }
  
  for(int i0=indexi;i0<indexf;i0++){
    eventGap[i0] = (1.0*eventGapTcnt[i0])/2560000000.;
    char RFNStr[200];
    if(sepRun) sprintf(RFNStr,"%d_%s_nHTH%dL%dCRun",eventGapTcnt[i0],TimeStr,TriggerS,TriggerL);
    else       sprintf(RFNStr,"%d_%s_nHTH%dL%d",eventGapTcnt[i0],TimeStr,TriggerS,TriggerL);
    
    if(sepRun) VerticalTracksSubControlSepR( RFNStr, testMode);
    else VerticalTracksSubControl( RFNStr, testMode);
    cout<<endl<<"end convert !!\n";
    for(int irun=0; irun<(sepRun?RunNums:1); irun++){
      
      int tmpRID = RunID[irun];
      string str_Run =  sepRun ?Form("_Run%02.f",tmpRID*1.):"";
      
      string TrackData = 
        sepRun ? Form("%sTracksGT%s%d.root",DirOperate,RFNStr,tmpRID)
                : Form("%sTracksGT%s.root",DirOperate,RFNStr);
      cout<<" check size of "<<TrackData.data()<<endl;
      if(FileSize(TrackData.data())<=0){
        cout<<"Skip this null run, run: "<<tmpRID<<endl;
        continue;
      } 
      TFile *rotfile_0 = TFile::Open(TrackData.data());
      TTree *tf = (TTree*) rotfile_0 ->Get("t");
      cout<<"L38 Opening the root file: "<<TrackData<<endl;
      cout<<"Opening the root file: "<<rotfile_0->GetName()<<endl;
      
      TH1F *hMod         = new TH1F("hMod"    ,"",ndivise,FirDaySec,FinDaySec);
      TH1F *hModG        = new TH1F("hModG"   ,"",ndivise,FirDaySec,FinDaySec);
      TH1F *hN0XX3       = new TH1F("hN0XX3"  ,"",NumChLys,0,NumChLys);
      TH1F *hNX12X       = new TH1F("hNX12X"  ,"",NumChLys,0,NumChLys);
      TH1F *hN01X3       = new TH1F("hN01X3"  ,"",NumChLys,0,NumChLys);
      TH1F *hN0X23       = new TH1F("hN0X23"  ,"",NumChLys,0,NumChLys);
      TH1F *hNX123       = new TH1F("hNX123"  ,"",NumChLys,0,NumChLys);
      TH1F *hN012X       = new TH1F("hN012X"  ,"",NumChLys,0,NumChLys);
      TH1F *hN0123       = new TH1F("hN0123"  ,"",NumChLys,0,NumChLys);
      TH1F *hEfEq1       = new TH1F("hEfEq1"  ,"",NumChLys,0,NumChLys);
      TH2F *hNHit2D      = new TH2F("hNHit2D" ,"",NumChLys,0,NumChLys,NumLY,0,NumLY);
      for(int ichlys = 0;ichlys<NumChLys;ichlys++){
        hEfEq1->Fill(ichlys);
      }
      
      TCanvas *ctmp = new TCanvas("ctmp","",100,100);
      ctmp->cd();
      char inputForm[30];
      sprintf(inputForm,"iX+iY*%d",NumnX*NumBX);
      tf->Draw(Form("%s>>hN0XX3",inputForm),"BLay==15||BLay==13||BLay==11||BLay==9","box");
      tf->Draw(Form("%s>>hNX12X",inputForm),"BLay==15||BLay==6"                    ,"box");
      tf->Draw(Form("%s>>hN01X3",inputForm),"BLay==15||BLay==13"                   ,"box");
      tf->Draw(Form("%s>>hN0X23",inputForm),"BLay==15||BLay==11"                   ,"box");
      tf->Draw(Form("%s>>hNX123",inputForm),"BLay==15||BLay==7"                    ,"box");
      tf->Draw(Form("%s>>hN012X",inputForm),"BLay==15||BLay==14"                   ,"box");
      tf->Draw(Form("%s>>hN0123",inputForm),"BLay==15"                             ,"box");
      ctmp->Close();
      
      TCanvas *c1 = new TCanvas("c1","",1800,1200);
      c1->Divide(3,2);
      
      c1->cd(1);
      hN01X3->Draw("box");
      hN0123->Draw("boxsame");
      c1->cd(2);
      hN0X23->Draw("box");
      hN0123->Draw("boxsame");
      c1->cd(4);
      hN012X->Draw("box");
      hN0123->Draw("boxsame");
      c1->cd(5);
      hNX123->Draw("box");
      hN0123->Draw("boxsame");
      TH1F *hNX[7]={
        hNX123,hN0X23,hN01X3,hN012X,
      };
      

      int NExtrem[2]={1000000000,0};
      for(int iL=0;iL<NumLY;iL++){

        if(NExtrem[0] > hNX[iL]->GetMinimum()) NExtrem[0] =hNX[iL]->GetMinimum();
        if(NExtrem[1] < hNX[iL]->GetMaximum()) NExtrem[1] =hNX[iL]->GetMaximum();
      }
      cout<<"NExtrem [0]:[1]\t"<<NExtrem[0]<<"\t"<<NExtrem[1]<<endl;
      
      hN0123->SetLineColor(3);
      hEfEq1->SetLineColor(1);
      hEfEq1->SetLineStyle(2);
      hEfEq1->GetXaxis()->SetTitle("Channel(iX + iY*NumnX*NumBX)");
      hEfEq1->GetXaxis()->CenterTitle(true);
      hEfEq1->GetYaxis()->SetTitle("Effciency");
      hEfEq1->GetYaxis()->SetRangeUser(0,1.2);
      hEfEq1->GetXaxis()->SetLabelSize(.035);
      hEfEq1->GetYaxis()->SetLabelSize(.035);
      hEfEq1->GetXaxis()->SetTitleOffset(1.6);
      hEfEq1->GetYaxis()->SetTitleOffset(1.6);
      hEfEq1->SetStats(0);
      
      
      for(int iL=0;iL<NumLY;iL++){
        hNX[iL]->GetXaxis()->SetTitle("Channel(iX + iY*NumnX*NumBX)");
        hNX[iL]->GetXaxis()->CenterTitle(true);
        hNX[iL]->GetYaxis()->SetTitle("Number of Event");
        hNX[iL]->GetXaxis()->SetLabelSize(.035);
        hNX[iL]->GetYaxis()->SetLabelSize(.035);
        hNX[iL]->GetXaxis()->SetTitleOffset(1.6);
        hNX[iL]->GetYaxis()->SetTitleOffset(1.6);
        hNX[iL]->GetYaxis()->SetRangeUser(NExtrem[0],NExtrem[1]*1.2);
        hNX[iL]->SetLineColor(2-iL/2);
      }
      
      // TH1F *hEffL0, *hEffL1, *hEffL2, *hEffL3;
      TH1F *hEff[7]={};
      for(int iL=0;iL<NumLY;iL++){
        hEff[iL] = (TH1F*) hN0123->Clone();
        hEff[iL]-> SetName(Form("hEffL%d",iL));
        hEff[iL]-> SetTitle(Form("Efficiency: Lay %d(iZ = %d)",iL,iL));
        hEff[iL]-> Divide(hNX[iL]);
        hEff[iL]->SetStats(0);
        hEff[iL]->SetLineColor(iL/2+1);
      }

      c1->cd(3);
      hEfEq1->Draw("box");
      hEff[1]->Draw("boxsame");
      hEff[2]->Draw("boxsame");
      c1->cd(6);
      hEfEq1->Draw("box");
      hEff[0]->Draw("boxsame");
      hEff[3]->Draw("boxsame");
      
      DrawPdfPng(c1, Form("%sEFF_2TrackGT%d_%s%s",DirEFF2T.data(),eventGapTcnt[i0],TimeStr,str_Run.data()));

      int num0XX3=0,num01X3=0,num0X23=0,num0123=0;  
      int numX12X=0,numX123=0,num012X=0;  
      for(int ichlys = 1;ichlys<=NumChLys;ichlys++){
        num0XX3+=hN0XX3->GetBinContent(ichlys);
        num01X3+=hN01X3->GetBinContent(ichlys);
        num0X23+=hN0X23->GetBinContent(ichlys);
        num0123+=hN0123->GetBinContent(ichlys);
        numX12X+=hNX12X->GetBinContent(ichlys);
        numX123+=hNX123->GetBinContent(ichlys);
        num012X+=hN012X->GetBinContent(ichlys);
      }
      cout<<eventGapTcnt[i0]<<"\t";
      cout<<num0XX3<<"\t";
      cout<<num01X3<<"\t";
      cout<<num0X23<<"\t";
      cout<<num0123<<"\t";
      cout<<numX12X<<"\t";
      cout<<numX123<<"\t";
      cout<<num012X<<"\n";
      
      
      char seCondition[50] = "BLay==15||BLay==13||BLay==11||BLay==9";
      
      TH1F *hETExtrem = new TH1F("hETExtrem","",NumChLys,0,NumChLys);
      int ETExtrem[2]={1000000000,0};
      for(int iL=0;iL<NumLY;iL++){
        tf->Draw(Form("%s>>hETExtrem",inputForm),Form("%s&&iZ==%d",seCondition,iL),"box");
        if(ETExtrem[0] > hETExtrem->GetMinimum()) ETExtrem[0] =hETExtrem->GetMinimum();
        if(ETExtrem[1] < hETExtrem->GetMaximum()) ETExtrem[1] =hETExtrem->GetMaximum();
      }
      cout<<"ETExtrem [0]:[1]\t"<<ETExtrem[0]<<"\t"<<ETExtrem[1]<<endl;
      
      
      TCanvas *cTrackLy = new TCanvas("cTrackLy","",360*NumLY,360);
      cTrackLy->Divide(NumLY,1);
      TH2F *hETrack[NumLY]={};
      for(int iL=0;iL<NumLY;iL++){
        hETrack[iL] = new TH2F(Form("hETrackL%d",iL),"",NumnX*NumBX,0,NumnX*NumBX,NumnY*NumBY,0,NumnY*NumBY);
        hETrack[iL]-> SetTitle(Form("Fire position of EFF-used track at iZ = %d",iL));
        setTitle(hETrack[iL],"iX","iY","");
        hETrack[iL]->GetXaxis()->CenterTitle(true);
        hETrack[iL]->GetYaxis()->CenterTitle(true);
        hETrack[iL]->GetXaxis()->CenterLabels(true);
        hETrack[iL]->GetYaxis()->CenterLabels(true);
        hETrack[iL]->GetXaxis()->SetNdivisions(8);
        hETrack[iL]->GetYaxis()->SetNdivisions(8);
        hETrack[iL]->GetXaxis()->SetTitleOffset(1.0);
        hETrack[iL]->GetYaxis()->SetTitleOffset(1.0);
        hETrack[iL]->GetZaxis()->SetTitleOffset(0);
        hETrack[iL]->GetXaxis()->SetLabelSize(0.05);
        hETrack[iL]->GetYaxis()->SetLabelSize(0.05);
        hETrack[iL]->GetZaxis()->SetLabelSize(0.04);
        hETrack[iL]->GetXaxis()->SetTitleSize(0.05);
        hETrack[iL]->GetYaxis()->SetTitleSize(0.05);
        hETrack[iL]->GetZaxis()->SetRangeUser(ETExtrem[0] , ETExtrem[1]);
        hETrack[iL]->SetStats(0);
        cTrackLy->cd(1+iL);
        tf->Draw(Form("iY:iX>>hETrackL%d",iL),Form("%s&&iZ==%d",seCondition,iL),"colz");
      }
      
      
      DrawPdfPng(cTrackLy, Form("%sFirePositionGT%d_%s%s",DirFiPo.data(),eventGapTcnt[i0],TimeStr,str_Run.data()));
      
      cTrackLy->Close();
      c1->Close();
      delete hETExtrem, *hETrack;
    }
  }
}
