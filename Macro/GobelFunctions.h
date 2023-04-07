#include <typeinfo>
#include <algorithm>
#include <TStyle.h>
#include <TF1.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TText.h>
#include <TPDF.h>
#include "TH2.h"
#include "TProfile.h"
#include <TProfile2D.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TGaxis.h>
#include <TChain.h>
#include <TObject.h>
#include <TStopwatch.h>
#include <TSystem.h>
#ifndef FATAL
#define FATAL(msg) do { fprintf(stderr, "FATAL: %s\n", msg); gSystem->Exit(1); } while (0)
#endif
using namespace MuographAnaVariable;
namespace MuographGobelFuns{
  const Double_t TcntOneSec  = 2.56E+9;
  
  int BDcheck(const int b){
    int z = -1;
    for(int i=0;i<NumBD;i++){
      if( b == BD[i] ){
        z = i;
        break;
      } 
    }if(z==-1){
      cout<<"Warning: GobelFun.h: Please Enter the true value b into the int BDcheck(int b)!!!   ";
      cout<<"Now the Enter value b is:  "<< b <<endl;
      //throw;
    } 
    return z;
  }
  int BDCheck(const int b){ return BDcheck(b);}
  int CHcheck(const int c){
    int z = -1;
    if(c>NumCh-1||c<0){
      cout<<"Warning: GobelFun.h: Please Enter the true value c into the int CHcheck(int c)!!!   ";
      cout<<"Now the Enter value c is:  "<< c <<endl;
      //throw;
    }else{
      z = c;
    }
    return z;
  }
  char  MonthC[13][5]={
    "-","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
  };
  
  int BCIDCovGID(int BID, int CID){
    int TmpBID = BDcheck(BID);
    if(TmpBID==-1) return -1;
    return TmpBID*NumCh+CID;
  }
  
  int BIDOfGID(int GID){
    return BDcheck(GID/NumCh);
  }
  int CIDOfGID(int GID){
    return GID%NumCh;
  }
  void BCIDOfGID(int GID, int &BID, int &CID){
    BID = BIDOfGID(GID);
    CID = CIDOfGID(GID);
  }
  
  vector<string> RawRootList(const char * DirOperate){
    vector<string> rootFilePathList;
    char TmpFileRoute[200];
    char rootFileList[200];
    sprintf(rootFileList, "%sDataRootNameAna.dat",DirOperate);
    // cout<<rootFileList<<endl;
    ifstream in(rootFileList);
    for(int i= 0; in>>TmpFileRoute ; i++){
      rootFilePathList.push_back(TmpFileRoute);
      // cout<<rootFilePathList[i]<<endl;
    }
    return rootFilePathList;
  }
  
  vector<string> HKRootList(const char * DirOperate){
    vector<string> HKFilePathList;
    char TmpFileRoute[200];
    char HKFileList[200];
    sprintf(HKFileList, "%sHKTxtNameAna.dat",DirOperate);
    // cout<<HKFileList<<endl;
    ifstream in(HKFileList);
    for(int i= 0; in>>TmpFileRoute ; i++){
      HKFilePathList.push_back(TmpFileRoute);
      //cout<<HKFilePathList[i]<<endl;
    }
    return HKFilePathList;
  }
  
  void FastStyleStats(float boundaryX,float boundaryY){
    gStyle-> SetStatX(boundaryX);
    gStyle-> SetStatY(boundaryY);
    gStyle-> SetStatW(0.2);
    gStyle-> SetStatH(0.1); 
  }
  
  float MaxSetUserRatio(const float M, const float R, const int ord=1){
    return float(int(M*pow(10,ord)*R)+pow(10,ord-1))/float(pow(10,ord));
  }
  float MinSetUserRatio(const float m, const float R, const int ord=1){
    return float(int(m*pow(10,ord)*R)-pow(10,ord-1))/float(pow(10,ord));
  }
  
  const char VModeRootOption[6][10]={
    "ROOT","root","Root",".root","r","R"
  };
  const char VModeTxtOption[6][10]={
    "TXT","txt","Txt",".txt","t","T"
  };
  const char VModeCsvOption[6][10]={
    "CSV","csv","Csv",".csv","c","C"
  };
  const char VModePrintOption[6][10]={
    "PRI","pri","Pri","cout","p","P"
  };
  
