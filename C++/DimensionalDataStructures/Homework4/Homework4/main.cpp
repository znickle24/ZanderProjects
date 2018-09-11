//
//  main.cpp
//  Homework4
//
//  Created by Zander Nickle on 6/12/18.
//  Copyright © 2018 Zander Nickle. All rights reserved.
//

#include <iostream>
#include " BucketKNN.hpp"
#include "Generators.hpp"
#include "KDTree.hpp"
#include "Point.hpp"
#include "Stopwatch.hpp"
#include "QuadTree.hpp"
#include <vector>
#include <fstream>


//int dimension = 2;
//std::vector<Point<2>> boxSet () {
//  std::vector<Point<2>> testSet;
//  std::vector<float> xy;
//  xy.push_back(0);
//  xy.push_back(0);
//  xy.push_back(1);
//  xy.push_back(1);
//  xy.push_back(-1);
//  xy.push_back(1);
//  xy.push_back(-1);
//  xy.push_back(-1);
//  xy.push_back(1);
//  xy.push_back(-1);
//  for (int i = 0; i < xy.size()-2; i+=2) {
//    std::array<float, 2> pointArr;
//    pointArr[i] = xy[i];
//    pointArr[i+1] = xy[i+1];
//    testSet.push_back(Point<2>{pointArr});
//  }
//  return testSet;
//}


