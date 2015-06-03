#include "Grid.hpp"
#include <cstdio>
#include "DataFileReader.hpp"
#include <cfloat>
#include <algorithm>

namespace IP
{
  Grid::Grid(int x, int y, int z, double lower, double upper)
  {
    printf("Generating a grid of dimension: %d, %d, %d\n", x, y, z);
    this->x = x;
    this->y = y;
    this->z = z;
    this->range_lower = lower;
    this->range_upper = upper;
    this->cellUnit = (upper-lower)/x;

    grid = new unordered_set<FLAT::Vertex, VertexHash, VertexEqual>***[x];
    for(int i = 0; i < x; ++i)
    {
      grid[i] = new unordered_set<FLAT::Vertex, VertexHash, VertexEqual>**[y];
      for(int j = 0; j < y; ++j)
      {
        grid[i][j] = new unordered_set<FLAT::Vertex, VertexHash, VertexEqual>*[z];
        for(int k = 0; k < z; ++k)
        {
          grid[i][j][k] = new unordered_set<FLAT::Vertex, VertexHash, VertexEqual>();
        }
      }
    }
  }

  Grid::Grid(int x, int y, int z) : Grid::Grid(x, y, z, DEFAULT_RANGE_LOWER_BOUND, DEFAULT_RANGE_UPPER_BOUND)
  {
  }

  Grid::Grid() : Grid::Grid(DEFAULT_SIZE, DEFAULT_SIZE, DEFAULT_SIZE)
  {
  }

  vector<int> Grid::getCellCoordinates(FLAT::Vertex& v)
  {
    vector<int> coordinates(3);
    double interval_range = this->range_upper-this->range_lower;
    coordinates[0] = v[0]/(interval_range/this->x) < 0 ? 0 : 
      (v[0]/(interval_range/this->x) >= this->x ? this->x-1 : v[0]/(interval_range/this->x));
    coordinates[1] = v[1]/(interval_range/this->y) < 0 ? 0 : 
      (v[1]/(interval_range/this->y) >= this->y ? this->y-1 : v[1]/(interval_range/this->y));
    coordinates[2] = v[2]/(interval_range/this->z) < 0 ? 0 : 
      (v[2]/(interval_range/this->z) >= this->z ? this->z-1 : v[2]/(interval_range/this->z));
    return coordinates;
  }

  unordered_set<FLAT::Vertex, VertexHash, VertexEqual>* Grid::getCell(FLAT::Vertex& v)
  {
    vector<int> coordinates = getCellCoordinates(v);
    return grid[coordinates[0]][coordinates[1]][coordinates[2]];
  }

  void Grid::insert(FLAT::Vertex& v)
  {
    unordered_set<FLAT::Vertex, VertexHash, VertexEqual>* cell = getCell(v);
    FLAT::Vertex* newV = new FLAT::Vertex(v[0], v[1], v[2]);

    if(cell->find(*newV) == cell->end())
    {
      cell->insert(*newV);
    } else 
    {
      printf("Already in\n");
    }
  }

  bool Grid::find(FLAT::Vertex& v)
  {
    unordered_set<FLAT::Vertex, VertexHash, VertexEqual>* cell = getCell(v);
    FLAT::Vertex* newV = new FLAT::Vertex(v[0], v[1], v[2]);
    unordered_set<FLAT::Vertex, VertexHash, VertexEqual>::iterator result = cell->find(*newV);
    delete newV;
    return !(result == cell->end());
  }

  // This function return true if and only if the given cell at x, y and z
  // is valid neightbour at radius r
  bool Grid::isValidCell(bool edge, vector<int>& q, vector<int>& neighbour, int radius)
  {
    if(neighbour[0] >= 0 && neighbour[0] < this->x && 
        neighbour[1] >= 0 && neighbour[1] < this->y && 
        neighbour[2] >= 0 && neighbour[2] < this->z)
    {
      if(edge)
      {
        return true;
      } else 
      {
        return abs(q[1]-neighbour[1]) == radius || abs(q[2]-neighbour[2]) == radius;
      }
    }
    return false;
  }

