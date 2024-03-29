#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TMath.h>
#include <TH1F.h>
#include <TH1.h>
#include <TH3F.h>
#include <TGraph2D.h>
#include <TPolyLine3D.h>
#include <TCanvas.h>
#include <TGraphErrors.h>

#include "/data4/YuSiang/personalLib/Structure/Structure.h"
#include "/data4/YuSiang/personalLib/RPU/DBMTH2F.h"
using namespace std;
using namespace Structure;

using namespace ROOT::Math;


const bool LineFit3D_C_TestMode_only = 0; //@@ LineFit3D_C_TestMode_only

// define the parametric line equation
double line( double *x, const double *p ) {
  return p[0] *( x[0] - p[1] );
}
double Hline( double *x, const double *p ) {
  return x[0]*0. + p[1] ;
}


class line3D{
public:
  double rzz,rzx,rzy;
  double dzz,dzx,dzy;
  double erzz,erzx,erzy;
  double edzz,edzx,edzy;
  double chi2zx,chi2zy;
  int    NDFzx,NDFzy;
  double Sigzx,Sigzy;
  char   Nrzx[15];
  char   Nrzy[15];
  char   Ndzx[15];
  char   Ndzy[15];
  char   name[50];
  DBMLine *Lfit3D;
  TGraphErrors *lzx, *lzy;
  int npoint;
  double ErrThe, ErrPhi;
  TF1  *flzx, *flzy;
  
  line3D(){};
  ~line3D(){
    delete Lfit3D;
    delete lzx;
    delete lzy;
    delete flzx;
    delete flzy;
  };
  void SetFitInitial(){
    if(abs(rzx)<1.E-3){
      rzx = 0;
      flzx->SetParameter(0,dzx);
    }else{
      flzx->SetParameters(rzx,dzx);
    }          
    if(abs(rzy)<1.E-3){
      rzy = 0;
      flzy->SetParameter(0,dzy);
    }else{
      flzy->SetParameters(rzy,dzy); 
    }
  }
  
  void GetFitImf(){
    rzz = 1.;
    dzz = 0.;
    erzz = 0.;
    edzz = 0.;
    if(abs(rzx)>1.E-3) rzx = flzx->GetParameter(0);
    dzx = flzx->GetParameter(1);
    if(abs(rzy)>1.E-3) rzy = flzy->GetParameter(0);
    dzy = flzy->GetParameter(1);
    erzx = flzx->GetParError(0);
    edzx = flzx->GetParError(1);
    erzy = flzy->GetParError(0);
    edzy = flzy->GetParError(1);
    chi2zx = flzx->GetChisquare();
    chi2zy = flzy->GetChisquare();
    NDFzx  = flzy->GetNDF();
    NDFzy  = flzy->GetNDF();
    if(NDFzx==0) Sigzx = sqrt(chi2zx);
    else Sigzx  = sqrt(chi2zx / (NDFzx*1.));
    if(NDFzy==0) Sigzy = sqrt(chi2zy);
    else Sigzy  = sqrt(chi2zy / (NDFzy*1.));
    sprintf(Nrzx,"Slope X-Z");
    sprintf(Nrzy,"Slope Y-Z");
    sprintf(Ndzx,"Offset X-Z");
    sprintf(Ndzy,"Offset Y-Z");
  }
  
  void fit3D(){
    if(abs(rzx)<1.E-3) flzx = new TF1("flzx",Hline,-60,1560,2);
    else               flzx = new TF1("flzx",line,-60,1560,2);
    if(abs(rzy)<1.E-3) flzy = new TF1("flzy",Hline,-60,1560,2);
    else               flzy = new TF1("flzy",line,-60,1560,2);
    SetFitInitial();
    lzx->Fit(flzx,"NQ");
    lzy->Fit(flzy,"NQ");
    GetFitImf();
  }
  
