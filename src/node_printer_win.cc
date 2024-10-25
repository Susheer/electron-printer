#include "node_printer.hpp"

#if _MSC_VER
#include <windows.h>
#include <Winspool.h>
#include <Wingdi.h>
#pragma  comment(lib, "Winspool.lib")
#else
#error "Unsupported compiler for windows. Feel free to add it."
#endif

#include <string>
#include <map>
#include <utility>
#include <sstream>
#include <node_version.h>
#include <cstdio> // Include for printf

// Utility function to output messages
void ConsolePrint(const std::string& message) {
    printf("%s\n", message.c_str());
    fflush(stdout); // Ensure the output is flushed immediately
}

namespace{
    typedef std::map<std::string, DWORD> StatusMapType;

    /** Memory value class management to avoid memory leak
    */
    template<typename Type>
    class MemValue: public MemValueBase<Type> {
    public:
        /** Constructor of allocating iSizeKbytes bytes memory;
        * @param iSizeKbytes size in bytes of required allocating memory
        */
        MemValue(const DWORD iSizeKbytes) {
            _value = (Type*)malloc(iSizeKbytes);
        }
		
        ~MemValue () {
            free();
        }

        Type* get() {
        return _value;
        }
    protected:
        virtual void free() {
            if(_value != NULL)
            {
                ::free(_value);
                _value = NULL;
            }
        }

    private:
    Type* _value; // Declaration of the member variable
    };

    struct PrinterHandle
    {
        PrinterHandle(LPWSTR iPrinterName)
        {
            _ok = OpenPrinterW(iPrinterName, &_printer, NULL);
        }
        ~PrinterHandle()
        {
            if(_ok)
            {
                ClosePrinter(_printer);
            }
        }
        operator HANDLE() {return _printer;}
        operator bool() { return (!!_ok);}
        HANDLE & operator *() { return _printer;}
        HANDLE * operator ->() { return &_printer;}
        const HANDLE & operator ->() const { return _printer;}
        HANDLE _printer;
        BOOL _ok;
    };

    const StatusMapType& getStatusMap()
    {
        static StatusMapType result;
        if(!result.empty())
        {
            return result;
        }
        // add only first time
#define STATUS_PRINTER_ADD(value, type) result.insert(std::make_pair(value, type))
        STATUS_PRINTER_ADD("BUSY", PRINTER_STATUS_BUSY);
        STATUS_PRINTER_ADD("DOOR-OPEN", PRINTER_STATUS_DOOR_OPEN);
        STATUS_PRINTER_ADD("ERROR", PRINTER_STATUS_ERROR);
        STATUS_PRINTER_ADD("INITIALIZING", PRINTER_STATUS_INITIALIZING);
        STATUS_PRINTER_ADD("IO-ACTIVE", PRINTER_STATUS_IO_ACTIVE);
        STATUS_PRINTER_ADD("MANUAL-FEED", PRINTER_STATUS_MANUAL_FEED);
        STATUS_PRINTER_ADD("NO-TONER", PRINTER_STATUS_NO_TONER);
        STATUS_PRINTER_ADD("NOT-AVAILABLE", PRINTER_STATUS_NOT_AVAILABLE);
        STATUS_PRINTER_ADD("OFFLINE", PRINTER_STATUS_OFFLINE);
        STATUS_PRINTER_ADD("OUT-OF-MEMORY", PRINTER_STATUS_OUT_OF_MEMORY);
        STATUS_PRINTER_ADD("OUTPUT-BIN-FULL", PRINTER_STATUS_OUTPUT_BIN_FULL);
        STATUS_PRINTER_ADD("PAGE-PUNT", PRINTER_STATUS_PAGE_PUNT);
        STATUS_PRINTER_ADD("PAPER-JAM", PRINTER_STATUS_PAPER_JAM);
        STATUS_PRINTER_ADD("PAPER-OUT", PRINTER_STATUS_PAPER_OUT);
        STATUS_PRINTER_ADD("PAPER-PROBLEM", PRINTER_STATUS_PAPER_PROBLEM);
        STATUS_PRINTER_ADD("PAUSED", PRINTER_STATUS_PAUSED);
        STATUS_PRINTER_ADD("PENDING-DELETION", PRINTER_STATUS_PENDING_DELETION);
        STATUS_PRINTER_ADD("POWER-SAVE", PRINTER_STATUS_POWER_SAVE);
        STATUS_PRINTER_ADD("PRINTING", PRINTER_STATUS_PRINTING);
        STATUS_PRINTER_ADD("PROCESSING", PRINTER_STATUS_PROCESSING);
        STATUS_PRINTER_ADD("SERVER-UNKNOWN", PRINTER_STATUS_SERVER_UNKNOWN);
        STATUS_PRINTER_ADD("TONER-LOW", PRINTER_STATUS_TONER_LOW);
        STATUS_PRINTER_ADD("USER-INTERVENTION", PRINTER_STATUS_USER_INTERVENTION);
        STATUS_PRINTER_ADD("WAITING", PRINTER_STATUS_WAITING);
        STATUS_PRINTER_ADD("WARMING-UP", PRINTER_STATUS_WARMING_UP);
#undef STATUS_PRINTER_ADD
        return result;
    }

