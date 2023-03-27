  //V03.00
  /*Detector Information variable*/
  char detVer[10] = {"V3.00"};  //detector version
  //number of boards, nuber of channel per board, number of board layers
  const int    NumBD = 16, NumCh = 16, NumLY = 4;
  //number of channel on x-axis per board, ... on y-axis ..., , ... on z-axis ..., 
  const int    NumnX =  4, NumnY =  4, NumnZ = 1;
  //number of boards on x-axis per layer, ... on y-axis ..., , ... on z-axis ..., 
  const int    NumBX =  2, NumBY =  2, NumBZ = NumLY;
  //number of total channel
  const int    BinNumOnCha = NumBD*NumCh;
  //number of total channel per layer
  const int    NumChLys    = BinNumOnCha/NumLY;
  //MTB ID of a board at position index(ix,iy,iz): BD[ix+NumBY*iy+NumBX*NumBY*iz]
  const int    BD[NumBD]   = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  //board index of MTB ID is X: BD[X]
  const int    BDINV[NumBD+1] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
  //pwidth scale factor of MTB board because the Magnification of DAQ at index x: BDPwWei[x]
  const int    BDPwWei[NumBD] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  //(MTB id)%NumLY at board position index(BiX,BiY): LayArr[BiX][BiY]
  const int    LayArr[NumBX][NumBY]={{3,1},{4,2}};
  //Mother board MTB ID
  const int    MotherBoard = 1;
  //Size of Scintillator on X=bx, Y=by, Z=bz dim. , unit: mm
  const double bx  = 50., by  = 50., bz  = 12.;
  //gap size MTBs on X=bx, Y=by, Z=bz dim. , unit: mm
  const double gbx = 12 , gby = 12., gbz = 500.;
  //half size of Scintillator on XYZ dim
  const double bx_2 = bx/2., by_2 = by/2., bz_2 = bz/2.;