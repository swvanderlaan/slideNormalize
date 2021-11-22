// Adapted from https://stackoverflow.com/questions/24341114/simple-illumination-correction-in-images-opencv-c/24341809#24341809

#include <opencv2/opencv.hpp>
#include <vector>       // std::vector
#include <iostream>
#include <fstream>
#include <string>
#include <tclap/CmdLine.h>

bool is_file_exist(std::string & fileName) {
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

std::string remove_extension(const std::string& filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot); 
}

void normalize_slide_image(std::string & input_filename, std::string & output_filename, bool show = false) {
    // READ RGB color image and convert it to Lab
    // add in verbose comments processing
    cv::Mat bgr_image = cv::imread(input_filename);
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
   if (show) {
       std::cout << "Press any key to quit..." << std::endl;
       cv::imshow("image original", bgr_image);
       cv::imshow("image CLAHE", image_clahe);
       cv::waitKey();
   }

   cv::imwrite(output_filename, image_clahe);
}

int main(int argc,char **argv) {
    TCLAP::CmdLine cmd("slideNormalize: normalizes an image by assessing image texture using OpenCV's CLAHE (Contrast Limited Adaptive Histogram Equalization)", ' ', "0.9");

    TCLAP::ValueArg<std::string> filenameArg("f","file","The filename of the NDPI/TIF file to process.", true, "", "string", cmd);
    TCLAP::ValueArg<std::string> outnameArg("o","out","The output-filename of the NDPI/TIF file to process.", false, "", "string", cmd);
    TCLAP::ValueArg<std::string> extnameArg("e","ext","The standard output-filename extension of the image-tile to process.", false, "", "string", cmd);
    TCLAP::SwitchArg showArg("s","show","Show results in a graphical interface", cmd, false);

    cmd.parse(argc, argv);

    std::string inname = filenameArg.getValue();
    std::string extname = extnameArg.getValue();
    std::string outname = outnameArg.getValue();
    bool show = showArg.getValue();

    if (outname.size() == 0) {
       outname = replaceString(inname, ".tile.tissue.png", ".normalized.tile.tissue.png" );
    }

    if (extname.size() != 0) {
        outname = remove_extension(outname) + "." + extname;
    }

    if (!is_file_exist(inname)) {
        std::cout << "Creating normalized image for file: " << inname << std::endl;
        std::cout << "ERROR: File not found." << std::endl;
        exit(1);
    }

    if (inname == outname) {
        std::cout << "Creating normalized image for file: " << inname << std::endl;
      std::cout << "ERROR: Refusing to overwrite sourcefile with output" << std::endl;
      exit(1);
    }

    std::cout << "input:  " << inname << std::endl;
    std::cout << "output: " << outname << std::endl;

    normalize_slide_image(inname, outname, show);
}
