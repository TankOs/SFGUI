# Options.
opts = Variables( "custom.py" )

opts.Add( BoolVariable( "debug", "Set to 1 to build with debug symbols and without optimization.", 1 ) )
#opts.Add( BoolVariable( "samples", "Set to 1 to build samples.", 1 ) )
#opts.Add( BoolVariable( "nocpp0x", "Don't use (experimental) C++0x support..", 0 ) )
opts.Add( PathVariable( "prefix", "Installation directory.", "/usr/local/" ) )

# Base environment with help.
confenv = Environment( variables = opts )
Help( opts.GenerateHelpText( confenv ) )

# Default properties.
cflags   = "-fno-strict-aliasing -Wall -Wextra -Werror -Wconversion -Wfatal-errors"
sfmllibs = ["sfml2-system", "sfml2-window", "sfml2-graphics", "sfml2-audio"]

#if confenv["nocpp0x"] == 0:
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
#if confenv["samples"] != 0:
#SConscript( "samples/SConscript" )
