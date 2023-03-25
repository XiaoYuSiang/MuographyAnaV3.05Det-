#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>

namespace DataConst{
  class DSLData{
  public:
    DSLData(){}
    virtual ~DSLData(){}

    void SetTStr(const char TStr, const char TStrTitle){
      
    }
    void SetTotalNum(const Long64_t Evs){
      
    }
    void SetFirT(
      const Int_t    iunixtimei  ,
      const Int_t    iYeari      ,
      const Int_t    iMoni       ,
      const Int_t    iDayi       ,
      const Int_t    iHouri      ,
      const Int_t    iMini       ,
      const Int_t    iSeci       ,
      const Int_t    iFirDaySec  
    ){
      
    }
    void SetFinT(
      const Int_t    iunixtimef  ,
      const Int_t    iYearf      ,
      const Int_t    iMonf       ,
      const Int_t    iDayf       ,
      const Int_t    iHourf      ,
      const Int_t    iMinf       ,
      const Int_t    iSecf       ,
      const Int_t    iFinDaySec  
    ){
      
    }
    void SetOverView(
      const Int_t    ndivise    ,
      const Int_t    OOL        ,
      const Int_t    TotalTime  ,
      const Int_t    TotalDTime ,
      const Int_t    BinOnTime  ,
      const Int_t    BinOnTimeD ,
      const Int_t    yetSec     ,
      const Int_t    WorkTime   ,
      const Float_t  RateWei    
    ){
        
    }
    void SetEffRun(){
      
    }
  private:
    /*Time string which can use on file name*/
    char     TimeStr[25];     
     //Ex: "S20220830E20230107";
    char     TimeStrTitle[50];
     //Ex: "Date From  2022/08/30 to 2023/01/07";
    
    /*Total Event Numbers Of Input Files*/
    Long64_t evs        ; //Ex: 15839178;
    
    /*time Information of the first event*/
    Int_t    unixtimei  ; //Ex: 1661838805;
    Int_t    Yeari      ; //Ex: 2022;
    Int_t    Moni       ; //Ex: 8;
    Int_t    Dayi       ; //Ex: 30;
    Int_t    Houri      ; //Ex: 13;
    Int_t    Mini       ; //Ex: 53;
    Int_t    Seci       ; //Ex: 25;
    Int_t    FirDaySec  ; //Ex: 1661788800;
  
    /*time Information of the last event*/
    Int_t    unixtimef  ; //Ex: 1673020809;
    Int_t    Yearf      ; //Ex: 2023;
    Int_t    Monf       ; //Ex: 1;
    Int_t    Dayf       ; //Ex: 7;
    Int_t    Hourf      ; //Ex: 0;
    Int_t    Minf       ; //Ex: 0;
    Int_t    Secf       ; //Ex: 9;
    Int_t    FinDaySec  ; //Ex: 1673107200;

    /*Information of the time bin or chip operate*/
    Int_t    ndivise    ; //Ex: 131;
    Int_t    OOL        ; //Ex: 8;
    Int_t    TotalTime  ; //Ex: 11182004;
    Int_t    TotalDTime ; //Ex: 11318400;
    Int_t    BinOnTime  ; //Ex: 3106;
    Int_t    BinOnTimeD ; //Ex: 3144;
    Int_t    yetSec     ; //Ex: 36395;
    Int_t    WorkTime   ; //Ex: 11182004;
    Float_t  RateWei    ; //Ex: 8.94294e-08;
    
    /*Effcitive run number*/
    int     *RunID;
    //EX: Tmp_RunID[12] = {0,1,-1,-1,-1,-1,-1,-1,8,9,10,11};
    //    RunID = Tmp_RunID
    
  };
}
