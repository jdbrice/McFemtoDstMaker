#include "TH1.h"
#include "TH2.h"
#include "TChain.h"
#include "TSystem.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class StAssociationMaker;
class StChain;

int doEvent(   Int_t nEvents=100,
        const Char_t *FileList="test.list",
        const Char_t *OutRoot="test.root")
{
    cout << "doEvent(" << nEvents << ", " << FileList << ", " << OutRoot << " )" << endl;
    load();
    StChain *chain = new StChain;

    ifstream infile;
    infile.open(FileList);
    string line;
    getline(infile,line);
    infile.close();

    StIOMaker *IOMk = new StIOMaker("IO","r",Form("@%s",FileList),"bfcTree");
    IOMk->SetDebug(0);
    IOMk->SetIOMode("r");
    IOMk->SetBranch("*",0,"0");           //deactivate all branches
    IOMk->SetBranch("eventBranch",0,"r"); //activate event Branch
    IOMk->SetBranch("geantBranch",0,"r"); //activate geant Branch
    IOMk->SetBranch("McEventBranch",0,"r"); //activate McEvent Branch
    IOMk->SetBranch("*",0,"r"); //activate McEvent Branch

    //St_db_Maker *dbMk = new St_db_Maker("StarDb", "MySQL:StarDb", "$STAR/StarDb","StarDb");
    //StMagFMaker *magfMk = new StMagFMaker;
    // St_db_Maker *dbMk = new St_db_Maker("db","MySQL:StarDb","$STAR/StarDb","StarDb");
    //StMagFMaker *magfMk = new StMagFMaker; 
    //StMtdMatchMaker *mtdMatchMaker = new StMtdMatchMaker();
    //mtdMatchMaker->setGeomTag("y2015b");
    //StMtdCalibMaker *mtdCalibMaker = new StMtdCalibMaker("mtdcalib");

    StMcEventMaker *mcEvent = new StMcEventMaker();
    StAssociationMaker *association = new StAssociationMaker( );       // TPC association maker
    association->SetDebug(0);

    McFemtoDstWriter *fmtWriter = new McFemtoDstWriter( "MtdMc");
    fmtWriter->SetDebug(0);
    fmtWriter->setOutputFileName( OutRoot );
    
    
    

    chain->Init();
    if(nEvents<0)nEvents = 1e6;
    cout<<"total entries: "<<nEvents<<endm<<endl;

    int istat=0,iev=1;
// EventLoop: if (iev<=nEvents && istat!=2) {
//                if(iev%1000==0) cout << "Working on eventNumber " << iev << endm<<endl;
//                chain->Clear();
//                istat = chain->Make(iev); // This should call the Make() method in ALL makers
//                if (istat == 2) { cout << "Last  Event Processed. Status = " << istat << endm<<endl; }
//                if (istat == 3) { cout << "Error Event Processed. Status = " << istat << endm<<endl; }
//                iev++; goto EventLoop;
//            } // Event Loop


            bool good = true;
            int iEvent = 0;
            while ( good ){

                if ( iEvent >= nEvents )
                    break;

                chain->Clear();
                int istat = 0;
                istat = chain->Make(iEvent);

                // cout << "Checking StAssociationMaker = " << association->mcTrackMap()->size() << endl;

                if (istat == 2) {
                    cout << "Last  event processed. Status = " << istat << endl;
                    break;
                }
                if (istat == 3) {
                    cout << "Error event processed. Status = " << istat << endl;
                }

                if ( iEvent % 1000 == 0 ) {
                    cout << " Event " << iEvent << " Complete" << endl;
                }

                iEvent++;
            }

           chain->Finish();

           //TFile *fout = new TFile(Form("jpsi.embed.%s",OutRoot),"RECREATE");
           //embed->GetHistList()->Write();
           //fout->Close();
           //delete fout;

           printf("========================================\n");
           printf("Input file list: %s\n",FileList);
           printf("Process %d events\n",iEvent-1);
           printf("Output histo file: %s\n",OutRoot);
           printf("========================================\n");

           delete chain;

           return 0;

}

//-----//
void load() {

    gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
    //gROOT->LoadMacro("/star/u/marr/mtd/analysis/StarVMC/Geometry/macros/loadStarGeometry.C");
    loadSharedLibraries();

    gSystem->Load("StDetectorDbMaker.so");
    gSystem->Load("StTpcDb");
    gSystem->Load("StEvent");
    gSystem->Load("StMcEvent");
    gSystem->Load("StMcEventMaker");
    gSystem->Load("StDaqLib");
    gSystem->Load("libgen_Tables");
    gSystem->Load("libsim_Tables");
    gSystem->Load("libglobal_Tables");
    gSystem->Load("StMagF");

    gSystem->Load("StDbUtilities");
    gSystem->Load("StEEmcUtil");
    gSystem->Load("StEEmcDbMaker");
    gSystem->Load("St_g2t.so");
    gSystem->Load("St_geant_Maker.so");
    gSystem->Load("StAssociationMaker");
    gSystem->Load("StMcAnalysisMaker");
    gSystem->Load("libgeometry_Tables");   
    gSystem->Load("StTriggerUtilities");


    gSystem->Load("StEmcUtil");
    gSystem->Load("StEmcRawMaker");
    gSystem->Load("StEmcADCtoEMaker");
    gSystem->Load("StPreEclMaker");
    gSystem->Load("StEpcMaker");
    gSystem->Load("StEmcSimulatorMaker");


    gSystem->Load("StDbLib");
    gSystem->Load("StDbBroker");
    gSystem->Load("StDetectorDbMaker");
    gSystem->Load("St_db_Maker");

    //gSystem->Load("StEPCalibMaker");
    gSystem->Load("StBTofUtil");
    gSystem->Load("StMtdHitMaker");
    gSystem->Load("StMtdUtil");
    gSystem->Load("StMtdMatchMaker");
    gSystem->Load("StVpdCalibMaker");
    gSystem->Load("StBTofCalibMaker");
    gSystem->Load("StMtdCalibMaker");

    gSystem->Load("StRefMultCorrVPDMBZDCNoVtx");
    gSystem->Load("StAssociationMaker");
    gSystem->Load("FemtoDstFormat");
    gSystem->Load("McFemtoDstWriter");
    gSystem->Load("StMtdEmbedding");
    // gSystem->Load("StMtdEmbeddingTreeMaker");
}

//-----//
