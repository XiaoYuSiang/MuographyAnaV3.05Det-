#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TTree.h>
#include <TGaxis.h>

#include "/home/yusiang/personalLib/Style/DBMTStyle.h"

#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "GobelFormats.h"
#include "path_dir.h"


using namespace std;
using namespace DBMMath;
using namespace MuoAna_path_dir;
using namespace MuographGobelFuns;
using namespace MuographGobelForm;

void ODectImfAna(char Opt='q') {

  
  /*Set Detector Information which before any rotate the position of scintillator are the coordinate of it own inner position*/
  //setTDRStyle();
  defstyle();

  char Path_ODetIMFData[180];
  sprintf(Path_ODetIMFData,"%s",DirIMFdat);
  // cout<<DirIMFdat<<endl;
  char version[8] = {};
  DetVerCheck(version,detVer);
  cout<<"Now, detector Ver is:  "<<version<<endl;
  /*Set Sci size and layer ID*/
  if(strcmp(version,"V02_00")==0){
    /*Set Sci Array Number*/
    
    /*Data Setting*/
      
    char Path_rotfile[100];
    sprintf(Path_rotfile,"%sSciPosDatV02_00",Path_ODetIMFData);
    TFile *rotfile = new TFile(Form("%s.root",Path_rotfile),"RECREATE");
    ofstream outfiletxt(Form("%s.txt",Path_rotfile));
    ofstream outfilecsv(Form("%s.csv",Path_rotfile));
    
    scintillator Scints;
    Scints.Sci_TxtNotation(outfiletxt);
    Scints.Sci_CsvNotation(outfilecsv);
    if(Opt!='q') Scints.Sci_CoutNotation();

    Int_t     GeneralID; //General ID of channel
    Int_t     boardID; //MTB ID
    Int_t     channelID; //ID of channel in MTB
    Int_t     iX, iY, iZ;
    float     pX, pY, pZ;
    
    TTree *tPos = new TTree("tPos","original Position of scintillator in detector V02.00");
    tPos->Branch("GeneralID"   ,&Scints.GeneralID    , "GeneralID/I");
    tPos->Branch("name"        ,&Scints.name         , "name/C"  );
    tPos->Branch("boardID"     ,&Scints.boardID      , "boardID/I"  );
    tPos->Branch("channelID"   ,&Scints.channelID    , "channelID/I");
    tPos->Branch("BiX"         ,&Scints.BiX          , "BiX/I"      );
    tPos->Branch("BiY"         ,&Scints.BiY          , "BiY/I"      );
    tPos->Branch("BiZ"         ,&Scints.BiZ          , "BiZ/I"      );
    tPos->Branch("iX"          ,&Scints.iX           , "iX/I"       );
    tPos->Branch("iY"          ,&Scints.iY           , "iY/I"       );
    tPos->Branch("iZ"          ,&Scints.iZ           , "iZ/I"       );
    tPos->Branch("pX"          ,&Scints.pX           , "pX/F"       );
    tPos->Branch("pY"          ,&Scints.pY           , "pY/F"       );
    tPos->Branch("pZ"          ,&Scints.pZ           , "pZ/F"       );
    tPos->Branch("face"        ,&Scints.face         , "face/O"     );
    tPos->Branch("PwWei"       ,&Scints.PwWei        , "PwWei/F"    );
    
    //Fill the data
    for (int idZ = 0; idZ <4; ++idZ) {//evs; ++ev) {
      for (int ich = 0; ich <16; ++ich) {//evs; ++ev) {
        boardID   = BD[idZ];
        channelID = ich;
        GeneralID = BCIDCovGID(boardID, channelID);
        Scints.SetID(GeneralID,boardID,channelID);

        Scints.SetName(Form("Scintillator-%03.f",Scints.GeneralID*1.0));

        Scints.face = true;

        iX = ich%4;
        iY = ich/4;
        iZ = idZ;
        pX = +bx*1.5  - bx*iX;
        pY = -by*1.5  + by*iY;
        pZ = +gbz*(iZ-1.5);
        
        Scints.SetBIndex  ( 0, 0, iZ);
        Scints.SetIndex   ( iX , iY , iZ );
        Scints.SetPosition( pX , pY , pZ );
        
        Scints.PwWei = BDPwWei[BDcheck(boardID)];
        
        if(Opt!='q') Scints.Sci_CoutPrint();
        Scints.Sci_TxtPrint(outfiletxt);
        Scints.Sci_CsvPrint(outfilecsv);

        tPos->Fill();
      }
    }

    rotfile->Write();
    cout<<Form("%s is RECREATE.",Path_rotfile)<<endl;
    rotfile->Close();
  }else if(strcmp(version,"V03_00")==0){
      
    char Path_rotfile[100];
    sprintf(Path_rotfile,"%sSciPosDatV03_00",Path_ODetIMFData);
    TFile *rotfile = new TFile(Form("%s.root",Path_rotfile),"RECREATE");
    ofstream outfiletxt(Form("%s.txt",Path_rotfile));
    ofstream outfilecsv(Form("%s.csv",Path_rotfile));
    
    scintillator Scints;
    Scints.Sci_TxtNotation(outfiletxt);
    Scints.Sci_CsvNotation(outfilecsv);
    if(Opt!='q') Scints.Sci_CoutNotation();
    
    TTree *tPos = new TTree("tPos","original Position of scintillator in detector V03.05");
    tPos->Branch("GeneralID"   ,&Scints.GeneralID    , "GeneralID/I");
    tPos->Branch("name"        ,&Scints.name         , "name/C"  );
    tPos->Branch("boardID"     ,&Scints.boardID      , "boardID/I"  );
    tPos->Branch("channelID"   ,&Scints.channelID    , "channelID/I");
    tPos->Branch("BiX"         ,&Scints.BiX          , "BiX/I"      );
    tPos->Branch("BiY"         ,&Scints.BiY          , "BiY/I"      );
    tPos->Branch("BiZ"         ,&Scints.BiZ          , "BiZ/I"      );
    tPos->Branch("iX"          ,&Scints.iX           , "iX/I"       );
    tPos->Branch("iY"          ,&Scints.iY           , "iY/I"       );
    tPos->Branch("iZ"          ,&Scints.iZ           , "iZ/I"       );
    tPos->Branch("pX"          ,&Scints.pX           , "pX/F"       );
    tPos->Branch("pY"          ,&Scints.pY           , "pY/F"       );
    tPos->Branch("pZ"          ,&Scints.pZ           , "pZ/F"       );
    tPos->Branch("face"        ,&Scints.face         , "face/O"     );
    tPos->Branch("PwWei"       ,&Scints.PwWei        , "PwWei/F"    );

    int bid, cid, ix, iy, iz, bix, biy, biz;
    double px, py, pz;
    
    //Fill the data
    for(int ibd=0;ibd<NumBD;ibd++){
      for(int ich=0;ich<NumCh;ich++){
        
        bid = BD[ibd]; 
        cid = ich; 
        Scints.SetID(BCIDCovGID(bid,cid),bid,cid);
        Scints.SetName(Form("Scintillator-%03.f",Scints.GeneralID*1.0));
        
        Scints.face = true;
        
        bix = abs(bid%2-1);
        biy = 1-((bid-1)/2)%2;
        biz = BDcheck(bid)/NumnZ;//@@
        
        ix = ich/4 + bix*4;
        iy = ich%4 + biy*4;
        iz = biz;
        
        px = bx_2 +ix*bx +bix*gbx;
        py = by_2 +iy*by +biy*gby;
        pz = bz_2        +biz*gbz;
        
        Scints.SetBIndex  ( bix, biy, biz);
        Scints.SetIndex   ( ix , iy , iz );
        Scints.SetPosition( px , py , pz );
        
        if(Opt!='q') Scints.Sci_CoutPrint();
        Scints.Sci_TxtPrint(outfiletxt);
        Scints.Sci_CsvPrint(outfilecsv);
        tPos->Fill();
      }
    }

    rotfile->Write();
    cout<<Form("%s is RECREATE.",Path_rotfile)<<endl;
    rotfile->Close();
  }else if(strcmp(version,"V03_05")==0){//@@
      
    char Path_rotfile[100];
    sprintf(Path_rotfile,"%sSciPosDatV03_05",Path_ODetIMFData);
    TFile *rotfile = new TFile(Form("%s.root",Path_rotfile),"RECREATE");
    ofstream outfiletxt(Form("%s.txt",Path_rotfile));
    ofstream outfilecsv(Form("%s.csv",Path_rotfile));

    scintillator Scints;
    Scints.Sci_TxtNotation(outfiletxt);
    Scints.Sci_CsvNotation(outfilecsv);
    if(Opt!='q') Scints.Sci_CoutNotation();
    
    TTree *tPos = new TTree("tPos","original Position of scintillator in detector V03.05");
    tPos->Branch("GeneralID"   ,&Scints.GeneralID    , "GeneralID/I");
    tPos->Branch("name"        ,&Scints.name         , "name/C"  );
    tPos->Branch("boardID"     ,&Scints.boardID      , "boardID/I"  );
    tPos->Branch("channelID"   ,&Scints.channelID    , "channelID/I");
    tPos->Branch("BiX"         ,&Scints.BiX          , "BiX/I"      );
    tPos->Branch("BiY"         ,&Scints.BiY          , "BiY/I"      );
    tPos->Branch("BiZ"         ,&Scints.BiZ          , "BiZ/I"      );
    tPos->Branch("iX"          ,&Scints.iX           , "iX/I"       );
    tPos->Branch("iY"          ,&Scints.iY           , "iY/I"       );
    tPos->Branch("iZ"          ,&Scints.iZ           , "iZ/I"       );
    tPos->Branch("pX"          ,&Scints.pX           , "pX/F"       );
    tPos->Branch("pY"          ,&Scints.pY           , "pY/F"       );
    tPos->Branch("pZ"          ,&Scints.pZ           , "pZ/F"       );
    tPos->Branch("face"        ,&Scints.face         , "face/O"     );
    tPos->Branch("PwWei"       ,&Scints.PwWei        , "PwWei/F"    );

    int bid, cid, ix, iy, iz, bix, biy, biz;
    double px, py, pz;
    for(int ibd=0;ibd<NumBD;ibd++){
      for(int ich=0;ich<NumCh;ich++){
        
        bid = BD[ibd]; 
        cid = ich; 
        Scints.SetID(BCIDCovGID(bid,cid),bid,cid);
        Scints.SetName(Form("Scintillator-%03.f",Scints.GeneralID*1.0));
        
        if   (bid%4<2) Scints.face = true;
        else           Scints.face = false;
        
        bix = abs(bid%2-1);
        biy = 1-((bid-1)/2)%2;
        biz = BDcheck(bid)/NumnZ;//@@
        
        if   (Scints.face) ix = cid%4+bix*4;
        else               ix = 3-cid%4+bix*4;
        iy = 3-cid/4+biy*4;
        iz  =  biz;
        
        px = bx_2 +ix*bx +bix*gbx;
        py = by_2 +iy*by +biy*gby;
        pz = bz_2        +biz*gbz;
        
        Scints.SetBIndex  ( bix, biy, biz);
        Scints.SetIndex   ( ix , iy , iz );
        Scints.SetPosition( px , py , pz );
        
        Scints.PwWei = BDPwWei[BDcheck(bid)];
        
        if(Opt!='q') Scints.Sci_CoutPrint();
        Scints.Sci_TxtPrint(outfiletxt);
        Scints.Sci_CsvPrint(outfilecsv);
        tPos->Fill();
      }
    }
    rotfile->Write();
    cout<<Form("%s is RECREATE.",Path_rotfile)<<endl;
    
    
    gStyle->SetHistMinimumZero(kTRUE);
    TH2F *HiXiY = new TH2F("HiXiY","",8,0,8,8,0,8);
    
    HiXiY->SetStats(0);
    setTitle(HiXiY,"iX","iY","");
    HiXiY->GetXaxis()->CenterTitle(true);
    HiXiY->GetYaxis()->CenterTitle(true);
    HiXiY->GetXaxis()->CenterLabels(true);
    HiXiY->GetYaxis()->CenterLabels(true);
    HiXiY->GetXaxis()->SetNdivisions(8);
    HiXiY->GetYaxis()->SetNdivisions(8);
    HiXiY->GetXaxis()->SetTitleOffset(1.0);
    HiXiY->GetYaxis()->SetTitleOffset(1.0);
    HiXiY->GetZaxis()->SetTitleOffset(0);
    HiXiY->GetXaxis()->SetLabelSize(0.05);
    HiXiY->GetYaxis()->SetLabelSize(0.05);
    HiXiY->GetXaxis()->SetTitleSize(0.05);
    HiXiY->GetYaxis()->SetTitleSize(0.05);

    HiXiY->SetMarkerSize(3);
    HiXiY->SetMarkerColor(1);
    
    
    TH2F *HiB = new TH2F("HiB","",2,0,8,2,0,8);
    HiB->SetFillStyle(4000);
    HiB->SetStats(0);
    setTitle(HiB,"","","");
    HiB->GetXaxis()->SetLabelOffset(5);
    HiB->GetYaxis()->SetLabelOffset(5);
    HiB->GetXaxis()->SetNdivisions(8);
    HiB->GetYaxis()->SetNdivisions(8);
    HiB->SetMarkerSize(5);
    HiB->SetMarkerColorAlpha(2,0.4);
    
    
    TH2F *HBox = new TH2F("Hox","",2,0,8,2,0,8);
    for(int i=0;i<4;i++) HBox->Fill(i/2*4,i%2*4,1);
    HBox->SetStats(0);
    HBox->GetXaxis()->SetNdivisions(2);
    HBox->GetYaxis()->SetNdivisions(2);
    HBox->SetMarkerColor(2);
    HBox->SetLineColor(2);
    HBox->SetLineStyle(2);
    HBox->GetZaxis()->SetRangeUser(0,1);

    TGaxis *AXmm = new TGaxis(0,0,8,0,0,40,8,"+");
    TGaxis *AYmm = new TGaxis(0,0,0,8,0,40,8,"-");
    AXmm->SetTitle("pX(mm)");
    AYmm->SetTitle("pY(mm)");

    AXmm->SetLabelSize(0.025); 
    AYmm->SetLabelSize(0.025); 
    AXmm->SetTitleColor(4);
    AYmm->SetTitleColor(4);
    AXmm->SetLabelColor(4);
    AYmm->SetLabelColor(4);


    TCanvas *c;
    c = new TCanvas("c","c",1080,1080);
    TPDF *pdfc = new TPDF(Form("%sChID&iXYZ.pdf",DirIMFdat));
    PDFCanPage(pdfc    ,c ,Form("Title"),1,NumLY);
    delete c;

    TCanvas *cA = new TCanvas("cA","",1080,1080*NumLY);
    cA->Divide(1,4);

    for(int i=3;i>=0;i--){
      c = new TCanvas("c","c",1080,1080);
      c->SetGrid(1,1);
      c->cd();
      
      tPos->Draw("iY:iX>>HiXiY",Form("int((channelID)*(iZ==%d))",i),"text");
      
      TPad *pad1 = new TPad("pad1","",0,0,1,1);
      pad1->SetFillStyle(4000); //will be transparent
      pad1->SetFrameFillStyle(4000);
      pad1->SetGrid(1,1);
      
      pad1->Draw();
      pad1->cd();
      HiB->SetTitle(Form("iZ=%d, pZ=%.0fmm",i,bz_2+gbz*i));
      tPos->Draw("BiY*4:BiX*4>>HiB",Form("int((boardID)*(iZ==%d))/16",i),"text");
      c->cd();
      AXmm->Draw("same");
      AYmm->Draw("same");
      
      TPad *pad2 = new TPad("pad2","",0,0,1,1);
      pad2->SetFillStyle(4000); //will be transparent
      pad2->SetFrameFillStyle(4000);
      pad2->SetGrid(1,1);
      pad2->SetLineColor(2);
      
      pad2->Draw();
      pad2->cd();
      HBox->Draw("boxsameA");
      DrawPdfPng(c  ,Form("%sChID&iXY_iZ%d",DirIMFdat,i));
      PDFCanPage(pdfc,c,Form("iZ=%d",i),4-i,NumLY);
      cA->cd(4-i);
      c->DrawClonePad();
      delete c;
    }
    
    cA->Print(Form("%sChID&iXYZ.png",DirIMFdat));
    c = new TCanvas("c","c",300,900);
    c->cd();
    c->cd()->SetPhi(15);
    c->cd()->SetTheta(15);
    
    TH3F *g0 = new TH3F("g0","Scintillators Array",8*2,2.5,42.5,8*2,2.5,42.5,200*2,0,200);

    tPos->Draw("pZ/10:pY/10:pX/10>>g0","1","box");

    g0->SetName("Scintillators Array");
    g0->SetStats(0);
    g0->GetXaxis()->SetRangeUser(0,40);
    g0->GetYaxis()->SetRangeUser(0,40);
    g0->GetZaxis()->SetRangeUser(0,160);
    g0->GetXaxis()->SetTitle("X position(cm)");
    g0->GetYaxis()->SetTitle("Y position(cm)");
    g0->GetZaxis()->SetTitle("Z position(cm)");
    g0->GetXaxis()->SetTitleOffset(1.2);
    g0->GetYaxis()->SetTitleOffset(1.8);
    g0->GetZaxis()->SetTitleOffset(1.5);
    g0->GetXaxis()->CenterTitle(true);
    g0->GetYaxis()->CenterTitle(true);
    
    g0->Draw("box");
    
    
    
    c->Print(Form("%sScintillators_Array.png",DirIMFdat));
    c->Print(Form("%sScintillators_Array.pdf",DirIMFdat));
    
    rotfile->Close();


  }
}








  
  
  
  
  