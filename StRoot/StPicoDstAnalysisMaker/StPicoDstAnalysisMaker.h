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
  StPicoDstAnalysisMaker(StPicoDstMaker *maker,
                         TString oFileName = "oStPicoDstAnalysisMaker.root" ); // Constructor
  virtual ~StPicoDstAnalysisMaker();  // Destructor
  // inherited methods from StMaker
  virtual Int_t Init();
  virtual Int_t Make();
  virtual Int_t Finish();
  //
  inline void setDebugStatus(bool status)                      { mDebug = status; }

  inline void setVtxZ(const float& lo, const float& hi)        { mVtxZ[0]=lo; mVtxZ[1]=hi; } // cut on z-position of the primary vertex
  inline void setVtxR(const float& lo, const float& hi)        { mVtxR[0]=lo; mVtxR[1]=hi; }
  inline void setNHits(const int& lo, const int& hi)           { mNHits[0] = lo; mNHits[1] = hi; } //cut on nHits
  inline void setPt(const float& lo, const float& hi)          { mPt[0] = lo; mPt[1] = hi; } //cut on track pT
  inline void setEta(const float& lo, const float& hi)         { mEta[0] = lo; mEta[1] = hi; } //cut on track pseudorapidity

  inline void addTriggerId(const unsigned int& id) { mTriggerId.push_back(id); }

 private:
  void CreateHistograms();

  Bool_t IsGoodTrigger(StPicoEvent *event); // Check if at least one of the selected triggers is present in the event's trigger list
  Bool_t EventCut(StPicoEvent *event);
  Bool_t TrackCut(StPicoTrack *track);

  StPicoDstMaker *mPicoDstMaker;
  StPicoDst *mPicoDst;

  Bool_t mDebug;  // Debug mode

  std::vector<unsigned int> mTriggerId;   // List of triggers to select

  Float_t mVtxZ[2];  // z-position of the primary vertex [min,max]
  Float_t mVtxR[2];  // Radial position of the primary vertex [min,max]
  Short_t mNHits[2]; // nHits [min,max]
  Float_t mPt[2]; // Transverse momentum [min,max]
  Float_t mEta[2];  // PseudoRapidity [min,max]

  TString mOutFileName;  // Output file name
  TFile *mOutFile;  // Output file
 
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

  // Tree to store analysis results
  TTree *mTree; 
  MyTreeEvent *myTreeEvent; // Object to hold event data at the time of Fill()

  ClassDef(StPicoDstAnalysisMaker, 0)
};

#endif // StPicoDstAnalysisMaker_h
