#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <iostream>
#include "chunk.h"
#include "worldGenerator.h"

using namespace v8;

Chunk::Chunk() {};
Chunk::~Chunk() {};

Persistent<Function> Chunk::constructor;

void Chunk::Init() {
  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Chunk"));
  tpl->InstanceTemplate()->SetInternalFieldCount(6);

  constructor = Persistent<Function>::New(tpl->GetFunction());
}

Handle<Value> Chunk::New(const Arguments& args) {
  HandleScope scope;

  Chunk* obj = new Chunk();
  obj->X = args[0]->NumberValue();
  obj->Z = args[1]->NumberValue();
  obj->seed = args[2]->NumberValue();
  obj->wgen = new worldGenerator(args[3]->NumberValue(), args[4]->NumberValue(), args[5]->NumberValue(), obj->seed);
  obj->Wrap(args.This());

  return args.This();
}

Handle<Value> Chunk::NewInstance(const Arguments& args) {
  HandleScope scope;

  const unsigned argc = 6;
  Handle<Value> argv[argc] = { args[0],args[1],args[2], args[3], args[4], args[5] };
  Local<Object> instance = constructor->NewInstance(argc, argv);

  return scope.Close(instance);
}
