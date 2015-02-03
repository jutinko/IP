#include <vector>
#include <boost/random.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/lexical_cast.hpp>
#include <limits>
#include "Box.hpp"

using namespace std;
namespace FLAT
{
	static boost::mt11213b generator (42u);

	// only for 3D
	void Box::getAllVertices(const Box& source,std::vector<Vertex>& vertices)
	{
		if (DIMENSION!=3) cout << "GetAllVertices only for 3D";

		Vertex temp;
		temp = source.low; vertices.push_back(temp);
		temp[0] = source.high[0];vertices.push_back(temp);
		temp[1] = source.high[1];vertices.push_back(temp);
		temp[0] = source.low[0];vertices.push_back(temp);

		temp = source.high; vertices.push_back(temp);
		temp[0] = source.low[0];vertices.push_back(temp);
		temp[1] = source.low[1];vertices.push_back(temp);
		temp[0] = source.high[0];vertices.push_back(temp);
	}

	std::ostream & operator << (std::ostream & lhs,const Box & rhs)
	{
		lhs << "Low: " << rhs.low << "  High: " << rhs.high;
		return lhs;
	}

	// Make a bounding Box over a sphere or eliptical surface
	void Box::coveringBox (const Vertex &center, const Vertex &radialVector, Box &box)
	{
		box.low  = center - radialVector;
		box.high = center + radialVector;
	}

	bool IntersectLineAABB(const Vertex& O,    const Vertex& D,    const Vertex& min,    const Vertex& max,    spaceUnit t[],    spaceUnit epsilon)
	{
		Vertex C = (min + max) * 0.5;
		Vertex e = (max - min) * 0.5;
		int parallel = 0;
		bool found = false;
		Vertex d = C - O;
		for (int i = 0; i < DIMENSION; ++i)
		{
			if (fabsf(D[i]) < epsilon)
				parallel |= 1 << i;
			else
			{
				spaceUnit es = (D[i] > 0.0) ? e[i] : -e[i];
				spaceUnit invDi = 1.0 / D[i];
				if (!found)
				{
					t[0] = (d[i] - es) * invDi;
					t[1] = (d[i] + es) * invDi;
					found = true;
				}
				else
				{
					spaceUnit s = (d[i] - es) * invDi;
					if (s > t[0])
						t[0] = s;
					s = (d[i] + es) * invDi;
					if (s < t[1])
						t[1] = s;
					if (t[0] > t[1])
						return false;
				}
			}
		}
		if (parallel)
			for (int i = 0; i < DIMENSION; ++i)
				if (parallel & (1 << i))
					if (fabsf(d[i] - t[0] * D[i]) > e[i] || fabs(d[i] - t[1] * D[i]) > e[i])
						return false;
		return true;
	}

	// Box Line intersection
	bool GetIntersection( spaceUnit fDst1, spaceUnit fDst2,const Vertex& P1,const Vertex& P2, Vertex &Hit)
	{
		//cout << " GetIntersection " << fDst1 << " " << fDst2 << " ";
		if ( (fDst1 * fDst2) >= 0.0f) return false;
		if ( fDst1 == fDst2) return false;

		for (int i=0;i<DIMENSION;++i)
			Hit[i] = P1[i] + (P2[i]-P1[i]) * ( -fDst1/(fDst2-fDst1) );
		//cout << Hit << endl;
		return true;
	}

	bool InBox( const Vertex& Hit, const FLAT::Box& B, const int Axis)
	{
	if ( Axis==1 && Hit[2] > B.low[2] && Hit[2] < B.high[2] && Hit[1] > B.low[1] && Hit[1] < B.high[1]) {return true;}
	if ( Axis==2 && Hit[2] > B.low[2] && Hit[2] < B.high[2] && Hit[0] > B.low[0] && Hit[0] < B.high[0]) {return true;}
	if ( Axis==3 && Hit[0] > B.low[0] && Hit[0] < B.high[0] && Hit[1] > B.low[1] && Hit[1] < B.high[1]) {return true;}
	return false;
	}

