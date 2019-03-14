#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <vector>

namespace forest {
template <typename Arithmetic, int Capacity = 1>
class QuadTree {
  static_assert(Capacity > 0, "Invalid QuadTree Capacity");

 public:
  using Point = std::array<Arithmetic, 2>;
  using Points = std::vector<Point>;
  using PointsIt = typename std::vector<Point>::iterator;
  using Callback = std::function<void(const Point &)>;

 public:
  class Range {
    template <typename T, int K>
    friend class QuadTree;

   private:
    Point origin;
    Point transform;

   public:
    Range() = default;

    Range(const Point &ORIGIN, const Point &TRANSFORM)
        : origin(ORIGIN), transform(TRANSFORM) {}

    ~Range() = default;

   public:
    void setOrigin(const Point &ORIGIN) { origin = ORIGIN; }

    void setTransform(const Point &TRANSFORM) { transform = TRANSFORM; }

   public:
    Point getOrigin() const { return origin; }

    Point getTransform() const { return transform; }

   public:
    bool contains(const Point &point) const {
      return point[0] >= origin[0] - transform[0] &&
             point[0] <= origin[0] + transform[0] &&
             point[1] >= origin[1] - transform[1] &&
             point[1] <= origin[1] + transform[1];
    }

    // bool contains(const Point & point) const {
    //	for (int i = 0; i < 2; ++i) {
    //		if (!(point[i] >= origin[i] - transform[i] && point[i] <= origin[i] +
    //transform[i])) { 			return false;
    //		}
    //	}
    //	return true;
    //}
    bool intersects(const Range &other) const {
      return origin[0] - transform[0] <= other.origin[0] + other.transform[0] &&
             origin[0] + transform[0] >= other.origin[0] - other.transform[0] &&
             origin[1] - transform[1] <= other.origin[1] + other.transform[1] &&
             origin[1] + transform[1] >= other.origin[1] - other.transform[1];
    }

    // bool intersects(const Range & other) const {
    //	for (int i = 0; i < 2; ++i) {
    //		if (!(origin[i] - transform[i] <= other.origin[i] + other.transform[i]
    //&& origin[i] + transform[i] >= other.origin[i] - other.transform[i])) {
    //			return false;
    //		}
    //	}
    //	return true;
    //}
  };

 private:
  Points bucket;

 private:
  Range boundary;

 private:
  bool divided{false};

 private:
  QuadTree *NW{nullptr};
  QuadTree *NE{nullptr};
  QuadTree *SW{nullptr};
  QuadTree *SE{nullptr};

 private:
  void divide() {
    NW = new QuadTree<Arithmetic, Capacity>(
        {{boundary.origin[0] - boundary.transform[0] / 2,
          boundary.origin[1] + boundary.transform[1] / 2},
         {boundary.transform[0] / 2, boundary.transform[1] / 2}});
    NE = new QuadTree<Arithmetic, Capacity>(
        {{boundary.origin[0] + boundary.transform[0] / 2,
          boundary.origin[1] + boundary.transform[1] / 2},
         {boundary.transform[0] / 2, boundary.transform[1] / 2}});
    SW = new QuadTree<Arithmetic, Capacity>(
        {{boundary.origin[0] - boundary.transform[0] / 2,
          boundary.origin[1] - boundary.transform[1] / 2},
         {boundary.transform[0] / 2, boundary.transform[1] / 2}});
    SE = new QuadTree<Arithmetic, Capacity>(
        {{boundary.origin[0] + boundary.transform[0] / 2,
          boundary.origin[1] - boundary.transform[1] / 2},
         {boundary.transform[0] / 2, boundary.transform[1] / 2}});
    divided = true;
  }

  void merge() {
    delete NW;
    delete NE;
    delete SW;
    delete SE;
    NW = nullptr;
    NE = nullptr;
    SW = nullptr;
    SE = nullptr;
    divided = false;
  }

 public:
  QuadTree() = delete;

  QuadTree(const Range &BOUNDARY) : boundary(BOUNDARY) {}

  ~QuadTree() { clear(); }

 public:
  bool insert(const Point &point) {
    if (!boundary.contains(point)) return false;
    if (!divided) {
      if (std::find(bucket.begin(), bucket.end(), point) != bucket.end())
        return false;
      bucket.push_back(point);
      if (bucket.size() > Capacity) {
        divide();
        PointsIt it = bucket.begin();
        while (it != bucket.end()) {
          if (NW->boundary.contains(*it))
            NW->insert(*it);
          else if (NE->boundary.contains(*it))
            NE->insert(*it);
          else if (SW->boundary.contains(*it))
            SW->insert(*it);
          else if (SE->boundary.contains(*it))
            SE->insert(*it);
          it = bucket.erase(it);
        }
      }
      return true;
    }
    return NW->insert(point) || NE->insert(point) || SW->insert(point) ||
           SE->insert(point);
  }

  bool remove(const Point &point) {
    if (!boundary.contains(point)) return false;
    if (!divided) {
      bucket.erase(std::remove(bucket.begin(), bucket.end(), point),
                   bucket.end());
      return true;
    }
    if (NW->remove(point) || NE->remove(point) || SW->remove(point) ||
        SE->remove(point)) {
      if (!NW->divided && !NE->divided && !SW->divided && !SE->divided) {
        if (NW->bucket.empty() && NE->bucket.empty() && SW->bucket.empty() &&
            SE->bucket.empty()) {
          merge();
        }
      }
      return true;
    }
    return false;
  }

  bool search(const Point &point) {
    if (!boundary.contains(point)) return false;
    if (divided) {
      return NW->search(point) || NE->search(point) || SW->search(point) ||
             SE->search(point);
    }
    return std::find(bucket.begin(), bucket.end(), point) != bucket.end();
  }

  void query(const Range &range, const Callback callback) {
    if (!range.intersects(boundary)) return;
    if (divided) {
      NW->query(range, callback);
      NE->query(range, callback);
      SW->query(range, callback);
      SE->query(range, callback);
    } else {
      for (auto child : bucket) {
        if (range.contains(child)) {
          callback(child);
        }
      }
    }
  }

 public:
  void clear() {
    if (divided) {
      NW->clear();
      NE->clear();
      SW->clear();
      SE->clear();
    }
    bucket.clear();
    merge();
  }
};
}  // namespace forest
