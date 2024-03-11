#include <iterator>
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
#include "DSLData.h"
#include "LoadODectTable.h"

using namespace std;
using namespace DBMMath;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace MuoAna_path_dir;
using namespace DataConst;

const int siteIndex = 3;
const float MeasureDist = 600;//m
void TrackInFoAna(
  const int indexi,
  const double trigger = TriggerTrFitSigSizeX,
  const bool testMode=false 
) {
  setTDRStyle();
  
  char FileRead[200];
  sprintf(FileRead,"%sETracksGT%d_%s_nHTH%dL%d.root",DirOperate,eventGapTcnt[indexi],TimeStr,TriggerS,TriggerL);
  TFile *SelectRfile;
  cout<<"opening....."<<FileRead<<endl;
  SelectRfile = TFile::Open(FileRead);
  TTree *tSE  = (TTree*) SelectRfile ->Get("t");

  float A0 = Altitudes[siteIndex];
  TH2F *h2 = new TH2F("h2","h2",17,-170,170,17,260,260+340);
  float CxOf = bx_2*NumBX*NumnX, CyOf = by_2*NumBY*NumnY;
  float MeanAng = tan(14.6/180*3.14159265758);//CxOf*2/gbz/(NumLY-1.);
  TCanvas *c1 = new TCanvas("c1","c1",640,640);
  c1->cd();
  string DrawTarget = Form( 
    "DyDzPos*%f+(pY[0]-%f)/1000.+%f*%f+%f:DxDzPos*%f+(pX[0]-%f)/1000.>>h2"
    ,MeasureDist,CyOf,MeasureDist,MeanAng,A0,MeasureDist,CxOf
  );
  string weight = Form(
    "(!CodeEmp)*(1./EffWei/AreaFac/(1.*NumbFac)/pow(tan(%f+DyDzPos),1.762))",MeanAng);
  cout<<DrawTarget<<endl; 
  tSE->Draw(DrawTarget.data(),weight.data(),"colz" );
  h2->Draw("colz");
  cout<<h2->Integral(-1,-1)<<endl;
  c1->Print("/home/yusiang/TestV5/aerg.png");
  
  TH2F *h3 = new TH2F("h3","h3",17,-.28,.28,17,-.280,.280);

  tSE->Draw("DyDzPos:DxDzPos>>h3",weight.data(),"colz" );

  c1->Print("/home/yusiang/TestV5/ae13rg.png");
}


void TrackInFoAna(
  const int indexi,   const int indexf, 
  const double trigger = TriggerTrFitSigSizeX,
  const bool testMode=false 
) {
  for(int i=indexi;i<indexf;i++)
    TrackInFoAna( indexi, trigger, false);
  
  
}