	bool OnLine( const Vertex& Hit, const Vertex& L1,const Vertex& L2)
	{
		//return true;
		Box B;
		for (int i=0;i<DIMENSION;++i)
			if (L1[i]<L2[i]) {B.low[i] = L1[i];B.high[i] = L2[i];}
			else {B.low[i] = L2[i];B.high[i] = L1[i];}

		int j;
		for (j=0;j<DIMENSION;j++)
			if (B.low[j]>Hit[j] || B.high[j]<Hit[j])
				break;
		if (j==DIMENSION) {return true;}
		return false;
	}

	bool InBox2(const Vertex& Hit, const FLAT::Box& B,const int a)
	{
		int j;
		for (j=0;j<DIMENSION;++j)
			if (B.low[j]>Hit[j] || B.high[j]<Hit[j])
				break;
		if (j==DIMENSION) return true;
		return false;
	}

	// Very inefficient.. modified code from.. only first intersection
	// http://www.3dkingdoms.com/weekly/weekly.php?a=3
	int Box::lineIntersection(const Box& B, const Vertex& L1, const Vertex& L2, Vertex& Hit)
	{
		int count=0;

		if (L2[0] < B.low[0] && L1[0] < B.low[0]) return count;
		if (L2[0] > B.high[0] && L1[0] > B.high[0]) return count;
		if (L2[1] < B.low[1] && L1[1] < B.low[1]) return count;
		if (L2[1] > B.high[1] && L1[1] > B.high[1]) return count;
		if (L2[2] < B.low[2] && L1[2] < B.low[2]) return count;
		if (L2[2] > B.high[2] && L1[2] > B.high[2]) return count;
		if (FLAT::Box::enclose(B,L1) && FLAT::Box::enclose(B,L2)) return count;

		if (GetIntersection( L1[0]-B.low[0], L2[0]-B.low[0], L1, L2, Hit))
			if (InBox( Hit, B,1 ))
				if (OnLine(Hit,L1,L2)) count++;
		if (count==0)
		if (GetIntersection( L1[1]-B.low[1], L2[1]-B.low[1], L1, L2, Hit))
			if (InBox( Hit, B,2 ))
				if (OnLine(Hit,L1,L2)) count++;
		if (count==0)
		if (GetIntersection( L1[2]-B.low[2], L2[2]-B.low[2], L1, L2, Hit))
			if (InBox( Hit, B,3))
				if (OnLine(Hit,L1,L2)) count++;
		if (count==0)
		if (GetIntersection( L1[0]-B.high[0], L2[0]-B.high[0], L1, L2, Hit))
			if (InBox( Hit, B,1 ))
				if (OnLine(Hit,L1,L2)) count++;
		if (count==0)
		if (GetIntersection( L1[1]-B.high[1], L2[1]-B.high[1], L1, L2, Hit))
			if (InBox( Hit, B,2 ))
				if (OnLine(Hit,L1,L2)) count++;
		if (count==0)
		if (GetIntersection( L1[2]-B.high[2], L2[2]-B.high[2], L1, L2, Hit))
			if (InBox( Hit, B,3))
				if (OnLine(Hit,L1,L2)) count++;

//		if (count>0)
//			if (!Box::encloseEQU(B,Hit))
//				cout << "Intersection outside Box!! " << B << " int= " << Hit<< " c " << count<<endl;
		return count;
	}


//  bool firstLineIntersection(
//			const Vertex& O,
//			const Vertex& D,
//			const Vertex& min,
//			const Vertex& max,
//			spaceUnit t[],
//			spaceUnit epsilon)
//	{
//		Vertex C = (min + max) * (spaceUnit)0.5;
//		Vertex e = (max - min) * (spaceUnit)0.5;
//		int parallel = 0;
//		bool found = false;
//		Vertex d = C - O;
//		for (int i = 0; i < DIMENSION; ++i)
//		{
//			if (Math<spaceUnit>::Fabs(D[i]) < epsilon)
//				parallel |= 1 << i;
//			else
//			{
//				spaceUnit es = (D[i] > (spaceUnit)0.0) ? e[i] : -e[i];
//				spaceUnit invDi = (spaceUnit)1.0 / D[i];
//				if (!found)
//				{
//					t[0] = (d[i] - es) * invDi;
//					t[1] = (d[i] + es) * invDi;
//					found = true;
//				}
//				else
//				{
//					spaceUnit s = (d[i] - es) * invDi;
//					if (s > t[0])
//						t[0] = s;
//					s = (d[i] + es) * invDi;
//					if (s < t[1])
//						t[1] = s;
//					if (t[0] > t[1])
//						return false;
//				}
//			}
//		}
//		if (parallel)
//			for (int i = 0; i < 3; ++i)
//				if (parallel & (1 << i))
//					if (Math<spaceUnit>::Fabs(d[i] - t[0] * D[i]) > e[i] || Math<spaceUnit>::Fabs(d[i] - t[1] * D[i]) > e[i])
//						return false;
//		return true;
//	}

