# monotinic cubic spline

In ordinary spline interpolation, interpolation curves may overshort. In other words, the interpolation curve for interpolating a monotonically increasing point sequence is not necessarily a monotonically increasing function. Therefore, a more intuitive interpolation is the monotonic cubic spline. Instead of discarding the continuity of the second derivative, which is the usual requirement for spline interpolation curves, the coefficient conditions are given such that the interpolating function is monotonically increasing. The implementation is in C.

Translated with www.DeepL.com/Translator
