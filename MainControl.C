#ifndef FATAL
#define FATAL(msg) do { fprintf(stderr, "FATAL: %s\n", msg); gSystem->Exit(1); } while (0)
#endif
#ifndef GET_BIT
#define	GET_BIT(x, bit)	((x & (1 << bit)) >> bit)
#endif
using namespace std;
//if the char variable is empty, then set the default;
const string AOPT[32] = {
  "RRRM",
  "TESM","NorM","NegM","LStM","RTFM","SETM","PTXM",
  "SKBS",
  "BEnv","BDOI","BFOF","BCon","BFRF","BDSL",
  "BDEF","BEGP","BEME","BVTr","BVTE","BEPW",
  "BFTr","BFTR","BFPW","BFSE","BRVT","-",
};

void MainControl(
  char *Ori_path_Mac, char *path_Raw, char *path_Hk,
  char *path_Run    , char *path_Rot, char *path_OpR,
  char *path_Mac    , char *name_Raw, char *name_Hk,
  const string runOPT = "nLrP")
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

  int OPT = 0 ;
  for (int i=0; AOPT[i]!="-" ;i++)
    if(int( runOPT.find(AOPT[i])) != -1) OPT |= 1UL << i;
  
  cout<<OPT<<endl;
  bool ReRunRunMode    = OPT>>0;

  bool ReRunRunMode    = GET_BIT(OPT,0);
  bool testMode        = GET_BIT(OPT,1);//pwidth ana. test mode
  bool normalizeMode   = GET_BIT(OPT,2);
  bool NegHiRatChMode  = GET_BIT(OPT,3);
  bool lowstatisticMode= GET_BIT(OPT,4);
  bool rootFileMode    = GET_BIT(OPT,5);  
  bool SETightMode     = GET_BIT(OPT,6);
  bool PointTextMode   = GET_BIT(OPT,7);
  bool SkipBlockStop   = GET_BIT(OPT,8);//default skip the progress = true
  
  bool BlockEnv        = GET_BIT(OPT,9);//default skip the progress = true
  bool BlockODetImf    = GET_BIT(OPT,10);//default skip the progress = true
  bool BlockFindOriFile= GET_BIT(OPT,11);//default skip the progress = true
  bool BlockConvertor  = GET_BIT(OPT,12);//default skip the progress = true
  bool BlockFindRFile  = GET_BIT(OPT,13);//default skip the progress = true
  bool BlockDSLAna     = GET_BIT(OPT,14);//default skip the progress = true
  bool BlockDayEff     = GET_BIT(OPT,15);//default skip the progress = true
  bool BlockEventGaps  = GET_BIT(OPT,16);//default skip the progress = true
  bool BlockEzMuEID    = GET_BIT(OPT,17);//default skip the progress = true
  bool BlockVTrack     = GET_BIT(OPT,18);//default skip the progress = true
  bool BlockVTrackEff  = GET_BIT(OPT,19);//default skip the progress = true
  bool BlockEzPWAna    = GET_BIT(OPT,20);//default skip the progress = true
  bool BlockFitTrick   = GET_BIT(OPT,21);//default skip the progress = true
  bool BlockFitTrickRes= GET_BIT(OPT,22);//default skip the progress = true
  bool BlockFitPWAna   = GET_BIT(OPT,23);//default skip the progress = true
  bool BlockFinalSelect= GET_BIT(OPT,24);//default skip the progress = true
  bool BlockRateVTime  = GET_BIT(OPT,25);//default skip the progress = true
  
  bool OperMode[7]={
    testMode,         normalizeMode,    NegHiRatChMode,
    lowstatisticMode, rootFileMode,     SETightMode,
    PointTextMode,
  };
  
  bool SkipBlocks = false;
    if(BlockEnv){
      /* P.01 */
      /* Copy the AnaVariable.h and the macros into path_Mac and locate tmpdir*/
      gROOT->LoadMacro("./RenewMacros.h");
      RenewMacros(TString(path_Mac),TString(Ori_path_Mac));
      /* const path of Macro compiling */
      gSystem->SetBuildDir(TString(path_Mac)+"tmpdir", kTRUE);
    
      /* P.02 *///-Able-
      /* Establish the Compile dir set */
      gROOT->LoadMacro(Form("%sDataBaseLocate.C+",path_Mac));
      DataBaseLocateV3( path_Raw, path_Hk, path_Run, path_Rot, path_OpR, path_Mac, name_Raw, name_Hk, '' );
      cout<<"Path of Raw data: "<<path_Raw<<endl;
      cout<<"Path of Root Dir: "<<path_OpR<<endl;
    }
    
  if(!SkipBlocks){
    if(ReRunRunMode){
      /* P.03 *///-Able-
      gROOT->LoadMacro(Form("%sRunDataSortAnaV2.C+",path_Mac));
      RunDataSortAnaV2();
    }

    if(BlockODetImf){
      /* P.04 *///-New-
      /* Locate the data of detector */
      gROOT->LoadMacro(Form("%sODectImfAna.C+",path_Mac));
      //void ODectImfAna()
      ODectImfAna();
      TillRunODInf();
      EffTillRunODInf("./EffTestV2_V3.txt");
      
      /* P.05 *///-New-
      /* Locate the data of detector */
      // gROOT->LoadMacro(Form("%sCombinationFac.C+",path_Mac));
      //GetPosCode();
      //OCombinationFac();
      // CombinationFac();
    }
    
    if(BlockFindOriFile){
      /* P.06 */
      /* Find Raw .txt data Names and path */
      gROOT->LoadMacro(Form("%sDataNameAna.C+",path_Mac));
      int txtfnum = DataNameAnaTxt_Mu();
      //int DataNameAnaTxt_Mu(const char OPT='P') 
      cout<<"Find Raw .txt data Number:  "<<txtfnum<<endl;
      
      /* P.07 */
      /* Find HK .txt data Names and path */
      gROOT->LoadMacro(Form("%sDataNameAna.C+",path_Mac));
      int Hktxtfnum = DataNameAnaTxt_Hk();
      //int DataNameAnaTxt_Mu(const char OPT='P') 
      // cout<<"Find Hk .txt data Number:  "<<Hktxtfnum<<endl;
      
    }
        // throw;
    if(BlockConvertor){
      /* P.08 *///-New-
      /* Convert File from raw .txt to be raw .root */
      gROOT->LoadMacro(Form("%sCaConvertor.C+",path_Mac));
      MuoCaConvertor();
    }

    if(BlockFindRFile){
      /* P.09 */
      /* Find Raw .root data Names and path */
      gROOT->LoadMacro(Form("%sDataNameAna.C+",path_Mac));
      int rotfnum = DataNameAnaRoot_Mu('');
      //int DataNameAnaRoot_Mu(const char OPT='P') 
      cout<<"Find Raw .root data Number:  "<<rotfnum<<endl;
    }
  
    if(BlockDSLAna){
      /* P.10 *///-Able-
      /*Analize the Data for time start to end and save to head file*/
      gROOT->LoadMacro(Form("%sDSLAnaV3.C+",path_Mac));
      DSLAnaV3();
    }

    if(BlockDayEff){
      /* P.11 *///-New-
      /* Data Collection Efficiency on every day */
      gROOT->LoadMacro(Form("%sDayEAnaV4.C+",path_Mac));
      DayEAnaV4();
    }
    
    if(BlockEventGaps){
      /* P.12 *///-New-
      /*Convert Raw_Mu.root to be Gap*.root by tcnt cut*/
      gROOT->LoadMacro(Form("%sEventAna.C+",path_Mac));
      //int Num_RawEVE = EventAna(33,34);
      //void EventAna(const int indexi=28, const int indexf=29, const int unixtimeini = unixtimei, const int unixtimefin = unixtimef, const char * runName="-NA-" ) {//140
      EventAna(33,34);
      EventGapAna();
    }
  

    if(BlockEzMuEID){
      /* P.13 *///-New-
      /*Simpling ID of Mu and Ele to be Events*.root */
      gROOT->LoadMacro(Form("%sMuonElectronID.C+",path_Mac));
      MuoEleIDAna(OperMode, 33,34);//300
      //MuoEleIDAna(c.bool*OperMode, c.int indexi=28, c.int indexf=29)
    }
    
    if(BlockVTrack){
      /* P.14 */
      /* Verticle Straight line Event Ana to be Track*.root */
      gROOT->LoadMacro(Form("%sVerticalTracksAna.C+",path_Mac));
      VerticalTracksAnaV2(OperMode,33,34);
      //VerticalTracksAna(c.int indexi=28, c.int indexf=29 )
    }
  
    if(BlockVTrackEff){
      /* P.15 */
      /* Find real events in straight line Tracking Ana. */
      gROOT->LoadMacro(Form("%sSTRealEvAna.C+",path_Mac));
      STRealEvAnaV2(33);
    }
  }
  if(!SkipBlocks){
  }

  
    if(BlockEzPWAna){
      /* P.16 *///-New-
      /* Pwidth analyze for function fitting and trigger ana. */
      gROOT->LoadMacro(Form("%sPwidthAna.C+",path_Mac));
      PwidthAnaV2(OperMode,33);
      //PWidthPeakAna(c.bool*OperMode)
    }
