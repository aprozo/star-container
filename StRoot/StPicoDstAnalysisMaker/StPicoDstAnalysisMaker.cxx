// STAR headers
#include "St_base/StMessMgr.h"

// StPicoDstAnalysisMaker headers
#include "StPicoDstAnalysisMaker.h"

// StPicoDstMaker headers
#include "StPicoDstMaker/StPicoDstMaker.h"
// StPicoEvent headers
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoDstReader.h"
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
                                               const char* oFileName) :
  StMaker(), mDebug(false), mOutFileName(nullptr), mOutFile(nullptr),
  mPicoDstMaker(maker), mPicoDst(nullptr), hVtxXvsY(nullptr),
  hVtxZ(nullptr), hPrimaryPt(nullptr), hGlobalNHits(nullptr),
  hPrimaryNHits(nullptr), hGlobalEta(nullptr), hPrimaryEta(nullptr),
  hPrimaryDedxVsPt(nullptr), hPrimaryInvBetaVsP(nullptr),
  hBemcTowerAdc(nullptr), mEventCounter(0), mIsFromMaker(true) {
  // Constructor

  // Set output file name
  mOutFileName = oFileName;

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
  // Destructor
  /* empty */
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

  // Create histograms
  CreateHistograms();


  //=============================================================
  // Create TTree for analysis results
  mTree = new TTree("events", "Analysis Results");
  myTreeEvent = new MyTreeEvent();
  mTree->Branch("MyTreeEvent", &myTreeEvent);
  //=============================================================



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

  CreateEventHistograms();
  CreateTrackHistograms();
  CreateBemcHitHistograms();

  if (mDebug) {
    LOG_INFO << "Histograms have been created" << endm;
  }
}

//________________
void StPicoDstAnalysisMaker::CreateEventHistograms() {

  if (mDebug) {
    LOG_INFO << "Creating event histograms...";
  }

  hVtxXvsY = new TH2F("hVtxXvsY","Primary vertex y vs. x;x (cm);y (cm)",
                      200, -10., 10., 200, -10., 10.);
  hVtxZ = new TH1F("hVtxZ","Primary vertex z;z (cm); Entries",
                   240, -120., 120.);

  if (mDebug) {
    LOG_INFO << "\t[DONE]" << endm;
  }
}

//________________
void StPicoDstAnalysisMaker::CreateTrackHistograms() {

  if (mDebug) {
    LOG_INFO << "Creating track histograms...";
  }

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

  if (mDebug) {
    LOG_INFO << "\t[DONE]" << endm;
  }
}

//________________
void StPicoDstAnalysisMaker::CreateBemcHitHistograms() {

  if (mDebug) {
    LOG_INFO << "Creating BEMC hit histograms...";
  }

  hBemcTowerAdc = new TH1D("hBemcTowerAdc","BEMC tower ADC;ADC;Entries",
                           500, 0., 3000.);

  if (mDebug) {
    LOG_INFO << "\t[DONE]" << endm;
  }
}

