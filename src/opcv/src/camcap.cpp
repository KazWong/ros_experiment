#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

string hot_keys =
    "\n\nHot keys: \n"
    "\tESC - quit the program\n"
    "\tc - stop the tracking\n"
    "\tb - switch to/from backprojection view\n"
    "\th - show/hide object histogram\n"
    "\tp - pause video\n"
    "To initialize tracking, select the object with mouse\n";
static void help()
{
    cout << "\nThis is a demo that shows mean-shift based tracking\n"
            "You select a color objects such as your face and it tracks it.\n"
            "This reads from video camera (0 by default, or the camera number the user enters\n"
            "Usage: \n"
            "   ./camshiftdemo [camera number]\n";
    cout << hot_keys;
}
const char* keys =
{
    "{help h | | show help message}{@camera_number| 0 | camera number}"
};
int main( int argc, const char** argv )
{
    VideoCapture cap;

    CommandLineParser parser(argc, argv, keys);
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    int camNum = parser.get<int>(0);
    cap.open(camNum);
    
    cap.set(CAP_PROP_FPS, 5);
    cap.set(CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(CAP_PROP_FRAME_HEIGHT, 1080);
    cap.set(CAP_PROP_AUTO_EXPOSURE, 1);
    //cap.set(CAP_PROP_EXPOSURE, 200);
    //cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('M', 'J', 'P', 'G'));
    //cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('H', '2', '6', '4'));
    cap.set(CV_CAP_PROP_FOURCC, CV_FOURCC('Y', 'U', 'Y', 'V'));
    cap.set(CAP_PROP_ZOOM, 400);
    
    if( !cap.isOpened() )
    {
        help();
        cout << "***Could not initialize capturing...***\n";
        cout << "Current parameter's value: \n";
        parser.printMessage();
        return -1;
    }
    
    namedWindow( "video0", 0 );
    Mat frame, image;
    bool paused = false;
    for(;;)
    {
        if( !paused )
        {
            cap >> frame;
            if( frame.empty() )
                break;
        }
        frame.copyTo(image);
        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        switch(c)
        {
        case 'p':
            paused = !paused;
            break;
        case 'r':
            cap.set(CAP_PROP_EXPOSURE, 200);
            break;
        case 't':
            cap.set(CAP_PROP_ZOOM, 400);
            cap.set(CAP_PROP_EXPOSURE, 3);
            break;
        default:
            ;
        }
        
        imshow( "video0", image );
    }
    return 0;
}
