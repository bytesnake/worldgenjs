#define BUILDING_NODE_EXTENSION
#ifndef Chunk_H
#define Chunk_H

#include <node.h>
#include "worldGenerator.h"

#define BLOCK_INDEX(x,z) (x+z*16)

class Chunk : public node::ObjectWrap {
 public:
  static void Init();
  static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
  uint8_t getField(int x, int z) const { return fields[BLOCK_INDEX(x,z)]; }
  void setField(int x, int z, uint8_t id ) { fields[BLOCK_INDEX(x,z)] = id; }
  worldGenerator* wgen;
  int X;
  int Z;
  int seed;
  uint8_t fields[16*16];

  Chunk();
  ~Chunk();

 private:


  static v8::Persistent<v8::Function> constructor;
  static v8::Handle<v8::Value> New(const v8::Arguments& args);

};

#endif
