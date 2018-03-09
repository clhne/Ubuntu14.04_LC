#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <list>

#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc/segmentation.hpp>
using namespace std;
using namespace cv;
using namespace cv::ximgproc::segmentation;

#include "selective_search.h"

SelectiveSearch::Edge::Edge(SelectiveSearch * ss): sim(0), col_sim(0), tex_sim(0), size_sim(0), fill_sim(0), ss(ss){

}

void SelectiveSearch::Edge::calcTexSim(){
  //DMsg dmsg(__PRETTY_FUNCTION__);
  TexHist &h0 = from->tex_hist;
  TexHist &h1 = to->tex_hist;
  CV_Assert(h0 != h1);
  tex_sim = 0;
  for(int i = 0; i < h0.size(); ++i){
    for(int j = 0; j < h0[i].size(); ++j){
      for(int k = 0; k < h0[i][j].size(); ++k){
	tex_sim += min(h0[i][j][k], h1[i][j][k]);
      }
    }
  }
  CV_Assert(tex_sim <= 1);
}

void SelectiveSearch::Edge::calcFillSim(){
  //DMsg dmsg(__PRETTY_FUNCTION__);
  Rect mrect = from->region | to->region;
  fill_sim = 1 - (mrect.area() - from->size - to->size)/(double)ss->planes[0].total();
  
  if(fill_sim > 1){
    cout << "fill_sim " << fill_sim << endl;
    cout << "from->size " << from->size << endl;
    cout << "to->size " << to->size << endl;
    cout << "mrect.area " << mrect.area() << endl;
    cout << "ss->planes[0].total() " << ss->planes[0].total() << endl;
    exit(1);
  }
}

void SelectiveSearch::Edge::calcSizeSim(){
  //DMsg dmsg(__PRETTY_FUNCTION__);
  CV_Assert(from->size != 0 || to->size != 0);
  size_sim = 1 - (from->size + to->size)/(float)ss->planes[0].total();
  CV_Assert(size_sim <= 1);
}

void SelectiveSearch::Edge::calcColSim(){
  //DMsg dmsg(__PRETTY_FUNCTION__);
  ColHist &h0 = from->col_hist;
  ColHist &h1 = to->col_hist;

  col_sim = 0;
  for(int i = 0; i < h0.size(); ++i){
    for(int j = 0; j < h0[i].size(); ++j){
      col_sim += min(h0[i][j], h1[i][j]);
    }
  }
  
  CV_Assert(col_sim <= 1);
};

void SelectiveSearch::Edge::calcSim(){
  //DMsg dmsg(__PRETTY_FUNCTION__);
  calcTexSim();
  calcFillSim();
  calcSizeSim();
  calcColSim();

  sim = ss->tex_sim_weight * tex_sim + ss->fill_sim_weight * fill_sim
    + ss->size_sim_weight * size_sim + ss->col_sim_weight * col_sim;
}
