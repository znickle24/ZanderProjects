//
//  QuadTree.hpp
//  Homework4
//
//  Created by Zander Nickle on 6/20/18.
//  Copyright © 2018 Zander Nickle. All rights reserved.
//

#ifndef QuadTree_h
#define QuadTree_h

#include "Point.hpp"
#include <memory>
#include <vector>
#include <queue>
#include <algorithm>


template<int Dimension>
class QuadTree {
public:
  QuadTree(const std::vector<Point<2> >& points, int maxPointsPerNode) {
    box = getBounds(points);
    std::vector<Point<2>> pointCopy = points;
    maxPoints = maxPointsPerNode;
    root = std::unique_ptr<Node>(new Node(pointCopy, maxPoints, box));
  }
  void rangeQuery(std::vector<Point<2>> & vec, Point<2> point, int r) {
    rangeQueryRecurse(vec, point, root, r);
  }
  void KNN(std::vector<Point<Dimension>> & knn, Point<Dimension> p, int k) {
    //    knn.push_back(p);
    KNNRecurse(knn, root, p, k);
  }
private:

  struct Node{
    float xmid, ymid;
    AABB<2> nwBox, neBox, swBox, seBox;
    bool isLeaf = false;
    //once we find out we are at a leaf, add the proper points to this vector
    std::vector<Point<2>> pointList;
    //access outside of Node struct as node->children[Node::NW];
    std::unique_ptr<Node> NW;
    std::unique_ptr<Node> NE;
    std::unique_ptr<Node> SW;
    std::unique_ptr<Node> SE;
    std::vector<Point<2>> NorthWest;
    std::vector<Point<2>> NorthEast;
    std::vector<Point<2>> SouthWest;
    std::vector<Point<2>> SouthEast;
    Node(std::vector<Point<2>> &points, int &maxPoints, AABB<2> box)
    {
      if (points.size() <= maxPoints) {
        isLeaf = true;
        pointList = points;
        return;
      }
      xmid = ((box.maxs[0] + box.mins[0])/2);
      ymid = ((box.maxs[1] + box.mins[1])/2);
      //find each quadrant and set them in the array of children starting with the NW and ending with the SE
      //base case is when the quadrant is a leaf, meaning it has fewer than MaxPoints # of points in the quadrant each max should be used exactly twice and each min should be used exactly twice to meet the critera
      for (Point p: points) {
        if (p[0] < xmid) { //go west
          if (p[1] > ymid) { //go north
            NorthWest.push_back(p);
          } else {
            SouthWest.push_back(p);
          }
        }
        if (p[0] > xmid) { //go east
          if (p[1] > ymid){ // go north
            NorthEast.push_back(p);
          } else {
            SouthEast.push_back(p);
          }
        }
      }
   
      nwBox.maxs[0] = xmid;
      nwBox.mins[1] = ymid;

      neBox.mins[0] = xmid;
      neBox.mins[1] = ymid;
      
      swBox.maxs[0] = xmid;
      swBox.maxs[1] = ymid;

      seBox.mins[0] = xmid;
      seBox.maxs[1] = ymid;
      
      NW = std::unique_ptr<Node>(new Node(NorthWest, maxPoints, nwBox));
      NE = std::unique_ptr<Node>(new Node(NorthEast, maxPoints, neBox));
      SW = std::unique_ptr<Node>(new Node(SouthWest, maxPoints, swBox));
      SE = std::unique_ptr<Node>(new Node(SouthEast, maxPoints, seBox));
    }
  };
  
  void rangeQueryRecurse(std::vector<Point<2>> &vec, Point<2> point, std::unique_ptr<Node> &node, int r) {
    //once at a leaf node, just need to check each of the points and see if they should be added to the vector passed in.
    if (node->isLeaf) {
      for (Point p: node->pointList) {
        if (distance(p, point) <= r) {
          vec.push_back(p);
        }
      }
      return;
    }
    //each node has 4 member variables that are the 4 quadrants. Need to see if the point is within that box and if it's possible for any point in that box to be within range. If so, recurse down that quadrant.
    if (node->NorthWest.size() > 0){
      AABB<2> nwBox = getBounds(node->NorthWest);
      if (distance(nwBox.closestInBox(point), point) <= r){
        rangeQueryRecurse(vec, point, node->NW, r);
      }
    }
    if (node->NorthEast.size() > 0) {
      AABB<2> neBox = getBounds(node->NorthEast);
      if (distance(neBox.closestInBox(point), point) <= r) {
        rangeQueryRecurse(vec, point, node->NE, r);
      }
    }
    if (node->SouthWest.size() > 0) {
      AABB<2> swBox = getBounds(node->SouthWest);
      if (distance(swBox.closestInBox(point), point) <= r) {
        rangeQueryRecurse(vec, point, node->SW, r);
      }
    }
    if (node-> SouthEast.size() > 0) {
      AABB<2> seBox = getBounds(node->SouthEast);
      if (distance(seBox.closestInBox(point), point) <= r) {
        rangeQueryRecurse(vec, point, node->SE, r);
      }
    }
  }
  
  void KNNRecurse(std::vector<Point<2>> &knn,const std::unique_ptr<Node> &n, Point<2> p, int k) {
    if (n->isLeaf) {
      for (Point<2> point: n->pointList) {
        if (knn.size() < k) {
          knn.push_back(point);
          std::push_heap(knn.begin(), knn.end(), DistanceComparator<2>{p});
        } else if (distance(point, p) < distance(knn.front(), p)){
          std::pop_heap(knn.begin(), knn.end(), DistanceComparator<2>{p});
          knn.pop_back();
          knn.push_back(point);
          std::push_heap(knn.begin(), knn.end(), DistanceComparator<2>{p});
        }
      }
    }
    
    if (n->NW) {
      AABB<2> nwBox = n->nwBox;
      if(knn.size() < k || distance(nwBox.closestInBox(p), p) < distance(knn.front(), p)) {
        KNNRecurse(knn, n->NW, p, k);
      }
    }
    if (n->NE) {
      AABB<2> neBox = n->neBox;
      if(knn.size() < k || distance(neBox.closestInBox(p), p) < distance(knn.front(), p)) {
        KNNRecurse(knn, n->NE, p, k);
      }
    }
    if (n->SW) {
      AABB<2> swBox = n->swBox;
      if(knn.size() < k || distance(swBox.closestInBox(p), p) < distance(knn.front(), p)) {
        KNNRecurse(knn, n->SW, p, k);
      }
    }
    if (n->SE) {
      AABB<2> seBox = n->seBox;
      if(knn.size() < k || distance(seBox.closestInBox(p), p) < distance(knn.front(), p)) {
        KNNRecurse(knn, n->SE, p, k);
      }
    }
  }
  std::unique_ptr<Node> root;
  AABB<2> box;
  int maxPoints;
};

#endif /* QuadTree_h */
