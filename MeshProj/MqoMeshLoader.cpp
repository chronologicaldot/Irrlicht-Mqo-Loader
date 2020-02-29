/*
Mqo Mesh Loader
(c) 2011, Nic Anderson

This is free and opensource software.
This software is provided AS IS without any guarantee that it will function.

The program is supposed to load .mqo files.
*/

#pragma once

#include "MqoMeshLoader.h"

using namespace irr;
using namespace scene;

#include "MqoMesh.h"

// Using Linus Listix (linked list)
#include "LinusListix.h"
using namespace Linus;


bool MqoMeshLoader::isALoadableFileExtension(const io::path& filename) const
{
	// mqo files can end in either ".mqo" or ".txt"
	return core::hasFileExtension ( filename, "mqo" );
}


IAnimatedMesh* MqoMeshLoader::createMesh(io::IReadFile* file)
{
	// Have the manager make a new scene mesh to be added to (?)

	// mesh needed for loading into
	MqoMesh* mesh = new MqoMesh();

	if (mesh)
	{
		if (loadFile(file, (MqoMesh*)mesh) )
			return mesh;

		mesh->drop();
	}

	return 0;
}


bool MqoMeshLoader::loadFile( io::IReadFile* file, MqoMesh* mesh )
{
	if (!file) // don't bother loading if the file does not exist
		return false;

	// TODO
}
