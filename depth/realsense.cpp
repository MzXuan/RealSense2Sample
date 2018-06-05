#include "realsense.h"
#include <string>

 int file_index = 0;

inline std::string
fixedWidth( int value, int width = 5 )
{
    std::ostringstream results;
    results.fill( '0' );
    results.setf( std::ios_base::internal, std::ios_base::adjustfield );
    results << std::setw( value < 0 ? width + 1 : width ) << value;
    return results.str();
}

// Constructor
RealSense::RealSense()
{
    // Initialize
    initialize();
}

// Destructor
RealSense::~RealSense()
{
    // Finalize
    finalize();
}

// Processing
void RealSense::run()
{

    // Main Loop
    while( true ){
        // Update Data
        update();

        // Draw Data
        draw();

        // Show Data
        show();

        file_index++;
        // Key Check
        const int32_t key = cv::waitKey( 10 );
        if( key == 'q' ){
            break;
        }
    }
}

// Initialize
void RealSense::initialize()
{
    cv::setUseOptimized( true );

    // Initialize Sensor
    initializeSensor();
}

// Initialize Sensor
inline void RealSense::initializeSensor()
{
    // Set Device Config
    rs2::config config;
    config.enable_stream( rs2_stream::RS2_STREAM_DEPTH, depth_width, depth_height, rs2_format::RS2_FORMAT_Z16, depth_fps );

    // Start Pipeline
    pipeline_profile = pipeline.start( config );
}

// Finalize
void RealSense::finalize()
{
    // Close Windows
    cv::destroyAllWindows();

    // Stop Pipline
    pipeline.stop();
}

// Update Data
void RealSense::update()
{
    // Update Frame
    updateFrame();

    // Update Depth
    updateDepth();
}

// Update Frame
inline void RealSense::updateFrame()
{
    // Update Frame
    frameset = pipeline.wait_for_frames();
}

// Update Depth
inline void RealSense::updateDepth()
{
    // Retrieve Depth Frame
    depth_frame = frameset.get_depth_frame();

    // Retrive Frame Size
    depth_width = depth_frame.as<rs2::video_frame>().get_width();
    depth_height = depth_frame.as<rs2::video_frame>().get_height();
}

// Draw Data
void RealSense::draw()
{
    // Draw Depth
    drawDepth();
}

// Draw Depth
inline void RealSense::drawDepth()
{
    // Create cv::Mat form Depth Frame
    depth_mat = cv::Mat( depth_height, depth_width, CV_16SC1, const_cast<void*>( depth_frame.get_data() ) );
}

// Show Data
void RealSense::show()
{
    // Show Depth
    showDepth();
}

// Show Depth
inline void RealSense::showDepth()
{
    if( depth_mat.empty() ){
        return;
    }

    // Scaling
    cv::Mat scale_mat;
    depth_mat.convertTo( scale_mat, CV_8U, -255.0 / 10000.0, 255.0 ); // 0-10000 -> 255(white)-0(black)
    //depth_mat.convertTo( scale_mat, CV_8U, 255.0 / 10000.0, 0.0 ); // 0-10000 -> 0(black)-255(white)

    // Apply False Colour
    //cv::applyColorMap( scale_mat, scale_mat, cv::COLORMAP_BONE );

    // Show Depth Image
    cv::imshow( "Depth", scale_mat );

    //write depth imge
    //TODO: add save key checking
    writeDepth(scale_mat);
}


inline void RealSense::writeDepth(cv::Mat scale_mat)
{
    std::string s = fixedWidth(file_index);
    // std::cout << s <<std::endl;

    //Write Depth Image
    cv::Mat resize_mat;
    cv::resize(scale_mat,resize_mat,cv::Size(640,480));
    cv::imwrite("./data/depth.png", scale_mat);

    

}