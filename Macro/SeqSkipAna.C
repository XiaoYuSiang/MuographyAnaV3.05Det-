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
#include <TGaxis.h>
#include <TText.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
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

void SeqSkipAna() {
  vector<string> vRootFiles = RawRootList(DirOperate);
  TreeReader data(vRootFiles);

  //Data Variable
  ofstream out(Form("%sSeqSkip.dat",DirOperate));

  Long64_t   unixtime;
  
  Int_t      nHits ;//total number for hit in a frame/event
  
  Int_t*     channel;
  Int_t*     board;
  Int_t*     seq;
  int*       pw;
  
  int seqNow[NumBD][NumCh]={};
  int NumSeqSK[NumBD][NumCh]={};
  int ChID[NumBD][NumCh]={};
  
  TH2F *hSkT[NumBD];
  TH2F *hSkTMod = Mod_DateLabel("hSkTMod","",NumCh,0,NumCh,"Channel ID");
  TH2F *hSkTModc[NumBD];
  TGraphErrors *DateGrid = Mod_DateGrid(0,NumCh);
  TH1F *hSKN[NumBD];
    
    
  hSkTMod->GetYaxis()->SetTitle("Channel ID");
  hSkTMod->GetYaxis()->SetTickLength(0.01);
  
  for(int i0=0;i0<NumCh;i0++){
    hSkTMod->GetYaxis()->SetBinLabel(i0+1,Form("%02.f",i0*1.));
  }
  
  cout<<NumBD<<endl;

  for(int i0=0;i0<NumBD;i0++){
    hSkT[i0] = new TH2F(Form("hSkT%02.f",BD[i0]*1.),"",BinOnTimeD,FirDaySec,FinDaySec,NumCh,0,NumCh);
    hSkTModc[i0] = (TH2F*) hSkTMod->Clone();
    hSkTModc[i0] -> SetTitle(Form("MTB-%02.f iZ: %d",BD[i0]*1.,i0/4));
    hSkT[i0]     ->GetZaxis()->SetLabelSize(0.02);
    hSkT[i0]     ->GetZaxis()->SetTitle("Missing Number");

  }

    
  // cout<<"BDINV[1,3,5,4]="<< BDINV[1]<<"  "<< BDINV[3]<<"  "<< BDINV[5]<<"  "<< BDINV[4]<<"\n";
  for (Long64_t ev = 0; ev < evs ; ++ev) {

    data.GetEntry(ev);
    
    // if(ev%1000 == 0) cout<<Form("\r%.5f%%\t%d",(ev*100.)/(1.*evs),unixtime)<<flush;
    unixtime  = data.GetLong64("unixtime");
    if(unixtime<1616402214) continue;
    if(ev%1000 == 0) cout<<Form("\r%.5f%%\t%d",(ev*100.)/(1.*evs),unixtime)<<flush;
    nHits     = data.GetInt("nHits");
    board     = data.GetPtrInt("board");
    channel   = data.GetPtrInt("channel");
    seq       = data.GetPtrInt("seq");
    pw        = data.GetPtrInt("pwidth");
    
    // cout<<"ev:"<<ev<<"  nH:  "<<nHits<<endl;
    for(int iH=0; iH<nHits; iH++){
      int tmp_skipNum = (seq[iH] - seqNow[board[iH]][channel[iH]] );
      if(tmp_skipNum != 1  && seqNow[board[iH]][channel[iH]]!=0 &&(tmp_skipNum>9 && seq[iH]!=0 )){
        hSkT[board[iH]-1]->Fill(unixtime,channel[iH],tmp_skipNum);
        NumSeqSK[board[iH]][channel[iH]] += tmp_skipNum;
        // cout<<unixtime<<"  B:C:W  "<<board[iH]<<"  "<<channel[iH]<<" "<<pw[iH]<<"\tSeq0->Seq: "<<seqNow[BDINV[board[iH]]][channel[iH]]<<"\t->\t"<<seq[iH]<<"\t(Skip) "<<tmp_skipNum<<endl;
        out<<unixtime<<"\t"<<board[iH]<<"\t"<<channel[iH]<<"\t"<<seqNow[board[iH]][channel[iH]]<<"\t"<<seq[iH]<<"\t"<<tmp_skipNum<<endl;
        
        //sleep(0.5);
      }
      seqNow[board[iH]][channel[iH]] = seq[iH];
      
    }
    
  }
  cout<<"\nEND Calculate, drawing graphs"<<endl;
  int canvasNumXY = ceil(sqrt(NumBD));
  TCanvas *c1 = new TCanvas("c1","c1",canvasNumXY*1920,canvasNumXY*1080);
  c1->Divide(canvasNumXY,canvasNumXY);

  for(int i0=0;i0<NumBD;i0++){
    c1->cd(i0+1);
    c1->cd(i0+1)->SetLeftMargin(0.08);
    c1->cd(i0+1)->SetRightMargin(0.12);
    hSkTModc[i0]->Draw("box");
    hSkT[i0]->Draw("colzsame");
    DateGrid->Draw("esame");
  }
  c1->Print("good.pdf");
  c1->Print("good.png");
  delete c1;
  
  c1 = new TCanvas("c1","c1",canvasNumXY*1080,canvasNumXY*1080);
  c1->Divide(canvasNumXY,canvasNumXY);

  for(int i0=0;i0<NumBD;i0++){
    c1->cd(i0+1);
    hSKN[i0] = new TH1F(Form("hSKN%02.f",i0*1.+1),Form("MTB%02.f",BDINV[i0+1]*1.),NumCh,0,NumCh);
    hSKN[i0]->SetLineColor(4);
    hSKN[i0]->GetXaxis()->SetTitle("Channel ID");
    hSKN[i0]->GetYaxis()->SetTitle("Missing Number");
    hSKN[i0]->GetYaxis()->SetTitleOffset(1.6);
    hSKN[i0]->SetStats(0);
    for(int i1=0;i1<NumCh;i1++){
      hSKN[i0]->SetBinContent(i1+1,NumSeqSK[i0][i1]);
      hSKN[i0]->GetXaxis()->SetBinLabel(i1+1,Form("%02.f",i1*1.));
    }
    hSKN[i0]->Draw("box");
    
  }
  c1->Print("good2.pdf");
  c1->Print("good2.png");
  
  
  
}