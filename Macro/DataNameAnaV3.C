//include
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "path_dir.h"
#include "AnaVariable.h"

using namespace std;
using namespace MuoAna_path_dir;
using namespace MuographAnaVariable;

//channel vs rate combine
void DataNameAnaV2(){
  cout<<"Please use the last version: DataNameAnaV3.C"<<endl; throw;
}
const int Num_modes = 4;
const char ModeChar[Num_modes] = {'R','G','M','H'};
const string ModeName[Num_modes] = {"Mu.root","MC.root","Mu.txt","HK.txt"};
const string ModeExt [Num_modes] = {".root",".root",".txt",".txt"};
const string Sor_DataPath[Num_modes] = {DirRotfile,DirRotfile,DirRawData,DirHkData};
const string Sor_DataName[Num_modes] = {name_Raw_root,name_RMC_root,name_Raw_txt,name_Hk_txt};
const string Res_DataNameDat[Num_modes] = {"DataRootNameAna.dat","DataRootNameAna.dat","DataTxtNameAna.dat","HKTxtNameAna.dat"};
const string str_ErrFileExpress[Num_modes] = {" Mu .root","Mc .root","Mu .txt","Hk .txt"};
const size_t Thr_FileSizes[Num_modes] = {50,4,50,50};
const string str_tmp_fileName = "tmp_sysGetName.dat";

int DataNameAnaV3(const char Mode, const char OPT='P'){
  cout<<"-----------test------------"<<endl;
  cout<<name_Raw_root<<endl;
  cout<<name_RMC_root<<endl;
  cout<<name_Raw_txt<<endl;
  cout<<name_Hk_txt<<endl;
  cout<<"-----------test------------"<<endl;
  int ModeFig = -1;
  if (Mode==ModeChar[0]) ModeFig = 0;
  else if (Mode==ModeChar[1]) ModeFig = 1;
  else if (Mode==ModeChar[2]) ModeFig = 2;
  else if (Mode==ModeChar[3]) ModeFig = 3;
  else {
    cout<<("Have not select the Mode of DataNameAnaV3().\n  The Mode should like this form:\n");
    for(int i=0;i<Num_modes;i++) 
      cout<<"'"<<ModeChar[i]<<"',";
    throw;
  }
  string path_target, path_saved;
  path_target = Sor_DataPath[ModeFig]+Sor_DataName[ModeFig];
  cout<<"finding raw .root:  "<<Sor_DataName[ModeFig]<<" file under dir:  "<<DirRotfile<<endl;
  // vector<string> files = find_files(path_target);
  string inTmpFilePN = Form("%s%s",DirOperate,str_tmp_fileName.data());
  cout<<(Form("ls -1 -s %s > %s",path_target.data(),inTmpFilePN.data()))<<endl;
  system(Form("ls -1 -s %s > %s",path_target.data(),inTmpFilePN.data()));
  // FATAL("53");
  ifstream inTmpFile(inTmpFilePN.data());
  size_t fileSize;
  string file_name;
  vector<string> files;
  while(inTmpFile>>fileSize>>file_name){
    if(fileSize<Thr_FileSizes[ModeFig]) 
      cout<<"Check if the file is Empty? : \n  "<<fileSize<<" "<<file_name<<endl;
    else if(fileSize>TriggerRFSize&&Mode!='M')
      cout<<"Warning: DataNameAnaV3:L62: file size of raw data is too large: \n  "<<fileSize<<"KB, "<<file_name<<endl;
    else 
      files.push_back(file_name);
  }
  cout<<(Form("Temp: save %s",str_tmp_fileName.data()))<<endl;

  path_saved = DirOperate+Res_DataNameDat[ModeFig];
  ofstream out(path_saved.data());
  cout<<"Save: "<< str_ErrFileExpress[Num_modes] <<" files:  \""<<Sor_DataName[ModeFig]<<"\"  name data in "<< path_saved <<endl;

  //cout<<"FileVectorSize:\t"<<files.size()<<endl;
  
  int nf = files.size();
  if(nf == 0){
    cout<<"Error: Not find any file of "<<str_ErrFileExpress<<"!!!"<<endl;
    cout<<"Please check the target name of "<<str_ErrFileExpress<<" or the existence of files!!!"<<endl;
    cout<<"Now, the target name of "<<str_ErrFileExpress<<" is:    "<<path_target<<endl;
    throw;
  }
  for(int i=0;i<nf;i++){
    if(i+1==int(files.size())){
      out<<files[i];
      if(OPT=='P') cout<<files[i]<<endl;
    }else{
      out<<files[i]<<endl;
      if(OPT=='P') cout<<files[i]<<endl;
    }
  }
  if(OPT!='P'){
    cout<<"hint: Skip Print the Information of Directory of All Raw .txt data.(!='P')"<<endl;
  }
  cout<<path_saved << " is saved"<<endl;
  
  system(Form("rm %s*",inTmpFilePN.data()));
  cout<<(Form("rm %s*",inTmpFilePN.data()))<<endl;
  return nf;
}

