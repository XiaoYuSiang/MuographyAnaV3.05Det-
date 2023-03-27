  //V02.00
  /*Detector Information variable*/
  char detVer[10] = {"V2.00"};  //detector version
  //number of boards, nuber of channel per board, number of board layers
  const int    NumBD =  4, NumCh = 16, NumLY = 4;
  //number of channels on x-axis per board, ... on y-axis ..., , ... on z-axis ..., 
  const int    NumnX =  4, NumnY =  4, NumnZ = 1;
  //number of boards on x-axis per layer, ... on y-axis ..., , ... on z-axis ..., 
  const int    NumBX =  1, NumBY =  1, NumBZ = NumLY;
  //number of total channel
  const int    BinNumOnCha = NumBD*NumCh;
  //number of total channel per layer
  const int    NumChLys    = BinNumOnCha/NumLY;
  //MTB ID of a board at position index(ix,iy,iz): BD[ix+NumBY*iy+NumBX*NumBY*iz]
  const int    BD[NumBD]   = {1,3,5,4};
  //board index of MTB ID is X: BD[X]
  const int    BDINV[NumBD+2] = {0,0,0,1,3,2};
  //pwidth scale factor of MTB board because the Magnification of DAQ at index x: BDPwWei[x]
  const int    BDPwWei[NumBD] = {2,1,1,1};
  //(MTB id)%NumLY at board position index(BiX,BiY): LayArr[BiX][BiY]
  const int    LayArr[NumBX][NumBY]={{1}};
  //Mother board MTB ID
  const int    MotherBoard = 1;
  //Size of Scintillator on X=bx, Y=by, Z=bz dim. , unit: mm
  const double bx  = 50., by  = 50., bz  = 12.;
  //gap size MTBs on X=bx, Y=by, Z=bz dim. , unit: mm
  const double gbx =  0 , gby =  0., gbz = 500.;
  //half size of Scintillator on XYZ dim
  const double bx_2 = bx/2., by_2 = by/2., bz_2 = bz/2.;