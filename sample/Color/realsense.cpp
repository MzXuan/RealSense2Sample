#include "realsense.h"
#include <string>
#include <vector>

 int FILE_INDEX = 0;
 bool FLAG_SAVE = false;

 std::string fixedWidth( int value, int width = 2 )
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
                std::cout <<"start saving color images" << std::endl;
            }
            else
                std::cout << "stop saving color images" << std::endl;
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
    config.enable_stream( rs2_stream::RS2_STREAM_COLOR, color_width, color_height, rs2_format::RS2_FORMAT_BGR8, color_fps );


    // Start Pipeline
    pipeline_profile = pipeline.start( config );

    auto depth_stream = pipeline_profile.get_stream(rs2_stream::RS2_STREAM_COLOR)
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

    // Update Color
    updateColor();
}

// Update Frame
inline void RealSense::updateFrame()
{
    // Update Frame
    frameset = pipeline.wait_for_frames();
}

// Update Color
inline void RealSense::updateColor()
{
    // Retrieve Color Frame
    color_frame = frameset.get_color_frame();

    // Retrive Frame Size
    color_width = color_frame.as<rs2::video_frame>().get_width();
    color_height = color_frame.as<rs2::video_frame>().get_height();
}

// Draw Data
void RealSense::draw()
{
    // Draw Color
    drawColor();
}

// Draw Color
inline void RealSense::drawColor()
{
    // Create cv::Mat form Color Frame
    color_mat = cv::Mat( color_height, color_width, CV_8UC3, const_cast<void*>( color_frame.get_data() ) );
}

// Show Data
void RealSense::show()
{
    // Show Color
    showColor();
}

// Show Color
inline void RealSense::showColor()
{
    if( color_mat.empty() ){
        return;
    }


    // Show Color Image
    cv::imshow( "Color", color_mat );
}

void RealSense::write(){
    //write depth imge
    //TODO: add save key checking
    if( FLAG_SAVE == true){
        FILE_INDEX++;
        writeColor();
    }
    FLAG_SAVE = false;
}

inline void RealSense::writeColor()
{
    if( color_mat.empty() ){
        return;
    }
    // cv::Size size(640,480);

    // cv::Mat resize_mat;
    // cv::resize(depth_mat,resize_mat,size);

    // cv::Mat mat_save(480,640, CV_8UC3, cv::Scalar(255,6,0)); //bgr
    
    // for (int i=0; i<resize_mat.rows; i++){
    //     for (int j=0; j<resize_mat.cols; j++){
    //     short val = resize_mat.at<short>(i,j);
    //     int r = 0xFF;  // actually nothing
    //     int g = ((val >> 8) & 0xFF);   // Extract the 8 G bits high
    //     int b = ((val) & 0xFF);  

    //     cv::Vec3b p;
    //     p[0] = b;
    //     p[1] = g;
    //     p[2] = r;

    //     mat_save.at<cv::Vec3b>(i,j) = p;
    //     }
    // }

    std::string s = fixedWidth(FILE_INDEX);
    // std::string s = std::to_string(FILE_INDEX);

    //Write Depth Image
    cv::imwrite("../../calibrate/data/color_"+s+".jpg", color_mat);
}