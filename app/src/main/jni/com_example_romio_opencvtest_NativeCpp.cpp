#include <com_example_romio_opencvtest_NativeCpp.h>

#define LOG_TAG "taoufik"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))

JNIEXPORT void JNICALL Java_com_example_romio_opencvtest_NativeCpp_convert2Grey
  (JNIEnv *, jclass, jlong imgSrc, jlong imgDst) {
        Mat& mSrc = *(Mat*)imgSrc;
        Mat& mDst = *(Mat*)imgDst;

        cvtColor(mSrc,mDst,CV_RGB2GRAY);
  }

  JNIEXPORT void JNICALL Java_com_example_romio_opencvtest_NativeCpp_detect
    (JNIEnv *, jclass, jlong imgSrc, jlong imgDst) {
          Mat& mSrc = *(Mat*)imgSrc;
          Mat& mDst = *(Mat*)imgDst;

          String stop_cascade_name = "/sdcard/lbpCascade.xml";

          cvtColor(mSrc,mDst,CV_RGB2GRAY);

          vector<cv::Rect> objects;

          CascadeClassifier cascade;
          cascade.load(stop_cascade_name);

          cascade.detectMultiScale(mDst, objects, 1.1, 3, 0,Size(25,25), Size(80, 80));

          for( vector<cv::Rect>::const_iterator r = objects.begin(); r != objects.end(); r++)
              {            //rectangle(img, *r, Scalar(0,0,255), 2, 8, 0);
                           rectangle( mSrc, cvPoint( r->x, r->y ), cvPoint( r->x + r->width, r->y + r->height ),Scalar(0,0,255));
              }

              mDst = mSrc;

    }

JNIEXPORT void JNICALL Java_com_example_romio_opencvtest_NativeCpp_detectContour
  (JNIEnv *, jclass, jlong imgSrc, jlong imgDst) {

           Mat& img_src = *(Mat*)imgSrc;
           Mat& img_dst = *(Mat*)imgDst;

           Mat img_gray, img_canny;


           SimpleShapes shapes;
           vector<vector<Point> > contours; contours.clear();
           vector<Point> vertices_contour; vertices_contour.clear();
           vector<vector<Point> > triangles; triangles.clear();
           vector<RotatedRect> rectangles; rectangles.clear();
           vector<Cerc> cercles; cercles.clear();

           int RATIO = 3;
           int kernel_size = 3;


           //convert to Grayscale
           cvtColor(img_src, img_gray, CV_BGR2GRAY);

           //binarisation (Canny filter)
           Canny(img_gray, img_canny, 50, RATIO*50, kernel_size);

           //find coutours
           findContours(img_canny.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
           img_dst = img_src.clone();

           //find shapes
           for (int i = 0; i < contours.size(); i++) {
               //approximate contour with accuracy proportional
               //to the contour perimeter
               approxPolyDP(Mat(contours[i]), vertices_contour, arcLength(Mat(contours[i]), true)*0.02, true);

               //skip small or non-convex objects
               if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(vertices_contour)) {
                   continue;
               }

               if (vertices_contour.size() == 3) {//TRIANGLES
                   triangles.push_back(vertices_contour); //add to the set of detected triangles

                   //drawing
                   for (int i = 0; i < 3; i++)
                       line(img_dst, vertices_contour[i], vertices_contour[(i+1)%3], Scalar(0,255,0), 3);

                   //label
                   setLabel(img_dst, "TRI", contours[i]);
               }
               else if (vertices_contour.size() == 4) {
                   int nb_vertices = 4; //vertices_contour.size();

                   //calculation of "cos" from all corners
                   vector<double> cos;
                   for (int j = 2; j < nb_vertices+1; j++) {
                       cos.push_back(angleCos(vertices_contour[j%nb_vertices], vertices_contour[j-2], vertices_contour[j-1]));
                   }

                   //storage of "cos" in ascending order
                   sort(cos.begin(), cos.end());

                   //save the min and max values of "cos"
                   double mincos = cos.front();
                   double maxcos = cos.back();

                   //check that the corners are right angles
                   if (mincos >= -0.3 && maxcos <= 0.3) { //RECTANGLES
                       RotatedRect rotRect = minAreaRect(contours[i]);
                       rectangles.push_back(rotRect); //add to the set of detected rectangles

                       //drawing
                       Point2f coins_rect[4];
                       rotRect.points(coins_rect);
                       for (int i = 0; i < 4; i++)
                           line(img_dst, coins_rect[i], coins_rect[(i+1)%4], Scalar(0,255,0), 3);

                       //label
                       stringstream sentence;
                       sentence << "RECT rot = " << rotRect.angle;
                       string rotLabel = sentence.str();
                       setLabel(img_dst, rotLabel, contours[i]);
                   }
               }
               else { //CIRCLES
                   double area = contourArea(contours[i]);
                   Rect r = boundingRect(contours[i]);
                   int radius = r.width / 2;

                   if (abs(1 - ((double)r.width / r.height)) <= 0.2 && abs(1 - (area / (CV_PI * pow(radius, 2)))) <= 0.2) {
                       Cerc cercle;
                       minEnclosingCircle(contours[i], cercle.center, cercle.radius);
                       cercles.push_back(cercle); //add to the set of detected circles

                       //drawing
                       circle(img_dst, cercle.center, cercle.radius, CV_RGB(255,255,0), 3);

                       //label
                       setLabel(img_dst, "CIR", contours[i]);
                   }
               }
           }

           //save data
           shapes.img_src = img_src;
           shapes.img_dst = img_dst;
           shapes.img_bw = img_canny;
           shapes.rectangles = rectangles;
           shapes.triangles = triangles;
           shapes.cercles = cercles;


       }


