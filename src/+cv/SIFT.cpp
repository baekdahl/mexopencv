/**
 * @file SIFT.cpp
 * @brief mex interface for SIFT
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
		plhs[0] = MxArray(SIFT().descriptorSize());
		return;
	}
	
	// Option processing
	int _nOctaves = SIFT::CommonParams::DEFAULT_NOCTAVES;
	int _nOctaveLayers = SIFT::CommonParams::DEFAULT_NOCTAVE_LAYERS;
	int _firstOctave = SIFT::CommonParams::DEFAULT_FIRST_OCTAVE;
	int _angleMode = SIFT::CommonParams::FIRST_ANGLE;
	double _threshold = SIFT::DetectorParams::GET_DEFAULT_THRESHOLD();
	double _edgeThreshold = SIFT::DetectorParams::GET_DEFAULT_EDGE_THRESHOLD();
	double _magnification = SIFT::DescriptorParams::GET_DEFAULT_MAGNIFICATION();
	bool _isNormalize = SIFT::DescriptorParams::DEFAULT_IS_NORMALIZE;
	bool _recalculateAngles = true;
	Mat mask;
	for (int i=1; i<nrhs; i+=2) {
		string key = rhs[i].toString();
		if (key=="NOctaves")
			_nOctaves = rhs[i+1].toInt();
		else if (key=="NOctaveLayers")
			_nOctaveLayers = rhs[i+1].toInt();
		else if (key=="FirstOctave")
			_firstOctave = rhs[i+1].toInt();
		else if (key=="AngleMode")
			_angleMode = rhs[i+1].toInt();
		else if (key=="Threshold")
			_threshold = rhs[i+1].toDouble();
		else if (key=="EdgeThreshold")
			_edgeThreshold = rhs[i+1].toDouble();
		else if (key=="Magnification")
			_magnification = rhs[i+1].toDouble();
		else if (key=="IsNormalize")
			_isNormalize = rhs[i+1].toBool();
		else if (key=="RecalculateAngles")
			_recalculateAngles = rhs[i+1].toBool();
		else if (key=="Mask")
			mask = rhs[i+1].toMat(CV_8U);
		else
			mexErrMsgIdAndTxt("mexopencv:error","Unrecognized option");
	}
	
	// Process
	SIFT sift(
		SIFT::CommonParams(_nOctaves,_nOctaveLayers,_firstOctave,_angleMode),
		SIFT::DetectorParams(_threshold,_edgeThreshold),
		SIFT::DescriptorParams(_magnification,_isNormalize,_recalculateAngles)
	);
	Mat image(rhs[0].toMat());
	vector<KeyPoint> keypoints;
	bool useProvidedKeypoints=false;
	if (nlhs>1) {
		Mat descriptors;
		sift(image, mask, keypoints, descriptors, useProvidedKeypoints);
		plhs[1] = MxArray(descriptors);
	}
	else
		sift(image, mask, keypoints);
	plhs[0] = MxArray(keypoints);
#else
	mexErrMsgIdAndTxt("mexopencv:error","SIFT not supported in this version");
#endif
}
