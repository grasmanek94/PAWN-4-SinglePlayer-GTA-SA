if you need to load another d3d9.dll name it "proxyloader_d3d9.dll" and Pawn4Sp will load it.

any other DLL's you want to load need to be placed in \libraries\
All the DLL's you want to load need to be in \libraries\plugins.ini,

for >>example<< there are 3 files:

sound.dll
extra.dll
hello.dll

and you only want to load extra.dll and hello.dll

then plugins.ini would containt this:

extra.dll hello.dll

And that's it! All DLL's on one line separated by spaces.

In \pawno\ you have the script editor and compiler (and all include files).

-\libraries\pluginsa\ is the pawn scripts directory, all amx plugins goto \libraries\pluginsa\plugins 
and to load them you also place all dll filenames on one line in amx_plugins.ini.

In \liraries\pluginsa\scriptfiles\ are all the files created/eited by .amx scripts with file functions.

In \libraries\pluginsa\scripts\ there are scripts. 
Those scripts can be loaded by using functions from plugins 
or loading from other scripts or specifying a script in amx_scripts.ini

If you want to load multiple scripts, put the filenames on one line in amx_scripts.ini