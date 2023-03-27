#include "/data4/YuSiang/personalLib/FileIO/FileIO.h"

void SingleExe(
  const char *Settingfile = "/home/yusiang/TestV3/Ana20230210_20230308.json"
){
  if(strcmp(Settingfile,"")==0){
    cerr<<"No file in"<<endl;
    throw;
  } 
  cout<<"Open the "<<Settingfile<<endl;
  char *path_Lib = FindJSONAttrS( Settingfile, "path_Lib").data();
  char *path_Raw = FindJSONAttrS( Settingfile, "path_Raw").data();
  char *path_Hk  = FindJSONAttrS( Settingfile, "path_Hk" ).data();
  char *path_Run = FindJSONAttrS( Settingfile, "path_Run").data();
  char *path_Rot = FindJSONAttrS( Settingfile, "path_Rot").data();
  char *path_OpR = FindJSONAttrS( Settingfile, "path_OpR").data();
  char *path_Mac = FindJSONAttrS( Settingfile, "path_Mac").data();
  char *name_Raw = FindJSONAttrS( Settingfile, "name_Raw").data();
  char *name_Hk  = FindJSONAttrS( Settingfile, "name_Hk" ).data();
  char *runOPT   = FindJSONAttrS( Settingfile, "runOPT"  ).data();
    
  gROOT->LoadMacro("./MainControl.C");
  MainControl(path_Lib ,  path_Raw  ,  path_Hk  ,  path_Run ,  path_Rot,
              path_OpR ,  path_Mac  ,  name_Raw ,  name_Hk  ,  runOPT  );
  
}

