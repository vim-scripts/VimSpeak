all : VimSpeak.dll

VimSpeak.dll : VimSpeak.c
	cl /LD VimSpeak.c OLEAUT32.lib OLE32.lib
