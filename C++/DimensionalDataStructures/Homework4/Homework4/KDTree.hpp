//
//  KDTree.hpp
//  Homework4
//
//  Created by Zander Nickle on 6/12/18.
//  Copyright © 2018 Zander Nickle. All rights reserved.
//

#ifndef KDTree_h
#define KDTree_h

#pragma once

#include "Point.hpp"
#include <memory>
#include <queue>


template<int Dimension>
class KDTree {
public:
  
  KDTree(const std::vector<Point<Dimension> >& points){
    std::vector<Point<Dimension>> pointCopy = points;
    root = std::unique_ptr<Node<0>>(new Node<0>(pointCopy.begin(), pointCopy.end()));
  }
  void KNN(std::vector<Point<Dimension>> & knn, Point<Dimension> p, int k) {
//    knn.push_back(p); 
    AABB<Dimension> box;
    KNNRecurse(knn, root, p, k, box);
  }
  void rangeQuery(Point<Dimension> p, int r, std::vector<Point<Dimension>> &vec) {
    rangeRecurse(root, p, r, vec);
  }
  
private:
  
  template<int SplitDimension> //Don't store the split dimension explicitly
  struct Node{
    Point<Dimension> p;
    //The children will have the "next" splitting dimension
    //Since this is part of the type, we can't "forget" to set it properly... nice!
    std::unique_ptr<Node<(SplitDimension + 1)%Dimension>> left, right;
    /*
     We'll use iterators to describe the chunk of our points array that belong to this node/subtree
     */
    template<typename Iter>
    Node(Iter begin, Iter end)
    {
      //Our children (if we have any) will use the "next" splitting dimension
      using ChildType = Node<(SplitDimension +1)%Dimension>;
      auto median = begin+((end-begin)/2);
      std::nth_element(begin, median, end, CompareBy<SplitDimension>{});
      p = *median;
      if (begin != median) {
        left = std::unique_ptr<ChildType>(new ChildType(begin, median));
      }
      if ((median +1) != end) {
        right = std::unique_ptr<ChildType>(new ChildType((median +1), end));
      }
    }
  };
  
  template <int SplitDimension>
  void KNNRecurse(std::vector<Point<Dimension>> & knn, std::unique_ptr<Node<SplitDimension>> &n, Point<Dimension> p, int k, AABB<Dimension>& box) {
    if (knn.size() < k) {
      knn.push_back(n->p);
      std::push_heap(knn.begin(), knn.end(), DistanceComparator<Dimension>{p});
    } else if (distance(n->p, p) < distance(knn.front(), p)){
      std::pop_heap(knn.begin(), knn.end(), DistanceComparator<Dimension>{p});
      knn.pop_back();
      knn.push_back(n->p);
      std::push_heap(knn.begin(), knn.end(), DistanceComparator<Dimension>{p});
    }
    if (n->left) {
      AABB<Dimension> leftBox = box;
      leftBox.maxs[SplitDimension] = n->p[SplitDimension];
      if (knn.size() < k) {
        KNNRecurse(knn, n->left, p, k, leftBox);
      } else if (distance(leftBox.closestInBox(p), p) < distance(knn.front(), p)) {
        KNNRecurse(knn, n->left, p, k, leftBox);
      }
    }
    if (n->right) {
      AABB<Dimension> rightBox = box;
      rightBox.mins[SplitDimension] = n->p[SplitDimension];
      if (knn.size() < k) {
        KNNRecurse(knn, n->right, p, k, rightBox);
      } else if (distance(rightBox.closestInBox(p), p) < distance(knn.front(), p)) {
        KNNRecurse(knn, n->right, p, k, rightBox);
      }
    }
  }
  template<int SplitDimension>
  bool goLeft(Point<Dimension> p, int r) {
    if (p[SplitDimension] >= p[SplitDimension] - r) {
      return true;
    } else {
      return false;
    }
  }
  template<int SplitDimension>
  bool goRight(Point<Dimension> p, int r) {
    if (p[SplitDimension] <= p[SplitDimension] + r) {
      return true;
    } else {
      return false;
    }
  }
  
  template<int SplitDimension>
  void rangeRecurse(const std::unique_ptr<Node<SplitDimension>> & n, Point<Dimension> &p, int r, std::vector<Point<Dimension>> & vec) {
    if (distance(n->p, p) <= r) {
      vec.push_back(n->p);
    }
    if (n->left) {
      if (goLeft<SplitDimension>(n->p, r)) {
        rangeRecurse(n->left, p, r, vec);
      }
    }
    if (n->right) {
      if (goRight<SplitDimension>(n->p, r)) {
        rangeRecurse(n->right, p, r, vec);
      }
    }
  }
  std::unique_ptr<Node<0>> root;
};


#endif /* KDTree_h */
