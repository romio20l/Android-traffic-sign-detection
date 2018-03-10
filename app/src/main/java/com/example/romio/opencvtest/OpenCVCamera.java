package com.example.romio.opencvtest;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.view.SurfaceView;
import android.view.WindowManager;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Core;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Scalar;
import org.opencv.core.Size;
import org.opencv.imgproc.Imgproc;

import java.io.FileNotFoundException;
import java.io.InputStream;

public class OpenCVCamera extends Activity implements CameraBridgeViewBase.CvCameraViewListener2 {
    private static final String TAG = "OCVSample::Activity";
    private static boolean IMG = false;

    private CameraBridgeViewBase mOpenCvCameraView;
    private boolean mIsJavaCamera = true;
    private MenuItem mItemSwitchCamera = null;
    Bitmap img;
    Mat imgSrc,imgDst1,imgDst2,imgDst,mDst;

    int LowH = 0;
    int LowV = 90;
    int LowS = 240;
    int HighH = 15;
    int HighV = 130;
    int HighS = 213;
    Scalar sc1,sc2;



    static {
        try {
            System.loadLibrary("myNativeLibs");
            System.loadLibrary("gnustl_shared");
            System.loadLibrary("fann-test");
        } catch (UnsatisfiedLinkError e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    static {

    }

    private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");
                    mOpenCvCameraView.enableView();
                } break;
                default:
                {
                    super.onManagerConnected(status);
                } break;
            }
        }
    };

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "called onCreate");

        if(getIntent().getStringExtra("imgUri")!=null) {
            try {
                final Uri imageUri = Uri.parse(getIntent().getStringExtra("imgUri"));
                final InputStream imageStream = getContentResolver().openInputStream(imageUri);
                img = BitmapFactory.decodeStream(imageStream);
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }

            IMG = true;
        }

        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.opencv_camera);

        mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.tutorial1_activity_java_surface_view);

        mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);

        mOpenCvCameraView.setCvCameraViewListener(this);

        sc1 = new Scalar(LowH,LowS,LowV);
        sc2 = new Scalar(HighH,HighS,HighV);
    }

    @Override
    public void onPause()
    {
        super.onPause();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    @Override
    public void onResume()
    {
        super.onResume();
        if (!OpenCVLoader.initDebug()) {
            Log.d(TAG, "Internal OpenCV library not found. Using OpenCV Manager for initialization");
            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_0_0, this, mLoaderCallback);
        } else {
            Log.d(TAG, "OpenCV library found inside package. Using it!");
            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS);
        }
    }

    public void onDestroy() {
        super.onDestroy();
        if (mOpenCvCameraView != null)
            mOpenCvCameraView.disableView();
    }

    public void onCameraViewStarted(int width, int height) {
        imgSrc = new Mat(width,height, CvType.CV_8UC4);
        if(IMG) {
            Imgproc.resize(imgSrc,imgSrc,new Size(img.getWidth(),img.getHeight()));
            Utils.bitmapToMat(img,imgSrc);
        }

        mDst = new Mat();
        imgDst1 = new Mat(width,height, CvType.CV_8UC1);
        imgDst2 = new Mat(width,height, CvType.CV_8UC1);
        imgDst = new Mat(width,height, CvType.CV_8UC1);

    }

    public void onCameraViewStopped() {
        imgSrc.release();
        imgDst1.release();
        imgDst2.release();
        imgDst.release();
    }

    public Mat onCameraFrame(CameraBridgeViewBase.CvCameraViewFrame inputFrame) {

        imgSrc = inputFrame.rgba();
        NativeCpp.detect(imgSrc.getNativeObjAddr(),imgDst.getNativeObjAddr());
        //Core.bitwise_or(detectyellow(imgSrc.clone()),detectRed(imgSrc.clone()),imgDst1);
        //Core.bitwise_or(detectyellow(imgSrc.clone()),imgDst1,imgDst2);
        //NativeCpp.detectContour(imgSrc.getNativeObjAddr(),imgDst.getNativeObjAddr());

        return imgDst;
    }

    public Mat detectBlue(Mat input) {
        Imgproc.cvtColor(input,imgSrc,Imgproc.COLOR_RGB2HSV);
        Core.inRange(imgSrc,new Scalar(100, 50, 0),new Scalar(140, 255, 255), imgDst);
        Imgproc.resize(imgDst,imgDst,input.size());
        Imgproc.threshold(imgDst,imgDst,100,256,0);
        return imgDst;
    }

    public Mat detectyellow(Mat input) {
        Imgproc.cvtColor(input,imgSrc,Imgproc.COLOR_RGB2HSV);
        Core.inRange(imgSrc,new Scalar(20, 100, 100),new Scalar(30, 255, 255), imgDst);
        Imgproc.resize(imgDst,imgDst,input.size());
        Imgproc.threshold(imgDst,imgDst,100,256,0);
        return imgDst;
    }

    public Mat detectShape(Mat input) {
        NativeCpp.detectContour(input.getNativeObjAddr(),imgDst.getNativeObjAddr());
        return imgDst;
    }




}

