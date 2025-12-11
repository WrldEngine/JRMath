package org.main;

import java.lang.constant.Constable;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class JRMathLib {
    static {
        System.loadLibrary("build/Release/jnimathlib");
    }

    public native static double CMathExp(double x);
    public native static int[] SumArrays(int[] a, int[] b);
    public native static int[] MulArrays(int[] a, int[] b);
    public static native void SumBuffers(ByteBuffer a, ByteBuffer b, ByteBuffer r, int n);
    public static native void MulBuffers(ByteBuffer a, ByteBuffer b, ByteBuffer r, int n);
    public static native void MatMulBuffers(ByteBuffer a, ByteBuffer b, ByteBuffer r, int n, int m, int k);
}