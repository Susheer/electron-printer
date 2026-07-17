const assert = require("assert/strict");
const test = require("node:test");

const addon = require("../lib");

test("getPrinters exists", () => {
    assert.equal(typeof addon.getPrinters, "function");
});

test("getPrinters returns array", () => {
    const printers = addon.getPrinters();

    assert.ok(Array.isArray(printers));

    printers.forEach(printer => {
        assert.equal(typeof printer, "object");
    });
});