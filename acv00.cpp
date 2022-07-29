#include "acv00.h"

void hw00::performHistEqualization(std::string inputImagePath,
		std::string outputFilePath) {
	DebugManager::writePrintfToLog("hw00::performHistEqualization",
			"Starting detectBandObjects");
	Mat inputImage;
	try {
		inputImage = imread(inputImagePath.c_str());
		if (inputImage.empty()) {
			std::stringstream errMsg;
			errMsg << "Could not load input image '" << inputImagePath << "'";
			throw std::runtime_error(errMsg.str());
		}

		DebugManager::writePrintfToLog("hw00::performHistEqualization",
				"Finished reading image.");



	} catch (std::exception const &errThrown) {
		std::cout << "--error start-- \n\n" << errThrown.what()
				<< "\n\n--error end-- \n\n";
		DebugManager::writePrintfToLog("hw00::performHistEqualization",
				"Error thrown %s", errThrown.what());
		return;
	}

	Mat outputImage, outputImage2, imageYCrCb;
	vector<Mat> vector_components, distributed_components;
	cvtColor(inputImage, imageYCrCb, COLOR_BGR2HSV);

	split(imageYCrCb, vector_components);
	MatType(vector_components[0]);

	Mat hComponentEqualized;
	equalizeHist( vector_components[0], hComponentEqualized );

	distributed_components.push_back(hComponentEqualized);
	distributed_components.push_back(vector_components[1]);
	distributed_components.push_back(vector_components[2]);

	merge(distributed_components, outputImage);

	cvtColor(inputImage, outputImage2, COLOR_HSV2BGR);
	getImageHistogram(inputImage);
	
	getImageHistogram(outputImage2);

	imshow("Input Image", inputImage);
	// cout<<"Pixels: "<<inputImage.cols<<" ::: "<<inputImage.rows<<endl;
	
	imshow("Output Equalized Image", outputImage2);

	
	waitKey();
	
}
