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
  const int    timeZone    = +8 ;
  
  //breaktime is the time detector sleep
  const Int_t  breaktime   = 120;//(s)
  const Int_t  SlicePerDay =   6;//(24hr/SlicePerDay for one bin)
  const Int_t  TGapTHVPos  = 1800;//(s) time gap of THVPosAna shows 
  
    //Variable for the startRun index Ex: if experiment data start from Run01 SSR = 1
  const Int_t  StartSearchRun = 0;
  
  //nHit Triggers from S -> L
  //nH of event in Trigger would be record
  const Int_t TriggerS = 1;
  const Int_t TriggerL = 64;
  
  //Rate ana. trigger
  //Trigger for how much RMS sigma would be error
  const Float_t Trigger_RateRMS = 1.5;
  
  //R4VT Rate Over high Trigger
  //if rate > trigger, fix max to trigger
  const Float_t TriggerRatOver = 2;

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
  
  //Line fitting tracking for the hit number setting 
  //if NHit not in trigger range (S,L), skip the events
  const int TriggerTraNHitS = 4;
  const int TriggerTraNHitL = 4;
  
  //Line fitting tracking for the fire layer number setting 
  //if NHit smaller than trigger , skip the events
  const int TriggerTrNLayer = 4;
  
  //Line fitting tracking for the sigma range setting on XZ, YZ
  //if fitting error larger than trigger(size of quarter scintillator(mm))
  //  on X or Y dim, skip the events
  const float TriggerTrFitSigSizeX = 12.5*1.414;
  const float TriggerTrFitSigSizeY = 12.5*1.414;
  
  //minimum of reasonable unixtime in program
  //if UT < TriggerUTLeast, skip the data
  const Float_t TriggerUTLeast = 1.6E+9;
  
  //minimum of channel efficiency in program
  //if Eff < TriggerChanEff, skip to save the data
  const Float_t TriggerChanEff = 0.9;
  
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
  
  //Direction vector table division on x and y
  const int binNumFluxX = 281;//93;
  const int binNumFluxY = 281;//93;
  
  const bool NegLowSta = true;
  const bool GammaCut  = false; // if pwidth ==0 neglect this hit in EventAna.C
  
  // if Selection Event number of some Combination Lower than trigger, Neglect.
  const Float_t TriggerNegLowSEComb = 0;//15;
  
  //Neglect the data which DxDz or DyDz < Trigger
  const Float_t TriggerLessDxDz = -100;
  const Float_t TriggerLessDyDz = -0.11;//0.11;
  
  //Not recreate the GapT file when size > TriggerEventAnaSkip% of the predict size
  
  const Float_t TriggerEventAnaSkip = 0.8;
  
  
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
  
  
  const int    binpw = 90;// for 90 pwidth = 9ms
  const int V200BTSpXYZ [3][6]= 
  {
    {165, 25,175,175, 25,100},
    { 52, 50, 50,150,150,100},
    {-25,-14,-14,-14,-14,-14} 
  };
  const int V305BTSpXYZ [2][3][6]= 
  {
    { { 35,175, 25, 25,175,100},
      { 52, 50, 50,150,150,100},
      { 37, 26, 26, 26, 26,-14} },
    { {165, 25,175,175, 25,100},
      { 52, 50, 50,150,150,100},
      {-25,-14,-14,-14,-14,-14} }
  };
  
  const float V200AngRang[2][5][2]={
    {{0,0},{0,0},{-0.45,0.45},{-0.25,0.25},{-0.15,0.15}},
    {{0,0},{0,0},{-0.45,0.45},{-0.25,0.25},{-0.15,0.15}}
  };
  const float V305AngRang[2][5][2]={
    {{0,0},{0,0},{-0.8,0.8},{-0.6,0.6},{-0.3,0.3}},
    {{0,0},{0,0},{-0.8,0.8},{-0.6,0.6},{-0.3,0.3}}
  };
  
  const float V200AngDmM[3] = { 76,-0.15,0.15};
  const float V305AngDmM[3] = {401,-0.80,0.80};
  
  
  /*Ana constant don't change!!!*/
  //the setting of Ev gap 
  const int eventGapTcnt[46]= {
    2,5,8,10,13,16,20,24,28,30,
    31,32,34,36,38,40,42,46,50,55,
    60,70,80,90,100,90,100,120,140,160,
    180,200,250,300,350,400,500,600,700,800,
    1000,1200,1400,1600,1800,2000
  };
  //how many Byte /s (pass GT cut)
  const double TimeSizeRate = 1250;
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
  //const float  Altitudes[4]={121.271228, 121.286611, 121.191712, 121.192117};
  
  /*G4 Convert setting*/
  
  //G4 channel GID definition
  int G4GIDToGID[2][256] = //Transfer the G4GID to be GID
  {
    {// the G4 GID to GID for V3.00
      44,  45,  46,  47,  60,  61,  62,  63,  40,  41,  42,  43,  56,  57,  58,  59,
      36,  37,  38,  39,  52,  53,  54,  55,  32,  33,  34,  35,  48,  49,  50,  51,
      12,  13,  14,  15,  28,  29,  30,  31,  8,  9,  10,  11,  24,  25,  26,  27,
      4,  5,  6,  7,  20,  21,  22,  23,  0,  1,  2,  3,  16,  17,  18,  19,
      108,  109,  110,  111,  124,  125,  126,  127,  104,  105,  106,  107,  120,  121,  122,  123,
      100,  101,  102,  103,  116,  117,  118,  119,  96,  97,  98,  99,  112,  113,  114,  115,
      76,  77,  78,  79,  92,  93,  94,  95,  72,  73,  74,  75,  88,  89,  90,  91,
      68,  69,  70,  71,  84,  85,  86,  87,  64,  65,  66,  67,  80,  81,  82,  83,
      172,  173,  174,  175,  188,  189,  190,  191,  168,  169,  170,  171,  184,  185,  186,  187,
      164,  165,  166,  167,  180,  181,  182,  183,  160,  161,  162,  163,  176,  177,  178,  179,
      140,  141,  142,  143,  156,  157,  158,  159,  136,  137,  138,  139,  152,  153,  154,  155,
      132,  133,  134,  135,  148,  149,  150,  151,  128,  129,  130,  131,  144,  145,  146,  147,
      236,  237,  238,  239,  252,  253,  254,  255,  232,  233,  234,  235,  248,  249,  250,  251,
      228,  229,  230,  231,  244,  245,  246,  247,  224,  225,  226,  227,  240,  241,  242,  243,
      204,  205,  206,  207,  220,  221,  222,  223,  200,  201,  202,  203,  216,  217,  218,  219,
      196,  197,  198,  199,  212,  213,  214,  215,  192,  193,  194,  195,  208,  209,  210,  211
    },
    {// the G4 GID to GID for V3.05
      63,  62,  61,  60,  44,  45,  46,  47,  59,  58,  57,  56,  40,  41,  42,  43,
      55,  54,  53,  52,  36,  37,  38,  39,  51,  50,  49,  48,  32,  33,  34,  35,
      12,  13,  14,  15,  31,  30,  29,  28,  8,  9,  10,  11,  27,  26,  25,  24,
      4,  5,  6,  7,  23,  22,  21,  20,  0,  1,  2,  3,  19,  18,  17,  16,
      127,  126,  125,  124,  108,  109,  110,  111,  123,  122,  121,  120,  104,  105,  106,  107,
      119,  118,  117,  116,  100,  101,  102,  103,  115,  114,  113,  112,  96,  97,  98,  99,
      76,  77,  78,  79,  95,  94,  93,  92,  72,  73,  74,  75,  91,  90,  89,  88,
      68,  69,  70,  71,  87,  86,  85,  84,  64,  65,  66,  67,  83,  82,  81,  80,
      191,  190,  189,  188,  172,  173,  174,  175,  187,  186,  185,  184,  168,  169,  170,  171,
      183,  182,  181,  180,  164,  165,  166,  167,  179,  178,  177,  176,  160,  161,  162,  163,
      140,  141,  142,  143,  159,  158,  157,  156,  136,  137,  138,  139,  155,  154,  153,  152,
      132,  133,  134,  135,  151,  150,  149,  148,  128,  129,  130,  131,  147,  146,  145,  144,
      255,  254,  253,  252,  236,  237,  238,  239,  251,  250,  249,  248,  232,  233,  234,  235,
      247,  246,  245,  244,  228,  229,  230,  231,  243,  242,  241,  240,  224,  225,  226,  227,
      204,  205,  206,  207,  223,  222,  221,  220,  200,  201,  202,  203,  219,  218,  217,  216,
      196,  197,  198,  199,  215,  214,  213,  212,  192,  193,  194,  195,  211,  210,  209,  208
    }
  };
  
  const Int_t    G4TheFirstUT  = 1600000001;
    //Suppose the first unixtime in data is TheFirstUT
  const Int_t    G4FramIn1Sec  = 1100;
    //suppose in one second contain 1100 frames
  const Double_t G4EdeptoPWit  = 6.55;
    //suppose pwidth(100s) = EdeptoPWit*Edep(GeV)
  const Double_t G4eventGap    = 140;
    //suppose pwidth(100s) = EdeptoPWit*Edep(GeV)
  
  /*MTB System const*/
  const Int_t    MTBSysMaxBID  = 256;

  //V03.05
  /*Detector Information variable*/
  char detVer[10] = {"V3.05"};  //detector version
  //number of boards, nuber of channel per board, number of board layers
  const int    NumBD = 16, NumCh = 16, NumLY = 4;
  //number of channel on x-axis per board, ... on y-axis ..., , ... on z-axis ..., 
  const int    NumnX =  4, NumnY =  4, NumnZ = 1;
  //number of boards on x-axis per layer, ... on y-axis ..., , ... on z-axis ..., 
  const int    NumBX =  2, NumBY =  2, NumBZ = 1;
  //number of total channel
  const int    BinNumOnCha = NumBD*NumCh;
  //number of total channel per layer
  const int    NumChLys    = BinNumOnCha/NumLY;
  //MTB ID of a board at position index(ix,iy,iz): BD[ix+NumBY*iy+NumBX*NumBY*iz]
  const int    BD[NumBD]   = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  //pwidth scale factor of MTB board because the Magnification of DAQ at index x: BDPwWei[x]
  const int    BDPwWei[NumBD] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  //(MTB id)%NumLY at board position index(BiX,BiY): LayArr[BiX][BiY]
  const int    LayArr[NumBX][NumBY]={{3,1},{4,2}};
  //Mother board MTB ID
  const int    MotherBoard = 1;
  //Size of Scintillator on X=bx, Y=by, Z=bz dim. , unit: mm
  const double bx  = 50., by  = 50., bz  = 12. ;
  //gap size MTBs on X=bx, Y=by, Z=bz dim. , unit: mm
  const double gbx = 0  , gby = 0. , gbz = 500.;
  //half size of Scintillator on XYZ dim
  const double bx_2 = bx/2., by_2 = by/2., bz_2 = bz/2.;
}