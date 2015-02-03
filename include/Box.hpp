#ifndef BOX_HPP
#define BOX_HPP

#include "SpatialObject.hpp"
#include "Vertex.hpp"
#include <vector>

namespace FLAT
{
/*
* Axis Aligned Box
* Structure and Basic Geometric Functions
* N-Dimensional unless Commented on the function
*
* Author: Farhan Tauheed
*/
	class Box : public SpatialObject
	{
	public:
		Vertex low;
		Vertex high;

		Box()
			{

			}

		Box(const Vertex &low, const Vertex &high)
			{
				this->low  = low;
				this->high = high;
			}

		~Box()
		{

		}
		Box& 		   operator  =(const Box &rhs);
		bool 		   operator ==(const Box &rhs) const;
		bool 		   operator !=(const Box &rhs) const;
		Box 		   operator+(const Box &rhs) const;
		friend std::ostream & operator << (std::ostream & lhs,const Box & rhs);

		static void getAllVertices(const Box& source,std::vector<Vertex>& vertices);
		static bigSpaceUnit volume(const Box &b);
		static void center(const Box &b, Vertex &center);
		static spaceUnit length(const Box &b,const int dimension);
		//static bool intersectLine(const Box& b1,const Vertex& v1, const Vertex  v2);
		static bool overlap (const Box &b1,const Box &b2);
		static bool enclose (const Box &bigger,const Box &smaller);
		static bool enclose (const Box &b,const Vertex &v);
		static bool encloseEQU (const Box &b,const Vertex &v);
		static void combine (const Box &b1,const Box &b2,Box &combined);
		static void coveringBox (const Vertex &center,const Vertex &radialVector, Box &box);
		static void boundingBox (Box& bb,const std::vector<Vertex> &vertices);
		static void boundingBox (Box& bb,const std::vector<SpatialObject*> &vertices);
		static void randomShapedBox   (const Box& world, const bigSpaceUnit volume,Box& random,int dimFactor);
		static void randomBox   (const Box& world, const bigSpaceUnit volume,Box& random);
		static void randomBoxWithDimensions(const Box& world, const spaceUnit dimension,Box& random);
		static void infiniteBox (Box& box);
		static int lineIntersection(const Box& B, const Vertex& L1, const Vertex& L2, Vertex& intersection);
		static int lineOverlap(const Box& B, const Vertex& L1, const Vertex& L2, Vertex& intersection);
		static int  LargestDimension(Box& box);
		static void splitBox(const Box& source,std::vector<Box>& splits,int cuberootFactor);
		static bigSpaceUnit overlapVolume(const Box&b1,const Box &b2);
		static void expand(Box&b1,spaceUnit width);
		static Box make_box (const std::vector<std::string> & coords);

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

	inline Box&  Box::operator=(const Box &rhs)
	{
		low  = rhs.low;
		high = rhs.high;
		return *this;
	}

	inline bool Box::operator==(const Box &rhs) const
	{
		return (low==rhs.low && high==rhs.high);
	}
	inline bool Box::operator!=(const Box &rhs) const
	{
		for (int i=0;i<DIMENSION;++i)
			if (low[i]!=rhs.low[i] || high[i]!=rhs.high[i])
				return true;
		return false;
	}

	// Do 2 Boxes overlap Any Volume?
	inline bool Box::overlap (const Box &b1,const Box &b2)
	{
		for (int i=0;i<DIMENSION;i++)
		{
			bool overlap=false;
			if (b1.low.Vector[i]  >= b2.low.Vector[i] && b1.low.Vector[i]  <= b2.high.Vector[i]) overlap=true;
			if (b1.high.Vector[i] >= b2.low.Vector[i] && b1.high.Vector[i] <= b2.high.Vector[i]) overlap=true;
			if (b2.low.Vector[i]  >= b1.low.Vector[i] && b2.low.Vector[i]  <= b1.high.Vector[i]) overlap=true;
			if (b2.high.Vector[i] >= b1.low.Vector[i] && b2.high.Vector[i] <= b1.high.Vector[i]) overlap=true;
			if (!overlap) return false;
		}
		return true;
	}

	// Is Vertex inside the Box??
	inline bool Box::enclose (const Box &b,const Vertex &v)
	{
		for (int i=0;i<DIMENSION;i++)
			if (b.low.Vector[i]>v.Vector[i] || b.high.Vector[i]<=v.Vector[i]) return false;
		return true;
	}

	inline bool Box::encloseEQU (const Box &b,const Vertex &v)
	{
		for (int i=0;i<DIMENSION;i++)
			if (b.low.Vector[i]>v.Vector[i] || b.high.Vector[i]<v.Vector[i]) return false;
		return true;
	}

	// Does bigger Box encloses smaller Box completely?
	inline bool Box::enclose (const Box &bigger,const Box &smaller)
	{
		if (enclose(bigger,smaller.low)==false)  return false;
		if (enclose(bigger,smaller.high)==false) return false;
		return true;
	}

	// Calculate Bounding Box over set of MBRs
	inline void Box::boundingBox (Box& bb,const std::vector<SpatialObject*>& items)
	{
		if (items.empty()==true)
		{
			std::cout << "Items empty";
		}
		else
		{
			bb = items.front()->getMBR();
		}
		int size=items.size();
		for (int i=0;i<size;i++)
		{
			Box temp = items.at(i)->getMBR();
			for (int j=0;j<DIMENSION;j++)
			{
				if (bb.low.Vector[j]>temp.low.Vector[j])   bb.low.Vector[j]  = temp.low.Vector[j];
				if (bb.high.Vector[j]<temp.high.Vector[j]) bb.high.Vector[j] = temp.high.Vector[j];
			}
		}
	}

}

#endif