	int Box::lineOverlap(const Box& B, const Vertex& L1, const Vertex& L2, Vertex& Hit)
	{
		int count=0;

		if (L2[0] < B.low[0] && L1[0] < B.low[0]) return count;
		if (L2[0] > B.high[0] && L1[0] > B.high[0]) return count;
		if (L2[1] < B.low[1] && L1[1] < B.low[1]) return count;
		if (L2[1] > B.high[1] && L1[1] > B.high[1]) return count;
		if (L2[2] < B.low[2] && L1[2] < B.low[2]) return count;
		if (L2[2] > B.high[2] && L1[2] > B.high[2]) return count;
		if (FLAT::Box::enclose(B,L1) && FLAT::Box::enclose(B,L2)) return 1;

		if (count==0)
		if (GetIntersection( L1[0]-B.low[0], L2[0]-B.low[0], L1, L2, Hit))
			if (InBox( Hit, B,1 ))
				if (OnLine(Hit,L1,L2)) count++;

		if (count==0)
		if (GetIntersection( L1[1]-B.low[1], L2[1]-B.low[1], L1, L2, Hit))
			if (InBox( Hit, B,2 ))
				if (OnLine(Hit,L1,L2)) count++;

		if (count==0)
		if (GetIntersection( L1[2]-B.low[2], L2[2]-B.low[2], L1, L2, Hit))
			if (InBox( Hit, B,3))
				if (OnLine(Hit,L1,L2)) count++;
		if (count==0)
		if (GetIntersection( L1[0]-B.high[0], L2[0]-B.high[0], L1, L2, Hit))
			if (InBox( Hit, B,1 ))
				if (OnLine(Hit,L1,L2)) count++;
		if (count==0)
		if (GetIntersection( L1[1]-B.high[1], L2[1]-B.high[1], L1, L2, Hit))
			if (InBox( Hit, B,2 ))
				if (OnLine(Hit,L1,L2)) count++;
		if (count==0)
		if (GetIntersection( L1[2]-B.high[2], L2[2]-B.high[2], L1, L2, Hit))
			if (InBox( Hit, B,3))
				if (OnLine(Hit,L1,L2)) count++;

		return count;
	}

	// Give the volume of the Box
	bigSpaceUnit Box::volume(const Box &b)
	{
		bigSpaceUnit v=1;
		Vertex distance;
		Vertex::differenceVector(b.low,b.high,distance);

		for (int i=0;i<DIMENSION;i++)
			v *= distance.Vector[i];
		return v;
	}

