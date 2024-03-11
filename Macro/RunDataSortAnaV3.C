#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <sstream>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include "/data4/YuSiang/personalLib/Style/DBMTStyle.h"
#include "AnaVariable.h"
#include "GobelFunctions.h"
#include "path_dir.h"

using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;
using namespace MuographGobelFuns;


class CRunData{
public:
  char     RunName[7];
  int      RunID;
  int      StartDay, StaY, StaM, StaD;
  int      EndDay  , EndY, EndM, EndD;
  float    DPhi;
  float    DTheta;
  char     ChNames  [NumBD][NumCh][15];
  float    Threshold[NumBD][NumCh];
  float    Voltage  [NumBD][NumCh];
  int      BID[NumBD];
  int      CID[NumCh];
  char     *TableName[11];
  int      StartUT;
  int      EndUT;
  
  CRunData(){
    SetIni();
    SetBIDCID();
  }
  CRunData(const int bid[NumBD], const int cid[NumBD]){
    SetIni();
    SetBIDCID( bid, cid );
  }
  ~CRunData(){};
  
  void SetIni(){
    RunName[0] = '-';
    StartDay = -1;
    EndDay   = -1;
    StartUT = -1;
    EndUT   = -1;
    DPhi    =  0;
    DTheta  =  0; 
    for(int ibd =0; ibd<NumBD ; ibd++ ){
      for(int ich =0; ich<NumCh ; ich++ ) ChNames[ibd][ich][0]='-';
      fill_n( Threshold[ibd], NumBD, -1);
      fill_n( Voltage  [ibd], NumBD, -1);
    }
    SetTableName();
  }
  
  void sortRunInf(){
    sscanf(RunName,"%*3s%d",&RunID);
    sscanf(Form("%d",StartDay),"%4d%2d%2d",&StaY,&StaM,&StaD);
    sscanf(Form("%d",EndDay  ),"%4d%2d%2d",&EndY,&EndM,&EndD);
    //cout<<Form("%4d\t%02d\t%02d",StaY,StaM,StaD)<<endl;
    //cout<<Form("%4d\t%02d\t%02d",EndY,EndM,EndD)<<endl;
    StartUT = YMDHmSZtoUT(StaY,StaM,StaD);//@@
    EndUT   = YMDHmSZtoUT(EndY,EndM,EndD);
    //cout<<StartUT<<"\t"<<EndUT<<endl;
  }
  
  void ShowRunNODay(){
    stringstream TmpOuput;
    GetRunNODay(TmpOuput , 'P');
    cout<<TmpOuput.str();
  }
  void ShowChNames(){ 
    ShowChInf(3);
  }
  void ShowChThreshold(){
    ShowChInf(4);
  }
  void ShowChVoltage(){
    ShowChInf(5);
  }
  void ShowRunDegs(){
    stringstream TmpOuput;
    GetRunDegs(TmpOuput , 'P');
    cout<<TmpOuput.str();
  }
  
  
  void OutRunData(ofstream &out, const char *Optype = "txt"){
    if(StartUT==-1||EndUT==-1) sortRunInf();
    stringstream TmpOuput;
    char FT = FileTypeCheck((char *)Optype);
    GetRunNODay(TmpOuput,FT);
    out<<TmpOuput.str();
    GetChInf(3,TmpOuput,FT);
    out<<TmpOuput.str();
    GetChInf(4,TmpOuput,FT);
    out<<TmpOuput.str();
    GetChInf(5,TmpOuput,FT);
    out<<TmpOuput.str();
    GetRunDegs (TmpOuput,FT);
    out<<TmpOuput.str();
    GetEndStamp(TmpOuput);
    out<<TmpOuput.str();  
  }
  
private:
  void SetBIDCID(){
    fill_n( &BID[0], NumBD, -1);
    fill_n( &CID[0], NumCh, -1);
  }
  void SetBIDCID(const int bid[NumBD], const int cid[NumBD]){
    for(int ibd =0; ibd<NumBD ; ibd++ ) BID[ibd] = bid[ibd];
    for(int ich =0; ich<NumCh ; ich++ ) CID[ich] = cid[ich];
  }
  void SetTableName(){
    TableName[0] = (char *) "#RName";
    TableName[1] = (char *) "#StaD";
    TableName[2] = (char *) "#EndD";
    TableName[3] = (char *) "#Names";
    TableName[4] = (char *) "#Thres";
    TableName[5] = (char *) "#Volta";
    TableName[6] = (char *) "#DPhi";
    TableName[7] = (char *) "#DThe";
    TableName[8] = (char *) "#StaU";
    TableName[9] = (char *) "#EndU";
    TableName[10]= (char *) "#LastLine";
  }
  