JNIEXPORT void JNICALL Java_com_example_romio_opencvtest_NativeCpp_detectRed
  (JNIEnv *, jclass, jlong imgSrc, jlong imgDst) {

        Mat& mSrc = *(Mat*)imgSrc;
        Mat& mDst = *(Mat*)imgDst;

        Mat hsvImg;
        cvtColor(mSrc,hsvImg,CV_BGR2HSV);
        inRange(hsvImg,Scalar(110, 50, 50),Scalar(130, 255, 255), mDst);

  }

JNIEXPORT void JNICALL Java_com_example_romio_opencvtest_NativeCpp_detectBlue
  (JNIEnv *, jclass, jlong imgSrc, jlong imgDst) {
        Mat& mSrc = *(Mat*)imgSrc;
        Mat& mDst = *(Mat*)imgDst;

        Mat hsvImg;
        cvtColor(mSrc,hsvImg,CV_BGR2HSV);
        inRange(hsvImg,Scalar(110, 50, 50),Scalar(130, 255, 255), mDst);
  }

JNIEXPORT void JNICALL Java_com_example_romio_opencvtest_NativeCpp_detectYellow
  (JNIEnv *, jclass, jlong imgSrc, jlong imgDst) {
        Mat& mSrc = *(Mat*)imgSrc;
        Mat& mDst = *(Mat*)imgDst;

        Mat hsvImg;
        cvtColor(mSrc,hsvImg,CV_BGR2HSV_FULL);
        inRange(hsvImg,Scalar(20, 100, 100),Scalar(30, 255, 255), mDst);
  }

double angleCos(Point pt1, Point pt2, Point pt0) {
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2));

}

/**
 * Viewing "label" on the contour detected in the picture "img"
 */
void setLabel(Mat& img, const string label, vector<Point>& contour) {
    static int fontface = FONT_HERSHEY_SIMPLEX;
    static double scale = 0.4;
    static int thickness = 1;
    static int baseline = 0;

    Size text = getTextSize(label, fontface, scale, thickness, &baseline);
    Rect r = boundingRect(contour); //returns the smallest rectangle containing the "contour"

    Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
    rectangle(img, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
    putText(img, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}