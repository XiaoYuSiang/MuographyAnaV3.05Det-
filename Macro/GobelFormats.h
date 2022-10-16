#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TFile.h>
#include <TTree.h>
#include <map>

using namespace std;
namespace MuographGobelForm{//@@ADD read.h at IMF
  class scintillator{
  public:
    char  name[30];
    int   GeneralID, boardID, channelID;
    int   iX,  iY,  iZ;
    int   BiX, BiY, BiZ;
    float pX,  pY,  pZ;
    float sX,  sY,  sZ;
    bool  face;//true = face outside, false = face inside
    float Threshold;
    float Voltage;
    float PwWei;//true = face outside, false = face inside
    
    scintillator(const char names[]="default", double GID=0,double BID=0,double CID=0){
      SetName(names);
      SetID(GID, BID, CID);
      SetIndex();
      SetBIndex();
      SetPosition();
    }
    virtual ~scintillator(){}
    
    void SetName(const char *names){
      sprintf(name,names);
    }
    char *GetName(){
      return name;
    }
    void SetID(int GID=0, int BID=0, int CID=0){
      GeneralID = GID;
      boardID   = BID;
      channelID = CID;
    }
    void SetIndex(int ix=0, int iy=0, int iz=0){
      iX = ix;
      iY = iy;
      iZ = iz;
    }
    void SetBIndex(int bix=0, int biy=0, int biz=0){
      BiX = bix;
      BiY = biy;
      BiZ = biz;
    }
    void SetPosition(int px=0, int py=0, int pz=0){
      pX = px;
      pY = py;
      pZ = pz;
    }
  private:
  protected:
    void Sci_CoutNotation(){
      cout<<"#Name\t\t\tGID\tBID\tCID\tiX\tiY\tiZ\tpX\tpY\tpZ\tface\tPwWei\n";
    }
    void Sci_CoutPrint(){
      cout<<name<<"\t"<<GeneralID<<"\t"<<boardID<<"\t"<<channelID<<"\t"<<iX<<"\t"<<iY<<"\t"<<iZ<<"\t"<<pX<<"\t"<<pY<<"\t"<<pZ<<"\t"<<face<<"\t"<<PwWei<<endl;
    }
    void Sci_TxtNotation(ofstream &out){
      out<<"#GID\tBID\tCID\tiX\tiY\tiZ\tpX\tpY\tpZ\tface\tPwWei"<<endl;
    }
    void Sci_CsvNotation(ofstream &out){
      out<<"#GID,BID,CID,iX,iY,iZ,pX,pY,pZ,face,PwWei"<<endl;
    }
    void Sci_TxtPrint(ofstream &out){
      out<<GeneralID<<"\t"<<boardID<<"\t"<<channelID<<"\t"<<iX<<"\t"<<iY<<"\t"<<iZ<<"\t"<<pX<<"\t"<<pY<<"\t"<<pZ<<"\t"<<face<<endl;
    }
    void Sci_CsvPrint(ofstream &out){
      out<<GeneralID<<","<<boardID<<","<<channelID<<","<<iX<<","<<iY<<","<<iZ<<","<<pX<<","<<pY<<","<<pZ<<","<<face<<","<<PwWei<<endl;
    }
  };
  
  
  
  class Detector{
  public:
    char  name[30];
    char  Version[10];
    int   DetectorID;
    int   NumLY, NumBD, NumCh;
    int   NumnX, NumnY, NumnZ;
    int   NumBX, NumBY, NumBZ;
    int   NSci;
    float DetPx, DetPy, DetPz;
    float DetTh, DetPh;
    scintillator *Scintillators[];
    //double 
    //ex: Shimen 24.839314, 121.271310

    Detector(const char names[]="default", const char Ver[]="V2.0", const double DID=0){
      SetName(names);
      SetVersion(Ver);
      Initialize(DID);
    }
    virtual ~Detector(){}
    
    void SetName(const char *names){
      sprintf(name,names);
    }
    char *GetName(){
      return this->name;
    }
    void SetVersion(const char Ver[]="V2.0"){
      sprintf(Version,Ver);
    }
    void SetID(const int DID=0){
      DetectorID = DID;
    }
    

    void Initialize(const double DID=0){
      SetID(DID);

      NSci = 0;
    }
    
    void Fill(const scintillator *TmpSci){
      Scintillators[NSci] = (scintillator*) TmpSci;
      NSci++;
    }

