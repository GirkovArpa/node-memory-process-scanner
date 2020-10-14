'use strict';

const { scan } = require('bindings')('/release/addon');

console.log(scan('* Untitled - Notepad3', '(@@@[a-z ]{5,}@@@)'));