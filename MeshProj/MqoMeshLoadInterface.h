/*
(c) 2013 Nicolaus Anderson
License: zlib
*/

#include <irrlicht.h>
#include <InputStream.h>
#include "MqoMesh.h"

#ifndef __MQO_MESH_LOAD_INTERFACE_H__
#define __MQO_MESH_LOAD_INTERFACE_H__

namespace irr
{
namespace scene
{

//! Class MqoMeshLoadInterface
/*
This class is designed for handling the loading of mqo (Metasequoia files).
Most of it's functions are used internally, but need to be implemented.
*/
class MqoMeshLoadInterface
{
public:
	// ***************** IMPLEMENTED IN CPP FILE *****************

		//! Load
	/* Use a source-independent feed to send this function the strings
	of data.
	\param port - The stream of data from which the model data will be
	read. */
	void load( InputStream* port );

protected:

		//! Get string name
	/* Uses the port to acquire a string given between two quotation marks.
	\param port - The stream of data boing read from.
	\param namestr - String within which the name will be stored.
	\param term_token - Terminating token - Upon finding this token, the
	function will be halted. This should only be used to indicate an error in
	the stream because it is a token that is not supposed to be encountered.
	\return - true if successful, false if an error. */
	bool getStringName(
		InputStream* port,
		irr::core::stringc& namestr,
		irr::c8* term_token
		);

		//! Acquire materials from the stream
	/* Attempts to acquire the materials from the stream.
	Note that models may or may not have materials, therefore it is
	irrelevant whether this is successful or not.
	Boolean return kept for debugging purposes.
	\return "true" if successful. */
	bool acquireMaterials( InputStream* port );

		//! Acquire Objects/Groups
	/* Attempts to acquire the objects/groups from the stream.
	Note that models may or may not have objects, therefore it is
	irrelevant whether this is successful or not.
	Boolean return kept for debugging purposes.
	\return "true" if successful. */
	bool acquireObjects( InputStream* port );

		//! Acquire Vertices
	/* Attempts to acquire the vertices from the stream.
	Note that models may or may not have vertices, therefore it is
	irrelevant whether this is successful or not.
	Boolean return kept for debugging purposes.
	\param object - The object/group to which these vertices are to be added.
	\return "true" if successful. */
	bool acquireVertices( InputStream* port, irr::s32 object );

		//! Acquire Faces
	/* Attempts to acquire the faces from the stream.
	Note that models may or may not have faces, therefore it is
	irrelevant whether this is successful or not.
	Boolean return kept for debugging purposes.
	\param object - The object/group to which these faces are to be added.
	\return "true" if successful. */
	bool acquireFaces( InputStream* port, irr::s32 object );


	// **************** NEED TO BE IMPLEMENTED ***************

public:

			//! Get Metasequia file version
	/* Returns the identified version of the file - i.e. how the file
	was loaded. This may not correspond to the actual file version.
	A discrepancy will occur if the original file has been corrupted. */
	virtual irr::f32 getMqoVersion();

protected:

		//! Set Metasequoia file version
	/* Sets the version for the Metasequoia file. As of now, there are only
	two versions of the file, both of which are identical in their basic
	features (though the latter may add some functionality). */
	virtual void setMqoVersion( irr::f32 version_number )=0;

		//! Add an object/group to the mesh
	/* Metasequoia creates groups of vertices and faces it calls "objects".
	These correspond to mesh buffers in irrlicht.
	\param name - The name of the object/group.
	\return - The index of the object. */
	virtual irr::s32 addObject( irr::core::stringc name )=0;

		//! Set working object
	/* Sets the current working object. Thencefore, any vertex or face added
	without a specific reference to an object will be added to this object. */
	virtual void setWorkingObject( irr::s32 obj )=0;

		//! Allocate space for vertices
	/* Allocates memory space for the vertices to be added to the mesh.
	Please note that future versions of Metasequoia files may not provide
	this information, and thus this function should be used merely as a
	convenience for time saving rather than a necessity. */
	virtual void allocateForVertices( irr::u32 vertex_count )=0;

		//! Add vertex
	/* Adds a vertex to the given object. If the index is -1, it will be added
	to the current working object. */
	virtual void addVertex( irr::core::vector3df vert, irr::s32 group=-1 )=0;

		//! Allocate space for faces
	/* Allocates memory space for the faces to be added to the mesh.
	Please note that future versions of Metasequoia files may not provide
	this information, and thus this function should be used merely as a
	convenience for time saving rather than a necessity. */
	virtual void allocateForFaces( irr::u32 face_count )=0;

		//! Add face
	/* Adds a face to the given object. If the index is -1, it will be added
	to the current working object. */
	virtual void addFace( MqoFace face, irr::s32 group=-1 )=0;

		//! Allocate space for materials
	/* Allocates memory space for the materials to be added to the mesh.
	Please note that future versions of Metasequoia files may not provide
	this information, and thus this function should be used merely as a
	convenience for time saving rather than a necessity. */
	virtual void allocateForMaterials( irr::u32 material_count )=0;

		//! Add material
	/* Adds a material to the list of available materials.
	\param mat - The material to be added.
	\param name - The name of the material. */
	virtual void addMaterial( irr::video::SMaterial mat, irr::core::stringc name="" )=0;

		//! Set Mirroring Type
	/* Sets the mirroring type of the given object. This indicates if
	the object will be mirrored, and if so, if mirrored vertices will be
	joined or separated.
	If the index is -1, this will effect the current working object. */
	virtual void setMirrorType( EMqoMirrorType type, irr::s32 group=-1 )=0;

		//! Set Mirroring Axis
	/* Sets the axis over which the given object will be mirrored.
	If the index is -1, this will effect the current working object. */
	virtual void setMirrorAxis( EMqoMirrorAxis axis, irr::s32 group=-1 )=0;

		//! Set sub-divisioning
	/* Sets the sub-divisioning algorithm for the model. */
	virtual void setSubDivisioning( EMqoSubdivision algorithm )=0;
};

} // end namespace scene
} // end namespace irr

#endif // #ifndef __MQO_MESH_LOAD_INTERFACE_H__