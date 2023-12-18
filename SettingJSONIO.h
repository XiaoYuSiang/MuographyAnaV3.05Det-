#include "/data4/YuSiang/personalLib/FileIO/FileIO.h"

class JSONFileIOSet{
public:
  char *path_Lib,*path_Mac, *path_OpR;
  char *det_ver, *path_Run;
  bool MCMode;
  char *path_MC, *name_MC;
  char *path_Raw,*path_Hk, *path_Rot, *name_Raw, *name_Hk  ;
  
  char *PBCode;
  
  bool ReRunRunMode    ;
  bool testMode        , normalizeMode   , GammaCutMode    ;
  bool NegHiRatChMode  , lowstatisticMode, rootFileMode    ;
  bool SETightMode     , PointTextMode   , RotateSciMode   ;
  
  bool SkipBlockStop   ;
  
  bool BlockEnv        , BlockODetImf    , BlockFindOriFile;
  bool BlockConvertor  , BlockFindRFile  , BlockDSLAna     ;
  bool BlockDayEff     , BlockEventGaps  , BlockEzMuEID    ;
  bool BlockVTrack     , BlockVTrackEff  , BlockEzPWAna    ;
  bool BlockFitTrick   , BlockFitTrickRes, BlockFitPWAna   ;
  bool BlockFinalSelect, BlockRateVTime  ;
  
  JSONFileIOSet(const bool MCorNot=false){
    if(!MCorNot)      SetDataDefault();
    else if(MCorNot)  SetMotCDefault();
  }
  JSONFileIOSet(const char *JSONFiletmp, bool MCorNot=false){
    CheckJSONGood(JSONFiletmp);
    SetJSONName(JSONFiletmp);
    if(!MCorNot)      SetDataDefault();
    else              SetMotCDefault();
    // cout<<"if !mcornot "<<!MCorNot<<endl;
    ImportJSON();
  }
  void CheckJSONGood(const char *JSONFiletmp){
    ifstream fin(JSONFiletmp, ifstream::in | ifstream::binary);
    fin.seekg(0, ios::end);
    Long64_t length = fin.tellg();
    fin.close();
    if(length>0)  cout<<"The size of JSON File: "<<JSONFiletmp<<" is "<<length<<" bytes."<<endl;
    else  FATAL(Form("Error: SetJSONIO.h::CheckJSONGOOD: The JSON file: %s is empty or not exist",JSONFiletmp));
  }
  void SetJSONName(const char *JSONFiletmp){
    JSONFileName = Form("%s",JSONFiletmp);
  } 
  
