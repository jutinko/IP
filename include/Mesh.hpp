#ifndef MESH_TRIANGLE_HPP
#define MESH_TRIANGLE_HPP

#include "SpatialObject.hpp"
#include "Box.hpp"
#include <cassert>
#include <iostream>
#include "math.h"

namespace FLAT
{

class Mesh: public SpatialObject
{
public:
    Vertex vertex1, vertex2, vertex3;
    uint32 neuronId;
    uint32 v1,v2,v3;

    Mesh()
        {
        }

    Mesh(Vertex vr1, Vertex vr2 ,Vertex vr3,uint32 nId, uint32 vtx1,uint32 vtx2 ,uint32 vtx3)
        {
            vertex1 = vr1;
            vertex2 = vr2;
            vertex3 = vr3;
            neuronId = nId;
            v1 = vtx1;
            v2 = vtx2;
            v3 = vtx3;
        }

	Mesh 		 & operator  =(const Mesh& rhs);
	bool 		   operator ==(const Mesh& rhs) const;
	friend std::ostream & operator << (std::ostream & lhs, const Mesh& rhs);

	static void boundingBox(const Mesh &c,Box &box);

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
};

inline Mesh & Mesh::operator=(const Mesh &rhs)
	{
		vertex1 = rhs.vertex1;
		vertex2 = rhs.vertex2;
		vertex3 = rhs.vertex3;
		neuronId= rhs.neuronId;
		v1=rhs.v1;
		v2=rhs.v2;
		v3=rhs.v3;
		return *this;
	}

inline bool Mesh::operator==(const Mesh &rhs) const
	{
		return  (vertex1==rhs.vertex1 &&
				 vertex2==rhs.vertex2 &&
				 vertex3==rhs.vertex3 &&
				 neuronId==rhs.neuronId &&
				 v1==rhs.v1 &&
				 v2==rhs.v2 &&
				 v3==rhs.v3);
	}

inline void Mesh::boundingBox(const Mesh &c, Box &box)
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
