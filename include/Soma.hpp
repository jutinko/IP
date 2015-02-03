#ifndef SOMA_HPP
#define SOMA_HPP

#include <math.h>
#include "SpatialObject.hpp"
#include "Box.hpp"

namespace FLAT
{
	/*
	 * Soma Neuron Atom
	 * and Basic Geometric Functions
	 *
	 * Author: Farhan Tauheed
	 */
	class Soma:public SpatialObject
	{
	public:
		Vertex center;
		spaceUnit radius;
		uint32 neuronId;

		Soma()
		{
		}
		Soma(const Vertex& c,const spaceUnit& r,uint32 nId)
		{
			center = c;
			radius = r;
			neuronId = nId;
		}

		Soma 		 & operator  =(const Soma& rhs);
		bool 		   operator ==(const Soma& rhs) const;
		friend std::ostream & operator << (std::ostream & lhs, const Soma& rhs);

		static spaceUnit diameter(const Soma &s);
		static bigSpaceUnit volume(const Soma &s);
		static void boundingBox(const Soma &s,Box &box);
		static bool collide(const Soma &s1,const Soma &s2);

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

	inline Soma & Soma::operator=(const Soma &rhs)
	{
		center  = rhs.center;
		radius   = rhs.radius;
		neuronId = rhs.neuronId;
		return *this;
	}

	inline bool Soma::operator==(const Soma &rhs) const
	{
		return  (center==rhs.center && radius==rhs.radius && neuronId==rhs.neuronId);
	}

	inline bool Soma::collide(const Soma &s1,const Soma &s2)
	{
		if (Vertex::distance(s1.center,s2.center)<s1.radius+s2.radius)
			return true;
		else return false;
	}


}
#endif