  void GetRunNODay(stringstream &TmpOuput, char Optype = 'P'){
    TmpOuput.str("");
    char delimiter[3];
    if(Optype=='P'||Optype=='T') sprintf(delimiter,"\t");
    else if(Optype=='C') sprintf(delimiter,",");
    TmpOuput<<TableName[0]<<delimiter<<RunName<<endl;
    TmpOuput<<TableName[1]<<delimiter<<StartDay<<endl;
    TmpOuput<<TableName[2]<<delimiter<<EndDay<<endl;
    TmpOuput<<TableName[8]<<delimiter<<StartUT<<endl;
    TmpOuput<<TableName[9]<<delimiter<<EndUT<<endl;
  }
  
  void GetRunDegs(stringstream &TmpOuput, char Optype = 'P'){
    TmpOuput.str("");
    char delimiter[3];
    if(Optype=='P'||Optype=='T') sprintf(delimiter,"\t");
    else if(Optype=='C') sprintf(delimiter,",");
    TmpOuput<<TableName[6]<<delimiter<<DTheta<<endl;
    TmpOuput<<TableName[7]<<delimiter<<DPhi<<endl;
  }
  
  void GetChInf(int icase, stringstream &TmpOuput, char Optype = 'P'){
    TmpOuput.str("");
    char delimiter[3];
    if(Optype=='P'||Optype=='T') sprintf(delimiter,"\t");
    else if(Optype=='C') sprintf(delimiter,",");
    if(Optype=='P')TmpOuput<<"Showing Channels ID & Board ID & "<<TableName[icase]<<endl;
    for(int ibd=-1;ibd<NumBD;ibd++){
      if(ibd==-1) TmpOuput<<TableName[icase];
      else TmpOuput<<BID[ibd];
      for(int ich=0;ich<NumCh;ich++){
        if(ibd==-1) TmpOuput<<delimiter<<CID[ich];
        else {
          switch(icase){
            case 3:
              TmpOuput<<delimiter<<ChNames[ibd][ich];
              break;
            case 4:
              TmpOuput<<delimiter<<Threshold[ibd][ich];
              break;
            case 5:
              TmpOuput<<delimiter<<Voltage[ibd][ich];
              break;
          }
          
        }
      }
      TmpOuput<<endl;
    }
  }
  
  void GetEndStamp(stringstream &TmpOuput, char Optype = 'P'){
    TmpOuput.str("");
    TmpOuput<<TableName[10];
  }
  
  void ShowChInf(int icase){
    stringstream TmpOuput;
    GetChInf(icase,TmpOuput);
    cout<<TmpOuput.str();
  }
  
  
  
};

