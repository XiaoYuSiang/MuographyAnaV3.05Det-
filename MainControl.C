#include "./SettingJSONIO.h"

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
const string DataDetVer[10] = {
  "V2.00","V3.00","V3.05","User","-"
};

void MainControl(char *Settingfile){// = "/home/yusiang/TestV4/AdjustPath-AnaR13_20230309.json")
  JSONFileIOSet PMS(Settingfile);//PathModeSetting

  PMS.Show();
  
  bool OperMode[7]={
    PMS.testMode,         PMS.normalizeMode,    PMS.NegHiRatChMode,
    PMS.lowstatisticMode, PMS.rootFileMode,     PMS.SETightMode,
    PMS.PointTextMode,
  };
  
  string vertmp = PMS.det_ver;
  cout<<"Find vertmp is: "<<vertmp<<endl;
  
  /**/
  bool SkipBlocks = false;
    if(PMS.BlockEnv){
      /* P.01 */
      /* Copy the AnaVariable.h and the macros into path_Mac and locate tmpdir*/
      gROOT->LoadMacro("./RenewMacros.h");
      cout<<"vertmp.data(),TString(path_Mac),TString(path_Lib)"<<endl;
      cout<<vertmp.data()<<" "<<TString(PMS.path_Mac)<<" "<<TString(PMS.path_Lib)<<endl;
      // throw;
      RenewMacrosV2(vertmp.data(),TString(PMS.path_Mac),TString(PMS.path_Lib));
      /* const path of Macro compiling */
      gSystem->SetBuildDir(TString(PMS.path_Mac)+"tmpdir", kTRUE);
    
      /* P.02 *///-Able-
      /* Establish the Compile dir set */
      gROOT->LoadMacro(Form("%sDataBaseLocate.C+",PMS.path_Mac));
      DataBaseLocateV3(PMS.path_Raw, PMS.path_Hk, PMS.path_Run, PMS.path_Rot, PMS.path_OpR, PMS.path_Mac, PMS.name_Raw, PMS.name_Hk, '' );
      cout<<"Path of Raw data: "<<PMS.path_Raw<<endl;
      cout<<"Path of Root Dir: "<<PMS.path_OpR<<endl;
    }
    
  if(!SkipBlocks){
    if(PMS.ReRunRunMode){
      /* P.03 *///-Able-
      gROOT->LoadMacro(Form("%sRunDataSortAnaV3.C+",PMS.path_Mac));
      RunDataSortAnaV3();
    }

    if(PMS.BlockODetImf){
      /* P.04 *///-New-
      /* Locate the data of detector */
      gROOT->LoadMacro(Form("%sODectImfAna.C+",PMS.path_Mac));
      //void ODectImfAna()
      ODectImfAna();
      TillRunODInf();
      if(PMS.MCMode) EffTillRunODInf("./EffTestV2_V3.txt");
      else EffTillRunODInf("./EffV0305N0_New.txt");//@@
      
      /* P.05 *///-New-
      /* Locate the data of detector */
      gROOT->LoadMacro(Form("%sCombinationFac.C+",PMS.path_Mac));
      //GetPosCode();
      //OCombinationFac();
      CombinationFac();
    }
    
    if(PMS.BlockFindOriFile){
      /* P.06 */
      /* Find Raw .txt data Names and path */
      gROOT->LoadMacro(Form("%sDataNameAna.C+",PMS.path_Mac));
      int txtfnum = DataNameAnaTxt_Mu();
      //int DataNameAnaTxt_Mu(const char OPT='P') 
      cout<<"Find Raw .txt data Number:  "<<txtfnum<<endl;
      
      /* P.07 */
      /* Find HK .txt data Names and path */
      gROOT->LoadMacro(Form("%sDataNameAna.C+",PMS.path_Mac));
      int Hktxtfnum = DataNameAnaTxt_Hk();
      //int DataNameAnaTxt_Mu(const char OPT='P') 
      // cout<<"Find Hk .txt data Number:  "<<Hktxtfnum<<endl;
      
    }
  
  }
  if(!SkipBlocks){
    if(PMS.BlockConvertor){
      /* P.08 *///-New-
      /* Convert File from raw .txt to be raw .root */
      if(!PMS.MCMode){
        gROOT->LoadMacro(Form("%sCaConvertor.C+",PMS.path_Mac));
        MuoCaConvertor();
      }else{
        gROOT->LoadMacro(Form("%sG4CaConvertorV2.C+",PMS.path_Mac));
        const char* sourceFs = Form("%s%s", PMS.path_MC, PMS.name_MC);
        G4CaConvertorV2(sourceFs, PMS.path_Rot, PMS.name_MC);
      }
      
    }
    if(PMS.BlockFindRFile){
      /* P.09 */
      /* Find Raw .root data Names and path */
      gROOT->LoadMacro(Form("%sDataNameAna.C+",PMS.path_Mac));
      int rotfnum;
      if(PMS.MCMode) rotfnum = MCNameAnaRoot_Mu(PMS.name_MC, '');
      else rotfnum = DataNameAnaRoot_Mu('');
      //int DataNameAnaRoot_Mu(const char OPT='P') 
      cout<<"Find Raw .root data Number:  "<<rotfnum<<endl;
    }
  
    if(PMS.BlockDSLAna){
      /* P.10 *///-Able-
      /*Analize the Data for time start to end and save to head file*/
      gROOT->LoadMacro(Form("%sDSLAnaV4.C+",PMS.path_Mac));
      DSLAnaV3(PMS.MCMode);
    }

    if(PMS.BlockDayEff){
      /* P.11 *///-New-
      /* Data Collection Efficiency on every day */
      gROOT->LoadMacro(Form("%sDayEAnaV4.C+",PMS.path_Mac));
      DayEAnaV4();
    }
  }
    if(PMS.BlockEventGaps){
      /* P.12 *///-New-
      /*Convert Raw_Mu.root to be Gap*.root by tcnt cut*/
      gROOT->LoadMacro(Form("%sEventAnaV2.C+",PMS.path_Mac));
      //int Num_RawEVE = EventAna(33,34);
      //void EventAna(const int indexi=28, const int indexf=29, const int unixtimeini = unixtimei, const int unixtimefin = unixtimef, const char * runName="-NA-" ) {//140
      EventAnaV2(33,34);
      // EventGapAna();
    }
  // FATAL("ueotngpbv");
  if(!SkipBlocks){
    if(PMS.BlockEzMuEID){
      /* P.13 *///-New-
      /*Simpling ID of Mu and Ele to be Events*.root */
      gROOT->LoadMacro(Form("%sMuonElectronID.C+",PMS.path_Mac));
      MuoEleIDAna(OperMode, 33,34);//300
      //MuoEleIDAna(c.bool*OperMode, c.int indexi=28, c.int indexf=29)
    }
    
    if(PMS.BlockVTrack){
      /* P.14 */
      /* Verticle Straight line Event Ana to be Track*.root */
      gROOT->LoadMacro(Form("%sVerticalTracksAna.C+",PMS.path_Mac));
      VerticalTracksAnaV2(OperMode,33,34);
      //VerticalTracksAna(c.int indexi=28, c.int indexf=29 )
    }
  
    if(PMS.BlockVTrackEff){
      /* P.15 */
      /* Find real events in straight line Tracking Ana. */
      gROOT->LoadMacro(Form("%sSTRealEvAna.C+",PMS.path_Mac));
      STRealEvAnaV2(33);
    }
  }

  
  if(!SkipBlocks){
    if(PMS.BlockEzPWAna){
      /* P.16 *///-New-
      /* Pwidth analyze for function fitting and trigger ana. */
      gROOT->LoadMacro(Form("%sPwidthAna.C+",PMS.path_Mac));
      PwidthAnaV2(OperMode,33);
      //PWidthPeakAna(c.bool*OperMode)
    }

    if(!PMS.SkipBlockStop){
      /* P.17 */
      /* Pwidth analyze for function fitting and trigger ana. */
      //gROOT->LoadMacro(Form("%sPWidthPeakAna.C+",PMS.path_Mac));
      //PWidthPeakAna(OperMode);
      //PWidthPeakAna(const bool*OperMode)
    }

    if(PMS.BlockFitTrick){
      /* P.18 *///-New--@@
      /* Straight line Tracking of Event to be ETracks*.txt */
      gROOT->LoadMacro(Form("%sTrackAna.C+",PMS.path_Mac));
      TrackAna(33,34);
      FitTrackInfoAna(33);
      //void  FitTrackResAna(c.int indexi=28, c.int indexf=29 )
    }
  }

    if(PMS.BlockFitTrickRes){
      /* P.19 *///-New--
      /* Straight line Tracking Result display*.txt */
      gROOT->LoadMacro(Form("%sFitTrackResAna.C+",PMS.path_Mac));
      FitTrackResAnaV2(33,PMS.RotateSciMode);
      //void  FitTrackResAna(const int indexGap=28)
    }
  if(!SkipBlocks){
    if(PMS.BlockFitPWAna){
      /* P.20 *///-New-
      /* Pwidth analyze for function fitting and trigger ana. */
      gROOT->LoadMacro(Form("%sPwidthAna.C+",PMS.path_Mac));
      PwidthAnaV2P(OperMode,33);
    }
  }
    if(PMS.BlockFinalSelect){
      /* P.21 */
      /* ana. of Select Event numbers per day and pwidth distribution */
      gROOT->LoadMacro(Form("%sSEAnaV2.C+",PMS.path_Mac));
      SEAnaV2(33);
    }

  FATAL("wogf");
  if(!SkipBlocks){
    if(PMS.BlockRateVTime){
      /* P.22 *///-New-
      /* Rate ana for separate board and chID (easy to observe all) */
      gROOT->LoadMacro(Form("%sR4VTAna.C+",PMS.path_Mac));
      R4VTAna();
      
      /* P.23 */
      /* Rate ana for general chID (easy to observe all) */
      gROOT->LoadMacro(Form("%sRVTAna.C+",PMS.path_Mac));
      RVTAna();
      
      /* P.24 */
      /* Rate Vs Time cp of channel or board for different display way */
      //gROOT->LoadMacro(Form("%sRVTCAna.C+",PMS.path_Mac));
      //RVTCAna();
    }  
  }
    /* P.25 */
    /* Find HK data and record data to be .dat */
    gROOT->LoadMacro(Form("%sHKProAna.C+",PMS.path_Mac));
    HKProAnaV3();

    /* P.26 */
    /* draw th2f colz for TVTime or HVTime and save average TVtime and HVTime*/
    gROOT->LoadMacro(Form("%sHKProAna.C+",PMS.path_Mac));
    HKAnaV2();
    
    /* P.27 */
    /* Draw TVTime or HVTime by TGraph */
    gROOT->LoadMacro(Form("%sHKProAna.C+",PMS.path_Mac));
    HKTGAnaV2();
    /* P.28 */
    /* Draw the relationship of Temperature/Humidity Vs Position Vs time pass*/
    gROOT->LoadMacro(Form("%sHKPosAna.C+",PMS.path_Mac));
    HKPosAna();
  

  // throw;
  if(!PMS.SkipBlockStop){
    
    /* P.29 */
    /* Rate Vs house keeping data for Temperature or humidity */
    //gROOT->LoadMacro(Form("%sRVHKAna.C+",PMS.path_Mac));
    //RVHKAna(OperMode);
    
    /* P.30 */
    /* Hit count/sequence skip ana. */
    //gROOT->LoadMacro(Form("%sSeqSkipAna.C+",PMS.path_Mac));
    //SeqSkipAna();
  }
  
  cout<<"End the Program for:  "<<name_Raw<<"   "<<name_Hk<<endl;
  throw;
  


}

