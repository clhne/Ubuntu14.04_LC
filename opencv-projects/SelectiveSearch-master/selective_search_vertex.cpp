#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc/segmentation.hpp>
using namespace std;
using namespace cv;
using namespace cv::ximgproc::segmentation;

#include "selective_search.h"

SelectiveSearch::Vertex::Vertex(const int _index, SelectiveSearch * ss): is_alive(true), size(0), index(_index), base0(-1), base1(-1), top_left(INT_MAX, INT_MAX), bottom_right(0, 0), ss(ss){
}

SelectiveSearch::Vertex::~Vertex(){  
}

void SelectiveSearch::Vertex::init(){
	DMsg dmsg("init");
  calcColHist();
  calcTexHist();
  size = (int)gs_pts.size();
  calcRegion();
  gs_pts_set.push_back(&gs_pts);
}

void SelectiveSearch::Vertex::calcColHist(){
	DMsg dmsg("calcColHist");
  col_hist.resize(3);
  for(int i = 0; i < col_hist.size(); ++i)
    col_hist[i].resize(25, 0.f);

  float istep = 25.f / 256.f;
  for(int i = 0; i < gs_pts.size(); ++i){
    for(int j = 0; j < 3; ++j){
      const uchar * p = ss->planes[j].ptr<uchar>(0);
      int bin = saturate_cast<int>(floor(p[gs_pts[i]] * istep));
      if(!(bin < 25)){
	cout << "bin : " <<  bin << endl;
	exit(1);
      }
      col_hist[j][bin] += 1.0;
    }
  }

  //normalize color histgram
  float isize = 1.f/(float)(gs_pts.size()*3.f);
  for(int i = 0; i < col_hist.size(); ++i){
    for(int j = 0; j < col_hist[i].size(); ++j){
      col_hist[i][j] *= isize;
      CV_Assert(col_hist[i][j] <= 1.0);
    }
  }
}

void SelectiveSearch::Vertex::calcTexHist()
{
	DMsg dmsg("calcTexHist");
  tex_hist.resize(3);
  for(int i = 0; i < tex_hist.size(); ++i){
    tex_hist[i].resize(8);
    for(int j = 0; j < tex_hist[i].size(); ++j){
      tex_hist[i][j].resize(10, 0.f);
    }
  }

  const float max_grad = ss->max_grad;
  const float min_grad = ss->min_grad; 
  const float istep_grad = 10.f / (max_grad - min_grad + 0.05f);
  const float istep_ori = (8.f ) / (360.f+0.05f);
  for(int i = 0; i < gs_pts.size(); ++i){
    int pt = gs_pts[i];
    
    for(int j = 0; j < 3; ++j){
      const float * pori_img = ss->ori_imgs[j].ptr<float>(0);
      const float * pgrad_img = ss->grad_imgs[j].ptr<float>(0);

      int ori_bin = (int)floor(pori_img[pt] * istep_ori);
      if(!(ori_bin < 8 && ori_bin >= 0)){
	cout << "ori_bin : " << ori_bin << endl;
	cout << "pt : " << pt << endl;
	cout << "pori_img[pt] : " << pori_img[pt] << endl;
	cout << "iste_ori : " << istep_ori << endl;
	exit(1);
      }
      
      int grad_bin = (int)floor((pgrad_img[pt] - min_grad) * istep_grad);
      if(!(grad_bin < 10 && grad_bin >= 0)){
	cout << "grad_bin : " << grad_bin << endl;
	cout << "grad : " << pgrad_img[pt] << endl;
	cout << "max : " << max_grad << endl;
	cout << "min : " << min_grad << endl;
	exit(1);
      }

      tex_hist[j][ori_bin][grad_bin] += 1.0;
    }
  }
 
  float isize = 1.f/((float)gs_pts.size()*3);
  for(int i = 0; i < tex_hist.size(); ++i){
    for(int j = 0; j < tex_hist[i].size(); ++j){
      for(int k = 0; k < tex_hist[i][j].size(); ++k){
	tex_hist[i][j][k] *= isize;
	CV_Assert(tex_hist[i][j][k] <= 1.f);
      }
    }
  }
  CV_Assert(tex_hist.size() < 100000);
}

void SelectiveSearch::Vertex::calcRegion(){
	DMsg dmsg("calcRegion");
  region = Rect(top_left, Point(bottom_right.x + 1, bottom_right.y + 1));
}
