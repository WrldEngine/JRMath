package org.main;

public class JRMathLib {
    static {
        System.loadLibrary("build/Release/jnimathlib");
    }

    public native static double CMathExp(double x);
    public native static int[] SumArrays(int[] a, int[] b);
    public native static int[] MulArrays(int[] a, int[] b);

    // NOTE: Testing entrypoint
    public static void main(String[] args) {
        System.out.println("MathLib.main() called");

        double x = 1.0;
        System.out.printf("x: %f%n", x);
        System.out.printf("C++ : exp(x): %f%n", JRMathLib.CMathExp(x));

        /// EXAMPLE:
        /// <b>Sum</b> of two integer arrays
        int[] a = new int[]{1, 2, 3, 4, 5};
        int[] b = new int[]{1, 2, 3, 4, 5};

        long start = System.nanoTime();

        int[] r = JRMathLib.SumArrays(a, b);

        long end = System.nanoTime();
        long duration = end - start;
        System.out.println("Elapsed time: " + duration / 1_000_000.0 + " ms");

        int[] r1 = JRMathLib.SumArrays(a, b);

        System.out.println("Sum of arrays: ");
        for (int i = 0; i < r.length; i++) {
            System.out.println(r[i]);
        }

        /// EXAMPLE:
        /// <b>Multiplication</b> of two integer arrays
        int[] am = new int[]{1, 2, 3, 4, 5};
        int[] bm = new int[]{1, 2, 3, 4, 5};
        int[] rm = JRMathLib.MulArrays(am, bm);

        System.out.println("Multiplication of arrays: ");
        for (int i = 0; i < rm.length; i++) {
            System.out.println(rm[i]);
        }
    }
}