  void Show(bool detail=false){
    if(detail){
      cout<<Form("┌───────Shows───────Fitting───────Start─────────┐")<<endl;
      cout<<Form("│ ParName          Value              Error     │")<<endl;
      cout<<Form("│   Rxz:    %15.5e    %15.5e  │",rzx,erzx)<<endl;
      cout<<Form("│   Dxz:    %15.5e    %15.5e  │",dzx,edzx)<<endl;
      cout<<Form("│   Ryz:    %15.5e    %15.5e  │",rzy,erzy)<<endl;
      cout<<Form("│   Dyz:    %15.5e    %15.5e  │",dzy,edzy)<<endl;
      cout<<Form("│ XZ-Chi2: %9.3e  NDF:%4.d  Sig: %9.3e  │",chi2zx,NDFzx,Sigzx)<<endl;
      cout<<Form("│ YZ-Chi2: %9.3e  NDF:%4.d  Sig: %9.3e  │",chi2zy,NDFzy,Sigzy)<<endl;
      cout<<Form("└───────Shows───────Fitting────────End──────────┘")<<endl;
    }else{
      cout<<"    "<<
      "x-"<<Form("%9.2e",dzx)<<"   "<<
      "y-"<<Form("%9.2e",dzy)<<"   "<<
      "z-"<<Form("%9.2e",dzz)<<endl;
      cout<<"LF:"<<
      "(───────────)+(───────────)+(───────────)=d, ";
      cout<<Form("d:(-inf,+inf)")<<endl;
      cout<<"     "<<
      Form("%9.2e",rzx)<<"     "<<
      Form("%9.2e",rzy)<<"     "<<
      Form("%9.2e",rzz)<<endl;
    }
  }
  
