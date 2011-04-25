libpath = /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib" /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib" /LIBPATH:"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86"
inc = /I"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include" /I"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\INCLUDE" /I"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\include"
cl = cl
link = link
clflags = /nologo /EHsc /c /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /Zc:wchar_t /W3 /O2 /Oi $(inc)
linkflags = /RELEASE /SUBSYSTEM:CONSOLE /NOLOGO /INCREMENTAL:NO /MACHINE:X86 $(libpath)
libs = kernel32.lib user32.lib gdi32.lib d3d9.lib d3dx9.lib dinput8.lib dxguid.lib ws2_32.lib
objs = main.obj game.obj text.obj input.obj graphics.obj debug.obj

all: mmclient.exe

clean:
	del $(objs) mmclient.exe

.cpp.obj:
	$(cl) $(clflags) $*.cpp

mmclient.exe: $(objs)
	$(link) $(linkflags) /OUT:mmclient.exe $(libs) $** 
