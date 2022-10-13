//include
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include "/home/yusiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "path_dir.h"

using namespace std;
using namespace MuoAna_path_dir;

//channel vs rate combine
void DataNameAna(){
  
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
