#include <node.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "chunk.h"
#include "worldGenerator.h"

using namespace v8;

Handle<Value> generate(const Arguments& args);
void AsyncWork(uv_work_t* req);
void AsyncAfter(uv_work_t* req);

std::string itos(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

Handle<Value> create(const Arguments& args) {
  HandleScope scope;
  return scope.Close(Chunk::NewInstance(args));
}

Handle<Value> get(const Arguments& args) {
  HandleScope scope;

  Chunk* obj = node::ObjectWrap::Unwrap<Chunk>(
      args[0]->ToObject());

  return scope.Close(Number::New(obj->getField(args[1]->NumberValue(), args[2]->NumberValue())));
}

Handle<Value> saveToFile(const Arguments& args) {
  HandleScope scope;

  Chunk* obj = node::ObjectWrap::Unwrap<Chunk>(
      args[0]->ToObject());
	
  String::AsciiValue filename(args[1]->ToString());

  std::ofstream file;
  file.open (*filename);
  
  for(int i=0; i<16*16;i++) {
  	file << itos((uint32_t)obj->fields[i]) << "\n";
  }
  file.close();  
  
}
Handle<Value> loadFromFile(const Arguments& args) {
  HandleScope scope;

  Chunk* obj = node::ObjectWrap::Unwrap<Chunk>(
      args[0]->ToObject());

  String::AsciiValue filename(args[1]->ToString());

  std::string line;
  int num = 0;
  std::ifstream file (*filename);
  if (file.is_open())
  {
    while ( file.good() )
    {
      getline (file,line);
      obj->fields[num] = atoi(line.c_str());
      num++;
    }
  }
}

Handle<Value> set(const Arguments& args) {
  HandleScope scope;

  Chunk* obj = node::ObjectWrap::Unwrap<Chunk>(
      args[0]->ToObject());

  obj->setField(args[1]->NumberValue(), args[2]->NumberValue(), args[3]->NumberValue());
}

struct Baton {
    // libuv's request struct.
    uv_work_t request;

    //The Callback Function
    Persistent<Function> callback;

    int X,Z,seed;
    Chunk* obj;
};

Handle<Value> generate(const Arguments& args) {
    HandleScope scope;

    if (!args[1]->IsFunction()) {
        return ThrowException(Exception::TypeError(
            String::New("Fourt argument must be a callback function")));
    }

    Local<Function> callback = Local<Function>::Cast(args[1]);

    //Create new Data Structur
    Baton* baton = new Baton();
    baton->request.data = baton;
    baton->callback = Persistent<Function>::New(callback);

    baton->obj = node::ObjectWrap::Unwrap<Chunk>(args[0]->ToObject());

    // Schedule our work request with libuv.
    int status = uv_queue_work(uv_default_loop(), &baton->request, AsyncWork, AsyncAfter);
    assert(status == 0);

    return Undefined();
}

void AsyncWork(uv_work_t* req) {
    Baton* baton = static_cast<Baton*>(req->data);
 

   for(int x=0; x < 16; x++) {
        for(int z=0; z < 16; z++) {
            baton->obj->setField(x,z, (uint8_t) (baton->obj->wgen->getNoiseData(baton->obj->X+x,baton->obj->Z+z)));
       }
    }
}


void AsyncAfter(uv_work_t* req) { 
     HandleScope scope;
     Baton* baton = static_cast<Baton*>(req->data);

     //We will give nothing back
     const unsigned argc = 2;

     Local<Value> argv[argc] = {
        Local<Value>::New(Integer::New(baton->obj->X)),
        Local<Value>::New(Integer::New(baton->obj->Z)) 
     };

     //Call the callback
     TryCatch try_catch;
     baton->callback->Call(Context::GetCurrent()->Global(), argc, argv);
     if (try_catch.HasCaught()) {
        node::FatalException(try_catch);
     }


     baton->callback.Dispose();
     delete baton;
}

void RegisterModule(Handle<Object> target) {
    Chunk::Init();

    target->Set(String::NewSymbol("create"),
        FunctionTemplate::New(create)->GetFunction());

    target->Set(String::NewSymbol("generate"),
        FunctionTemplate::New(generate)->GetFunction());

    target->Set(String::NewSymbol("get"),
	FunctionTemplate::New(get)->GetFunction());

    target->Set(String::NewSymbol("set"),
	FunctionTemplate::New(set)->GetFunction());

    target->Set(String::NewSymbol("saveToFile"),
	FunctionTemplate::New(saveToFile)->GetFunction());

    target->Set(String::NewSymbol("loadFromFile"),
	FunctionTemplate::New(loadFromFile)->GetFunction());
}

NODE_MODULE(chunk, RegisterModule);
