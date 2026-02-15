# monotinic cubic spline

Standard spline interpolation often suffers from "overshoot." In other words, an interpolation curve derived from a monotonically increasing set of data points is not guaranteed to be a monotonically increasing function itself.

To achieve a more intuitive result, monotonic cubic spline interpolation was developed. This method sacrifices the continuity of the second derivative—a standard requirement for cubic splines—in exchange for specific coefficient constraints that ensure the resulting function remains monotonic.
