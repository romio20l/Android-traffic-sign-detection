package com.example.romio.opencvtest;

/**
 * Created by romio on 2/15/2018.
 */

public class NativeCpp {
    public native static void convert2Grey(long imgSrc ,long imgDst);
    public native static void detectRed(long imgSrc,long imgDst);
    public native static void detectBlue(long imgSrc,long imgDst);
    public native static void detectYellow(long imgSrc,long imgDst);
    public native static void detectContour(long imgSrc,long imgDst);

}