  void CalculateErrOnAngle(){
    DBMLine *Lrtm = new DBMLine("Lrtm",
      rzx+ erzx, rzy+ erzy, rzz+ erzz,
      dzx+ edzx, dzy+ edzy, dzz+ edzz, "C");
    DBMLine *LrtM = new DBMLine("LrtM",
      rzx- erzx, rzy- erzy, rzz- erzz,
      dzx- edzx, dzy- edzy, dzz- edzz, "C");
    ErrThe = (*(LrtM->dirSp[1]) - *(Lrtm->dirSp[1])) /2.;
    ErrPhi = (*(LrtM->dirSp[2]) - *(Lrtm->dirSp[2])) /2.;
    delete Lrtm;
    delete LrtM;
  }
  void SetDrawTGEOPT( TGraphErrors* TGEOBJ){
    TGEOBJ->SetFillColorAlpha(23,0.5);
    TGEOBJ->SetMarkerColorAlpha(2,1);
    TGEOBJ->SetMarkerStyle(5);
  }
  void SetDrawTGETF1OPT( TGraphErrors* TGEOBJ){
    TGEOBJ->SetFillColorAlpha(71,0.5);
    TGEOBJ->SetFillStyle(3001);
  }
  
  
  void RunGraph(const char* OutGName = "/home/yusiang/MuographyAna/Result/Test/FitL3D.pdf"){
    TH2F *BGXZ = new TH2F("BGXZ",Form("%s at X-Z plane",name),135,-63,1557,10,-50,450);
    // BGXZ->GetXaxis()->SetRangeUser(-5,155);
    BGXZ->GetXaxis()->SetRangeUser(-60,1560);
    BGXZ->GetYaxis()->SetRangeUser(0,400);
    BGXZ->GetXaxis()->SetNdivisions(4);
    BGXZ->GetYaxis()->SetNdivisions(408);
    BGXZ->SetLineColor(12);
    BGXZ->SetFillColor(12);
    BGXZ->SetMarkerColor(0);
    BGXZ->GetZaxis()->SetRangeUser(0,1);
    BGXZ->SetStats(0);
    BGXZ->GetXaxis()->SetTitle("position Z(mm)");
    BGXZ->GetXaxis()->SetTitleOffset(1.5);
    TH2F *BGYZ = (TH2F*) BGXZ->Clone();
    BGYZ->SetName("BGYZ");
    BGXZ->GetYaxis()->SetTitle("position X(mm)");
    BGYZ->GetYaxis()->SetTitle("position Y(mm)");
    BGYZ->SetTitle(Form("%s at Y-Z plane",name));
    
    TH3F *hSolid = new TH3F("hSolid","3D-Show",135,-63,1557,20,-465,35,20,-35,465);
    hSolid->SetStats(0);
    hSolid->GetXaxis()->SetTitle("position Z(mm)");
    hSolid->GetYaxis()->SetTitle("position X(mm)");
    hSolid->GetZaxis()->SetTitle("position Y(mm)");
    hSolid->GetXaxis()->SetRangeUser(-60,1560);
    hSolid->GetYaxis()->SetRangeUser(-400,0);
    hSolid->GetZaxis()->SetRangeUser(0,400);
    hSolid->GetXaxis()->SetNdivisions(4);
    hSolid->GetYaxis()->SetNdivisions(416);
    hSolid->GetZaxis()->SetNdivisions(416);
    hSolid->GetXaxis()->SetTitleOffset(1.5);
    hSolid->GetYaxis()->SetTitleOffset(1.5);
    hSolid->GetZaxis()->SetTitleOffset(1.6);
    hSolid->GetYaxis()->SetLabelOffset(-.01);
    
    TPolyLine3D l3dext(2);
    l3dext.SetLineColor(kRed);

    //hSolid->GetYaxis()->SetLabelSize(0.0);
    
    TGraph2D * TG3d = new TGraph2D();
    TG3d->SetMarkerStyle(4);
    TG3d->SetLineColor(2);
    // for(int i=0;i<5;i++) hSolid->Fill(10,10+15,i*100+15);
    for(int ip=0;ip<npoint;ip++){
      Double_t tmppx, tmppy, tmppz;
      lzx->GetPoint(ip, tmppz, tmppx);
      lzy->GetPoint(ip, tmppz, tmppy);
      BGXZ->Fill(tmppz,tmppx);
      BGYZ->Fill(tmppz,tmppy);
      hSolid->Fill(tmppz,-tmppy,tmppx);
      TG3d->SetPoint(ip,tmppz,-tmppy,tmppx);
    }
    
    double xge[100],yge[100],zge[100],exge[100],eyge[100],ezge[100];
    for(int i=0;i<100;i++){
      zge [i] = -50+1700./100.*i;
      if(rzx==0) xge [i] = dzx;
      else xge [i] = rzx*(zge [i]-dzx);
      if(rzy==0) yge [i] = dzy;
      else yge [i] = rzy*(zge [i]-dzy);
      exge[i] = Sigzx;
      eyge[i] = Sigzy;
      ezge[i] = 1700./100.;
    }
    
    l3dext.SetPoint(0,zge[0],-yge[0],xge[0]);
    l3dext.SetPoint(1,zge[99],-yge[99],xge[99]);
    
    
    TGraphErrors *TFTGErrzx= new TGraphErrors(100,zge,xge,ezge,exge);
    TGraphErrors *TFTGErrzy= new TGraphErrors(100,zge,yge,ezge,eyge);
    // TH1F *BGYZ = new TH1F("BGYZ",Form("%s at Y-Z plane",name),4,0,20);
    TCanvas *c1 = new TCanvas("c1","c1",1080,0,900,900);
    c1->Divide(1,3);
    TLegend *L1 = new TLegend(.91,.1,1.,.9);
    setLegendDefault(L1,1);
    L1->AddEntry(lzx  ,"Fire Range" ,"f");   
    L1->AddEntry(lzx  ,"Channel Center","p");   
    L1->AddEntry(flzx ,"Fitting Track ","l");   
    L1->AddEntry(TFTGErrzx ,"Possible Track ","F");   
    L1->AddEntry(TFTGErrzx ,"(95% CI)","");   
    SetDrawTGEOPT(lzx);
    SetDrawTGEOPT(lzy);
    SetDrawTGETF1OPT(TFTGErrzx);
    SetDrawTGETF1OPT(TFTGErrzy);
    
    c1->cd(1);
    c1->cd(1)->SetGrid(0,1);
    BGXZ ->Draw("box");
    lzx  ->Draw("pE2same");
    flzx ->Draw("lsame");
    TFTGErrzx->Draw("E2same");
    L1->Draw("same");
    
    c1->cd(2);
    c1->cd(2)->SetGrid(0,1);
    BGYZ ->Draw("box");
    lzy  ->Draw("pE2same");
    flzy ->Draw("lsame");
    TFTGErrzy->Draw("E2same");
    L1->Draw("same");
    
    c1->cd(3);
    hSolid->Draw("boxRX");
    TG3d->Draw("P0same");
    l3dext.Draw("lsame");
    
    c1->cd(3)->SetPhi(5);
    c1->cd(3)->SetTheta(30);
    
    c1->Print(OutGName);
    if(!LineFit3D_C_TestMode_only){
      c1->Close();
      delete BGXZ, BGYZ;
      delete TFTGErrzx, TFTGErrzy;
      delete hSolid, TG3d;
      //delete l3dext;
      delete c1;
      delete L1;
    }
    
  }

