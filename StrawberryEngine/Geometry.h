#pragma once

float cubeArray[] = { 1.f, 1.f, 1.f,   0.f, 1.f, 1.f,   0.f, 0.f, 1.f,   0.f, 0.f, 1.f,   1.f, 0.f, 1.f,   1.f, 1.f, 1.f,   1.f, 1.f, 1.f,   1.f, 0.f, 1.f,
					  1.f, 0.f, 0.f,   1.f, 0.f, 0.f,   1.f, 1.f, 0.f,   1.f, 1.f, 1.f }; // Two sides only

float cubeVertices[] = { 3.f, 1.f, 1.f,
						 2.f, 1.f, 1.f,
						 2.f, 0.f, 1.f,
						 3.f, 0.f, 1.f,
						 3.f, 0.f, 0.f,
						 3.f, 1.f, 0.f,
						 2.f, 1.f, 0.f,
						 2.f, 0.f, 0.f };

uint cubeIndices[] = { 0,1,2, 2,3,0,
					   0,3,4, 4,5,0,
					   0,5,6, 6,1,0,
					   1,6,7, 7,2,1,
					   7,4,3, 3,2,7,
					   4,7,6, 6,5,4 };