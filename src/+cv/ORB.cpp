/**
 * @file ORB.cpp
 * @brief mex interface for ORB
 * @author Kota Yamaguchi
 * @date 2011
 */
#include "mexopencv.hpp"
using namespace std;
using namespace cv;

/**
 * Main entry called from Matlab
 * @param nlhs number of left-hand-side arguments
 * @param plhs pointers to mxArrays in the left-hand-side
 * @param nrhs number of right-hand-side arguments
 * @param prhs pointers to mxArrays in the right-hand-side
 */
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] )
{
#if CV_MINOR_VERSION >= 2
	// Check the number of arguments
	if (nrhs<1 || ((nrhs%2)!=1) || nlhs>2)
        mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    
	// Argument vector
	vector<MxArray> rhs(prhs,prhs+nrhs);
	
	if (nrhs==1 && rhs[0].isChar() && rhs[0].toString()=="DescriptorSize") {
		plhs[0] = MxArray(ORB().descriptorSize());
		return;
	}
	
	// Option processing
	size_t n_features = 500;
	float scale_factor = 1.2f;
	unsigned int n_levels = ORB::CommonParams::DEFAULT_N_LEVELS;
    int edge_threshold = 31;
    unsigned int first_level = ORB::CommonParams::DEFAULT_FIRST_LEVEL;
	Mat mask;
	for (int i=1; i<nrhs; i+=2) {
		string key = rhs[i].toString();
		if (key=="NFeatures")
			n_features = rhs[i+1].toInt();
		else if (key=="ScaleFactor")
			scale_factor = rhs[i+1].toDouble();
		else if (key=="NLevels")
			n_levels = rhs[i+1].toInt();
		else if (key=="EdgeThreshold")
			edge_threshold = rhs[i+1].toInt();
		else if (key=="FirstLevel")
			first_level = rhs[i+1].toInt();
		else if (key=="Mask")
			mask = rhs[i+1].toMat(CV_8U);
		else
			mexErrMsgIdAndTxt("mexopencv:error","Unrecognized option");
	}
	
	// Process
	ORB orb(
		n_features,
		ORB::CommonParams(scale_factor,n_levels,edge_threshold,first_level)
	);
	Mat image(rhs[0].toMat());
	vector<KeyPoint> keypoints;
	bool useProvidedKeypoints=false;
	if (nlhs>1) {
		Mat descriptors;
		orb(image, mask, keypoints, descriptors, useProvidedKeypoints);
		plhs[1] = MxArray(descriptors);
	}
	else
		orb(image, mask, keypoints);
	plhs[0] = MxArray(keypoints);
#else
	mexErrMsgIdAndTxt("mexopencv:error","ORB not supported in this version");
#endif
}