int DataNameAnaRoot_Mu(const char OPT='P') {
  char path_target[180],path_saved[180];
  sprintf(path_target,"%s%s",DirRotfile,name_Raw_root);
  cout<<"finding raw .root:  "<<name_Raw_root<<" file under dir:  "<<DirRotfile<<endl;
  vector<string> files = find_files(path_target);
  
  
  sprintf(path_saved,"%sDataRootNameAna.dat",DirOperate);
  ofstream out(path_saved);
  cout<<"Save: Raw .txt files:  \""<<name_Raw_root<<"\"  name data in "<< path_saved <<endl;

  //cout<<"FileVectorSize:\t"<<files.size()<<endl;
  
  int nf = files.size();
  if(nf == 0){
    cout<<"Error: Not find any file of Mu .root!!!"<<endl;
    cout<<"Please check the target name of Mu .root or the existence of files!!!"<<endl;
    cout<<"Now, the target name of Mu .root is:    "<<path_target<<endl;
    throw;
  }
  for(int i=0;i<nf;i++){
    if(i+1==int(files.size())){
      out<<files[i];
      if(OPT=='P') cout<<files[i]<<endl;
    }else{
      out<<files[i]<<endl;
      if(OPT=='P') cout<<files[i]<<endl;
    }
  }
  if(OPT!='P'){
    cout<<"hint: Skip Print the Information of Directory of All Raw .txt data.(!='P')"<<endl;
  }
  cout<<path_saved << " is saved"<<endl;
  return nf;
}
int MCNameAnaRoot_Mu(const char *MCFile, const char OPT='P') {
  char path_target[180],path_saved[180];
  sprintf(path_target,"%s%s",DirRotfile,MCFile);
  cout<<"finding raw .root:  "<<MCFile<<" file under dir:  "<<DirRotfile<<endl;
  vector<string> files = find_files(path_target);
  //@@
  
  sprintf(path_saved,"%sDataRootNameAna.dat",DirOperate);
  ofstream out(path_saved);
  cout<<"Save: Raw .txt files:  \""<<MCFile<<"\"  name data in "<< path_saved <<endl;

  //cout<<"FileVectorSize:\t"<<files.size()<<endl;
  
  int nf = files.size();
  if(nf == 0){
    cout<<"Error: Not find any file of Mu .root!!!"<<endl;
    cout<<"Please check the target name of Mu .root or the existence of files!!!"<<endl;
    cout<<"Now, the target name of Mu .root is:    "<<path_target<<endl;
    throw;
  }
  for(int i=0;i<nf;i++){
    if(i+1==int(files.size())){
      out<<files[i];
      if(OPT=='P') cout<<files[i]<<endl;
    }else{
      out<<files[i]<<endl;
      if(OPT=='P') cout<<files[i]<<endl;
    }
  }
  if(OPT!='P'){
    cout<<"hint: Skip Print the Information of Directory of All Raw .txt data.(!='P')"<<endl;
  }
  cout<<path_saved << " is saved"<<endl;
  return nf;
}


int DataNameAnaTxt_Mu(const char OPT='P') {
  
  char path_target[180],path_saved[180];
  sprintf(path_target,"%s%s",DirRawData,name_Raw_txt);
  cout<<"finding raw .txt:  \""<<name_Raw_txt<<"\"  file under dir:  "<<DirRawData<<endl;
  vector<string> files = find_filestxt(path_target);
  
  
  sprintf(path_saved,"%sDataTxtNameAna.dat",DirOperate);
  ofstream out(path_saved);
  cout<<"Save: Raw .txt files:  "<< name_Raw_txt <<" name data in "<< path_saved <<endl;

  //cout<<"FileVectorSize:\t"<<files.size()<<endl;
  
  int nf = files.size();
  if(nf == 0){
    cout<<"Error: Not find any file of Mu .txt!!!"<<endl;
    cout<<"Please check the target name of Mu .txt or the existence of files!!!"<<endl;
    cout<<"Now, the target name of Mu .txt is:    "<<path_target<<endl;
    throw;
  }
  for(int i=0;i<nf;i++){
    if(i+1==int(files.size())){
      out<<files[i];
      if(OPT=='P') cout<<files[i]<<endl;
    }else{
      out<<files[i]<<endl;
      if(OPT=='P') cout<<files[i]<<endl;
    }
  }
  
  if(OPT!='P'){
    cout<<"hint: Skip Print the Information of Directory of All Raw .txt data.(!='P')"<<endl;
  }
  cout<<path_saved << " is saved"<<endl;
  return nf;
}


int DataNameAnaTxt_Hk(const char OPT='P') {
  
  char path_target[180],path_saved[180];
  sprintf(path_target,"%s%s",DirHkData,name_Hk_txt);
  cout<<"finding Hk .txt:  \""<<name_Hk_txt<<"\"  file under dir:  "<<DirHkData<<endl;
  vector<string> files = find_filestxt(path_target);
  
  
  sprintf(path_saved,"%sHKTxtNameAna.dat",DirOperate);
  ofstream out(path_saved);
  cout<<"Save: Hk .txt files:  "<< name_Hk_txt <<" name data in "<< path_saved <<endl;

  //cout<<"FileVectorSize:\t"<<files.size()<<endl;
  int nf = files.size();
  if(nf == 0){
    cout<<"Error: Not find any file of Hk .txt!!!"<<endl;
    cout<<"Please check the target name of Hk .txt or the existence of files!!!"<<endl;
    cout<<"Now, the target name of Hk .txt is:    "<<path_target<<endl;
    throw;
  }
  for(int i=0;i<nf;i++){
    if(i+1==int(files.size())){
      out<<files[i];
      if(OPT=='P') cout<<files[i]<<endl;
    }else{
      out<<files[i]<<endl;
      if(OPT=='P') cout<<files[i]<<endl;
    }
  }
  
  if(OPT!='P'){
    cout<<"hint: Skip Print the Information of Directory of All HK .txt data.(!='P')"<<endl;
  }
  cout<<path_saved << " is saved"<<endl;
  return nf;
}