	// returns true if line (L1, L2) intersects with the box (B1, B2)
	// returns intersection point in Hit
//	bool Box::intersectLine(const Box& b1,const Vertex& v1,const Vertex& v2)// CVec3 B1, CVec3 B2, CVec3 L1, CVec3 L2, CVec3 &Hit)
//	{
////	if (L2.x < B1.x && L1.x < B1.x) return false;
////	if (L2.x > B2.x && L1.x > B2.x) return false;
////	if (L2.y < B1.y && L1.y < B1.y) return false;
////	if (L2.y > B2.y && L1.y > B2.y) return false;
////	if (L2.z < B1.z && L1.z < B1.z) return false;
////	if (L2.z > B2.z && L1.z > B2.z) return false;
////	if (L1.x > B1.x && L1.x < B2.x &&
////	    L1.y > B1.y && L1.y < B2.y &&
////	    L1.z > B1.z && L1.z < B2.z)
////	    {Hit = L1;
////	    return true;}
////	if ( (GetIntersection( L1.x-B1.x, L2.x-B1.x, L1, L2, Hit) && InBox( Hit, B1, B2, 1 ))
////	  || (GetIntersection( L1.y-B1.y, L2.y-B1.y, L1, L2, Hit) && InBox( Hit, B1, B2, 2 ))
////	  || (GetIntersection( L1.z-B1.z, L2.z-B1.z, L1, L2, Hit) && InBox( Hit, B1, B2, 3 ))
////	  || (GetIntersection( L1.x-B2.x, L2.x-B2.x, L1, L2, Hit) && InBox( Hit, B1, B2, 1 ))
////	  || (GetIntersection( L1.y-B2.y, L2.y-B2.y, L1, L2, Hit) && InBox( Hit, B1, B2, 2 ))
////	  || (GetIntersection( L1.z-B2.z, L2.z-B2.z, L1, L2, Hit) && InBox( Hit, B1, B2, 3 )))
////		return true;
////
////	return false;
//	}

	// Give the Center Coordinate of the Box
	void Box::center(const Box &b, Vertex &center)
	{
		Vertex::midPoint (b.low,b.high,center);
	}

	// Side Length of a particular dimension
	spaceUnit Box::length(const Box &b,const int dimension)
	{
		return (b.high.Vector[dimension]-b.low.Vector[dimension]);
	}

	bigSpaceUnit Box::overlapVolume(const Box&b1,const Box &b2)
	{
		if (overlap(b1,b2)==false) return 0;

		int countOverlap=0;
		Box overlap;
		combine(b1,b2,overlap);

		for (int i=0;i<DIMENSION;i++)
		{
			if (b1.low[i]  >= b2.low[i] && b1.low[i]  <= b2.high[i]) {countOverlap++;overlap.low[i]=b1.low[i];}
			if (b1.high[i] >= b2.low[i] && b1.high[i] <= b2.high[i]) {countOverlap++;overlap.high[i]=b1.high[i];}
			if (b2.low[i]  >= b1.low[i] && b2.low[i]  <= b1.high[i]) {countOverlap++;overlap.low[i]=b2.low[i];}
			if (b2.high[i] >= b1.low[i] && b2.high[i] <= b1.high[i]) {countOverlap++;overlap.high[i]=b2.high[i];}
		}

		return volume(overlap);
	}

	// Combine 2 Axis Aligned Bounding Boxes into a bigger cube without
	void Box::combine (const Box &b1,const Box &b2,Box &combined)
	{
		for (int i=0;i<DIMENSION;i++)
		{
			if (b1.low.Vector[i] <=b2.low.Vector[i])  combined.low.Vector[i]  = b1.low.Vector[i];  else combined.low.Vector[i]  = b2.low.Vector[i];
			if (b1.high.Vector[i]>=b2.high.Vector[i]) combined.high.Vector[i] = b1.high.Vector[i]; else combined.high.Vector[i] = b2.high.Vector[i];
		}
	}

