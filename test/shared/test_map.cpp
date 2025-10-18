#include <boost/test/unit_test.hpp>

#include <state.h>
#include <fstream>

class Map_Observer : public state::Map
{
public:
  Map_Observer() : state::Map(){};
  int get_max_round() { return max_round; }
  std::vector<state::Area> get_areas() { return areas; }
  std::vector<std::vector<int>> get_area_connections() { return area_connections; }
};

std::string json_data = R"({
  "biomes": { "0": ["MOUNTAIN"], "1": ["HILL"], "2": ["HILL"] },
  "specializations": { "0": [], "1": [], "2": ["MAGIC_REGION"] },
  "units": { "0": 0, "1": 0, "2": 0 },
  "relations": { "0": [1], "1": [0, 2], "2": [1] }
})";




BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestMap)
{
  {
    std::ofstream tmp("tmp.json");
    tmp << json_data;
    tmp.close();
    Map_Observer observer;

    // error testing
    BOOST_CHECK_THROW(observer.get_area(100), std::invalid_argument);
    BOOST_CHECK_THROW(observer.load_from_json("non_existing_file.json"), std::runtime_error);

    // methods testing
    observer.load_from_json("tmp.json");
    BOOST_CHECK_EQUAL(observer.get_areas().size(), 3);

    BOOST_CHECK_EQUAL(observer.get_area_connections().size(), 3);

    BOOST_CHECK_EQUAL(observer.get_area_connections()[0].size(), 1);
    BOOST_CHECK_EQUAL(observer.get_area_connections()[1].size(), 2);
    BOOST_CHECK_EQUAL(observer.get_area_connections()[2].size(), 1);

    BOOST_CHECK_EQUAL(observer.get_area(0)->id, 0);
    std::remove("tmp.json");

  }
}

/* vim: set sw=2 sts=2 et : */
