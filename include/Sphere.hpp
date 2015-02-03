#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <math.h>
#include "SpatialObject.hpp"
#include "Box.hpp"

namespace FLAT
{
	/*
	 * Sphere Structure
	 * and Basic Geometric Functions
	 *
	 * Author: Farhan Tauheed
	 */
	class Sphere:public SpatialObject
	{
	public:
		Vertex center;
		spaceUnit radius;

		Sphere()
		{
		}
		Sphere(const Vertex& c,const spaceUnit& r)
		{
			center = c;
			radius = r;
		}

		Sphere 		 & operator  =(const Sphere& rhs);
		bool 		   operator ==(const Sphere& rhs) const;
		friend std::ostream & operator << (std::ostream & lhs, const Sphere& rhs);

		static spaceUnit diameter(const Sphere &s);
		static bigSpaceUnit volume(const Sphere &s);
		static void boundingBox(const Sphere &s,Box &box);
		static bool collide(const Sphere &s1,const Sphere &s2);

		// Spatial Object virtual functions
		Box getMBR();
		Vertex getCenter();
		spaceUnit getSortDimension(int dimension);
		bool IsResult(Box& region);
		void serialize(int8* buffer);
		void unserialize(int8* buffer);
		uint32 getSize();
		SpatialObjectType getType();
		bigSpaceUnit pointDistance(Vertex& p);
	};

	inline Sphere & Sphere::operator=(const Sphere &rhs)
	{
		center  = rhs.center;
		radius   = rhs.radius;
		return *this;
	}

	inline bool Sphere::operator==(const Sphere &rhs) const
	{
		return  (center==rhs.center && radius==rhs.radius);
	}

	inline bool Sphere::collide(const Sphere &s1,const Sphere &s2)
	{
		if (Vertex::distance(s1.center,s2.center)<s1.radius+s2.radius)
			return true;
		else return false;
	}


}
#endif
