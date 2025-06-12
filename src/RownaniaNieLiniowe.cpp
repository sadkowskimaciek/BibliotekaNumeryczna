#include "../include/numlib/RownaniaNieLiniowe.h"
#include <cmath>
#include <algorithm>
#include <string>


namespace numlib {
    Result bisection(std::function<double(double)> f, double a, double b, double tolerance, int maxIterations) {
        Result result;
        result.converged = false;

        if (f(a) * f(b) > 0) {
            return result;
        }

        double c;
        for (int i = 0; i < maxIterations; i++) {
            c = (a + b) / 2.0;
            result.approximations.push_back(c);

            if (std::abs(f(c)) < tolerance || (b - a) / 2.0 < tolerance) {
                result.root = c;
                result.iterations = i + 1;
                result.error = std::abs(b - a) / 2.0;
                result.converged = true;
                break;
            }

            if (f(c) * f(a) < 0) {
                b = c;
            } else {
                a = c;
            }
        }

        if (!result.converged) {
            result.root = c;
            result.iterations = maxIterations;
            result.error = std::abs(b - a) / 2.0;
        }

        return result;
    }

    Result newton(std::function<double(double)> f, std::function<double(double)> df, double x0, double tolerance, int maxIterations) {
        Result result;
        result.converged = false;

        double x = x0;
        double x_prev;

        for (int i = 0; i < maxIterations; i++) {
            result.approximations.push_back(x);

            double fx = f(x);
            double dfx = df(x);

            if (std::abs(dfx) < 1e-12) {
                break;
            }

            x_prev = x;
            x = x - fx / dfx;

            if (std::abs(x - x_prev) < tolerance) {
                result.root = x;
                result.iterations = i + 1;
                result.error = std::abs(x - x_prev);
                result.converged = true;
                break;
            }
        }

        if (!result.converged) {
            result.root = x;
            result.iterations = maxIterations;
            result.error = std::abs(x - x_prev);
        }

        return result;
    }

    Result secant(std::function<double(double)> f, double x0, double x1, double tolerance, int maxIterations) {
        Result result;
        result.converged = false;

        double x_prev = x0;
        double x_curr = x1;
        double x_next;

        for (int i = 0; i < maxIterations; i++) {
            result.approximations.push_back(x_curr);

            double f_prev = f(x_prev);
            double f_curr = f(x_curr);

            if (std::abs(f_curr - f_prev) < 1e-12) {
                break;
            }

            x_next = x_curr - f_curr * (x_curr - x_prev) / (f_curr - f_prev);

            if (std::abs(x_next - x_curr) < tolerance) {
                result.root = x_next;
                result.iterations = i + 1;
                result.error = std::abs(x_next - x_curr);
                result.converged = true;
                break;
            }

            x_prev = x_curr;
            x_curr = x_next;
        }

        if (!result.converged) {
            result.root = x_next;
            result.iterations = maxIterations;
            result.error = std::abs(x_next - x_curr);
        }

        return result;
    }

    std::vector<std::pair<double, double>> findSignChangeIntervals(std::function<double(double)> f, double start, double end, double step) {
        std::vector<std::pair<double, double>> intervals;

        for (double x = start; x < end - step; x += step) {
            double f1 = f(x);
            double f2 = f(x + step);

            if (!std::isfinite(f1) || !std::isfinite(f2)) continue;

            if (f1 * f2 < 0) {
                intervals.push_back({x, x + step});
            }
        }

        return intervals;
    }

    std::vector<double> removeDuplicates(std::vector<double> roots, double tolerance) {
        if (roots.empty()) return roots;

        std::sort(roots.begin(), roots.end());
        std::vector<double> unique_roots;
        unique_roots.push_back(roots[0]);

        for (size_t i = 1; i < roots.size(); i++) {
            if (std::abs(roots[i] - unique_roots.back()) > tolerance) {
                unique_roots.push_back(roots[i]);
            }
        }

        return unique_roots;
    }

    std::vector<double> findAllRoots(std::function<double(double)> f, std::function<double(double)> df, double start, double end, const std::string& method, double step) {
        std::vector<double> all_roots;

        if (method == "bisection") {
            auto intervals = findSignChangeIntervals(f, start, end, step);
            for (auto& interval : intervals) {
                Result result = bisection(f, interval.first, interval.second, 1e-8, 1000);
                if (result.converged) {
                    all_roots.push_back(result.root);
                }
            }
        }
        else if (method == "newton") {
            for (double x0 = start; x0 <= end; x0 += 0.2) {
                Result result = newton(f, df, x0, 1e-8, 1000);
                if (result.converged && result.root >= start && result.root <= end) {
                    all_roots.push_back(result.root);
                }
            }
        }
        else if (method == "secant") {
            for (double x0 = start; x0 <= end - 0.2; x0 += 0.3) {
                for (double x1 = x0 + 0.1; x1 <= std::min(x0 + 0.5, end); x1 += 0.2) {
                    Result result = secant(f, x0, x1, 1e-8, 1000);
                    if (result.converged && result.root >= start && result.root <= end) {
                        all_roots.push_back(result.root);
                    }
                }
            }
        }

        return removeDuplicates(all_roots,2);
    }
}