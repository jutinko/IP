#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "SpatialObject.hpp"
#include <cassert>
#include <iostream>
#include "math.h"

namespace FLAT
{
/*
* Vertex or Point Structure
* And Basic Geometric Functions
* Supports N-Dimensions
*
* Author: Farhan Tauheed
*/

class Vertex: public SpatialObject
{
public:
    spaceUnit Vector[DIMENSION];

    Vertex()
        {
            for (int i=0;i<DIMENSION;i++)
                Vector[i]=0;
        }

    Vertex(spaceUnit x, spaceUnit y ,spaceUnit z) //For just 3D Vertex
        {
            Vector[0]=x;
            Vector[1]=y;
            Vector[2]=z;
        }

    friend std::ostream  & operator << (std::ostream & lhs,const Vertex & rhs);

    spaceUnit       & operator[](int subscript);

    const spaceUnit & operator[](int subscript) const;

    Vertex          & operator=(const Vertex &rhs);

    bool 		 operator==(const Vertex &rhs) const;

    Vertex 		 operator+(const Vertex &rhs) const;

    Vertex 		 operator-(const Vertex &rhs) const;

    Vertex 		 operator*(const Vertex &rhs) const;

    Vertex 		 operator/(const Vertex &rhs) const;

    Vertex 		 operator+(const spaceUnit &rhs) const;

    Vertex 		 operator-(const spaceUnit &rhs) const;

    Vertex 		 operator*(const spaceUnit &rhs) const;

    Vertex 		 operator/(const spaceUnit &rhs) const;

    static void differenceVector(const Vertex &v1,const Vertex &v2, Vertex &difference);

    static spaceUnit distance (const Vertex &v1,const Vertex &v2);

    static bigSpaceUnit squaredDistance (const Vertex &v1,const Vertex &v2);

    static spaceUnit dotProduct (const Vertex &v1,const Vertex &v2);

    static spaceUnit manhattanDistance (const Vertex &v1,const Vertex &v2);

    static spaceUnit magnitude (const Vertex &v1);

    static void midPoint (const Vertex &v1,const Vertex &v2, Vertex &mid);

    static void randomPoint   (const Box& world,Vertex& random);



	// Spatial Object virtual functions
    Box getMBR();
	Vertex getCenter();
	spaceUnit getSortDimension(int dimension);
	bool IsResult(Box& region);
	void serialize(int8* buffer);
	void unserialize(int8* buffer);
	SpatialObjectType getType();
	uint32 getSize();
	bigSpaceUnit pointDistance(Vertex& p);
};


	inline spaceUnit & Vertex::operator [](int subscript)
	{
		assert (subscript >= 0 && subscript < DIMENSION);
		return Vector[subscript];
	}

	inline const spaceUnit & Vertex::operator [](int subscript) const
	{
		assert (subscript >= 0 && subscript < DIMENSION);
		return Vector[subscript];
	}

	inline Vertex& Vertex::operator = (const Vertex & rhs)
	{
		for (int i = 0; i < DIMENSION; i++)
			Vector[i] = rhs.Vector[i];
		return *this;
	}

	inline bool   Vertex::operator==(const Vertex &rhs) const
	{
		for (int i=0;i<DIMENSION;i++)
			if (Vector[i]!=rhs.Vector[i])
				return false;
		return true;
	}

}

#endif