int FindCaseIndex(string tmp){
  switch(tmp[1]) {
    case 'R':
      return 0;
    case 'S':
      if(tmp[4]=='D') return 1;
      if(tmp[4]=='U') return 8;
      else{
        cout<<"Not define #Dtax in program, line: \n  ";
        cout<<tmp<<endl;
        throw;
      }
    case 'E':
      if(tmp[4]=='D') return 2;
      if(tmp[4]=='U') return 9;
      else{
        cout<<"Not define #Endx in program, line: \n  ";
        cout<<tmp<<endl;
        throw;
      }
    case 'N':
      return 3;
    case 'T':
      return 4;
    case 'V':
      return 5;
    case 'D':
      if(tmp[2]=='P') return 6;
      if(tmp[2]=='T') return 7;
      else{
        cout<<"Not define #Dx in program, line: \n  ";
        cout<<tmp<<endl;
        throw;
      }
    case 'L':
      return 10;
    default:
      cout<<"Not define #x in program, line: \n  ";
      cout<<tmp<<endl;
      throw;
  }
}

int CalFileLine(const char* infilename, int *casePos, char OPT = 'C'){
  if(OPT!='N'&&OPT!='C'){
    cout<<"Error: int CalFileLine(C.char infilename, int *casePos, char OPT):"<<endl;
    cout<<"OPT == "<<OPT<<endl;
    cout<<"OPT need to be 'C'ommon or 'N'eglect"<<endl;
    throw;
  }
  //Initialize the case position
  for(int iCasePos = 0; iCasePos<11; iCasePos++)
    casePos[iCasePos] = -1;
  
  int lineNum = -1, emptyline = 0, NSharp = 0;
  string tmp;
  ifstream inCL(infilename);
  while(inCL.good()){
    getline(inCL, tmp);
    if(tmp=="") emptyline++;
    else{
      lineNum++;
      if(tmp[0]=='#'){
        int index = FindCaseIndex(tmp);
        casePos[index] = lineNum;
        NSharp++;
      } 
    }
  }

  inCL.close();
  if(emptyline>1){
    if(OPT=='C'){
      cout<<"Error: empty line is too much, file:\n  ";
      cout<<infilename<<endl;
      throw;
    }else if(OPT=='N'){
      cout<<"Warning: empty line is too much, file:\n  ";
      cout<<infilename<<endl;
    }
  }
  
  if(OPT=='C'){
    if(lineNum<0){
      cout<<"Error: The Input file is empty."<<endl;
      cout<<"File name is: "<<infilename<<endl;
      throw;
    }else if(lineNum==0){
      cout<<"Warning: The Input file don't have information."<<endl;
      cout<<"File name is: "<<infilename<<endl;
      // throw;
    } 
  }else if(OPT=='N'){
    if(lineNum<=0) cout<<"Warning: Problem File!: "<<infilename<<endl;
  } 
  if(OPT=='N') cout<<"find line number"<<lineNum<<endl;
  return lineNum;
}

void TakeChsName(
  const char* infilename,
  CRunData    &runData,
  int         caseposi=-1,
  int         caseposf=-1)
{
  cout<<"Reading: "<<infilename<<endl;
  ifstream in(infilename);
  int lineNum = 0, tmpPos[11] = {};
  if( caseposi==-1||caseposf==-1) lineNum = CalFileLine(infilename, tmpPos);
  else{
    for( int il = 0; il<caseposi ; il++ ){
      string tmp;
      getline(in,tmp);
    }
    lineNum = caseposf - caseposi-1;
  }
  // cout<<lineNum<<endl;
  union inputForm{
    int   Int;
    char  Cha[10];
    float Flo;
  }input;
  
  for(int iline=-1;iline<lineNum;iline++){
    int tmpBID = -1;
    if(iline!=-1){
      in>>input.Int;
      tmpBID = input.Int;
      runData.BID[BDCheck(tmpBID)] = tmpBID;
    }
    else{
      in>>input.Cha;
      // cout<<input.Cha;
    } 
    for(int ich=0;ich<NumCh;ich++){
      if(tmpBID==-1){
        in>>input.Int;
        runData.CID[ich] = input.Int;
        // cout<<"\t"<<CID[ich];
      } 
      else{
        in>>input.Cha;
        if(input.Cha[0]!='-')
          sprintf(runData.ChNames[BDCheck(tmpBID)][ich],input.Cha);
      }
    }
  }
  
  in.close();
}


