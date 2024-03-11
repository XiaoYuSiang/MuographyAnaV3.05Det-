#include <typeinfo>
#include <algorithm>
#include <map>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TText.h>
#include <TStyle.h>
#include <TPaveStats.h>

#include "/data4/YuSiang/personalLib/RPU/DBMTThread/DBMTThread.h"
#include "/data4/YuSiang/personalLib/Style/DBMTStyle.h"
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"
// #include "DSLData.h"
#include "LoadODectTable.h"

using namespace std;
using namespace DBMMath;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace MuoAna_path_dir;
// using namespace DataConst;


const int CPUNum = cpuCores;
const int ProcessNum = FluxBinDXYDZCases;
const bool processShow   = false;

void GetPosCode(){
  map<int, GeometricAreaData>    MIGAD;
  LoadPosCodeTable(MIGAD);
  map<string, GeometricAreaData> MSGAD;
  LoadPosCodeTable(MSGAD);
  MIGAD[0].ShowTitle();
  MIGAD[0].Show();
  // cout<<95<<"  "<<int(MIGAD.size())<<endl;
  MIGAD[1].ShowTitle();
  MIGAD[1].Show();
  // cout<<98<<"  "<<int(MIGAD.size())<<endl;
  MSGAD["0,0_0,0_0,0"].ShowTitle();
  MSGAD["0,0_0,0_0,0"].Show();
}

/*

void OCombinationFac(){
  defstyle();
  vector <double> px0, px1, py0, py1;

  for(int ix = 0; ix<NumnX*NumBX; ix++){
    px0.push_back(bx_2+ix*bx);
    px1.push_back(bx_2+ix*bx);
  }
  for(int iy = 0; iy<NumnY*NumBY; iy++){
    py0.push_back(by_2+iy*by);
    py1.push_back(by_2+iy*by);
  }
  
  TH1F *hTanxz = new TH1F("hTanxz","",401,-1.4,1.4);
  TH1F *hTanyz = new TH1F("hTanyz","",401,-1.4,1.4);
  
  for(int ix1 = 0; ix1<NumnX*NumBX; ix1++){
    for(int ix0 = 0; ix0<NumnX*NumBX; ix0++){
      for(int igbz=1; igbz<=3; igbz++){
        hTanxz->Fill((px1[ix1]-px0[ix0])/(gbz*igbz),4-igbz);
        // if(abs((px1[ix1]-px0[ix0])/(gbz*igbz)-0.7)<0.1) cout<<(px1[ix1]-px0[ix0])/(gbz*igbz)<<endl;
      }
    }
  }
  
  for(int iy1 = 0; iy1<NumnY*NumBY; iy1++){
    for(int iy0 = 0; iy0<NumnY*NumBY; iy0++){
      for(int igbz=1; igbz<=3; igbz++){
        hTanyz->Fill((py1[iy1]-py0[iy0])/(gbz*igbz),4-igbz);
      }
    }
  }
  TH2F *hDxDyDz = new TH2F("hDxDyDz","",401,-1.4,1.4,401,-1.4,1.4);
  for(int ibinx=1; ibinx<=401;ibinx++){
    for(int ibiny=1; ibiny<=401;ibiny++){
      int wei = (hTanxz->GetBinContent(ibinx))
              *(hTanyz->GetBinContent(ibiny));
      hDxDyDz->SetBinContent(ibinx,ibiny,wei);
    }
  }
  hDxDyDz->SetStats(0);    
  hDxDyDz->SetTitle("Original detector Combinaiton Factor");
  hDxDyDz->GetXaxis()->SetTitle("dx/dz");
  hDxDyDz->GetXaxis()->SetLabelSize(0.03);
  hDxDyDz->GetYaxis()->SetTitle("dy/dz");
  hDxDyDz->GetYaxis()->SetTitleOffset(1.4);
  hDxDyDz->GetYaxis()->SetLabelSize(0.03);
  hDxDyDz->GetZaxis()->SetTitle("Numbers of event");
  hDxDyDz->GetZaxis()->SetTitleOffset(1.4);
  hDxDyDz->GetZaxis()->RotateTitle(true);
  hDxDyDz->GetZaxis()->SetLabelSize(0.03);
  hDxDyDz->GetXaxis()->SetRangeUser(-0.8,0.8);
  hDxDyDz->GetYaxis()->SetRangeUser(-0.8,0.8);
  FastStyleStats(0.2,1);

  TCanvas *C = new TCanvas("Canvas","",0,0,1050*8,1000*8);
  C->SetRightMargin(0.15); 
  C->SetPhi(0);
  C->SetTheta(90);
  C->SetLogz();
  hDxDyDz->Draw("colz");
  
  DrawPdfPng(C  ,Form("%sOAngCombinationFactor",DirIMFdat));

  TFile *myFile = new TFile(Form("%sOAngComFac.root",DirIMFdat), "RECREATE");
  myFile->WriteObject(C, "cCombinationFac");
  myFile->WriteObject(hDxDyDz, "hCombinationFac");
  cout<<myFile->GetName()<<" have created!"<<endl;

  
}

*/
void GeoAreaFac(
  const int binNFX = 281,//binNumFluxX,
  const int binNFY = 281//binNumFluxY
){
  FATAL("Please use the new Version GeoAreaFacV2(c.d. binWidth)");
}
/*
void GeoAreaFac(
  const int binNFX = binNumFluxX,
  const int binNFY = binNumFluxY
){
  defstyle();
  map<int, GeometricAreaData>    MIGAD;
  LoadPosCodeTable(MIGAD);
  map<string, GeometricAreaData> MSGAD;
  LoadPosCodeTable(MSGAD);
  char version[8] = {};
  DetVerCheck(version,detVer);
  cout<<"Now, detector Ver is:  "<<version<<endl;*/
  /*Set Sci size and layer ID*/
