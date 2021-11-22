# g++ `pkg-config opencv --cflags --libs -Iinclude` -o ../binaries/sideNormalize slideNormalize.cpp
g++ `pkg-config opencv --cflags --libs` -o ../binaries/slideNormalize slideNormalize.cpp
