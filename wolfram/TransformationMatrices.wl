(* ::Package:: *)

WTranslateMatrix = {
	{1, 0, 0, #1},
	{0, 1, 0, #2},
	{0, 0, 1, #3},
	{0, 0, 0, 1}
} &;
WScaleMatrix = {
	{#1, 0, 0, 0},
	{0, #2, 0, 0},
	{0, 0, #3, 0},
	{0, 0, 0, 1}
} &;


Vec = {10, 10, 10, 1};
Vec2 = WTranslateMatrix[10, 0, 0] . Vec
Vec3 = WScaleMatrix[2, 2, 2] . Vec2


(*
Model Coordinates --(Model Matrix)-->
	World Coordinates --(View Matrix)-->
		Camera Coordinates --(Projection Matrix)-->
			Projective Coordinates

Model Coordinates - coordinate system placed in the center of a shape. After we apply
translation, scale or rotation we get to World Coordinates.
World Coordinates - camera is located in the center of world coordinates. If we want to
move / rotate the camera - we need to move / rotate the whole world. After that we
get to Camera Coordinates.
Camera Coordinates - we need to project objects to the screen. For that we apply the matrix
that makes perspective projection. After that we get to Projective Coordinates.
*)
