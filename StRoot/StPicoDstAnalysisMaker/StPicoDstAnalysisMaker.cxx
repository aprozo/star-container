// STAR headers
#include "St_base/StMessMgr.h"

// StPicoDstAnalysisMaker headers
#include "StPicoDstAnalysisMaker.h"

// StPicoDstMaker headers
#include "StPicoDstMaker/StPicoDstMaker.h"
// StPicoEvent headers
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoEvent/StPicoBTowHit.h"
#include "StPicoEvent/StPicoBTofPidTraits.h"

// ROOT headers
#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TTree.h"

#include "MyTrack.h"
#include "MyTreeEvent.h"

// C++ headers
#include <limits>

ClassImp(StPicoDstAnalysisMaker)

//________________
StPicoDstAnalysisMaker::StPicoDstAnalysisMaker(StPicoDstMaker *maker,
                                               TString oFileName) :
  StMaker(), mDebug(false), mOutFileName(oFileName), mOutFile(nullptr), //this is initiation list 
  mPicoDstMaker(maker), mPicoDst(nullptr){            //at the time of creation
 
  // Clean trigger ID collection
  if( !mTriggerId.empty() ) {
    mTriggerId.clear();
  }
  // Set default event cut values
  mVtxZ[0] = -70.; mVtxZ[1] = 70.;
  mVtxR[0] = 0.; mVtxR[1] = 2.;

  // Set default track cut values
  mNHits[0] = 15; mNHits[1] = 90;
  mPt[0] = 0.15; mPt[1] = 10.;
  mEta[0] = -1.; mEta[1] = 1.;
}

//________________
StPicoDstAnalysisMaker::~StPicoDstAnalysisMaker() {
}

//________________
Int_t StPicoDstAnalysisMaker::Init() {
  // Initialization of the Maker

  if (mDebug) {
    LOG_INFO << "Initializing StPicoDstAnalysisMaker..." << endm;
  }
  // Retrieve PicoDst
  if (mPicoDstMaker) {
    // Retrieve pointer to the StPicoDst structure
    mPicoDst = mPicoDstMaker->picoDst();
  } else {
    LOG_ERROR << "[ERROR] No StPicoDstMaker has been found. Terminating." << endm;
    return kStErr;
  }

  // Check that picoDst exists
  if (!mPicoDst) {
    LOG_ERROR << "[ERROR] No StPicoDst has been provided. Terminating." << endm;
    return kStErr;
  }
  // Create output file
  if (!mOutFile) {
    mOutFile = new TFile(mOutFileName, "recreate");
  }
  else {
    LOG_WARN << "\n [WARNING] Output file: " << mOutFileName
             << " already exist!" << endm;
  }

  //---------------------------------------------------------------------------
  // Create TTree for analysis results
  mTree = new TTree("events", "Analysis Results");
  myTreeEvent = new MyTreeEvent();
  mTree->Branch("MyTreeEvent", &myTreeEvent);
  //---------------------------------------------------------------------------
  // Create histograms
  CreateHistograms();

  if (mDebug) {
    LOG_INFO << "StPicoDstAnalysisMaker has been initialized\n" << endm;
  }

  return kStOk;
}

//________________
Int_t StPicoDstAnalysisMaker::Finish() {
  // Initialization of the Maker

  if (mDebug) {
    LOG_INFO << "Finishing StPicoDstAnalysisMaker..." << endm;
  }

  // Write histograms to the file and then close it
  if (mOutFile) {
    LOG_INFO << "Writing file: " << mOutFileName;
    mOutFile->Write();
    mOutFile->Close();
    LOG_INFO << "\t[DONE]" << endm;
  }
  else {
    LOG_WARN << "[WARNING] Output file does not exist. Nowhere to write!" << endm;
  }

  if (mDebug) {
    LOG_INFO << "StPicoDstAnalysisMaker has been initialized\n" << endm;
  }

  return kStOk;
}

