#include "Soma.hpp"


namespace FLAT
{
	std::ostream & operator << (std::ostream & lhs, const Soma& rhs)
	{
		lhs << "Center: " << rhs.center << "[" << rhs.radius << "]" << "  NeuronId:" << rhs.neuronId;
		return lhs;
	}

	spaceUnit Soma::diameter(const Soma &s)
	{
		return s.radius*2;
	}

	bigSpaceUnit Soma::volume(const Soma &s)
	{
		return (s.radius*s.radius*s.radius*M_PI*4/3);
	}

	// Spatial Object virtual functions
	Box Soma::getMBR()
	{
		Box box;
		for (int i=0;i<DIMENSION;i++)
		{
			box.high.Vector[i] = center.Vector[i]+radius;
			box.low.Vector[i]  = center.Vector[i]-radius;
		}
		return box;
	}
	Vertex Soma::getCenter()
	{
		return center;
	}
	spaceUnit Soma::getSortDimension(int dimension)
	{
		return center.Vector[dimension];
	}
	bool Soma::IsResult(Box& region)
	{
		return true;
	}
	SpatialObjectType Soma::getType()
	{
		return SOMA;
	}

	void Soma::serialize(int8* buffer)
	{
		int8* ptr = buffer;
		memcpy(ptr,&(center.Vector),DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(ptr,&radius,sizeof(spaceUnit));
		ptr += sizeof(spaceUnit);
		memcpy(ptr,&neuronId,sizeof(uint32));
	}

	void Soma::unserialize(int8* buffer)
	{
		int8* ptr = buffer;
		memcpy(&(center.Vector), ptr,DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(&radius, ptr,sizeof(spaceUnit));
		ptr += sizeof(spaceUnit);
		memcpy(&neuronId, ptr,sizeof(uint32));
	}

	uint32 Soma::getSize()
	{
		return ((DIMENSION+1)*sizeof(spaceUnit))+sizeof(uint32);
	}

	bigSpaceUnit Soma::pointDistance(Vertex& p)
	{
		std::cout << "ERROR: pointDistance method is not overloaded with this geometry: SOMA" << std::endl;
		return 0;
	}
}
