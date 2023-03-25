TString Addcommon(TString &s){
  return "\\\""+s+"\\\"";
}
void Runs(){
  TString cmdline;
  const char path_MacroTmp[30] = "./MacroTmp/";
  cmdline = TString("mkdir -p ")+path_MacroTmp;
  cout<<cmdline<<endl;
  if (system((char*)cmdline)!=0)  system(cmdline);
  
  const char MutiExecutetorName[50];
  sprintf(MutiExecutetorName,"%sscrip.sh",path_MacroTmp);
  ofstream out(MutiExecutetorName);
    // gROOT->LoadMacro("MainControl.C");
    
  int iRunStart = 8, iRunFinal = 9;
    
  const TString Ori_path_Mac = "./Macro/";
  const TString path_Raw = "/data4/YuSiang/MTB2_EffTest/";
  const TString path_Hk  = "/data4/YuSiang/MTB2_EffTest/HKData/";
  const TString path_Run = "/data4/YuSiang/MTB2_EffTest/OdetData/";
  const TString path_Rot = "/data4/YuSiang/MTB2_EffTest/rootfile/";
  char   path_OpR[100]; //= "/data4/YuSiang/DaXi/Ana/";
  char   path_Mac[100]; //= "./Macro1/";
  char   name_Raw[100]; 
  char   name_Hk [100]; 
  TString runOPT = ""; 



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




  for(int i=iRunStart; i<=iRunFinal;i++){
    sprintf(path_OpR,"/data4/YuSiang/MTB2_EffTest/AnaRun%02.f/",i*1.);
    sprintf(path_Mac,"%sRun%02.f/",path_MacroTmp,i*1.);
    sprintf(name_Raw,"*Run%d*Mu",i);
    sprintf(name_Hk ,"*Run%d*HK",i);
    // if(i!=0) RunOPT= "C";
    TString tmp =
      "sleep "+
      TString(Form("%d",abs((i==iRunStart)-1)*10+(i-iRunStart)))
      +" &&root -l -b MainControl.C\\("
      +Addcommon(Ori_path_Mac)+","
      +Addcommon(path_Raw)+","
      +Addcommon(path_Hk)+","
      +Addcommon(path_Run)+","
      +Addcommon(path_Rot)+","
      +Addcommon(TString(path_OpR))+","
      +Addcommon(TString(path_Mac))+","
      +Addcommon(TString(name_Raw))+","
      +Addcommon(TString(name_Hk ))+","
      +Addcommon(TString(runOPT  ))+"\\) &"
    ;

    cout<<tmp<<endl;
    out<<tmp<<endl;
    cmdline=tmp;
    
    //cout<<tmpc<<endl;
    //cout<<tmpc<<endl;
    const int leng = cmdline.Length();
    //cout<<leng<<endl;
    char cmdlinec[leng] = {};
    for(int ic=0;ic<leng;ic++) cmdlinec[ic] = cmdline[ic];
    // system(Form("%s",cmdline));
  }
  out<<"wait"<<endl;
  out<<"echo \"Finish the multiple execute of programs.\""<<endl;
  out.close();
  system(Form("sh %s",MutiExecutetorName));
  //@@ delete "sh ./scrip.sh"
}

