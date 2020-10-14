'use strict';

const { scan } = require('bindings')('/release/addon');

console.log(scan());