	// Calculate Bounding Box over a Polyherdra
	void Box::boundingBox (Box& bb,const std::vector<Vertex> &vertices)
	{
		if (vertices.empty()==true)
		{
			std::cout << "list empty";
		}
		else
		{
			bb.low = vertices.at(0);
			bb.high = vertices.at(0);
		}
		int size= vertices.size();
		for (int i=0;i<size;i++)
		{
			for (int j=0;j<DIMENSION;j++)
			{
				if (bb.low.Vector[j]>vertices.at(i).Vector[j]) bb.low.Vector[j] = vertices.at(i).Vector[j];
				if (bb.high.Vector[j]<vertices.at(i).Vector[j]) bb.high.Vector[j] = vertices.at(i).Vector[j];
			}
		}
	}

	// Make a random box inside the World Box with the given percentage Volume
	void Box::randomBox(const Box& world, const bigSpaceUnit volume,Box& random) {
        
        spaceUnit dimensionInc = pow(volume, 1.0/3.0);

        for (size_t i = 0; i < DIMENSION; i++) {
            boost::uniform_real<> uni_dist (world.low.Vector[i], world.high.Vector[i] - dimensionInc*(world.high.Vector[i] - world.low.Vector[i]));
            boost::variate_generator<boost::mt11213b &,boost::uniform_real<> > uni(generator, uni_dist);
            spaceUnit low = uni();
            random.low.Vector[i]  = low;
            random.high.Vector[i] = low + dimensionInc*(world.high.Vector[i] - world.low.Vector[i]);
		}
	}

	// Make a random box inside the World Box with the given percentage Volume
	void Box::randomBoxWithDimensions(const Box& world, const spaceUnit dimension,Box& random)
	{
	for (size_t i = 0; i < DIMENSION; i++)
		{
		boost::uniform_real<> uni_dist (world.low.Vector[i]+dimension,world.high.Vector[i]-dimension);
		boost::variate_generator<boost::mt11213b &,boost::uniform_real<> > uni(generator, uni_dist);
		spaceUnit center = uni();
		random.low.Vector[i]  = center-dimension;
		random.high.Vector[i] = center+dimension;
		}
	}

	// Make a random box inside the World Box with the given percentage Volume
	void Box::randomShapedBox(const Box& world, const bigSpaceUnit volume,Box& random,int dimFactor)
	{

	//make a random vector
	Vertex randomWeights;
	Box limits(Vertex(1,1,1),Vertex(dimFactor,dimFactor,dimFactor));
	Vertex::randomPoint(limits,randomWeights);

	// select dimension to randomly normalize the vector
	int randomDimension = rand() % DIMENSION;

	// normalize weight vector
	for (int i=0;i<DIMENSION;i++)
		randomWeights.Vector[i] /= randomWeights.Vector[randomDimension];

	// Calculate the magnitude m.. such that wm * hm * dm = v where w,h,d,v = width,height,depth and volume

	spaceUnit m = pow( (volume/ (randomWeights[0]*randomWeights[1]*randomWeights[2]) ) ,1.0/3.0 ) /2;

	// make randomBox
	for (size_t i = 0; i < DIMENSION; i++)
		{
		boost::uniform_real<> uni_dist (world.low.Vector[i]+(randomWeights[i]*m),world.high.Vector[i]-(randomWeights[i]*m) );
		boost::variate_generator<boost::mt11213b &,boost::uniform_real<> > uni(generator, uni_dist);
		spaceUnit center = uni();
		random.low.Vector[i]  = center-(randomWeights[i]*m);
		random.high.Vector[i] = center+(randomWeights[i]*m);
		}
	}

	Box Box::make_box (const std::vector<std::string> & coords)
	{
		assert (coords.size () == 6);
		Box temp;
		for (int i=0;i<DIMENSION;i++)
		{
			temp.low.Vector[i]  = boost::lexical_cast<spaceUnit> (coords[i]);
			temp.high.Vector[i] = boost::lexical_cast<spaceUnit> (coords[i+DIMENSION]);
		}
		return temp;
	}

	void Box::infiniteBox (Box& box)
	{
		for (int i=0;i<DIMENSION;i++)
		{
			box.low.Vector[i] = -std::numeric_limits<spaceUnit>::max();
			box.high.Vector[i]  = std::numeric_limits<spaceUnit>::max();
		}
	}

