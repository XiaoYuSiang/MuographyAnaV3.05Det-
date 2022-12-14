#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TColor.h>
#include "lineFit3D.h"

using namespace std;


namespace MuographAnaVariable
{
  
  /*Analysis variable */
  //th2fbinwidth is tim bin in Analysis
  const double th2fbinwidth = 3600;
  Int_t bindays = 1;
  //timeZone of Taiwan is UTC+8
  const int    timeZone     = +8 ;
  
  //breaktime is the time detector sleep
  const Int_t  breaktime   = 120;//(s)
  const Int_t  SlicePerDay =   6;//(24hr/SlicePerDay for one bin)
  
  //nHit Triggers from S -> L
  //nH of event in Trigger would be record
  const Int_t TriggerS = 1;
  const Int_t TriggerL = 64;
  
  //Rate ana. trigger
  //Trigger for how much RMS sigma would be error
  const Float_t Trigger_RateRMS = 1.5;
  
  //R4VT Rate Over high Trigger
  //if rate > trigger, fix max to trigger
  const Float_t TriggerRatOver = 1.4;//Hz

  //DayE Eff. Over Trigger 
  //if Eff > trigger, fix Eff = 100%
  const Float_t TriggerEffOver = 0.999;
  
  //DayE Eff. Trigger of Eff. for Hour Neglect Mode
  //if Eff < trigger, save the time range nformation
  const Float_t TriggerNegEffInHour = 0.9;
  
  //Straight track for measure Eff. of all channel
  //if NHit > trigger, Neglect the events
  const int TriggerNHitOfEffB4 = 0;
  const int TriggerNHitOfEffB3 = 0;
  //Rate similarity on sunny day
  //select the sunny day simple
  const Int_t UT_SelectDay = 1643212800; //ex: 2022/01/27 UTC+8
  const Int_t Num_SelectDay=3;
  Int_t SelectDaySec[Num_SelectDay]={
    1643212800+86400*0,
    1643212800+86400*1,
    1643212800+86400*2,
  };
  
  
  int neglectHourRange[2] = {11,15};
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  /*Art Option*/
  // the color for 16 kind color
  const int colorArr[16]={1,90,93,96,99,205,51,60,65,70,80};
  const int ColorArr9[9]={
    TColor::GetColor("#7882A4"),
    TColor::GetColor("#FF7979"),
    TColor::GetColor("#C0A080"),
    TColor::GetColor("#FFB770"),
    TColor::GetColor("#586284"),
    TColor::GetColor("#DF5959"),
    TColor::GetColor("#A08060"),
    TColor::GetColor("#DD9750"),
    1
  };
  const int ColorArr17[17]={
    TColor::GetColor("#7882A4"),
    TColor::GetColor("#FF7979"),
    TColor::GetColor("#C0A080"),
    TColor::GetColor("#FFB770"),
    TColor::GetColor("#586284"),
    TColor::GetColor("#DF5959"),
    TColor::GetColor("#A08060"),
    TColor::GetColor("#DD9750"),
    TColor::GetColor("#3C4152"),
    TColor::GetColor("#7FBCBC"),
    TColor::GetColor("#605040"),
    TColor::GetColor("#7FDBB8"),
    TColor::GetColor("#2C3142"),
    TColor::GetColor("#6FACAC"),
    TColor::GetColor("#504030"),
    TColor::GetColor("#6ECBA8"),
    1
  };
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  /* pwidth fitting parameters */
  const double SPLminP[3][7] ={//SetParLimitmin
    {0.15,-3.0,1.5                     },//Landau--muon data
    {0.20,-6.0,0.00,0.00,0.00,10.0,0.00},//Landau+Landau--0
    {0.60,-10.,2.00,.055,0.00,10.0,0.00},//Landau+gau--85
  };
  const double SPLmaxP[3][7] ={//SetParLimitmax
    {0.65,17.0,10.0                    },//Landau--muon data
    {2.00,10.0,0.25,3.0 ,0.5 ,25.0,5.00},//Landau+Landau--0
    {2.00,15.0,3.00,3.0 ,6.0 ,25.0,3.00},//Landau+gau--85
  };
  const double SPSP[3][7] ={
    {0.40,10.0,3.33                    },
    {0.18,4.00,3.7 ,0.2 ,14. ,3.7 ,    },
    {.194,4.00,0.0 ,2.3 ,3.0           },
  };
  const double SPP[5]=
    {.194,4.00,0.0 ,2.3 ,3.0           
  };
  
  char WeatherData[][5]={
    "sn","Su","Su",//0318~20
    "c","cr","c","st","su","su","su","su","su","c","cr",//21~0431
    "cr","c","su","c","cr","cr","r","cr","cr","w",//0401~10
    "cr","su","su","cr","su","su","su","c","cr","cr",//11~20
    "c","su","cr","c","su","su","cr","cr","cr","su",//21~0430
    "su","su","C","C","C","su","C","c","su","su",//0501~10
    "su","su","su","su","su","su","su","su","cr","su",//11~20
    "su","su","su","R","su","cr","su","cr","cr","r","r",//21~0531
    "r","cr","su"/*,"r","r","r","r","cr","r","r","su","cr"*///0601~0610
    
  };
  
  
  
  
  
  
  
  
  
  
  
  
  //V02.00
  /*Detector Information variable*/
  /*
  char detVer[10] = {"V2.00"};
  const int    NumBD = 4;
  const int    NumCh = 16;
  const int    NumLY = 4;
  const int    NumnX = 4, NumnY = 4, NumnZ = 4;
  const int    NumBX = 1, NumBY = 1, NumBZ = NumLY;
  const int    NumChLys = NumnX*NumBX*NumnY*NumBY;
  const int    BinNumOnCha = NumBD*NumCh;
  const int    BD[4]    = {1,3,5,4};
  const int    BDINV[6] = {0,0,0,1,3,2};
  const int    BDPwWei[4] = {2,1,1,1};
  const double bx  = 50., by  = 50., bz  = 12.;
  const double bx_2 = bx/2.;
  const double by_2 = by/2.;
  const double bz_2 = bz/2.;
  const int    binpw = 90;// for 90 pwidth = 9ms
  */
  
