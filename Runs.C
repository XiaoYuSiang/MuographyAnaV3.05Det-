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
    
  int iRunStart = 0, iRunFinal = 2;
    
  const TString Ori_path_Mac = "./Macro/";
  const TString path_Raw = "/data4/YuSiang/DaXi/";
  const TString path_Hk  = "/data4/YuSiang/DaXi/HKData/";
  const TString path_Run = "/data4/YuSiang/DaXi/OdetData/";
  const TString path_Rot = "/data4/YuSiang/DaXi/rootfile/";
  char   path_OpR[100]; //= "/data4/YuSiang/DaXi/Ana/";
  char   path_Mac[100]; //= "./Macro1/";
  char   name_Raw[100]; 
  char   name_Hk [100]; 
  TString RunOPT = "R"; 

  for(int i=iRunStart; i<=iRunFinal;i++){
    sprintf(path_OpR,"/data4/YuSiang/DaXi/AnaRun%02.f/",i*1.);
    sprintf(path_Mac,"%sRun%02.f/",path_MacroTmp,i*1.);
    sprintf(name_Raw,"*Run%d*Mu",i);
    sprintf(name_Hk ,"*Run%d*HK",i);
    if(i!=0) RunOPT= "C";
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
      +Addcommon(TString(RunOPT  ))+"\\) &"
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

/*
root -l MainControl.C\(\"./Macro/\",\"/data4/YuSiang/DaXi/\",\"/data4/YuSiang/DaXi/HKData/\",\"/data4/YuSiang/DaXi/rootfile/\",\"/data4/YuSiang/DaXi/Ana/\",\"./Macro2/\",\"*_Mu\",\"*_HK\"\) 

*/
/*
root -l MainControl.C\("./Macro/","/data4/YuSiang/DaXi/","/data4/YuSiang/DaXi/","/data4/YuSiang/DaXi/rootfile/","/data4/YuSiang/DaXi","/data4/YuSiang/DaXi","*_Mu","*_HK"\)
root -l MainControl.C\("./Macro/","/data4/YuSiang/DaXi/","/data4/YuSiang/DaXi/","/data4/YuSiang/DaXi/rootfile/","/data4/YuSiang/DaXi","/data4/YuSiang/DaXi","*_Mu","*_HK"\)
root -l MainControl.C\("./Macro/","/data4/YuSiang/DaXi/","/data4/YuSiang/DaXi/HKData/","/data4/YuSiang/DaXi/rootfile/","/data4/YuSiang/DaXi/Ana/","./Macro2/","*_Mu","*_HK"\)

root -l MainControl.C\(\"./Macro/\",\"/data4/YuSiang/DaXi/\",\"/data4/YuSiang/DaXi/HKData/\",\"/data4/YuSiang/DaXi/rootfile/\",\"/data4/YuSiang/DaXi/Ana/\",\"./Macro2/\",\"*_Mu\",\"*_HK\"\) 

*/