/*
 
  TH2F *hDxDyDz = new TH2F("hDxDyDz","",binNFX,FluxDxDzRx[0],FluxDxDzRx[1],binNFY,FluxDxDzRy[0],FluxDxDzRy[1]);
  
  // cout<<184<<"  "<<int(MIGAD.size())<<endl;
  
  for(int i=0; i<int(MIGAD.size()); i++){
    hDxDyDz->Fill(MIGAD[i].DXZ,MIGAD[i].DYZ,MIGAD[i].AreaFactor*MIGAD[i].NumbFactor);
    //cout<<MIGAD[i].DXZ<<"\t"<<MIGAD[i].DYZ<<"\t"<<MIGAD[i].AreaFactor<<"\t"<<MIGAD[i].NumbFactor<<endl;
  }
  
  hDxDyDz->SetStats(0);
  hDxDyDz->SetTitle("Original detector Combinaiton Factor");
  hDxDyDz->GetXaxis()->SetTitle("dx/dz");
  hDxDyDz->GetXaxis()->SetLabelSize(0.03);
  hDxDyDz->GetYaxis()->SetTitle("dy/dz");
  hDxDyDz->GetYaxis()->SetTitleOffset(1.4);
  hDxDyDz->GetYaxis()->SetLabelSize(0.03);
  hDxDyDz->GetZaxis()->SetTitle("Numbers of event");
  hDxDyDz->GetZaxis()->SetTitleOffset(1.4);
  hDxDyDz->GetZaxis()->RotateTitle(true);
  hDxDyDz->GetZaxis()->SetLabelSize(0.03);
  hDxDyDz->GetXaxis()->SetRangeUser(-0.8,0.8);
  hDxDyDz->GetYaxis()->SetRangeUser(-0.8,0.8);
  FastStyleStats(0.2,1);
  
  if(strcmp(version,"V03_05")==0){ 
    hDxDyDz->GetXaxis()->SetRangeUser(V305AngRang[0][TriggerTrNLayer][0],V305AngRang[0][TriggerTrNLayer][1]);
    hDxDyDz->GetYaxis()->SetRangeUser(V305AngRang[1][TriggerTrNLayer][0],V305AngRang[1][TriggerTrNLayer][1]);
  }else{
    cout<<"Error: Version content yo be addition!!!  CombinationFac.C~L160~L180"<<endl;
    cout<<"Now, version: "<< version<<endl;
    throw;
  }

  TCanvas *C = new TCanvas("Canvas","",0,0,1050*8,1000*8);
  C->SetRightMargin(0.15); 
  C->SetPhi(0);
  C->SetTheta(90);
  C->SetLogz();
  C->SetGrid(1,1);
  hDxDyDz->Draw("colz");
  
  DrawPdfPng(C  ,Form("%sOAngCombinationFactorBX%03.fBY%03.f",DirIMFdat,binNFX*1.,binNFY*1.));

  TFile *myFile = new TFile(Form("%sOAngComFacBX%03.fBY%03.f.root",DirIMFdat,binNFX*1.,binNFY*1.), "RECREATE");
  myFile->WriteObject(C, "cCombinationFac");
  myFile->WriteObject(hDxDyDz, "hCombinationFac");
  cout<<myFile->GetName()<<" have created!"<<endl;
  myFile->Close();
  C->Close();
  delete hDxDyDz;
  
  
  
}*/

