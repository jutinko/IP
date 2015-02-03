#ifndef SPATIAL_OBJECT_FACTORY_HPP
#define SPATIAL_OBJECT_FACTORY_HPP

#include "GlobalCommon.hpp"
#include <string>

namespace FLAT
{
	class SpatialObject;  //Avoid Circular Includes
	/*
	 * 3D Spatial Objects Types
	 */
	enum SpatialObjectType
	{
		VERTEX,
		BOX,
		CONE,
		TRIANGLE,
		SPHERE,
		SEGMENT,
		MESH,
		SOMA,
		SYNAPSE,
		NONE
		//TETRAHEDRA,
	};

	/*
	 * Class for Creating Spatial Objects
	 */
	class SpatialObjectFactory
	{
	public:
		 static SpatialObject* create (SpatialObjectType objType);
		 static uint32 getSize(SpatialObjectType objType);
		 static std::string getTitle (SpatialObjectType objType);
	};
}

#endif
