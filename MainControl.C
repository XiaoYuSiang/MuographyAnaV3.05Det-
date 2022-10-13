using namespace std;


//if the char variable is empty, then set the default;

int runOPTDetect(const string runOPT){
  int BOPT = 0;
  // cout<<8<<endl;
  TString AOPT[10] = {
    "R","C","==","-","-","-","-","-","-","-"
  };
  // cout<<13<<endl;
  for (int i=0;;i++){
    // cout<<i<<" "<<14<<endl;
    if(AOPT[i]=="-") return BOPT;
    if(int( runOPT.find(AOPT[i]))!=-1){
      BOPT |= 1UL << i;
      // cout<<i<<" "<<16<<endl;
    }
      
  } 
  return BOPT;
}

void MainControl(
  char *Ori_path_Mac, char *path_Raw, char *path_Hk, char *path_Run, char *path_Rot,
  char *path_OpR    , char *path_Mac, char *name_Raw, char *name_Hk ,const string runOPT = "R")
{
  /*The setting and define of variable are at MainControl.C: L233, void MainControl()*/
  cout<<"Enter Setting:"<<endl;
  cout<<"   Ori_path_Mac:  "<<Ori_path_Mac<<endl;
  cout<<"   path_Raw:      "<<path_Raw<<endl;
  cout<<"   path_Hk:       "<<path_Hk<<endl;
  cout<<"   path_Run:      "<<path_Run<<endl;
  cout<<"   path_Rot:      "<<path_Rot<<endl;
  cout<<"   path_OpR:      "<<path_OpR<<endl;
  cout<<"   path_Mac:      "<<path_Mac<<endl;
  cout<<"   name_Raw:      "<<name_Raw<<endl;
  cout<<"   name_Hk:       "<<name_Hk<<endl;
  
  int OPT = runOPTDetect(runOPT);
  
  
  bool testMode        = false;//pwidth ana. test mode
  bool normalizeMode   = true;
  bool NegHiRatChMode  = false;
  bool lowstatisticMode= true;
  bool rootFileMode    = true;  
  bool SETightMode     = false;
  bool PointTextMode   = true;
  bool OperMode[7]={
    testMode,         normalizeMode,    NegHiRatChMode,
    lowstatisticMode, rootFileMode,     SETightMode,
    PointTextMode,
  };
  
  bool SkipBlock  = true;//default skip the progress = true
  bool SkipDSLAna = true;//default skip the progress = true
  
  /* P.01 */
  /* Copy the AnaVariable.h and the macros into path_Mac and locate tmpdir*/
  gROOT->LoadMacro("./RenewMacros.h");
  RenewMacros(TString(path_Mac),TString(Ori_path_Mac));
  /* const path of Macro compiling */
  gSystem->SetBuildDir(TString(path_Mac)+"tmpdir", kTRUE);
  
  if(SkipBlock==false){
    /* P.02 *///-Able-
    /* Establish the Compile dir set */
    gROOT->LoadMacro(Form("%sDataBaseLocate.C+",path_Mac));
    DataBaseLocateV3( path_Raw, path_Hk, path_Run, path_Rot, path_OpR, path_Mac, name_Raw, name_Hk, '' );
    cout<<"Path of Raw data: "<<path_Raw<<endl;
    cout<<"Path of Root Dir: "<<path_OpR<<endl;
  }
  
  if(OPT>>0 == 1){
    /* P.03 *///-Able-
    gROOT->LoadMacro(Form("%sRunDataSortAna.C+",path_Mac));
    RunDataSortAna();
  }
  
  if(SkipBlock==false){
    /* P.04 *///-New-
    /* Locate the data of detector */
    gROOT->LoadMacro(Form("%sODectImfAna.C+",path_Mac));
    //void ODectImfAna()
    ODectImfAna();
  }
  
  /* File search engine */
  gROOT->LoadMacro(Form("%sDataNameAna.C+",path_Mac));
  
  if(SkipBlock==false){
    /* P.05 */
    /* Find Raw .txt data Names and path */
    int txtfnum = DataNameAnaTxt_Mu();
    //int DataNameAnaTxt_Mu(const char OPT='P') 
    cout<<"Find Raw .txt data Number:  "<<txtfnum<<endl;
  }
  
  if(SkipBlock==false){
    /* P.06 */
    /* Find HK .txt data Names and path */
    int Hktxtfnum = DataNameAnaTxt_Hk();
    //int DataNameAnaTxt_Mu(const char OPT='P') 
    cout<<"Find Hk .txt data Number:  "<<Hktxtfnum<<endl;
  }
  
  if(SkipBlock==false){
    /* P.07 *///-New-
    /* Convert File from raw .txt to be raw .root */
    gROOT->LoadMacro(Form("%sCaConvertor.C+",path_Mac));
    MuoCaConvertor();
  }
  
  if(SkipBlock==false){
    /* P.08 */
    /* Find Raw .root data Names and path */
    int rotfnum = DataNameAnaRoot_Mu('');
    //int DataNameAnaRoot_Mu(const char OPT='P') 
    cout<<"Find Raw .root data Number:  "<<rotfnum<<endl;
  }
  
  if(SkipDSLAna==false){
    /* P.09 *///-Able-
    /*Analize the Data for time start to end and save to head file*/
    gROOT->LoadMacro(Form("%sDSLAnaV2.C+",path_Mac));
    DSLAnaV2();
  }
  
  if(SkipBlock==false){
    /* P.10 *///-New-
    /* Data Collection Efficiency on every day */
    gROOT->LoadMacro(Form("%sDayEAnaV3.C+",path_Mac));
    DayEAnaV4();
  }

    /* P.11 *///-New-
    /*Convert Raw_Mu.root to be Gap*.root by tcnt cut*/
    gROOT->LoadMacro(Form("%sEventAna.C+",path_Mac));
  if(SkipBlock==false){
    //int Num_RawEVE = EventAna(33,34);
    //void EventAna(const int indexi=28, const int indexf=29, const int unixtimeini = unixtimei, const int unixtimefin = unixtimef, const char * runName="-NA-" ) {//140
    EventAna(0,50);
  }
    EventGapAna();
  
  if(SkipBlock==false){
    /* P.12 *///-New-
    /*Simpling ID of Mu and Ele to be Events*.root */
    gROOT->LoadMacro(Form("%sMuonElectronID.C+",path_Mac));
    MuoEleIDAna(OperMode, 33,34);//300
    //MuoEleIDAna(c.bool*OperMode, c.int indexi=28, c.int indexf=29)
  }
  
  if(SkipBlock==false){
    /* P.13 */
    /* Verticle Straight line Event Ana to be Track*.root */
    gROOT->LoadMacro(Form("%sVerticalTracksAna.C+",path_Mac));
    VerticalTracksAnaV2(OperMode,33,34);
    //VerticalTracksAna(c.int indexi=28, c.int indexf=29 )
  }
  
  if(SkipBlock==false){
    /* P.14 */
    /* Find real events in straight line Tracking Ana. */
    gROOT->LoadMacro(Form("%sSTRealEvAna.C+",path_Mac));
    STRealEvAna(33);
  }
  
  if(SkipBlock==false){
    /* P.15 *///-New-
    /* Pwidth analyze for function fitting and trigger ana. */
    gROOT->LoadMacro(Form("%sPwidthAna.C+",path_Mac));
    PwidthAnaV2(OperMode,33);
    //PWidthPeakAna(c.bool*OperMode)
  }
  
  if(SkipBlock==false){
    /* P.16 */
    /* Pwidth analyze for function fitting and trigger ana. */
    gROOT->LoadMacro(Form("%sPWidthPeakAna.C+",path_Mac));
    PWidthPeakAna(OperMode);
    //PWidthPeakAna(const bool*OperMode)
  }
  
  if(SkipBlock==false){
    /* P.17 *///-New--@@
    /* Straight line Tracking of Event to be ETracks*.txt */
    gROOT->LoadMacro(Form("%sTrackAna.C+",path_Mac));
    TrackAna(33,34);
    //void  FitTrackResAna(c.int indexi=28, c.int indexf=29 )
  }
  
  if(SkipBlock==false){
    /* P.18 *///-New--
    /* Straight line Tracking Result display*.txt */
    gROOT->LoadMacro(Form("%sFitTrackResAna.C+",path_Mac));
    FitTrackResAna(33);
    //void  FitTrackResAna(const int indexGap=28)
  }
  
  if(SkipBlock==false){
    /* P.19 *///-New-
    /* Pwidth analyze for function fitting and trigger ana. */
    gROOT->LoadMacro(Form("%sPwidthAna.C+",path_Mac));
    PwidthAnaV2P(OperMode,33);
  }
  
  if(SkipBlock==false){
    /* P.20 */
    /* ana. of Select Event numbers per day and pwidth distribution */
    gROOT->LoadMacro(Form("%sSEAnaV2.C+",path_Mac));
    SEAnaV2(33);
  }

  if(SkipBlock==false){
    /* P.21 *///-New-
    /* Rate ana for separate board and chID (easy to observe all) */
    gROOT->LoadMacro(Form("%sR4VTAna.C+",path_Mac));
    R4VTAna();
  }
  
  if(SkipBlock==false){
    /* P.22 */
    /* Rate ana for general chID (easy to observe all) */
    gROOT->LoadMacro(Form("%sRVTAna.C+",path_Mac));
    RVTAna();
  }
  
  if(SkipBlock==false){
    /* P.23 */
    /* Rate Vs Time cp of channel or board for different display way */
    gROOT->LoadMacro(Form("%sRVTCAna.C+",path_Mac));
    RVTCAna();
  }
  
  if(SkipBlock==false){
    /* P.24 */
    /* Find HK data and record data to be .dat */
    gROOT->LoadMacro(Form("%sHKProAna.C+",path_Mac));
    HKProAna();
  }
  
  if(SkipBlock==false){
    /* P.25 */
    /* draw th2f colz for TVTime or HVTime and save average TVtime and HVTime*/
    //gROOT->LoadMacro(Form("%sHKProAna.C+",path_Mac));
    HKAna();
  }
  
  if(SkipBlock==false){
    /* P.26 */
    /* Draw TVTime or HVTime by TGraph */
    //gROOT->LoadMacro(Form("%sHKProAna.C+",path_Mac));
    HKTGAna();
  }
  
  if(SkipBlock==false){
    /* P.27 */
    /* Rate Vs house keeping data for Temperature or humidity */
    gROOT->LoadMacro(Form("%sRVHKAna.C+",path_Mac));
    RVHKAna(OperMode);
  }
  
  if(SkipBlock==false){
    /* P.28 */
    /* Hit count/sequence skip ana. */
    gROOT->LoadMacro(Form("%sSeqSkipAna.C+",path_Mac));
    SeqSkipAna();
  }
  
  cout<<"End the Program for:  "<<name_Raw<<"   "<<name_Hk<<endl;
  throw;
  

}