    const StatusMapType& getJobStatusMap()
    {
        static StatusMapType result;
        if(!result.empty())
        {
            return result;
        }
        // add only first time
#define STATUS_PRINTER_ADD(value, type) result.insert(std::make_pair(value, type))
        // Common statuses
        STATUS_PRINTER_ADD("PRINTING", JOB_STATUS_PRINTING);
        STATUS_PRINTER_ADD("PRINTED", JOB_STATUS_PRINTED);
        STATUS_PRINTER_ADD("PAUSED", JOB_STATUS_PAUSED);

        // Specific statuses
        STATUS_PRINTER_ADD("BLOCKED-DEVQ", JOB_STATUS_BLOCKED_DEVQ);
        STATUS_PRINTER_ADD("DELETED", JOB_STATUS_DELETED);
        STATUS_PRINTER_ADD("DELETING", JOB_STATUS_DELETING);
        STATUS_PRINTER_ADD("ERROR", JOB_STATUS_ERROR);
        STATUS_PRINTER_ADD("OFFLINE", JOB_STATUS_OFFLINE);
        STATUS_PRINTER_ADD("PAPEROUT", JOB_STATUS_PAPEROUT);
        STATUS_PRINTER_ADD("RESTART", JOB_STATUS_RESTART);
        STATUS_PRINTER_ADD("SPOOLING", JOB_STATUS_SPOOLING);
        STATUS_PRINTER_ADD("USER-INTERVENTION", JOB_STATUS_USER_INTERVENTION);
        // XP and later
#ifdef JOB_STATUS_COMPLETE
        STATUS_PRINTER_ADD("COMPLETE", JOB_STATUS_COMPLETE);
#endif
#ifdef JOB_STATUS_RETAINED
        STATUS_PRINTER_ADD("RETAINED", JOB_STATUS_RETAINED);
#endif

#undef STATUS_PRINTER_ADD
        return result;
    }