int main(int argc, const char * argv[]) {
  int divisions = 4;
  std::vector<Point<2>> testVec;
  GaussianGenerator<2> test (2,2);
  for (int i =0; i < 15; i++) {
    Point<2> p = test.generatePoint();
//    std::cout << p[0] << " " << p[1] << "\n";
    testVec.push_back(p);
  }
  std::vector<Point<2>> kdTestVec = testVec;
  std::vector<Point<2>> qtTestVec = testVec;
  BucketKNN<2> buckets = BucketKNN<2>(testVec, divisions);
//  for (Point point : testVec) {
//    std::array<int, 2> findBucket = buckets.findBucket(point) ;
//    std::cout << "Bucket: \n" ;
//    for (float i : findBucket) {
//      std::cout << i << "\n";
//    }
//  }
  KDTree<2> kdtrees = KDTree<2>(kdTestVec);
  Point<2> testPoint = {4.02367, 7.60222};
  std::vector<Point<2>> pointsInRange = buckets.rangeQuery(testPoint, 10);
  std::vector<Point<2>> pointsInRangeKD;
  kdtrees.rangeQuery(testPoint, 10, pointsInRangeKD);
  //sorting these so that it's easier to compare the two
  QuadTree<2> quadTree = QuadTree<2>(qtTestVec, 5);
  std::vector<Point<2>> pointsInRangeQT;
  quadTree.rangeQuery(pointsInRangeQT, testPoint, 10);
  std::sort(pointsInRangeKD.begin(), pointsInRangeKD.end(), CompareBy<0>{});
  std::sort(pointsInRange.begin(), pointsInRange.end(), CompareBy<0>{});
  std::sort(pointsInRangeQT.begin(), pointsInRangeQT.end(), CompareBy<0>{});
  
  
  //using differential testing to make sure that I get the same results in the Bucket Vector and the KD-Tree
  for(int i = 0; i < pointsInRangeKD.size(); i++) {
    assert(pointsInRangeQT[i][0]==pointsInRange[i][0]);
    assert(pointsInRangeKD[i][0]==pointsInRange[i][0]);
    assert(pointsInRangeKD[i][1]==pointsInRange[i][1]);
    assert(pointsInRangeQT[i][1]==pointsInRange[i][1]);
  }
  std::vector<Point<2>> knnTestBuck;
  GaussianGenerator<2> knnTest (2,2);
  for (int i =0; i < 15; i++) {
    Point<2> point = knnTest.generatePoint();
    knnTestBuck.push_back(point);
  }
  int k = 5;
  Point<2> knnTestPoint = {knnTestBuck[0]};
  std::vector<Point<2>> knnTestKD = knnTestBuck;
  std::vector<Point<2>> knnTestQT = knnTestBuck;
  BucketKNN<2> knnBuck = BucketKNN<2>(knnTestBuck, divisions);
  KDTree<2> knnKDT = KDTree<2>(knnTestKD);
  QuadTree<2> knnQT = QuadTree<2>(knnTestQT, 5);
  std::vector<Point<2>> knnPointsBuck, knnPointsQT, knnPointsKD;
  knnPointsBuck = knnBuck.KNN(knnTestPoint, k);
  knnKDT.KNN(knnPointsKD, knnTestPoint, k);
  knnQT.KNN(knnPointsQT, knnTestPoint, k);
  std::sort(knnPointsBuck.begin(), knnPointsBuck.end(), CompareBy<0>{});
  std::sort(knnPointsKD.begin(), knnPointsKD.end(), CompareBy<0>{});
  std::sort(knnPointsQT.begin(), knnPointsQT.end(), CompareBy<0>{});
  for (int i = 0; i < knnPointsBuck.size(); i++) {
    std::cout << "Points in Bucket: " << knnPointsBuck[i] << "\n";
    std::cout << "Points in KD: " << knnPointsKD[i] << "\n";
    std::cout << "Points in QT: " << knnPointsQT[i] << "\n";
    assert(knnPointsBuck[i][0]==knnPointsKD[i][0]);
    assert(knnPointsBuck[i][0]==knnPointsKD[i][0]);
    assert(knnPointsQT[i][1]==knnPointsKD[i][1]);
    assert(knnPointsQT[i][1]==knnPointsKD[i][1]);
  }
  std::vector<std::string> bucketStrings, kdStrings, quadStrings;
  
  /*
   Format for dataframe will be Data Structure type, size of k, dimensions, number of points, followed by time it takes
   The only dimension for QT is 2, while the others will have a 2, 5, and 10 dimension version
   Tested on up to 200,000 points with k ranging from 1 to 15
   Each test is averaged over 100 trials
  */
  double timeSumBucket = 0.0;
  double timeSumKD = 0.0;
  double timeSumQT = 0.0;
  Stopwatch bucketWatch, kdWatch, qtWatch;
  UniformGenerator<2> gen3 (-10,10);
  //to be used with the 2 dimension version given we're doing all of the data structures
  //i is k number
  for (int i = 1; i < 10; i++) {
    //where j is the number of points in the distribution
    for (int j = 50; j <= 100000; j*=2) {
      TrialData<2> kd2 = getTrialData<2>(j, 100, gen3);
      TrialData<2> buck = getTrialData<2>(j, 100, gen3);
      TrialData<2> qt2 = getTrialData<2>(j, 100, gen3);
      BucketKNN<2> buckKnnTimingD2 = BucketKNN<2>(buck.testing, 4);
      KDTree<2> kdtKnnTimingD2 = KDTree<2>(kd2.testing);
      QuadTree<2> qtKnnTimingD2 = QuadTree<2>(qt2.testing, 50);
      //for averaging the testing points
      for (int t = 0; t < 10; t++) {
        std::vector<Point<2>> btvD2, kdtvD2, qttvD2;
        //generate a random point to test each of the data structures on.
        Point<2> testPoint = knnTest.generatePoint();
        bucketWatch.start();
        btvD2 = buckKnnTimingD2.KNN(testPoint, i);
        timeSumBucket += bucketWatch.stop();
        kdWatch.start();
        kdtKnnTimingD2.KNN(kdtvD2, testPoint, i);
        timeSumKD += kdWatch.stop();
        qtWatch.start();
        qtKnnTimingD2.KNN(qttvD2, testPoint, i);
        timeSumQT += qtWatch.stop();
      }
      timeSumBucket = timeSumBucket/10;
      std::string bString =  "Bucket," +std::to_string(i) + ",2," + std::to_string(j) + "," + std::to_string(timeSumBucket) + ",Uniform\n";
      bucketStrings.push_back(bString);
      timeSumKD = timeSumKD/10;
      std::string kdString =  "KDTree," +std::to_string(i) + ",2," + std::to_string(j) + "," + std::to_string(timeSumBucket) + ",Uniform\n";
      kdStrings.push_back(kdString);

      timeSumQT = timeSumQT/10;
      std::string qtString =  "QuadTree," +std::to_string(i) + ",2," + std::to_string(j) + "," + std::to_string(timeSumBucket) + ",Uniform\n";
      quadStrings.push_back(qtString);
    }
  }
  timeSumKD = 0;
  timeSumBucket = 0;
  UniformGenerator<5> gen1 (-10,10);
  for (int i = 1; i < 10; i++) {
    //where j is the number of points in the distribution
    for (int j = 50; j <= 100000; j*=2) {
      TrialData<5> kd5 = getTrialData<5>(j, 100, gen1);
      TrialData<5> buck5 = getTrialData<5>(j, 100, gen1);
      BucketKNN<5> buckKnnTimingD5 = BucketKNN<5>(buck5.testing, 4);
      KDTree<5> kdtKnnTimingD5 = KDTree<5>(kd5.testing);
      //for averaging the testing points
      for (int t = 0; t < 10; t++) {
        std::vector<Point<5>> btvD5, kdtvD5;
        //generate a random point to test each of the data structures on.
        Point<5> testPointD5 = gen1.generatePoint();
        bucketWatch.start();
        btvD5 = buckKnnTimingD5.KNN(testPointD5, i);
        timeSumBucket += bucketWatch.stop();
        kdWatch.start();
        kdtKnnTimingD5.KNN(kdtvD5, testPointD5, i);
        timeSumKD += kdWatch.stop();
      }
      timeSumBucket = timeSumBucket/10;
      std::string bString =  "Bucket," +std::to_string(i) + ",5," + std::to_string(j) + "," + std::to_string(timeSumBucket) + ",Uniform\n";
      bucketStrings.push_back(bString);
      timeSumKD = timeSumKD/10;
      std::string kdString =  "KDTree," +std::to_string(i) + ",5," + std::to_string(j) + "," + std::to_string(timeSumBucket) + ",Uniform\n";
      kdStrings.push_back(kdString);
    }
  }
  timeSumKD = 0;
  timeSumBucket = 0;
  UniformGenerator<10> gen2 (-10,10);
  for (int i = 1; i < 10; i++) {
    //where j is the number of points in the distribution
    for (int j = 50; j <= 100000; j*=2) {
      TrialData<10> kd20 = getTrialData<10>(j, 100, gen2);
      TrialData<10> buck20 = getTrialData<10>(j, 100, gen2);
      BucketKNN<10> buckKnnTimingD20 = BucketKNN<10>(buck20.testing, 4);
      KDTree<10> kdtKnnTimingD20 = KDTree<10>(kd20.testing);
      //for averaging the testing points
      for (int t = 0; t < 10; t++) {
        std::vector<Point<10>> btvD20, kdtvD20;
        //generate a random point to test each of the data structures on.
        Point<10> testPointD20 = gen2.generatePoint();
        bucketWatch.start();
        btvD20 = buckKnnTimingD20.KNN(testPointD20, i);
        timeSumBucket += bucketWatch.stop();
        kdWatch.start();
        kdtKnnTimingD20.KNN(kdtvD20, testPointD20, i);
        timeSumKD += kdWatch.stop();
      }
      timeSumBucket = timeSumBucket/10;
      std::string bString =  "Bucket," +std::to_string(i) + ",10," + std::to_string(j) + "," + std::to_string(timeSumBucket) + ",Uniform\n";
      bucketStrings.push_back(bString);
      timeSumKD = timeSumKD/10;
      std::string kdString =  "KDTree," +std::to_string(i) + ",10," + std::to_string(j) + "," + std::to_string(timeSumBucket) + ",Uniform\n";
      kdStrings.push_back(kdString);
    }
  }
  std::ofstream output1;
  output1.open("/Users/znickle/znickle/DataAnalysesAndVisualization/Homework/Homework4/Homework4/Homework4/Buckets1.csv");
  output1 << "DataType,KNN,Dimensions,NumPoints,Time,Distribution\n";
  for (std::string s: bucketStrings) {
    output1 << s;
  }
  output1.flush();
  output1.close();

  std::ofstream output2;
  output2.open("/Users/znickle/znickle/DataAnalysesAndVisualization/Homework/Homework4/Homework4/Homework4/KD1.csv");
  output2 << "DataType,KNN,Dimensions,NumPoints,Time,Distribution\n";
  for (std::string s: kdStrings) {
    output2 << s;
  }
  output2.flush();
  output2.close();

  std::ofstream output3;
  output3.open("/Users/znickle/znickle/DataAnalysesAndVisualization/Homework/Homework4/Homework4/Homework4/QT1.csv");
  output3 << "DataType,KNN,Dimensions,NumPoints,Time,Distribution\n";
  for (std::string s: quadStrings) {
    output3 << s;
  }
  output3.flush();
  output3.close();
//  output1.flush();
//  output1.close();
//  output2.flush();
//  output2.close();
//  output3.flush();
//  output3.close();
  return 0;
}
