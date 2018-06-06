#include "realsense.h"
#include "utils.hpp"

 int FILE_INDEX = 0;
 bool FLAG_SAVE = false;


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

        //write Data
        write();
        // Key Check
        const int32_t key = cv::waitKey( 10 );
        if( key == 'q' ){
            break;
        }
        else if (key == 's'){
            FLAG_SAVE = !FLAG_SAVE;
            if (FLAG_SAVE == true){
                std::cout <<"start saving depth images" << std::endl;
            }
            else
                std::cout << "stop saving depth images" << std::endl;
            //start saving / not saving img
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


    auto depth_stream = pipeline_profile.get_stream(rs2_stream::RS2_STREAM_DEPTH)
                             .as<rs2::video_stream_profile>();
    auto i = depth_stream.get_intrinsics();
    // float fov[2] = { i.hfov, i.vfov };
    // float fov[2]; // X, Y fov
    // rs2_fov(&i, fov);
    std::cout << i.fx <<i.fy<< std::endl;

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


    
}

void RealSense::write(){
    //write depth imge
    //TODO: add save key checking
    if( FLAG_SAVE == true){
        FILE_INDEX++;
        writeDepth();
    }
}

inline void RealSense::writeDepth()
{
    if( depth_mat.empty() ){
        return;
    }
    cv::Size size(640,480);

    cv::Mat resize_mat;
    cv::resize(depth_mat,resize_mat,size);

    cv::Mat mat_save(480,640, CV_8UC3, cv::Scalar(255,6,0)); //bgr
    
    for (int i=0; i<resize_mat.rows; i++){
        for (int j=0; j<resize_mat.cols; j++){
        short val = resize_mat.at<short>(i,j);
        int r = 0xFF;  // actually nothing
        int g = ((val >> 8) & 0xFF);   // Extract the 8 G bits high
        int b = ((val) & 0xFF);  

        cv::Vec3b p;
        p[0] = b;
        p[1] = g;
        p[2] = r;

        mat_save.at<cv::Vec3b>(i,j) = p;
        }
    }

    std::string s = fixedWidth(FILE_INDEX);

    //Write Depth Image
    cv::imwrite("../data/depth_"+s+".png", mat_save);
}