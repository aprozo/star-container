// C++ headers
#include <iostream>

//
// Forward declarations
//

class StMaker;
class StChain;
class StPicoDstMaker;

//_________________
void runPicoDstAnalysisMaker(const char* inFileName = "st_physics_20069002_raw_1500008.picoDst.root") {

  std::cout << "Lets run the StPicoDstAnalysisMaker, Master" << std::endl;
  // Load all the STAR libraries
  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
  loadSharedLibraries();

  // Load specific libraries
  gSystem->Load("StPicoEvent");
  gSystem->Load("StPicoDstMaker");
  gSystem->AddDynamicPath(gSystem->ExpandPathName("$PWD/.sl79_gcc485/LIB"));
  gSystem->Load("StPicoDstAnalysisMaker");

  // Create new chain
  StChain *chain = new StChain();

  std::cout << "Creating StPicoDstMaker to read and pass file list" << std::endl;
  // Read via StPicoDstMaker
  // I/O mode: write=1, read=2; input file (or list of files); name
  StPicoDstMaker* picoMaker = new StPicoDstMaker(2, inFileName, "picoDst");
  // Set specific branches ON/OFF
  picoMaker->SetStatus("*", 0);
  picoMaker->SetStatus("Event*", 1);
  picoMaker->SetStatus("Track*", 1);
  picoMaker->SetStatus("BTofPidTraits*", 1);
  picoMaker->SetStatus("BTowHit*", 1);
  std::cout << "... done" << std::endl;

  std::cout << "Constructing StPicoDstAnalysisMaker with StPicoDstMaker" << std::endl;
  // Example of how to create an instance of the StPicoDstAnalysisMaker and initialize
  // it with StPicoDstMaker
  StPicoDstAnalysisMaker *anaMaker1 = new StPicoDstAnalysisMaker(picoMaker, "oPicoAnaMaker_1.root");
  // Add vertex cut
  anaMaker1->setVtxZ(-40., 40.);
  std::cout << "... done" << std::endl;

  // std::cout << "Constructing StPicoDstAnalysisMaker with file list and will use StPicoDstReader";
  // Example of how to create an instance of the StPicoDstAnalysisMaker and initialize
  // it with StPicoDstRader
  // StPicoDstAnalysisMaker *anaMaker2 = new StPicoDstAnalysisMaker(inFileName, "oPicoAnaMaker_2.root");
  // // Add vertex cut
  // anaMaker2->setVtxZ(-50., 50.);
  // std::cout << "... done" << std::endl;

  std::cout << "Initializing chain" << std::endl;
  // Check that all maker has been successfully initialized
  if( chain->Init() == kStErr ){ 
    std::cout << "Error during the chain initializtion. Exit. " << std::endl;
    return;
  }
  std::cout << "... done" << std::endl;


  std::cout << "Lets process data, Master" << std::endl;
  // Retrieve number of events picoDst files
  int nEvents2Process = picoMaker->chain()->GetEntries();
  std::cout << " Number of events in files: " << nEvents2Process << std::endl;
  // Also one can set a very large number to process, whyle the special return
  // flag will send when there will be EndOfFile (EOF)

  // Processing events
  for (Int_t iEvent=0; iEvent<nEvents2Process; iEvent++) {
    
    if( iEvent % 1000 == 0 ) std::cout << "Macro: working on event: " << iEvent << std::endl;
    chain->Clear();

    // Check return code
    int iret = chain->Make();
    // Quit event processing if return code is not 0
    if (iret) { std::cout << "Bad return code!" << iret << endl; break; }
  } // for (Int_t iEvent=0; iEvent<nEvents2Process; iEvent++)
  std::cout << "Data have been processed, Master" << std::endl;

  std::cout << "Finalizing chain" << std::endl;
  // Finalize all makers in chain
  chain->Finish();
  std::cout << "... done" << std::endl;

  // Delete all pointers
  // delete anaMaker2;
  delete anaMaker1;
  delete picoMaker;
  delete chain;
  
  std::cout << "Analysis has been finished, Master" << std::endl;
}
