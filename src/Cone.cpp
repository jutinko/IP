#include "Cone.hpp"

namespace FLAT
{
	std::ostream& operator << (std::ostream & lhs, const Cone & rhs)
		{
		lhs << "Begin: " << rhs.begin << "[" << rhs.radiusBegin << "]   End: " << rhs.end << "[" << rhs.radiusEnd << "]";
		return lhs;
		}

	void Cone::center(const Cone &c,Vertex &center)
		{
		Vertex::midPoint (c.begin,c.end,center);
		}

	spaceUnit Cone::length(const Cone &c)
		{
		return Vertex::distance(c.begin,c.end);
		}

	//http://www.grc.nasa.gov/WWW/K-12/rocket/Images/volumenose.gif
	bigSpaceUnit Cone::volume(const Cone &c)
		{
		bigSpaceUnit volume=0;
		volume = (M_PI/12) * length(c) * ((c.radiusBegin*c.radiusBegin) + (c.radiusBegin*c.radiusEnd) + (c.radiusEnd*c.radiusEnd) );
		return volume;
		}

	//Farhan's implementation of the tightest bounding box
	void Cone::boundingBox(const Cone &c, Box &box)
		{
		spaceUnit distance=0;
		Vertex difference,projectionBegin,projectionEnd;

		// Calculate Projection on End Points
		distance = length(c);
		Vertex::differenceVector(c.begin,c.end,difference);

		for (int i=0;i<DIMENSION;i++)
			if (distance>0)
			{
			projectionBegin.Vector[i]= sinf(acosf(difference.Vector[i] / distance))*c.radiusBegin;
			projectionEnd.Vector[i]  = sinf(acosf(difference.Vector[i] / distance))*c.radiusEnd;
			}

		// Make Covering Box on both end Points and Combine
		Box boxBegin,boxEnd;

		Box::coveringBox(c.begin,projectionBegin,boxBegin);
		Box::coveringBox(c.end,projectionEnd,boxEnd);

		Box::combine(boxBegin,boxEnd,box);
		}

	//Discretize vertices on the Surface of Cone
	void Cone::discretize(const Cone &c __attribute__((__unused__)),
						  Vertex vertices[] __attribute__((__unused__)))
		{
		//TODO
		}

	// BBP method of if Cone overlaps the Box
	bool Cone::overlap(const Cone&c,const Box&b)
		{
		Vertex vertices[DISCRETIZE_RESOLUTION];
		discretize(c,vertices);

		int i=0;
		for (i=0;i<DISCRETIZE_RESOLUTION;i++)
			if (Box::enclose(b,vertices[i])==true) return true;

		return false;
		}

	//Farhan's implementation of the tightest bounding box
	Box Cone::getMBR()
		{
//		Box mbr;
//		for (int i=0;i<DIMENSION;i++)
//		{
//			if (begin.Vector[i]<end.Vector[i])
//				{
//				mbr.low.Vector[i] = begin.Vector[i]-radiusBegin;
//				mbr.high.Vector[i] = end.Vector[i]+radiusEnd;
//				}
//			else
//				{
//				mbr.low.Vector[i] = end.Vector[i]-radiusEnd;
//				mbr.high.Vector[i] = begin.Vector[i]+radiusBegin;
//				}
//		}
//		return mbr;
		spaceUnit distance=0;
		Vertex difference,projectionBegin,projectionEnd;
		Box box;

		// Calculate Projection on End Points
		distance = Vertex::distance(begin,end);
		Vertex::differenceVector(begin,end,difference);

		for (int i=0;i<DIMENSION;i++)
			if (distance>0)
			{
			projectionBegin.Vector[i]= sinf(acosf(difference.Vector[i] / distance))*radiusBegin;
			projectionEnd.Vector[i]  = sinf(acosf(difference.Vector[i] / distance))*radiusEnd;
			}

		// Make Covering Box on both end Points and Combine
		Box boxBegin,boxEnd;

		Box::coveringBox(begin,projectionBegin,boxBegin);
		Box::coveringBox(end,projectionEnd,boxEnd);

		Box::combine(boxBegin,boxEnd,box);
		return box;
		}

	Vertex Cone::getCenter()
		{
		Vertex center;
		for (int i=0;i<DIMENSION;i++)
			center[i] = (begin.Vector[i]+end.Vector[i])/2;
		return center;
		}

	spaceUnit Cone::getSortDimension(int dimension)
	{
//		if (begin.Vector[dimension]<end.Vector[dimension])
//			return (begin.Vector[dimension]-radiusBegin+end.Vector[dimension]+radiusEnd)/2;
//		else
//			return (begin.Vector[dimension]+radiusBegin+end.Vector[dimension]-radiusEnd)/2;
		return (begin.Vector[dimension]+end.Vector[dimension])/2;
	}

	SpatialObjectType Cone::getType()
	{
		return CONE;
	}

	bool Cone::IsResult(Box& region)
	{
		return true;
	}

	void Cone::serialize(int8* buffer)
	{
		int8* ptr = buffer;
		memcpy(ptr,&(begin.Vector),DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(ptr,&(end.Vector),DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(ptr,&radiusBegin,sizeof(spaceUnit));
		ptr += sizeof(spaceUnit);
		memcpy(ptr,&radiusEnd,sizeof(spaceUnit));
	}

	void Cone::unserialize(int8* buffer)
	{
		int8* ptr = buffer;
		memcpy(&(begin.Vector), ptr,DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(&(end.Vector), ptr,DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(&radiusBegin,ptr,sizeof(spaceUnit));
		ptr += sizeof(spaceUnit);
		memcpy(&radiusEnd,ptr,sizeof(spaceUnit));
	}

	uint32 Cone::getSize()
	{
		return ((DIMENSION*2)+2)*sizeof(spaceUnit);
	}

	bigSpaceUnit Cone::pointDistance(Vertex& p)
	{
		std::cout << "ERROR: pointDistance method is not overloaded with this geometry: CONE" << std::endl;
		return 0;
	}
}