/*Float Information*/
void TakeChsFInf(
  const char* infilename,
  CRunData    &runData,
  const char  OPT,
  int         caseposi=-1,
  int         caseposf=-1)
{
  if(OPT!='T'&&OPT!='V') cout<<"Error: TakeChsFInf() OPT is wrong: "<<OPT<<endl;
  cout<<"Reading: "<<infilename<<endl;
  ifstream in(infilename);
  int lineNum = 0, tmpPos[11] = {};
  if( caseposi==-1||caseposf==-1) lineNum = CalFileLine(infilename, tmpPos);
  else{
    for( int il = 0; il<caseposi ; il++ ){
      string tmp;
      getline(in,tmp);
    }
    lineNum = caseposf - caseposi-1;
  }
  float ChFInf[NumBD][NumCh];
  
  union inputForm{
    int   Int;
    char  Cha[10];
    float Flo;
  }input;
  
  if(lineNum != 0){
    
    for(int iline=-1;iline<lineNum;iline++){
      int tmpBID = -1;
      if(iline!=-1){
        in>>input.Int;
        tmpBID = input.Int;
        runData.BID[BDCheck(tmpBID)] = tmpBID;
        // cout<<tmpBID;
      }
      else{
        in>>input.Cha;
        // cout<<"B/CID";
      } 
      for(int ich=0;ich<NumCh;ich++){
        if(tmpBID==-1){
          in>>input.Int;
          runData.CID[ich] = input.Int;
          // cout<<"\t"<<CID[ich];
        } 
        else{
          in>>input.Flo;
          if(int(input.Flo+1)!=0)
            if     (OPT=='T') runData.Threshold[BDCheck(tmpBID)][ich] = input.Flo;
            else if(OPT=='V') runData.Voltage[BDCheck(tmpBID)][ich]   = input.Flo;
          // cout<<"\t"<<input.Flo;
        }
      }
      // cout<<endl;
    }
  }
  
    
  in.close();
}

 
void TakeRunNODay(
  const char* infilename,
  CRunData    &runData,
  int*        casePos)
{
  ifstream in(infilename);
  
  union inputForm{
    int   Int;
    char  Cha[10];
    float Flo;
  }input;
  
  for( int il = 0; il<casePos[0] ; il++ ){
    string tmp;
    getline(in,tmp);
  }
  
  in>>input.Cha>>input.Cha;
  sprintf(runData.RunName,input.Cha);
  
  for( int il = casePos[0]; il<casePos[1] ; il++ ){
    string tmp;
    getline(in,tmp);
  }
  
  in>>input.Cha>>input.Int;
  runData.StartDay = input.Int;
  
  for( int il = casePos[1]; il<casePos[2] ; il++ ){
    string tmp;
    getline(in,tmp);
  }
  
  in>>input.Cha>>input.Int;
  runData.EndDay = input.Int;
  
}

void TakeRunDegs(
  const char* infilename,
  CRunData    &runData,
  int*        casePos)
{
  ifstream in(infilename);
  
  union inputForm{
    int   Int;
    char  Cha[10];
    float Flo;
  }input;
  
  for( int il = 0; il<casePos[6] ; il++ ){
    string tmp;
    getline(in,tmp);
  }

  in>>input.Cha>>input.Flo;
  // cout<<input.Cha<<"\t";
  runData.DPhi = input.Flo;
  // cout<<runData.DPhi;
  
  for( int il = casePos[6]; il<casePos[7] ; il++ ){
    string tmp;
    getline(in,tmp);
  }
  
  in>>input.Cha>>input.Flo;
  // cout<<input.Cha<<"\t";
  runData.DTheta = input.Flo;
  // cout<<runData.DTheta;
  
  // cout<<"\t"<<input.Flo<<endl;
  //cout<<input.Cha<<"\t"<<runData.DPhi<<"\t"<<runData.DTheta<<endl;
}

