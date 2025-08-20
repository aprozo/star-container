#ifndef StPicoDstAnalysisMaker_h
#define StPicoDstAnalysisMaker_h

// STAR headers
#include "StMaker.h"

// C++ headers
#include <vector>
#include <iostream>

// My headers
#include "MyTreeEvent.h"

// Forward declarations
class StPicoDstMaker;

// StPicoEvent
class StPicoDst;
class StPicoEvent;
class StPicoTrack;

// ROOT
class TFile;
class TH1F;
class TH1D;
class TH2F;

//________________
class StPicoDstAnalysisMaker : public StMaker {

 public:
  /// Constructor
  StPicoDstAnalysisMaker(StPicoDstMaker *maker,
                         TString oFileName = "oStPicoDstAnalysisMaker.root" );
  /// Destructor
  virtual ~StPicoDstAnalysisMaker();
  /// Init method inherited from StMaker
  virtual Int_t Init();
  /// Make method inherited from StMaker
  virtual Int_t Make();
  /// Finish method inherited from StMaker
  virtual Int_t Finish();
  /// Set debug status
  void setDebugStatus(bool status)                      { mDebug = status; }

  /// Add trigger id to select
  void addTriggerId(const unsigned int& id);
  /// Set cut on z-position of the primary vertex
  void setVtxZ(const float& lo, const float& hi)        { mVtxZ[0]=lo; mVtxZ[1]=hi; }
  /// Set cut on the radial position of the primary vertex
  void setVtxR(const float& lo, const float& hi)        { mVtxR[0]=lo; mVtxR[1]=hi; }
  /// Set cut on nHits
  void setNHits(const int& lo, const int& hi)           { mNHits[0] = lo; mNHits[1] = hi; }
  /// Set cut on track pT
  void setPt(const float& lo, const float& hi)          { mPt[0] = lo; mPt[1] = hi; }
  /// Set cut on track pseudorapidity
  void setEta(const float& lo, const float& hi)         { mEta[0] = lo; mEta[1] = hi; }

 private:
  /// Create histograms
  void CreateHistograms();

  /// Check the at least one triggers to select is in the event triggers list
  Bool_t IsGoodTrigger(StPicoEvent *event);

  /// Event cut
  Bool_t EventCut(StPicoEvent *event);

  /// Track cut
  Bool_t TrackCut(StPicoTrack *track);

  /// Debug mode
  Bool_t mDebug;

  /// List of triggers to select
  std::vector<unsigned int> mTriggerId;
  /// z-position of the primary vertex [min,max]
  Float_t mVtxZ[2];
  /// Radial position of the primary vertex [min,max]
  Float_t mVtxR[2];

  /// nHits [min,max]
  Short_t mNHits[2];
  /// Transverse momentum [min,max]
  Float_t mPt[2];
  /// PseudoRapidity [min,max]
  Float_t mEta[2];

  /// Output file name
  TString mOutFileName;
  /// Output file
  TFile *mOutFile;

  StPicoDstMaker *mPicoDstMaker;
  
  // Pointer to StPicoDst
  StPicoDst *mPicoDst;

  // Event histograms
  TH2F *hVtxXvsY;
  TH1F *hVtxZ;

  // Track histograms
  TH1D *hGlobalPt;
  TH1D *hPrimaryPt;
  TH1D *hGlobalNHits;
  TH1D *hPrimaryNHits;
  TH1D *hGlobalEta;
  TH1D *hPrimaryEta;
  TH2F *hPrimaryDedxVsPt;
  TH2F *hPrimaryInvBetaVsP;

  // BEMC hit information
  TH1D *hBemcTowerAdc;

  TTree *mTree; // Tree to store analysis results
  MyTreeEvent *myTreeEvent; // Object to hold event data

  ClassDef(StPicoDstAnalysisMaker, 0)
};

#endif // StPicoDstAnalysisMaker_h