void GeoAreaFacV2(
  const double binWidth = 0.01
){
  
  vector <float> vRxyBin = GetFluxRangeBins(FluxBinDXYDZRange,binWidth,false);
  int     NumBins = vRxyBin.size()-1;
  double RangeBin[2] = {vRxyBin[0],vRxyBin[vRxyBin.size()-1]};

  defstyle();
  map<int, GeometricAreaData>    MIGAD;
  LoadPosCodeTable(MIGAD);
  map<string, GeometricAreaData> MSGAD;
  LoadPosCodeTable(MSGAD);
  char version[8] = {};
  DetVerCheck(version,detVer);
  cout<<"Now, detector Ver is:  "<<version<<endl;
  /*Set Sci size and layer ID*/

  TH2F *hDxDyDz = SetTDDDModelRAD("hDxDyDz",Form("G.A. of binWidth = %.4f",binWidth),binWidth);

  // cout<<184<<"  "<<int(MIGAD.size())<<endl;
  
  for(int i=0; i<int(MIGAD.size()); i++){
    hDxDyDz->Fill(MIGAD[i].DXZ,MIGAD[i].DYZ,MIGAD[i].AreaFactor*MIGAD[i].NumbFactor);
    //cout<<MIGAD[i].DXZ<<"\t"<<MIGAD[i].DYZ<<"\t"<<MIGAD[i].AreaFactor<<"\t"<<MIGAD[i].NumbFactor<<endl;
  }
  
  hDxDyDz->SetStats(0);
  hDxDyDz->SetTitle("Original detector Geometry Area(G.A.)");
  hDxDyDz->GetZaxis()->SetTitle("G.A.");
  FastStyleStats(0.2,1);
  
  TCanvas *C = new TCanvas("Canvas","",0,0,1050*8,1000*8);
  C->SetRightMargin(0.15); 
  C->SetPhi(0);
  C->SetTheta(90);
  C->SetLogz();
  C->SetGrid(1,1);
  hDxDyDz->Draw("colz");
  
  DrawPdfPng(C  ,Form("%sOAngCombinationFactorBXY%03.f",DirIMFdat,NumBins*1.));

  TFile *myFile = new TFile(Form("%sOAngComFacBXY%03.f.root",DirIMFdat,NumBins*1.), "RECREATE");
  myFile->WriteObject(C, "cCombinationFac");
  myFile->WriteObject(hDxDyDz, "hCombinationFac");
  cout<<myFile->GetName()<<" have created!"<<endl;
  myFile->Close();
  C->Close();
  delete hDxDyDz;
  
}

  
void GeoAreaFacV3( const int iProc, const int iCore, const char OPT='N' ){
  const double binWidth = FluxBinDXYDZWidth[iProc];

  vector <float> vRxyBin = GetFluxRangeBins(FluxBinDXYDZRange,binWidth,false);
  int     NumBins = vRxyBin.size()-1;
  string OptFileName = Form("%sOAngComFacBXY%03.f.root",DirIMFdat,NumBins*1.);

  if( FileSize(OptFileName.data())>1.E+3){
    cout<< "The file: "<<OptFileName<<" had been created!!"<<endl;
    cout<< "Skip the file, if want to recreate files, use OPT = 'R'|'r'."<<endl;
    if(OPT != 'R'&&OPT != 'r') return;
  }

  double RangeBin[2] = {vRxyBin[0],vRxyBin[vRxyBin.size()-1]};

  defstyle();
  map<int, GeometricAreaData>    MIGAD;
  LoadPosCodeTable(MIGAD);
  map<string, GeometricAreaData> MSGAD;
  LoadPosCodeTable(MSGAD);
  char version[8] = {};
  DetVerCheck(version,detVer);
  cout<<"Now, detector Ver is:  "<<version<<endl;
  /*Set Sci size and layer ID*/

  TH2F *hDxDyDz = SetTDDDModelRAD(Form("hDxDyDz%d",iProc),Form("G.A. of binWidth = %.4f",binWidth),binWidth);

  // cout<<184<<"  "<<int(MIGAD.size())<<endl;
  
  for(int i=0; i<int(MIGAD.size()); i++){
    hDxDyDz->Fill(MIGAD[i].DXZ,MIGAD[i].DYZ,MIGAD[i].AreaFactor*MIGAD[i].NumbFactor);
    //cout<<MIGAD[i].DXZ<<"\t"<<MIGAD[i].DYZ<<"\t"<<MIGAD[i].AreaFactor<<"\t"<<MIGAD[i].NumbFactor<<endl;
  }
  
  hDxDyDz->SetStats(0);
  hDxDyDz->SetTitle(Form("Detector Geometry Area(G.A.) @ binWidth = %.4f",binWidth));
  hDxDyDz->GetZaxis()->SetTitle("G.A.");
  FastStyleStats(0.2,1);
  ths[iCore]->TThread::Lock();
  TCanvas *C = new TCanvas(Form("Canvas_%d",iProc),"",0,0,1050*8,1000*8);
  C->SetRightMargin(0.15); 
  C->SetPhi(0);
  C->SetTheta(90);
  C->SetLogz();
  C->SetGrid(1,1);
  hDxDyDz->Draw("colz");
  DrawPdfPng(C  ,Form("%sOAngCombinationFactorBXY%03.f",DirIMFdat,NumBins*1.));
  TFile *myFile = new TFile(OptFileName.data(), "RECREATE");
  myFile->WriteObject(C, "cCombinationFac");
  myFile->WriteObject(hDxDyDz, "hCombinationFac");
  cout<<myFile->GetName()<<" have created!"<<endl;
  
  myFile->Close();
  C->Close();
  myFile->Delete();
  hDxDyDz->Delete();
  
  ths[iCore]->TThread::UnLock();
}
  