  char FileTypeCheck(const char* FileMode){
    char Tmp_Mode = 'R';
    for(int i=0 ; i<6 ; i++){
      if(strcmp(FileMode,VModeRootOption[i])==0) {
        break;
      }else if(strcmp(FileMode,VModeTxtOption[i])==0) {
        Tmp_Mode = 'T';
        break;
      }else if(strcmp(FileMode,VModeCsvOption[i])==0) {
        Tmp_Mode = 'C';
        break;
      }else if(strcmp(FileMode,VModePrintOption[i])==0) {
        Tmp_Mode = 'P';
        break;
      }
    }
    return Tmp_Mode;
  }
  
  bool DetVerCheck(char *version, const char *DetectorVersion = "v03.05") {
    // cout<<DirIMFdat<<endl;

    const char V2OptionName  [20][10]={
      "V2.0","v2.0","V2","v2","V2.","v2.","V02.00","v02.00","V2.00","v2.00","v2_00","V2_00","v2_0","V2_0","v02","V02_0","v02_0","V02_0","v02_00","V02_00"
    };
    const char V300OptionName[16][10]={
      "V3.00","v3.00","V03.00","v03.00","V3.0","v3.0","V3","v3","V3_00","v3_00","V03_00","v03_00","V3_0","v3_0","V03_0","v03_0",
    };
    const char V305OptionName[8][10]={
      "V3.05","v3.05","V03.05","v03.05","V3_05","v3_05","V03_05","v03_05"
    };
    
    for(int iV2Ns=0 ; iV2Ns<20 ; iV2Ns++){
      if(strcmp(DetectorVersion,V2OptionName[iV2Ns])==0) {
        sprintf(version,"V02_00");
        break;
      }
    }
    for(int iV3Ns=0 ; iV3Ns<16; iV3Ns++){
      if(strcmp(DetectorVersion,V300OptionName[iV3Ns])==0) {
        sprintf(version,"V03_00");
        break;
      }
    }
    for(int iV305Ns=0 ; iV305Ns<8 ; iV305Ns++){
      if(strcmp(DetectorVersion,V305OptionName[iV305Ns])==0) {
        sprintf(version,"V03_05");
        break;
      }
    }
    if(strcmp(version,"V02_00")==0){
      return true;
    }else if(strcmp(version,"V03_00")==0){
      return true;
    }else if(strcmp(version,"V03_05")==0){
      return true;
    }else{
      cout<<endl;
      cout<<"Error: GobelFunction.C:: bool DetVerCheck(char *version, c.char *DetectorVersion = \"v03.05\") need a correct version number!!!"<<endl;
      cout<<"Now, Enter version number is:    "<<DetectorVersion<<"    can not be Identified by program"<<endl;
      cout<<"Please Enter Like this List:    "<<endl;
      cout<<"-------List of V02_00:-------"<<endl;
      for(int iV2Ns=0 ; iV2Ns<20 ; iV2Ns++){
        cout<<Form("%10.10s    =    %10.10s",V2OptionName[iV2Ns],"V02_00")<<endl;
      }
      cout<<"-------List of V03_00:-------"<<endl;
      for(int iV300Ns=0 ; iV300Ns<16 ; iV300Ns++){
        cout<<Form("%10.10s    =    %10.10s",V300OptionName[iV300Ns],"V03_00")<<endl;
      }
      cout<<"-------List of V03_05:-------"<<endl;
      for(int iV305Ns=0 ; iV305Ns<8 ; iV305Ns++){
        cout<<Form("%10.10s    =    %10.10s",V305OptionName[iV305Ns],"V03_05")<<endl;
      }
      cout<<"Or contact the author to update the detector version table."<<endl;
      
      cout<<endl;
      
      throw;
      return false;
    }
  }
  
