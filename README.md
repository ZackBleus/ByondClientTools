# ByondClientTools
Tools for profiling the BYOND Dreamseeker client

Uses code from: [D3D9-Hook-ImGui](https://github.com/adamhlt/D3D9-Hook-ImGui)

Compile, place the compiled .dll "ByondClientTools.dll" by default in your BYOND project directory and call it after your client mob is set using the below code:
```
call_ext("ByondClientTools.dll", "init")()
```
