  //V03.05
  /*Detector Information variable*/
  char detVer[10] = {"V0203"};  //detector version
  //number of boards, nuber of channel per board, number of board layers
  const int    NumBD = 4, NumCh = 16, NumLY = 4;
  //number of channel on x-axis per board, ... on y-axis ..., , ... on z-axis ..., 
  const int    NumnX =  4, NumnY =  4, NumnZ = 1;
  //number of boards on x-axis per layer, ... on y-axis ..., , ... on z-axis ..., 
  const int    NumBX =  1, NumBY =  1, NumBZ = 1;
  //number of total channel
  const int    BinNumOnCha = NumBD*NumCh;
  //number of total channel per layer
  const int    NumChLys    = BinNumOnCha/NumLY;
  //MTB ID of a board at position index(ix,iy,iz): BD[ix+NumBY*iy+NumBX*NumBY*iz]
  const int    BD[NumBD]   = {2,20,17,16};
  //MTB ID of a board at position index(ix,iy,iz): BD[ix+NumBY*iy+NumBX*NumBY*iz]
  const int    BDLay[NumBD]= {0,1,2,3};
  //pwidth scale factor of MTB board because the Magnification of DAQ at index x: BDPwWei[x]
  const int    BDPwWei[NumBD] = {1,1,1,1};
  //(MTB id)%NumLY at board position index(BiX,BiY): LayArr[BiX][BiY]
  const int    LayArr[NumBX][NumBY]={{1}};
  //Mother board MTB ID
  const int    MotherBoard = 2;
  //Size of Scintillator on X=bx, Y=by, Z=bz dim. , unit: mm
  const double bx  = 50., by  = 50., bz  = 12. ;
  //gap size MTBs on X=bx, Y=by, Z=bz dim. , unit: mm
  const double gbx = 0  , gby = 0. , gbz = 110.;
  //half size of Scintillator on XYZ dim
  const double bx_2 = bx/2., by_2 = by/2., bz_2 = bz/2.;