//include
#include "TrackAnaSub.C"

void TrackAnaV4(
  const int indexi=28, 
  const int indexf=29, 
  const double trigger = TriggerTrFitSigSizeX,
  const bool testMode=false 
) {
  defstyle();
  double eventGap[46]={};
  char version[8] = {};
  DetVerCheck(version,detVer);
  
  char cmdline[250], DirTfitG[200];
  
  sprintf(DirTfitG,"%sTrackingDetail/",DirRes_LFirTra);
  sprintf(cmdline,"mkdir -p %s",DirTfitG);
  if (system(cmdline)!=0) system(cmdline);
  //Printf("Files found: %i", (int) files.size());
  //ofstream out(Form("%sSEVertical4v3.dat",DirOperate));
  
  string EffFiles = string(Form("%sEDetCh%s_Till_Run*.root",DirIMFdat,version));
  vector<string> vStrRunTillData = find_files(EffFiles.data());
  const int totalRunNum = int(vStrRunTillData.size());
  
  map<string, GeometricAreaData> MSGAD;
  LoadPosCodeTable(MSGAD);
    
  ofstream ott("/home/yusiang/G4DataFindR2/lbhgrsabhvrs.txt", ofstream::out | ofstream::app );

  
  for(int i0=indexi;i0<indexf;i0++){
    int int_Os[6]={0};
    eventGap[i0] = (1.0*eventGapTcnt[i0])/2560000000.;
    char RFNStr[200];
    sprintf(RFNStr,"%d_%s_nHTH%dL%d",eventGapTcnt[i0],TimeStr,TriggerS,TriggerL);
    string InputGapTFile = Form("%sGapT%s.root",DirOperate,RFNStr);
    cout<<InputGapTFile<<" is opening"<<endl;
    TreeReader data(InputGapTFile.data());
    string OutputGapTFile = Form("%sETracksGT%s.root", DirOperate, RFNStr);
    string OutputGapTFiles = Form("%stmp_ETracksGT%s*.root", DirOperate, RFNStr);
    // TFile *rotfile = new TFile(OutputGapTFile.data(),"RECREATE");
    cout<<Form("%sETracksGT%s", DirOperate, RFNStr)<<" is opened"<<endl;

    //Data Variable
    //new Tree unit and tree unit
    
    Int_t      TrackIndex=0;
    Long64_t   unixtime_;
    
    //take time and set anlyze Constant for boundry condition
    Long64_t evsTr = data.GetEntriesFast(), lastev = 0;
    cout <<"total event:\t"<<evsTr<<endl;
    if(testMode||evsTr>3000) evsTr = evsTr*0.01;
    //Fill the data
    
    vector<int> vEffir;
    vector<Long64_t> vRunStartev, vRunFinalev; 

    for(int ir = 0; ir < totalRunNum ; ir ++){
      if(RunID[ir] == -1) continue;
      vEffir.push_back(ir);
      RunData      runDatatmp;
      map<int, scintillator> MScints = LoadEDectTable(vStrRunTillData[ir].data());
      TFile *ScinEfile = TFile::Open(vStrRunTillData[ir].data());
      TTree *tERPos = (TTree*) ScinEfile ->Get("tERPos");
      tERPos->SetBranchAddress("StartUT"     ,&runDatatmp.StartUT );
      tERPos->SetBranchAddress("EndUT"       ,&runDatatmp.EndUT   );
      tERPos->GetEntry(0);
      cout<<"StartUT : EndUT  "<<runDatatmp.StartUT<<"  "<<runDatatmp.EndUT<<endl;
      int rnum = 0;
      for (Long64_t ev = lastev; ev <evsTr; ++ev) {
        data.GetEntry(ev);
        unixtime_ = data.GetLong64("unixtime");
        if(ev%1000 == 0) 
          cout<<Form("\r%.5f%%",(ev*100.)/(1.*evsTr))
              <<Form("   %d",unixtime_)
              <<Form("   %d",rnum)<<flush;
        
        if(!MCMode){
          if(unixtime_<runDatatmp.StartUT){
            if(vRunStartev.size()==vEffir.size()) 
              vRunStartev[vEffir.size()-1] = ev+1;
            else vRunStartev.push_back(ev+1);//@@
            continue;
          } 
          if(unixtime_>runDatatmp.EndUT){
            lastev = ev;
            vRunFinalev.push_back(lastev);//@@
            break;
          } 
        }else{
          vRunStartev.push_back(0);
          vRunFinalev.push_back(evsTr);
          break;
        }
      }
    }
    vector<string> vOsfiles, vTmp_ETrFile;
    cout<<"int(vEffir.size())"<<int(vEffir.size())<<endl;
    string RfileName = Form("%stmp_mutiBash.sh", DirOperate,RfileName.data());
    ofstream MutiCoreBash(RfileName.data());
    cout<<"Create muti process bash file: "<<RfileName.data()<<endl;
    for(int ir = 0; ir < int(vEffir.size()) ; ir ++){
      
      int Num_RunTotalev = vRunFinalev[ir] - vRunStartev[ir]+1;
      cout<< "Num_RunTotalev"<<Num_RunTotalev <<endl;
      int batchSize = Num_RunTotalev/cpuCores;
      for(int ibatch = 0; ibatch<cpuCores; ibatch++){
        int evFirFin[2] = {
          vRunStartev[ir]+ibatch*batchSize,
          vRunStartev[ir]+(ibatch+1)*batchSize
        };
        if(ibatch==cpuCores-1) evFirFin[1] = vRunFinalev[ir];
        string evhash = Form("%d%d", evFirFin[0], evFirFin[1]);
        string Tmp_ETrFile = Form("%stmp_ETracksGT%s%s.root", DirOperate, RFNStr, evhash.data());
        string OsName = Form("%sOs%s.txt", DirOperate, evhash.data());

        int timeDelay = ibatch==0?ibatch*1:ibatch*1+7;
        MutiCoreBash<<"sleep "<<timeDelay<<" &&root -l -b "
          <<DirMacros<<"TrackAnaSub.C+"
            "\\\("
              <<"\\\""<<InputGapTFile<<"\\\""<<","
              <<"\\\""<<Tmp_ETrFile<<"\\\""<<","
              <<"\\\""<<evhash<<"\\\""<<","
              <<"\\\""<<OsName<<"\\\""<<","
              <<ir<<","
              <<evFirFin[0]<<","
              <<evFirFin[1]<<","
              <<trigger
            <<"\\"<<") &\n";
        cout<<"sleep "<<timeDelay<<" &&root -l -b "
          <<DirMacros<<"TrackAnaSub.C+"
            "\\\("
              <<"\\\""<<InputGapTFile<<"\\\""<<","
              <<"\\\""<<Tmp_ETrFile<<"\\\""<<","
              <<"\\\""<<evhash<<"\\\""<<","
              <<"\\\""<<OsName<<"\\\""<<","
              <<ir<<","
              <<evFirFin[0]<<","
              <<evFirFin[1]<<","
              <<trigger
            <<"\\"<<")"<<" &\n";
        vTmp_ETrFile.push_back(Tmp_ETrFile);
        // TrackAnaSub(InputGapTFile, Tmp_ETrFile, evhash, OsName, evFirFin, trigger);
        vOsfiles.push_back(OsName);
      }
      MutiCoreBash<<"wait\n";
    
      
    }
    MutiCoreBash<<endl<<"echo Finish all tracking by muti process."<<endl;
    
    MutiCoreBash<<"echo merge the files to be: "<<OutputGapTFile<<endl;
    // MutiCoreBash<<"hadd -f "<<OutputGapTFile<<" "<<OutputGapTFiles<<endl;
    
    // MutiCoreBash<<"echo remove unmergefiles"<<OutputGapTFiles<<endl;

    MutiCoreBash.close();
    system(Form("sh %s",RfileName.data()));
    // system(Form("rm %s",RfileName.data()));
    cout<<"\nFinish all process & rm: "<<RfileName.data()<<endl;
    
    int Osss [6] = {0,0,0,0,0,0}, StaTrackIndex = 0;
    vector<int> vStaTrackIndex;
    for(int ifOs; ifOs<int(vOsfiles.size()); ifOs++){
      ifstream in((vOsfiles[ifOs]).data());
      // cout<< vOsfiles[ifOs].data() <<endl;
      int tmp_OsData[6] = {0};

      for(int iOs=0; iOs<6; iOs++){
        in>>tmp_OsData[iOs];
        Osss[iOs] += tmp_OsData[iOs];
      } 
      in>>StaTrackIndex;
      vStaTrackIndex.push_back(StaTrackIndex);
      in.close();
    }
    ott<<DirOperate<<"\t"<<trigger;
    cout<<DirOperate<<":\n\t"<<trigger;
    for(int iOs = 0; iOs<6; iOs++){ 
      cout<<"\t"<<Osss[iOs];
      ott<<"\t"<<Osss[iOs];
    }
    cout<<endl;
    ott<<endl;
    
    
    TrackAnaSubHAdd(OutputGapTFile,vTmp_ETrFile,vStaTrackIndex);
    cout<<"\nFinished HAdd ETrack files, and sort leaf TrackIndex"<<endl;
    
    for(int i=0; i<int(vTmp_ETrFile.size());i++)
      system(Form("rm %s",vTmp_ETrFile[i].data()));
    cout<<"rm all Tmp_ETrack* files."<<endl;
    
    for(int i=0; i<int(vOsfiles.size());i++)
      system(Form("rm %s",vOsfiles[i].data()));
    cout<<"rm all Os files."<<endl;
  }
  ott.close();
  // throw;
}



