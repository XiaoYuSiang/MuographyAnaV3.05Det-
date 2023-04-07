//include
#include <iostream>
#include <vector>
#include <fstream>
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
  cout<<"Now, detector Ver is:  "<<version<<endl;
  /*Set Sci size and layer ID*/

 
  TH2F *hDxDyDz = new TH2F("hDxDyDz","",binNFX,-1.4,1.4,binNFY,-1.4,1.4);
  
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
  hDxDyDz->Draw("colz");
  
  DrawPdfPng(C  ,Form("%sOAngCombinationFactorBX%03.fBY%03.f",DirIMFdat,binNFX*1.,binNFY*1.));

  TFile *myFile = new TFile(Form("%sOAngComFacBX%03.fBY%03.f.root",DirIMFdat,binNFX*1.,binNFY*1.), "RECREATE");
  myFile->WriteObject(C, "cCombinationFac");
  myFile->WriteObject(hDxDyDz, "hCombinationFac");
  cout<<myFile->GetName()<<" have created!"<<endl;
  myFile->Close();
  C->Close();
  delete hDxDyDz;
  
  
  
}




void CombinationFac(){
  // GetPosCode();
  //OCombinationFac();

  GeoAreaFac(281,281);
  GeoAreaFac(93,93);
  if(binNumFluxX!=93&&binNumFluxX!=281&&binNumFluxY!=93&&binNumFluxY!=281)
    GeoAreaFac();
  
}

