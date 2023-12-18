#include "MuonElectronIDSub.C"

void MuonEleIDAna( const char *RFNStr,const char *TestMarker, const double eventgap) {
  setTDRStyle();
  char DirResultPwVCase[150]={};//save path graph of Pwidth Rate
  sprintf(DirResultPwVCase,Form("%s%sBoardPwVCase/",DirResult,TestMarker));
  if (system(Form("mkdir -p %s",DirResultPwVCase))!=0) system(Form("mkdir -p %s",DirResultPwVCase));
  string EventFuileName = Form("%s%sEventsGT%s.root",DirOperate,TestMarker,RFNStr);
  TFile *rotfile_0 = TFile::Open(EventFuileName.data());
  cout<<EventFuileName.data()<<" is opened"<<endl;
  //cout<<"No ERR  new TH1F"<<endl;
  TTree *tf = (TTree*) rotfile_0 ->Get("t");
  TH1F *hPwidth       = new TH1F("hPwidth"      ,"hPwidth"      ,binpw,0,binpw);
  TH1F *hPwidthMuEle  = new TH1F("hPwidthMuEle" ,"hPwidthMuEle" ,binpw,0,binpw);
  TH1F *hPwidthNoiID  = new TH1F("hPwidthNoiID" ,"hPwidthNoiID" ,binpw,0,binpw);
  
  TH1F *hPwidthNoiID0 = new TH1F("hPwidthNoiID0","hPwidthNoiID0",binpw,0,binpw);
  TH1F *hPwidthNoiID1 = new TH1F("hPwidthNoiID1","hPwidthNoiID1",binpw,0,binpw);
  TH1F *hPwidthNoiID2 = new TH1F("hPwidthNoiID2","hPwidthNoiID2",binpw,0,binpw);
  TH1F *hPwidthNoiID3 = new TH1F("hPwidthNoiID3","hPwidthNoiID3",binpw,0,binpw);
  
  TH1F *hPwidthMuoID5 = new TH1F("hPwidthMuoID5","hPwidthMuoID5",binpw,0,binpw);
  TH1F *hPwidthMuoID4 = new TH1F("hPwidthMuoID4","hPwidthMuoID4",binpw,0,binpw);
  TH1F *hPwidthMuoID3 = new TH1F("hPwidthMuoID3","hPwidthMuoID3",binpw,0,binpw);
  TH1F *hPwidthMuoID2 = new TH1F("hPwidthMuoID2","hPwidthMuoID2",binpw,0,binpw);
  TH1F *hPwidthEleID  = new TH1F("hPwidthEleID" ,"hPwidthEleID" ,binpw,0,binpw);
  
  TH1F *hPwidthCases[11] = {hPwidth,hPwidthNoiID,hPwidthNoiID0,hPwidthNoiID1,hPwidthNoiID2,hPwidthNoiID3,hPwidthMuoID5,hPwidthMuoID4,hPwidthMuoID3,hPwidthMuoID2,hPwidthEleID};
  
  
  TH1F *hPwidthCasesS[5] = {hPwidthMuoID5,hPwidthMuoID4,hPwidthMuoID3,hPwidthMuoID2,hPwidthEleID};
  
  for(int i=0;i<11;i++){
    hPwidthCases[i]->SetLineColor(colorArr[i]);
  }
  
  TLegend *LpwWN = new TLegend(0.60,0.66,0.78,0.90);
  TLegend *LpwNN = new TLegend(0.78,0.70,0.96,0.90);
  char WNNN[2][3]={"WN","NN"};
  setLegendDefault(LpwWN,1);
  setLegendDefault(LpwNN,1);
  LpwWN->AddEntry(hPwidth      ,"All hit"              ,"f");   
  LpwWN->AddEntry(hPwidthNoiID ,"Noise hit"            ,"f");   
  LpwWN->AddEntry(hPwidthNoiID0,"Noise hit(nH=1)"      ,"f");   
  LpwWN->AddEntry(hPwidthNoiID1,"Noise hit(nH=2)"      ,"f");   
  LpwWN->AddEntry(hPwidthNoiID2,"Noise hit(nH=3)"      ,"f");   
  LpwWN->AddEntry(hPwidthNoiID3,"Noise hit(nH>3)"      ,"f");   
  //cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
  LpwNN->AddEntry(hPwidthMuoID5,"Muon hit(4M+0N)"      ,"f");   
  LpwNN->AddEntry(hPwidthMuoID4,"Muon hit(4M+1N|3M)"   ,"f");   
  LpwNN->AddEntry(hPwidthMuoID3,"Muon hit(4M+2N|3M+1N)","f");   
  LpwNN->AddEntry(hPwidthMuoID2,"Muon hit(4M+3N|3M+2N)","f");   
  LpwNN->AddEntry(hPwidthEleID ,"Electron hit(>4)"     ,"f");   
  //cout<<"No ERR TCanvas"<<endl;
  TCanvas *cpwvcase = new TCanvas("cpwvcase","",1920,1080*2);
  cpwvcase->Divide(1,2);
  cpwvcase->cd(2);
  TPad *pad1 = new TPad("pad1","",0,0,1,1);
  pad1->Draw();
  pad1->Divide(2,2);

  cpwvcase->cd(1);
  // fill histrogram
  tf->Draw("pwidth/pwidthScaleFactor>>hPwidth","","hist");
  cout<<"Finish Drawing pwidth All Event "<<endl;
  tf->Draw("pwidth/pwidthScaleFactor>>hPwidthNoiID","NoiseID","histsame");
  cout<<"Finish Drawing pwidth All hPwidthNoiID "<<endl;
  tf->Draw("pwidth/pwidthScaleFactor>>hPwidthNoiID0","NoiseID&&nH==1","histsame");
  cout<<"Finish Drawing pwidth All hPwidthNoiID nH=1"<<endl;
  tf->Draw("pwidth/pwidthScaleFactor>>hPwidthNoiID1","NoiseID&&nH==2","histsame");
  cout<<"Finish Drawing pwidth All hPwidthNoiID nH=2"<<endl;
  tf->Draw("pwidth/pwidthScaleFactor>>hPwidthNoiID2","NoiseID&&nH==3","histsame");
  cout<<"Finish Drawing pwidth All hPwidthNoiID nH=3"<<endl;
  tf->Draw("pwidth/pwidthScaleFactor>>hPwidthNoiID3","NoiseID&&nH>3","histsame");
  cout<<"Finish Drawing pwidth All hPwidthNoiID nH>3"<<endl;
  tf->Draw("pwidth/pwidthScaleFactor>>hPwidthMuoID5","MuonID&&IDtight==5","histsame");
  cout<<"Finish Drawing pwidth All MuonID IDtight==5"<<endl;
  tf->Draw("pwidth/pwidthScaleFactor>>hPwidthMuoID4","MuonID&&IDtight==4","histsame");
  cout<<"Finish Drawing pwidth All MuonID IDtight==4"<<endl;
  tf->Draw("pwidth/pwidthScaleFactor>>hPwidthMuoID3","MuonID&&IDtight==3","histsame");
  cout<<"Finish Drawing pwidth All MuonID IDtight==3"<<endl;
  tf->Draw("pwidth/pwidthScaleFactor>>hPwidthMuoID2","MuonID&&IDtight==2","histsame");
  cout<<"Finish Drawing pwidth All MuonID IDtight==2"<<endl;
  tf->Draw("pwidth/pwidthScaleFactor>>hPwidthEleID","ElectronID","histsame");
  cout<<"Finish Drawing pwidth All ElectronID"<<endl; //cout<<287<<endl;
  Long64_t Num_hPwidthCases[11] ={};
  TH1F *hPwidthCasesNor[11] = {};
  TH1F *hPwidthCasesSta[11] = {}; //cout<<290<<endl;
  string Str_CaseNumOptLog = Form("%s%sCaseNumber_TG%.0f.txt",DirOperate,TestMarker,eventgap);
  cout<<Str_CaseNumOptLog<<endl;
  ofstream outCaseNum(Str_CaseNumOptLog.data());
  // cout<<286<<endl;
  for(int i=0;i<11;i++){
    hPwidthCases[i]->SetTitle("");
    hPwidthCases[i]->GetXaxis()->SetTitle("Scale Normalized pwidth(100ns)");
    hPwidthCases[i]->GetYaxis()->SetTitle("Hits Number");
    hPwidthCases[i]->GetXaxis()->SetRangeUser(-10,90);
    hPwidthCases[i]->SetStats(0);
  }
  // cout<<294<<endl;
  int    BinMaxI[2] = {};
  double BinMaxD[2] = {};
  
  double binmaxtem = 0;
  for(int icase= 0;icase<11;icase++){
    Num_hPwidthCases[icase]=hPwidthCases[icase]->Integral(-1,-1);
    binmaxtem = hPwidthCases[icase]->GetBinContent(hPwidthCases[icase]->GetMaximumBin());
    if(binmaxtem>BinMaxI[0]) BinMaxI[0]= binmaxtem;
    outCaseNum<<hPwidthCases[icase]->GetTitle()<<"\t"<<Num_hPwidthCases[icase]<<endl;
    hPwidthCasesNor[icase] = (TH1F*) hPwidthCases[icase]->Clone();
    hPwidthCasesNor[icase] ->Scale(1./Num_hPwidthCases[icase]);
    hPwidthCasesNor[icase] ->SetTitle(Form("%sNor",hPwidthCases[icase]->GetTitle()));
    hPwidthCasesNor[icase] ->SetName(Form("%sNor",hPwidthCases[icase]->GetName()));
    binmaxtem = hPwidthCasesNor[icase]->GetBinContent(hPwidthCasesNor[icase]->GetMaximumBin());
    if(binmaxtem>BinMaxD[0]) BinMaxD[0]= binmaxtem;
    hPwidthCasesNor[icase]->GetXaxis()->SetRangeUser(0,60);
    ///hPwidthCasesNor[inn*6]->GetYaxis()->SetRangeUser(0,0.15);
    hPwidthCasesNor[icase]->GetYaxis()->SetTitle("Hits Number Ratio");
    
    hPwidthCasesSta[icase] = (TH1F*) hPwidthCases[icase]->Clone();
    hPwidthCasesSta[icase]->SetFillColorAlpha(colorArr[icase],1);
  }
  // cout<<317<<endl;
  THStack *hsPwidthVCaseWN = new THStack("hsPwidthVCaseWN","");//histrogramStachForNormalScale
  THStack *hsPwidthVCaseNN = new THStack("hsPwidthVCaseNN","");//histrogramStachForNormalScale
  THStack *hsPwidthVCase[2]={hsPwidthVCaseWN,hsPwidthVCaseNN};
  
  for(int iNcase=10;iNcase>1;iNcase--)  hsPwidthVCaseWN->Add(hPwidthCasesSta[iNcase]);
  for(int iNcase=6;iNcase<11 ;iNcase++) hsPwidthVCaseNN->Add(hPwidthCasesSta[iNcase]);
  
  for(int icase=0;icase<6;icase++){
    hPwidthCases   [icase]->GetYaxis()->SetRangeUser(1,BinMaxI[0]*1.2);
    hPwidthCasesNor[icase]->GetYaxis()->SetRangeUser(1,BinMaxD[0]*1.2);
  } 
  
  for(int iScase=0;iScase<5;iScase++) hPwidthMuEle->Add(hPwidthCasesS[iScase]);
  BinMaxI[1] = hPwidthMuEle->GetBinContent(hPwidthMuEle->GetMaximumBin());
  BinMaxD[1] = (1.*BinMaxI[1])/(1.*hPwidthMuEle->Integral(-1,-1));
  for(int iScase=0;iScase<5;iScase++){
    hPwidthCases   [iScase+6]->GetYaxis()->SetRangeUser(1,BinMaxI[1]*1.2);
    hPwidthCasesNor[iScase+6]->GetYaxis()->SetRangeUser(1,BinMaxD[1]*1.2);
  } 
  
  for(int inn=0;inn<2;inn++){

    cpwvcase->cd(1);
    hPwidthCasesNor[0+inn*6]->Draw("hist");
    for(int i1=1+inn*6;i1<11;i1++) hPwidthCasesNor[i1]->Draw("histsame");
    LpwWN->Draw("same");
    LpwNN->Draw("same");
    
    cpwvcase->cd(2);
    pad1->cd(2)->SetLogy();
    pad1->cd(4)->SetLogy();
    
    
    pad1->cd(1);
    hPwidthCases[0+inn*6]->Draw("hist");
    for(int i1=1+inn*6;i1<11;i1++) hPwidthCases[i1]->Draw("histsame");
    if(inn==0) LpwWN->Draw("same");
    LpwNN->Draw("same");
    
    pad1->cd(2);
    hPwidthCases[0+inn*6]->Draw("hist");
    for(int i1=1+inn*6;i1<11;i1++) hPwidthCases[i1]->Draw("histsame");
    if(inn==0) LpwWN->Draw("same");
    LpwNN->Draw("same");
    
    pad1->cd(3);
    hPwidthCases[0+inn*6]->Draw("hist");
    hsPwidthVCase[inn]->Draw("Fhistsame");
    if(inn==0) LpwWN->Draw("same");
    LpwNN->Draw("same");
      
    pad1->cd(4);
    hPwidthCases[0+inn*6]->Draw("hist");
    hsPwidthVCase[inn]->Draw("Fhistsame");
    if(inn==0) LpwWN->Draw("same");
    LpwNN->Draw("same");
      
    cpwvcase->Print(Form("%sPwidthVSCaseTG%.0f%s.pdf",DirResultPwVCase,eventgap,WNNN[inn]));
  }
  outCaseNum.close();
  // cout<<377<<endl;
  rotfile_0->Close();
}