void  FitTrackInfoAna(const int indexGap=28){
  defstyle();
  char DirRoot[1][100] = {
    ""
  };
  sprintf(DirRoot[0],"%s",DirOperate);

  char DirSave[100] ;
  sprintf(DirSave,"%sFitTrackInfo/",DirRes_LFirTra);
  char cmdline[150];
  sprintf(cmdline,"mkdir -p %s",DirSave);
  if (system(cmdline)!=0) system(cmdline);
  
  char FileSave[200];
  sprintf(FileSave,"%sETracksGT%d_%s_nHTH%dL%d.root",DirOperate,eventGapTcnt[indexGap],TimeStr,TriggerS,TriggerL);

  
  TFile *_file0 = TFile::Open(FileSave);
  cout<<"Tracking result File: "<<FileSave<<" is Opening:  "<<endl;
  TTree *t  = (TTree*) _file0 ->Get("t");
  
  TCanvas *C = new TCanvas("Canvas","",1920,1080);
  C->cd();
  C->SetLogy(1);
  TH1F *hTrErrXZ = new TH1F("hTrErrXZ","Fitting Err(1-#sigma) on X-Z, Y-Z planes",500,0,100);
  TH1F *hTrErrYZ = new TH1F("hTrErrYZ","Fitting Err(1-#sigma) on X-Z, Y-Z planes",500,0,100);
  hTrErrXZ->SetLineColor(1);
  hTrErrYZ->SetLineColor(2);
  hTrErrXZ->SetStats(0);
  hTrErrYZ->SetStats(0);
  TLegend *Lerrzxy = new TLegend(0.6,0.65,.9,.9);
  Lerrzxy->SetFillColor(0);
  Lerrzxy->SetLineColor(1);
  t->Draw("Tr_ZX_Err>>hTrErrXZ","Tr_ZX_Err<100","box");
  t->Draw("Tr_ZY_Err>>hTrErrYZ","Tr_ZY_Err<100","boxsame");
  
  Lerrzxy->AddEntry(hTrErrXZ,Form("Track Err on Z-X plane"),"l");
  Lerrzxy->AddEntry(hTrErrYZ,Form("Track Err on Z-Y plane"),"l");
  Lerrzxy->AddEntry(hTrErrXZ,Form("Entries:%10.0f",hTrErrXZ->GetEntries()),"h");
  Lerrzxy->Draw("same");
  
  DrawPdfPng(C,Form("%sFittingErrZXZY",DirSave));
  
  C->cd();
  C->SetLogy(0);
  TH2F *hTrDXYZ = new TH2F("hTrDXYZ","Angular distribution",201,-1.400,1.400,201,-1.400,1.400);
  hTrDXYZ->GetXaxis()->SetRangeUser(-0.75,0.75);
  hTrDXYZ->GetYaxis()->SetRangeUser(-0.75,0.75);
  t->Draw("abs(DyDz):abs(DxDz)>>hTrDXYZ","nLayers==2&&nH==3","colz");
  DrawPdfPng(C,Form("%sFittingNL2NH3",DirSave));
  
  

}




