#include "MyHit.hh"

G4ThreadLocal G4Allocator<MyHit>* MyHit::MyHitAllocator_ = NULL;

MyHit::MyHit() {

}

MyHit::~MyHit() {

}