//________________
Bool_t StPicoDstAnalysisMaker::IsGoodTrigger(StPicoEvent *event) {

  Bool_t isGood = false;
  if ( !mTriggerId.empty()) {
    for (unsigned int iIter=0; iIter<mTriggerId.size(); iIter++) {
      if ( event->isTrigger( mTriggerId.at(iIter) ) ) {
        isGood = true;
        break;
      } // if ( event->isTrigger( mTriggerId.at(iIter) ) )
    } // for (unsigned int iIter=0; iIter<mTriggerId.size(); iIter++)
  } // if ( !mTriggerId.empty())
  else {
    isGood = true;
  } // else

  return isGood;
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

  // Increment event counter
  mEventCounter++;

  // Print event counter
  if ( (mEventCounter % 1000) == 0) {
    LOG_INFO << "Working on event: " << mEventCounter << "/"
             << mPicoDstMaker->chain()->GetEntries() << endl;
  }

  // Check that PicoDst exists
  if ( !mPicoDst ) {
    LOG_ERROR << "[ERROR] No PicoDst has been found. Terminating" << endm;
    return kStErr;
  }

  //
  // The example that shows how to access event information
  //

  // Retrieve pico event
  StPicoEvent *theEvent = mPicoDst->event();
  if ( !theEvent ) {
    LOG_ERROR << "[ERROR] PicoDst does not contain event information. Terminating" << endm;
    return kStErr;
  }

  // Check if event passes event cut (declared and defined in this
  // analysis maker)
  if ( !EventCut(theEvent) ) {
    return kStOk;
  }


  // Fill event histograms
  hVtxXvsY->Fill( theEvent->primaryVertex().X(),
                  theEvent->primaryVertex().Y() );
  hVtxZ->Fill( theEvent->primaryVertex().Z() );


  //=============================================================

  vector<MyTrack> *inclusiveTracks = &myTreeEvent->inclusiveTracks;

  MyTrack inclusiveTrack;
  inclusiveTracks->clear();


  Float_t &vertexX = myTreeEvent->vertexX;
  Float_t &vertexY = myTreeEvent->vertexY;
  Float_t &vertexZ = myTreeEvent->vertexZ;

  vertexX = theEvent->primaryVertex().X();
  vertexY = theEvent->primaryVertex().Y();
  vertexZ = theEvent->primaryVertex().Z();

  Int_t &centrality = myTreeEvent->centrality;
  Int_t &eventId = myTreeEvent->eventId;
  // centrality = 

  // //=============================================================

  //
  // The example that shows how to access track information
  //

  // Retrieve number of tracks in the event. Make sure that
  // SetStatus("Track*",1) is set to 1. In case of 0 the number
  // of stored tracks will be 0, even if those exist
  unsigned int nTracks = mPicoDst->numberOfTracks();


  // Track loop
  for (unsigned int iTrk=0; iTrk<nTracks; iTrk++) {

    // Retrieve i-th pico track
    StPicoTrack *theTrack = (StPicoTrack*)mPicoDst->track(iTrk);
    // Track must exist
    if (!theTrack) continue;

    // Check if track passes track cut
    if ( !TrackCut(theTrack) ) continue;


    //=============================================================
    inclusiveTrack.id = theTrack->id();
    inclusiveTrack.pt = theTrack->gPt();
    inclusiveTrack.eta = theTrack->gMom().Eta();
    inclusiveTrack.phi = theTrack->gMom().Phi();
    inclusiveTrack.charge = theTrack->charge();
    inclusiveTracks->push_back(inclusiveTrack);
 //=============================================================



    // Fill global track parameters
    hGlobalPt->Fill( theTrack->gPt() );
    hGlobalNHits->Fill( theTrack->nHits() );
    hGlobalEta->Fill( theTrack->gMom().Eta() );

    // Primary track analysis
    if ( !theTrack->isPrimary() ) continue;

    // Fill primary track histograms
    hPrimaryPt->Fill( theTrack->pPt() );
    hPrimaryNHits->Fill( theTrack->nHits() );
    hPrimaryEta->Fill( theTrack->pMom().Eta() );
    hPrimaryDedxVsPt->Fill( theTrack->charge() * theTrack->pPt(),
                            theTrack->dEdx() );

    // Accessing TOF PID traits information.
    // One has to remember that TOF information is valid for primary tracks ONLY.
    // For global tracks the path length and time-of-flight have to be
    // recalculated by hands.
    if ( theTrack->isTofTrack() ) {
      StPicoBTofPidTraits *trait =
        (StPicoBTofPidTraits*)mPicoDst->btofPidTraits( theTrack->bTofPidTraitsIndex() );
      if (!trait) continue;

      // Fill primary track TOF information
      hPrimaryInvBetaVsP->Fill( theTrack->charge() * theTrack->pPt(),
                                1./trait->btofBeta() );
    } // if ( theTrack->isTofTrack() )
  } // for (Int_t iTrk=0; iTrk<nTracks; iTrk++)

  //
  // The example that shows how to access hit information
  //

  // Get number of BTOW hits (make sure that SetStatus("BTowHit*",1) is set to 1)
  // If it is set to 0, then mPicoDst will return 0 hits all the time
  UInt_t nBTowHits = mPicoDst->numberOfBTowHits();

  // Dummy check, but always good to know that the amount is okay
  if (nBTowHits > 0) {

    // Loop over BTOW hits
    for (UInt_t iHit=0; iHit<nBTowHits; iHit++) {
      // Retrieve i-th BTOW hit
      StPicoBTowHit *btowHit = (StPicoBTowHit*)mPicoDst->btowHit(iHit);
      // Hit must exist
      if (!btowHit) continue;
      // Fill tower ADC
      hBemcTowerAdc->Fill( btowHit->adc() );
    } // for (UInt_t iHit=0; iHit<nBTowHits; iHit++)
  } // if (nBTowHits > 0)


  //=============================================================
  mTree->Fill(); // Fill the TTree with the current myTreeEvent
 //=============================================================

  return kStOk;
}
