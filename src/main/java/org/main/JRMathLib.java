package org.main;

public class JRMathLib {
    static {
        System.loadLibrary("build/Release/jnimathlib");
    }

    public native static double CMathExp(double x);
    public native static int[] SumArrays(int[] a, int[] b);

    // NOTE: Testing entrypoint
    public static void main(String[] args) {
        System.out.println("MathLib.main() called");

        double x = 1.0;
        System.out.printf("x: %f%n", x);
        System.out.printf("C++ : exp(x): %f%n", JRMathLib.CMathExp(x));

        int[] a = new int[]{1, 2, 3, 4, 5};
        int[] b = new int[]{1, 2, 3, 4, 5};
        int[] r = JRMathLib.SumArrays(a, b);

        System.out.println("Sum of arrays: ");
        for (int i = 0; i < r.length; i++) {
            System.out.println(r[i]);
        }
    }
}