void MainControl()
{
  //path of original Macros, it is the Macro path your download from GitHub  
  char *Ori_path_Mac = "./Macro/";
  
  /* Adjustable Path Variable */
  char *path_Raw = "/data4/YuSiang/DaXi/";

  //path of the raw data for muon (*_Mu.txt)
  char *path_Hk  = "/data4/YuSiang/DaXi/HKData/";
  
  //path of the runs data for detector information (Setup_*.txt)
  char *path_Run = "/data4/YuSiang/DaXi/OdetData/";
  
  //path of the raw data for House keeping (*_HK.txt)
  char *path_Rot = "/data4/YuSiang/DaXi/rootfile/";
  
  //path of the raw data for House keeping (*_HK.txt)
  char *path_OpR = "/data4/YuSiang/DaXi/Ana/";
    
  //path of result and operation data
  char *path_Mac = "./Macro1/";
  //path of operation Macros, it is the Macro path your put in the case dir
  
  /* Data name want to search!! */
  char *name_Raw = "*_Mu";
  char *name_Hk  = "*_HK";
  /*Example:   
    1. A directory contain 3 file:
        Name1.txt & Name.txt & N.txt
    2. case for name_Raw = "Name":
        Will analyze the file: Name.txt
    3. case for name_Raw = "Name*":
        Will analyze the file: Name.txt & Name1.txt
    4. case for name_Raw = "*":
        Will analyze the file: Name.txt & Name1.txt % N.txt
  Finish Example */
  
  string runOPT = "R";
  
  //Execute the program!
  MainControl(Ori_path_Mac,  path_Raw  ,  path_Hk ,  path_Run ,  path_Rot,
             path_OpR    ,  path_Mac  ,  name_Raw ,  name_Hk , runOPT);

}