#monotinic cubic spline
####通常のスプライン補間では補間曲線がオーバーショートする可能性がある．すなわち単調増加する点列を補間する補間曲線が単調増加関数になるとは限らない．
そこでより直感的な補間を実現したのがmonotonic cubic spline(単調スプライン)である．
これはスプライン補間の要件である二次の導関数の連続性を捨てる代わりに単調増加になるような条件を与えたものである．具体的な式と証明は省略する