void GeoAreaFacV3RAD( const int iProc, const int iCore, const char OPT='N' ){
  const double binWidth = FluxBinDXYDZWidth[iProc];

  vector <float> vRxyBin = GetFluxRangeBins(FluxBinDXYDZRange,binWidth,false);
  int     NumBins = vRxyBin.size()-1;
  string OptFileName = Form("%sOAngComFacRADBXY%03.f.root",DirIMFdat,NumBins*1.);

  if( FileSize(OptFileName.data())>1.E+3){
    cout<< "The file: "<<OptFileName<<" had been created!!"<<endl;
    cout<< "Skip the file, if want to recreate files, use OPT = 'R'|'r'."<<endl;
    if(OPT != 'R'&&OPT != 'r') return;
  }

  double RangeBin[2] = {vRxyBin[0],vRxyBin[vRxyBin.size()-1]};

  defstyle();
  map<int, GeometricAreaData>    MIGAD;
  LoadPosCodeTable(MIGAD);
  map<string, GeometricAreaData> MSGAD;
  LoadPosCodeTable(MSGAD);
  char version[8] = {};
  DetVerCheck(version,detVer);
  cout<<"Now, detector Ver is:  "<<version<<endl;
  /*Set Sci size and layer ID*/

  TH2F *hDxDyDz = SetTDDDModelRAD(Form("hDxDyDz%d",iProc),Form("G.A. of binWidth = %.4f",binWidth),binWidth);

  // cout<<184<<"  "<<int(MIGAD.size())<<endl;
  
  for(int i=0; i<int(MIGAD.size()); i++){
    hDxDyDz->Fill(MIGAD[i].DXZ,MIGAD[i].DYZ,MIGAD[i].AreaFactor*MIGAD[i].NumbFactor);
    //cout<<MIGAD[i].DXZ<<"\t"<<MIGAD[i].DYZ<<"\t"<<MIGAD[i].AreaFactor<<"\t"<<MIGAD[i].NumbFactor<<endl;
  }
  
  hDxDyDz->SetStats(0);
  hDxDyDz->SetTitle(Form("Detector Geometry Area(G.A.) @ binWidth = %.4f",binWidth));
  hDxDyDz->GetZaxis()->SetTitle("G.A.");
  FastStyleStats(0.2,1);
  ths[iCore]->TThread::Lock();
  TCanvas *C = new TCanvas(Form("Canvas_%d",iProc),"",0,0,1050*8,1000*8);
  C->SetRightMargin(0.15); 
  C->SetPhi(0);
  C->SetTheta(90);
  C->SetLogz();
  C->SetGrid(1,1);
  hDxDyDz->Draw("colz");
  DrawPdfPng(C  ,Form("%sOAngCombinationFactorRADBXY%03.f",DirIMFdat,NumBins*1.));
  TFile *myFile = new TFile(OptFileName.data(), "RECREATE");
  myFile->WriteObject(C, "cCombinationFac");
  myFile->WriteObject(hDxDyDz, "hCombinationFac");
  cout<<myFile->GetName()<<" have created!"<<endl;
  
  myFile->Close();
  C->Close();
  myFile->Delete();
  hDxDyDz->Delete();
  
  ths[iCore]->TThread::UnLock();
}

