// Adapted from https://stackoverflow.com/questions/24341114/simple-illumination-correction-in-images-opencv-c/24341809#24341809

#include <opencv2/opencv.hpp>
#include <vector>       // std::vector
#include <iostream>
#include <fstream>
#include <string>
#include <tclap/CmdLine.h>

using namespace std;

void print_help()  {

	cout << "slideNormalize: Normalize an image by assessing the image texture using OpenCV's CLAHE (Contrast Limited Adaptive Histogram Equalization). " << endl << endl << "usage" << endl << "  slideNormalize <filename> " << endl;
// 	cout << "  -c/--cp/--cellprofiler: " << endl << "\t Whether to prepare the image for CellProfiler (save as bmp instead of png, and prepend \"ENTROPY_\" to the filename), default 'false'" << endl << endl;
	cout << "description" << endl;
	cout << "  slideNormalize normalizes an image by assessing image texture using OpenCV's CLAHE (Contrast Limited Adaptive Histogram Equalization). " << endl << endl;
	cout << "slideNormalize was written by Tim Bezemer, and edited by Sander W. van der Laan. For questions/comments please contact s.w.vanderlaan@gmail.com" << endl << endl;
// 	cout << "slideEMask uses the CImg library, released under the CeCILL license" << endl;
}

// commandline arguments, where the output filename is optional
// int main(int argc,char **argv) 
// {
// 	TCLAP::CmdLine cmd("slideNormalize: normalizes an image by assessing image texture using OpenCV's CLAHE (Contrast Limited Adaptive Histogram Equalization)", ' ', "0.9");
// 
// 	TCLAP::ValueArg<std::string> filenameArg("f","file","The filename of the NDPI/TIF file to process.", true, "", "string", cmd);
// 	TCLAP::ValueArg<std::string> outnameArg("o","out","The output-filename of the NDPI/TIF file to process.", false, "", "string", cmd);
// 	TCLAP::ValueArg<std::string> extnameArg("e","ext","The standard output-filename extension of the image-tile to process.", false, "", "string", cmd);
// 
// 	cmd.parse( argc, argv );
// 
// 	std::string filename = filenameArg.getValue();
// 	
// }


bool is_file_exist(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

std::string replaceString(std::string subject, const std::string& search,
						  const std::string& replace) {
	size_t pos = 0;
	while((pos = subject.find(search, pos)) != std::string::npos) {
		 subject.replace(pos, search.length(), replace);
		 pos += replace.length();
	}
	return subject;
}

int main(int argc, char** argv)
{

	if (is_file_exist(argv[1]) == false) {
	  cout << "Creating normalized image for file: " << argv[1] << endl;
	  std::cout << "ERROR: File not found." << std::endl;
	  exit(1);

	}

	// READ RGB color image and convert it to Lab
	// add in verbose comments processing
	// cout << "Creating normalized image for file: " << argv[1] << endl;
	cv::Mat bgr_image = cv::imread(argv[1]);
	cv::Mat lab_image;
	cv::cvtColor(bgr_image, lab_image, CV_BGR2Lab);

	// Extract the L channel
	std::vector<cv::Mat> lab_planes(3);
	cv::split(lab_image, lab_planes);  // now we have the L image in lab_planes[0]

	// apply the CLAHE algorithm to the L channel
	cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
	clahe->setClipLimit(2);
	cv::Mat dst;
	clahe->apply(lab_planes[0], dst);

	// Merge the the color planes back into an Lab image
	dst.copyTo(lab_planes[0]);
	cv::merge(lab_planes, lab_image);

   // convert back to RGB
   cv::Mat image_clahe;
   cv::cvtColor(lab_image, image_clahe, CV_Lab2BGR);

   // display the results  (you might also want to see lab_planes[0] before and after).
   //cv::imshow("image original", bgr_image);
   //cv::imshow("image CLAHE", image_clahe);
   //cv::waitKey();

// OPTIONAL: Make sure that you can control the extension name
   std::string new_filename = replaceString( std::string(argv[1]), ".tile.tissue.png", ".normalized.tile.tissue.png" );

// Make sure that  if an output file name is given, it will output the file appropriately
//    if (argv[2] == true) {
//      cout << "Saving normalized image as " << argv[2] << endl;
//      std::string new_filename = std::string(argv[2]);
//    }
   
   cv::imwrite(new_filename, image_clahe);
}