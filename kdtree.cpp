/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include<math.h>
#include <algorithm>
using namespace std;

template<int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim> & first, const Point<Dim> & second, int curDim) const
{
	if(curDim < 0 || curDim > Dim)
		return false;
	if(first[curDim] == second[curDim])
		return first < second;
	return first[curDim] < second[curDim];
}


template<int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim> & target, const Point<Dim> & currentBest, const Point<Dim> & potential) const
{
	int po_dis = 0;
	int cur_dis = 0;
	for(int i =0; i < Dim; i++){
		po_dis += pow(potential[i]-target[i],2);
		cur_dis += pow(currentBest[i]-target[i],2);
	}
	if (po_dis == cur_dis)
		return potential < currentBest;
	else
		return po_dis < cur_dis;
}

template<int Dim>
KDTree<Dim>::KDTree(const vector< Point<Dim> > & newPoints)
{
	if(newPoints.size()==0)
		return;
//	points = newPoints;
//	int newsize = newPoints.size();
	for(size_t i=0; i<newPoints.size(); i++){
		points.push_back(newPoints[i]);
	}
	build(0, newPoints.size()-1, 0);
}

template<int Dim>
void KDTree<Dim>::build(int left, int right, int dimension){
	if(left >= right)
		return;
	int median = (left + right) / 2;
	select(left, right, median, dimension);
	build(left, median-1, (dimension+1)%Dim);
	build(median+1, right, (dimension+1)%Dim);
}

template<int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim> & query) const
{
    /**
     * @todo Implement this function!
     */
	Point<Dim> ret = findhelper(query,points[(points.size()-1)/2], 0, points.size()-1,  0);
	return ret;
}

template<int Dim>
Point<Dim> KDTree<Dim>::findhelper(const Point<Dim> & query, const Point<Dim> & target, int left, int right,int dimension) const
{
	Point<Dim> ret=target;
	if(left>=right){
       		if(shouldReplace(query, target, points[left])){
			return points[left];
        	}
        	return ret;
    	}
	int median = (left + right)/2;
	if(smallerDimVal(points[median], query, dimension))
        	ret=findhelper(query, target,median+1, right, (dimension+1)%Dim);
    	else
        	ret=findhelper(query, target,left, median-1, (dimension+1)%Dim);
	
	if(shouldReplace(query, ret, points[median]))
        	ret=points[median];

   	Point<Dim> a=points[median];
	int d = 0;
	for(int i =0; i < Dim; i++){
		d += pow(query[i]-ret[i],2);
	}

	if((a[dimension] - query[dimension])*(a[dimension] - query[dimension]) <= d){
       		if(smallerDimVal(points[median], query, dimension))
            		ret=findhelper(query, ret, left, median - 1, (dimension+1)%Dim);
        	else
            		ret=findhelper(query, ret, median + 1, right, (dimension+1)%Dim);
   	 }
	return ret;
}

/*quick select area based on wikipedia*/

template<int Dim>
void KDTree<Dim>::swap(int index1, int index2){
	auto temp = points[index1];
	points[index1] = points[index2];
	points[index2] = temp;
}

template<int Dim>
int KDTree<Dim>::partition(int left, int right, int dimension, int pivotIndex){
	auto pivotValue = points[pivotIndex];
	swap(pivotIndex, right);
	int storeIndex = left;
	for(int i=left; i < right; i++){
		if(smallerDimVal(points[i] , pivotValue, dimension)){
			swap(storeIndex,i);
			storeIndex += 1;
		}
	}
	swap(right,storeIndex);
	return storeIndex;
}

template<int Dim>
void KDTree<Dim>::select(int left, int right, int k, int dimension){
	if(left >= right)
		return;
	int pivotIndex = (left + right)/2;
	int newIndex = partition(left, right, dimension, pivotIndex);
	if(k==newIndex)
		return;
	else if(k < newIndex)
		select(left, newIndex - 1, k, dimension);
	else
		select(newIndex + 1, right, k, dimension);
}


