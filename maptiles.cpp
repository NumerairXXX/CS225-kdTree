/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */
	 			
#include <iostream>
#include <map>
#include "maptiles.h"

using namespace std;

MosaicCanvas * mapTiles(SourceImage const & theSource, vector<TileImage> const & theTiles){
	MosaicCanvas *ret = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
	vector<Point<3>> rgb;
	map<Point<3>, TileImage> amap;
	KDTree<3>::KDTree atree(rgb);
	for(size_t i = 0; i < theTiles.size(); i++){
		RGBAPixel pixel = theTiles[i].getAverageColor();
		Point<3> color(pixel.red,pixel.green,pixel.blue);
		rgb.push_back(color);
		amap[color] = theTiles[i];
	}
	for(int i = 0; i < theSource.getRows(); i++){
		for(int j = 0; j < theSource.getColumns(); j++){
			RGBAPixel region = theSource.getRegionColor(i,j);
			Point<3> regionpoint(region.red, region.green, region.blue);
			Point<3> closest = atree.findNearestNeighbor(regionpoint);
			TileImage fin = amap[closest];
			ret->setTile(i,j,fin);
		}
	}
return ret;
}