  bool ExSiteCheck(char *retunSite, const char *InputSite = "S4_509",const char Opt='q') {
    // cout<<DirIMFdat<<endl;
    const char S459OptionName[33][10]={
      "S4-509" ,"S4_509" ,"S4 509" ,"S4-5f","S4-5F","S4_5f","S4_5F" ,"S4 5F" ,"S4 5f" ,"5_f",
      "s4-509" ,"s4_509" ,"s4 509" ,"s4-5f","s4-5F","s4_5f","s4_5F" ,"s4 5F" ,"s4 5f" ,"5_F",
      "S4509"  ,"S4509"  ,"s4509"  ,"s4509","5F"   ,"5f"   ,"5 F"   ,"5 f"   ,"5-f"   ,"5-F",
      "509"    ,"S459"   ,"s459"
    };
    const char ShMeOptionName[22][10]={
      "Shi_men","Shi_men","shi_men","ShiMen","Shimen","shimen","Shi Men","Shi men","shi men",
      "Shi-men","Shi-men","shi-men","S.M."  ,"SM"    ,"Sm"    ,"sm"     ,"S_M"    ,"S_m"    ,
      "s_m"    ,"S-M"    ,"S-m"    ,"s-m"
    };
    const char S411OptionName[36][10]={
      "S4-11"    ,"S4_11"    ,"S4 11"    ,"S4-11f"   ,"S4-11F"   ,"S4_11f"   ,"S4_11F"   ,"S4 11F"   ,"S4 11f" ,
      "s4-11"    ,"s4_11"    ,"s4 11"    ,"s4-11f"   ,"s4-11F"   ,"s4_11f"   ,"s4_11F"   ,"s4 11F"   ,"s4 11f" ,
      "S411F"    ,"S411f"    ,"s411F"    ,"s411f"    ,"top"      ,"Top"      ,"Top Floor","Top floor","top floor",
      "Top_Floor","Top_floor","top_floor","Top-Floor","Top-floor","top-floor","TopFloor" ,"Topfloor" ,"topfloor" 
    };
    const char DaXiOptionName[44][10]={
      "Da_Xi"  ,"Da_xi"  ,"da_xi"  ,"DaXi"  ,"Daxi"  ,"daxi"  ,"Da Xi"  ,"Da xi"  ,"da xi"  ,
      "Da_Shi" ,"Da_shi" ,"da_shi" ,"DaShi" ,"Dashi" ,"dashi" ,"Da Shi" ,"Da shi" ,"da shi" ,
      "D.X."   ,"D.S."   ,"DX"     ,"DS"    ,"Dx"    ,"Ds"    ,"dx"     ,"ds"     ,"D_X"    ,
      "D_S"    ,"D_x"    ,"D_s"    ,"d_x"   ,"d_s"   ,"Da-Xi" ,"Da-xi"  ,"da-xi"  ,"Da-Shi" ,
      "Da-shi" ,"da-shi" ,"D-X"    ,"D-S"   ,"D-x"   ,"D-s"   ,"d-x"    ,"d-s"
    };
    int NumCase[4] = {33,22,36,44};
    
    
    for(int iCaseName=0 ; iCaseName<NumCase[0] ; iCaseName++){
      if(strcmp(InputSite,S459OptionName[iCaseName])==0) {
        sprintf(retunSite,Sites[0]);
        return true;
      }
    }
    for(int iCaseName=0 ; iCaseName<NumCase[1] ; iCaseName++){
      if(strcmp(InputSite,ShMeOptionName[iCaseName])==0) {
        sprintf(retunSite,Sites[1]);
        return true;
      }
    }
    for(int iCaseName=0 ; iCaseName<NumCase[2] ; iCaseName++){
      if(strcmp(InputSite,S411OptionName[iCaseName])==0) {
        sprintf(retunSite,Sites[2]);
        return true;
      }
    }
    for(int iCaseName=0 ; iCaseName<NumCase[3] ; iCaseName++){
      if(strcmp(InputSite,DaXiOptionName[iCaseName])==0) {
        sprintf(retunSite,Sites[3]);
        return true;
      }
    }
    
    if(Opt!='q'&&Opt!='Q'){
      char ExceptionCase[4][60]={
        "-------List of S4_509:-------",
        "-------List of Shimen:-------",
        "-------List of S4_11F:-------",
        "-------List of  DaXi :-------"
      };
      cout<<endl;
      cout<<"Error: GobelFunction.C:: bool ExSiteCheck(char *retunSite, const char *InputSite = \"v03.05\",const char Opt='q') need a correct Site name!!!\n";
      cout<<"Now, Enter Site name is:    "<<InputSite<<"  can't be Identified by program"<<endl;
      cout<<"Please Enter Like this List:    "<<endl;
      for(int iCase=0 ; iCase<4 ; iCase++){
        cout<<ExceptionCase[iCase]<<endl;
        for(int iCaseName=0 ; iCaseName<NumCase[iCase] ; iCaseName++){
          if(iCaseName%6 ==0) cout<<endl;
          if(iCase==0) cout<<Form("%10.10s",S459OptionName[iCaseName]);
          if(iCase==1) cout<<Form("%10.10s",ShMeOptionName[iCaseName]);
          if(iCase==2) cout<<Form("%10.10s",S411OptionName[iCaseName]);
          if(iCase==3) cout<<Form("%10.10s",DaXiOptionName[iCaseName]);
        }
      }
      cout<<"Or contact the author to update the detector version table."<<endl;
      
      cout<<endl;
    }
    sprintf(retunSite,"Null Site");
    return false;
  }
  