	int  Box::LargestDimension(Box& box)
	{
		Vertex diff;
		Vertex::differenceVector(box.low,box.high,diff);
		spaceUnit max =0;
		int maxIndex =0;
		for (int i=0;i<DIMENSION;i++)
			if (diff[i]>max)
			{
				max  = diff[i];
				maxIndex = i;
			}
		return maxIndex;
	}

	Box Box::operator+(const Box &rhs) const
	{
		Box temp;
		Box::combine(*this,rhs,temp);
		return temp;
	}

	Box Box::getMBR()
	{
		return *this;
	}

	Vertex Box::getCenter()
	{
		Vertex v;
		Box::center(*this,v);
		return v;
	}

	spaceUnit Box::getSortDimension(int dimension)
	{
		return (high.Vector[dimension]+low.Vector[dimension])/2;
		//return low.Vector[dimension];
	}

	bool Box::IsResult(Box& region)
	{
		return Box::overlap(region,*this);
	}

	SpatialObjectType Box::getType()
	{
		return BOX;
	}

	void Box::serialize(int8* buffer)
	{
		int8* ptr = buffer;
		memcpy(ptr,&(low.Vector),DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(ptr,&(high.Vector),DIMENSION * sizeof(spaceUnit));
	}

	void Box::unserialize(int8* buffer)
	{
		int8* ptr = buffer;
		memcpy(&(low.Vector), ptr,DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(&(high.Vector), ptr,DIMENSION * sizeof(spaceUnit));
	}

	void Box::expand(Box& b1,spaceUnit width)
	{
		b1.low = b1.low-width;
		b1.high = b1.high+width;
	}

	uint32 Box::getSize()
	{
		return 2* DIMENSION * sizeof(spaceUnit);
	}
	// Splits 3D boxes into mini 3D boxes
	void Box::splitBox(const Box& source,std::vector<Box>& splits,int cuberootFactor)
	{
		//int total = cuberootFactor * cuberootFactor *cuberootFactor;
		Vertex difference;
		for (int i=0;i<DIMENSION;i++)
			difference[i] = (source.high[i]-source.low[i])/(cuberootFactor+0.0);

		for (spaceUnit x = 0; x<cuberootFactor; x++)
			for (spaceUnit y = 0; y<cuberootFactor; y++)
				for (spaceUnit z = 0; z<cuberootFactor; z++)
				{
				Box temp;
				temp.low[0] = source.low[0]+(x*difference[0]); temp.high[0] = source.low[0]+((x+1)*difference[0]);
				temp.low[1] = source.low[1]+(y*difference[1]); temp.high[1] = source.low[1]+((y+1)*difference[1]);
				temp.low[2] = source.low[2]+(z*difference[2]); temp.high[2] = source.low[2]+((z+1)*difference[2]);
				splits.push_back(temp);
				}
	}
	//http://graphics.cs.uiuc.edu/surface/doc/aabb_8cpp-source.html
	bigSpaceUnit Box::pointDistance(Vertex& p)
	{
		//cout << "\tPoint :" <<p  << " Box: " << *this <<endl;
		Vertex center;
		Vertex::midPoint (low,high,center);
		//cout << "\tCenter :" <<center <<endl;

		Vertex diff;
		Vertex::differenceVector(center,p,diff);
		//cout << "\tDifference :" <<diff <<endl;

		bigSpaceUnit distance=0;
		Vertex halfLength;
		spaceUnit delta=0;

		for (int i=0;i<DIMENSION;i++)
		{
			halfLength.Vector[i] = (high.Vector[i]-low.Vector[i])/2;
			//cout << "\tdimension: " << i << " Half length: " <<halfLength.Vector[i] <<endl;
			if (diff[i] > halfLength[i])
			{
				delta = diff[i] - halfLength[i];
				//cout << "\tdelta square: " << delta* delta <<endl;
				distance += delta*delta;
			}
		}

		return distance;
	}
}

