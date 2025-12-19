#ifndef TRACKINGACTION_HH
#define TRACKINGACTION_HH

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"
#include "globals.hh"

class MyTrackingAction : public G4UserTrackingAction {
public:
    MyTrackingAction();
    virtual ~MyTrackingAction();

    virtual void PreUserTrackingAction(const G4Track* track);
};

#endif

