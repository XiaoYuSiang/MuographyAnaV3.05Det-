using namespace std;
#include "/data4/YuSiang/personalLib/Style/tdrstyle.C"


void AllEffAna(){
  char Path_OpRes[150];
  char Name_EfficienceExperimentDat1[150];
  char Name_EfficienceExperimentDat2[150];
  sprintf(Path_OpRes,"/home/yusiang/MuographyAna/PSAna/OperateResule/");
  sprintf(Name_EfficienceExperimentDat1,"st2022-02-23ed2022-03-09ExpS4_11F_4lay_5deg");
  sprintf(Name_EfficienceExperimentDat2,"st2021-12-23ed2022-01-18ExpS4_11F_4lay");
  int invBD[6]={-1,0,-1,1,3,2};
  int BD[6]={1,3,5,4};
  setTDRStyle();
  int dataTGap[2]={140,1000};

  for(int i0=0;i0<2;i0++){
    ifstream innilo(Form("%sGT%ddata_s4_4l_tf_nilo_0.csv",Path_OpRes,dataTGap[i0]));
    ifstream inilo(Form("%sGT%ddata_s4_4l_tf_ilo_0.csv",Path_OpRes,dataTGap[i0]));
    ofstream outEff(Form("%sEffOfV2_0AtS4_11FExpGT%d.txt",Path_OpRes,dataTGap[i0]));
    double Eff[4][16] = {};
    int bd=0;
    int trash=0;
    double xt,yt,eylt,eyht;
    double x[4][16]={},y[4][16]={},ex[16]={},eyl[4][16]={},eyh[4][16]={};
    int ibd[4]={};
    for(int i=0;innilo>>bd>>xt>>yt>>eylt>>eyht;i++){
      if(bd==3||bd==5){
          x[invBD[bd]][ibd[invBD[bd]]]=xt;
          y[invBD[bd]][ibd[invBD[bd]]]=yt;
        eyl[invBD[bd]][ibd[invBD[bd]]]=eylt;
        eyh[invBD[bd]][ibd[invBD[bd]]]=eyht;
        ibd[invBD[bd]]++;
      }
    }
    for(int i=0;inilo>>bd>>xt>>yt>>eylt>>eyht;i++){
      if(bd==1||bd==4){
          x[invBD[bd]][ibd[invBD[bd]]]=xt;
          y[invBD[bd]][ibd[invBD[bd]]]=yt;
        eyl[invBD[bd]][ibd[invBD[bd]]]=eylt;
        eyh[invBD[bd]][ibd[invBD[bd]]]=eyht;
        ibd[invBD[bd]]++;
      }
    }
    cout<<ibd[0]<<ibd[1]<<ibd[2]<<ibd[3]<<endl;
    int SetColorArr[9]={
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
    
    TH1F  *hMod          = new TH1F("hMod"   ,"",16,0,16);
    TH1F  *H3_X354       = new TH1F("H3_X354","",16,0,16);
    TH1F  *H3_1X54       = new TH1F("H3_1X54","",16,0,16);
    TH1F  *H3_13X4       = new TH1F("H3_13X4","",16,0,16);
    TH1F  *H3_135X       = new TH1F("H3_135X","",16,0,16);
    TH1F  *H4_1354       = new TH1F("H4_1354","",16,0,16);
    TH1F  *H3_1354[4]={H3_X354,H3_1X54,H3_13X4,H3_135X};
    TLegend *pwdVSddt1 = new TLegend(0.23,.76,.93,0.89);
    pwdVSddt1->SetFillColor(0);
    pwdVSddt1->SetLineColor(0);
    pwdVSddt1->SetNColumns(4);
    
    TCanvas *cevent = new TCanvas("cevent","",960,540);
    hMod->SetStats(0);


      TH1F *hMod2 = (TH1F*) hMod->Clone();



    for(int i=0;i<16;i++){
      hMod2->Fill(i);
      hMod->GetXaxis()->SetBinLabel(i+1,Form("ch.%02.f",i*1.));
      hMod2->GetXaxis()->SetBinLabel(i+1,Form("ch.%02.f",i*1.));
    }
    cevent->cd();
    hMod2->GetYaxis()->SetRangeUser(0.1,1.3);
    hMod2->Draw("l");
    hMod2->GetYaxis()->SetTitle("Channel Effciency");
    hMod2->GetXaxis()->SetTitle("Channel ID");
    hMod2->SetLineStyle(2);
    pwdVSddt1->AddEntry(H3_1354[0],Form("chnnal on MTB-1"),"FEl");
    pwdVSddt1->AddEntry(H3_1354[1],Form("chnnal on MTB-3"),"FEl");
    pwdVSddt1->AddEntry(H3_1354[2],Form("chnnal on MTB-5"),"FEl");
    pwdVSddt1->AddEntry(H3_1354[3],Form("chnnal on MTB-4"),"FEl");
    pwdVSddt1->Draw();
    for(int bd=0;bd<4;bd++){

      TGraphAsymmErrors *EFFErr = new TGraphAsymmErrors(16,x[bd],y[bd],ex,ex,eyl[bd],eyh[bd]);
      for(int ch=0;ch<16;ch++){
        cout<<BD[bd]<<"\t"<<ch<<"\t"<<x[bd][ch]<<"\t"<<y[bd][ch]<<"\t"<<eyl[bd][ch]<<"\t"<<eyh[bd][ch]<<"\n";
        outEff<<BD[bd]<<"\t"<<ch<<"\t"<<x[bd][ch]<<"\t"<<y[bd][ch]<<"\t"<<eyl[bd][ch]<<"\t"<<eyh[bd][ch]<<"\n";
        H3_1354[bd]->Fill(x[bd][ch],y[bd][ch]);
      }
      H3_1354[bd]->Draw("histbarSame");
      H3_1354[bd]->SetFillColor(SetColorArr[bd]);
      H3_1354[bd]->SetBarWidth(0.18);
      H3_1354[bd]->SetBarOffset(0.11+0.2*bd);
      H3_1354[bd]->SetLineColor(SetColorArr[bd+4]);
      H3_1354[bd]->GetYaxis()->SetRangeUser(0,1);
      EFFErr->GetYaxis()->SetRangeUser(0,1);
      EFFErr->Draw("esame");
      EFFErr->SetLineColor(SetColorArr[bd+4]);
      EFFErr->SetLineStyle(1);
      EFFErr->SetMarkerStyle(104);
      EFFErr->SetMarkerColor(SetColorArr[bd+4]);
    }
    hMod2->Draw("lsame");

    
    cevent->Print(Form("/home/yusiang/MuographyAna/PSAna/Result/EffAllNoGT%dBothNoise.pdf",dataTGap[i0]));
    
    char Path_rotfile[100];
    sprintf(Path_rotfile,"%sEffFactorGT%d.root",Path_OpRes,dataTGap[i0]);
    TFile *rotfile = new TFile(Path_rotfile,"RECREATE");
    cout<<Form("%s is RECREATE.",Path_rotfile)<<endl;
    char      EffTestNames[40];
    Int_t     boardID; //MTB ID
    Int_t     channelID; //ID of channel in MTB
    Int_t     GeneralID; //General ID of channel
    Float_t   cheff;
    Float_t   cheffEMin;
    Float_t   cheffEMax;
    
    TTree *tEff = new TTree("tEff","Efficiency factor from Eff. test");
    tEff->Branch("EffTestNames"  , EffTestNames   , "EffTestNames/C");
    tEff->Branch("boardID"       ,&boardID        , "boardID/I"     );
    tEff->Branch("channelID"     ,&channelID      , "channelID/I"   );
    tEff->Branch("GeneralID"     ,&GeneralID      , "GeneralID/I"   );
    tEff->Branch("cheff"         ,&cheff          , "cheff/F"       );
    tEff->Branch("cheffEMin"     ,&cheffEMin      , "cheffEMin/F"   );
    tEff->Branch("cheffEMax"     ,&cheffEMax      , "cheffEMax/F"   );
    
    
    for(int bd=0;bd<4;bd++){
      for(int ch=0;ch<16;ch++){
        if(bd==0||bd==3){
          sprintf(EffTestNames,Name_EfficienceExperimentDat1);
        }else if(bd==1||bd==2){
          sprintf(EffTestNames,Name_EfficienceExperimentDat2);
        }
        
        boardID   = BD[bd];
        channelID = ch;
        GeneralID = ch+BD[bd]*16;
        cheff     = y[bd][ch];
        cheffEMin = eyl[bd][ch];
        cheffEMax = eyh[bd][ch];
        
        tEff->Fill();
      }
    }
    rotfile->Write();
    rotfile->Close();
      
      
  }
}