    const StatusMapType& getAttributeMap()
    {
        static StatusMapType result;
        if(!result.empty())
        {
            return result;
        }
        // add only first time
#define ATTRIBUTE_PRINTER_ADD(value, type) result.insert(std::make_pair(value, type))
        ATTRIBUTE_PRINTER_ADD("DIRECT", PRINTER_ATTRIBUTE_DIRECT);
        ATTRIBUTE_PRINTER_ADD("DO-COMPLETE-FIRST", PRINTER_ATTRIBUTE_DO_COMPLETE_FIRST);
        ATTRIBUTE_PRINTER_ADD("ENABLE-DEVQ", PRINTER_ATTRIBUTE_ENABLE_DEVQ);
        ATTRIBUTE_PRINTER_ADD("HIDDEN", PRINTER_ATTRIBUTE_HIDDEN);
        ATTRIBUTE_PRINTER_ADD("KEEPPRINTEDJOBS", PRINTER_ATTRIBUTE_KEEPPRINTEDJOBS);
        ATTRIBUTE_PRINTER_ADD("LOCAL", PRINTER_ATTRIBUTE_LOCAL);
        ATTRIBUTE_PRINTER_ADD("NETWORK", PRINTER_ATTRIBUTE_NETWORK);
        ATTRIBUTE_PRINTER_ADD("PUBLISHED", PRINTER_ATTRIBUTE_PUBLISHED);
        ATTRIBUTE_PRINTER_ADD("QUEUED", PRINTER_ATTRIBUTE_QUEUED);
        ATTRIBUTE_PRINTER_ADD("RAW-ONLY", PRINTER_ATTRIBUTE_RAW_ONLY);
        ATTRIBUTE_PRINTER_ADD("SHARED", PRINTER_ATTRIBUTE_SHARED);
        ATTRIBUTE_PRINTER_ADD("OFFLINE", PRINTER_ATTRIBUTE_WORK_OFFLINE);
        // XP
#ifdef PRINTER_ATTRIBUTE_FAX
        ATTRIBUTE_PRINTER_ADD("FAX", PRINTER_ATTRIBUTE_FAX);
#endif
        // vista
#ifdef PRINTER_ATTRIBUTE_FRIENDLY_NAME
        ATTRIBUTE_PRINTER_ADD("FRIENDLY-NAME", PRINTER_ATTRIBUTE_FRIENDLY_NAME);
        ATTRIBUTE_PRINTER_ADD("MACHINE", PRINTER_ATTRIBUTE_MACHINE);
        ATTRIBUTE_PRINTER_ADD("PUSHED-USER", PRINTER_ATTRIBUTE_PUSHED_USER);
        ATTRIBUTE_PRINTER_ADD("PUSHED-MACHINE", PRINTER_ATTRIBUTE_PUSHED_MACHINE);
#endif
        // server 2003
#ifdef PRINTER_ATTRIBUTE_TS
        ATTRIBUTE_PRINTER_ADD("TS", PRINTER_ATTRIBUTE_TS);
#endif
#undef ATTRIBUTE_PRINTER_ADD
        return result;
    }

    const StatusMapType& getJobCommandMap()
    {
        static StatusMapType result;
        if(!result.empty())
        {
            return result;
        }
        // add only first time
#define COMMAND_JOB_ADD(value, type) result.insert(std::make_pair(value, type))
        COMMAND_JOB_ADD("CANCEL", JOB_CONTROL_CANCEL);
        COMMAND_JOB_ADD("PAUSE", JOB_CONTROL_PAUSE);
        COMMAND_JOB_ADD("RESTART", JOB_CONTROL_RESTART);
        COMMAND_JOB_ADD("RESUME", JOB_CONTROL_RESUME);
        COMMAND_JOB_ADD("DELETE", JOB_CONTROL_DELETE);
        COMMAND_JOB_ADD("SENT-TO-PRINTER", JOB_CONTROL_SENT_TO_PRINTER);
        COMMAND_JOB_ADD("LAST-PAGE-EJECTED", JOB_CONTROL_LAST_PAGE_EJECTED);
#ifdef JOB_CONTROL_RETAIN
        COMMAND_JOB_ADD("RETAIN", JOB_CONTROL_RETAIN);
#endif
#ifdef JOB_CONTROL_RELEASE
        COMMAND_JOB_ADD("RELEASE", JOB_CONTROL_RELEASE);
#endif
#undef COMMAND_JOB_ADD
        return result;
    }