  void SetNXMSciArr(TH2F *TH2FOBJ, const int NX,const int MY,const char *Name = "TH2FOBJ",const char *Title = "Title"){
    *TH2FOBJ = TH2F(Name,Title,NX,0,NX,MY,0,MY);
    TH2FOBJ->SetStats(0);
    setTitle(TH2FOBJ,"iX","iY","");
    TH2FOBJ->GetXaxis()->CenterTitle(true);
    TH2FOBJ->GetYaxis()->CenterTitle(true);
    TH2FOBJ->GetXaxis()->CenterLabels(true);
    TH2FOBJ->GetYaxis()->CenterLabels(true);
    TH2FOBJ->GetXaxis()->SetNdivisions(NX);
    TH2FOBJ->GetYaxis()->SetNdivisions(MY);
    TH2FOBJ->GetXaxis()->SetTitleOffset(1.0);
    TH2FOBJ->GetYaxis()->SetTitleOffset(1.0);
    TH2FOBJ->GetXaxis()->SetLabelSize(0.05);
    TH2FOBJ->GetYaxis()->SetLabelSize(0.05);
    TH2FOBJ->GetXaxis()->SetTitleSize(0.05);
    TH2FOBJ->GetYaxis()->SetTitleSize(0.05);
  }
  
  int UTtotHour(const Long64_t UT, const int TZ){
    return (UT/3600+TZ)%24;
  }
  
  void SortNegHourRange( vector<Long64_t> &VUTS, vector<Long64_t> &VUTF){
    vector<Long64_t> RetVUTS, RetVUTF;
    RetVUTS.push_back(VUTS[0]);
    // cout<<RetVUTS[0]<<"\t";
    Long64_t TmpF = 0;
    TmpF = VUTF[0];
    for(int iRange = 1 ; iRange<int (VUTS.size())-1 ; iRange++){
      if( VUTS[iRange] == TmpF ){
        TmpF = VUTF[iRange];
        // cout<<TmpF<<"\t";
      }else{
        // cout<<TmpF<<"\n";
        RetVUTF.push_back(TmpF);
        RetVUTS.push_back(VUTS[iRange]);
        TmpF = VUTF[iRange];
        // cout<<VUTS[iRange]<<"\t";
      }
    }
    // cout<<VUTF[int (VUTF.size())-1]<<"\n";
    RetVUTF.push_back(VUTF[int (VUTF.size())-1]);
    VUTS.clear();
    VUTF.clear();
    for(int iRange = 0 ; iRange<int (RetVUTS.size()) ; iRange++){
      VUTS.push_back(RetVUTS[iRange]);
      VUTF.push_back(RetVUTF[iRange]);
    }
    // cout<<"Finish Sort"<<endl;
  }
  
  void GetNegHourRange(vector<Long64_t> &VUTS, vector<Long64_t> &VUTF, const char * DirOperate){
    ifstream inPR(Form("%sDayEffProblemRange.dat",DirOperate));
    Long64_t TmpUTS = 0 ,TmpUTF = 0;
    while(inPR>>TmpUTS>>TmpUTF){
      VUTS.push_back(TmpUTS);
      VUTF.push_back(TmpUTF);
    }
    if(VUTS.size()!=VUTF.size()){
      cout<<"Error! GobelFunction.h::GetNegHourRange() VUTS.size()!=VUTF.size()"<<endl;
      throw;
    }
    if (VUTS.size()!=0)  SortNegHourRange(VUTS,VUTF);
    /* 
    for(int iRange = 0 ; iRange<int (VUTS.size()) ; iRange++){
      cout<<VUTS[iRange]<<"\t"<<VUTF[iRange]<<endl;
    } 
    */
  }
  
  bool InNegHourRange(Long64_t UT, const vector<Long64_t> VUTS, const vector<Long64_t> VUTF){
    for(int iRange = 0 ; iRange<int (VUTS.size()) ; iRange++){
      // cout<<"if("<<UT<<">"<<VUTS[iRange]<<"&&"<<UT<<"<"<<VUTF[iRange]<<")= "<<bool(UT>VUTS[iRange]&&UT<VUTF[iRange])<<endl;
      if(UT>VUTS[iRange]&&UT<VUTF[iRange]) return true;
      
    }
    return false;
  }
  
