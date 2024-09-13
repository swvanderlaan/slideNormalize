# CentOS7: 
# g++ `pkg-config opencv --cflags ` -I/hpc/local/CentOS7/dhl_ec/software/Anaconda3_2021_05/envs/cp4/include/ -o ../binaries/sideNormalize_CentOS7 slideNormalize.cpp
# macOS Sequoia with openCV4 installed via brew:
# https://stackoverflow.com/questions/49943161/opencv-4-x-requires-enabled-c11-support-compilation-darknet-fatal-error
g++ -std=c++11 slideNormalize.cpp `pkg-config opencv --cflags --libs` -o ../binaries/slideNormalize_macOS_Sequoia150 slideNormalize.cpp
