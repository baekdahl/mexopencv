/**
 * @file SVD_compute_.cpp
 * @brief mex interface for SVD_compute_
 * @author Kota Yamaguchi
 * @date 2012
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
	// Check the number of arguments
	if (nrhs<1 || ((nrhs%2)!=1) || nlhs>3)
        mexErrMsgIdAndTxt("mexopencv:error","Wrong number of arguments");
    
	// Argument vector
	vector<MxArray> rhs(prhs,prhs+nrhs);
	Mat A(rhs[0].toMat());
	bool modifyA = true;
	bool noUV = false;
	bool fullUV = false;
	for (int i=1; i<nrhs; i+=2) {
		string key(rhs[i].toString());
		if (key=="ModifyA")
			modifyA = rhs[i+1].toBool();
		else if (key=="NoUV")
			noUV = rhs[i+1].toBool();
		else if (key=="FullUV")
			fullUV = rhs[i+1].toBool();
		else
			mexErrMsgIdAndTxt("mexopencv:error","Unrecognized option");
	}
	int flags = (modifyA) ? SVD::MODIFY_A : 0|
				(noUV) ?    SVD::NO_UV :    0|
				(fullUV) ?  SVD::FULL_UV :  0;
	SVD svd(A, flags);
	plhs[0] = MxArray(svd.w);
	if (nlhs>1)
		plhs[1] = MxArray(svd.u);
	if (nlhs>2)
		plhs[2] = MxArray(svd.vt);
}
