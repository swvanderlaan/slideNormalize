#g++ `pkg-config --cflags opencv` -o ../binaries/sideNormalize slideNormalize.cpp
g++ `pkg-config opencv --cflags --libs` -o ../binaries/slideNormalize slideNormalize.cpp