  // TODO: Modify this method to return set pointers to the cells
  vector<vector<int>> Grid::findNeighbourWithRadius(vector<int>& q, int radius)
  {
    vector<vector<int>> result;
    int queryX = q[0];
    int queryY = q[1];
    int queryZ = q[2];
    for(int i = -radius; i <= radius; ++i)
    {
      vector<int> cell(3);
      for(int j = -radius; j <= radius; ++j)
      {
        for(int k = -radius; k <= radius; ++k)
        {
          cell[0] = queryX+i; 
          cell[1] = queryY+j;
          cell[2] = queryZ+k;
          if(isValidCell(abs(i) == radius, q, cell, radius))
          {
            //printf("Pushing with radius %d x: %d y: %d z: %d\n", radius, cell[0], cell[1], cell[2]);
            result.push_back(cell);
          }
        }
      }
    }
    //printf("Size: %lu\n", result.size());
    return result;
  }

  void Grid::putSetIntoVector(vector<FLAT::Vertex>& tempResult,
    unordered_set<FLAT::Vertex, VertexHash, VertexEqual>& cellObjects)
  {
    auto it = cellObjects.begin();
    while(it != cellObjects.end())
    {
      tempResult.push_back(*it);
      ++it;
    }
  }


  vector<FLAT::Vertex> Grid::getVerticesOfCell(vector<int> cell)
  {
    vector<FLAT::Vertex> result;
    for(int i = 0; i < 2; ++i)
    {
      for(int j = 0; j < 2; ++j)
      {
        for(int k = 0; k < 2; ++k)
        {
          FLAT::Vertex v((cell[0]+i)*cellUnit, (cell[1]+j)*cellUnit, (cell[2]+k)*cellUnit);
          result.push_back(v);
        }
      }
    }
    return result;
  }

  double Grid::findMinDintanceToNeighbourCells(FLAT::Vertex& p, vector<vector<int>>& neighbours)
  {
    double result = DBL_MAX;
    vector<FLAT::Vertex> vertices;
    auto it = neighbours.begin();
    //unordered_set<FLAT::Vertex, VertexHash, VertexEqual>* cell;
    while(it != neighbours.end())
    {
      //cell = grid[(*it)[0]][(*it)[1]][(*it)[2]];
      //auto it1 = cell->begin();
      //while(it1 != cell->end())
      //{
      //  double dist = FLAT::Vertex::distance(p, *it1);
      //  result = dist < result ? dist : result;
      //  ++it1;
      //}
      double dist = distancePointToAxisAlignedCube(p, *it);
      result = result < dist ? result : dist;
      ++it;
    }
    return result;
  }

  FLAT::spaceUnit Grid::distancePointToAxisAlignedCube(FLAT::Vertex v, vector<int> cellCoordinates)
  {
    int count = 0;
    int minIndex;
    double min = DBL_MAX;
    double max = -DBL_MAX;
    double result;
    FLAT::Vertex normalisedV = v;
    for(int i = 0; i < 3; ++i)
    {
      normalisedV[i] -= cellCoordinates[i]*cellUnit;
      normalisedV[i] = normalisedV[i] < 0 ? -normalisedV[i]+cellUnit : normalisedV[i];
      //printf("v becomes: %f %f %f cellcoodri: %d %d %d\n", normalisedV[0], normalisedV[1], normalisedV[2], cellCoordinates[0], cellCoordinates[1], cellCoordinates[2]);
      // TODO: SET EPSILON CONSTANT
      if(normalisedV[i]+0.0000001 > cellUnit)
        ++count;
      max = max < normalisedV[i] ? normalisedV[i] : max;
      if(min > normalisedV[i])
      {
        minIndex = i;
        min = normalisedV[i];
      }
    }
    if(count == 1)
    {
      result = max-cellUnit;
    } else if(count == 2)
    {
      double a = normalisedV[(minIndex+1)%3]-cellUnit;
      double b = normalisedV[(minIndex+2)%3]-cellUnit;
      result = sqrt(a*a+b*b);
      //printf("Result is: %f\n", result);
    } else
    {
      result = DBL_MAX;
      vector<FLAT::Vertex> vertices;
      vertices = getVerticesOfCell(cellCoordinates);
      auto it1 = vertices.begin();
      while(it1 != vertices.end())
      {
        double dist = FLAT::Vertex::distance(v, *it1);
        result = dist < result ? dist : result;
        ++it1;
      }
    }
    //printf("Result is: %f\n", result);
    return result;
  }

