// #include <boost/python.hpp>
// #include "boost/python/extract.hpp"
// #include "boost/python/numeric.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>


// #include <numpy/ndarrayobject.h>
#include <iostream>
#include <string>
#include <stdlib.h>

#include <MERCLazyGreedy.h>
#include <MERCInputImage.h>
#include <MERCOutputImage.h>
#include <Image.h>
#include <ImageIO.h>

namespace py = pybind11;
using namespace std;
class ErsPy {
public:
	ErsPy() { lambda_ = 0.5; sigma_ = 5.0; }
	double lambda_;
	double sigma_;
	void SetLambda(double lambda) { lambda_ = lambda; };
	void SetSigma(double sigma) { sigma_ = sigma; };
	py::tuple ComputeSegmentation(py::array &inputImg, int nC) {
		py::array<double>* pyImg = (PyArrayObject*)PyArray_FROM_O(inputImg.ptr());
		int rgbFlag = (pyImg->nd == 3) ? 1 : 0;
		//std::cout << plim->nd;
		int height = *(pyImg->dimensions);
		int width  = *(pyImg->dimensions + 1);
		int conn8 = 1;
		int kernel = 0;
		uchar* imgData = (uchar *)pyImg->data;
		Image<uchar> inputImage;
		MERCInputImage<uchar> input;
		// Create Iamge
		inputImage.Resize(width, height, false);
		// Read the image from MATLAB
		for (int col = 0; col < width; col++)
			for (int row = 0; row < height; row++)
				inputImage.Access(col, row) = imgData[row*width + col];
		// Read the image for segmentation
		input.ReadImage(&inputImage, conn8);
		// Entropy rate superpixel segmentation
		MERCLazyGreedy merc;
		merc.ClusteringTreeIF(input.nNodes_, input, kernel, sigma_, lambda_*1.0*nC, nC);
		vector<int> label = MERCOutputImage::DisjointSetToLabel(merc.disjointSet_);
		float *out = new float[height*width];
		for (int col = 0; col < width; col++)
			for (int row = 0; row < height; row++)
				out[col + row*width] = (float)label[col + row*width];
		int ndims = 2;
		ssize_t dims[2];
		//dims[0] = height; dims[1] = width; dims[2] = nDisp;
		dims[0] = height; dims[1] = width;
		PyObject * pyObj = PyArray_SimpleNewFromData(ndims, dims, NPY_FLOAT, out);
		py::handle handle(pyObj);
		py::array arr(handle);
		delete[] out;
		return py::make_tuple(arr.copy());
	}
};



// BOOST_PYTHON_MODULE(erspy)
// {
// 	boost::python::numeric::array::set_module_and_type("numpy", "ndarray");
// 	import_array();
// 	class_<ErsPy>("ers")
// 		.def("ComputeSegmentation", &ErsPy::ComputeSegmentation)
// 		.def("SetSigma", &ErsPy::SetSigma)
// 		.def("SetLambda", &ErsPy::SetLambda)
// 		;
// }

PYBIND11_MODULE(erspy, m) {
	// boost::python::numeric::array::set_module_and_type("numpy", "ndarray");
	// import_array();
  
    py::class_<ErsPy>(m, "ers")
        .def("ComputeSegmentation", &ErsPy::ComputeSegmentation)
		.def("SetSigma", &ErsPy::SetSigma)
		.def("SetLambda", &ErsPy::SetLambda)
        ;
}
