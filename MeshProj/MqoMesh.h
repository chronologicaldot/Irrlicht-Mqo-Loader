/*
Mqo Mesh class
(c) 2011, Nic Anderson

This is free and opensource software.
This software is provided AS IS without any guarantee that it will function.
*/

// Using Linus Listix (linked list)
#include <irrlicht.h>
#include "LinusListix.h"

#pragma once

using namespace Linus;

enum EMqoMirrorType
{
	EMqoMT_NONE =0,		// no mirroring
	EMqoMT_SEPARATE,	// mirroring, but mirrored vertices are separated
	EMqoMT_JOINED		// mirroring, but mirrored vertices are joined with faces
};

enum EMqoMirrorAxis
{
	EMqoMA_FAIL=0,		// no mirror axis
	EMqoMA_X,			// x-axis only
	EMqoMA_Y,			// y-axis only
	EMqoMA_XY,			// x and y-axis
	EMqoMA_Z,			// z-axis only
	EMqoMA_XZ,			// x and z-axis
	EMqoMA_YZ,			// y and z-axis
	EMqoMA_ALL,			// all axis
};

enum EMqoSubdivision
{
	EMqoSD_NONE=0,		// no sub-divisioning
	EMqoSD_SPLINE1,		// use Spline-type 1 algorithm for sub-divisioning
	EMqoSD_SPLINE2,		// use Spline-type 2 algorithm for sub-divisioning
	EMqoSD_CATMULL,		// use Catmull-Clark algorithm for sub-divisioning
};

//// class for material information
//class BasicMaterial
//{
//public:
//	// name of the material
//	irr::core::stringc name;
//
//	// plain color info
//	// 0 = transparency/alpha, 1 = red, 2 = green, 3 = blue
//	irr::core::array<irr::u32> color(3);
//
//	// ambient color
//	// 0 = red, 1 = green, 2 = blue
//	irr::core::array<irr::f32> amb(3);
//
//	// diffuse color
//	irr::f32 dif;
//
//	// specular color
//	irr::f32 spec;
//};

// class for face information
class MqoFace
{
	irr::core::array<irr::s32> v; // v[4]
	// v[4] = -1 if only three vertices

public:

	MqoFace()
	{
		v.reallocate(4);
		v[0] = -1;
		v[1] = -1;
		v[2] = -1;
		v[3] = -1;
	}

	// if there is a fourth vertex
	bool v4th;

	// (note: .mqo begins listing with 0)
	irr::s32 group; // the group that this face belongs to
	irr::s32 mat; // which material is applied to this face

	void set( irr::s32 which, irr::s32 new_vertex )
	{
		v[which] = new_vertex;
	}

	void swap(s32 vertex1, s32 vertex2) {
		irr::u16 orig = 0;
		
		orig = v[vertex1];
		v[vertex1] = v[vertex2];
		v[vertex2] = orig;
	}
};


class MqoMesh
{
private:
	LListix<LListix<irr::video::S3DVertex>>* vertices;
	LListix<MqoFace>* faces;
	LListix<irr::video::SMaterial>* materials;
};