  void LSM(const double X[], const double Y[], const int np, double &dxy, double &rxy, double &mX, double &mY/*, double &SigXY*/){
    /* least square Method In 2D X-Y plane*/
    double Sxy=0, nxy=0, Sxx=0, nxx=0;
    mX = 0, mY=0;
    // SigXY = 0;
    for(int i=0;i<np;i++){
      mX   += X[i];
      mY   += Y[i];
      Sxy  += X[i]*Y[i];
      Sxx  += X[i]*X[i];
    }
    mX /= np*1.;
    mY /= np*1.;
    nxy = np* mX*mY;
    nxx = np* mX*mX;
    rxy = (Sxy - nxy) / (Sxx - nxx);
    if(abs(rxy)<1.E-3){
      dxy= mY;
      rxy= 0;
    } else dxy = mX - mY / rxy;
    // for(int i=0;i<np;i++){
      // SigXY += pow((Y[i]-(rxy*(X[i]-dxy))),2);
      // cout<< X[i]<<"\t"<<dxy<<"\t"<< rxy<<"\t" << Y[i] <<endl;
    // }
    // SigXY = sqrt(SigXY);
    
  }
  
  void line3Dfit(const double points[][3] , const int npo, const char names[]="fitting result"){
    npoint = npo;
    
    // cout<<"┌────────────┐"<<endl;
    // cout<<"│npoint:"<<Form("%4.d",npoint)<<" │"<<endl;
    // cout<<"└────────────┘"<<endl;
    double px[npo],py[npo],pz[npo],ex[npo],ey[npo],ez[npo];
    double mx=0, my=0, mz=0;
    double sigx, sigy, sigz;
    for(int i=0;i<npoint;i++){
      px[i]=( points[i][0]);
      py[i]=( points[i][1]);
      pz[i]=( points[i][2]);
      ex[i]=( 2.5 );
      ey[i]=( 2.5 );
      ez[i]=( 0.5 );
    }
    LSM(pz, px, npo, dzx, rzx, mz, mx);//, Sigzx);
    LSM(pz, py, npo, dzy, rzy, mz, my);//, Sigzy);
    
    // Show(1);
    lzx = new TGraphErrors(npoint,pz,px,ez,ex);
    lzy = new TGraphErrors(npoint,pz,py,ez,ey);

    fit3D();
    
    // if(LineFit3D_C_TestMode_only) CalculateErrOnAngle(); //@@
    Lfit3D = new DBMLine("Lfit3D", rzx, rzy, rzz, dzx, dzy, dzz, "C");

  }
  
  
  void line3Dfit( const vector<double> px,const vector<double> py,const vector<double> pz, const char names[]="fitting result"){
    int npoint=px.size();
    double  points[npoint][3];
    for(int i=0;i<npoint;i++){
      points[i][0] = px[i];
      points[i][1] = py[i];
      points[i][2] = pz[i];
    }
    line3Dfit(points,npoint,names);
  }
  

};

void lineFit3D() {  // example and test
  srand(time(NULL));
  const int num = 4;
  double poi [num][3];
  double pos[num][3]={
    {0,0,3},
    {1,1,2},
    {2,2,1},
    {3,3,0},
  };
  for(int i=0;i<num;i++){
    poi[i][0] = 2.5+pos[i][0]*5.;
    poi[i][1] = 2.5+pos[i][1]*5.;
    poi[i][2] = 0.5+pos[i][2]*50.;
  }
  line3D *L3D;
  // while(1){
    L3D = new line3D() ;
    L3D->line3Dfit( poi , num);
    
    if(LineFit3D_C_TestMode_only){
      L3D->Lfit3D->Show(1); //@@ tesat
      cout<<Form("%-13.6s   %-13.6s","The","Phi")<<endl;
      cout<<Form("%8.3f(deg)   %8.3f(deg)",(L3D->Lfit3D->Dthe)/PI*180.,(L3D->Lfit3D->Dphi)/PI*180.)<<endl;
      cout<<Form("%8.3f(rad)   %8.3f(rad)",L3D->Lfit3D->Dthe,L3D->Lfit3D->Dphi)<<endl;
      cout<<Form("%-13.6s   %-13.6s","ErrThe","ErrPhi")<<endl;
      cout<<Form("%8.3f(deg)   %8.3f(deg)",L3D->ErrThe/PI*180.,L3D->ErrPhi/PI*180.)<<endl;
      L3D->Show(1); //@@ tesat
      L3D->RunGraph(); //@@ tesat
    }
  // }
}

//@@ Sigma<1.25

