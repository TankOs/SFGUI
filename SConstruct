import os

# Options.
opts = Variables( "custom.py" )

opts.Add( BoolVariable( "debug", "Set to 1 to build with debug symbols and without optimization.", 1 ) )
#opts.Add( PathVariable( "prefix", "Installation directory.", "/usr/local/" ) )
opts.Add( BoolVariable( "samples", "Set to 1 to build samples.", 1 ) )
opts.Add( BoolVariable( "codeblocks", "Set to 1 to generate Code::Blocks project file, disables normal build.", 0 ) )
opts.Add( "CXX", "Compiler to use." )
#opts.Add( BoolVariable( "nocpp0x", "Don't use (experimental) C++0x support..", 0 ) )

# Base environment with help.
confenv = Environment( variables = opts, ENV = {"PATH": os.environ["PATH"], "TERM": os.environ["TERM"], "HOME": os.environ["HOME"]} )
Help( opts.GenerateHelpText( confenv ) )

# Default properties.
cflags   = "-fno-strict-aliasing -Wall -Wextra -Werror -Wconversion -Wfatal-errors -std=c++0x"
sfmllibs = ["sfml-system", "sfml-window", "sfml-graphics", "sfml-audio"]

#cflags += " -std=c++0x -pedantic -pedantic-errors"

if confenv["debug"] != 0:
	cflags += " -g"
else:
	cflags += " -O4 -Os -s"


confenv.Replace( CPPFLAGS = cflags, LIBS = sfmllibs )

Export( "confenv" )

# Build yaml-cpp.
#SConscript( "ext/yaml-cpp-0.2.2/SConscript" )

# Custom build directory.
VariantDir( "obj", "src", duplicate = 0 )
SConscript( "obj/SConscript" )

# Samples.
if confenv["samples"] != 0 and confenv["codeblocks"] == 0:
	SConscript( "samples/SConscript" )
