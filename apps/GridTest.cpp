#include <cstdio>
#include "Grid.hpp"
#include <assert.h>
#include "DataFileReader.hpp"
#include "SpatialQuery.hpp"

#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

void insertOneTest()
{
  IP::Grid* myGrid = new IP::Grid(10, 10, 10, 0, 1000);
  FLAT::Vertex v(100, 99, 250);
  myGrid->insert(v);
}

void insertManyTest()
{
  IP::Grid* myGrid = new IP::Grid(10, 10, 10, 0, 1000);
  FLAT::Vertex v(100, 99, 250);
  for (int i = 0; i < 1000; ++i)
  {
    v[0] = i;
    v[1] = i;
    v[2] = i;
    myGrid->insert(v);
  }
}

void findTestNegative()
{
  IP::Grid* myGrid = new IP::Grid(10, 10, 10, 0, 1000);
  FLAT::Vertex v(100, 99, 250);
  myGrid->insert(v);
  v[0] = 2;
  assert(myGrid->find(v) == false);
}

void findTestPositive()
{
  IP::Grid* myGrid = new IP::Grid(10, 10, 10, 0, 1000);
  FLAT::Vertex v(100, 99, 250);
  myGrid->insert(v);
  assert(myGrid->find(v) == true);
}

IP::Grid* testLoadFromFile(string datafile)
{
  IP::Grid* myGrid = new IP::Grid(10, 10, 10, 0, 1000);
  FLAT::DataFileReader* input = new FLAT::DataFileReader(datafile);
  FLAT::SpatialObject* object;
  FLAT::Vertex vertex;
  FLAT::Vertex v;
  while(input->hasNext())
  {
    object = input->getNext();
    vertex = object->getCenter();
    for(int i = 0; i < 3; ++i)
    {
      v[0] = vertex[0];
      v[1] = vertex[1];
      v[2] = vertex[2];
    }
    //printf("x: %f y: %f z: %f\n", data[0], data[1], data[2]);
    myGrid->insert(v);
  }
  return myGrid;
}

void findNeighbourWithRadiusTest()
{
  IP::Grid* myGrid = new IP::Grid(10, 10, 10, 0, 1000);
  int temp[] = {1, 2, 3};
  vector<int> v(temp, temp+sizeof(temp)/sizeof(temp[0]));
  myGrid->findNeighbourWithRadius(v, 2);
}

void putSetIntoVectorTest()
{
  IP::Grid* myGrid = new IP::Grid(10, 10, 10, 0, 1000);
  FLAT::Vertex v(1, 2, 3);
  myGrid->insert(v);
  v[0] = 101;
  myGrid->insert(v);
  vector<FLAT::Vertex> result;
  auto cell = myGrid->getCell(v);
  myGrid->putSetIntoVector(result, *cell);
  printf("temp size: %lu\n", result.size());
}

void notEnoughPointKNNTest()
{
  IP::Grid* myGrid = new IP::Grid(10, 10, 10, 0, 1000);
  FLAT::Vertex v(1, 2, 3);
  myGrid->insert(v);
  v[0] = 0.5;
  vector<FLAT::Vertex> result;
  FLAT::Vertex p(1, 2.5, 3);
  FLAT::SpatialQuery q;
  q.k = 3;
  q.Point = p;
  myGrid->kNNQuery(q, result);
  assert(result.size() == 2);
  auto it = result.begin();
  while(it != result.end())
  {
    printf("x: %f y: %f z: %f\n", (*it)[0], (*it)[1], (*it)[2]);
    ++it;
  }
}

void simpleKNNTest()
{
  IP::Grid* myGrid = new IP::Grid(10, 10, 10, 0, 1000);
  FLAT::Vertex v(1, 2, 3);
  myGrid->insert(v);
  v[0] = 0.5;
  myGrid->insert(v);
  v[2] = 0.5;
  myGrid->insert(v);
  v[1] = 3.5;
  vector<FLAT::Vertex> result;
  FLAT::Vertex p(1, 2, 3.5);
  FLAT::SpatialQuery q;
  q.k = 3;
  q.Point = p;
  myGrid->kNNQuery(q, result);
  auto it = result.begin();
  while(it != result.end())
  {
    printf("x: %f y: %f z: %f\n", (*it)[0], (*it)[1], (*it)[2]);
    ++it;
  }
}

// This method is wrong?!
void testPointDistance()
{
  IP::Grid* myGrid = new IP::Grid(10, 10, 10);
  FLAT::Vertex v(-100, -100, -100);
  vector<int> cell(3);
  cell[0] = 0;
  cell[1] = 0;
  cell[2] = 0;
  double dist = myGrid->distancePointToAxisAlignedCube(v, cell);
  v[0] = 300;;
  v[1] = 10;;
  v[2] = 30;;
  dist = myGrid->distancePointToAxisAlignedCube(v, cell);
  assert(dist-200 < 0.0001);
}

void query(IP::Grid* myGrid, string queryfile)
{
  FLAT::Timer querying;
  querying.start();
  /********************** DO QUERIES **********************/
  vector<FLAT::SpatialQuery> queries;
  FLAT::SpatialQuery::ReadQueries(queries, queryfile);

  for(vector<FLAT::SpatialQuery>::iterator query = queries.begin(); query != queries.end(); query++) 
  {
    vector<FLAT::Vertex> result;
    myGrid->kNNQuery(*query, result);
  //  for(vector<FLAT::Vertex>::iterator it = result.begin(); it != result.end(); ++it)
  //  {
  //    printf("%f ", (*it).getCenter()[0]);
  //  }
  //  printf("\n");
  }
  cout << querying << endl;
}

int main(int argc, const char* argv[])
{
  string queryfile;
  string datafile;

  po::options_description desc("Options");
  desc.add_options()
    ("help", "produce help message")
    ("datafile", po::value<string>(&datafile), "name fordatafile")
    ("queryfile", po::value<string>(&queryfile), "file containing the queries");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  IP::Grid* myGrid = testLoadFromFile(datafile);
  vector<FLAT::Vertex> result;
  FLAT::Vertex p(312.684000, 95.985200, 291.729000);
  FLAT::SpatialQuery q;
  q.k = 3;
  q.Point = p;
  myGrid->kNNQuery(q, result);
  query(myGrid, queryfile);
  //findNeighbourWithRadiusTest();
  //printf("Insert one test\n");
  //insertOneTest();
  //printf("Insert many test\n");
  //insertManyTest();
  //printf("Testing find\n");
  //findTestPositive();
  //findTestNegative();
  //putSetIntoVectorTest();
  //simpleKNNTest();
  //notEnoughPointKNNTest();
  //testPointDistance();
  return 0;
}
