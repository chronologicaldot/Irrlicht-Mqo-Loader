/*
Mqo Mesh Loader
(c) 2011, Nic Anderson

This is free and opensource software.
This software is provided AS IS without any guarantee that it will function.

The code is supposed to load .mqo files.
*/

#include "iMeshLoader.h"
#include "MqoMeshLoadInterface.h"

#pragma once

using namespace irr;
using namespace scene;

// Using Linus Listix (linked list)
//#include "LinusListix.h"
//using namespace Linus;

class MqoMeshLoader : public IMeshLoader, public MqoMeshLoadInterface
{
public:
	//! Constructor
	MqoMeshLoader( scene::ISceneManager* manager );

	//! returns true if the file can be loaded by this class
	//! based on file extension
	virtual bool isALoadableFileExtension( const io::path& filename ) const;

	//! creates/loads an animated mesh from the file.
	//! \return Pointer to the created mesh. Returns 0 if loading failed.
	virtual IAnimatedMesh* createMesh( io::IReadFile* file );

private:
	//! loads the file data into the mesh
	bool loadFile(io::IReadFile* file, MqoMesh* mesh );
};
