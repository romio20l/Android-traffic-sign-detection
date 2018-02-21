#include <com_example_romio_opencvtest_NativeCpp.h>

JNIEXPORT jint JNICALL Java_com_example_romio_opencvtest_NativeCpp_convert2Grey
  (JNIEnv *, jclass, jlong imgSrc, jlong imgDst) {
        Mat& mSrc = *(Mat*)imgSrc;
        Mat& mDst = *(Mat*)imgDst;

        cvtColor(mSrc,mDst,CV_RGB2GRAY);
        if(mSrc.rows==mDst.rows && mSrc.cols==mDst.cols) {
            return (jint)1;
        }
        return (jint)0;
  }

JNIEXPORT jint JNICALL Java_com_example_romio_opencvtest_NativeCpp_detectRed
  (JNIEnv *, jclass, jlong imgSrc, jlong imgDst) {
        Mat& mSrc = *(Mat*)imgSrc;
        Mat& mDst = *(Mat*)imgDst;

        cv::Mat orig_image = mSrc.clone();

        cv::medianBlur(orig_image, orig_image, 3);

        	// Convert input image to HSV
        	cv::Mat hsv_image;
        	cv::cvtColor(orig_image, hsv_image, cv::COLOR_BGR2HSV);

        	// Threshold the HSV image, keep only the red pixels
        	cv::Mat lower_red_hue_range;
        	cv::Mat upper_red_hue_range;
        	cv::inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
        	cv::inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);

        	// Combine the above two images
        	cv::Mat red_hue_image;
        	cv::addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

        	cv::GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);

            mDst = red_hue_image;

        if(mSrc.rows==mDst.rows && mSrc.cols==mDst.cols) {
            return (jint)1;
        }
        return (jint)0;
  }