    void parseJobObject(JOB_INFO_2W *job, v8::Local<v8::Object> result_printer_job)
    {
        MY_NODE_MODULE_ISOLATE_DECL
        //Common fields
        //DWORD                JobId;
        Nan::Set(result_printer_job, V8_STRING_NEW_UTF8("id"), V8_VALUE_NEW(Number, job->JobId));
#define ADD_V8_STRING_PROPERTY(name, key) if((job->##key != NULL) && (*job->##key != L'\0'))    \
        {                                   \
            Nan::Set(result_printer_job, V8_STRING_NEW_UTF8(#name), V8_STRING_NEW_2BYTES((uint16_t*)job->##key)); \
        }
        //LPTSTR               pPrinterName;
        ADD_V8_STRING_PROPERTY(name, pPrinterName)
        //LPTSTR               pPrinterName;
        ADD_V8_STRING_PROPERTY(printerName, pPrinterName);
        //LPTSTR               pUserName;
        ADD_V8_STRING_PROPERTY(user, pUserName);
        //LPTSTR               pDatatype;
        ADD_V8_STRING_PROPERTY(format, pDatatype);
        //DWORD                Priority;
        Nan::Set(result_printer_job, V8_STRING_NEW_UTF8("priority"), V8_VALUE_NEW(Number, job->Priority));
        //DWORD                Size;
        Nan::Set(result_printer_job, V8_STRING_NEW_UTF8("size"), V8_VALUE_NEW(Number, job->Size));
        //DWORD                Status;
        v8::Local<v8::Array> result_printer_job_status = V8_VALUE_NEW_DEFAULT(Array);
        int i_status = 0;
        for(StatusMapType::const_iterator itStatus = getJobStatusMap().begin(); itStatus != getJobStatusMap().end(); ++itStatus)
        {
            if(job->Status & itStatus->second)
            {
                Nan::Set(result_printer_job_status, i_status++, V8_STRING_NEW_UTF8(itStatus->first.c_str()));
            }
        }
        //LPTSTR               pStatus;
        if((job->pStatus != NULL) && (*job->pStatus != L'\0'))
        {
            Nan::Set(result_printer_job_status, i_status++, V8_STRING_NEW_2BYTES((uint16_t*)job->pStatus));
        }
        Nan::Set(result_printer_job, V8_STRING_NEW_UTF8("status"), result_printer_job_status);

        // Specific fields
        //LPTSTR               pMachineName;
        ADD_V8_STRING_PROPERTY(machineName, pMachineName);
        //LPTSTR               pDocument;
        ADD_V8_STRING_PROPERTY(document, pDocument);
        //LPTSTR               pNotifyName;
        ADD_V8_STRING_PROPERTY(notifyName, pNotifyName);
        //LPTSTR               pPrintProcessor;
        ADD_V8_STRING_PROPERTY(printProcessor, pPrintProcessor);
        //LPTSTR               pParameters;
        ADD_V8_STRING_PROPERTY(parameters, pParameters);
        //LPTSTR               pDriverName;
        ADD_V8_STRING_PROPERTY(driverName, pDriverName);
#undef ADD_V8_STRING_PROPERTY
        //LPDEVMODE            pDevMode;
        //PSECURITY_DESCRIPTOR pSecurityDescriptor;
        //DWORD                Position;
        Nan::Set(result_printer_job, V8_STRING_NEW_UTF8("position"), V8_VALUE_NEW(Number, job->Position));
        //DWORD                StartTime;
        Nan::Set(result_printer_job, V8_STRING_NEW_UTF8("startTime"), V8_VALUE_NEW(Number, job->StartTime));
        //DWORD                UntilTime;
        Nan::Set(result_printer_job, V8_STRING_NEW_UTF8("untilTime"), V8_VALUE_NEW(Number, job->UntilTime));
        //DWORD                TotalPages;
        Nan::Set(result_printer_job, V8_STRING_NEW_UTF8("totalPages"), V8_VALUE_NEW(Number, job->TotalPages));
        //SYSTEMTIME           Submitted;
        //DWORD                Time;
        Nan::Set(result_printer_job, V8_STRING_NEW_UTF8("time"), V8_VALUE_NEW(Number, job->Time));
        //DWORD                PagesPrinted;
        Nan::Set(result_printer_job, V8_STRING_NEW_UTF8("pagesPrinted"), V8_VALUE_NEW(Number, job->PagesPrinted));
    }

    /**
     * Returns last error code and message string
     */
    std::string getLastErrorCodeAndMessage() {
    	std::ostringstream s;
    	DWORD erroCode = GetLastError();
    	s << "code: " << erroCode;
    	DWORD retSize;
    	LPTSTR pTemp = NULL;
    	retSize = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|
                                FORMAT_MESSAGE_FROM_SYSTEM|
                                FORMAT_MESSAGE_ARGUMENT_ARRAY,
                                NULL,
                                erroCode,
                                LANG_NEUTRAL,
                                (LPTSTR)&pTemp,
                                0,
                                NULL );
        if (retSize && pTemp != NULL) {
	    //pTemp[strlen(pTemp)-2]='\0'; //remove cr and newline character
	    //TODO: check if it is needed to convert c string to std::string
	    std::string stringMessage(pTemp);
	    s << ", message: " << stringMessage;
	    LocalFree((HLOCAL)pTemp);
	}

    	return s.str();
    }

    std::string retrieveAndParseJobs(const LPWSTR iPrinterName,
                                     const DWORD& iTotalJobs,
                                     v8::Local<v8::Object> result_printer_jobs,
                                     PrinterHandle& iPrinterHandle)
    {
        MY_NODE_MODULE_ISOLATE_DECL
        DWORD bytes_needed = 0, totalJobs = 0;
        BOOL bError = EnumJobsW(*iPrinterHandle, 0, iTotalJobs, 2, NULL, bytes_needed, &bytes_needed, &totalJobs);
        MemValue<JOB_INFO_2W> jobs(bytes_needed);
        if(!jobs)
        {
            std::string error_str("Error on allocating memory for jobs: ");
            error_str += getLastErrorCodeAndMessage();
            v8::Local<v8::Object> result_printer_job = V8_VALUE_NEW_DEFAULT(Object);
            Nan::Set(result_printer_job, V8_STRING_NEW_UTF8("error"), V8_STRING_NEW_UTF8(error_str.c_str()));
            Nan::Set(result_printer_jobs, 0, result_printer_job);
            return std::string("");
        }
        DWORD dummy_bytes = 0;
        bError = EnumJobsW(*iPrinterHandle, 0, iTotalJobs, 2, (LPBYTE)jobs.get(), bytes_needed, &dummy_bytes, &totalJobs);
        if(!bError)
        {
            std::string error_str("Error on EnumJobsW: ");
            error_str += getLastErrorCodeAndMessage();
            v8::Local<v8::Object> result_printer_job = V8_VALUE_NEW_DEFAULT(Object);
            Nan::Set(result_printer_job, V8_STRING_NEW_UTF8("error"), V8_STRING_NEW_UTF8(error_str.c_str()));
            Nan::Set(result_printer_jobs, 0, result_printer_job);
            return std::string("");
        }
        JOB_INFO_2W *job = jobs.get();
        for(DWORD i = 0; i < totalJobs; ++i, ++job)
        {
            v8::Local<v8::Object> result_printer_job = V8_VALUE_NEW_DEFAULT(Object);
            parseJobObject(job, result_printer_job);
            Nan::Set(result_printer_jobs, i, result_printer_job);
        }
        return std::string("");
    }

    std::string parsePrinterInfo(const PRINTER_INFO_2W *printer, v8::Local<v8::Object> result_printer, PrinterHandle& iPrinterHandle)
    {
        MY_NODE_MODULE_ISOLATE_DECL
    #define ADD_V8_STRING_PROPERTY(name, key) if((printer->##key != NULL) && (*printer->##key != L'\0'))    \
        {                                   \
            Nan::Set(result_printer, V8_STRING_NEW_UTF8(#name), V8_STRING_NEW_2BYTES((uint16_t*)printer->##key)); \
        }
        //LPTSTR               pPrinterName;
        ADD_V8_STRING_PROPERTY(name, pPrinterName)
        //LPTSTR               pServerName;
        ADD_V8_STRING_PROPERTY(serverName, pServerName)
        //LPTSTR               pShareName;
        ADD_V8_STRING_PROPERTY(shareName, pShareName)
        //LPTSTR               pPortName;
        ADD_V8_STRING_PROPERTY(portName, pPortName)
        //LPTSTR               pDriverName;
        ADD_V8_STRING_PROPERTY(driverName, pDriverName)
        //LPTSTR               pComment;
        ADD_V8_STRING_PROPERTY(comment, pComment)
        //LPTSTR               pLocation;
        ADD_V8_STRING_PROPERTY(location, pLocation)
        //LPTSTR               pSepFile;
        ADD_V8_STRING_PROPERTY(sepFile, pSepFile)
        //LPTSTR               pPrintProcessor;
        ADD_V8_STRING_PROPERTY(printProcessor, pPrintProcessor)
        //LPTSTR               pDatatype;
        ADD_V8_STRING_PROPERTY(datatype, pDatatype)
        //LPTSTR               pParameters;
        ADD_V8_STRING_PROPERTY(parameters, pParameters)
    #undef ADD_V8_STRING_PROPERTY
        //DWORD                Status;
        // statuses from:
        // http://msdn.microsoft.com/en-gb/library/windows/desktop/dd162845(v=vs.85).aspx
        v8::Local<v8::Array> result_printer_status = V8_VALUE_NEW_DEFAULT(Array);
        int i_status = 0;
        for(StatusMapType::const_iterator itStatus = getStatusMap().begin(); itStatus != getStatusMap().end(); ++itStatus)
        {
            if(printer->Status & itStatus->second)
            {
                Nan::Set(result_printer_status, i_status, V8_STRING_NEW_UTF8(itStatus->first.c_str()));
                ++i_status;
            }
        }
        Nan::Set(result_printer, V8_STRING_NEW_UTF8("status"), result_printer_status);
        Nan::Set(result_printer, V8_STRING_NEW_UTF8("statusNumber"), V8_VALUE_NEW(Number, printer->Status));
        //DWORD                Attributes;
        v8::Local<v8::Array> result_printer_attributes = V8_VALUE_NEW_DEFAULT(Array);
        int i_attribute = 0;
        for(StatusMapType::const_iterator itAttribute = getAttributeMap().begin(); itAttribute != getAttributeMap().end(); ++itAttribute)
        {
            if(printer->Attributes & itAttribute->second)
            {
                Nan::Set(result_printer_attributes, i_attribute, V8_STRING_NEW_UTF8(itAttribute->first.c_str()));
                ++i_attribute;
            }
        }
        Nan::Set(result_printer, V8_STRING_NEW_UTF8("attributes"), result_printer_attributes);
        //DWORD                Priority;
        Nan::Set(result_printer, V8_STRING_NEW_UTF8("priority"), V8_VALUE_NEW(Number, printer->Priority));
        //DWORD                DefaultPriority;
        Nan::Set(result_printer, V8_STRING_NEW_UTF8("defaultPriority"), V8_VALUE_NEW(Number, printer->DefaultPriority));
        //DWORD                cJobs;
        //Nan::Set(result_printer, V8_STRING_NEW_UTF8("jobs"), V8_VALUE_NEW(Number, printer->cJobs));
        //DWORD                AveragePPM;
        Nan::Set(result_printer, V8_STRING_NEW_UTF8("averagePPM"), V8_VALUE_NEW(Number, printer->AveragePPM));

        //DWORD                StartTime;
        if(printer->StartTime > 0)
        {
            Nan::Set(result_printer, V8_STRING_NEW_UTF8("startTime"), V8_VALUE_NEW(Number, printer->StartTime));
        }
        //DWORD                UntilTime;
        if(printer->UntilTime > 0)
        {
            Nan::Set(result_printer, V8_STRING_NEW_UTF8("untilTime"), V8_VALUE_NEW(Number, printer->UntilTime));
        }

        //TODO: to finish to extract all data
        //LPDEVMODE            pDevMode;
        //PSECURITY_DESCRIPTOR pSecurityDescriptor;

        if(printer->cJobs > 0)
        {
            v8::Local<v8::Array> result_printer_jobs = V8_VALUE_NEW(Array, printer->cJobs);
            // get jobs
            std::string error_str = retrieveAndParseJobs(printer->pPrinterName, printer->cJobs, result_printer_jobs, iPrinterHandle);
            if(!error_str.empty())
            {
                return error_str;
            }
            Nan::Set(result_printer, V8_STRING_NEW_UTF8("jobs"), result_printer_jobs);
        }
        return "";
    }
}

MY_NODE_MODULE_CALLBACK(getPrinters)
{
    MY_NODE_MODULE_HANDLESCOPE;
    //ConsolePrint("Invoked ------------ getPrinters ");
    DWORD printers_size = 0;
    DWORD printers_size_bytes = 0, dummyBytes = 0;
    DWORD Level = 2;
    DWORD flags = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS;

    // First try to retrieve the number of printers
    BOOL bError = EnumPrintersW(flags, NULL, Level, NULL, 0, &printers_size_bytes, &printers_size);
    
    // Check for errors in the first call
    if (!bError && GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
        RETURN_EXCEPTION_STR("Error retrieving printer size.");
    }

    // Check if we got a valid size
    if (printers_size_bytes == 0) {
        RETURN_EXCEPTION_STR("No printers found or invalid size returned.");
    }

    // Allocate the required memory
    MemValue<PRINTER_INFO_2W> printers(printers_size_bytes);
    if (!printers.get()) // Make sure to check the pointer
    {
        RETURN_EXCEPTION_STR("Error allocating memory for printers in getPrinters.");
    }

    // Second call to retrieve printer information
    bError = EnumPrintersW(flags, NULL, Level, (LPBYTE)(printers.get()), printers_size_bytes, &dummyBytes, &printers_size);
    if (!bError)
    {
        std::string error_str("Error on EnumPrinters: ");
        error_str += getLastErrorCodeAndMessage();
        RETURN_EXCEPTION_STR(error_str.c_str());
    }

    // Prepare result array
    v8::Local<v8::Array> result = V8_VALUE_NEW(Array, printers_size);
    PRINTER_INFO_2W *printer = printers.get();
    DWORD i = 0;
    for (; i < printers_size; ++i, ++printer)
    {
        v8::Local<v8::Object> result_printer = V8_VALUE_NEW_DEFAULT(Object);
        PrinterHandle printerHandle((LPWSTR)(printer->pPrinterName));
        std::string error_str = parsePrinterInfo(printer, result_printer, printerHandle);
        if (!error_str.empty())
        {
            RETURN_EXCEPTION_STR(error_str.c_str());
        }
        Nan::Set(result, i, result_printer);
    }
    
    MY_NODE_MODULE_RETURN_VALUE(result);
}

MY_NODE_MODULE_CALLBACK(getDefaultPrinterName)
{
    MY_NODE_MODULE_HANDLESCOPE;
    
    // Get the required size for the printer name
    DWORD cSize = 0;
    ConsolePrint("Calling GetDefaultPrinterW to get size...");
    
    if (!GetDefaultPrinterW(NULL, &cSize) && GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
        ConsolePrint("Failed to get printer size.");
        MY_NODE_MODULE_RETURN_VALUE(V8_STRING_NEW_UTF8(""));
    }

    ConsolePrint("Printer name size: " + std::to_string(cSize));
    
    // Allocate memory for the printer name
    MemValue<uint16_t> bPrinterName(cSize * sizeof(uint16_t));
    if (!bPrinterName.get()) {
        ConsolePrint("Memory allocation for printer name failed.");
        MY_NODE_MODULE_RETURN_VALUE(V8_STRING_NEW_UTF8(""));
    }

    // Retrieve the printer name
    BOOL res = GetDefaultPrinterW((LPWSTR)(bPrinterName.get()), &cSize);
    if (!res) {
         // If the call failed, you can retrieve the error code
        DWORD error = GetLastError();
        ConsolePrint("Failed to get printer name. Error code: " + std::to_string(error));
        MY_NODE_MODULE_RETURN_VALUE(V8_STRING_NEW_UTF8(""));
    }

    ConsolePrint("Successfully retrieved printer name.");

    // Return the printer name as a V8 string
    MY_NODE_MODULE_RETURN_VALUE(V8_STRING_NEW_2BYTES((uint16_t*)bPrinterName.get()));
}

