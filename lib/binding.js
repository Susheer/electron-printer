const path = require('path');
const os = require('os');
let addon = {}, binary_path;
switch (os.platform()) {
    case 'win32':
        if (os.arch() === 'ia32') {
            binary_path = path.join(__dirname, 'electron_printer_win32_ia32.node');
        } else if (os.arch() === 'x64') {
            binary_path = path.join(__dirname, 'electron_printer_win32_x64.node');
        }
        addon = require(binary_path);
        break;
    case 'darwin':
        binary_path = path.join(__dirname, 'electron_printer.node'); 
        addon = require(binary_path);
        break;
    case 'linux':
        if (os.arch() === 'ia32') {
            addon = path.join(__dirname, 'electron_printer.node');
        } else if (os.arch() === 'x64') {
            addon = path.join(__dirname, 'electron_printer.node');
        }
        break;
    default:
        binary_path = path.join(__dirname, 'electron_printer.node'); 
        addon = require(binary_path);
}

module.exports.sayMyName = addon.SayMyName
module.exports.getPrinters = addon.getPrinters
module.exports.printDirect = printDirect
module.exports.getDefaultPrinterName = addon.getDefaultPrinterName
module.exports.getPrinter = getPrinter;


/*
 print raw data. This function is intend to be asynchronous

 parameters:
 parameters - Object, parameters objects with the following structure:
 data - String, mandatory, data to printer
 printer - String, optional, name of the printer, if missing, will try to print to default printer
 docname - String, optional, name of document showed in printer status
 type - String, optional, only for wind32, data type, one of the RAW, TEXT
 options - JS object with CUPS options, optional
 success - Function, optional, callback function
 error - Function, optional, callback function if exists any error

 or

 data - String, mandatory, data to printer
 printer - String, optional, name of the printer, if missing, will try to print to default printer
 docname - String, optional, name of document showed in printer status
 type - String, optional, data type, one of the RAW, TEXT
 options - JS object with CUPS options, optional
 success - Function, optional, callback function with first argument job_id
 error - Function, optional, callback function if exists any error
 */
 function printDirect(parameters){
    var data = parameters
        , printer
        , docname
        , type
        , options
        , success
        , error;

    if(arguments.length==1){
        //TODO: check parameters type
        //if (typeof parameters )
        data = parameters.data;
        printer = parameters.printer;
        docname = parameters.docname;
        type = parameters.type;
        options = parameters.options||{};
        success = parameters.success;
        error = parameters.error;
    }else{
        printer = arguments[1];
        type = arguments[2];
        docname = arguments[3];
        options = arguments[4];
        success = arguments[5];
        error = arguments[6];
    }

    if(!type){
        type = "RAW";
    }

    // Set default printer name
    if(!printer) {
        printer = addon.getDefaultPrinterName();
    }

    type = type.toUpperCase();

    if(!docname){
        docname = "node print job";
    }

    if (!options){
        options = {};
    }

    //TODO: check parameters type
    if(addon.printDirect){// call C++ binding
        try{
            var res = addon.printDirect(data, printer, docname, type, options);
            if(res){
                success(res);
            }else{
                error(Error("Something wrong in printDirect"));
            }
        }catch (e){
            error(e);
        }
    }else{
        error("Not supported");
    }
}

/** Get printer info with jobs
 * @param printerName printer name to extract the info
 * @return printer object info:
 *		TODO: to enum all possible attributes
 */
 function getPrinter(printerName)
 {
     if(!printerName) {
         printerName = addon.getDefaultPrinterName();
     }
     var printer = addon.getPrinter(printerName);
     correctPrinterinfo(printer);
     return printer;
 }


function correctPrinterinfo(printer) {
    if(printer.status || !printer.options || !printer.options['printer-state']){
        return;
    }

    var status = printer.options['printer-state'];
    // Add posix status
    if(status == '3'){
        status = 'IDLE'
    }
    else if(status == '4'){
        status = 'PRINTING'
    }
    else if(status == '5'){
        status = 'STOPPED'
    }

    // correct date type
    var k;
    for(k in printer.options) {
        if(/time$/.test(k) && printer.options[k] && !(printer.options[k] instanceof Date)) {
            printer.options[k] = new Date(printer.options[k] * 1000);
        }
    }

    printer.status = status;
}