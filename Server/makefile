libpath = /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib" /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib" /LIBPATH:"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x86"
inc = /I"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include" /I"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\INCLUDE" /I"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\include"
cl = cl
link = link
clflags = /nologo /EHsc /c /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /Zc:wchar_t /W3 /O2 /Oi $(inc)
linkflags = /RELEASE /SUBSYSTEM:CONSOLE /NOLOGO /INCREMENTAL:NO /MACHINE:X86 $(libpath)
libs = kernel32.lib user32.lib ws2_32.lib winmm.lib
objs = main.obj game.obj debug.obj
target=mmserver.exe

all: $(target)

clean:
	del $(objs) $(target)

.cpp.obj:
	$(cl) $(clflags) $*.cpp

$(target): $(objs)
	$(link) $(linkflags) /OUT:$(target) $(libs) $** 
