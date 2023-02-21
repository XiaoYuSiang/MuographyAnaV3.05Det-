//include
#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TH1F.h>
#include <TH2F.h>
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang.h"
#include "/data4/YuSiang/personalLib/Math/UnixTranslator.h"

using namespace std;
/*program constant variable don't change!!!*/

const Double_t tcntOneSec  = 2.56E+8;
  //The tcntOneSec tcnt = 1 second
const Int_t    TheFirstUT  = 1600000001;
  //Suppose the first unixtime in data is TheFirstUT
const Int_t    FramIn1Sec  = 1100;
  //suppose in one second contain 1100 frames
const Double_t EdeptoPWit  = 6.55;
  //suppose pwidth(100s) = EdeptoPWit*Edep(GeV)
  
/*Finish program constant variable setting!*/

/*Adjustable variable */
const char VectorInputRootFilesDefault[200] = 
  "/data4/chengwei/to_xiao/ED_t3.root"
;
const char OuputRootFilePathName      [200] = 
  "./"
;//Format:   Path/Name(GapT*_MC_nHTH*L*.root)

const Int_t MaximumEventGapOfTcnt          = 300;
const Int_t TriggerLessNumberOfHitInEvent  = 1;
const Int_t TriggerLargeNumberOfHitInEvent = 64;

int GetBIDbyG4GID(const int G4GID){
  return int(G4GID)/16;
}
int GetCIDbyG4GID(const int G4GID){
  return int(G4GID)/4+(int(G4GID)%4)*4;
}