void TakeRunSEUT(
  const char* infilename,
  CRunData    &runData,
  int*        casePos)
{
  ifstream in(infilename);
  
  union inputForm{
    int   Int;
    char  Cha[10];
    Long64_t Lon;
  }input;
  
  for( int il = 0; il<casePos[8] ; il++ ){
    string tmp;
    getline(in,tmp);
  }

  in>>input.Cha>>input.Lon;
  // cout<<input.Cha<<"\t";
  runData.StartUT = input.Lon;
  // cout<<runData.DPhi;
  
  for( int il = casePos[8]; il<casePos[9] ; il++ ){
    string tmp;
    getline(in,tmp);
  }
  
  in>>input.Cha>>input.Lon;
  // cout<<input.Cha<<"\t";
  runData.EndUT = input.Lon;
  // cout<<runData.DTheta;
  
  // cout<<"\t"<<input.Flo<<endl;
  //cout<<input.Cha<<"\t"<<runData.DPhi<<"\t"<<runData.DTheta<<endl;
}

void SaveRunData(const char *infileName, CRunData &runData,int indexOfRun){
  if(runData.StartUT==-1||runData.EndUT==-1) runData.sortRunInf();
  TFile *rotfile = new TFile(Form("%s.root",infileName),"RECREATE");
  TTree *tRun = new TTree("tRun","Run Information and scintillator parameter");
  cout<<runData.StartUT<<"\t"<<runData.EndUT<<endl;
  Int_t        GeneralID;
  Int_t        boardID;
  Int_t        channelID;
  float        Threshold_;
  float        Voltage_;
  char         ChNames_[15];
  Long64_t     UT0 = runData.StartUT, UTF = runData.EndUT;
  runData.RunID    = indexOfRun;

  tRun->Branch("RunName"     ,&runData.RunName  , "RunName/C"    );
  tRun->Branch("RID"         ,&runData.RunID    , "RID/I"        );
  tRun->Branch("DPhi"        ,&runData.DPhi     , "DPhi/F"       );
  tRun->Branch("DTheta"      ,&runData.DTheta   , "DTheta/F"     );
  tRun->Branch("Sta"         ,&runData.StartDay , "StaDay/I"     );
  tRun->Branch("End"         ,&runData.EndDay   , "EndDay/I"     );
  tRun->Branch("StaYear"     ,&runData.StaY     , "StaDay/I"     );
  tRun->Branch("EndYear"     ,&runData.EndY     , "EndDay/I"     );
  tRun->Branch("StaMon"      ,&runData.StaM     , "StaDay/I"     );
  tRun->Branch("EndMon"      ,&runData.EndM     , "EndDay/I"     );
  tRun->Branch("StaDay"      ,&runData.StaD     , "StaDay/I"     );
  tRun->Branch("EndDay"      ,&runData.EndD     , "EndDay/I"     );
  tRun->Branch("StartUT"     ,&UT0              , "StartUT/L"    );
  tRun->Branch("EndUT"       ,&UTF              , "EndUT/L"      );
  tRun->Branch("GeneralID"   ,&GeneralID        , "GeneralID/I"  );
  tRun->Branch("boardID"     ,&boardID          , "boardID/I"    );
  tRun->Branch("channelID"   ,&channelID        , "channelID/I"  );
  tRun->Branch("ChNames"     ,&ChNames_         , "ChNames[15]/C");
  tRun->Branch("Threshold"   ,&Threshold_       , "Threshold/F"  );
  tRun->Branch("Voltage"     ,&Voltage_         , "Voltage/F"    );
  
  
  
  for(int ibd=0;ibd<NumBD;ibd++){
    for(int ich=0;ich<NumCh;ich++){
      GeneralID = BCIDCovGID(BD[ibd],ich);
      boardID   = runData.BID[ibd];
      channelID = runData.CID[ich];
      sprintf(ChNames_,runData.ChNames[ibd][ich]);
      // cout<<ChNames_<<"\t"<<runData.ChNames[ibd][ich]<<endl;  
      Threshold_= runData.Threshold[ibd][ich];
      Voltage_  = runData.Voltage[ibd][ich];

      tRun->Fill();
   }
  }
  rotfile->Write();
  cout<<Form("%s.root is RECREATE.",infileName)<<endl;
  rotfile->Close();
}

