#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TMath.h>
#include <TF1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TText.h>
#include <TPDF.h>
#include "TH2.h"
#include "TProfile.h"
#include <TProfile2D.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TGaxis.h>
#include <TChain.h>
#include <TObject.h>
#include <TStopwatch.h>
#include "/home/yusiang/personalLib/Style/DBMTStyle.h"
#include "/home/yusiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
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



//channel vs rate combine
void PwidthAna() {
  cout<< "Please use the last version: PwidthAnaV2(const bool*OperMode, const int itcntgap=28)"<<endl;
  throw;
}


void PwidthAnaV2(const bool*OperMode, const int itcntgap=28) {
  defstyle();
  bool testMode        =true;  testMode         = OperMode[0];
  bool normalizeMode   =true;  normalizeMode    = OperMode[1];
  bool NegHiRatChMode  =true;  NegHiRatChMode   = OperMode[2];
  bool lowstatisticMode=false; lowstatisticMode = OperMode[3];
  bool rootFileMode    =true;  rootFileMode     = OperMode[4];
  bool SETightMode     =true;  SETightMode      = OperMode[5];
  vector<string> vRootFiles = RawRootList(DirOperate);
  // for(int i=0;i<100;i++) cout<<vRootFiles[i]<<endl;
  
  vector<Long64_t> VNUTRS, VNUTRF;//Vector of Neglect Unixtime Range Start to Final
  GetNegHourRange(VNUTRS,VNUTRF, DirOperate);
  string Tmp_SGTR = UTNotInNegHourRange(VNUTRS,VNUTRF);//Tmp_StrGoodTimeRange
  char SGTR[Tmp_SGTR.length()+1];
  for(int i=0;i<int(Tmp_SGTR.length()+1);i++) SGTR[i]=Tmp_SGTR[i];
    
  int binrt = 0;
  int Starttime = unixtimei, Endtime = unixtimef;
  char TestMarker[15]={};

  if (testMode){
    sprintf(TestMarker,"TST_");
    // NumBD = 1;
    // NumCh = 1;
  }
  
  if(SETightMode==false&&testMode) sprintf(TestMarker,"TST_Loose_");
  else if(SETightMode==false) sprintf(TestMarker,"Loose_");
  
  TFile *SelectRfile;
  //cout<<"000000"<<endl;
  char RFNStr[200];
  sprintf(RFNStr,"%d_%s_nHTH%dL%d.root",eventGapTcnt[itcntgap],TimeStr,TriggerS,TriggerL);
  
  if(SETightMode) SelectRfile = TFile::Open(Form("%sTracksGT%s",DirOperate,RFNStr));
  else SelectRfile = TFile::Open(Form("%sEventsGT%s",DirOperate,RFNStr));
  //else SelectRfile = TFile::Open(Form("%s%sEventsGT%s",DirOperate,TestMarker,RFNStr));
  TTree *tR  = (TTree*) SelectRfile ->Get("t");
  cout<<"opening....."<<SelectRfile->GetName()<<endl;
  
  
  

  char DirResultGPw[150]={""}, DirResultGRt[150]={""}, DirResultBDD[170]={""};
  //save path graph of Pwidth Rate

  sprintf(DirResultGPw,"%s%sBoardGPwNor/",DirResult,TestMarker);
  if (system(Form("mkdir -p %s",DirResultGPw))!=0) system(Form("mkdir -p %s",DirResultGPw));
  sprintf(DirResultGRt,"%s%sBoardGRtNor/",DirResult,TestMarker);
  if (system(Form("mkdir -p %s",DirResultGRt))!=0) system(Form("mkdir -p %s",DirResultGRt));
  sprintf(DirResultBDD,"%sBoardDetail/",DirResultGPw);  
  if (system(Form("mkdir -p %s",DirResultBDD))!=0) system(Form("mkdir -p %s",DirResultBDD));
  
  TStopwatch optime;
  optime.Start();
  binrt = (Endtime-Starttime)*1.0/th2fbinwidth;
  cout<<"binrt number:   "<<binrt<<endl;
  int fileVecSize = int(vRootFiles.size());
  cout<<"File number:   "<<fileVecSize<<endl;
  char *infilename[200]={};
  TChain *t = new TChain("t");
  if(testMode) fileVecSize = 1;
  //fileVecSize = 10;//@@@@
  for(int i=0;i<fileVecSize;i++){
    infilename[i] = &vRootFiles[i][0];
    t->Add(infilename[i]);
    ////cout<<infilename[i]<<endl;
  }
  
  
  
  gStyle->SetLineScalePS(0.3);
  //vector<string> vRootFiles = find_files(inpaths);
  //Printf("vRootFiles found: %i", (int) vRootFiles.size());
  //TreeReader data(inpaths);
  

  ofstream outRt(Form("%sRtBinsTooLarge.txt",DirOperate));
  TCanvas *cpwch = new TCanvas("cpwch","",4160,4160);
  TCanvas *cpwchSE = new TCanvas("cpwchSE","",4160,4160);
  TCanvas *cpwrt = new TCanvas("cpwrt","",4160,4160);
  TCanvas *cpw[NumBD]={},*cpwAll[NumBD]={};
  TCanvas *crt[NumBD]={},*crtAll[NumBD]={};
  TPDF *pdfcpw = new TPDF(Form("%sPwidthCompareNormalize.pdf",DirResultGPw));
  TPDF *pdfcrt = new TPDF(Form("%sRateCompareNormalize.pdf",DirResultGRt));
  TPDF *pdfcpwA = new TPDF(Form("%sPwidthACompareNormalize.pdf",DirResultGPw));
  TPDF *pdfcrtA = new TPDF(Form("%sRateACompareNormalize.pdf",DirResultGRt));
  TH1F *hPw[NumBD*NumCh]={},*hPwSE[NumBD*NumCh]={};
  TH1F *hRt[NumBD*NumCh]={};
  // TF1  *fPw[64]={},*fPwSE[64]={};

  TH2F *hchPw   = new TH2F("hchPw","hchPw",binpw,0,binpw,NumBD*NumCh,0,NumBD*NumCh);//ch64, pw: 0~180//@@

  TH2F *hchPwSE = new TH2F("hchPwSE","hchPwSE",binpw,0,binpw,NumBD*NumCh,0,NumBD*NumCh);//ch64, pw: 0~180
  TH2F *hchPwDS[2] = {hchPw,hchPwSE};//ch64, pw: 0~180
  TH2F *hchRt = new TH2F("hchRt","hchRt",binrt,Starttime,Endtime,NumBD*NumCh,0,NumBD*NumCh);//ch64, pw: 0~180
  cpwch->SetPhi(-30);
  cpwch->SetTheta(70);
  cpwchSE->SetPhi(-30);
  cpwchSE->SetTheta(70);
  FastStyleStats(.9,.9);
  cpwchSE->cd();
  char PWDDataName[3][200]={};
  sprintf(PWDDataName[0],"%s%soutPwHIData.txt"  ,DirOperate,TestMarker);
  sprintf(PWDDataName[1],"%s%soutPwSEData.txt"  ,DirOperate,TestMarker);
  if(rootFileMode){
    if(SETightMode) tR->Draw("(channel+(board-1)*16):pwidth/pwidthScaleFactor>>hchPwSE",Form("BiZ==15 &&eventHit==4&&%s",SGTR),"colz");
    else tR->Draw("(channel+(board-1)*16):pwidth/pwidthScaleFactor>>hchPwSE",Form("IDtight>=4&&%s",SGTR),"colz");
    cpwch->cd();

    t->Draw("(channel+(board-1)*16):pwidth>>hchPw",Form("%s",SGTR),"colz");//PS: no pwidthScaleFactor in *mu.root
    
    //cpwch->Update();
    char devoteCondition[40]={};
    if(NegHiRatChMode){
      sprintf(devoteCondition,"channel!=15||board!=3");
      t->Draw(Form("(channel+(board-1)*16):pwidth>>hchPw"),Form("%s",devoteCondition),"colz");
    }
    for(int i=0;i<2;i++){
      ofstream outPwData(PWDDataName[i]);
      
      if(normalizeMode){
        for(int ibd=0;ibd<NumBD;ibd++){
          for(int ich=0;ich<NumCh;ich++){
            if(NegHiRatChMode){
              if(BD[ibd]==3&&ich==15) continue;
            }
            int Tmp_CHGID = (BD[ibd]-1)*16+ich;
            // cout<<"Doing the normalization: ChGID: " <<Tmp_CHGID<<endl;
            double ChIntegralNum  = hchPwDS[i]->Integral(-1,-1,Tmp_CHGID+1,Tmp_CHGID+1);
            outPwData<<BD[ibd]<<"\t"<<ich<<"\t"<<int(ChIntegralNum);

            for(int ibinpw=0;ibinpw<binpw;ibinpw++){
              double ChOriginNum =hchPwDS[i]->Integral(ibinpw+1,ibinpw+1,Tmp_CHGID+1,Tmp_CHGID+1);
              double interData = 0;
              if(ChOriginNum==0) interData=0;
              else interData = ChOriginNum/ChIntegralNum;
              hchPwDS[i]->SetBinContent(ibinpw+1,Tmp_CHGID+1,interData);
              outPwData<<"\t"<<interData;
              // cout<<ibinpw<<"    "<<Tmp_CHGID+1<<"    "<<ChOriginNum/ChIntegralNum<<endl;
              // cout<<ibinpw<<"    "<<Tmp_CHGID+1<<"    "<<hchPw->GetBinContent(2*ibinpw+2,Tmp_CHGID+1+1)<<endl;
            } 
            outPwData<<"\n";
            //cout<<ich+ibd*16<<"      "<<hchPwDS[i]->Integral(-1,-1,Tmp_CHGID+1+1,Tmp_CHGID+1+1)<<endl;
          }
        }
      }
      outPwData.close();
      setTitle(hchPwDS[i],"Pwidth/pwidthScaleFactor(100ns)","ChGID (board*16+channel)","Number of Hit");
      hchPwDS[i]->GetXaxis()->SetTitleOffset(1.4);
      hchPwDS[i]->GetYaxis()->SetTitleOffset(1.4);
      hchPwDS[i]->GetZaxis()->SetTitleOffset(1.4);
      hchPwDS[i]->GetZaxis()->SetLabelSize(0.025);
      if(normalizeMode) hchPwDS[i]->GetZaxis()->SetTitle("Ratio");
    }
  }
  
  
  
  
  DrawPdfPng(cpwchSE,Form("%shchPwSEColzNormal",DirResultGPw));
  DrawPdfPng(cpwch  ,Form("%shchPwColzNormal"  ,DirResultGPw));
  double PWDTmpA[NumBD+1][NumCh][binpw];//[SE | Hit][bd][ch][pwd]
  double PWDTmpB[NumBD+1][NumCh][binpw];//[SE | Hit][bd][ch][pwd]
  int    PWNTmp[2][NumBD+1][NumCh];
  
  // cout<<198<<endl;
 // for(int ic=0;ic<2;ic++){
      //"/data4/YuSiang/S4_11F_4lay/OperateResule/outRTHIData.txt",
    //cout<<PWDDataName[1]<<endl;
    ifstream in0(PWDDataName[0]);
    cout<<"reading:  "<<PWDDataName[0]<<endl;
    ifstream in1(PWDDataName[1]);
    cout<<"reading:  "<<PWDDataName[1]<<endl;
    // cout<<203<<endl;
    for(int i0=0,ich=0,ibd=0,numtmp;i0<NumCh*NumBD;i0++){
      
      in0>>ibd>>ich;
      in1>>ibd>>ich;
      
      ibd = BDcheck(ibd);//@@ibd  bid
      
      in0>>numtmp;
      PWNTmp[0][ibd][ich]=numtmp;
      in1>>numtmp;
      PWNTmp[1][ibd][ich]=numtmp;

      cout<<ibd<<"\t"<<ich<<"\t"<<PWNTmp[0][ibd][ich]<<"\t"<<PWNTmp[1][ibd][ich]<<endl;
      for(int ibin=0;ibin<binpw;ibin++){
        double pwtmp0=0,pwtmp1=0;
        in0>>pwtmp0;
        in1>>pwtmp1;
        // const int bd = ibd, ch =ich;
        PWDTmpA[ibd][ich][ibin] = pwtmp0;
        PWDTmpB[ibd][ich][ibin] = pwtmp1;
    // cout<<Form("%d\t%d\t%d\t%d\t%f",0,ibd,ich,ibin,pwtmp0)<<endl;
    // cout<<Form("%d\t%d\t%d\t%d\t%f",1,ibd,ich,ibin,pwtmp1)<<endl;
        //cout<<"\t"<<ic<<"\t"<<ibd<<"\t"<<ich<<"\t"<<ibin;
        //cout<<"\t"<<PWDTmp[ic][ibd][ich][ibin];
        //cout<<"\t"<<pwtmp;
      }
      //cout<<endl;
    }
    in0.close();
    in1.close();

  // }

  FastStyleStats(0.95,0.95);
  TH2F *hchPwL   = new TH2F(*hchPw);
  TH2F *hchPwLSE = new TH2F(*hchPwSE);
  TH2F    *hchPwLDS[2] ={hchPwL,hchPwLSE};//all data and selection data 
  TCanvas *cpwchDS [2] ={cpwch,cpwchSE};//all data and selection data 

  for(int i=0;i<2;i++){
    cpwchDS[i]->cd();
    if(hchPwLDS[i]->Integral(-1,-1,-1,-1)!=0){
      cout<<"hchPwLDS[i]->Integral(-1,-1,-1,-1)!=0"<<endl;
      hchPwLDS[i]->Draw("Lego2");
      hchPwLDS[i]->GetXaxis()->SetRangeUser(0,binpw);
      setAxisLabelSize(hchPwLDS[i],0.03,0.03,0.025);
      setAxisTitleSize(hchPwLDS[i],0.035,0.035,0.04);
      setAxisTitleOffset(hchPwLDS[i],1.6,1.6,1.4);
      setAxisCenterTitle(hchPwLDS[i],true,true,true);
    }
  }
  /*@@
  if(hchPwLDS[0]->Integral(-1,-1,-1,-1)!=0)
  cpwchDS[0]->Print(Form("%s%sNormal.pdf",DirResultGPw,"hchPwLego"));
  if(hchPwLDS[1]->Integral(-1,-1,-1,-1)!=0)
  cpwchDS[1]->Print(Form("%s%sNormal.pdf",DirResultGPw,"hchPwSELego"));
  */
  cpwchDS[0]->Close();
  cpwchDS[1]->Close();


  cpwrt->cd();
  cpwrt->SetPhi(-30);
  cpwrt->SetTheta(70);
  setTitle(hchRt,"unixtime","ChGID (board*16+channel)","Data tacking Rate(Hz)");
  FastStyleStats(0.85,0.44);
  
  t->Draw(Form("(channel+(board-1)*16):unixtime>>hchRt"),"","colz");
  
  cpwrt->Print(Form("%s%sNormal.pdf",DirResultGRt,"hchRtColz"));
  FastStyleStats(0.95,0.95);
  TH2F *hchRtL = (TH2F*) hchRt->Clone();
  hchRtL->Draw("Lego2");
  setAxisLabelSize(hchRtL,0.03,0.03,0.025);
  setAxisTitleSize(hchRtL,0.035,0.035,0.040);
  setAxisTitleOffset(hchRtL,1.6,1.6,1.4);
  setAxisCenterTitle(hchRtL,true,true,true);

  cpwrt->Print(Form("%s%sNormal.pdf",DirResultGRt,"hchRtLego"));
  cpwrt->Close();
  FastStyleStats(0.95,0.6);
  for(int ibd=0;ibd<NumBD;ibd++){


    char cpwname[30]={};
    char crtname[30]={};
    sprintf(cpwname ,Form("cpwB%02.f",BD[ibd]*1.));
    sprintf(crtname ,Form("crtB%02.f",BD[ibd]*1.));
    cpw[ibd] = new TCanvas(cpwname,"",2160,2160);
    crt[ibd] = new TCanvas(crtname,"",2160,2160);
    cpwAll[ibd] = new TCanvas(Form("%sAll",cpwname),"",2160,2160);
    crtAll[ibd] = new TCanvas(Form("%sAll",crtname),"",2160,2160);
    cpw[ibd]->Divide(4,4);
    crt[ibd]->Divide(4,4);



    //cpw[ibd]->cd();

    for(int ich=0;ich<NumCh;ich++){

      char hpwname[30]={},hrtname[30]={};

      sprintf(hpwname  ,"hPwB%02.fCh%02.f(GID=%03.f)",BD[ibd]*1.,ich*1.,1.*((BD[ibd]-1)*NumCh+ich));
      sprintf(hrtname  ,"hRtB%02.fCh%02.f(GID=%03.f)",BD[ibd]*1.,ich*1.,1.*((BD[ibd]-1)*NumCh+ich));

      // cout<<"Progress: General Ch ID: "<<ich+ibd*NumCh<<endl;
      hPw[ich+ibd*NumCh]  = new TH1F(Form("%sHI",hpwname),"",binpw,0,binpw);
      hPwSE[ich+ibd*NumCh]= new TH1F(Form("%sSE",hpwname),"",binpw,0,binpw);
      hRt[ich+ibd*NumCh]  = new TH1F(hrtname,hrtname,binrt,Starttime,Endtime);

      for(int ibinpw=1; ibinpw<binpw+1;ibinpw++){
        float TmpPwV0= hchPwDS[0]->GetBinContent(ibinpw,(BD[ibd]-1)*NumCh+ich+1),
              TmpPwV1= hchPwDS[1]->GetBinContent(ibinpw,(BD[ibd]-1)*NumCh+ich+1);
        hPw[ich+ibd*NumCh]->SetBinContent(ibinpw,TmpPwV0);
        hPwSE[ich+ibd*NumCh]->SetBinContent(ibinpw,TmpPwV1);
      }
      /*
            for(int ibinrt=1; ibinrt<binrt+1;ibinrt++){
        RtValue[1] = 1.0*(hchRt->GetBinContent(ibinrt,(BD[ibd]-1)*NumCh+ich+1));
        hRt[ich+ibd*NumCh]->SetBinContent(ibinrt,RtValue[1]);
        double RtRate = (RtValue[1]/RtValue[0]);
        if(ibinrt>20){
          if     ( RtRate<1.E+8&&RtRate>1.165) outRt<<BD[ibd]<<"\t"<<ich<<"\t"<<ibinrt<<"\t"<<RtValue[1]/RtValue[0]<<endl;
          else if(RtValue[1]/RtValue[0]<0.835) outRt<<BD[ibd]<<"\t"<<ich<<"\t"<<ibinrt<<"\t"<<RtValue[1]/RtValue[0]<<endl;
        }

        RtValue[0] = RtValue[1];
      }
      
      */
      
      TLegend *LChpw  = new TLegend(0.4,0.45,0.93,0.9);
      setLegendDefault(LChpw);
      TLegend *LChrt  = new TLegend(0.4,0.75,0.93,0.9);
      setLegendDefault(LChrt);
      LChpw->AddEntry(hPw[ich+ibd*NumCh]  ,Form("Ch-%02.f All  event",ich*1.),"l");
      LChpw->AddEntry(hPw[ich+ibd*NumCh]  ,Form("Hit number: %d",PWNTmp[0][ibd][ich]),"h");
      LChpw->AddEntry(hPwSE[ich+ibd*NumCh],Form("Ch-%02.f Muon event",ich*1.),"F");
      LChpw->AddEntry(hPwSE[ich+ibd*NumCh],Form("Hit number: %d",PWNTmp[1][ibd][ich]),"h");

      LChrt->AddEntry(hRt[ich+ibd*NumCh]  ,Form("Ch-%02.f All  hits ",ich*1.),"l");
      cpw[ibd]->cd(2*(ich%4)+13-ich);//@@ have to chance to be table 
      hPw[ich+ibd*NumCh]  ->Draw("hist");

      hPw[ich+ibd*NumCh]  ->SetStats(0);

      hPwSE[ich+ibd*NumCh]->Draw("histsame");

      LChpw->Draw("same");


      cpwAll[ibd]->cd();
      if (ich==0){
        hPw[ich+ibd*NumCh]->Draw("hist");
        LChpw->Draw("same");
      }
      else       hPw[ich+ibd*NumCh]->Draw("whistsame");
      hPwSE[ich+ibd*NumCh]->Draw("histsame");
      // cout<<"load data hrt"<<endl;
      
      double RtValue[2] = {};
      RtValue[0] = 1.0*(hchRt->GetBinContent(1,(BD[ibd]-1)*NumCh+ich+1));
      for(int ibinrt=1; ibinrt<binrt+1;ibinrt++){
        RtValue[1] = 1.0*(hchRt->GetBinContent(ibinrt,(BD[ibd]-1)*NumCh+ich+1));
        hRt[ich+ibd*NumCh]->SetBinContent(ibinrt,RtValue[1]);
        double RtRate = (RtValue[1]/RtValue[0]);
        if(ibinrt>20){
          if     ( RtRate<1.E+8&&RtRate>1.165) outRt<<BD[ibd]<<"\t"<<ich<<"\t"<<ibinrt<<"\t"<<RtValue[1]/RtValue[0]<<endl;
          else if(RtValue[1]/RtValue[0]<0.835) outRt<<BD[ibd]<<"\t"<<ich<<"\t"<<ibinrt<<"\t"<<RtValue[1]/RtValue[0]<<endl;
        }

        RtValue[0] = RtValue[1];
      }
      // cout<<"draw data hrt"<<endl;
      crt[ibd]->cd(2*(ich%4)+13-ich);//@@ have to chance to be table 
      hRt[ich+ibd*NumCh]->SetStats(0);
      hRt[ich+ibd*NumCh]->Scale(1/th2fbinwidth);
      hRt[ich+ibd*NumCh]->Draw("hist");
      
      
      LChrt->Draw("same");
      crtAll[ibd]->cd();

      if (ich==0){
        hRt[ich+ibd*NumCh]->Draw("hist");
        LChrt->Draw();
      }else       hRt[ich+ibd*NumCh]->Draw("histsame");
      
      setTitle(hPw[ich+ibd*NumCh]  ,hpwname,"pwidth/pwidthScaleFactor(100ns)","Number of Hit");
      setTitle(hPwSE[ich+ibd*NumCh],hpwname,"pwidth/pwidthScaleFactor(100ns)","Number of Hit");
      hPw[ich+ibd*NumCh]->GetXaxis()->SetTitleOffset(1.4);
      hPw[ich+ibd*NumCh]->GetYaxis()->SetTitleOffset(1.5);
      hPw[ich+ibd*NumCh]->GetZaxis()->SetTitleOffset(1.4);
      hPwSE[ich+ibd*NumCh]->GetXaxis()->SetTitleOffset(1.4);
      hPwSE[ich+ibd*NumCh]->GetYaxis()->SetTitleOffset(1.5);
      hPwSE[ich+ibd*NumCh]->GetZaxis()->SetTitleOffset(1.4);
      if(normalizeMode){
        hPw[ich+ibd*NumCh]  ->GetYaxis()->SetTitle("Ratio");
        hPwSE[ich+ibd*NumCh]->GetYaxis()->SetTitle("Ratio");
      }
      setTitle(hRt[ich+ibd*NumCh],hrtname,"unixtime(s) half hour/bin","Rate of channel(Hz)");


      hRt[ich+ibd*NumCh]  ->SetTitle(hrtname);
      hRt[ich+ibd*NumCh]  ->GetXaxis()->SetTitle("unixtime(s) half hour/bin");
      hRt[ich+ibd*NumCh]  ->GetYaxis()->SetTitle("Rate of channel(Hz)");
      setAxisRangeUser(hPw[ich+ibd*NumCh],0,binpw,0,0.2);
      hRt[ich+ibd*NumCh]  ->GetYaxis()->SetRangeUser(0,1.2);
      hPw[ich+ibd*NumCh]  ->SetLineWidth(0.5);
      hPwSE[ich+ibd*NumCh]->SetLineWidth(0.5);
      hRt[ich+ibd*NumCh]  ->SetLineWidth(0.5);

      hPw[ich+ibd*NumCh]  ->SetLineColor(2);

      hPwSE[ich+ibd*NumCh]->SetLineColorAlpha(3,0.1);
      hPwSE[ich+ibd*NumCh]->SetFillColorAlpha(3,0.2);
      hRt[ich+ibd*NumCh]  ->SetLineColor(2);

      // cout<<"finish draw hrt"<<endl;
      
      

    }

    char namessss[200]={};
    sprintf(namessss,"%s%sNormal",DirResultGPw,cpwname);
    DrawPdfPng(cpw[ibd],namessss);
    char namessss1[200]={};
    sprintf(namessss1,"%s%sNormal",DirResultGRt,crtname);


    DrawPdfPng(crt[ibd],namessss1);
    
    PDFCanPage(pdfcpw,cpw[ibd],Form("MTB-%02.f",BD[ibd]*1.),ibd+1,NumBD);
    PDFCanPage(pdfcrt,crt[ibd],Form("MTB-%02.f",BD[ibd]*1.),ibd+1,NumBD);
    PDFCanPage(pdfcpwA,cpwAll[ibd],Form("MTB-%02.f",BD[ibd]*1.),ibd+1,NumBD);
    PDFCanPage(pdfcrtA,crtAll[ibd],Form("MTB-%02.f",BD[ibd]*1.),ibd+1,NumBD);
    
    cpw[ibd]->Close();
    crt[ibd]->Close();
    cpwAll[ibd]->Close();
    crtAll[ibd]->Close();
  }
  delete *cpw, *crt, *cpwAll, *crtAll;

  optime.Stop();
  optime.Print();
}





