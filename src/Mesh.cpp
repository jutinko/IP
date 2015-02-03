#include "Mesh.hpp"

namespace FLAT
{
	std::ostream& operator << (std::ostream & lhs, const Mesh & rhs)
	{
		lhs << rhs.vertex1 << " " << rhs.vertex2 << " " << rhs.vertex3 << " nID:" <<rhs.neuronId << " v's:" << rhs.v1 <<"-" <<rhs.v2 << "-" <<rhs.v3 ;
		return lhs;
	}

	Box Mesh::getMBR()
		{
		Box box;
		boundingBox(*this,box);
		return box;
		}

	Vertex Mesh::getCenter()
		{
		Vertex center;
		for (int i=0;i<DIMENSION;i++)
			center.Vector[i] = (vertex1.Vector[i]+vertex2.Vector[i]+vertex3.Vector[i])/3;
		return center;
		}

	spaceUnit Mesh::getSortDimension(int dim)
		{
		return (vertex1.Vector[dim]+vertex2.Vector[dim]+vertex3.Vector[dim])/3;
		}

	SpatialObjectType Mesh::getType()
	{
		return MESH;
	}

	bool Mesh::IsResult(Box& region)
	{
		return true;
	}

	void Mesh::serialize(int8* buffer)
	{
		int8* ptr = buffer;
		memcpy(ptr,&(vertex1.Vector),DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(ptr,&(vertex2.Vector),DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(ptr,&(vertex3.Vector),DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);

		memcpy(ptr,&neuronId,sizeof(uint32));
		ptr += sizeof(uint32);

		memcpy(ptr,&v1,sizeof(uint32));
		ptr += sizeof(uint32);

		memcpy(ptr,&v2,sizeof(uint32));
		ptr += sizeof(uint32);

		memcpy(ptr,&v3,sizeof(uint32));
	}

	void Mesh::unserialize(int8* buffer)
	{
		int8* ptr = buffer;
		memcpy(&(vertex1.Vector), ptr,DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(&(vertex2.Vector), ptr,DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);
		memcpy(&(vertex3.Vector), ptr,DIMENSION * sizeof(spaceUnit));
		ptr += DIMENSION * sizeof(spaceUnit);

		memcpy(&neuronId, ptr,sizeof(uint32));
		ptr += sizeof(uint32);
		memcpy(&v1, ptr,sizeof(uint32));
		ptr += sizeof(uint32);
		memcpy(&v2, ptr,sizeof(uint32));
		ptr += sizeof(uint32);
		memcpy(&v3, ptr,sizeof(uint32));
	}

	uint32 Mesh::getSize()
	{
		return (DIMENSION*3*sizeof(spaceUnit))+(sizeof(uint32)*4);
	}

	//http://www.geometrictools.com/Documentation/DistancePoint3Triangle3.pdf
	// Converted from matlab code
	bigSpaceUnit Mesh::pointDistance(Vertex& P)
	{
		bigSpaceUnit sqrDistance=0;

		Vertex B  = this->vertex1;
		Vertex E0 = this->vertex2 - B;  // TODO confirm
		Vertex E1 = this->vertex3 - B;  // TODO confirm
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
}
