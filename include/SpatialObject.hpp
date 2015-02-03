#ifndef SPATIAL_OBJECT_HPP
#define SPATIAL_OBJECT_HPP

#include "SpatialObjectFactory.hpp"

namespace FLAT
{
class Box;      // To avoid Circular Includes
class Vertex;   // To avoid Circular Includes

	/*
	 * Abstract Base Class for Indexing Spatial Objects
	 * Objects other than 3D objects have not been tested
	 */
	class SpatialObject
	{
	public:
		SpatialObject();

		virtual ~SpatialObject();

		virtual Box getMBR()=0;

		virtual Vertex getCenter()=0;

		// For efficient sorting
		virtual spaceUnit getSortDimension(int dimension)=0;

		// Filter if object is inside the region . true means yes false means no
		virtual bool IsResult(Box& region)=0;

		virtual void serialize(int8* buffer)=0;

		virtual void unserialize(int8* buffer)=0;

		virtual uint32 getSize()=0;

		virtual SpatialObjectType getType()=0;

		virtual bigSpaceUnit pointDistance(Vertex& p)=0;
	};

	struct SpatialObjectClosestPoint : public std::binary_function<SpatialObject* const, SpatialObject* const, bool>
	{
		bool operator()(SpatialObject* const r1, SpatialObject* const r2);
	};

	struct SpatialObjectXAsc : public std::binary_function<SpatialObject* const, SpatialObject* const, bool>
	{
		bool operator()(SpatialObject* const r1, SpatialObject* const r2);
	};

	struct SpatialObjectYAsc : public std::binary_function<SpatialObject* const, SpatialObject* const, bool>
	{
		bool operator()(SpatialObject* const r1, SpatialObject* const r2);
	};

	struct SpatialObjectZAsc : public std::binary_function<SpatialObject* const, SpatialObject* const, bool>
	{
		bool operator()(SpatialObject* const r1, SpatialObject* const r2);
	};

}

#endif
