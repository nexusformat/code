#include "Segment.h"

Segment::Segment() {}

Segment::Segment( int dID, int rw, int col, float len, float wid,
		  float dep, float eff, int sID ) {
  detID = dID;
  row = rw;
  column = col;
  length = len;
  width = wid;
  depth = dep;
  efficiency = eff;
  segID = sID;
  
}

int Segment::DetID(){
  return detID;
}

int Segment::Row(){
  return row;
}

int Segment::Column(){
  return column;
}

int Segment::SegID(){
  return segID;
}

float Segment::Length(){
  return length;
}

float Segment::Width(){
  return width;
}

float Segment::Depth(){
  return depth;
}

float Segment::Efficiency(){
  return efficiency;
}

void Segment::SetDetID(int dID){
  detID = dID;
}
void Segment::SetRow(int rw){
  row = rw;
}
void Segment::SetColumn(int col){
  column = col;
}
void Segment::SetSegID(int sID){
  segID = sID;
}
void Segment::SetLength(float len){
  length = len;
}
void Segment::SetWidth(float wid){
  width = wid;
}
void Segment::SetDepth(float dep){
  depth = dep;
}
void Segment::SetEfficiency(float eff){
  efficiency = eff;
}

