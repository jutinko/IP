#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "SpatialObject.hpp"
#include "Box.hpp"
#include <cassert>
#include <iostream>
#include "math.h"

namespace FLAT
{

class Triangle: public SpatialObject
{
public:
    Vertex vertex1, vertex2, vertex3;

    Triangle()
        {
        }

    Triangle(Vertex v1, Vertex v2 ,Vertex v3)
        {
            vertex1 = v1;
            vertex2 = v2;
            vertex3 = v3;
        }

	Triangle 		 & operator  =(const Triangle& rhs);
	bool 		   operator ==(const Triangle& rhs) const;
	friend std::ostream & operator << (std::ostream & lhs, const Triangle& rhs);

	static void boundingBox(const Triangle &c,Box &box);

	// Spatial Object virtual functions
    Box getMBR();
	Vertex getCenter();
	spaceUnit getSortDimension(int dimension);
	bool IsResult(Box& region);
	SpatialObjectType getType();
	void serialize(int8* buffer);
	void unserialize(int8* buffer);
	uint32 getSize();
	bigSpaceUnit pointDistance(Vertex& p);
	bigSpaceUnit pointDistance1(Vertex& p);
};

inline Triangle & Triangle::operator=(const Triangle &rhs)
	{
		vertex1 = rhs.vertex1;
		vertex2 = rhs.vertex2;
		vertex3 = rhs.vertex3;
		return *this;
	}

inline bool Triangle::operator==(const Triangle &rhs) const
	{
		return  (vertex1==rhs.vertex1 && vertex2==rhs.vertex2 && vertex3==rhs.vertex3);
	}

inline void Triangle::boundingBox(const Triangle &c, Box &box)
	{
		box.low = c.vertex1;
		box.high = c.vertex1;

		for (int i=0;i<DIMENSION;i++)
		{
			if (c.vertex2.Vector[i]<box.low.Vector[i]) box.low.Vector[i] = c.vertex2.Vector[i];
			if (c.vertex3.Vector[i]<box.low.Vector[i]) box.low.Vector[i] = c.vertex3.Vector[i];

			if (c.vertex2.Vector[i]>box.high.Vector[i]) box.high.Vector[i] = c.vertex2.Vector[i];
			if (c.vertex3.Vector[i]>box.high.Vector[i]) box.high.Vector[i] = c.vertex3.Vector[i];
		}
	}

}

#endif
