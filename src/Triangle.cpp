#include "Triangle.hpp"

namespace FLAT
{
	std::ostream& operator << (std::ostream & lhs, const Triangle & rhs)
	{
		lhs << rhs.vertex1 << " " << rhs.vertex2 << " " << rhs.vertex3;
		return lhs;
	}

	Box Triangle::getMBR()
		{
		Box box;
		boundingBox(*this,box);
		return box;
		}

	Vertex Triangle::getCenter()
		{
		Vertex center;
		for (int i=0;i<DIMENSION;i++)
			center.Vector[i] = (vertex1.Vector[i]+vertex2.Vector[i]+vertex3.Vector[i])/3;
		return center;
		}

	spaceUnit Triangle::getSortDimension(int dim)
		{
		return (vertex1.Vector[dim]+vertex2.Vector[dim]+vertex3.Vector[dim])/3;
		}

	SpatialObjectType Triangle::getType()
	{
		return TRIANGLE;
	}

	bool Triangle::IsResult(Box& region)
	{
		return true;
	}

	void Triangle::serialize(int8* buffer)
	{
		int8* ptr = buffer;
		memcpy(ptr,&(vertex1.Vector),DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(ptr,&(vertex2.Vector),DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(ptr,&(vertex3.Vector),DIMENSION * sizeof(spaceUnit));
	}

	void Triangle::unserialize(int8* buffer)
	{
		int8* ptr = buffer;
		memcpy(&(vertex1.Vector), ptr,DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(&(vertex2.Vector), ptr,DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(&(vertex3.Vector), ptr,DIMENSION * sizeof(spaceUnit));
	}

	uint32 Triangle::getSize()
	{
		return DIMENSION * 3 * sizeof(spaceUnit);
	}

	//http://www.geometrictools.com/Documentation/DistancePoint3Triangle3.pdf
	// Converted from matlab code
	bigSpaceUnit Triangle::pointDistance(Vertex& P)
	{
		bigSpaceUnit sqrDistance=0;

		Vertex B  = this->vertex1;
		Vertex E0 = this->vertex2 - B;
		Vertex E1 = this->vertex3 - B;
		Vertex D  = B-P;

		spaceUnit a = Vertex::dotProduct(E0,E0);
		spaceUnit b = Vertex::dotProduct(E0,E1);
		spaceUnit c = Vertex::dotProduct(E1,E1);
		spaceUnit d = Vertex::dotProduct(E0,D);
		spaceUnit e = Vertex::dotProduct(E1,D);
		spaceUnit f = Vertex::dotProduct(D,D);

		spaceUnit det = a*c - b*b;
		spaceUnit s   = b*e - c*d;
		spaceUnit t   = b*d - a*e;

		// Terrible tree of conditions begin

		if (s+t <= det)
		{
			if (s < 0)
			{
				if (t < 0)
				{
					if (d < 0) //region4 START
					{
						t = 0;
						if (-d >= a)
						{
							s = 1;
							sqrDistance = a + 2*d + f;
						}
						else
						{
							s = -d/a;
							sqrDistance = d*s + f;
						}
					}
					else
					{
						s = 0;
						if (e >= 0)
						{
							t = 0;
							sqrDistance = f;
						}
						else
						{
							if (-e >= c)
							{
								t = 1;
								sqrDistance = c + 2*e + f;
							}
							else
							{
								t = -e/c;
								sqrDistance = e*t + f;
							}
						}
					} // Region 4 END
				}
				else // region 3 START
				{
					s = 0;
					if (e >= 0)
					{
						t = 0;
						sqrDistance = f;
					}
					else
					{
						if (-e >= c)
						{
							t = 1;
							sqrDistance = c + 2*e +f;
						}
						else
						{
							t = -e/c;
							sqrDistance = e*t + f;
						}
					}
				} //region 3 END
			}
			else
			{
				if (t < 0) // region 5
				{
					t = 0;
					if (d >= 0)
					{
						s = 0;
						sqrDistance = f;
					}
					else
					{
						if (-d >= a)
						{
							s = 1;
							sqrDistance = a + d*s + f;
						}
						else
						{
							s = -d/a;
							sqrDistance = d*s + f;
						}
					}
				}
				else // region 0
				{
					spaceUnit invDet = 1/det;
					s = s*invDet;
					t = t*invDet;
					sqrDistance = s*(a*s + b*t + 2*d) + t*(b*s + c*t + 2*e) + f;
				}
			}
		}
		else
		{
			if (s < 0) // region 2 START
			{
				spaceUnit tmp0 = b + d;
				spaceUnit tmp1 = c + e;
				if (tmp1 > tmp0) // minimum on edge s+t=1
				{
					spaceUnit numer = tmp1 - tmp0;
					spaceUnit denom = a - 2*b + c;
					if (numer >= denom)
					{
						s = 1;
						t = 0;
						sqrDistance = a + 2*b + f;
					}
					else
					{
						s = numer/denom;
						t = 1-s;
						sqrDistance = s*(a*s + b*t + 2*d) + t*(b*s + c*t + 2*e) + f;
					}
				}
				else // minimum on edge s=0
				{
					s = 0;
					if (tmp1 <= 0)
					{
						t = 1;
						sqrDistance = c + 2*e + f;
					}
					else
					{
						if (e >= 0)
						{
							t = 0;
							sqrDistance = f;
						}
						else
						{
							t = -e/c;
							sqrDistance = e*t + f;
						}
					}
				}
			} //region 2 END
			else
			{
				if (t < 0) // region6 START
				{
					spaceUnit tmp0 = b + e;
					spaceUnit tmp1 = a + d;
					if (tmp1 > tmp0)
					{
						spaceUnit numer = tmp1 - tmp0;
						spaceUnit denom = a-2*b+c;
						if (numer >= denom)
						{
							t = 1;
							s = 0;
							sqrDistance = c + 2*e + f;
						}
						else
						{
							t = numer/denom;
							s = 1 - t;
							sqrDistance = s*(a*s + b*t + 2*d) + t*(b*s + c*t + 2*e) + f;
						}
					}
					else
					{
						t = 0;
						if (tmp1 <= 0)
						{
							s = 1;
							sqrDistance = a + 2*d + f;
						}
						else
						{
							if (d >= 0)
							{
								s = 0;
								sqrDistance = f;
							}
							else
							{
								s = -d/a;
								sqrDistance = d*s + f;
							}
						}
					}
				} //region 6 END
				else // region 1 START
				{
					spaceUnit numer = c + e - b - d;
					if (numer <= 0)
					{
						s = 0;
						t = 1;
						sqrDistance = c + 2*e + f;
					}
					else
					{
						spaceUnit denom = a - 2*b + c;
						if (numer >= denom)
						{
							s = 1;
							t = 0;
							sqrDistance = a + 2*d + f;
						}
						else
						{
							s = numer/denom;
							t = 1-s;
							sqrDistance = s*(a*s + b*t + 2*d)+ t*(b*s + c*t + 2*e) + f;
						}
					}
				} // region 1 END
			}
		}


		return sqrDistance;
	}

	// Mihas implementation converted
	bigSpaceUnit Triangle::pointDistance1(Vertex& P)
	{
		bigSpaceUnit sqrDistance=0;
	    Vertex u = vertex2 - vertex1;
	    Vertex v = vertex3 - vertex1;
	    Vertex D = vertex1 - P;

		spaceUnit a = Vertex::dotProduct(u,u);
		spaceUnit b = Vertex::dotProduct(u,v);
		spaceUnit c = Vertex::dotProduct(v,v);
		spaceUnit d = Vertex::dotProduct(u,D);
		spaceUnit e = Vertex::dotProduct(v,D);

		spaceUnit det = a*c - b*b;
		spaceUnit s   = b*e - c*d;
		spaceUnit t   = b*d - a*e;

		bool region[7] = {false,false,false,false,false,false,false};

	    if (s + t <= det)
	    {
	        if (s < 0)
	        {
	            if (t < 0)
	                region[4] = true;
	            else
	                region[3] = true;
	        }
	        else if (t < 0)
	            region[5] = true;
	        else
	            region[0] = true;
	    }
	    else
	    {
	        if (s < 0)
	            region[2] = true;
	        else if (t < 0)
	            region[6] = true;
	        else
	            region[1] = true;
	    }

	    if (region[6])
	    {
	    	spaceUnit tmp0 = a + d;
	    	spaceUnit tmp1 = b + e;
	        if (tmp1 < tmp0)
	        {
	            spaceUnit numer = tmp0 - tmp1;
	            spaceUnit denom = a - 2 * b + c;
	            if (numer >= denom)
	                t = 1;
	            else
	                t = numer / denom;
	            s = t - 1;
	        }
	        else
	        {
	            t = 0;
	            if (tmp0 < 0)
	                s = 1;
	            else if (d >= 0)
	                s = 0;
	            else
	                s = -d / a;
	        }
	    }
	    else if (region[5])
	    {
	        t = 0;
	        if (d >=0)
	            s = 0;
	        else if (-d > a)
	            s = 1;
	        else
	            s = -d / a;
	    }
	    else if (region[4])
	    {
	        if (d <= 0)
	        {
	            t = 0;
	            if (-d >= a)
	                s = 1;
	            else
	                s = -d / a;
	        }
	        else
	        {
	            s = 0;
	            if (-e >= c)
	                t = 1;
	            else if (e >= 0)
	                t = 0;
	            else
	                t = -e / c;
	        }
	    }
	    else if (region[3])
	    {
	        s = 0;
	        if (e >= 0)
	            t = 0;
	        else if (-e >= c)
	            t = 1;
	        else
	            t = -e / c;
	    }
	    else if (region[2])
	    {
	        spaceUnit tmp0 = b + d;
	        spaceUnit tmp1 = c + e;
	        if (tmp1 > tmp0)
	        {
	            spaceUnit numer = tmp1-tmp0;
	            spaceUnit denom = a - 2 * b + c;
	            if (numer > denom)
	                s = 1;
	            else
	                s = numer / denom;
	            t = 1 - s;
	        }
	        else
	        {
	            s = 0;
	            if (tmp1 <=0)
	                t = 1;
	            else if (e >= 0)
	                t = 0;
	            else
	                t = -e / c;
	        }
	    }
	    else if (region[1])
	    {
	        spaceUnit numer = c + e - b - d;
	        if (numer <= 0)
	            s = 0;
	        else
	        {
	            spaceUnit denom = a - 2 * b + c;
	            if (numer >= denom)
	            	s = 1;
	            else
	            	s = numer / denom;
	        }
	        t = 1 - s;
	    }
	    else if (region[0])
		{
	        spaceUnit invDet = (1.0)/ det;
	        s *= invDet;
	        t *= invDet;
		}

	    Vertex closest_point;

	    for (int i=0;i<DIMENSION;i++)
	    	closest_point.Vector[i] = vertex1.Vector[i] + s *u.Vector[i] + t *v.Vector[i];

	    sqrDistance = Vertex::squaredDistance(closest_point,P);
	    return sqrDistance;

	}

}