  struct comparator
  {
    FLAT::Vertex v;
    comparator(FLAT::Vertex v)
    {
      this->v = v;
    }

    bool operator() (FLAT::Vertex v1, FLAT::Vertex v2)
    {
      return FLAT::Vertex::distance(v, v1) < FLAT::Vertex::distance(v, v2);
    }
  };

  void sortAccordingToDinstance(FLAT::Vertex& v, vector<FLAT::Vertex>& neighbours)
  {
    comparator c(v);
    sort(neighbours.begin(), neighbours.end(), c);
  }

  // Resizes the vector too
  double findMaxDistance(int k, FLAT::Vertex p, vector<FLAT::Vertex>& tempResult)
  {
    double result = DBL_MAX;
    //auto it = tempResult.begin();
    //while(it != tempResult.end())
    //{
    //  double dist = FLAT::Vertex::distance(p, *it);
    //  result = dist > result ? dist : result;
    //  ++it;
    //}
    sortAccordingToDinstance(p, tempResult);
    if(tempResult.size() < (size_t)k)
      return result;
    return FLAT::Vertex::distance(tempResult[k-1], p);
    //int index = tempResult.size() > (size_t)k ? k-1 : tempResult.size()-1;
    //tempResult.resize(index+1);
    //return FLAT::Vertex::distance(tempResult[index], p);
  }
 
  void Grid::filterNeighbourList(FLAT::Vertex v, vector<vector<int>>& neighbours, double d)
  {
    vector<vector<int>> temp;
    auto it = neighbours.begin();
    while(it != neighbours.end())
    {
      if(distancePointToAxisAlignedCube(v, *it) < d)
      {
        temp.push_back(*it);
      }
      ++it;
    }
    neighbours.clear();
    neighbours.insert(neighbours.begin(), temp.begin(), temp.end());
  }

  // This method assumes that the cells in the grid are squares
  void Grid::kNNQuery(FLAT::SpatialQuery& q, vector<FLAT::Vertex>& result)
  {
    FLAT::Vertex p = q.Point;
    int k = q.k;
    vector<int> coordinates = getCellCoordinates(p);
    vector<FLAT::Vertex> tempResult;
    // Put the objects that are in the same cell into the tempResult
    putSetIntoVector(tempResult, *getCell(p));
    int radius = 1;

    // Neighbours are the cells that are within radius away from the cell the query
    // point is in
    vector<vector<int>> neighbours = findNeighbourWithRadius(coordinates, radius);
    //printf("Size before: %lu\n", neighbours.size());
    double maxDistance = findMaxDistance(k, p, tempResult);
    filterNeighbourList(p, neighbours, maxDistance);
    //printf("Size after: %lu tempResult size: %lu\n", neighbours.size(), tempResult.size());

    while(tempResult.size() < (size_t)k || 
        neighbours.size() > 0)
    {
      auto it = neighbours.begin();
      while(it != neighbours.end())
      {
        auto cell = grid[(*it)[0]][(*it)[1]][(*it)[2]];
        putSetIntoVector(tempResult, *cell);
        ++it;
      }
      ++radius;
      maxDistance = findMaxDistance(k, p, tempResult);
      neighbours = findNeighbourWithRadius(coordinates, radius);
      if(neighbours.size() == 0)
        break;
    //printf("Size before: %lu\n", neighbours.size());
      filterNeighbourList(p, neighbours, maxDistance);
    //printf("Size after: %lu\n", neighbours.size());
      //printf("Radius is %d neightbour size %lu\n", radius, neighbours.size());
    }
    sortAccordingToDinstance(p, tempResult);
    // Checking if the first point is the original query point
    if(result.size() > 0 && p == result[0]) 
    {
      int size = tempResult.size()-1 < (size_t)k ? tempResult.size()-1 : k;
      result.insert(result.begin(), tempResult.begin()+1, tempResult.begin()+size+1);
    } else
    {
      int size = tempResult.size() < (size_t)k ? tempResult.size() : k;
      result.insert(result.begin(), tempResult.begin(), tempResult.begin()+size);
    }
  }
}
