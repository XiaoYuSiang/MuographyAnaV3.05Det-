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
#include <TLegendEntry.h>
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
#include <TGraphAsymmErrors.h>
#include "/data4/YuSiang/personalLib/Style/DBMTStyle.h"
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"

using namespace std;
using namespace TMath;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;



void PWidthPeakAna(const bool*OperMode){
  defstyle();
  bool testMode        =true;  testMode         = OperMode[0];
  bool lowstatisticMode=false; lowstatisticMode = OperMode[3];
  bool SETightMode     =true;  SETightMode      = OperMode[5];
  
  int Dtheta =0;
  if(lowstatisticMode) Dtheta = 85;
  TStopwatch optime;
  optime.Start();
  int colorArr[16] = {};
  for(int i=0;i<16;i++){
    if(i<12) colorArr[i]=205+i;
    else if(i>11) colorArr[i]=209+i;
  }
  char TestMarker[15]={};
  int NumBDt = NumBD, NumCht = NumCh;
  if (testMode){
    sprintf(TestMarker,"TST_");
    // NumBDt = 1;
    // NumCht = 1;
  }
  if(SETightMode==false&&testMode) sprintf(TestMarker,"TST_Loose_");
  else if(SETightMode==false) sprintf(TestMarker,"Loose_");

  char DirResultGPw[150]={""};//save path graph of Pwidth 
  sprintf(DirResultGPw,"%s%sBoardGPwNor/",DirResult,TestMarker);
  if (system(Form("mkdir -p %s",DirResultGPw))!=0) system(Form("mkdir -p %s",DirResultGPw));
  TCanvas *cpw[4]={},*cpwAll[4]={},*cIntpw[4]={},*cEVMPV[5]={};
  TCanvas *c= new TCanvas("QQ","QQ",500,500);
  TH1F *hPw[64]={},*hPwSE[64]={};
  TH1F *hIntPw[64]={},*hIntPwSE[64]={};
  TH1F *SNR[64]={};
  TF1  *fPw[64]={},*fPwSE[64]={};
  TPDF *pdfcpw   = new TPDF(Form("%sPwidthCompareNormalize.pdf",DirResultGPw));
  TPDF *pdfcpwA  = new TPDF(Form("%sPwidthACompareNormalize.pdf",DirResultGPw));
  TPDF *pdfcSNRpw= new TPDF(Form("%sIntPwidthCompareNormalize.pdf",DirResultGPw));
  TPDF *pdfcEVMPV= new TPDF(Form("%sEffVsFitMPV1.pdf",DirResultGPw));
  PDFCanPage(pdfcpw    ,c ,Form("Title"),1,NumBDt);
  PDFCanPage(pdfcpwA   ,c ,Form("Title"),1,NumBDt);
  PDFCanPage(pdfcSNRpw ,c ,Form("Title"),1,NumBDt);
  PDFCanPage(pdfcEVMPV ,c ,Form("Title"),1,NumBDt+1);

  double PWDTmp[2][4][16][90] ={};//[SE | Hit][bd][ch][pwd]
  double EffTmp[3][4][16] ={};//[Y|El|Eh][bd][ch]
  double PWDFit[3][4][16] ={};//[Y|El|Eh][bd][ch]
  char PWDDataName[3][200]={};
  sprintf(PWDDataName[0],"%s%soutPwHIData.txt"  ,DirOperate,TestMarker);
  sprintf(PWDDataName[1],"%s%soutPwSEData.txt"  ,DirOperate,TestMarker);
  char PWDFitResul[3][200]={};
  sprintf(PWDFitResul[0],"%s%soutPwHIfit.txt"  ,DirOperate,TestMarker);
  sprintf(PWDFitResul[1],"%s%soutPwSEfit.txt"  ,DirOperate,TestMarker);
  sprintf(PWDFitResul[2],"%s%sFitMPV1andEff.csv"  ,DirOperate,TestMarker);
  ofstream outFH(PWDFitResul[0]);
  ofstream outFS(PWDFitResul[1]);
  ofstream outThanEff(PWDFitResul[2]);

  for(int ic=0;ic<2;ic++){
    ifstream in(PWDDataName[ic]);
    for(int i0=0,ich=0,ibd=0;i0<NumCht*NumBDt;i0++){
      in>>ibd>>ich;
      ibd = BDCheck(ibd);
      for(int ibin=0;ibin<binpw;ibin++){
        double pwtmp=0;
        in>>pwtmp;
        PWDTmp[ic][ibd][ich][ibin] = pwtmp;
      }
    }
    in.close();
  }

  ifstream inEff(Form("%sCheff.csv",DirOperate));//@@
  
  for(int i0=0,ich=0,ibd=0;i0<NumCht*NumBDt;i0++){
    double x=0,y=0,eyl=0,eyh=0;
    inEff>>ibd>>x>>y>>eyl>>eyh;
    ich = x/1;
    EffTmp[0][BDCheck(ibd)][ich] = y;
    EffTmp[1][BDCheck(ibd)][ich] = eyl;
    EffTmp[2][BDCheck(ibd)][ich] = eyh;
    cout<<ibd<<"\t"<<ich<<"\t"<<EffTmp[0][BDCheck(ibd)][ich]<<"\t"<<EffTmp[1][BDCheck(ibd)][ich]<<"\t"<< EffTmp[2][BDCheck(ibd)][ich]<<"\n";
  }
  
  
  inEff.close();
  TGraphAsymmErrors *EFFErr[4];
  
  //FastStyleStats(0.95,0.95);

  double MPVs[64][3]={};
  TLegend *LEFFErr[4];
  
  for(int bd=0;bd<NumBDt;bd++){
    //cout<<DirResultGPwaRt<<"      A         "<<endl;
    char cpwname[30]={};
    char cIntpwname[40]={};
    char cEVMPVname[40]={};
    sprintf(cpwname    ,Form("cpwB%d",BD[bd]));
    sprintf(cIntpwname ,Form("cIntpwB%d",BD[bd]));
    sprintf(cEVMPVname ,Form("cEVMPV%d",BD[bd]));
    cIntpw[bd] = new TCanvas(cIntpwname  ,"",2160,2160);
    cpw[bd]    = new TCanvas(cpwname     ,"",2160,2160);
    cEVMPV[bd] = new TCanvas(cEVMPVname  ,"", 540, 540);

    cpwAll[bd] = new TCanvas(Form("%sAll",cpwname),"",2160,2160);
    cpw[bd]->Divide(4,4);
    cIntpw[bd]->Divide(4,4);
    TLegend *LChpw  = new TLegend(0.4,0.60,0.93,0.9);
    setLegendDefault(LChpw,4);
    
    
    for(int ch=0;ch<NumCht;ch++){
      //cout<<DirResultGPwaRt<<"      B         "<<endl;
      char hpwname[30]={};
      char INTpwname[40]={},INTpwsename[40]={};
      char fpwname[30]={},fpwsename[30]={};
      char SNRname[30]={};
      sprintf(hpwname  ,"hPwB%dCh%d",BD[bd],ch);
      sprintf(INTpwname  ,"hIntPwB%dCh%d",BD[bd],ch);
      sprintf(INTpwsename,"hIntPwseB%dCh%d",BD[bd],ch);
      sprintf(fpwname  ,"fPwB%dCh%d",BD[bd],ch);
      sprintf(fpwsename,"fpwseB%dCh%d",BD[bd],ch);
      sprintf(SNRname,"SNR%dCh%d",BD[bd],ch);
       
      ////cout<<hpwname<<"   "<<hpwselectobject<<"   "<<selectcondition<<endl;

      hPw[ch+bd*16]      = new TH1F(hpwname,hpwname,binpw,0,binpw);
      hPwSE[ch+bd*16]    = new TH1F(hpwname,hpwname,binpw,0,binpw);
      hIntPw[ch+bd*16]   = new TH1F(INTpwname  ,INTpwname  ,binpw,0,binpw);
      hIntPwSE[ch+bd*16] = new TH1F(INTpwsename,INTpwsename,binpw,0,binpw);
      SNR[ch+bd*16]      = new TH1F(SNRname    ,SNRname    ,binpw,0,binpw);
      //fPw[ch+bd*16]   = new TF1(fpwname  ,pwidthHitfit,0,90,7);
      int hitcase = 1;
      if(lowstatisticMode) hitcase = 2;
      int casePnum[3] = {3,7,5};
      fPwSE[ch+bd*16] = new TF1(fpwsename,landaufun,0,90,3);
      landaufun(fPwSE[ch+bd*16]);
      if(hitcase==1){
        fPw[ch+bd*16] = new TF1(fpwname,HitFitFun_0   ,0,50,7);
        HitFitFun_0(fPw[ch+bd*16]);
      }
      TF1  *fPwAll[2];
      fPwAll[1]=fPwSE[ch+bd*16];
      TH1F *hPwAll[2]   ={hPw[ch+bd*16],hPwSE[ch+bd*16]};
      TH1F *hIntPwAll[2]={hIntPw[ch+bd*16],hIntPwSE[ch+bd*16]};//@@
      //outPwfit<<bd<<"\t"<<ch;
      for(int ibinpw=0; ibinpw<binpw;ibinpw++){
        hPwAll[0]->SetBinContent(ibinpw+1,PWDTmp[0][bd][ch][ibinpw]);
        hPwAll[1]->SetBinContent(ibinpw+1,PWDTmp[1][bd][ch][ibinpw]);
      }
      // for(int ise=0;ise<3;ise++){
        // if(SETightMode) fPwAll[1]->SetParLimits (ise,SPLminP[hitcase][ise],SPLmaxP[hitcase][ise]);
        // fPwAll[1]->SetParameter(ise,SPSP[hitcase][ise]);
      // }
      // Start SE Fit
      double SEChi2Th = 0.02;
      fPwAll[1]->SetLineWidth(0+1);
      double BestfitSEMPV =0;

      int Chi2MinIndex=0;
      float Chi2tmp=0,chi2Min=100,BestfitSE[3]={};
      for(int iP=0;iP<25;iP++){
        //for(int ise=0;ise<3;ise++) fPwAll[1]->SetParLimits(ise,SPLminP[0][ise],SPLmaxP[0][ise]);
        for(int ise=0;ise<3;ise++) fPwAll[1]->SetParLimits(ise,SPLminP[0][ise],SPLmaxP[0][ise]);
        fPwAll[1]->SetParameters(SPSP[0][0],SPLminP[0][1]+iP,SPSP[0][2]);
        hPwAll[1]->Fit(fPwAll[1],"NQ");
        Chi2tmp=fPwAll[1]->GetChisquare();
        //cout<<Chi2tmp<<endl;
        if(Chi2tmp<chi2Min){
          chi2Min = Chi2tmp;
          Chi2MinIndex = iP;
          for(int ips=0;ips<3;ips++) BestfitSE[ips]=fPwAll[1]->GetParameter(ips);
          //if(chi2Min<=SEChi2Th) break;
        }
      }
      for(int ise=0;ise<3;ise++) fPwAll[1]->SetParLimits(ise,SPLminP[0][ise],SPLmaxP[0][ise]);
      fPwAll[1]->SetParameters(BestfitSE[0],BestfitSE[1],BestfitSE[2]);
      BestfitSEMPV = BestfitSE[1];
      hPwAll[1]->Fit(fPwAll[1],"NQ");
      // End SE Fit
      double chitemp = 10,Partmp[5]={};int itypeN=0;
      double findfeft = 
         hPwAll[0]->GetBinContent(hPwAll[0]->GetMaximumBin())
        -hPwAll[0]->GetBinContent(hPwAll[0]->GetMaximumBin()+5);
      char findPeak[10]={};char caseName[1]={};
      if(lowstatisticMode){
        TF1 *fPwtryA = new TF1(fpwname,HitFitFun_85_Base_B5C03  ,0,90,5);
        TF1 *fPwtryB = new TF1(fpwname,HitFitFun_85_Base_B5C13  ,0,90,5);
        TF1 *fPwtryC = new TF1(fpwname,HitFitFun_85_Base_B1C02  ,0,90,5);
        TF1 *fPwtryD = new TF1(fpwname,HitFitFun_85_Base_B4C03  ,0,90,5);
        HitFitFun_85_Base_B5C03(fPwtryA);
        HitFitFun_85_Base_B5C03(fPwtryB);
        HitFitFun_85_Base_B5C03(fPwtryC);
        HitFitFun_85_Base_B5C03(fPwtryD);
        TF1* fPwtry[4] ={fPwtryA,fPwtryB,fPwtryC,fPwtryD};
        int ispc=0;
        int maxbinIndex = hPwAll[0]->GetMaximumBin(),iP0=0,iPM=15;
        if     (findfeft>0.021) {iP0 =12;iPM=17;sprintf(findPeak,">+4");}
        else if(findfeft>0.015) {iP0 = 8;iPM=12;sprintf(findPeak,">+0");}
        else if(findfeft>0.009) {iP0 = 5;iPM=9; sprintf(findPeak,">-1");}
        else if(findfeft>0.0064){iP0 = 3;iPM= 6;sprintf(findPeak,">-1");}
        else if(findfeft<0.0064){iP0 = 0;iPM= 3;sprintf(findPeak,"<-4");}
        for(int itype = 0 ;itype<4;itype++){
          for(int iscanx=iP0;iscanx<iPM;iscanx++){
            double SPC[5] = {0.374-iscanx*0.012,-7.6+iscanx*0.8,0,1.988+iscanx*0.156,3};
            for(int ise=0;ise<casePnum[2];ise++) {
              // fPwAll[0]->SetParLimits (ise,SPLminP[hitcase][ise],SPLmaxP[hitcase][ise]);
              fPwtry[itype]->SetParameter (ise,SPC[ise]);
            }
            // cout<<SPC[0]<<endl;
            // cout<<SPC[1]<<endl;
            // cout<<SPC[3]<<endl;
             fPwtry[itype]->SetParLimits (0,SPC[0]-0.03,SPC[0]+0.2);
             fPwtry[itype]->SetParLimits (1,SPC[1]-2,SPC[1]+2);
             fPwtry[itype]->SetParLimits (3,2,2.6);
             fPwtry[itype]->SetParLimits (4,1,3);
             /* 
              // fPwtry[1]->SetParLimits (0,0.1,1); */
              // fPwtry[itype]->SetParLimits (1,SPC[1]*5,SPC[1]/5.);
              // fPwtry[itype]->SetParLimits (2,SPC[2]*5,SPC[2]/5.);
            hPwAll[0]->Fit(fPwtry[itype],"NQ");
            fPwtry[itype]->SetLineWidth(0+1);
            if (chitemp>fPwtry[itype]->GetChisquare()){
              chitemp = fPwtry[itype]->GetChisquare();
              itypeN =itype;
              ispc   =iscanx;
              for(int ise=0;ise<casePnum[hitcase];ise++){
                Partmp[ise]=fPwtry[itype]->GetParameter(ise);
                
              }
              if     (itypeN==0) sprintf(caseName,"A");
              else if(itypeN==1) sprintf(caseName,"B");
              else if(itypeN==2) sprintf(caseName,"C");
              else if(itypeN==3) sprintf(caseName,"D");
            }
          }
        }
        for(int ise=0;ise<casePnum[2];ise++) fPwtry[itypeN]->SetParameter(ise,Partmp[ise]);
        double SPC[5] = {0.374-ispc*0.012,-7.6+ispc*0.8,0,1.988+ispc*0.156,3};
        // fPwtry[itypeN]->SetParLimits (0,SPC[0]-0.1,SPC[0]+0.1);
        // fPwtry[itypeN]->SetParLimits (1,SPC[1]-1,SPC[1]+1);
        // fPwtry[itypeN]->SetParLimits (3,SPC[3]-0.2,SPC[3]+0.2);
        fPwtry[itypeN]->SetParLimits (4,1,3);
        hPwAll[0]->Fit(fPwtry[itypeN],"NQ");
        fPw[ch+bd*16] = (TF1*) fPwtry[itypeN]->Clone();
        fPwAll[0] = fPw[ch+bd*16];
      }else{
        for(int ise=0;ise<casePnum[1];ise++) fPw[ch+bd*16]->SetParameter(ise,SPSP[1][ise]);
        hPwAll[0]->Fit(fPw[ch+bd*16],"NQ");
        fPwAll[0] = fPw[ch+bd*16];
      }
      
      cout<<fPwAll[0]->GetParameter(0)<<endl;
      
      MPVs[bd*16+ch][0]=fPwAll[0]->GetParameter(1);
      MPVs[bd*16+ch][1]=fPwAll[0]->GetParameter(5);
      MPVs[bd*16+ch][2]=fPwAll[1]->GetParameter(1);
      int fitParNum[2]={casePnum[hitcase],3};
      
      TLegend *LChs  = new TLegend(0.55,0.8,1,0.9);
      setLegendDefault(LChs);
      LChs->AddEntry(hPw[ch+bd*16]  ,Form("Ch-%d All  event",ch),"l");
      LChs->AddEntry(hPwSE[ch+bd*16],Form("Ch-%d Muon event",ch),"F");
      //LChs->AddEntry(hPw[ch+bd*16]  ,Form("%.6f  %s  %s  %d",findfeft,findPeak,caseName,itypeN),"");
      float LFHpy1m,LFHpy1M,LFHpy2m,LFHpy2M;
      LFHpy1M = 0.8;
      LFHpy1m = LFHpy1M-0.04*(fitParNum[0]+2);
      LFHpy2M = LFHpy1m;
      LFHpy2m = LFHpy2M-0.04*(fitParNum[1]+2);
      TPad *LFitHit = drawTF1LegendsCan(fPwAll[0],Form("Ch-%d Hits fit",ch),0.55,LFHpy1m,1,LFHpy1M,c);
      TPad *LFitSE = drawTF1LegendsCan(fPwAll[1],Form("Ch-%d Muon fit",ch),0.55,LFHpy2m,1,LFHpy2M,c);
      TLegend *LEff  = new TLegend(0.42,LFHpy2m-0.05,1.,LFHpy2m);//@@
      setLegendDefault(LEff);
      TLegendEntry* lEff = LEff->AddEntry("",Form("Eff: %.1f%%-%.1f%%+%.1f%%",EffTmp[0][bd][ch]*100,EffTmp[1][bd][ch]*100,EffTmp[2][bd][ch]*100),"");
      if     (EffTmp[0][bd][ch]>=0.97) lEff->SetTextColor(3);
      else if(EffTmp[0][bd][ch]>=0.95) lEff->SetTextColor(92);
      else                            lEff->SetTextColor(2);
      
      for(int ifit=0;ifit<2;ifit++){
        if(ifit==0)outFH<<BD[bd]<<"\t"<<ch<<"\t"<<fPwAll[ifit]->GetChisquare();
        else       outFS<<BD[bd]<<"\t"<<ch<<"\t"<<fPwAll[ifit]->GetChisquare();
        for(int iL=0;iL<fitParNum[ifit];iL++){
          //outPwfit  <<"\t"<<fPwAll[ifit]->GetParameter(iL);
          if(ifit==0) outFH<<"\t"<<fPwAll[ifit]->GetParameter(iL)<<"\t"<<fPwAll[ifit]->GetParError(iL);
          else        outFS<<"\t"<<fPwAll[ifit]->GetParameter(iL)<<"\t"<<fPwAll[ifit]->GetParError(iL);
        }if(ifit==0) outFH<<endl;
         else        outFS<<endl;
        if(ifit==0) outThanEff<<BD[bd]<<","<<ch<<","<<fPwAll[ifit]->GetParameter(1)<<","<<fPwAll[ifit]->GetParError(1)<<","<<EffTmp[0][bd][ch]<<endl;
        //if(lowstatisticMode&&SETightMode) break;ff
      }
        
      LChpw->AddEntry(hPwSE[ch+bd*16],Form("Ch-%d Muon event",ch),"F");
      LChpw->AddEntry(hPwSE[ch+bd*16],Form("Ch-%d Muon event",ch),"F");
      
      cpw[bd]->cd(2*(ch%4)+13-ch);
      hPw[ch+bd*16]  ->Draw("hist");
      hPw[ch+bd*16]  ->SetStats(0);
      hPwSE[ch+bd*16]->Draw("histsame");
      fPw[ch+bd*16]  ->Draw("lsame");
      if(lowstatisticMode==false||SETightMode==false) fPwSE[ch+bd*16]->Draw("lsame");
      LChs->Draw("same");
      LFitHit->Draw("same");
      LEff->Draw("same");
      if(lowstatisticMode==false||SETightMode==false) LFitSE->Draw("same");
      
      
      cIntpw[bd]->cd(2*(ch%4)+13-ch);
      double percentgeThershold = 0.035;
      for(int ibinpw=0; ibinpw<binpw;ibinpw++){
        double tmpInt[2]={};
        tmpInt[0]=hPw[ch+bd*16]  ->Integral(ibinpw,binpw);
        tmpInt[1]=hPwSE[ch+bd*16]->Integral(ibinpw,binpw);
        hIntPwAll[0]            ->Fill(ibinpw,tmpInt[0]);
        hIntPwAll[1]            ->Fill(ibinpw,tmpInt[1]);
        if(tmpInt[0]>percentgeThershold||
           tmpInt[1]>percentgeThershold)
            SNR[ch+bd*16]    ->Fill(ibinpw,tmpInt[1]/tmpInt[0]);//@@
        hIntPwAll[1]         ->SetBinContent(ibinpw-1,1-tmpInt[1]);
      }
      double xsnrMax=SNR[ch+bd*16]->GetMaximumBin()-0.5;
      double xsnr[2]={xsnrMax, xsnrMax}; double ysnr[2]={100, -100};
      TGraph *SNRMax = new TGraph(2,xsnr,ysnr);
      
      SNR[ch+bd*16]->Draw("histsame");
      hIntPwAll[0]->Draw("histsame");
      hIntPwAll[1]->Draw("histsame");
      SNRMax->Draw("lsame");
      hIntPwAll[0]->SetLineColor(1);
      hIntPwAll[1]->SetLineColor(2);
      SNR[ch+bd*16]->SetLineColor(3);
      hPw[ch+bd*16]->GetYaxis()->SetTitleOffset(1.5);
      SNRMax->SetLineStyle(2);
      SNRMax->SetLineColorAlpha(1,0.5);
      setTitle(SNR[ch+bd*16]  ,SNRname,"pwidth cut(100ns)","S/N and Noise VS data lost ");
      setAxisRangeUser(SNR[ch+bd*16],0,50,0,3);
      TLegend *LSNR  = new TLegend(0.3,0.7,0.95,0.9);
      LSNR->SetLineColor(0);
      LSNR->AddEntry(hIntPwAll[9] ,Form("Ch-%d Cumulative rate of Hit ",ch),"l");
      LSNR->AddEntry(hIntPwAll[1] ,Form("Ch-%d Cumulative rate of Muon",ch),"l");
      LSNR->AddEntry(SNR[ch+bd*16],Form("Ch-%d S/N (Normolize scale)  ",ch),"l");
      LSNR->AddEntry(SNRMax       ,Form("Ch-%d S/N Max: %.f #pm 50ns ",ch,xsnrMax*100),"l");
      LSNR->AddEntry(""           ,Form("    PS: In case S or N rate>%.1f%% ",percentgeThershold*100),"h");
      LSNR->Draw("same");
      cpwAll[bd]->cd();
      
      if (ch==0){
        hPw[ch+bd*16]->Draw("hist");
        LChpw->Draw("same");
      }
      else       hPw[ch+bd*16]->Draw("histsame");
      hPwSE[ch+bd*16]->Draw("histsame");
        
      setTitle(hPw[ch+bd*16]  ,hpwname,"pwidth/pwidthScaleFactor(100ns)","Ratio");
      setTitle(hPwSE[ch+bd*16],hpwname,"pwidth/pwidthScaleFactor(100ns)","Ratio");
      
      setAxisRangeUser(hPw[ch+bd*16],0,60,0,0.2);
      hPw[ch+bd*16]  ->SetLineWidth(0.5);
      hPwSE[ch+bd*16]->SetLineWidth(0.5);
      hPw[ch+bd*16]  ->SetLineColor(colorArr[ch]);
      fPw[ch+bd*16]  ->SetLineColor(colorArr[ch]);
      fPwSE[ch+bd*16]->SetLineColor(colorArr[ch]);
      fPwAll[1]->SetLineWidth(1);
      fPwAll[0]->SetLineWidth(1.5);
      hPwSE[ch+bd*16]->SetLineColorAlpha(colorArr[ch],0.1);
      hPwSE[ch+bd*16]->SetFillColorAlpha(colorArr[ch],0.2);
      
      PWDFit[0][bd][ch]=fPwAll[1]->GetParameter(1);
      PWDFit[1][bd][ch]=fPwAll[1]->GetParError(1);
      PWDFit[2][bd][ch]=fPwAll[1]->GetParError(1);
      //cpw[bd]->cd(2*(ch%4)+13-ch);
      //c->cd();
    }
    
    EFFErr[bd] = new TGraphAsymmErrors(16,PWDFit[0][bd],EffTmp[0][bd],PWDFit[1][bd],PWDFit[2][bd],EffTmp[1][bd],EffTmp[2][bd]);
    cEVMPV[bd]->cd();
    EFFErr[bd]->Draw("A2");
    EFFErr[bd]->Draw("P");
    EFFErr[bd]->SetLineWidth(2);
    EFFErr[bd]->SetMarkerSize(1);
    EFFErr[bd]->SetMarkerStyle(4);
    EFFErr[bd]->SetFillColorAlpha(2,0.1);
    EFFErr[bd]->GetXaxis()->SetTitleSize(0.03);
    EFFErr[bd]->GetYaxis()->SetTitleSize(0.03);
    // EFFErr[bd]->GetXaxis()->SetRangeUser(0,18);
    // EFFErr[bd]->GetYaxis()->SetRangeUser(0.5,1.05);
    EFFErr[bd]->GetXaxis()->SetTitleOffset(2);
    EFFErr[bd]->GetYaxis()->SetTitleOffset(2.4);
    EFFErr[bd]->GetXaxis()->SetTitle(Form("MPV of Muon @#Deltatcnt=140, #theta=%d#circ (100ns)",Dtheta));
    EFFErr[bd]->GetYaxis()->SetTitle("Eff. of 4M VS 3M(+1N) @#Deltatcnt=1000, #theta=0#circ");
    LEFFErr[bd]  = new TLegend(0.65,0.3,0.95,0.4);
    setLegendDefault(LEFFErr[bd]);
    LEFFErr[bd]->AddEntry(EFFErr[bd] ,Form("#mu Mpv VS Eff of Ch."),"fpel[]");
    LEFFErr[bd]->Draw("same");
    PDFCanPage(pdfcEVMPV,cEVMPV[bd],Form("MTB-%d",BD[bd]),bd+1,NumBDt+1); 

    PDFCanPage(pdfcSNRpw,cIntpw[bd],Form("MTB-%d",BD[bd]),bd+1,NumBDt);
    //cout<<DirResultGPwaRt<<"      E         "<<endl;
    //cout<<cpwname<<"      E         "<<endl;          

    char namessss[200]={},namesintsss[200]={},nameEVMPV[200]={};
    sprintf(namessss   ,"%s%sNormal",DirResultGPw,cpwname);
    sprintf(namesintsss,"%sInt%sNormal",DirResultGPw,cpwname);
    sprintf(nameEVMPV  ,"%sEVMPV%sNormal",DirResultGPw,cpwname);

    DrawPdfPng(cpw[bd],namessss);
    DrawPdfPng(cIntpw[bd],namesintsss);
    //DrawPdfPng(cEVMPV[bd],nameEVMPV);
    
    PDFCanPage(pdfcpw   ,cpw[bd]   ,Form("MTB-%d",BD[bd]),bd+1,NumBDt);
    PDFCanPage(pdfcpwA  ,cpwAll[bd],Form("MTB-%d",BD[bd]),bd+1,NumBDt);
    // cout<<DirResultGPwaRt<<"      E         "<<endl;
  }
  // cout<<"cEVMPV[4]->cd();"<<endl;
  cEVMPV[3]->cd();
  // cout<<"EFFErr[3]->Draw()"<<endl;
  //EFFErr[3]->Draw("A");
  EFFErr[3]->Draw("Ap");
  //EFFErr[3]->GetYaxis()->SetRangeUser(0.9,1.);
  EFFErr[0]->Draw("2same");
  EFFErr[1]->Draw("2same");
  EFFErr[2]->Draw("2same");
  EFFErr[3]->Draw("2same");
  EFFErr[0]->Draw("psame");
  EFFErr[1]->Draw("psame");
  EFFErr[2]->Draw("psame");
  EFFErr[3]->Draw("psame");
  LEFFErr[0]->Draw("same");
  //EFFErr[3]->Draw("psame");
  PDFCanPage(pdfcEVMPV,cEVMPV[3],Form("All"),5,NumBDt+1);

  for(int ibd=0;ibd<NumBDt;ibd++){
    for(int ich=0;ich<NumCht;ich++){
      cout<<BD[ibd]<<"\t"<<ich<<"\t"<<PWDFit[0][ibd][ich]<<"\t"<<PWDFit[1][ibd][ich]<<"\t"<< PWDFit[2][ibd][ich]<<"\n";
    }
  }
  optime.Stop();
  optime.Print();
  
  
  
}
