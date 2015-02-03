#ifndef GRID_HPP
#define GRID_HPP

#include <list>
#include "SpatialObject.hpp"

using namespace std;

namespace IP
{
  class Grid
  {
    list<FLAT::SpatialObject>* grid;
    public:
      Grid(int, int, int);   
  };
}

#endif
