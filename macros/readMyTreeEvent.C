#include <TBranch.h>
#include <TFile.h>
#include <TTree.h>
#include <iostream>

class MyTreeEvent;
class MyTrack;

#include "../StRoot/StPicoDstAnalysisMaker/MyTreeEvent.h"
#include "../StRoot/StPicoDstAnalysisMaker/MyTrack.h"

//==============================================================================
// Class implementations for ROOT dictionary
//==============================================================================
#ifdef __MAKECINT__
#pragma link C++ class MyTrack + ;
#pragma link C++ class MyTreeEvent + ;
#pragma link C++ class std::vector < MyTrack> + ;
#endif
// ClassImp implementations
ClassImp(MyTrack);
ClassImp(MyTreeEvent);

void readMyTreeEvent(TString filename = "/workspaces/star-tutorial/outputPicoAnaMaker.root")
{

  TFile *file = TFile::Open(filename, "READ");
  TTree *tree = (TTree *)file->Get("events");

  MyTreeEvent *event = new MyTreeEvent();
  tree->SetBranchAddress("MyTreeEvent", &event);

  Long64_t nEntries = tree->GetEntries();
  for (Long64_t i = 0; i < nEntries; i++)
  {
    tree->GetEntry(i);

    // Now you can access all members of your event
    std::cout << "\n--- Entry " << i << " ---" << std::endl;
    std::cout << "Run ID: " << event->runId << std::endl;
    std::cout << "Event ID: " << event->eventId << std::endl;
    std::cout << "Vertex Z: " << event->vertexZ << " cm " <<std::endl;

    // Access the track vectors
    std::cout << "Inclusive tracks: " << event->inclusiveTracks.size()
              << std::endl;
    if (event->inclusiveTracks.size()<5) continue;

    for (size_t i = 0; i < 5; i++) //print first tracks
    {
      std::cout << "Track ID: " << event->inclusiveTracks[i].id
                << ", pt: " << event->inclusiveTracks[i].pt << " GeV/c"
                << ", eta: " << event->inclusiveTracks[i].eta
                << ", phi: " << event->inclusiveTracks[i].phi
                << ", charge: " << event->inclusiveTracks[i].charge
                << std::endl;
    }

    // Break after first few events for demonstration
    if (i >= 4)
    {
      break;
    }
  }

  // Clean up
  delete event;
  file->Close();
}
