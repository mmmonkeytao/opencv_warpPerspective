#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <random>

using namespace cv;
using namespace std;

int main() {

        Mat image;
        image = imread("id.jpg", CV_LOAD_IMAGE_COLOR);   // Read the file

        namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.

        int thickness = -1;
        int lineType = 8;

        Point left_top = Point(226.5, 231.0);
        Point right_top = Point(429, 229.5);
        Point bot_left = Point(390, 589.5);
        Point bot_right = Point(919.5, 576.0);

        vector<Point> points;
        points.push_back(left_top);
        points.push_back(right_top);
        points.push_back(bot_left);
        points.push_back(bot_right);

        vector<float> distances;
        for(int i = 0; i < 4; ++i)
        {
                float min = 100000.0;
                if(points[i].x < min)
                {
                        min = points[i].x;
                }
                if(points[i].y < min)
                {
                        min = points[i].y;
                }
                if(image.cols - points[i].x < min)
                {
                        min = image.cols - points[i].x;
                }
                if(image.rows - points[i].y < min)
                {
                        min = image.rows - points[i].y;
                }

                for(int j = 0; j < 4; ++j)
                {
                        if(i != j)
                        {
                                float dist = sqrt(pow(points[i].x - points[j].x, 2)
                                                  + pow(points[i].y - points[j].y, 2));
                                if(dist < min)
                                {
                                        min = dist;
                                }
                        }
                }
                distances.push_back(min);
                //cout << min << endl;
        }

        vector<Point> new_points;
        std::default_random_engine generator;
        std::uniform_real_distribution<float> distribution(-180.0,180.0);
        for(int i = 0; i < 4; ++i)
        {
                float angle = distribution(generator);
                float dx = distances[i] * cos(angle) * 0.3;
                float dy = distances[i] * sin(angle) * 0.3;
                Point p(points[i].x + dx, points[i].y + dy);
                new_points.push_back(p);
                //circle( image, p, 8, Scalar( 0, 255, 255 ), thickness, lineType);
        }

        Mat op = Mat::zeros(4,2, CV_32F);
        Mat np = Mat::zeros(4,2, CV_32F);
        for(int i = 0; i < 4; ++i)
        {
                op.at<float>(i, 0) = points[i].x;
                op.at<float>(i, 1) = points[i].y;

                np.at<float>(i, 0) = new_points[i].x;
                np.at<float>(i, 1) = new_points[i].y;
        }
        Mat m = getPerspectiveTransform(op, np);
        warpPerspective(image, image, m, Size(image.cols, image.rows),
                        INTER_LINEAR, BORDER_CONSTANT, Scalar());

        for(int i = 0; i < 4; ++i)
                circle( image, new_points[i], 8, Scalar( 0, 0, 255 ), thickness, lineType);

        imshow( "Display window", image);
        waitKey(0);

  return 0;
}
