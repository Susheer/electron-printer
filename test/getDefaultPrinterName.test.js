const assert = require("assert/strict");
const test = require("node:test");

const addon = require("../lib");

test("getDefaultPrinterName exists", () => {
    assert.equal(typeof addon.getDefaultPrinterName, "function");
});

test("getDefaultPrinterName returns string", () => {
    assert.equal(typeof addon.getDefaultPrinterName(), "string");
});