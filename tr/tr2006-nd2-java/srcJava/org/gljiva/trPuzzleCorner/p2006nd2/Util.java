package org.gljiva.trPuzzleCorner.p2006nd2;

public final class Util {

    private Util() {
        // Private: no construction!
    }

    // ----------------------------------------------------------------------

    public static int gcd(int... numbers) {
        int gcd = 0;
        for (int n : numbers) {
            if (n < 0) {
                n = -n;
            }
            while (n != 0) {
                gcd = gcd % n;
                if (gcd == 0) {
                    gcd = n;
                    break;
                }
                n = n % gcd;
            }
        }
        return gcd;
    }

}
