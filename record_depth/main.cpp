#include <iostream>
#include <sstream>
#include "realsense.h"

// void hexToRGB(uint16_t hexValue)
// {

//     int r = 0xFF;  // actually nothing
//     int g = ((hexValue >> 8) & 0xFF);   // Extract the 8 G bits high
//     int b = ((hexValue) & 0xFF);        // Extract the 8 B bits low

//     // r = ((r * 255) / 31) - 4;
//     // g = ((g * 255) / 63) - 2;
//     // b = ((b * 255) / 31) - 4;

//     printf("r: %d, g: %d, b: %d\n",r, g, b);
// }


int main( int argc, char* argv[] )
{
	// hexToRGB(0x3D0F);
	// short val = 1;
	// std::cout << sizeof(short) <<std::endl;
    
    try{
        RealSense realsense;
        realsense.run();
    } catch( std::exception& ex ){
        std::cout << ex.what() << std::endl;
    }

    return 0;
}