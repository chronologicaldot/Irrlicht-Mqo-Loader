/*
(c) 2013 Nicolaus Anderson
License: zlib
*/

#include "MqoMeshLoadInterface.h"
#include <countBrackets.h>

#ifndef __MQO_MESH_LOAD_INTERFACE_CPP__
#define __MQO_MESH_LOAD_INTERFACE_CPP__

bool irr::scene::MqoMeshLoadInterface::getStringName(
	InputStream* port,
	irr::core::stringc& namestr,
	irr::c8* term_token
	)
{
	irr::core::stringc token;
	namestr = ""; /* clear the string, since it may contain junk. */

	/* Name of object (or a material):
	Three tokens to obtain: quotation marks, name of material, quotation marks. */
		// should be quotation marks
	if ( ! port->next( token ) )
		return false;

	/* Note that materials may be listed at the end of the file rather than
	before object information. */

	if ( token[0] != '\"' ) // Bad token?
		port->movePast( irr::core::stringc( "\"" ) );

	do {
		if ( ! port->next( token ) )
		{
			port->seek(-1);
			return false;
		}

		// separated from above to make it clear "token" has acquired the next token
		if ( token == term_token )
		{
			port->seek(-1);
			return false;
		}

		/* Spaces will be removed from the stream, so we need to add them between
		words. Only begin adding them if there is already an initial word. */
		if ( namestr.size() > 0 )
			namestr.append( ' ' );

		// Add to the name
		namestr.append( token );

	} while ( port->peek() != "\"" );

	return true;
}

void irr::scene::MqoMeshLoadInterface::load( InputStream* port )
{
	// item location in the file
	irr::s32 item_loc;
	irr::core::stringc token;
	irr::core::stringc name;
	irr::core::array<irr::f32> attr;	/* list of attributes (used to store
										attributes acquired from between parentheses) */

	/* Metasequoia file format information can be found in the
	document "Metasequoia file format information.txt".

	The file is divided into four parts:
	1) Header
	2) Scene
	3) Materials
	4) Object
	*/

	//! Header
	/* Contains version information. */
	item_loc = port->find( irr::core::stringc( "Ver" ) );

	if ( item_loc != -1 ) // if no error in finding the header
	{
		port->seek(item_loc, EPOS_BEGIN);
		// Acquire the version number
			/* It is expected to return a single, floating point number. */
		if ( port->next( token ) )
			setMqoVersion( (irr::f32)atof( token.c_str() ) );
	}
	else { // Corrupted file - assume version 1.0
		setMqoVersion( 1.0f );
	}

	//! Scene
	/* This section is skipped because programs will set up their own
	scene. */
	item_loc = port->find( irr::core::stringc( "Scene" ) );
	
	if ( item_loc != -1 ) // if no error in finding the scene settings
	{
		port->seek( item_loc, EPOS_BEGIN );
		util::countBrackets( port, '{', '}', false );
	}

	// else - File contains no (labeled) scene information

	//************************************************
	//! Materials

		// Find the section header
	item_loc = port->find( irr::core::stringc( "Material" ) );

	// If it was found
	if ( item_loc != -1 )
	{
		port->seek( item_loc, EPOS_BEGIN );
		acquireMaterials();
	}


	//************************************************
	//! Object

	// Restart from the beginning
	/* Now we want to acquire all of the objects without regard to where
	the materials were listed in the stream/file. */
	port->seek(0, EPOS_BEGIN);

	do {
		item_loc = port->find(
						"Object",
						false // don't cycle to the beginning
						);

		if ( item_loc == -1 )
			break;

		port->seek( item_loc, EPOS_BEGIN );

		acquireObjects( port );

	} while ( item_loc != -1 );
}

bool irr::scene::MqoMeshLoadInterface::acquireMaterials( InputStream* port )
{
	irr::core::stringc token;
	irr::core::stringc name;

		/* Materials contain 7 attributes or more:
	>> "name" - Name (a string in quotation marks)
	>> col - Color - 4 params (red, green, blue, alpha)
	>> dif - Diffuse coloring - 1 param
	>> amb - Ambient lighting - 1 param
	>> emi - Emissive (glowing) - 1 param
	>> spc - Specular - 1 param
	>> power - Power (pressumably shininess by its characteristics) - 1 param
	>> tex - Texture - (optional) Path to a texture file which is to be applied
	to the object.
	*/

		/* The next parameter should be a material count, but we also
		account for that as missing in case later versions of the format
		exclude it. */
	if ( ! port->next( token ) )
		return false; // could not grab the next token

		// If the next token was not the bracket, it must be the material count
	if ( token[0] != '{' )
	{
		allocateForMaterials( atoi( token.c_str() ) );
		
			// now we seek the bracket
		if ( port->peek() != "{" ) // Corrupted input stream or file
		{
			port->movePast( irr::core::stringc( "{" ) );
		} else {
			 // get the bracket (so that we can skip it)
			if ( ! port->next( token ) )
				return false; // could not grab the next token
		}
	}

		/* Presummably, the current reading location has now passed the first
		bracket. We can now look for material information. */
	irr::video::SMaterial mat;

	while ( port->peek() != "}" )
	{
		// Double-check the token
		if ( checkEOF( token ) )
			break;

		// Save the name
		// (but don't go past the parenthesis)
		if ( !getStringName( port, name, "(" ) )
		{
			/* If the if-statement executes, the name-grabbing function terminated
			ab-normally. Identify the reason. */

				// End of stream/file?
			if ( port->EOS() )
			{
				return false;
			} else if ( port->peek() == "(" )
			{
				// Minor error - most likely a forgotten quotation mark
				/* In addition to the step back in getStringName(), we step
				back once more in order to get the attribute name. */
				port->seek(-1);
			} else
				break;
		}

MAT_ATTR_LOOP:	/* A label for "goto". Horrible c++ practice, I know.
				We want every attribute of the material information, but we
				don't necessarily know the order in which they will be provided,
				so we search in a manner that doesn't care: everytime we find
				an attribute, we save it and start over. Note that we cannot
				use a while loop because there is no terminator (end-lines are
				dumped from the stream) or for-loops because any number of
				attributes might be missing. */

		// Obtain color information
			// color attribute name
		if ( ! port->next( token ) )
			return false;

		if ( token == "col" )
		{
			if ( port->peek() != "(" )
			{
				if ( port->EOS() )
					return false;
			} else {
				port->seek(1); // skip the parenthesis
			}

			for ( irr::u16 cv=0; cv < 4; cv++ )
			{
				if ( ! port->next( token ) )
					break;

				// TODO
			}

			goto MAT_ATTR_LOOP;

		} else if ( token == "amb" )
		{
			// TODO
		}


		// Save the material
		addMaterial( mat, name );
	}
}

bool irr::scene::MqoMeshLoadInterface::acquireObjects( InputStream* port )
{
	// TODO
}

#endif // #ifndef __MQO_MESH_LOAD_INTERFACE_CPP__