  private:
  protected:
 
  };
  
  
  class Experiment{
  public:
    char  name[30];
    char  site[10];
    int   ExperimentID, NDet;
    float Latitude, Longitude; 
    Long64_t  StartUTime, EndUTime;
    Detector *Detectors[];
    Experiment(const char names[]="IndoorTest", const float Lati = 24.971510, const float Long = 121.192117, const char AddSiteName[]="S4_509"){
      SetName(names);
      Initialize();
      SetSite(AddSiteName);
      SetPosition(Lati,Long);
    }
    Experiment(const char names[]="IndoorTest", const char SiteName[]="S4_509"){
      Initialize();
      SetName(names);
      SetSite(SiteName);
      // FindPos(site);
    }
    virtual ~Experiment(){}
    
    void SetName(const char *names){
      sprintf(name,names);
    }
    char *GetName(){
      return name;
    }
    void SetPosition( const float Lati = 24.971510, const float Long = 121.192117 ){
      Latitude  = Lati;
      Longitude = Long;
    }
    void SetID(int EID=0){
      ExperimentID = EID;
    }
    int GetID(){
      return ExperimentID;
    }
    
    void SetSite(const char SiteName[]="S4_509"){
      sprintf(site,SiteName);
    }
    string GetSite(){
      return site;
    }
    // void FindPos(const char SiteName[]="S4_509"){
      // for(int iCaseName=0;iCaseName<4;iCaseName++){
        // if(strcmp(SiteName,Sites[iCaseName])==0){
          // Latitude  = Latitudes [iCaseName];
          // Longitude = Longitudes[iCaseName];
          // break;
        // }
      // }
    // }
    //Move to LoadODET.h
    void SetUTRange(const Long64_t UTS=0, const Long64_t UTE=0){
      
      StartUTime   = UTS;
      EndUTime     = UTE;
    }
    void Initialize(){
      ExperimentID = 0;
      SetUTRange();
      NDet = 0;
    }
    void Fill(const Detector *TmpDet){
      Detectors[NDet] = (Detector*) TmpDet;
      NDet++;
    }
      
  private:
  protected:
    /*
    void Sci_CoutNotation(){
      cout<<"#Name\t\t\tGID\tBID\tCID\tiX\tiY\tiZ\tpX\tpY\tpZ\tface\tPwWei\n";
    }
    void Sci_CoutPrint(){
      cout<<name<<"\t"<<GeneralID<<"\t"<<boardID<<"\t"<<channelID<<"\t"<<iX<<"\t"<<iY<<"\t"<<iZ<<"\t"<<pX<<"\t"<<pY<<"\t"<<pZ<<"\t"<<face<<"\t"<<PwWei<<endl;
    }
    void Sci_TxtNotation(ofstream &out){
      out<<"#GID\tBID\tCID\tiX\tiY\tiZ\tpX\tpY\tpZ\tface\tPwWei"<<endl;
    }
    void Sci_CsvNotation(ofstream &out){
      out<<"#GID,BID,CID,iX,iY,iZ,pX,pY,pZ,face,PwWei"<<endl;
    }
    void Sci_TxtPrint(ofstream &out){
      out<<GeneralID<<"\t"<<boardID<<"\t"<<channelID<<"\t"<<iX<<"\t"<<iY<<"\t"<<iZ<<"\t"<<pX<<"\t"<<pY<<"\t"<<pZ<<"\t"<<face<<endl;
    }
    void Sci_CsvPrint(ofstream &out){
      out<<GeneralID<<","<<boardID<<","<<channelID<<","<<iX<<","<<iY<<","<<iZ<<","<<pX<<","<<pY<<","<<pZ<<","<<face<<","<<PwWei<<endl;
    }
    */
  };


  class RunData{
  public:
    char     RunName[7];
    int      RunID;
    int      StartDay, StaY, StaM, StaD;
    int      EndDay  , EndY, EndM, EndD;
    float    DPhi;
    float    DTheta;
    int      StartUT;
    int      EndUT;
    
    RunData(){
      SetIni();
    }

    ~RunData(){};
    
    void SetIni(){
      RunName[0] = '-';
      StartDay = -1;
      EndDay   = -1;
      StartUT = -1;
      EndUT   = -1;
      DPhi    =  0;
      DTheta  =  0; 
    }
    
    void sortRunInf(){
      sscanf(RunName,"%*3s%d",&RunID);
      sscanf(Form("%d",StartDay),"%4d%2d%2d",&StaY,&StaM,&StaD);
      sscanf(Form("%d",EndDay  ),"%4d%2d%2d",&EndY,&EndM,&EndD);
      StartUT = YMDHmSZtoUT(StaY,StaM,StaD);
      EndUT   = YMDHmSZtoUT(EndY,EndM,EndD);
    }
    
    
  private:

    
  };


};
void GobelFormats(){}