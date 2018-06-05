#ifndef __REALSENSE__
#define __REALSENSE__

#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>

class RealSense
{
private:
    // RealSense
    rs2::pipeline pipeline;
    rs2::pipeline_profile pipeline_profile;
    rs2::frameset frameset;

    // Depth Buffer
    rs2::frame depth_frame;
    cv::Mat depth_mat;
    uint32_t depth_width = 640;
    uint32_t depth_height = 480;
    uint32_t depth_fps = 30;

public:
    // Constructor
    RealSense();

    // Destructor
    ~RealSense();

    // Processing
    void run();

private:
    // Initialize
    void initialize();

    // Initialize Sensor
    inline void initializeSensor();

    // Finalize
    void finalize();

    // Update Data
    void update();

    // Update Frame
    inline void updateFrame();

    // Update Depth
    inline void updateDepth();

    // Draw Data
    void draw();

    // Draw Depth
    inline void drawDepth();

    // Show Data
    void show();

    // Show Depth
    inline void showDepth();

    inline void writeDepth(cv::Mat scale_mat);
};

#endif // __REALSENSE__