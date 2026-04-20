import { Component } from "singularity"

globalThis.result = Component({ foo: 123 })
console.log(globalThis.result);

console.log("Hello from javascript")