void IDCaseNumAna( const char *RFNStr,const char *TestMarker, const double eventgap) {
  defstyle();
  setTDRStyle();
  char DirResultPwVCase[150]={};//save path graph of Pwidth Rate
  sprintf(DirResultPwVCase,"%s%sBoardPwVCase/",DirResult,TestMarker);
  if (system(Form("mkdir -p %s",DirResultPwVCase))!=0) system(Form("mkdir -p %s",DirResultPwVCase));
  
  TFile *rotfile = TFile::Open(Form("%s%sEventsGT%s.root",DirOperate,TestMarker,RFNStr));
  cout<<Form("%s%sEventsGT%s.root",DirOperate,TestMarker,RFNStr)<<" is opened"<<endl;
  TTree *tf = (TTree*) rotfile ->Get("t");
  TH1F *hCaseID       = new TH1F("hCaseID"      ,"hCaseID"      ,65,0,65);
  TH1F *hCaseIDNoiID  = new TH1F("hCaseIDNoiID" ,"hCaseIDNoiID" ,65,0,65);
  TH1F *hCaseIDNoiID0 = new TH1F("hCaseIDNoiID0","hCaseIDNoiID0",65,0,65);
  TH1F *hCaseIDNoiID1 = new TH1F("hCaseIDNoiID1","hCaseIDNoiID1",65,0,65);
  TH1F *hCaseIDNoiID2 = new TH1F("hCaseIDNoiID2","hCaseIDNoiID2",65,0,65);
  TH1F *hCaseIDNoiID3 = new TH1F("hCaseIDNoiID3","hCaseIDNoiID3",65,0,65);
  TH1F *hCaseIDMuoID5 = new TH1F("hCaseIDMuoID5","hCaseIDMuoID5",65,0,65);
  TH1F *hCaseIDMuoID4 = new TH1F("hCaseIDMuoID4","hCaseIDMuoID4",65,0,65);
  TH1F *hCaseIDMuoID3 = new TH1F("hCaseIDMuoID3","hCaseIDMuoID3",65,0,65);
  TH1F *hCaseIDMuoID2 = new TH1F("hCaseIDMuoID2","hCaseIDMuoID2",65,0,65);
  TH1F *hCaseIDEleID  = new TH1F("hCaseIDEleID" ,"hCaseIDEleID" ,65,0,65);
  TH1F *hCaseIDCases[11] = {hCaseID,hCaseIDNoiID,hCaseIDNoiID0,hCaseIDNoiID1,hCaseIDNoiID2,hCaseIDNoiID3,hCaseIDMuoID5,hCaseIDMuoID4,hCaseIDMuoID3,hCaseIDMuoID2,hCaseIDEleID};
  for(int i=0;i<11;i++){
    hCaseIDCases[i]->SetLineColor(colorArr[i]);
  }
  TLegend *LCaseNum = new TLegend(0.6,0.65,0.96,0.9);
  LCaseNum->SetFillColor(0);
  LCaseNum->SetLineColor(0);
  LCaseNum->SetNColumns(2);
  LCaseNum->AddEntry(hCaseID      ,"All hit"              ,"f");   
  LCaseNum->AddEntry(hCaseIDNoiID ,"Noise hit"            ,"f");   
  LCaseNum->AddEntry(hCaseIDNoiID0,"Noise hit(nH=1)"      ,"f");   
  LCaseNum->AddEntry(hCaseIDNoiID1,"Noise hit(nH=2)"      ,"f");   
  LCaseNum->AddEntry(hCaseIDNoiID2,"Noise hit(nH=3)"      ,"f");   
  LCaseNum->AddEntry(hCaseIDNoiID3,"Noise hit(nH>3)"      ,"f");   
  LCaseNum->AddEntry(hCaseIDMuoID5,"Muon hit(4M+0N)"      ,"f");   
  LCaseNum->AddEntry(hCaseIDMuoID4,"Muon hit(4M+1N|3M)"   ,"f");   
  LCaseNum->AddEntry(hCaseIDMuoID3,"Muon hit(4M+2N|3M+1N)","f");   
  LCaseNum->AddEntry(hCaseIDMuoID2,"Muon hit(4M+3N|3M+2N)","f");   
  LCaseNum->AddEntry(hCaseIDEleID ,"Electron hit(>4)"     ,"f");   
  TCanvas *ccaseNum = new TCanvas("ccaseNum","",3840,2160*2);
  ccaseNum->Divide(1,2);
  ccaseNum->cd(2)->SetLogy();
  hCaseID->GetXaxis()->SetTitle("number of Hits");
  hCaseID->GetYaxis()->SetTitle("Hits Number");
  hCaseID->GetXaxis()->SetRangeUser(-10,90);
  // fill histrogram
  tf->Draw("iHit>>hCaseID"      ,""                        ,"hist");
  cout<<"Finish Drawing pwidth All Event "<<endl;
  tf->Draw("iHit>>hCaseIDNoiID" ,"(NoiseID)"           ,"histsame");
  cout<<"Finish Drawing pwidth All hCaseIDNoiID "<<endl;
  tf->Draw("iHit>>hCaseIDNoiID0","(NoiseID&&nH==1)"    ,"histsame");
  cout<<"Finish Drawing pwidth All hCaseIDNoiID nH=1"<<endl;
  tf->Draw("iHit>>hCaseIDNoiID1","(NoiseID&&nH==2)"    ,"histsame");
  cout<<"Finish Drawing pwidth All hCaseIDNoiID nH=2"<<endl;
  tf->Draw("iHit>>hCaseIDNoiID2","(NoiseID&&nH==3)"    ,"histsame");
  cout<<"Finish Drawing pwidth All hCaseIDNoiID nH=3"<<endl;
  tf->Draw("iHit>>hCaseIDNoiID3","(NoiseID&&nH>3)"     ,"histsame");
  cout<<"Finish Drawing pwidth All hCaseIDNoiID nH>3"<<endl;
  tf->Draw("iHit>>hCaseIDMuoID5","(MuonID&&IDtight==5)","histsame");
  cout<<"Finish Drawing pwidth All MuonID IDtight==5"<<endl;
  tf->Draw("iHit>>hCaseIDMuoID4","(MuonID&&IDtight==4)","histsame");
  cout<<"Finish Drawing pwidth All MuonID IDtight==4"<<endl;
  tf->Draw("iHit>>hCaseIDMuoID3","(MuonID&&IDtight==3)","histsame");
  cout<<"Finish Drawing pwidth All MuonID IDtight==3"<<endl;
  tf->Draw("iHit>>hCaseIDMuoID2","(MuonID&&IDtight==2)","histsame");
  cout<<"Finish Drawing pwidth All MuonID IDtight==2"<<endl;
  tf->Draw("iHit>>hCaseIDEleID","(ElectronID)"         ,"histsame");
  cout<<"Finish Drawing pwidth All ElectronID"<<endl; //cout<<456<<endl;
  for(int i=1;i<3;i++){
    ccaseNum->cd(i);
    hCaseID->Draw("hist"); //cout<<459<<endl;
    for(int i1=1;i1<11;i1++) hCaseIDCases[i1]->Draw("histsame");
    hCaseID->Draw("histsame"); //cout<<461<<endl;
    LCaseNum->Draw();
  }
  ccaseNum->Print(Form("%sCaseiHitTG%0.f.pdf",DirResultPwVCase,eventgap));
  // cout<<465<<endl;
  Long64_t Num_hPwidthCases[11] ={};

  for(int i=0;i<11;i++){
    Num_hPwidthCases[i]=hCaseIDCases[i]->Integral(-1,-1);
    cout<<hCaseIDCases[i]->GetTitle()<<"\t"<<Num_hPwidthCases[i]<<endl;
    hCaseIDCases[i]->Reset();
    hCaseIDCases[i]->SetBinContent(i+1,Num_hPwidthCases[i]);
    hCaseID->GetXaxis()->SetBinLabel(i+1,Form("%d",Num_hPwidthCases[i]));
  } //cout<<473<<endl;
  hCaseID->GetXaxis()->SetRangeUser(0,11);
  hCaseID->GetXaxis()->SetLabelSize(0.04);
  TH1F *hCaseIDLG = (TH1F*) hCaseID->Clone();
  TH1F *hCaseIDA[2] = {hCaseID,hCaseIDLG};
  hCaseID  ->GetYaxis()->SetRangeUser(1,Num_hPwidthCases[0]*1.2);
  hCaseIDLG->GetYaxis()->SetRangeUser(1,Num_hPwidthCases[0]*20);
  hCaseIDLG->GetXaxis()->SetTitle("Case & total Number display");
  hCaseIDLG->GetYaxis()->SetTitle("Event Number");
  // cout<<483<<endl;
  for(int i=1;i<3;i++){
    ccaseNum->cd(i);
    hCaseIDA[i-1]->Draw("hist");
    for(int i1=1;i1<11;i1++) hCaseIDCases[i1]->Draw("histsame");
    hCaseIDA[i-1]->Draw("histsame");
    LCaseNum->Draw();
  }
  // cout<<491<<endl;
  ccaseNum->cd(2)->SetGrid(0,1);
  ccaseNum->Print(Form("%sCaseNumTG%0.f.pdf",DirResultPwVCase,eventgap));
}


