package org.main;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;
import java.util.Random;

public class Benchmarks {
    static int N = 100_000_000;

    // ~90% Copying, ~10% Calucation
    public static void testSum() {
        ByteBuffer bufA = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufB = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufRNative = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufRGPU = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());

        for (int i = 1; i < N; i++) {
            bufA.putInt(i * 2);
            bufB.putInt(i);
        }

        bufA.rewind();
        bufB.rewind();
        bufRNative.rewind();
        bufRGPU.rewind();

        JRMathLib.SumBuffers(bufA, bufB, bufRGPU, N);
        long startGPU = System.nanoTime();
        JRMathLib.SumBuffers(bufA, bufB, bufRGPU, N);
        long endGPU = System.nanoTime();

        System.out.println("Sum: " + bufRGPU.getInt(0));
        System.out.println("GPU TIME ms: " + (endGPU - startGPU) / 1_000_000.0);

        bufA.rewind();
        bufB.rewind();

        long startCPU = System.nanoTime();
        for (int i = 0; i < N; i++) {
            int a = bufA.getInt(i * 4);
            int b = bufB.getInt(i * 4);
            bufRNative.putInt(i * 4, a + b);
        }
        long endCPU = System.nanoTime();

        System.out.println("Sum: " + bufRNative.getInt(0));
        System.out.println("CPU TIME ms: " + (endCPU - startCPU) / 1_000_000.0);

        double speedup = (double)(endCPU - startCPU) / (endGPU - startGPU);
        System.out.printf("\nDiff: %.2fx\n", speedup);
    }

    // ~90% Copying, ~10% Calucation
    public static void testMul() {
        ByteBuffer bufA = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufB = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufRNative = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufRGPU = ByteBuffer.allocateDirect(N * 4).order(ByteOrder.nativeOrder());

        for (int i = 1; i < N; i++) {
            bufA.putInt(i * 2);
            bufB.putInt(i);
        }

        bufA.rewind();
        bufB.rewind();
        bufRNative.rewind();
        bufRGPU.rewind();

        JRMathLib.MulBuffers(bufA, bufB, bufRGPU, N);
        long startGPU = System.nanoTime();
        JRMathLib.MulBuffers(bufA, bufB, bufRGPU, N);
        long endGPU = System.nanoTime();

        System.out.println("GPU Result: " + bufRGPU.getInt(0));
        System.out.println("GPU TIME ms: " + (endGPU - startGPU) / 1_000_000.0);

        bufA.rewind();
        bufB.rewind();

        long startCPU = System.nanoTime();
        for (int i = 0; i < N; i++) {
            int a = bufA.getInt(i * 4);
            int b = bufB.getInt(i * 4);
            bufRNative.putInt(i * 4, a * b);
        }
        long endCPU = System.nanoTime();

        System.out.println("CPU Result: " + bufRNative.getInt(0));
        System.out.println("CPU TIME ms: " + (endCPU - startCPU) / 1_000_000.0);

        double speedup = (double)(endCPU - startCPU) / (endGPU - startGPU);
        System.out.printf("\nDiff: %.2fx\n", speedup);
    }

    public static void testMatMul() {
        int M = 1000;
        int K = 1000;
        int N = 1000;

        ByteBuffer bufA = ByteBuffer.allocateDirect(M * K * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufB = ByteBuffer.allocateDirect(K * N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufRGPU = ByteBuffer.allocateDirect(M * N * 4).order(ByteOrder.nativeOrder());
        ByteBuffer bufRCPU = ByteBuffer.allocateDirect(M * N * 4).order(ByteOrder.nativeOrder());

        Random rand = new Random();
        for (int i = 0; i < M * K; i++) {
            bufA.putFloat(rand.nextFloat() * 10);
        }

        for (int i = 0; i < K * N; i++) {
            bufB.putFloat(rand.nextFloat() * 10);
        }

        bufA.rewind();
        bufB.rewind();

        JRMathLib.MatMulBuffers(bufA, bufB, bufRGPU, M, K, N);

        long startGPU = System.nanoTime();
        JRMathLib.MatMulBuffers(bufA, bufB, bufRGPU, M, K, N);
        long endGPU = System.nanoTime();

        System.out.println("GPU time: " + (endGPU - startGPU) / 1_000_000.0 + " ms");

        bufA.rewind();
        bufB.rewind();

        long startCPU = System.nanoTime();
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                float sum = 0.0f;
                for (int k = 0; k < K; k++) {
                    float a = bufA.getFloat((i * K + k) * 4);
                    float b = bufB.getFloat((k * N + j) * 4);
                    sum += a * b;
                }
                bufRCPU.putFloat((i * N + j) * 4, sum);
            }
        }
        long endCPU = System.nanoTime();

        System.out.println("CPU time: " + (endCPU - startCPU) / 1_000_000.0 + " ms");

        bufRGPU.rewind();
        bufRCPU.rewind();
        System.out.println("\nFirst element check:");
        System.out.println("GPU: " + bufRGPU.getFloat(0));
        System.out.println("CPU: " + bufRCPU.getFloat(0));

        double speedup = (double)(endCPU - startCPU) / (endGPU - startGPU);
        System.out.printf("\nSpeedup: %.2fx\n", speedup);
    }

    public static void main(String[] args) {
        testMatMul();
    }
}
