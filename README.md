# ByondClientTools
Tools for profiling the BYOND Dreamseeker client

Uses code from: [D3D9-Hook-ImGui](https://github.com/adamhlt/D3D9-Hook-ImGui)

Compile, place the compiled .dll "ByondClientTools.dll" by default in your BYOND project directory and call it after your client mob is set using the below code:
```
call_ext("ByondClientTools.dll", "init")()
```

If you need to need to profile a client that is not running the world locally, you must inject the .dll into the DreamSeeker application using a .dll injector such as [This one](https://github.com/adamhlt/DLL-Injector).
