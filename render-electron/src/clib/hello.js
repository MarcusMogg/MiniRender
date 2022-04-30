const addon = require("../../build/Release/hello.node");

console.log(addon.hello()); // 'world'

//export const hello = addon.hello;

module.exports = {
  hello: addon.hello,
}
