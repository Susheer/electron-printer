#ifndef NODE_PRINTER_HPP
#define NODE_PRINTER_HPP

#include "macros.hh"

#include <node.h>
#include <v8.h>

#include <string>


/** Retrieve all printers and jobs
 * posix: minimum version: CUPS 1.1.21/OS X 10.4
 */
MY_NODE_MODULE_CALLBACK(getPrinters);

/**
 * Return default printer name, if null then default printer is not set
 */
MY_NODE_MODULE_CALLBACK(getDefaultPrinterName);

// util class

/** Memory value class management to avoid memory leak
 * TODO: move to std::unique_ptr on switching to C++11
*/
template<typename Type>
class MemValueBase
{
public:
    MemValueBase(): _value(NULL) {}

    /** Destructor. The allocated memory will be deallocated
    */
    virtual ~MemValueBase() {}

    Type * get() {return _value; }
    Type * operator ->() { return &_value; }
    operator bool() const { return (_value != NULL); }
protected:
    Type *_value;

    virtual void free() {};
};

/**
 * try to extract String or buffer from v8 value
 * @param iV8Value - source v8 value
 * @param oData - destination data
 * @return TRUE if value is String or Buffer, FALSE otherwise
 */
bool getStringOrBufferFromV8Value(v8::Local<v8::Value> iV8Value, std::string &oData);

#endif