void GTCaseNumAna(const char* DirOperate,const char* DirResult){

  char DirResultPwVCase[150]={};//save path graph of Pwidth Rate
  sprintf(DirResultPwVCase,"%sBoardPwVCase/",DirResult);
  if (system(Form("mkdir -p %s",DirResultPwVCase))!=0) system(Form("mkdir -p %s",DirResultPwVCase));
  double caseNum[2][11]={
    {22568997,22247727,16777216,4752971,522656,194884,67255,214346,16638,12115,39669},
    {20666423,20451886,16777216,3188545,358205,127920,43858,140860,13028, 9262,29819},
  };
  TH1F *hCaseID       = new TH1F("hCaseID"      ,"hCaseID"      ,65,0,65);
  TH1F *hCaseIDNoiID  = new TH1F("hCaseIDNoiID" ,"hCaseIDNoiID" ,65,0,65);
  TH1F *hCaseIDNoiID0 = new TH1F("hCaseIDNoiID0","hCaseIDNoiID0",65,0,65);
  TH1F *hCaseIDNoiID1 = new TH1F("hCaseIDNoiID1","hCaseIDNoiID1",65,0,65);
  TH1F *hCaseIDNoiID2 = new TH1F("hCaseIDNoiID2","hCaseIDNoiID2",65,0,65);
  TH1F *hCaseIDNoiID3 = new TH1F("hCaseIDNoiID3","hCaseIDNoiID3",65,0,65);
  TH1F *hCaseIDMuoID5 = new TH1F("hCaseIDMuoID5","hCaseIDMuoID5",65,0,65);
  TH1F *hCaseIDMuoID4 = new TH1F("hCaseIDMuoID4","hCaseIDMuoID4",65,0,65);
  TH1F *hCaseIDMuoID3 = new TH1F("hCaseIDMuoID3","hCaseIDMuoID3",65,0,65);
  TH1F *hCaseIDMuoID2 = new TH1F("hCaseIDMuoID2","hCaseIDMuoID2",65,0,65);
  TH1F *hCaseIDEleID  = new TH1F("hCaseIDEleID" ,"hCaseIDEleID" ,65,0,65);
  TH1F *hCaseIDCases[11] = {
    hCaseID,hCaseIDNoiID,hCaseIDNoiID0,hCaseIDNoiID1,hCaseIDNoiID2,hCaseIDNoiID3,hCaseIDMuoID5,hCaseIDMuoID4,hCaseIDMuoID3,hCaseIDMuoID2,hCaseIDEleID
  };
  TLegend *LpwWN = new TLegend(0.20,0.66,0.58,0.90);
  TLegend *LpwNN = new TLegend(0.58,0.70,0.96,0.90);
  char WNNN[2][3]={"WN","NN"};
  setLegendDefault(LpwWN,1);
  setLegendDefault(LpwNN,1);
  LpwWN->AddEntry(hCaseID      ,"All hit"              ,"f");   
  LpwWN->AddEntry(hCaseIDNoiID ,"Noise hit"            ,"f");   
  LpwWN->AddEntry(hCaseIDNoiID0,"Noise hit(nH=1)"      ,"f");   
  LpwWN->AddEntry(hCaseIDNoiID1,"Noise hit(nH=2)"      ,"f");   
  LpwWN->AddEntry(hCaseIDNoiID2,"Noise hit(nH=3)"      ,"f");   
  LpwWN->AddEntry(hCaseIDNoiID3,"Noise hit(nH>3)"      ,"f");   
  //cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
  LpwNN->AddEntry(hCaseIDMuoID5,"Muon hit(4M+0N)"      ,"f");   
  LpwNN->AddEntry(hCaseIDMuoID4,"Muon hit(4M+1N|3M)"   ,"f");   
  LpwNN->AddEntry(hCaseIDMuoID3,"Muon hit(4M+2N|3M+1N)","f");   
  LpwNN->AddEntry(hCaseIDMuoID2,"Muon hit(4M+3N|3M+2N)","f");   
  LpwNN->AddEntry(hCaseIDEleID ,"Electron hit(>4)"     ,"f");   
  TCanvas *ccaseNum = new TCanvas("ccaseNum","",3840,2160);
  ccaseNum->cd();

      
  Long64_t Num_hPwidthCases[11] ={};

  for(int i=0;i<11;i++){
    hCaseIDCases[i]->SetLineColor(colorArr[i]);
    cout<<Form("%.f/%.f",caseNum[0][i],caseNum[1][i])<<endl;
    hCaseIDCases[i]->SetBinContent(i+1,caseNum[0][i]/caseNum[1][i]);
    hCaseID->GetXaxis()->SetBinLabel(i+1,Form("#frac{%.f}{%.f}",caseNum[0][i],caseNum[1][i]));
    hCaseID->GetXaxis()->SetTitle("#scale[0.8]{Case Number Display (#scale[0.6]{#frac{#Deltatcnt=140 }{#Deltatcnt=1000}})}");
    hCaseID->GetYaxis()->SetTitle("Case Number Ratio");
  }
  hCaseID->GetXaxis()->SetRangeUser(0,12);
  hCaseID->GetYaxis()->SetRangeUser(0.6,2.2);
  hCaseID->GetXaxis()->SetLabelSize(0.04);
  int gx[2]={-5,20},gy[2]={1,1};
  TGraph *g1 = new TGraph(2,gx,gy);
  
  hCaseIDCases[0]->Draw("hist");
  for(int i1=1;i1<11;i1++) hCaseIDCases[i1]->Draw("histsame");
  hCaseIDCases[0]->Draw("histsame");
  g1->SetLineWidth(0);
  g1->SetLineStyle(3);
  LpwWN->Draw();
  LpwNN->Draw();
  g1->Draw("lsame");
  ccaseNum->Print(Form("%sNumRatio140VS1000.pdf",DirResultPwVCase));

}