void G4EventAna(
  const char vRootFilesin[200] = VectorInputRootFilesDefault,
  const char OORtFN      [200] = OuputRootFilePathName,
  const int  TriggerS          = TriggerLessNumberOfHitInEvent,
  const int  TriggerL          = TriggerLargeNumberOfHitInEvent,
  const int  eventGapTcnt      = MaximumEventGapOfTcnt
) {
  //Input Data Read of TTree by TreeReader in untuplizerv8
  TreeReader data(vRootFilesin,"mu_detector");
  cout<<Form("Root data(Tree) opening from: %s",vRootFilesin)<<endl;
  
  char SaveName[250];
  sprintf(
    SaveName,"%sGapT%d_%s_nHTH%dL%d.root",
    OORtFN,eventGapTcnt,"MC",TriggerS,TriggerL
  );
  //Located Output TTree Data to be GapT*_MC_nHTH*L*.root
  TFile *rotfile = new TFile(SaveName,"RECREATE");
  cout<<Form("Root save to: %s",SaveName)<<endl;
  
  //Data Variable initialize
  Int_t      EvIndex=0; // the index of the event
  Int_t      frame_   ; // the frame number of the event
  Long64_t   unixtime_; // the computer unixtime time of this event(s)
  Int_t      tYear    ; // the year  of this unixtime
  Int_t      tMonth   ; // the Month of this unixtime
  Int_t      tDate    ; // the Date...
  Int_t      tHour    ; // the Hour...
  Int_t      tMinute  ; // the Minute...
  Int_t      tSecond  ; // the Second...
  //PS:2560000000 Hz
  Int_t      nLayers ;
  Int_t      nH      ;  // number of hit in one event
  Int_t      nH0, nH1, nH2, nH3; // nh"z" = number of hit on the "z" layer
  Int_t      channel_;  // Channel ID in G4 setup
  Double_t   Edep_   ;  // Energy deposit
  
  vector<Double_t>  dtime;    // the hit time calculate by tcnt and pcnt(s)
  vector<Long64_t>  dtimeInt; // the hit time calculate by tcnt and pcnt(1/2.56E+8 s)
  vector<Int_t>     channel;  // channel ID of the hit on the board
  vector<Int_t>     board;    // board ID of hit
  vector<Int_t>     pwidth;   // pwidth = 1 -> signal larger than thershold for 100ms
  vector<Int_t>     pwidthScaleFactor; // unimportant now
  vector<Int_t>     pcnt;     // the number of the board get the PPS signal(s)
  vector<Long64_t>  tcnt;     // the number of the board time clock count(1/2.56E+8 s)
  vector<Double_t>  dtcnt;    // the tcount difference between the last and this hit
  Double_t          dTimeOfEv;// unimportant now
  
  //Setting input TTree read way
  TTree *tree = new TTree("t","data from G4 analyzing file");
  tree->Branch("frame",&frame_);
  tree->Branch("EvIndex",&EvIndex);
  tree->Branch("unixtime",&unixtime_);
  tree->Branch("tYear",&tYear);
  tree->Branch("tMonth",&tMonth);
  tree->Branch("tDate",&tDate);
  tree->Branch("tHour",&tHour);
  tree->Branch("tMinute",&tMinute);
  tree->Branch("tSecond",&tSecond);

  tree->Branch("nH",&nH);
  tree->Branch("nH0",&nH0);
  tree->Branch("nH1",&nH1);
  tree->Branch("nH2",&nH2);
  tree->Branch("nH3",&nH3);
  
  tree->Branch("nLayers",&nLayers);
  tree->Branch("channel",&channel);
  tree->Branch("board",&board);
  tree->Branch("pwidth",&pwidth);
  tree->Branch("pwidthScaleFactor",&pwidthScaleFactor);
  tree->Branch("tcnt",&tcnt);
  tree->Branch("pcnt",&pcnt);
  tree->Branch("dtcnt",&dtcnt);
  tree->Branch("dtime",&dtime);
  tree->Branch("dtimeInt",&dtimeInt);
  tree->Branch("dTimeOfEv",&dTimeOfEv);
  
  //Set the histrogram to calculate the bH, nH"z"
  TH1F *hboard       = new TH1F("hboard ","",5,1,6);
  TH1F *hBoard       = new TH1F("hBoard ","",5,1,6);
  TH1F *hChannel     = new TH1F("hChannel ","",16,0,16);
  
  //Set some temporary variable
  Int_t Index =0, nHit = 0, frame0 = 0;
  double dtime0 = 0;
  dTimeOfEv = 0;
  
  //Set the max index of the input file
  int evs = data.GetEntriesFast();
  
  //Read the input data and output them to be experiment data format
  for (Long64_t ev = 0; ev < evs ; ++ev) {//@@@
    if(ev%(int(evs/100.)) == 0){
      cout<<Form("\r%.5f%%",(ev*100.)/(1.*evs))<<flush; // Scan the progress
    }
    data.GetEntry(ev); // point to the event index "ev"
    
    //read the input hit information
    frame_   = data.GetInt("eID");
    channel_ = data.GetDouble("channel");
    Edep_  = data.GetDouble("Edep");  
    
    //In condition: Prepare to save the data
    if (nHit!=0&&frame0!=frame_){
      unixtime_ = TheFirstUT+pcnt[nHit-1];// create a virtual unixtime for data format.
      
      // get the time information, +8 time zone
      unixTimeToHumanReadable(unixtime_,
        tYear, tMonth, tDate, tHour, tMinute, tSecond,+8);
        
      nH      = nHit;//total number for hit in a frame/event
      //total number for hit at "z" layer
      nH0     = hBoard->GetBinContent(0);
      nH1     = hBoard->GetBinContent(1);
      nH2     = hBoard->GetBinContent(2);
      nH3     = hBoard->GetBinContent(3);
      
      if(nH!=nH0+nH1+nH2+nH3) cout<<EvIndex<<"  Meow"<<endl;//Debug Code
      nLayers=0 ;//reconfigure number of layer of hits
      for(int i1=0; i1<4;i1++){
        if (hBoard->GetBinContent(i1)!=0) nLayers++; //Calculate the nLayers
      }
      if (nHit > TriggerS&&nHit < TriggerL){ //Select condition for number of hit
        if(/*unixtime_>TheFirstUT-1*/true){ //Old way to debug and save event
          dTimeOfEv = abs(dtimeInt[nHit-1]-dtime0);
          tree->Fill();
          
          Index++;
          EvIndex = Index;
        }
      }
      //clear the data for new event to storge
      board.clear();
      channel.clear();
      pwidth.clear();
      pwidthScaleFactor.clear();
      tcnt.clear();
      pcnt.clear();
      dtime.clear();
      dtimeInt.clear();
      dtcnt.clear();
      nHit = 0;
      hBoard->Reset();
      hChannel->Reset();
      dtcnt.push_back(0);

    }

    //record the hit data in to vector
    board            .push_back(GetBIDbyG4GID(channel_));
    channel          .push_back(GetCIDbyG4GID(channel_));
    pwidth           .push_back(Edep_*EdeptoPWit);
      //suppose pwidth(100s) = EdeptoPWit*Edep(GeV)
    pwidthScaleFactor.push_back(1);//default is 1
    tcnt             .push_back(rand()%eventGapTcnt);
    pcnt             .push_back(frame_/FramIn1Sec);
    dtime            .push_back(pcnt[nHit-1]+tcnt[nHit-1]/tcntOneSec);
    dtimeInt         .push_back(pcnt[nHit-1]*tcntOneSec+tcnt[nHit-1]);
    hBoard    ->Fill(board[nHit-1]);
    hboard    ->Fill(board[nHit-1]);
    hChannel  ->Fill(channel[nHit-1]);
    frame0    = frame_;
    dtime0    = dtimeInt[nHit-1];
    
    //calculate the hits number
    nHit++;
  }
  // tree->Fill();
  rotfile->Write();
  rotfile->Close();
  cout<<Form("\rFinished root file save: %s",SaveName)<<endl;
  

}




