(* ::Package:: *)

b1 = {1, 0, 0};
b2 = {0, 1, 0};
b3 = {0, 0, 1};
basis = {b1, b2, b3};


r1 = RotationMatrix[24, {1, 0, 0}];
r2 = RotationMatrix[52, {0, 1, 0}];
r3 = RotationMatrix[87, {0, 0, 1}];
v1 = {1, 1, 1};
v2 = r1 . r2 . r3 . v1 // N


basisArrows = {Red, Arrow[{{0, 0, 0}, b1}], Green, Arrow[{{0, 0, 0}, b2}], Blue, Arrow[{{0, 0, 0}, b3}]};
vectors = {v1, v2};
arrows = {Black, Arrow[{{0, 0, 0}, #}]} & /@ vectors // Flatten;
Graphics3D[Join[basisArrows, arrows], PlotRange -> {{-2, 2}, {-2, 2}, {-2, 2}}, Axes -> True, AxesLabel -> {"X", "Y", "Z"}]
