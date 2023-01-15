function DeRef(Memory, Ptr, Size)
{
    let Result = Memory.subarray(Ptr, Ptr + Size);
    return Result;
}

async function Start()
{
    var MemoryBuffer = undefined;
    var MemoryView   = undefined;
    
    const UTF8Decoder = new TextDecoder("utf-8");
    
    const WASM = await WebAssembly.instantiateStreaming(
	fetch("./main.wasm"),
	{
	    "env":
	    {
		"printw": (StackPtr, Bytes) =>
		{
		    let stringBytes = DeRef(MemoryBuffer, StackPtr, Bytes);
		    console.log(UTF8Decoder.decode(stringBytes));
		},
		"GetMaxMemory": () =>
		{
		    let MaxBytes = MemoryBuffer.length;
		    return MaxBytes;
		},
		"GrowMemory": (RequestedBytes) =>
		{
		    let NewBytes  = RequestedBytes;
		    let PageSize  = 64 * 1024;
		    let MorePages = Math.ceil(NewBytes / PageSize);

		    try
		    {
			MemoryView.grow(MorePages);
		    }
		    catch(e)
		    {
			console.error("Failed to grow memory with error: ", e);
		    }

		    MemoryBuffer = new Uint8Array(WASM.instance.exports.memory.buffer); //NOTE: calling grow invalidates MemoryBuffer
		    return(MemoryBuffer.length);
		}
	    },
	});

    MemoryView = WASM.instance.exports.memory;
    MemoryBuffer = new Uint8Array(WASM.instance.exports.memory.buffer);
    WASM.instance.exports.InitMemory(MemoryBuffer.length);
    
    console.log(WASM.instance.exports.TestFunction(2, 3));
}

Start().catch((Error) => console.error(Error));
