#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>

class MyTreeEvent;
class TrackCandidate;

#include "MyTreeEvent.h"
#include "TrackCandidate.h"

//==============================================================================
// Class implementations for ROOT dictionary
//==============================================================================
#ifdef __MAKECINT__
#pragma link C++ class TrackCandidate + ;
#pragma link C++ class MyTreeEvent + ;
#pragma link C++ class std::vector < TrackCandidate> + ;
#endif
// ClassImp implementations
ClassImp(TrackCandidate);
ClassImp(MyTreeEvent);

void readMyTreeEvent(
    TString filename = "/workspaces/star-container/oPicoAnaMaker_1.root") {

  TFile *file = TFile::Open(filename, "READ");
  TTree *tree = (TTree *)file->Get("events");

  MyTreeEvent *event = new MyTreeEvent();
  tree->SetBranchAddress("MyTreeEvent", &event);

  Long64_t nEntries = tree->GetEntries();
  for (Long64_t i = 0; i < nEntries; i++) {
    tree->GetEntry(i);

    // Now you can access all members of your event
    std::cout << "\n--- Entry " << i << " ---" << std::endl;
    std::cout << "Event ID: " << event->eventId << std::endl;
    std::cout << "Centrality: " << event->centrality << std::endl;

    // Access the track vectors
    std::cout << "Inclusive tracks: " << event->inclusiveTracks.size()
              << std::endl;

    // Break after first few events for demonstration
    if (i >= 4) {
      break;
    }
  }

  // Clean up
  delete event;
  file->Close();
}
