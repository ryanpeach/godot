#include <cmath>

namespace kepler {

    /**
    Provides a starting value to solve Kepler's equation.
    See "A Practical Method for Solving the Kepler Equation", Marc A. Murison, 2006

    @param e the eccentricity of the orbit
    @param M mean anomaly (in radians)
    @return starting value for the eccentric anomaly.
    */
    double keplerstart3(double e, double M) {
        double t34 = e * e;
        double t35 = e * t34;
        double t33 = std::cos(M);
        return M + (-0.5 * t35 + e + (t34 + 1.5 * t33 * t35) * t33) * std::sin(M);
    }


    /**
    An iteration (correction) method to solve Kepler's equation.
    See "A Practical Method for Solving the Kepler Equation", Marc A. Murison, 2006

    @param e the eccentricity of the orbit
    @param M mean anomaly (in radians)
    @param x starting value for the eccentric anomaly
    @return corrected value for the eccentric anomaly
    */
    double eps3(const double e, const double M, const double x) {
        double t1 = std::cos(x);
        double t2 = -1 + e * t1;
        double t3 = std::sin(x);
        double t4 = e * t3;
        double t5 = -x + t4 + M;
        double t6 = t5 / (0.5 * t5 * t4 / t2 + t2);

        return t5 / ((0.5 * t3 - 1 / 6 * t1 * t6) * e * t6 + t2);
    }

    /**
    Calculates the eccentric anomaly at time t by solving Kepler's equation.
    See "A Practical Method for Solving the Kepler Equation", Marc A. Murison, 2006

    @ref https://gist.githubusercontent.com/j-faria/1fd079e677325ce820971d9d5d286dad/raw/7e2e6bf112e8c34067f0423a522113ca2daf3003/kepler.cpp

    @param t the time at which to calculate the eccentric anomaly.
    @param period the orbital period of the planet
    @param ecc the eccentricity of the orbit
    @param t_peri time of periastron passage
    @return eccentric anomaly.
    */
    double ecc_anomaly(const double ecc, const double mean_anomaly) {
        double tol;
        if (ecc < 0.8) tol = 1e-14;
        else tol = 1e-13;

        double Mnorm = std::fmod(mean_anomaly, 2.0 * M_PI);
        double E0 = keplerstart3(ecc, Mnorm);
        double dE = tol + 1;
        double E;
        int count = 0;
        while (dE > tol) {
            E = E0 - eps3(ecc, Mnorm, E0);
            dE = std::abs(E - E0);
            E0 = E;
            count++;
            // failed to converge, this only happens for nearly parabolic orbits
            if (count == 100) break;
        }
        return E;
    }
}