//________________
void StPicoDstAnalysisMaker::CreateHistograms() {
  if (mDebug) {
    LOG_INFO << "Creating histograms..." << endm;
  }

  TDirectory *dir = gDirectory;
  dir->mkdir("qa_histograms")->cd();
   //--------------------------------------------------------------------------  
  hVtxXvsY = new TH2F("hVtxXvsY","Primary vertex y vs. x;x (cm);y (cm)",
                      200, -10., 10., 200, -10., 10.);
  hVtxZ = new TH1F("hVtxZ","Primary vertex z;z (cm); Entries",
                   240, -120., 120.);
  //--------------------------------------------------------------------------  
  hGlobalPt = new TH1D("hGlobalPt","Global track p_{T};p_{T} (GeV/c);Entries",
                      200, 0., 10.);
  hGlobalNHits = new TH1D("hGlobalNHits","Global track number of hits;nHits;Entries",
                          90, 0., 90.);
  hGlobalEta = new TH1D("hGlobalEta","Global track #eta;#eta;Entries",
                        300, -1.5, 1.5);
  hPrimaryPt = new TH1D("hPrimaryPt","Primary track p_{T};p_{T} (GeV/c);Entries",
                      200, 0., 10.);
  hPrimaryNHits = new TH1D("hPrimaryNHits","Primary track number of hits;nHits;Entries",
                          90, 0., 90.);
  hPrimaryEta = new TH1D("hPrimaryEta","Primary track #eta;#eta;Entries",
                        300, -1.5, 1.5);
  hPrimaryDedxVsPt = new TH2F("hPrimaryDedxVsPt","Primary track dE/dx vs. p_{T};charge #cross p_{T} (GeV/c);dE/dx (keV/cm)",
                              300, -1.5, 1.5, 120, 0., 12.);
  hPrimaryInvBetaVsP = new TH2F("hPrimaryInvBetaVsP","Primary track 1/#beta vs. p;charge #cross p (GeV/c);1/#beta",
                                420, -2.1, 2.1, 200, 0.8, 2.8);
  //--------------------------------------------------------------------------  
  hBemcTowerAdc = new TH1D("hBemcTowerAdc","BEMC tower ADC;ADC;Entries",
                           500, 0., 3000.);
  if (mDebug) {
    LOG_INFO << "Histograms have been created" << endm;
  }
}

//________________
Bool_t StPicoDstAnalysisMaker::IsGoodTrigger(StPicoEvent *event) {
  if (!mTriggerId.empty()) {
    for (unsigned int iIter=0; iIter<mTriggerId.size(); iIter++) {
      if ( event->isTrigger( mTriggerId.at(iIter) ) ) {
        return true;
      }
    } // for (unsigned int iIter=0; iIter<mTriggerId.size(); iIter++)
    return false;
  } // if ( !mTriggerId.empty())
  return true;
}

//________________
Bool_t StPicoDstAnalysisMaker::EventCut(StPicoEvent *event) {
  return ( event->primaryVertex().Z() >= mVtxZ[0] &&
           event->primaryVertex().Z() <=  mVtxZ[1] &&
           event->primaryVertex().Perp() >= mVtxR[0] &&
           event->primaryVertex().Perp() <= mVtxR[1] &&
           IsGoodTrigger( event ) );
}

//________________
Bool_t StPicoDstAnalysisMaker::TrackCut(StPicoTrack *track) {
  return ( track->nHits() >= mNHits[0] &&
           track->nHits() <= mNHits[1] &&
           track->gPt() >= mPt[0] &&
           track->gPt() <= mPt[1] &&
           track->gMom().Eta() >= mEta[0] &&
           track->gMom().Eta() <= mEta[1] );
}