void ReadRunData(const char *infileName, CRunData &runData, char OPT='-'){
  // CRunData runData = CRunData();
  int casePos[11]={};
  int Nfileline = CalFileLine(infileName, casePos);
  // for(int i = 0; i<11;i++)cout<<casePos[i]<<"\t";

  TakeRunNODay(infileName,runData,casePos);//.RunName,runData.StartDay,runData.EndDay,casePos);
  TakeChsName (infileName,runData,casePos[3],casePos[4]);
  TakeChsFInf (infileName,runData, 'T' ,casePos[4],casePos[5]);
  TakeChsFInf (infileName,runData, 'V' ,casePos[5],casePos[6]);
  TakeRunDegs (infileName,runData,casePos);//.RunName,runData.StartDay,runData.EndDay,casePos);
  TakeRunSEUT (infileName,runData,casePos);//.RunName,runData.StartDay,runData.EndDay,casePos);
  if(OPT!='q'&&OPT!='Q'){
    runData.ShowRunNODay();
    runData.ShowChNames();
    runData.ShowChThreshold();
    runData.ShowChVoltage();
    runData.ShowRunDegs();
  }
  
}

void RunFileNumCheck( int &iRunStart, int &iRunFinal ){
  for( iRunStart = 0 ; true ; iRunStart++){
    char tmpfile[250];
    sprintf(tmpfile,"%sSetup_Run%02.f.txt",DirRunData,iRunStart*1.);
    cout<<"Find 1st Run: "<<iRunStart<<"  ";
    Long64_t SizeOfFile = FileSize(tmpfile);
    if(SizeOfFile>10){
      iRunFinal--;
      break;
    } 
    if(iRunStart>MaxRun) {
      cout<<"Not find 1st RunFile.in "<<DirRunData<<endl;
      throw;
    }
  }
  for( iRunFinal = iRunStart ; true ; iRunFinal++){
    char tmpfile[250];
    sprintf(tmpfile,"%sSetup_Run%02.f.txt",DirRunData,iRunFinal*1.);
    cout<<"Find last Run: "<<iRunFinal<<"  ";
    Long64_t SizeOfFile = FileSize(tmpfile);
    if(SizeOfFile<10){
      iRunFinal--;
      break;
    } 
  }
}

int TillFileCheck(const char *dataPath, const int iRunStart, const int iRunFinal){
  int lastTillFileIndex = iRunFinal;
  char Filetype[2][4]={"txt","csv"};
  for(; lastTillFileIndex>iRunStart;lastTillFileIndex--){
    int Npass = 0;
    for(int icase = 0; icase<2; icase++){
      char tmpfile[250];
      sprintf(tmpfile,"%sSetup_Till_Run%02.f.%s",dataPath,lastTillFileIndex*1.,Filetype[icase]);
      int  casePos[11]={};
      cout<<"Checking: "<<tmpfile<<endl;
      Long64_t SizeOfFile = FileSize(tmpfile,'Q');
      //cout<<SizeOfFile<<"  "<<Filetype[icase]<<"  ";
      if(SizeOfFile<10) continue;
      CalFileLine(tmpfile, casePos,'N');
      if(casePos[6] != -1) Npass++;
      
    }
    char tmpfile[250];
    sprintf(tmpfile,"%sSetup_Till_Run%02.f.root",dataPath,lastTillFileIndex*1.);
    cout<<"Checking: "<<tmpfile<<endl;
    Long64_t SizeOfFile = FileSize(tmpfile,'Q');
    if(SizeOfFile>1000){
      TFile *_file0 = TFile::Open(tmpfile);
      TTree *tRun = (TTree*) _file0 ->Get("tRun");
      if(tRun->GetEntries()>=NumBD*NumCh) Npass++;
      delete _file0, tRun;
    }
    if(Npass==3){
      cout<<"Get the Final till_run: "<<lastTillFileIndex<<endl;
      return lastTillFileIndex;
    } else{
      cout<<"Not the Final till_run, Npass= "<<Npass<<endl;
    }
  }
}

