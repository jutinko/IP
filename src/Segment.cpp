#include "Segment.hpp"

namespace FLAT
{
	std::ostream& operator << (std::ostream & lhs, const Segment & rhs)
		{
		lhs << "Begin: " << rhs.begin << "[" << rhs.radiusBegin << "]   End: "
			<< rhs.end << "[" << rhs.radiusEnd << "]"
			<< "   nID:" << rhs.neuronId << " secID:" <<rhs.sectionId << " segID:" <<rhs.segmentId;
		return lhs;
		}

	void Segment::center(const Segment &c,Vertex &center)
		{
		Vertex::midPoint (c.begin,c.end,center);
		}

	spaceUnit Segment::length(const Segment &c)
		{
		return Vertex::distance(c.begin,c.end);
		}

	//http://www.grc.nasa.gov/WWW/K-12/rocket/Images/volumenose.gif
	bigSpaceUnit Segment::volume(const Segment &c)
		{
		bigSpaceUnit volume=0;
		volume = (M_PI/12) * length(c) * ((c.radiusBegin*c.radiusBegin) + (c.radiusBegin*c.radiusEnd) + (c.radiusEnd*c.radiusEnd) );
		return volume;
		}

	//Farhan's implementation of the tightest bounding box
	void Segment::boundingBox(const Segment &c, Box &box)
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

	//Discretize vertices on the Surface of Segment
	void Segment::discretize(const Segment &c __attribute__((__unused__)),
						  Vertex vertices[] __attribute__((__unused__)))
		{
		//TODO
		}

	// BBP method of if Segment overlaps the Box
	bool Segment::overlap(const Segment&c,const Box&b)
		{
		Vertex vertices[DISCRETIZE_RESOLUTION];
		discretize(c,vertices);

		int i=0;
		for (i=0;i<DISCRETIZE_RESOLUTION;i++)
			if (Box::enclose(b,vertices[i])==true) return true;

		return false;
		}

	//Farhan's implementation of the tightest bounding box
	Box Segment::getMBR()
		{
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

	Vertex Segment::getCenter()
		{
		Vertex center;
		for (int i=0;i<DIMENSION;i++)
			center[i] = (begin.Vector[i]+end.Vector[i])/2;
		return center;
		}

	spaceUnit Segment::getSortDimension(int dimension)
	{
		return (begin.Vector[dimension]+end.Vector[dimension])/2;
	}

	SpatialObjectType Segment::getType()
	{
		return SEGMENT;
	}

	bool Segment::IsResult(Box& region)
	{
		return true;
	}

	void Segment::serialize(int8* buffer)
	{
		int8* ptr = buffer;
		memcpy(ptr,&(begin.Vector),DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(ptr,&(end.Vector),DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(ptr,&radiusBegin,sizeof(spaceUnit));
		ptr += sizeof(spaceUnit);
		memcpy(ptr,&radiusEnd,sizeof(spaceUnit));
		ptr += sizeof(spaceUnit);
		memcpy(ptr,&neuronId,sizeof(uint32));
		ptr += sizeof(uint32);
		memcpy(ptr,&sectionId,sizeof(uint32));
		ptr += sizeof(uint32);
		memcpy(ptr,&segmentId,sizeof(uint32));
	}

	void Segment::unserialize(int8* buffer)
	{
		int8* ptr = buffer;
		memcpy(&(begin.Vector), ptr,DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(&(end.Vector), ptr,DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(&radiusBegin,ptr,sizeof(spaceUnit));
		ptr += sizeof(spaceUnit);
		memcpy(&radiusEnd,ptr,sizeof(spaceUnit));
		ptr += sizeof(spaceUnit);
		memcpy(&neuronId,ptr,sizeof(uint32));
		ptr += sizeof(uint32);
		memcpy(&sectionId,ptr,sizeof(uint32));
		ptr += sizeof(uint32);
		memcpy(&segmentId,ptr,sizeof(uint32));
	}

	uint32 Segment::getSize()
	{
		return (((DIMENSION*2)+2)*sizeof(spaceUnit))+(sizeof(uint32)*3);
	}

	bigSpaceUnit Segment::pointDistance(Vertex& p)
	{
		std::cout << "ERROR: pointDistance method is not overloaded with this geometry: SEGMENT" << std::endl;
		return 0;
	}
}
