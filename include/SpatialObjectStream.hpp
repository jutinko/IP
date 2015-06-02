#ifndef SPATIAL_OBJECT_STREAM_HPP_
#define SPATIAL_OBJECT_STREAM_HPP_

#include "Box.hpp"

namespace FLAT
{
	class SpatialObjectStream
	{
	public:
		SpatialObjectType objectType;
		uint64 objectCount;
		uint32 objectByteSize;
		Box universe;

		SpatialObjectStream();
		virtual ~SpatialObjectStream();

		virtual bool hasNext()=0;
		virtual SpatialObject* getNext()=0;
		virtual void rewind()=0;
	};
}


#endif
