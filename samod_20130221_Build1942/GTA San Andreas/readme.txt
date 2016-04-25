if you need to load another d3d9.dll name it "proxyloader_d3d9.dll" and Pawn4Sp will load it.

any other DLL's you want to load need to be placed in \libraries\ (default path)
All the DLL's you want to load need to be in samod2_config.ini on lines:

AMX Libraries To Load=
DLL Libraries To Load=

for >>example<< there are 3 DLLs:

sound.dll
extra.dll
hello.dll

and 2 AMX plugins:

RouteConnector.dll
DriftPointsCounter.dll

and you only want to load extra.dll,hello.dll and DriftPointsConter.dll, then you fill in:

AMX Libraries To Load=DriftPointsCounter.dll
DLL Libraries To Load=extra.dll hello.dll

And that's it! All DLL's on one line separated by spaces.

In \pawno\ you have the script editor and compiler (and all include files).

all amx plugins goto \libraries\pluginsa\plugins (default directory) 

In \libraries\pluginsa\scriptfiles\ (default) are all the files created/edited by .amx scripts with file functions.

In \libraries\pluginsa\scripts\ (default) there are scripts. 

Those scripts can be loaded by using functions from plugins 
or loading from other scripts or specifying it in the "AMX Scripts To Load" line in "samod2_config.ini"

If you want to load multiple scripts, put the filenames on one line in samod2_config.ini
like:

AMX Scripts To Load=plugin.amx script2.amx some_other_script.amx

To access the developer console hit 'CTRL + ALT + F10' and type:

help

and hit enter for more information.