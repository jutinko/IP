#ifndef SEGMENT_HPP_
#define SEGMENT_HPP_

#include <math.h>
#include "SpatialObject.hpp"
#include "Box.hpp"

namespace FLAT {

#define DISCRETIZE_RESOLUTION 10
	/*
	 * Segment Structure
	 * and Neuron Model Atom
	 *
	 * Author: Farhan Tauheed
	 */
	class Segment:public SpatialObject
	{
	public:
		Vertex begin;
		Vertex end;
		spaceUnit radiusBegin;
		spaceUnit radiusEnd;
		uint32 neuronId;
		uint32 sectionId;
		uint32 segmentId;

		Segment()
		{
		}
		Segment(const Vertex& b,const Vertex& e,const spaceUnit& r1, const spaceUnit& r2,uint32 nId,uint32 secId,uint32 segId)
		{
			begin = b;
			end   = e;
			radiusBegin = r1;
			radiusEnd   = r2;
			neuronId = nId;
			sectionId = secId;
			segmentId = segId;
		}

		Segment 		 & operator  =(const Segment& rhs);
		bool 		   operator ==(const Segment& rhs) const;
		friend std::ostream & operator << (std::ostream & lhs, const Segment& rhs);

		static void center(const Segment &c,Vertex &v);
		static spaceUnit length(const Segment &c);
		static bigSpaceUnit volume(const Segment &c);
		static void boundingBox(const Segment &c,Box &box);
		static bool overlap(const Segment &c, const Box &b);
		static void discretize(const Segment &c,Vertex vertices[]);

		// Spatial Object virtual functions
		Box getMBR();
		Vertex getCenter();
		spaceUnit getSortDimension(int dimension);
		bool IsResult(Box& region);
		SpatialObjectType getType();
		void serialize(int8* buffer);
		void unserialize(int8 *buffer);
		uint32 getSize();
		bigSpaceUnit pointDistance(Vertex& p);
	};

	inline Segment & Segment::operator=(const Segment &rhs)
	{
		begin  = rhs.begin;
		end    = rhs.end;
		radiusBegin = rhs.radiusBegin;
		radiusEnd   = rhs.radiusEnd;
		neuronId	= rhs.neuronId;
		sectionId	= rhs.sectionId;
		segmentId	= rhs.segmentId;
		return *this;
	}

	inline bool Segment::operator==(const Segment &rhs) const
	{
		return  (begin==rhs.begin && end==rhs.end &&
				 radiusBegin==rhs.radiusBegin && radiusEnd == rhs.radiusEnd &&
				 neuronId==rhs.neuronId && sectionId==rhs.sectionId && segmentId==rhs.segmentId);
	}

}
#endif