void RunDataSortAna(){
  cout<<"Please use the last version: RunDataSortAnaV2"<<endl;
}

void RunDataSortAnaV3(const char opt = 'N'){
  int iRunStart = 0, iRunFinal = 0;
  RunFileNumCheck(iRunStart,iRunFinal);
  
  char  infileName[200], outfileName[200];
  CRunData runData = CRunData();
  
  if(opt=='R'||opt=='r'){
    system(Form("rm %s*Till*",DirRunTillfile));
    cout<<"Warning: Remove all run till files !!!!!!"<<endl;
  }
  int lastTillFileIndex = TillFileCheck(DirRunTillfile, iRunStart, iRunFinal);
  // cout<<524<<endl;
  cout<<"Finish the checking of till file, final data is run"<<lastTillFileIndex<<endl;
  
  if(lastTillFileIndex==-1){
    for(int i=iRunStart; i<=iRunFinal;i++){
      
      sprintf(infileName,"%sSetup_Run%02.f.txt",DirRunData,i*1.);
      sprintf(outfileName,"%sSetup_Till_Run%02.f",DirRunTillfile,i*1.);
      char tmpTXT[250], tmpCSV[250];
      sprintf(tmpTXT,"%s.txt",outfileName);
      sprintf(tmpCSV,"%s.csv",outfileName);
      cout<<"\nRun"<<i<<"\n\n";
      
      cout<<"Reading:  "<<infileName<<endl;
      ReadRunData(infileName, runData,'q');
      
      cout<<"Saving:  "<<outfileName<<" .txt/.csv"<<endl;
      
      ofstream outTxt(Form("%s.txt",outfileName));
      ofstream outCsv(Form("%s.csv",outfileName));
      runData.OutRunData(outTxt,(char*)"Txt");
      runData.OutRunData(outCsv,(char*)"Csv");
      SaveRunData(outfileName, runData,i*1.);
    }
  }else if(lastTillFileIndex!=iRunFinal){
    if(lastTillFileIndex!=StartSearchRun){
      sprintf(infileName,"%sSetup_Till_Run%02.f.txt",DirRunTillfile,lastTillFileIndex*1.);
      ReadRunData(infileName, runData,'q');
    }
    for(int i=lastTillFileIndex; i<=iRunFinal;i++){
      
      sprintf(infileName,"%sSetup_Run%02.f.txt",DirRunData,i*1.);
      sprintf(outfileName,"%sSetup_Till_Run%02.f",DirRunTillfile,i*1.);
      char tmpTXT[250], tmpCSV[250];
      sprintf(tmpTXT,"%s.txt",outfileName);
      sprintf(tmpCSV,"%s.csv",outfileName);
      cout<<"\nRun"<<i<<"\n\n";
      
      cout<<"Reading:  "<<infileName<<endl;
      ReadRunData(infileName, runData,'q');
      
      cout<<"Saving:  "<<outfileName<<" .txt/.csv"<<endl;
      
      ofstream outTxt(Form("%s.txt",outfileName));
      ofstream outCsv(Form("%s.csv",outfileName));
      runData.OutRunData(outTxt,(char*)"Txt");
      runData.OutRunData(outCsv,(char*)"Csv");
      SaveRunData(outfileName, runData,i*1.);
    }
  }
  else cout<<"Finish the sort of run data files"<<endl;
}