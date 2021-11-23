# CentOS7: 
# g++ `pkg-config opencv --cflags ` -I/hpc/local/CentOS7/dhl_ec/software/Anaconda3_2021_05/envs/cp4/include/ -o ../binaries/sideNormalize_CentOS7 slideNormalize.cpp
# macOS Monterey
g++ `pkg-config opencv --cflags --libs` -o ../binaries/slideNormalize_macOS slideNormalize.cpp
