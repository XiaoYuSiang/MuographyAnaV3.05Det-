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
#include "/data4/YuSiang/personalLib/Style/DBMTStyle.h"

using namespace MuographAnaVariable;
using namespace MuographGobelFuns;
using namespace MuoAna_path_dir;
using namespace DataConst;

void DrawThickness(
  const int indexGap=28,
  const double binWidth=0.01,
  const char opt    = 'C',
  const bool Rotate = false
){
  const char DxDzVar[8] = "DxDz";
  const char DyDzVar[8] = "DyDz";
  const char PHOrNot[2] = "";
  
  char DirRoot[1][100] = {""};
  sprintf(DirRoot[0],"%s",DirOperate);
  
  vector <float> vRxyBin = GetFluxRangeBins(FluxBinDXYDZRange,binWidth,true);
  int     NumBins = vRxyBin.size()-1;

  int binNFX = NumBins, binNFY = NumBins;
  
  char GraphEndStamp[100];
  sprintf(GraphEndStamp,"%sBinXY%03.f",TimeStr,NumBins*1.);
  
  
  char saveInformation[200];
  sprintf(saveInformation,"GT%d_%s_nHTH%dL%d_BXY%03.f",eventGapTcnt[indexGap],TimeStr,TriggerS,TriggerL,NumBins*1.);

  double WorkTimeN = WorkTime - GetTotNegTime(DirOperate, unixtimei, unixtimef);
//561000*2
  char TriLesDxDzADyDz[100],TriLesDxDzADyDzPos[100], TriTimeRange[100];
  sprintf(TriTimeRange," ");
  // sprintf(TriTimeRange,"&&unixtime>1672200000&&unixtime<1673322000");
  if(Rotate){
    sprintf(TriLesDxDzADyDz,"(DxDz>%f)&&(DyDz>%f)%s",TriggerLessDyDz,TriggerLessDxDz,TriTimeRange);
    sprintf(TriLesDxDzADyDzPos,"(DxDzPos>%f)&&(DyDzPos>%f)%s",TriggerLessDyDz,TriggerLessDxDz,TriTimeRange);
  }else{
    sprintf(TriLesDxDzADyDz,"(DxDz>%f)&&(DyDz>%f)%s",TriggerLessDxDz,TriggerLessDyDz,TriTimeRange);
    sprintf(TriLesDxDzADyDzPos,"(DxDzPos>%f)&&(DyDzPos>%f)%s",TriggerLessDxDz,TriggerLessDyDz,TriTimeRange);
  }
    
  cout<<TriLesDxDzADyDz<<endl;
  cout<<TriLesDxDzADyDzPos<<endl;
  
  ifstream inFile("/home/yusiang/TestV6/Thickness/D2ToSky.dat");
  string str0;
  getline(inFile,str0);
  cout<<str0.data()<<endl;
  
  TH2F  *hThick = SetTDDDModelRAD("hThick","Thickness of rock on x-z plane",binWidth);
  float dx,dy,ex,ey,thick;
  while(inFile>>dx>>dy>>ex>>ey>>thick){
    hThick->Fill(dx,dy,thick);
  }
  TCanvas *C = new TCanvas("Canvas","",0,0,1000,1000);
  C->cd();
  hThick->Draw("colz");
}