FATAL("iiuonufi");
    if(!SkipBlockStop){
      /* P.17 */
      /* Pwidth analyze for function fitting and trigger ana. */
      //gROOT->LoadMacro(Form("%sPWidthPeakAna.C+",path_Mac));
      //PWidthPeakAna(OperMode);
      //PWidthPeakAna(const bool*OperMode)
    }

    if(BlockFitTrick){
      /* P.18 *///-New--@@
      /* Straight line Tracking of Event to be ETracks*.txt */
      gROOT->LoadMacro(Form("%sTrackAna.C+",path_Mac));
      TrackAna(33,34);
      FitTrackInfoAna(33);
      //void  FitTrackResAna(c.int indexi=28, c.int indexf=29 )
    }
  if(!SkipBlocks){
  }

    if(BlockFitTrickRes){
      /* P.19 *///-New--
      /* Straight line Tracking Result display*.txt */
      gROOT->LoadMacro(Form("%sFitTrackResAna.C+",path_Mac));
      FitTrackResAnaV2(33,true);//@@
      //void  FitTrackResAna(const int indexGap=28)
    }
    if(BlockFitPWAna){
      /* P.20 *///-New-
      /* Pwidth analyze for function fitting and trigger ana. */
      gROOT->LoadMacro(Form("%sPwidthAna.C+",path_Mac));
      PwidthAnaV2P(OperMode,33);
    }
  if(!SkipBlocks){
  }
    if(BlockFinalSelect){
      /* P.21 */
      /* ana. of Select Event numbers per day and pwidth distribution */
      gROOT->LoadMacro(Form("%sSEAnaV2.C+",path_Mac));
      SEAnaV2(33);
    }

  FATAL("wogf");
  if(!SkipBlocks){
    if(BlockRateVTime){
      /* P.22 *///-New-
      /* Rate ana for separate board and chID (easy to observe all) */
      gROOT->LoadMacro(Form("%sR4VTAna.C+",path_Mac));
      R4VTAna();
      
      /* P.23 */
      /* Rate ana for general chID (easy to observe all) */
      gROOT->LoadMacro(Form("%sRVTAna.C+",path_Mac));
      RVTAna();
      
      /* P.24 */
      /* Rate Vs Time cp of channel or board for different display way */
      //gROOT->LoadMacro(Form("%sRVTCAna.C+",path_Mac));
      //RVTCAna();
    }  
  }
    /* P.25 */
    /* Find HK data and record data to be .dat */
    gROOT->LoadMacro(Form("%sHKProAna.C+",path_Mac));
    HKProAnaV3();

    /* P.26 */
    /* draw th2f colz for TVTime or HVTime and save average TVtime and HVTime*/
    gROOT->LoadMacro(Form("%sHKProAna.C+",path_Mac));
    HKAnaV2();
    
    /* P.27 */
    /* Draw TVTime or HVTime by TGraph */
    gROOT->LoadMacro(Form("%sHKProAna.C+",path_Mac));
    HKTGAnaV2();
    /* P.28 */
    /* Draw the relationship of Temperature/Humidity Vs Position Vs time pass*/
    gROOT->LoadMacro(Form("%sHKPosAna.C+",path_Mac));
    HKPosAna();
  

  // throw;
  if(!SkipBlockStop){
    
    /* P.29 */
    /* Rate Vs house keeping data for Temperature or humidity */
    //gROOT->LoadMacro(Form("%sRVHKAna.C+",path_Mac));
    //RVHKAna(OperMode);
    
    /* P.30 */
    /* Hit count/sequence skip ana. */
    //gROOT->LoadMacro(Form("%sSeqSkipAna.C+",path_Mac));
    //SeqSkipAna();
  }
  
  cout<<"End the Program for:  "<<name_Raw<<"   "<<name_Hk<<endl;
  throw;
  

}