  //V03.00
  /*Detector Information variable*/
  /*
  char detVer[10] = {"V3.00"};
  const int    NumBD = 16;
  const int    NumCh = 16;
  const int    NumLY = 4;
  const int    NumnX = 4, NumnY = 4, NumnZ = 4;
  const int    NumBX = 2, NumBY = 2, NumBZ = NumLY;
  const int    NumChLys = NumnX*NumBX*NumnY*NumBY;
  const int    BinNumOnCha = NumBD*NumCh;
  const int    BD[NumCh]   = {1,2,3,4,9,10,11,12,5,6,7,8,13,14,15,16};
  const int    BDINV[NumCh+1] = {0,0,1,2,3,8,9,10,11,4,5,6,7,12,13,14,15};
  const int    BDPwWei[NumCh] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  const double bx  = 50., by  = 50., bz  = 12.;
  const double gbx = 12 , gby = 12., gbz = 500.;
  const double bx_2 = bx/2.;
  const double by_2 = by/2.;
  const double bz_2 = bz/2.;
  const int    binpw = 90;// for 90 pwidth = 9ms
  */
  

  //V03.05
  /*Detector Information variable*/

  char detVer[10] = {"V3.05"};
  const int    NumBD = 16;
  const int    NumCh = 16;
  const int    NumLY = 4;
  const int    NumnX = 4, NumnY = 4, NumnZ = 4;
  const int    NumBX = 2, NumBY = 2, NumBZ = NumLY;
  const int    NumChLys = NumnX*NumBX*NumnY*NumBY;
  const int    BinNumOnCha = NumBD*NumCh;
  const int    BD[NumCh]   = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  const int    BDINV[NumCh+1] = {0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  const int    BDPwWei[NumCh] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  const int    LayArr [NumBX][NumBY] = {{3,1},{4,2}};
  const double bx  = 50., by = 50., bz = 12.;
  const double gbx = 0  , gby = 0., gbz = 500.;
  const double bx_2 = bx/2.;
  const double by_2 = by/2.;
  const double bz_2 = bz/2.;
  const int    binpw = 90;// for 90 pwidth = 9ms

  
  
  /*Ana constant don't change!!!*/
  //the setting of Ev gap 
  const int eventGapTcnt[50]= {
    2,5,8,10,13,16,20,24,28,30,
    31,32,34,36,38,40,42,46,50,55,
    60,70,80,90,100,90,100,120,140,160,
    180,200,250,300,350,400,500,600,700,800,
    1000,1200,1400,1600,1800,2000,2400,2800,3200,3500
  };
  /*Ana constant don't change!!!*/
  
  const int RunUTTableI[35]={
    0 ,1 ,2 ,3 ,4 ,
    5 ,6 ,7 ,8 ,9 ,
    10,11,12,13,14,
    15,16,17,18,19,
    20,21,22,23,24,
    25,26,27,28,29,
    30,31,32,33,34,
  };
  
  const char SE_CaseA[80]={
    "(nLayers == 4 && nH == 4)"};
  const char SE_CaseB[80]={
    "(nLayers == 3 && nH0== 0 && nH == 3)"};
    
  const char   Sites[4][10]={   "S4_509",  "Shimen",   "S4_11F",    "Da_Xi"};
  const float  Latitudes[4]={ 24.839435,  24.823633,  24.971514,  24.971510};
  const float Longitudes[4]={121.271228, 121.286611, 121.191712, 121.192117};
};
/*

  const int RunUTTable0[35]={
    1616402214,1617090467,1617195712,1617251489,1617532292,
    1617691437,1617740199,1617821571,1618028712,1618729775,
    1619066437,1619427832,1620101351,1620272279,1621325802,
    1621706071,1622283762,1622623348,1622973571,1623209803,
    1623340809,1623570495,1623577265,1623954442,1624291205,
    1624523770,1624636804,1624723205,1625220911,1626061020,
    1626422686,1626796809,1626969602,1627275911,1627977362,
  };
  const int RunUTTableF[35]={
    1617087301,1617194960,1617249884,1617527743,1617671607,
    1617724805,1617807289,1618023280,1618729039,1619064847,
    1619425764,1620100000,1620271690,1621320806,1621688525,
    1622269293,1622622613,1622969783,1623208886,1623308442,
    1623570373,1623576861,1623952995,1624255266,1624387315,
    1624600926,1624665531,1625219032,1626059746,1626422225,
    1626766980,1626937073,1627272999,1627706241,1628137813,
  };

*/