  string UTNotInNegHourRange(const vector<Long64_t> VUTS, const vector<Long64_t> VUTF){
    string str;
    str="(";
    if(int(VUTS.size())==0) str+="1)"; 
    else{
      for(int iRange = 0 ; iRange<int (VUTS.size())-1 ; iRange++){ 
        str+=Form("(unixtime<%d||unixtime>%d)&&",VUTS[iRange],VUTF[iRange]);
      }
      str+=Form("(unixtime<%d||unixtime>%d))",VUTS[VUTS.size()-1],VUTF[VUTF.size()-1]);
    }
    
    return str;
  }
  
  Long64_t FileSize(const char *CheckFileName, const char OPT ='-'){
    ifstream fin(CheckFileName, ifstream::in | ifstream::binary);
    fin.seekg(0, ios::end);
    Long64_t length = fin.tellg();
    fin.close();
    if(OPT!='Q'&&OPT!='q') cout << length << " bytes." << endl;
    return length;
  }
  
  void SetTransparentPadcd(TPad &P){
    P.SetFillStyle(4000); //will be transparent
    P.SetFrameFillStyle(4000);
    P.SetGrid(1,1);
    P.Draw();
    P.cd();
  }
  void FindStartProduct(
    const char *path_product, const char *version , 
    int &iRunStart,  const char *TillfileForm = "ODetCh")
  {
    char tmpfile[250];
    while(true){
      sprintf(tmpfile,"%s%s%s_Till_Run%02.f.root",path_product,TillfileForm,version,iRunStart*1.);
      cout<<"Checking: "<<tmpfile<<endl;
      Long64_t SizeOfFile = FileSize(tmpfile,'q');
      if(SizeOfFile<5120) break;
      iRunStart++;
    }
  }



  void FindAllTillFile(
    const char *dataPath_RTill, const char *version,
    int &iRunStart, int &iRunFinal,
    const char *TillfileForm = "Setup_Till_Run")
  {
    cout<<"dataPath_RTill: "<<dataPath_RTill<<"\n version: "<<version<<"\t iRunStart&Final: "<<iRunStart<<"\t"<<iRunFinal<<endl;
    const int TriggerOfRunINum = 50;
    iRunStart = 0;
    char tmpfile[250];
    while(true){
      sprintf(tmpfile,"%s%s%02.f.root",dataPath_RTill,TillfileForm,iRunStart*1.);
      cout<<"Checking: "<<tmpfile<<endl;
      Long64_t SizeOfFile = FileSize(tmpfile,'q');
      if(SizeOfFile>5120) break;
      iRunStart++;
      if(iRunStart>TriggerOfRunINum){
        cout<<"Error: iRunStart of run data files is too large!!!"<<endl;
        cout<<"Identify this case to be \"No Run data files\"."<<endl;
        cout<<"Stop the program, if case is really > TriggerOfRunINum,"<<endl;
        cout<<"  please adjust the ODectImfAna.C::FindAllTillFile()::TriggerOfRunINum = ?."<<endl;
        throw;
      }
    }
    iRunFinal = iRunStart+1;
    while(true){
      sprintf(tmpfile,"%s%s%02.f.root",dataPath_RTill,TillfileForm,iRunFinal*1.);
      cout<<"Checking: "<<tmpfile<<endl;
      Long64_t SizeOfFile = FileSize(tmpfile,'q');
      if(SizeOfFile<5120) break;
      iRunFinal++;
    }
  }

  void GetEffExtUc(const float eff, const float statistic, float &min, float &max, float &ucl, float &uch){
    float StandErr = 1/sqrt(statistic);
    min = eff-StandErr;
    max = eff+StandErr;
    if(min<0) min = 0;
    if(max>1) max = 1;
    ucl = eff-min;
    uch = max-eff;
  }
  
  double GetTotNegTime(const char *DirOperate, const Long64_t unixtimeini, const Long64_t unixtimefin){
    double lossTime;
    vector<Long64_t> VNegUTRangeStart, VNegUTRangeFinal;
    GetNegHourRange(VNegUTRangeStart, VNegUTRangeFinal, DirOperate);
    for(Long64_t iUT = unixtimeini; iUT<unixtimefin; iUT++){
      if(InNegHourRange(iUT,VNegUTRangeStart,VNegUTRangeFinal)) lossTime+=1;
    }
    return lossTime;
  }
  



};

