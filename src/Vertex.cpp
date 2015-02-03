#include <cassert>
#include <limits>
#include <boost/random.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/lexical_cast.hpp>
#include "SpatialObjectFactory.hpp"
#include "Vertex.hpp"
#include "Box.hpp"
namespace FLAT
{
static boost::mt11213b generator (42u);

	std::ostream& operator << (std::ostream & lhs, const Vertex & rhs)
	{
		lhs << "(";
		for(int i=0;i<DIMENSION;i++)
			if (i==DIMENSION-1)
				lhs << rhs[i] << ")";
			else
				lhs << rhs[i] << ",";
		return lhs;
	}

	Vertex Vertex::operator+(const Vertex &rhs) const
	{
		Vertex temp;

		for (int i=0;i<DIMENSION;i++)
			temp.Vector[i] = Vector[i] + rhs.Vector[i];

		return temp;
	}

	Vertex Vertex::operator-(const Vertex &rhs) const
	{
		Vertex temp;

		for (int i=0;i<DIMENSION;i++)
			temp.Vector[i] = Vector[i] - rhs.Vector[i];

		return temp;
	}

	Vertex Vertex::operator*(const Vertex &rhs) const
	{
		Vertex temp;

		for (int i=0;i<DIMENSION;i++)
			temp.Vector[i] = Vector[i] * rhs.Vector[i];

		return temp;
	}

	Vertex Vertex::operator/(const Vertex &rhs) const
	{
		Vertex temp;

		for (int i=0;i<DIMENSION;i++)
			temp.Vector[i] = Vector[i] / rhs.Vector[i];

		return temp;
	}

	Vertex Vertex::operator+(const spaceUnit &rhs) const
	{
		Vertex temp;

		for (int i=0;i<DIMENSION;i++)
			temp.Vector[i] = Vector[i] + rhs;

		return temp;
	}

	Vertex Vertex::operator-(const spaceUnit &rhs) const
	{
		Vertex temp;

		for (int i=0;i<DIMENSION;i++)
			temp.Vector[i] = Vector[i] - rhs;

		return temp;
	}

	Vertex Vertex::operator*(const spaceUnit &rhs) const
	{
		Vertex temp;

		for (int i=0;i<DIMENSION;i++)
			temp.Vector[i] = Vector[i] * rhs;

		return temp;
	}

	Vertex Vertex::operator/(const spaceUnit &rhs) const
	{
		Vertex temp;

		for (int i=0;i<DIMENSION;i++)
			temp.Vector[i] = Vector[i] / rhs;

		return temp;
	}


	/*
	 * Static Geometric Calculation function
	 */

	// The Absolute difference in each dimension of the vector
	void Vertex::differenceVector(const Vertex &v1,const Vertex &v2, Vertex &difference)
	{
		for (int i=0;i<DIMENSION;i++)
		{
			if (v1.Vector[i]>=v2.Vector[i])
				difference.Vector[i] = v1.Vector[i]-v2.Vector[i];
			else
				difference.Vector[i] = v2.Vector[i]-v1.Vector[i];
		}
	}

	// Euclidean Distance between 2 points
	spaceUnit Vertex::distance (const Vertex &v1,const Vertex &v2)
	{
		spaceUnit d = (spaceUnit)sqrt(Vertex::squaredDistance(v1,v2));
		return d;
	}

	// Squared Euclidean Distances between 2 points FASter
	bigSpaceUnit Vertex::squaredDistance (const Vertex &v1,const Vertex &v2)
	{
		bigSpaceUnit d=0;
		Vertex difference;
		Vertex::differenceVector(v1,v2,difference);

		for (int i=0;i<DIMENSION;i++)
			d += (difference.Vector[i]) * (difference.Vector[i]);

		return d;
	}

	// Dot Product of 2 vectors
    spaceUnit Vertex::dotProduct (const Vertex &v1,const Vertex &v2)
    {
    	spaceUnit product=0;
    	for  (int i=0;i<DIMENSION;i++)
    		product += v1.Vector[i]*v2.Vector[i];
    	return product;
    }

	// Manhattan Distance between 2 points
	spaceUnit Vertex::manhattanDistance (const Vertex &v1,const Vertex &v2)
	{
		spaceUnit d=0;
		Vertex difference;
		Vertex::differenceVector(v1,v2,difference);

		for (int i=0;i<DIMENSION;i++)
			d += difference.Vector[i];

		return d;
	}

	// Magnitude of a Vector
	spaceUnit Vertex::magnitude (const Vertex &v1)
	{
		spaceUnit d=0;

		for (int i=0;i<DIMENSION;i++)
			d += v1.Vector[i]*v1.Vector[i];

		return pow(d,0.5);
	}

	// Midpoint of 2 Vertices
	void Vertex::midPoint (const Vertex &v1,const Vertex &v2,Vertex &mid)
	{
		for (int i=0;i<DIMENSION;i++)
			mid.Vector[i] = (v1.Vector[i]+v2.Vector[i]) /2;
	}

	void Vertex::randomPoint   (const Box& world,Vertex& random)
	{
		for (size_t i = 0; i < DIMENSION; i++)
			{
			boost::uniform_real<> uni_dist (world.low.Vector[i],world.high.Vector[i]);
			boost::variate_generator<boost::mt11213b &,boost::uniform_real<> > uni(generator, uni_dist);
			random[i] = uni();
			}
	}

	// for SpatialObject Base Class
	Box Vertex::getMBR()
	{
		return Box(*this,*this);
	}

	spaceUnit Vertex::getSortDimension(int dimension)
	{
		return Vector[dimension];
	}

	void Vertex::serialize(int8* buffer)
	{
		memcpy(buffer,&Vector,DIMENSION * sizeof(spaceUnit));
	}

	void Vertex::unserialize(int8* buffer)
	{
		memcpy(&Vector, buffer,DIMENSION * sizeof(spaceUnit));
	}

	uint32 Vertex::getSize()
	{
		return DIMENSION * sizeof(spaceUnit);
	}

	// for SpatialObject Base Class
	Vertex Vertex::getCenter()
	{
		return *this;
	}

	bool Vertex::IsResult(Box& region)
	{
		return Box::enclose(region,*this);
	}

	SpatialObjectType Vertex::getType()
	{
		return VERTEX;
	}

	bigSpaceUnit Vertex::pointDistance(Vertex& p)
	{
		return squaredDistance(*this,p);
	}
}
