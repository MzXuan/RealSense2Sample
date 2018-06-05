std::string
fixedWidth( int value, int width = 5 )
{
    std::ostringstream results;
    results.fill( '0' );
    results.setf( std::ios_base::internal, std::ios_base::adjustfield );
    results << std::setw( value < 0 ? width + 1 : width ) << value;
    return results.str();
}

void hexToRGB(uint16_t hexValue)
{

    int r = 0xFF;  // actually nothing
    int g = ((hexValue >> 8) & 0xFF);   // Extract the 8 G bits high
    int b = ((hexValue) & 0xFF);        // Extract the 8 B bits low

    // r = ((r * 255) / 31) - 4;
    // g = ((g * 255) / 63) - 2;
    // b = ((b * 255) / 31) - 4;

    printf("r: %d, g: %d, b: %d\n",r, g, b);
}