void MainControl()
{
  //Name of save the setting JSON file
  const char *JSONtmp = ReCreateJSON("./OutSettingTmp.json");
  //default: false; //The data is MC or not
  bool MCMode    = true;
  /* @AdjustPath */
  /* Adjustable Path Variable */
  JSONFileIOSet PathModeSetting(MCMode);
  //path of original Macros, it is the Macro path your download from GitHub  
  PathModeSetting.path_Lib = "./Macro/";
  // path of operation Macros, it is the Macro path your put in the case dir
  PathModeSetting.path_Mac = "/data4/YuSiang/TEST/MontAna/MacroTmp/";
  // path of result and operation data
  PathModeSetting.path_OpR = "/data4/YuSiang/TEST/MontAna/";
  
  // Detector version of search the detector imformattion file
  PathModeSetting.det_ver  = "V3.05";
  // path of the runs data for detector information (Setup_*.txt)
  PathModeSetting.path_Run = "/data4/YuSiang/TEST/OdetData/";
  
  if(PathModeSetting.MCMode){
    // path of the MC data for muon (*.root)
    PathModeSetting.path_MC  = "/data4/AndyLu/to_cw/";
    /* Data name want to search!! */
    PathModeSetting.name_MC  = "4_4_4_starightbeam_merge.root";
  }else{
    // path of the raw data for muon (*_Mu.txt)
    PathModeSetting.path_Raw = "/data4/YuSiang/DAXIFIX/RawMu/";
    // path of the raw data for House keeping (*_HK.txt)
    PathModeSetting.path_Hk  = "/data4/YuSiang/DAXIFIX/RawHK/";
    /* Data name want to search!! */
    PathModeSetting.name_Raw = "*Run1[0,1]*_*Mu*";
    PathModeSetting.name_Hk  = "*Run1[0,1]*_*HK*";
  }
  // path of the raw root file (*_Mu.root)
  PathModeSetting.path_Rot = "/data4/YuSiang/TEST/MontAna/rootfile/";
  
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
  
  /* @AdjustOpMode */
  
  if(!PathModeSetting.MCMode){
    PathModeSetting.ReRunRunMode    = true;
      //default: true ; //Run code for sort the runs data.

    PathModeSetting.testMode        = false;
      //default: false; //Test code by using small sample number in some pwidth ana. program.
      
    PathModeSetting.normalizeMode   = true;
      //default: true ; //Normalize the difference by the statistic in pwidth ana. program.
      
    PathModeSetting.NegHiRatChMode  = false;
      //default: true ; //Could not use now!
      
    PathModeSetting.lowstatisticMode= true;
      //default: false; //when the data come from large theta, use MuEelID to analyze the pwidth.
      //ex: theta == 75 deg ->True, Theta == 0 deg ->False
      
    PathModeSetting.rootFileMode    = true;  
      //default: true ; //re save the data of pwidth ana. want to use. 
      
    PathModeSetting.SETightMode     = false;
      //default: true ; //select event tightly for MuEelID in pwidth ana. 
      //ex: theta == 75 deg ->false, Theta == 0 deg ->true
      
    PathModeSetting.PointTextMode   = true;
      //default: true ; //draw the data point of HK VS Rate to be text(save time).
      
    PathModeSetting.RotateSciMode   = false;
      //Rotate the scintillator array buy 270 degree, default is false
    
    PathModeSetting.SkipBlockStop   = true;
      //default: true ; //Skip the program can't use now.
      
    PathModeSetting.BlockEnv        = true;
      //default: true ; //execute the block for establish the operation environment.
      
    PathModeSetting.BlockODetImf    = true;
      //default: true ; //execute the block for original detector information.

    PathModeSetting.BlockFindOriFile= true;
      //default: true ; //execute the block for find out the raw files corresponding to condition.

    PathModeSetting.BlockConvertor  = true;
      //default: true ; //execute the block for convert the raw data(_Mu.txt) to be (_Mu.root).

    PathModeSetting.BlockFindRFile  = true;
      //default: true ; //execute the block for find out the root files corresponding to condition.
      
    PathModeSetting.BlockDSLAna     = true;
      //default: true ; //execute the block for establish the operation environment.
      
    PathModeSetting.BlockDayEff     = true;
      //default: true ; //execute the block for measure channels detect efficiency on time.
      
    PathModeSetting.BlockEventGaps  = true;
      //default: true ; //execute the block for flit data by "tcnt" and save GapT*.root.
      
    PathModeSetting.BlockEzMuEID    = true;  
      //default: true ; //execute the block for ana. data by nHits... and save EventsGT*.root.
      
    PathModeSetting.BlockVTrack     = true;
      //default: true ; //execute the block for ana. data by chID and save TracksGT*.root.

    PathModeSetting.BlockVTrackEff  = true;
      //default: true ; //execute the block for ana. the channels efficiency on uncertainty.
      
    PathModeSetting.BlockEzPWAna    = true;
      //default: true ; //execute the block for ana. PWidth data of EventsGT*.root and GapT*.root.
      
    PathModeSetting.BlockFitTrick   = true;
      //default: true ; //execute the block for 3DLine fit of GapT*.root, save ETracksGT*.root.
      
    PathModeSetting.BlockFitTrickRes= true;
      //default: true ; //execute the block for print the distribution of ETracksGT*.root.

    PathModeSetting.BlockFitPWAna   = true;
      //default: true ; //execute the block for ana. PWidth data of All kind selection on ch.
      
    PathModeSetting.BlockFinalSelect= true;
      //default: true ; //execute the block for Ana. the final tight selection of ETracksGT*.root.
      
    PathModeSetting.BlockRateVTime  = true;
      //default: true ; //execute the block for draw the rate data of detector.
  }
   /* @EndAdjustOpMode */
  
  /* Output the Setting*/
  /*The setting and define of variable are at MainControl.C: L233, void MainControl()*/
  PathModeSetting.ExportJSON(JSONtmp);
  MainControl(JSONtmp);
}

