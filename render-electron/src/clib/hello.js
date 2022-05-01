const addon = require("bindings")("minirender-addon")

// just test
console.log(addon.hello()); // 'world'

//export const hello = addon.hello;

module.exports = {
  hello: addon.hello,
  loadFile: addon.loadFile,
  render: addon.render
}