  void ExportJSON(){
    SaveAttrJSON(JSONFileName,"comment1"         ,"------------PathSetting-----------"    );
    SaveAttrJSON(JSONFileName,"path_Lib"         ,path_Lib         );
    SaveAttrJSON(JSONFileName,"path_Mac"         ,path_Mac         );
    SaveAttrJSON(JSONFileName,"path_OpR"         ,path_OpR         );
    SaveBLinJSON(JSONFileName);
    SaveAttrJSON(JSONFileName,"det_ver"          ,det_ver          );
    SaveAttrJSON(JSONFileName,"path_Run"         ,path_Run         );
    SaveBLinJSON(JSONFileName);
    SaveAttrJSON(JSONFileName,"path_MC"          ,path_MC          );
    SaveAttrJSON(JSONFileName,"name_MC"          ,name_MC          );
    SaveBLinJSON(JSONFileName);
    SaveAttrJSON(JSONFileName,"path_Raw"         ,path_Raw         );
    SaveAttrJSON(JSONFileName,"path_Hk"          ,path_Hk          );
    SaveAttrJSON(JSONFileName,"path_Rot"         ,path_Rot         );
    SaveAttrJSON(JSONFileName,"name_Raw"         ,name_Raw         );
    SaveAttrJSON(JSONFileName,"name_Hk"          ,name_Hk          );
    SaveBLinJSON(JSONFileName);  
    SaveAttrJSON(JSONFileName,"PBCode"           ,PBCode           );
    SaveAttrJSON(JSONFileName,"comment2"         ,"-----------AnalizeModes-----------"    );
    SaveAttrJSON(JSONFileName,"MCMode"           ,MCMode           );
    SaveBLinJSON(JSONFileName);
    SaveAttrJSON(JSONFileName,"ReRunRunMode"     ,ReRunRunMode     );
    SaveAttrJSON(JSONFileName,"testMode"         ,testMode         );
    SaveAttrJSON(JSONFileName,"normalizeMode"    ,normalizeMode    );
    SaveAttrJSON(JSONFileName,"GammaCutMode"     ,GammaCutMode     );
    SaveAttrJSON(JSONFileName,"NegHiRatChMode"   ,NegHiRatChMode   );
    SaveAttrJSON(JSONFileName,"lowstatisticMode" ,lowstatisticMode );
    SaveAttrJSON(JSONFileName,"rootFileMode"     ,rootFileMode     );
    SaveAttrJSON(JSONFileName,"SETightMode"      ,SETightMode      );
    SaveAttrJSON(JSONFileName,"PointTextMode"    ,PointTextMode    );
    SaveAttrJSON(JSONFileName,"RotateSciMode"    ,RotateSciMode    );
    SaveBLinJSON(JSONFileName);
    SaveAttrJSON(JSONFileName,"comment3"         ,"------------BlockModes------------"    );
    SaveAttrJSON(JSONFileName,"SkipBlockStop"    ,SkipBlockStop    );
    SaveBLinJSON(JSONFileName);  
    SaveAttrJSON(JSONFileName,"BlockEnv"         ,BlockEnv         );
    SaveAttrJSON(JSONFileName,"BlockODetImf"     ,BlockODetImf     );
    SaveAttrJSON(JSONFileName,"BlockFindOriFile" ,BlockFindOriFile );
    SaveAttrJSON(JSONFileName,"BlockConvertor"   ,BlockConvertor   );
    SaveAttrJSON(JSONFileName,"BlockFindRFile"   ,BlockFindRFile   );
    SaveAttrJSON(JSONFileName,"BlockDSLAna"      ,BlockDSLAna      );
    SaveAttrJSON(JSONFileName,"BlockDayEff"      ,BlockDayEff      );
    SaveAttrJSON(JSONFileName,"BlockEventGaps"   ,BlockEventGaps   );
    SaveAttrJSON(JSONFileName,"BlockEzMuEID"     ,BlockEzMuEID     );
    SaveAttrJSON(JSONFileName,"BlockVTrack"      ,BlockVTrack      );
    SaveAttrJSON(JSONFileName,"BlockVTrackEff"   ,BlockVTrackEff   );
    SaveAttrJSON(JSONFileName,"BlockEzPWAna"     ,BlockEzPWAna     );
    SaveAttrJSON(JSONFileName,"BlockFitTrick"    ,BlockFitTrick    );
    SaveAttrJSON(JSONFileName,"BlockFitTrickRes" ,BlockFitTrickRes );
    SaveAttrJSON(JSONFileName,"BlockFitPWAna"    ,BlockFitPWAna    );
    SaveAttrJSON(JSONFileName,"BlockFinalSelect" ,BlockFinalSelect );
    SaveAttrJSON(JSONFileName,"BlockRateVTime"   ,BlockRateVTime   );
      
    EndWriteJSON(JSONFileName);
    cout<<"Enter Setting:"<<endl;
    Show();  
    cout<<"Save setting to be "<<JSONFileName<<endl;
  }
  void ExportJSON(const char *JSONFiletmp){
    SetJSONName(JSONFiletmp);
    ExportJSON();
  }
  void Show(){
    cout<<"comment1\t"         <<"------------PathSetting-----------"    <<endl;
    cout<<"path_Lib\t"         <<path_Lib         <<endl;
    cout<<"path_Mac\t"         <<path_Mac         <<endl;
    cout<<"path_OpR\t"         <<path_OpR         <<endl;
    cout<<endl;
    cout<<"det_ver\t"          <<det_ver          <<endl;
    cout<<"path_Run\t"         <<path_Run         <<endl;
    cout<<endl;
    cout<<"path_MC\t"          <<path_MC          <<endl;
    cout<<"name_MC\t"          <<name_MC          <<endl;
    cout<<endl;
    cout<<"path_Raw\t"         <<path_Raw         <<endl;
    cout<<"path_Hk\t"          <<path_Hk          <<endl;
    cout<<"path_Rot\t"         <<path_Rot         <<endl;
    cout<<"name_Raw\t"         <<name_Raw         <<endl;
    cout<<"name_Hk\t"          <<name_Hk          <<endl;
    cout<<endl;  
    cout<<"PBCode\t"           <<PBCode           <<endl;
    cout<<endl;  
    cout<<"comment2\t"         <<"-----------AnalizeModes-----------"    <<endl;
    cout<<"MCMode\t"           <<MCMode           <<endl;
    cout<<endl;
    cout<<"ReRunRunMode\t"     <<ReRunRunMode     <<endl;
    cout<<"testMode\t"         <<testMode         <<endl;
    cout<<"normalizeMode\t"    <<normalizeMode    <<endl;
    cout<<"GammaCutMode\t"     <<GammaCutMode     <<endl;
    cout<<"NegHiRatChMode\t"   <<NegHiRatChMode   <<endl;
    cout<<"lowstatisticMode\t" <<lowstatisticMode <<endl;
    cout<<"rootFileMode\t"     <<rootFileMode     <<endl;
    cout<<"SETightMode\t"      <<SETightMode      <<endl;
    cout<<"PointTextMode\t"    <<PointTextMode    <<endl;
    cout<<"RotateSciMode\t"    <<RotateSciMode    <<endl;
    cout<<endl;
    cout<<"comment3\t"         <<"------------BlockModes------------"    <<endl;
    cout<<"SkipBlockStop\t"    <<SkipBlockStop    <<endl;
    cout<<endl;
    cout<<"BlockEnv\t"         <<BlockEnv         <<endl;
    cout<<"BlockODetImf\t"     <<BlockODetImf     <<endl;
    cout<<"BlockFindOriFile\t" <<BlockFindOriFile <<endl;
    cout<<"BlockConvertor\t"   <<BlockConvertor   <<endl;
    cout<<"BlockFindRFile\t"   <<BlockFindRFile   <<endl;
    cout<<"BlockDSLAna\t"      <<BlockDSLAna      <<endl;
    cout<<"BlockDayEff\t"      <<BlockDayEff      <<endl;
    cout<<"BlockEventGaps\t"   <<BlockEventGaps   <<endl;
    cout<<"BlockEzMuEID\t"     <<BlockEzMuEID     <<endl;
    cout<<"BlockVTrack\t"      <<BlockVTrack      <<endl;
    cout<<"BlockVTrackEff\t"   <<BlockVTrackEff   <<endl;
    cout<<"BlockEzPWAna\t"     <<BlockEzPWAna     <<endl;
    cout<<"BlockFitTrick\t"    <<BlockFitTrick    <<endl;
    cout<<"BlockFitTrickRes\t" <<BlockFitTrickRes <<endl;
    cout<<"BlockFitPWAna\t"    <<BlockFitPWAna    <<endl;
    cout<<"BlockFinalSelect\t" <<BlockFinalSelect <<endl;
    cout<<"BlockRateVTime\t"   <<BlockRateVTime   <<endl;
  }
private:
  void SetDataDefault(){
    MCMode    = false;

    path_MC        = Form("NULL");
    name_MC        = Form("NULL");
    
    ReRunRunMode    = true;
    testMode        = false; normalizeMode   = true;  GammaCutMode    = false;
    NegHiRatChMode  = false; lowstatisticMode= true;  rootFileMode    = true;
    SETightMode     = false; PointTextMode   = true;  RotateSciMode   = false;

    SkipBlockStop   = true; 
    
    BlockEnv        = true; BlockODetImf    = true; BlockFindOriFile= true; 
    BlockConvertor  = true; BlockFindRFile  = true; BlockDSLAna     = true; 
    BlockDayEff     = true; BlockEventGaps  = true; BlockEzMuEID    = true;
    BlockVTrack     = true; BlockVTrackEff  = true; BlockEzPWAna    = true;
    BlockFitTrick   = true; BlockFitTrickRes= true; BlockFitPWAna   = true;
    BlockFinalSelect= true; BlockRateVTime  = true;
  }
  void SetMotCDefault(){
    MCMode    = true;

    path_Raw       = Form("NULL");
    path_Hk        = Form("NULL");
    path_Rot       = Form("NULL");
    name_Raw       = Form("NULL");
    name_Hk        = Form("NULL");
    PBCode         = Form("1111");
    
    ReRunRunMode    = true;  
    testMode        = false; normalizeMode   = true;  GammaCutMode    = false;
    NegHiRatChMode  = false; lowstatisticMode= false; rootFileMode    = true;
    SETightMode     = true;  PointTextMode   = false; RotateSciMode   = false;

    SkipBlockStop   = true; 
    
    BlockEnv        = true; BlockODetImf    = true; BlockFindOriFile= false; 
    BlockConvertor  = true; BlockFindRFile  = true; BlockDSLAna     = true; 
    BlockDayEff     = false;BlockEventGaps  = true; BlockEzMuEID    = true;
    BlockVTrack     = true; BlockVTrackEff  = true; BlockEzPWAna    = true;
    BlockFitTrick   = true; BlockFitTrickRes= true; BlockFitPWAna   = true;
    BlockFinalSelect= true; BlockRateVTime  = true;
  }
  void ImportJSON(){
    cout<<"Open the: "<<JSONFileName<<endl;
    path_Lib       = FindJSONAttrS( JSONFileName, "path_Lib"          ,'W');
    path_Mac       = FindJSONAttrS( JSONFileName, "path_Mac"          ,'W');
    path_OpR       = FindJSONAttrS( JSONFileName, "path_OpR"          ,'W');
    
    det_ver        = FindJSONAttrS( JSONFileName, "det_ver"           ,'W');
    path_Run       = FindJSONAttrS( JSONFileName, "path_Run"          ,'W');
    
    path_MC        = FindJSONAttrS( JSONFileName, "path_MC"           ,'W');
    name_MC        = FindJSONAttrS( JSONFileName, "name_MC"           ,'W');
    
    path_Raw       = FindJSONAttrS( JSONFileName, "path_Raw"          ,'W');
    path_Hk        = FindJSONAttrS( JSONFileName, "path_Hk"           ,'W');
    path_Rot       = FindJSONAttrS( JSONFileName, "path_Rot"          ,'W');
    name_Raw       = FindJSONAttrS( JSONFileName, "name_Raw"          ,'W');
    name_Hk        = FindJSONAttrS( JSONFileName, "name_Hk"           ,'W');
    
    PBCode         = FindJSONAttrS( JSONFileName, "PBCode"            ,'W');
    
    MCMode          = FindJSONAttrB( JSONFileName, "MCMode"           ,'W');
    // cout<<"if mcornot "<<MCMode<<endl;
    ReRunRunMode    = FindJSONAttrB( JSONFileName, "ReRunRunMode"     ,'W');
    testMode        = FindJSONAttrB( JSONFileName, "testMode"         ,'W');
    normalizeMode   = FindJSONAttrB( JSONFileName, "normalizeMode"    ,'W');
    GammaCutMode    = FindJSONAttrB( JSONFileName, "GammaCutMode"     ,'W');
    NegHiRatChMode  = FindJSONAttrB( JSONFileName, "NegHiRatChMode"   ,'W');
    lowstatisticMode= FindJSONAttrB( JSONFileName, "lowstatisticMode" ,'W');
    rootFileMode    = FindJSONAttrB( JSONFileName, "rootFileMode"     ,'W');
    SETightMode     = FindJSONAttrB( JSONFileName, "SETightMode"      ,'W');
    PointTextMode   = FindJSONAttrB( JSONFileName, "PointTextMode"    ,'W');
    RotateSciMode   = FindJSONAttrB( JSONFileName, "RotateSciMode"    ,'W');
    
    SkipBlockStop   = FindJSONAttrB( JSONFileName, "SkipBlockStop"    ,'W');

    BlockEnv        = FindJSONAttrB( JSONFileName, "BlockEnv"         ,'W');
    BlockODetImf    = FindJSONAttrB( JSONFileName, "BlockODetImf"     ,'W');
    BlockFindOriFile= FindJSONAttrB( JSONFileName, "BlockFindOriFile" ,'W');
    BlockConvertor  = FindJSONAttrB( JSONFileName, "BlockConvertor"   ,'W');
    BlockFindRFile  = FindJSONAttrB( JSONFileName, "BlockFindRFile"   ,'W');
    BlockDSLAna     = FindJSONAttrB( JSONFileName, "BlockDSLAna"      ,'W');
    BlockDayEff     = FindJSONAttrB( JSONFileName, "BlockDayEff"      ,'W');
    BlockEventGaps  = FindJSONAttrB( JSONFileName, "BlockEventGaps"   ,'W');
    BlockEzMuEID    = FindJSONAttrB( JSONFileName, "BlockEzMuEID"     ,'W');
    BlockVTrack     = FindJSONAttrB( JSONFileName, "BlockVTrack"      ,'W');
    BlockVTrackEff  = FindJSONAttrB( JSONFileName, "BlockVTrackEff"   ,'W');
    BlockEzPWAna    = FindJSONAttrB( JSONFileName, "BlockEzPWAna"     ,'W');
    BlockFitTrick   = FindJSONAttrB( JSONFileName, "BlockFitTrick"    ,'W');
    BlockFitTrickRes= FindJSONAttrB( JSONFileName, "BlockFitTrickRes" ,'W');
    BlockFitPWAna   = FindJSONAttrB( JSONFileName, "BlockFitPWAna"    ,'W');
    BlockFinalSelect= FindJSONAttrB( JSONFileName, "BlockFinalSelect" ,'W');
    BlockRateVTime  = FindJSONAttrB( JSONFileName, "BlockRateVTime"   ,'W');
    
  }
  
  
protected:
  char *JSONFileName;
};

