package com.example.romio.opencvtest;

/**
 * Created by romio on 2/15/2018.
 */

public class NativeCpp {
    public native static int convert2Grey(long imgSrc ,long imgDst);
    public native static int detectRed(long imgSrc,long imgDst);
}