void MuonElectronIDAna( 
  const bool testMode, const bool rootFileMode, const int indexi=28, const int indexf=29
){
  char TestMarker[10]={};
  if(testMode) sprintf(TestMarker,Form("%s","TST_"));
  char DirResultPwVCase[150]={};//save path graph of Pwidth Rate
  sprintf(DirResultPwVCase,Form("%s%sBoardPwVCase/",DirResult,TestMarker));
  if (system(Form("mkdir -p %s",DirResultPwVCase))!=0) system(Form("mkdir -p %s",DirResultPwVCase));
  
  double eventGap[46]={};
  for(int i0=indexi;i0<indexf;i0++){
    eventGap[i0] = (1.0*eventGapTcnt[i0])/2560000000.;
    char RFNStr[200];
    sprintf(RFNStr,"%d_%s_nHTH%dL%d",eventGapTcnt[i0],TimeStr,TriggerS,TriggerL);
    if(rootFileMode) MuEleIDMutiThread(RFNStr,TestMarker);
    
    MuonEleIDAna( RFNStr, TestMarker, eventGap[i0]);
    IDCaseNumAna( RFNStr, TestMarker, eventGap[i0]);
    cout<<"Finish MuonElectronIDAna @ eventGap["<<i0<<"]: "<<eventGap[i0]<<endl;
  }
}

void MuonElectronIDAna(const bool*OperMode, const int indexi=28, const int indexf=29){
  bool testMode = OperMode[0];
  bool rootFileMode = OperMode[4];
  // bool testMode = 1;
  // bool rootFileMode = 0;
  MuonElectronIDAna(testMode,rootFileMode,indexi,indexf);
}