#ifndef IPNS_RUNFILE_SEGMENT_H
#define IPNS_RUNFILE_SEGMENT_H

class Segment {
 public:
  Segment();
  Segment( int, int, int, float, float, float, float, int);
  int DetID();
  int Row();
  int Column();
  int SegID();
  float Length();
  float Width();
  float Depth();
  float Efficiency();
  void SetDetID(int);
  void SetRow(int);
  void SetColumn(int);
  void SetSegID(int);
  void SetLength(float);
  void SetWidth(float);
  void SetDepth(float);
  void SetEfficiency(float);

 private:
  int detID;
  int row;
  int column;
  int segID;
  float length;
  float width;
  float depth;
  float efficiency;
};

#endif
