package org.main;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;

public class Benchmarks {
    static int N = 100_000_000;

    // 90% Copying, 10% Calucation
    public static void testSum() {
        ByteBuffer bufA = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufB = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufRNative = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufRGPU = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());

        for (int i = 0; i < N; i++) {
            bufA.putInt(i * 2);
            bufB.putInt(i);
        }

        bufA.rewind();
        bufB.rewind();
        bufRNative.rewind();
        bufRGPU.rewind();

        JRMathLib.SumBuffers(bufA, bufB, bufRGPU, N);
        long start = System.nanoTime();
        JRMathLib.SumBuffers(bufA, bufB, bufRGPU, N);
        long end = System.nanoTime();

        System.out.println("Sum: " + bufRGPU);
        System.out.println("ms: " + (end - start) / 1_000_000.0);

//        IntBuffer intBuf = bufRGPU.asIntBuffer();
//        int value = intBuf.get(2);
//        System.out.println(value);

        long start_native = System.nanoTime();
        for (int i = 0; i < N; i++) {
            int a = bufA.getInt(i * 4);
            int b = bufB.getInt(i * 4);
            bufRNative.putInt(i * 4, a + b);
        }
        long end_native = System.nanoTime();

        System.out.println("Sum: " + bufRNative);
        System.out.println("ms: " + (end_native - start_native) / 1_000_000.0);
    }

    // 90% Copying, 10% Calucation
    public static void testMul() {
        ByteBuffer bufA = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufB = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufRNative = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufRGPU = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());

        for (int i = 0; i < N; i++) {
            bufA.putInt(i * 2);
            bufB.putInt(i);
        }

        bufA.rewind();
        bufB.rewind();
        bufRNative.rewind();
        bufRGPU.rewind();

        JRMathLib.MulBuffers(bufA, bufB, bufRGPU, N);
        long start = System.nanoTime();
        JRMathLib.MulBuffers(bufA, bufB, bufRGPU, N);
        long end = System.nanoTime();

        System.out.println("Sum: " + bufRGPU);
        System.out.println("ms: " + (end - start) / 1_000_000.0);

        long start_native = System.nanoTime();
        for (int i = 0; i < N; i++) {
            int a = bufA.getInt(i * 4);
            int b = bufB.getInt(i * 4);
            bufRNative.putInt(i * 4, a * b);
        }
        long end_native = System.nanoTime();

        System.out.println("Mul: " + bufRNative);
        System.out.println("ms: " + (end_native - start_native) / 1_000_000.0);
    }

    public static void main(String[] args) {
        testMul();
    }
}