void PwidthAnaV2P(const bool*OperMode, const int itcntgap=28) {
  defstyle();
  bool testMode        =true;  testMode         = OperMode[0];
  bool normalizeMode   =true;  normalizeMode    = OperMode[1];
  bool NegHiRatChMode  =true;  NegHiRatChMode   = OperMode[2];
  bool lowstatisticMode=false; lowstatisticMode = OperMode[3];
  bool rootFileMode    =true;  rootFileMode     = OperMode[4];
  bool SETightMode     =true; 
  if(testMode){
    cout<<"Bool testMode is true, ending to execute PwidthV2P!"<<endl;
    return;
  }
  map<int, scintillator> MScints = LoadODectTable();
  
  vector<string> vRootFiles = RawRootList(DirOperate);
  // for(int i=0;i<100;i++) cout<<vRootFiles[i]<<endl;
  
  int binrt = 0;
  int Starttime = unixtimei, Endtime = unixtimef;

  TFile *SelectRfile;
  //cout<<"000000"<<endl;
  char RFNStr[200];
  sprintf(RFNStr,"%d_%s_nHTH%dL%d.root",eventGapTcnt[itcntgap],TimeStr,TriggerS,TriggerL);
  cout<<Form("%sETracksGT%s",DirOperate,RFNStr)<<endl;

  SelectRfile = TFile::Open(Form("%sETracksGT%s",DirOperate,RFNStr));
  //else SelectRfile = TFile::Open(Form("%s%sEventsGT%s",DirOperate,TestMarker,RFNStr));
  TTree *tR  = (TTree*) SelectRfile ->Get("t");
  cout<<"opening....."<<SelectRfile->GetName()<<endl;
  

  char DirResultGPw[150]={""}, DirResultBDD[150]={""}, DirResultLYD[150]={""};//save path graph of Pwidth Rate

  sprintf(DirResultGPw,"%sBoardGPwNor/",DirResult);
  if (system(Form("mkdir -p %s",DirResultGPw))!=0) system(Form("mkdir -p %s",DirResultGPw));
  sprintf(DirResultBDD,"%sBoardDetail/",DirResultGPw);  
  if (system(Form("mkdir -p %s",DirResultBDD))!=0) system(Form("mkdir -p %s",DirResultBDD));
  sprintf(DirResultLYD,"%sLayerDetail/",DirResultGPw);  
  if (system(Form("mkdir -p %s",DirResultLYD))!=0) system(Form("mkdir -p %s",DirResultLYD));
  
  TStopwatch optime;
  optime.Start();
  binrt = (Endtime-Starttime)*1.0/th2fbinwidth;
  cout<<"binrt number:   "<<binrt<<endl;
  
  
  gStyle->SetLineScalePS(0.3);
  
  TCanvas *cpwchSE = new TCanvas("cpwchSE","",4320,4320);
  TCanvas *cpwchTS = new TCanvas("cpwchTS","",4320,4320);
  TCanvas *cpw[NumBD]={},*cpwLY[NumLY]={};
  TCanvas *TmpC[NumBD*NumCh]={};
  // TCanvas *cpwAllDX= new TCanvas("cpwAllDX","",2160,2160*NumLY);
  // TCanvas *cpwAllDY= new TCanvas("cpwAllDY","",2160*NumLY,2160);
  // cpwAllDX->Divide(1    ,NumLY);
  // cpwAllDY->Divide(NumLY,1    );
  
  TPDF *pdfcpw = new TPDF(Form("%sPwidthBDcpNor.pdf",DirResultGPw));

  TPDF *pdfcpwA = new TPDF(Form("%sPwidthDetArrNor.pdf",DirResultGPw));

  TH1F *hPwHI[NumBD*NumCh]={},*hPwES[NumBD*NumCh]={},*hPwTS[NumBD*NumCh]={};

  // TF1  *fPw[64]={},*fPwSE[64]={};
  TH2F *hchPwTS = new TH2F("hchPwTS","hchPwTS",binpw,0,binpw,NumBD*NumCh,0,NumBD*NumCh);//ch64, pw: 0~180
  TH2F *hchPwDS[1] = {hchPwTS};//ch64, pw: 0~180

  cpwchSE->SetPhi(-30);
  cpwchSE->SetTheta(70);
  FastStyleStats(.9,.9);
  
  cpwchSE->cd();
  char PWDDataName[3][200]={};
  sprintf(PWDDataName[0],"%sLoose_outPwHIData.txt"  ,DirOperate);
  sprintf(PWDDataName[1],"%sLoose_outPwSEData.txt"  ,DirOperate);
  sprintf(PWDDataName[2],"%soutPwTSData.txt"  ,DirOperate);
  if(rootFileMode){
    tR->Draw("(channel+(board-1)*16):pwidth/pwidthScaleFactor>>hchPwTS","","colz");
    for(int i=0;i<1;i++){
      ofstream outPwData(PWDDataName[2]);
      
      if(normalizeMode){
        for(int ibd=0;ibd<NumBD;ibd++){
          for(int ich=0;ich<NumCh;ich++){

            int Tmp_CHGID = (BD[ibd]-1)*16+ich;
            // cout<<"Doing the normalization: ChGID: " <<Tmp_CHGID<<endl;
            double ChIntegralNum  = hchPwTS->Integral(-1,-1,Tmp_CHGID+1,Tmp_CHGID+1);
            outPwData<<BD[ibd]<<"\t"<<ich<<"\t"<<int(ChIntegralNum);

            for(int ibinpw=0;ibinpw<binpw;ibinpw++){
              double ChOriginNum = hchPwTS->Integral(ibinpw+1,ibinpw+1,Tmp_CHGID+1,Tmp_CHGID+1);
              double interData = 0;
              if(ChOriginNum==0) interData=0;
              else interData = ChOriginNum/ChIntegralNum;
              hchPwTS->SetBinContent(ibinpw+1,Tmp_CHGID+1,interData);
              outPwData<<"\t"<<interData;
              //cout<<ibinpw<<"    "<<Tmp_CHGID<<"    "<<ChOriginNum<<"    "<<interData<<endl;

            } 
            outPwData<<"\n";
            //cout<<ich+ibd*16<<"      "<<hchPwTS->Integral(-1,-1,Tmp_CHGID+1+1,Tmp_CHGID+1+1)<<endl;
          }
        }
      }
      outPwData.close();
      setTitle(hchPwTS,"Pwidth/pwidthScaleFactor(100ns)","ChGID (board*16+channel)","Number of Hit");
      hchPwTS->GetXaxis()->SetTitleOffset(1.4);
      hchPwTS->GetYaxis()->SetTitleOffset(1.4);
      hchPwTS->GetZaxis()->SetTitleOffset(1.4);
      hchPwTS->GetXaxis()->SetLabelSize(0.025);
      hchPwTS->GetYaxis()->SetLabelSize(0.025);
      
      hchPwTS->GetZaxis()->SetLabelSize(0.025);
      if(normalizeMode) hchPwTS->GetZaxis()->SetTitle("Ratio");
    }
  }
  
  
  DrawPdfPng(cpwchSE,Form("%shchPwTSEColzNormal",DirResultGPw));

  double PWDTmp[3][NumBD+1][NumCh][binpw];//[SE | Hit][bd][ch][pwd]
  int    PWNTmp[3][NumBD+1][NumCh];
  
  for(int ic=0;ic<3;ic++){
    ifstream in(PWDDataName[ic]);
    cout<<"reading:  "<<PWDDataName[ic]<<endl;
    for(int i0=0,ich=0,ibd=0,numtmp;i0<NumCh*NumBD;i0++){
      in>>ibd>>ich;
      ibd = BDcheck(ibd);//@@ibd  bid
      in>>numtmp;
      PWNTmp[ic][ibd][ich]=numtmp;
      
      //cout<<ibd<<"\t"<<ich<<"\t"<<PWNTmp[0][ibd][ich]<<"\n";
      for(int ibin=0;ibin<binpw;ibin++){
        double pwtmp;
        in>>pwtmp;
        PWDTmp[ic][ibd][ich][ibin] = pwtmp;
      }
    }
    in.close();
  }


  FastStyleStats(0.95,0.6);
  
  for(int ily=0;ily<NumLY;ily++){
    char cpwname[10]={},cpwtitle[10]={};
    sprintf(cpwname ,Form("Cly%d",ily));
    sprintf(cpwtitle,Form("Layer %d(iZ = %d)",ily,ily));
    cpwLY[ily] = new TCanvas(cpwname,cpwtitle,2160*NumBX,2160*NumBY);
    cpwLY[ily] -> Divide(NumBX*NumnX,NumBY*NumnY);
  }
  
  for(int ibd=0;ibd<NumBD;ibd++){
    char cpwname[30]={};
    sprintf(cpwname ,Form("cpwB%02.f",BD[ibd]*1.));
    cpw[ibd] = new TCanvas(cpwname,"",2160,2160);
    cpw[ibd] ->Divide(4,4);

    for(int ich=0;ich<NumCh;ich++){
      int GID = BCIDCovGID(BD[ibd],ich);
      scintillator TmpMSc = MScints[GID];
      
      char hpwname [30]={};
      char hpwtitle[30]={};
      sprintf(hpwname  ,"hPwB%02.fCh%02.f",TmpMSc.boardID*1.,TmpMSc.channelID*1.);
      sprintf(hpwtitle ,"MTB%02.f - Ch%02.f (GID=%03.f) "
                       ,TmpMSc.boardID*1.,TmpMSc.channelID*1.,TmpMSc.GeneralID*1.);
      
      hPwHI[GID]= new TH1F(Form("%sHI",hpwname),"",binpw,0,binpw);
      hPwES[GID]= new TH1F(Form("%sES",hpwname),"",binpw,0,binpw);
      hPwTS[GID]= new TH1F(Form("%sTS",hpwname),"",binpw,0,binpw);
      
      TH1F *hPwAll[3]={hPwHI[GID],hPwES[GID],hPwTS[GID]};
      
      for(int ic=0;ic<3;ic++){
        for(int ibinpw=1; ibinpw<binpw+1;ibinpw++){
          hPwAll[ic]->SetBinContent(ibinpw,PWDTmp[ic][ibd][ich][ibinpw]);
        }
        setTitle(hPwAll[ic],"","pwidth/pwidthScaleFactor(100ns)","Number of Hit");
        hPwAll[ic]->GetXaxis()->SetTitleOffset(1.4);
        hPwAll[ic]->GetYaxis()->SetTitleOffset(1.5);
        hPwAll[ic]->GetZaxis()->SetTitleOffset(1.4);
        hPwAll[ic]->GetYaxis()->SetLabelSize(0.03);
        hPwAll[ic]->SetLineWidth(0.5);
        hPwAll[ic]->SetStats(0);
      
        if(normalizeMode){
          hPwAll[ic]->GetYaxis()->SetTitle("Ratio");
          setAxisRangeUser(hPwAll[ic],0,binpw*0.6,0,0.2);
        }
      }
      
      hPwAll[0]->SetLineColor(2);
      hPwAll[1]->SetLineColorAlpha(3,0.1);
      hPwAll[1]->SetFillColorAlpha(3,0.2);
      hPwAll[2]->SetLineColorAlpha(4,0.1);
      hPwAll[2]->SetFillColorAlpha(4,0.2);

      
      TLegend *LChpw  = new TLegend(0.5,0.3,0.95,0.9);
      setLegendDefault(LChpw);
      LChpw->AddEntry(hPwHI[ich+ibd*NumCh],Form("#scale[2.0]{MTB%02.f Ch-%02.f}",TmpMSc.boardID*1.,TmpMSc.channelID*1.),"h");
      LChpw->AddEntry(hPwHI[ich+ibd*NumCh],Form("   #scale[1.5]{iXYZ=(%d,%d,%d)}",TmpMSc.iX,TmpMSc.iY,TmpMSc.iZ),"h");
      LChpw->AddEntry(hPwHI[ich+ibd*NumCh],Form("No Select Hits" ),"l");
      LChpw->AddEntry(hPwHI[ich+ibd*NumCh],Form(" Hit number: %d",PWNTmp[0][ibd][ich]),"");
      LChpw->AddEntry(hPwES[ich+ibd*NumCh],Form("Easy ID Select" ),"F");
      LChpw->AddEntry(hPwES[ich+ibd*NumCh],Form(" Hit number: %d",PWNTmp[1][ibd][ich]),"");
      LChpw->AddEntry(hPwTS[ich+ibd*NumCh],Form("Tracking result"),"F");
      LChpw->AddEntry(hPwTS[ich+ibd*NumCh],Form(" Hit number: %d",PWNTmp[2][ibd][ich]),"");

      

      
      TmpC[GID] = new TCanvas(Form("Tmpc%03.f",GID*1.),"",540,540);
      TmpC[GID]->cd();
      hPwHI[ich+ibd*NumCh]  ->Draw("hist");
      hPwES[ich+ibd*NumCh]->Draw("histsame");
      hPwTS[ich+ibd*NumCh]->Draw("histsame");
      LChpw->Draw("same");
      
      cpw[ibd]->cd(2*(ich%4)+13-ich);//@@ have to chance to be table 
      TmpC[GID]->DrawClonePad();
      //delete TmpC;
      
      cpwLY[NumLY-TmpMSc.iZ-1]->cd(57-8*TmpMSc.iY+TmpMSc.iX);
      TmpC[GID]->DrawClonePad();
      
      
      
      
      
    }


    char namessss[200]={};
    cout<<DirResultGPw<<endl;
    sprintf(namessss,"%s%sNormal",DirResultBDD,cpwname);
    
    DrawPdfPng(cpw[ibd],namessss);
    
    
    PDFCanPage(pdfcpw,cpw[ibd],Form("MTB-%02.f",BD[ibd]*1.),ibd+1,NumBD);
    
    
  }
  
  //delete *cpw,*TmpC;

  
  for(int ily=0;ily<NumLY;ily++){
    cout<<Form("%sPwidthNorLayerChArr%d",DirResultLYD,ily)<<endl;
    
    cpwLY[ily]->Print(Form("%sPwidthNorLayerChArr%d.pdf",DirResultLYD,ily));
    
    PDFCanPage(pdfcpwA,cpwLY[ily],Form("Layer: %d ( iZ = %d )",NumLY-1-ily,NumLY-1-ily),1+ily,NumLY);
    
  }
  
  
  gStyle->SetPadRightMargin(0.14);
  gStyle->SetPadLeftMargin(0.10);
  gStyle->SetPadTopMargin(0.12);
  gStyle->SetPadBottomMargin(0.12);

  TPDF  *pdfNES = new TPDF(Form("%sHitNumEasySelect.pdf",DirResultGPw));
  TPDF  *pdfNTS = new TPDF(Form("%sHitNumTrackSelect.pdf",DirResultGPw));

  TCanvas *cNES = new TCanvas("cNES","",2160,2160);
  TCanvas *cNTS = new TCanvas("cNTS","",2160,2160);
  TCanvas *cNESA = new TCanvas("cNESA","",2160*NumLY,2160);
  TCanvas *cNTSA = new TCanvas("cNTSA","",2160*NumLY,2160);
  PDFCanPage(pdfNES,cNES,"Title",1,NumLY);
  PDFCanPage(pdfNTS,cNES,"Title",1,NumLY);

  cNESA->Divide(4,1);cNTSA->Divide(4,1);
  TH2F *hNES[NumLY],*hNTS[NumLY];
  
  for(int ily=0; ily<NumLY;ily++){

    hNES[ily] = new TH2F(); hNTS[ily] = new TH2F();
    SetNXMSciArr(hNES[ily],NumBX*NumnX,NumBY*NumnY,Form("hNES%d",ily),Form("Number of Hits for Easy Select on Layer%d (iZ=%d)",ily,ily));
    SetNXMSciArr(hNTS[ily],NumBX*NumnX,NumBY*NumnY,Form("hNTS%d",ily),Form("Number of Hits for Track Select on Layer%d (iZ=%d)",ily,ily));

    for(int ibd = 0; ibd<NumBD/NumLY;ibd++){
      for(int ich=0; ich<NumCh;ich++){
        // cout<<ily<<"  "<<ily*NumBD/NumLY+ibd<<"  "<<ich<<"\n";
        int Tmp_Num[2] = {PWNTmp[1][ily*NumBD/NumLY+ibd][ich],PWNTmp[2][ily*NumBD/NumLY+ibd][ich]};

        int GID = BCIDCovGID(BD[ily*NumBD/NumLY+ibd],ich);
        scintillator TmpMSc = MScints[GID];

        hNES[ily]->Fill(TmpMSc.iX,TmpMSc.iY,Tmp_Num[0]);
        hNTS[ily]->Fill(TmpMSc.iX,TmpMSc.iY,Tmp_Num[1]);

        // cout<<TmpMSc.iX<<"   "<<TmpMSc.iY<<"   "<<Tmp_Num[0]<<"   "<<Tmp_Num[1]<<endl;
      }
    }

  }

  int maxCon[2]={},minCon[2]={1.E+8,1.E+8};
  for(int ily=0; ily<NumLY;ily++){
    TH2F *hNS[2] = {hNES[ily],hNTS[ily]};
    for(int ic=0;ic<2;ic++){
      int TmpCon = hNS[ic]->GetMaximum();
      if(TmpCon>maxCon[ic])            maxCon[ic] = TmpCon;
      
      TmpCon = hNS[ic]->GetMinimum();
      if(TmpCon<minCon[ic]&&TmpCon!=0) minCon[ic] = TmpCon;
      hNS[ic]->SetStats(0);
      setTitle(hNS[ic],"iX","iY","Hits Number");

      hNS[ic]->GetZaxis()->SetTitleOffset(1.7);
      hNS[ic]->GetZaxis()->SetLabelSize(0.03);
      hNS[ic]->GetZaxis()->SetTitleSize(0.03);
      hNS[ic]->SetMarkerSize(1.2);
      hNS[ic]->SetMarkerColor(1);
    }
  }
  
  float maxSetUserBS = MaxSetUserRatio(maxCon[0]*1.,0.9,2);
  float maxSetUserTS = MaxSetUserRatio(maxCon[1]*1.,0.9,2);
  float minSetUserBS = MinSetUserRatio(minCon[0]*1.,1.2,2);
  float minSetUserTS = MinSetUserRatio(minCon[1]*1.,1.2,2);
  
  
  for(int ily=0; ily<NumLY;ily++){
    cNES->cd();
    hNES[ily]->GetZaxis()->SetRangeUser(minCon[0],maxCon[0]);
    hNES[ily]->Draw("ColzTEXT");
    
    cNTS->cd();
    hNTS[ily]->GetZaxis()->SetRangeUser(minCon[1],maxCon[1]);
    hNTS[ily]->Draw("ColzTEXT");
    
    DrawPdfPng(cNES,Form("%sHitNumEasySelectL%d.pdf",DirResultLYD,ily));
    DrawPdfPng(cNTS,Form("%sHitNumTrackSelectL%d.pdf",DirResultLYD,ily));
    PDFCanPage(pdfNES,cNES,Form("Layer: %d ( iZ = %d )",NumLY-1-ily,NumLY-1-ily),1+ily,NumLY);
    PDFCanPage(pdfNTS,cNTS,Form("Layer: %d ( iZ = %d )",NumLY-1-ily,NumLY-1-ily),1+ily,NumLY);
    cNESA->cd(ily+1);
    cNES->DrawClonePad();
    cNTSA->cd(ily+1);
    cNTS->DrawClonePad();
    
  }
  cNESA->Print(Form("%sHitNumEasySelect.png",DirResultGPw));
  cNTSA->Print(Form("%sHitNumTrackSelect.png",DirResultGPw));
  TCanvas *cALL = new TCanvas("cALL","",2160*NumLY,2160*2);
  cALL->Divide(1,2);
  cALL->cd(1);
  cNESA->DrawClonePad();
  cALL->cd(2);
  cNTSA->DrawClonePad();
  DrawPdfPng(cALL,Form("%sHitNumAllSelect.png",DirResultGPw));
  
  
  
  optime.Stop();
  optime.Print();
}