#ifndef MYTRACK_H
#define MYTRACK_H

#include "TObject.h"

class  MyTrack {
  // Class to represent a track candidate in the analysis
  // Contains basic properties of a track candidate
public:
    int id;          // Unique identifier for the track candidate
    float pt;       // Transverse momentum of the track candidate
    float eta;      // Pseudorapidity of the track candidate
    float phi;      // Azimuthal angle of the track candidate
    int charge;     // Electric charge of the track candidate
    MyTrack() {}
    ~MyTrack() {}

  ClassDef(MyTrack, 1);
};


#endif // MYTRACK_H

