const assert = require("assert/strict");
const test = require("node:test");

const addon = require("../lib");

test("sayMyName exists", () => {
    assert.equal(typeof addon.sayMyName, "function");
});

test("sayMyName returns expected value", () => {
    assert.equal(addon.sayMyName(), "Hello, From C++ !");
});