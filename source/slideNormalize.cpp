// Adapted from: 
// https://stackoverflow.com/questions/24341114/simple-illumination-correction-in-images-opencv-c/24341809#24341809

#include <opencv2/opencv.hpp>
#include <vector>       // std::vector
#include <iostream>
#include <fstream>
#include <string>
#include <tclap/CmdLine.h>

bool is_file_exist(std::string & filename) {
    std::ifstream infile(filename);
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

std::string remove_extension(const std::string & filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot); 
}

void normalize_slide_image(std::string & input_filename, std::string & output_filename, bool show = false) {
    std::cout << "Starting image normalization." << std::endl;
    // READ RGB color image and convert it to Lab
    // NICE TO HAVE: add in verbose comments processing
    std::cout << "\t...Reading RGB color image and converting to Lab." << std::endl;
    cv::Mat bgr_image = cv::imread(input_filename);
    cv::Mat lab_image;
    cv::cvtColor(bgr_image, lab_image, CV_BGR2Lab);

    // Extract the L channel
    // NICE TO HAVE: add in verbose comments processing
    std::cout << "\t...Extract the L channel." << std::endl;
    std::vector<cv::Mat> lab_planes(3);
    cv::split(lab_image, lab_planes);  // now we have the L image in lab_planes[0]

    // apply the CLAHE algorithm to the L channel
    // NICE TO HAVE: add in verbose comments processing
    std::cout << "\t...Applying the CLAHE algorithm to the L channel." << std::endl;
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(2);
    cv::Mat dst;
    clahe->apply(lab_planes[0], dst);

    // Merge the color planes back into a Lab image
    std::cout << "\t...Merging color planes back into a Lab image." << std::endl;
    // NICE TO HAVE: add in verbose comments processing
    dst.copyTo(lab_planes[0]);
    cv::merge(lab_planes, lab_image);

   // convert back to RGB
   // NICE TO HAVE: add in verbose comments processing
   std::cout << "\t...Converting back to RGB color image." << std::endl;
   cv::Mat image_clahe;
   cv::cvtColor(lab_image, image_clahe, CV_Lab2BGR);

   // display the results  (you might also want to see lab_planes[0] before and after).
   // NICE TO HAVE: add in verbose comments processing
   if (show) {
       std::cout << "Displaying results. (Press any key to quit...)" << std::endl;
       
       cv::namedWindow("Original image", CV_WINDOW_NORMAL);
       cv::imshow("Original image", bgr_image);
       cv::resizeWindow("Original image", 640, 640);
       
       cv::namedWindow("Normalized (CLAHE) image", CV_WINDOW_NORMAL);
       cv::imshow("Normalized (CLAHE) image", image_clahe);
       cv::resizeWindow("Normalized (CLAHE) image", 640, 640);
       
       cv::waitKey();
       cv::destroyAllWindows();

   }

   cv::imwrite(output_filename, image_clahe);
}

int main(int argc,char **argv) {
    TCLAP::CmdLine cmd("slideNormalize: normalizes an image by assessing image texture using OpenCV's CLAHE (Contrast Limited Adaptive Histogram Equalization). \nCopyright (c) 2017-2021. UMC Utrecht, Utrecht, the Netherlands. The MIT License (MIT): http://opensource.org/licenses/MIT. \nAuthor: Tim Bezemer. Edits by: Lennart P.L. Landsmeer. Last edits: 2021-11-22. \nSuggestions and project lead: Sander W. van der Laan (s.w.vanderlaan@gmail.com)", ' ', "1.0");

    TCLAP::ValueArg<std::string> filenameArg("f","file","The filename of the NDPI/TIF file to process.", true, "", "string", cmd);
    TCLAP::ValueArg<std::string> outnameArg("o","output","The output-filename of the NDPI/TIF file to process.", false, "", "string", cmd);
    TCLAP::ValueArg<std::string> extnameArg("e","extension","The standard output-filename extension of the image-tile to process.", false, "", "string", cmd);
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
