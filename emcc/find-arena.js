const Module = require('../build/emcc/find-arena-size');

let initialized = false;
Module.onRuntimeInitialized = function() {
    initialized = true;
};

class FindArenaSize {
    _initialized = false;

    constructor() {

    }

    init() {
        if (initialized === true) return Promise.resolve();

        return new Promise((resolve) => {
            Module.onRuntimeInitialized = () => {
                resolve();
                initialized = true;
            };
        });
    }

    findArenaSize(arenaBuffer, low, high) {
        if (!initialized) throw new Error('Module is not initialized');

        const obj = this._arrayToHeap(arenaBuffer);

        let ret = Module.find_arena_size(obj.byteOffset, low, high);

        Module._free(obj);

        if (ret < 0) {
            throw new Error('Find arena size failed (err code: ' + ret + ')');
        }

        return ret;
    }

    _arrayToHeap(data) {
        let typedArray = new Uint8Array(data);
        let numBytes = typedArray.length * typedArray.BYTES_PER_ELEMENT;
        let ptr = Module._malloc(numBytes);
        let heapBytes = new Uint8Array(Module.HEAPU8.buffer, ptr, numBytes);
        heapBytes.set(new Uint8Array(typedArray.buffer));
        return heapBytes;
    }
}

module.exports = FindArenaSize;
