const assert = require("node:assert");
const test = require("node:test");

test("module loads successfully", () => {
    const printer = require("../");

    assert.ok(printer);
});

test("printDirect is exported", () => {
    const printer = require("../");

    assert.equal(typeof printer.printDirect, "function");
});

test("getPrinters is exported", () => {
    const printer = require("../");

    assert.equal(typeof printer.getPrinters, "function");
});

test("getPrinter is exported", () => {
    const printer = require("../");

    assert.equal(typeof printer.getPrinter, "function");
});

test("getDefaultPrinterName is exported", () => {
    const printer = require("../");

    assert.equal(typeof printer.getDefaultPrinterName, "function");
});

test("printFile is exported", () => {
    const printer = require("../");

    assert.equal(typeof printer.printFile, "function");
});

test("getSupportedPrintFormats is exported", () => {
    const printer = require("../");

    assert.equal(typeof printer.getSupportedPrintFormats, "function");
});