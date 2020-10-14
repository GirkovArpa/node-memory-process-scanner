# Node Process Memory Grep

Scan the memory of a process (identified by its window title) for the first string matching the supplied regex.

**Windows only.**

## Installation

```
$ npm i node-process-memory-grep
```

## Usage

```javascript
const scan = require('node-process-memory-grep');
const firstMatch = scan('Notepad', '(foo[a-z ]+baz)'); // must be enclosed in parentheses
console.log(firstMatch); // => foo bar baz (if your Notepad window contained this string)
```