void MainControl()
{
  
  /* @AdjustPath */
  //path of original Macros, it is the Macro path your download from GitHub  
  char *Ori_path_Mac = "./Macro/";
  
  /* Adjustable Path Variable */
  char *path_Raw = "/data4/vivian/CheckThresholeMTB17-20/RawData/";

  //path of the raw data for muon (*_Mu.txt)
  char *path_Hk  = "/data4/vivian/CheckThresholeMTB17-20/HKData/";
  
  //path of the runs data for detector information (Setup_*.txt)
  char *path_Run = "/data4/YuSiang/MTBThresholeCheck/OdetData/";
  
  //path of the raw data for House keeping (*_HK.txt)
  char *path_Rot = "/data4/vivian/CheckThresholeMTB17-20/rootfile/";
  
  //path of the raw data for House keeping (*_HK.txt)
  char *path_OpR = "/data4/YuSiang/MTBThresholeCheck/AnaR1/";
    
  //path of result and operation data
  char *path_Mac = "/data4/YuSiang/MTBThresholeCheck/AnaR1/MacroTmp/";
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
  
  /* @EndAdjustPath */
  
  
  string runOPT = "";
  
  
  /* @AdjustOpMode */
  bool ReRunRunMode    = true;
    //default: true ; //Run code for sort the runs data.

  bool testMode        = false;
    //default: false; //Test code by using small sample number in some pwidth ana. program.
    
  bool normalizeMode   = true;
    //default: true ; //Normalize the difference by the statistic in pwidth ana. program.
    
  bool NegHiRatChMode  = false;
    //default: true ; //Could not use now!
    
  bool lowstatisticMode= true;
    //default: false; //when the data come from large theta, use MuEelID to analyze the pwidth.
    //ex: theta == 75 deg ->True, Theta == 0 deg ->False
    
  bool rootFileMode    = true;  
    //default: true ; //re save the data of pwidth ana. want to use. 
    
  bool SETightMode     = false;
    //default: true ; //select event tightly for MuEelID in pwidth ana. 
    //ex: theta == 75 deg ->false, Theta == 0 deg ->true
    
  bool PointTextMode   = true;
    //default: true ; //draw the data point of HK VS Rate to be text(save time).
  
  bool SkipBlockStop   = true;
    //default: true ; //Skip the program can't use now.
    
  bool BlockEnv        = true;
    //default: true ; //execute the block for establish the operation environment.
    
  bool BlockODetImf    = true;
    //default: true ; //execute the block for original detector information.

  bool BlockFindOriFile= true;
    //default: true ; //execute the block for find out the raw files corresponding to condition.

  bool BlockConvertor  = true;
    //default: true ; //execute the block for convert the raw data(_Mu.txt) to be (_Mu.root).

  bool BlockFindRFile  = true;
    //default: true ; //execute the block for find out the root files corresponding to condition.
    
  bool BlockDSLAna     = true;
    //default: true ; //execute the block for establish the operation environment.
    
  bool BlockDayEff     = true;
    //default: true ; //execute the block for measure channels detect efficiency on time.
    
  bool BlockEventGaps  = true;
    //default: true ; //execute the block for flit data by "tcnt" and save GapT*.root.
    
  bool BlockEzMuEID    = true;  
    //default: true ; //execute the block for ana. data by nHits... and save EventsGT*.root.
    
  bool BlockVTrack     = true;
    //default: true ; //execute the block for ana. data by chID and save TracksGT*.root.

  bool BlockVTrackEff  = true;
    //default: true ; //execute the block for ana. the channels efficiency on uncertainty.
    
  bool BlockEzPWAna    = true;
    //default: true ; //execute the block for ana. PWidth data of EventsGT*.root and GapT*.root.
    
  bool BlockFitTrick   = true;
    //default: true ; //execute the block for 3DLine fit of GapT*.root, save ETracksGT*.root.
    
  bool BlockFitTrickRes= true;
    //default: true ; //execute the block for print the distribution of ETracksGT*.root.

  bool BlockFitPWAna   = true;
    //default: true ; //execute the block for ana. PWidth data of All kind selection on ch.
    
  bool BlockFinalSelect= true;
    //default: true ; //execute the block for Ana. the final tight selection of ETracksGT*.root.
    
  bool BlockRateVTime  = true;
    //default: true ; //execute the block for draw the rate data of detector.
  
   /* @EndAdjustOpMode */
  
  
  
  /*OP==00*/ if( ReRunRunMode    )runOPT += "RRRM";
  
  /*OP==01*/ if( testMode        )runOPT += "TESM";
  /*OP==02*/ if( normalizeMode   )runOPT += "NorM";
  /*OP==03*/ if( NegHiRatChMode  )runOPT += "NegM";
  /*OP==04*/ if( lowstatisticMode)runOPT += "LStM";
  /*OP==05*/ if( rootFileMode    )runOPT += "RTFM";
  /*OP==06*/ if( SETightMode     )runOPT += "SETM";
  /*OP==07*/ if( PointTextMode   )runOPT += "PTXM";
  
  /*OP==08*/ if(SkipBlockStop    )runOPT += "SKBS";
  
  /*OP==09*/ if( BlockEnv        )runOPT += "BEnv";
  /*OP==10*/ if( BlockODetImf    )runOPT += "BDOI";
  /*OP==11*/ if( BlockFindOriFile)runOPT += "BFOF";
  /*OP==12*/ if( BlockConvertor  )runOPT += "BCon";
  /*OP==13*/ if( BlockFindRFile  )runOPT += "BFRF";
  /*OP==14*/ if( BlockDSLAna     )runOPT += "BDSL";
  /*OP==15*/ if( BlockDayEff     )runOPT += "BDEF";
  /*OP==16*/ if( BlockEventGaps  )runOPT += "BEGP";
  /*OP==17*/ if( BlockEzMuEID    )runOPT += "BEME";
  /*OP==18*/ if( BlockVTrack     )runOPT += "BVTr";
  /*OP==19*/ if( BlockVTrackEff  )runOPT += "BVTE";
  /*OP==20*/ if( BlockEzPWAna    )runOPT += "BEPW";
  /*OP==21*/ if( BlockFitTrick   )runOPT += "BFTr";
  /*OP==22*/ if( BlockFitTrickRes)runOPT += "BFTR";
  /*OP==23*/ if( BlockFitPWAna   )runOPT += "BFPW";
  /*OP==24*/ if( BlockFinalSelect)runOPT += "BFSE";
  /*OP==25*/ if( BlockRateVTime  )runOPT += "BRVT";
  
  
  
  
  //Execute the program!
  MainControl(Ori_path_Mac,  path_Raw  ,  path_Hk ,  path_Run ,  path_Rot,
             path_OpR    ,  path_Mac  ,  name_Raw ,  name_Hk , runOPT);

}
  


