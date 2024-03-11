  
  //V02.00
  /*Detector Information variable*/
  /*
  char detVer[10] = {"User"};  //detector version
  //number of boards, nuber of channel per board, number of board layers
  const int    NumBD =  4, NumCh = 16, NumLY = 4;
  //number of channels on x-axis per board, ... on y-axis ..., , ... on z-axis ..., 
  const int    NumnX =  4, NumnY =  4, NumnZ = 1;
  //number of boards on x-axis per layer, ... on y-axis ..., , ... on z-axis ..., 
  const int    NumBX =  1, NumBY =  1, NumBZ = 1;
  //number of total channel
  const int    BinNumOnCha = NumBD*NumCh;
  //number of total channel per layer
  const int    NumChLys    = BinNumOnCha/NumLY;
  //MTB ID of a board at position index(ix,iy,iz): BD[ix+NumBY*iy+NumBX*NumBY*iz]
  const int    BD[NumBD]   = {20,19,18,17};
  //pwidth scale factor of MTB board because the Magnification of DAQ at index x: BDPwWei[x]
  const int    BDPwWei[NumBD] = {1,1,1,1};
  //(MTB id)%NumLY at board position index(BiX,BiY): LayArr[BiX][BiY]
  const int    LayArr[NumBX][NumBY]={{1}};
  //Mother board MTB ID
  const int    MotherBoard = 17;
  //Size of Scintillator on X=bx, Y=by, Z=bz dim. , unit: mm
  const double bx  = 50., by  = 50., bz  = 12.;
  //gap size MTBs on X=bx, Y=by, Z=bz dim. , unit: mm
  const double gbx =  0 , gby =  0., gbz = 500.;
  //half size of Scintillator on XYZ dim
  const double bx_2 = bx/2., by_2 = by/2., bz_2 = bz/2.;
  */
    //V03.06
  /*Detector Information variable*/
  char detVer[10] = {"V3.05"};  //detector version
  //number of boards, nuber of channel per board, number of board layers
  const int    NumBD = 20, NumCh = 16, NumLY = 5;
  //number of channel on x-axis per board, ... on y-axis ..., , ... on z-axis ..., 
  const int    NumnX =  4, NumnY =  4, NumnZ = 1;
  //number of boards on x-axis per layer, ... on y-axis ..., , ... on z-axis ..., 
  const int    NumBX =  2, NumBY =  2, NumBZ = 1;
  //number of total channel
  const int    BinNumOnCha = NumBD*NumCh;
  //number of total channel per layer
  const int    NumChLys    = BinNumOnCha/NumLY;
  //MTB ID of a board at position index(ix,iy,iz): BD[ix+NumBY*iy+NumBX*NumBY*iz]
  const int    BD[NumBD]   = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
  //pwidth scale factor of MTB board because the Magnification of DAQ at index x: BDPwWei[x]
  const int    BDPwWei[NumBD] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  //(MTB id)%NumLY at board position index(BiX,BiY): LayArr[BiX][BiY]
  const int    LayArr[NumBX][NumBY]={{3,1},{4,2}};
  //Mother board MTB ID
  const int    MotherBoard = 1;
  //Size of Scintillator on X=bx, Y=by, Z=bz dim. , unit: mm
  const double bx  = 50., by  = 50., bz  = 12. ;
  //gap size MTBs on X=bx, Y=by, Z=bz dim. , unit: mm
  const double gbx = 0  , gby = 0. , gbz = 500.;//@@@
  //half size of Scintillator on XYZ dim
  const double bx_2 = bx/2., by_2 = by/2., bz_2 = bz/2.;