//________________
Int_t StPicoDstAnalysisMaker::Make() {
  // Make is executed on every new event

  // The example that shows how to access event information
  //
  // Retrieve pico event
  StPicoEvent *theEvent = mPicoDst->event();

  // Check if event passes event cut (declared and defined in this
  // analysis maker)
  if ( !EventCut(theEvent) ) {
    return kStOk;
  }

  // Fill event histograms
  hVtxXvsY->Fill( theEvent->primaryVertex().X(),
                  theEvent->primaryVertex().Y() );
  hVtxZ->Fill( theEvent->primaryVertex().Z() );

  // Create references to event-level data for easier access
  vector<MyTrack> *inclusiveTracks = &myTreeEvent->inclusiveTracks;

  MyTrack inclusiveTrack;
  inclusiveTracks->clear();


  float &vertexX = myTreeEvent->vertexX;
  float &vertexY = myTreeEvent->vertexY;
  float &vertexZ = myTreeEvent->vertexZ;

  vertexX = theEvent->primaryVertex().X();
  vertexY = theEvent->primaryVertex().Y();
  vertexZ = theEvent->primaryVertex().Z();


  int &eventId = myTreeEvent->eventId;
  int &runId = myTreeEvent->runId;

  eventId = theEvent->eventId();
  runId = theEvent->runId();
  //
  // The example that shows how to access track information
  //

  // Get number of tracks in the event
  unsigned int nTracks = mPicoDst->numberOfTracks();

  // Track loop
  for (unsigned int iTrack=0; iTrack<nTracks; iTrack++) {

    // Retrieve i-th pico track
    StPicoTrack *track = (StPicoTrack*)mPicoDst->track(iTrack);
    // Track must exist
    if (!track) continue;

    // Check if track passes track cut
    if ( !TrackCut(track) ) continue;

    MyTrack myTrack; // create a temporary MyTrack object to store track information
    //=============================================================
    myTrack.id = track->id();
    myTrack.pt = track->gPt();
    myTrack.eta = track->gMom().Eta();
    myTrack.phi = track->gMom().Phi();
    myTrack.charge = track->charge();
    inclusiveTracks->push_back(myTrack); // push_back into current tree variable
    //=============================================================
    // Fill global track parameters
    hGlobalPt->Fill( track->gPt() );
    hGlobalNHits->Fill( track->nHits() );
    hGlobalEta->Fill( track->gMom().Eta() );

    // Primary track analysis
    if ( !track->isPrimary() ) continue;
    // Fill primary track histograms
    hPrimaryPt->Fill( track->pPt() );
    hPrimaryNHits->Fill( track->nHits() );
    hPrimaryEta->Fill( track->pMom().Eta() );
    hPrimaryDedxVsPt->Fill( track->charge() * track->pPt(),
                            track->dEdx() );
    // Accessing TOF PID traits information.
    // One has to remember that TOF information is valid for primary tracks ONLY.
    // For global tracks the path length and time-of-flight have to be
    // recalculated by hands.
    if ( track->isTofTrack() ) {
      StPicoBTofPidTraits *trait =
        (StPicoBTofPidTraits*)mPicoDst->btofPidTraits( track->bTofPidTraitsIndex() );
      if (!trait) continue;

      // Fill primary track TOF information
      hPrimaryInvBetaVsP->Fill( track->charge() * track->pPt(),
                                1./trait->btofBeta() );
    } // if ( track->isTofTrack() )
  } // track loop

  // The example that shows how to access hit information
  //
  // Note: If the number of BTOW hits is 0, it might be because the
  //       BTowHit branch is not enabled or there are genuinely no hits.
  for (UInt_t iHit=0; iHit<mPicoDst->numberOfBTowHits(); iHit++) {
    // Retrieve i-th BTOW hit
    StPicoBTowHit *btowHit = (StPicoBTowHit*)mPicoDst->btowHit(iHit);
    // Hit must exist
    if (!btowHit) continue;
    // Fill tower ADC
    hBemcTowerAdc->Fill( btowHit->adc() );
  } // hit loop
  //=============================================================
  mTree->Fill(); // Fill the TTree with the current myTreeEvent
 //=============================================================
  return kStOk;
}
