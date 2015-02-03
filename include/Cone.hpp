#ifndef CONE_
#define CONE_

#include <math.h>
#include "SpatialObject.hpp"
#include "Box.hpp"

namespace FLAT {

#define DISCRETIZE_RESOLUTION 10
	/*
	 * Conic Structure
	 * and Basic Geometric Functions
	 *
	 * Author: Farhan Tauheed
	 */
	class Cone:public SpatialObject
	{
	public:
		Vertex begin;
		Vertex end;
		spaceUnit radiusBegin;
		spaceUnit radiusEnd;

		Cone()
		{
		}
		Cone(const Vertex& b,const Vertex& e,const spaceUnit& r1, const spaceUnit& r2)
		{
			begin = b;
			end   = e;
			radiusBegin = r1;
			radiusEnd   = r2;
		}

		Cone 		 & operator  =(const Cone& rhs);
		bool 		   operator ==(const Cone& rhs) const;
		friend std::ostream & operator << (std::ostream & lhs, const Cone& rhs);

		static void center(const Cone &c,Vertex &v);
		static spaceUnit length(const Cone &c);
		static bigSpaceUnit volume(const Cone &c);
		static void boundingBox(const Cone &c,Box &box);
		static bool overlap(const Cone &c, const Box &b);
		static void discretize(const Cone &c,Vertex vertices[]);

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

	inline Cone & Cone::operator=(const Cone &rhs)
	{
		begin  = rhs.begin;
		end    = rhs.end;
		radiusBegin = rhs.radiusBegin;
		radiusEnd   = rhs.radiusEnd;
		return *this;
	}

	inline bool Cone::operator==(const Cone &rhs) const
	{
		return  (begin==rhs.begin && end==rhs.end && radiusBegin==rhs.radiusBegin && radiusEnd == rhs.radiusEnd);
	}

}
#endif
