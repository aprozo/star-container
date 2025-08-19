#ifndef  MyTreeEvent_h
#define  MyTreeEvent_h

#include "MyTrack.h"
#include <vector>


class  MyTreeEvent{
public:
    Int_t centrality, eventId;
    Float_t vertexX, vertexY, vertexZ; // Vertex coordinates
    std::vector<MyTrack> inclusiveTracks; // Vector of inclusive track candidates
    MyTreeEvent() {}
    ~MyTreeEvent() {}

  ClassDef(MyTreeEvent, 1);
};


#endif // 

