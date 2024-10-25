#include "node_printer.hpp"

#include <node_buffer.h>

NAN_MODULE_INIT(Init) {
// only for node
    MY_MODULE_SET_METHOD(target, "getPrinters", getPrinters);
    MY_MODULE_SET_METHOD(target, "getDefaultPrinterName", getDefaultPrinterName);
}

#if NODE_MAJOR_VERSION >= 10
NAN_MODULE_WORKER_ENABLED(node_printer, Init)
#else
NODE_MODULE(node_printer, Init)
#endif

// Helpers

bool getStringOrBufferFromV8Value(v8::Local<v8::Value> iV8Value, std::string &oData)
{
    if(iV8Value->IsString())
    {
        Nan::Utf8String data_str_v8(V8_LOCAL_STRING_FROM_VALUE(iV8Value));
        oData.assign(*data_str_v8, data_str_v8.length());
        return true;
    }
    if(iV8Value->IsObject() && node::Buffer::HasInstance(iV8Value))
    {
        oData.assign(node::Buffer::Data(iV8Value), node::Buffer::Length(iV8Value));
        return true;
    }
    return false;
}
