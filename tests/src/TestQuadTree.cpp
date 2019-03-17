#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <forest/QuadTree.hpp>

SCENARIO("Test Quad Tree") {
  GIVEN("A Quad Tree") {
    forest::QuadTree<float, 2> QuadTree({{0, 0}, {10, 10}});
    WHEN("The Quad Tree is empty") {
      THEN("Test search({ 0, 0 })") {
        REQUIRE(QuadTree.search({0, 0}) == false);
      }
      THEN("Test query({ { 0 , 0 }, { 1, 1 } })") {
        forest::QuadTree<float, 2>::Points results;
        QuadTree.query({{0, 0}, {1, 1}},
                       [&](const forest::QuadTree<float, 2>::Point& point) {
                         results.push_back(point);
                       });
        REQUIRE(results.empty() == true);
      }
    }
    WHEN("Nodes are inserted in random order") {
      QuadTree.insert({1, 2});
      QuadTree.insert({-1, 1});
      QuadTree.insert({3, 4});
      QuadTree.insert({-4, 3});
      THEN("Test search({ 0, 0 })") {
        REQUIRE(QuadTree.search({0, 0}) == false);
      }
      THEN("Test search({ -4, 3 })") {
        REQUIRE(QuadTree.search({-4, 3}) == true);
      }
      THEN("Test query({ { 0 , 0 }, { 2, 2 } })") {
        forest::QuadTree<float, 2>::Points results;
        QuadTree.query({{0, 0}, {2, 2}},
                       [&](const forest::QuadTree<float, 2>::Point& point) {
                         results.push_back(point);
                       });
        REQUIRE(results.size() == 2);
      }
      THEN("Test remove({ 1, 2 })") {
        REQUIRE(QuadTree.remove({1, 2}) == true);
        REQUIRE(QuadTree.search({1, 2}) == false);
      }
      THEN("Test clear()") {
        QuadTree.clear();
        REQUIRE(QuadTree.search({1, 2}) == false);
        REQUIRE(QuadTree.search({-1, 1}) == false);
        REQUIRE(QuadTree.search({3, 4}) == false);
        REQUIRE(QuadTree.search({-4, 3}) == false);
      }
    }
    WHEN("Nodes are inserted in ascending order") {
      for (float i = 0; i < 10; ++i) {
        QuadTree.insert({i, i});
      }
      THEN("Test search({ -1, -1 })") {
        REQUIRE(QuadTree.search({-1, -1}) == false);
      }
      THEN("Test search({ 5, 5 })") {
        REQUIRE(QuadTree.search({5, 5}) == true);
      }
      THEN("Test remove({ i, i })") {
        for (float i = 0; i < 4; ++i) {
          REQUIRE(QuadTree.remove({i, i}) == true);
          REQUIRE(QuadTree.search({i, i}) == false);
        }
      }
      THEN("Test query({ { 0 , 0 }, { 5, 5 } })") {
        forest::QuadTree<float, 2>::Points results;
        QuadTree.query({{0, 0}, {5, 5}},
                       [&](const forest::QuadTree<float, 2>::Point& point) {
                         results.push_back(point);
                       });
        REQUIRE(results.size() == 6);
      }
      THEN("Test clear()") {
        QuadTree.clear();
        for (float i = 0; i < 10; ++i) {
          REQUIRE(QuadTree.search({i, i}) == false);
        }
      }
    }
    WHEN("Nodes are inserted in descending order") {
      for (float i = 9; i >= 0; --i) {
        QuadTree.insert({i, i});
      }
      THEN("Test search({ -1, -1 })") {
        REQUIRE(QuadTree.search({-1, -1}) == false);
      }
      THEN("Test search({ 5, 5 })") {
        REQUIRE(QuadTree.search({5, 5}) == true);
      }
      THEN("Test query({ { 0 , 0 }, { 5, 5 } })") {
        forest::QuadTree<float, 2>::Points results;
        QuadTree.query(
            {{0, 0}, {5, 5}},
            [&results](const forest::QuadTree<float, 2>::Point& point) {
              results.push_back(point);
            });
        REQUIRE(results.size() == 6);
      }
      THEN("Test remove({ i, i })") {
        for (float i = 4; i >= 0; --i) {
          REQUIRE(QuadTree.remove({i, i}) == true);
          REQUIRE(QuadTree.search({i, i}) == false);
        }
      }
      THEN("Test clear()") {
        QuadTree.clear();
        for (float i = 9; i >= 0; --i) {
          REQUIRE(QuadTree.search({i, i}) == false);
        }
      }
    }
  }
}