# -*- coding: utf-8 -*-
import os

def generate_codeblocks_project( target, source, env ):
	header_template = """
		<Unit filename="%s">
			<Option compileVar=""/>
			<Option compile="0"/>
			<Option link="0"/>
			<Option target="Debug"/>
			<Option target="Release"/>
		</Unit>"""
	
	source_template = """
		<Unit filename="%s">
			<Option compileVar="CPP"/>
			<Option target="Debug"/>
			<Option target="Release"/>
		</Unit>"""
	
	xmlstr = []
	rel_path = None if "cb_source_path_prefix" not in env else env["cb_source_path_prefix"]

	for f in source:
		filename = str( f )
		
		if rel_path is not None:
			filename = os.path.join( rel_path, filename )

		if filename.endswith( ".hpp" ) or filename.endswith( ".inl" ):
			xmlstr.append( header_template % filename )
		else:
			xmlstr.append( source_template % filename )

	output = open( env["cb_template_filename"], "r" ).read()
	output = output.replace( "__FILES__", "\n".join( xmlstr ) )
	open( str( target[0] ), "w" ).write( output )

	print "Code::Blocks project generated successfully."