void *process(void *arg){
  /*Don't change*/
  int iCore, iProc; SetProcStart(arg, iCore, iProc);
  
  /*Adjust this funtion*/
  GeoAreaFacV3RAD(iProc,iCore);
  
  /*Don't change*/
  SetProcFinish(arg, iCore, iProc);
}

void CombinationFac(){
  // GetPosCode();
  //OCombinationFac();
  vector <float> vRxyBin[FluxBinDXYDZCases];
  for(int i=0; i<FluxBinDXYDZCases;i++)
    vRxyBin[i] = GetFluxRangeBins(FluxBinDXYDZRange,FluxBinDXYDZWidth[i],false);
  bool widthCrush = false;
  for(int i=0; i<FluxBinDXYDZCases-1;i++){
    if(vRxyBin[i].size()==vRxyBin[i+1].size()){
      widthCrush = true;
      cerr<<"Warning !!!\n";
      cerr<<"  The bins number is the same value, please choose another value."<<endl;
      cerr<<"    Now, the crush index are: "<<i<<":"<<i+1<<endl;
      cerr<<"    Now, the crush width are: "
        <<FluxBinDXYDZWidth[i]<<":"<<FluxBinDXYDZWidth[i+1]<<endl;
    }
  }  
  if(widthCrush) throw;
  // for(int i=0; i<FluxBinDXYDZCases;i++)
    // GeoAreaFacV2(FluxBinDXYDZWidth[i]);
  
  ContralThread();
}

