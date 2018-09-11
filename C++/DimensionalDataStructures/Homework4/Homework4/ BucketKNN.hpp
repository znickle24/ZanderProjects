//
//   BucketKNN.hpp
//  Homework4
//
//  Created by Zander Nickle on 6/12/18.
//  Copyright © 2018 Zander Nickle. All rights reserved.
//

#ifndef _BucketKNN_h
#define _BucketKNN_h

#pragma once

#include "Point.hpp"
#include <vector>
#include <algorithm> 
template<int Dimension> //The dimension of the points.  This can be used like any other constant within.
class BucketKNN{
  
  
public:
  
  BucketKNN(const std::vector<Point<Dimension> >& points, int divisions_)
  {
    //generate an axis aligned bounding box with the dimensions passed in
    AABB<Dimension> boundingBox = getBounds(points);
    mins = boundingBox.mins;
    maxs = boundingBox.maxs;
    divisions = divisions_;
    buckets.resize(std::pow(divisions_, Dimension));
    numPoints = 0;
    for (int i = 0; i < Dimension; i++) {
      //makes all of the tiles the same size but defined in each dimension
      boxSize[i] = (maxs[i]-mins[i])/divisions_;
    }
    for (int i = 0; i < points.size(); i++) {
      std::array<int, Dimension> bucket = findBucket(points[i]);
      int index = indexForFlat(bucket);
      buckets[index].push_back(points[i]);
      numPoints++;
    }
    
  }
  std::array<int, Dimension>  findBucket(Point<Dimension> point) const {
    std::array<int, Dimension> ret;
    for (int i = 0; i < Dimension; i++) {
      int index = std::clamp<int>((point[i] - mins[i])/boxSize[i], 0, (divisions-1)); //clamp on this line, don't want to go out of bounds
      assert(index < divisions);
      ret[i] = index;
    }
    return ret;
  }
  //the way this is currently written, it would return only the last index. Need to adjust equation to match the fact this is already within a for loop.
  int indexForFlat(std::array<int, Dimension> coordinates) const {
    int index = 0;
//    for (int i: coordinates) {
//      std::cout << "Coordinates in indexForFlat: " << i << "\n";
//    }
    for(int dim = 0; dim < Dimension; dim++) {
      index += coordinates[dim] * std::pow(divisions,(Dimension-dim-1));
//      std::cout << "Dimension: " << dim << "\n";
//      std::cout << "Index: " << index << "\n";
    }
    return index;
  }
  //behave as if we didn't know in advance how many dimensions we're working with
  
  std::array<int, Dimension> nextBucket (std::array<int, Dimension>current, std::array<int, Dimension> min,
                            std::array<int, Dimension> max) const {
    int lastDimension = Dimension-1;
    current[lastDimension]++;
    for (int i = lastDimension; i > 0; i--) {
      //check to see if we've gone above the maximum for that dimension and carry if necessary
      if(current[i] > max[i]) {
        //reset dimension
        current[i] = min[i];
        //add to the next digit
        current[i-1]++;
      } else {
        //we're done with all the boxes
        break;
      }
    }
    return current;
  }
  std::vector<Point<Dimension> > rangeQuery(const Point<Dimension>& p, float radius) const{
    std::array<float, Dimension> minCoords;
    std::array<float, Dimension> maxCoords;
    std::vector<Point<Dimension>> ret;
    //get the minimums and maximums for each dimension given the point passed in
    for (int i = 0; i < Dimension; i++) {
      minCoords[i] = std::clamp(p[i]-radius, mins[i], mins[i]);
      maxCoords[i] = std::clamp(p[i]+radius, mins[i], maxs[i]);
    }
    //find out which buckets we'll need to traverse through starting with the min and ending with max
    std::array<int, Dimension> maxBucket = findBucket(Point<Dimension>{maxCoords});
//    for(int i: maxBucket) {
//      std::cout << "Max Coords in bucket: " << i << "\n";
//    }
    std::array<int, Dimension> minBucket = findBucket(Point<Dimension>{minCoords});
//    for(int i: minBucket) {
//      std::cout << "Max Coords in bucket: " << i << "\n";
//    }
    //create a starting point for the while loop
    auto coords = minBucket;
    //as long as we haven't hit the last bucket, traverse through each location and find its bucket/points
    while (coords != nextBucket(maxBucket, minBucket, maxBucket)) { // I don't believe this statement is working. index gets off
      //find the index to the bucket given the coordinates
      int index = indexForFlat(coords);
      assert(buckets.size() > index);
      std::vector<Point<Dimension>> bucket = buckets[index];
//      std::cout << "Coordinates 0, 1: " << coords[0] << coords[1] << "\n";
//      std::cout << "Max 0, 1: " << maxBucket[0] << maxBucket[1] << "\n";
      for (Point point: bucket) {
        //as long as we're within the radius, add the point to the vector of points
        if (distance(p, point) < radius) {
          ret.push_back(point);
        }
      }
      //move on to the next bucket and perform the same query
      coords = nextBucket(coords, minBucket, maxBucket);
    }
    return ret;
  }
  
  
  std::vector<Point<Dimension> > KNN(const Point<Dimension>& p, int k) const{
    int radius = 1;
    std::vector<Point<Dimension>> knn;
    //math to figure out radius
    for (int i = 0; i < Dimension; i++) {
      int temp = p[i];
      if (i == 0) {
        radius = p[i];
      } else if (temp < radius) {
        radius = temp;
      }
    }
    while (knn.size() < k) {
      knn = rangeQuery(p, radius);
      if (knn.size() < k) {
//        knn.clear();
        radius +=4;
      }
    }
    std::sort(knn.begin(), knn.end(), DistanceComparator(p));
    knn.resize(k);
    assert(numPoints >= k);
    return knn;
    
  }
  
private:
  int divisions, numPoints;
  std::array<float, Dimension> mins, maxs, boxSize;
  std::vector<std::vector<Point<Dimension>>> buckets;
};

#endif /* _BucketKNN_h */
