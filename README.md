# @thesusheer/electron-printer

**No recompilation required when upgrading Node.js versions, thanks to N-API!** 🎉

Native bind printers on POSIX and Windows OS from Node.js, Electron, and node-webkit.

## Runtime Compatibility

This package distributes N-API binaries.

The same binary can be used across supported versions of:

- Node.js
- Electron
- NW.js

without recompilation, provided the runtime supports the targeted N-API version.

> Supports Node.js versions from 8.0.0 onwards, including the latest versions, thanks to the transition to N-API.

If you have a problem, ask a question on !Gitter or find/create a new Github issue

___
### **Below is the original README**
___
### Reason:

I was involved in a project where I needed to print from Node.js. This is the reason why I created this project and I want to share my code with others.

### Features:

* No dependencies on NAN or V8;
* Native method wrappers for Windows and POSIX (which uses CUPS 1.4/MAC OS X 10.6) APIs;
* Compatible with Node.js versions that support N-API, ensuring long-term stability and compatibility;
* Compatible with Electron versions that support N-API, reducing the need for frequent recompilation;
* `getPrinters()` to enumerate all installed printers with current jobs and statuses;
* `getPrinter(printerName)` to get a specific/default printer info with current jobs and statuses;
* `getPrinterDriverOptions(printerName)` (POSIX only) to get a specific/default printer driver options such as supported paper size and other info;
* `getSelectedPaperSize(printerName)` (POSIX only) to get a specific/default printer default paper size from its driver options;
* `getDefaultPrinterName()` returns the default printer name;
* `printDirect(options)` to send a job to a specific/default printer, now supports CUPS options passed in the form of a JS object (see `cancelJob.js` example). To print a PDF from Windows, it is possible by using node-pdfium module to convert a PDF format into EMF and then send it to the printer as EMF;
* `printFile(options)` (POSIX only) to print a file;
* `getSupportedPrintFormats()` to get all possible print formats for the `printDirect` method, which depends on the OS. `RAW` and `TEXT` are supported on all OSes;
* `getJob(printerName, jobId)` to get specific job info including job status;
* `setJob(printerName, jobId, command)` to send a command to a job (e.g., `'CANCEL'` to cancel the job);
* `getSupportedJobCommands()` to get supported job commands for `setJob()`, depending on the OS. The `'CANCEL'` command is supported on all OSes.

### How to install:
```
npm install @thesusheer/electron-printer
```
### Distribution Change Notice
Starting from this version, the npm package distributes only runtime JavaScript
files and prebuilt binaries. Native C++ source files, build scripts, and other
development-only artifacts are intentionally excluded to ensure compatibility
with enterprise security scanners and clean production deployments.

If you need to modify native code, please use the GitHub repository instead of
node_modules.

### How to use:

See examples

### Author(s):

* Ion Lupascu, ionlupascu@gmail.com

### Contributors:

* Thiago Lugli, @thiagoelg
* Eko Eryanto, @ekoeryanto
* Sudheer Gupta, @susheer

### Project History:

This project was originally written by Ion Lupascu using NAN and V8. It has been rewritten by @susheer to use N-API exclusively, removing dependencies on NAN and V8 to ensure better stability and compatibility with future Node.js and Electron versions.

### Node.js Version Support:

This project supports Node.js versions from 8.0.0 onwards. N-API ensures that native addons do not require recompilation when upgrading Node.js versions. For more details, refer to the [Node.js N-API documentation](https://nodejs.org/api/n-api.html)¹(https://nodejs.org/api/n-api.html).

Feel free to download, test, and propose new features.

### License:
 The MIT License (MIT)

## Contributing

Contributions are welcome! Please open an issue or submit a pull request on GitHub.

## License

This project is licensed under the MIT License.

### Keywords:

* node-printer
* printer
* electron-printer
* node-addon-api
* POSIX printer
* Windows printer
* CUPS printer
* print job
* printer driver

---