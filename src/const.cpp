// Global variables to be used
#include "stdafx.h"
#include "const.h"

int cuadrants[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
int scanning_cuadrants[] = { 15,14,9,13,8,12,6 };
//int scanning_cuadrants[] = { 10,14,8,12,6 };

// Accuracy parameters:
double minArea = 10000.0;
double rratio = 1.1;

// Similarities for each cuadrant
double c1_similarity;
double c2_similarity;
double c3_similarity;
double c4_similarity;
double c5_similarity;
double c6_similarity;
double c7_similarity;
double c8_similarity;
double c9_similarity;
double c10_similarity;
double c11_similarity;
double c12_similarity;
double c13_similarity;
double c14_similarity;
double c15_similarity;
double c16_similarity;

// Thresholds for each cuadrant
double c1_threshold;
double c2_threshold;
double c3_threshold;
double c4_threshold;
double c5_threshold;
double c6_threshold;
double c7_threshold;
double c8_threshold;
double c9_threshold;
double c10_threshold;
double c11_threshold;
double c12_threshold;
double c13_threshold;
double c14_threshold;
double c15_threshold;
double c16_threshold;

//Functions to handle the cuadrants
double GetCuadrantSimilarity(int cuadrant)
{
	switch (cuadrant)
	{
	case 1: return c1_similarity;
	case 2: return c2_similarity;
	case 3: return c3_similarity;
	case 4: return c4_similarity;
	case 5: return c5_similarity;
	case 6: return c6_similarity;
	case 7: return c7_similarity;
	case 8: return c8_similarity;
	case 9: return c9_similarity;
	case 10: return c10_similarity;
	case 11: return c11_similarity;
	case 12: return c12_similarity;
	case 13: return c13_similarity;
	case 14: return c14_similarity;
	case 15: return c15_similarity;
	case 16: return c16_similarity;

	default:
		cout << "Error when retrieving the similarity of Cuadrant: " << cuadrant << endl;
	}
}

void SetCuadrantSimilarity(int cuadrant, double similarity)
{
	switch (cuadrant)
	{
	case 1: 
	{
		c1_similarity = similarity;
		break;
	}
	case 2:
	{
		c2_similarity = similarity;
		break;
	}
	case 3:
	{
		c3_similarity = similarity;
		break;
	}
	case 4:
	{
		c4_similarity = similarity;
		break;
	}
	case 5:
	{
		c5_similarity = similarity;
		break;
	}
	case 6:
	{
		c6_similarity = similarity;
		break;
	}
	case 7: 
	{
		c7_similarity = similarity;
		break;
	}
	case 8:
	{
		c8_similarity = similarity;
		break;
	}
	case 9:
	{
		c9_similarity = similarity;
		break;
	}
	case 10:
	{
		c10_similarity = similarity;
		break;
	}
	case 11:
	{
		c11_similarity = similarity;
		break;
	}
	case 12:
	{
		c12_similarity = similarity;
		break;
	}
	case 13:
	{
		c13_similarity = similarity;
		break;
	}
	case 14:
	{
		c14_similarity = similarity;
		break;
	}
	case 15:
	{
		c15_similarity = similarity;
		break;
	}
	case 16:
	{
		c16_similarity = similarity;
		break;
	}
	default:
		cout << "Error when setting the similiraty of cuadrant: " << cuadrant << endl;
	}
}

double GetCuadrantThreshold(int cuadrant)
{
	switch (cuadrant)
	{
	case 1: return c1_threshold;
	case 2: return c2_threshold;
	case 3: return c3_threshold;
	case 4: return c4_threshold;
	case 5: return c5_threshold;
	case 6: return c6_threshold;
	case 7: return c7_threshold;
	case 8: return c8_threshold;
	case 9: return c9_threshold;
	case 10: return c10_threshold;
	case 11: return c11_threshold;
	case 12: return c12_threshold;
	case 13: return c13_threshold;
	case 14: return c14_threshold;
	case 15: return c15_threshold;
	case 16: return c16_threshold;
	}
}

// Needs to be fixed
void SetCuadrantThreshold(int cuadrant, double threshold) 
{
	switch (cuadrant)
	{
	case 1: c1_threshold = threshold;
	case 2: c2_threshold = threshold;
	case 3: c3_threshold = threshold;
	case 4: c4_threshold = threshold;
	case 5: c5_threshold = threshold;
	case 6: c6_threshold = threshold;
	case 7: c7_threshold = threshold;
	case 8: c8_threshold = threshold;
	case 9: c9_threshold = threshold;
	case 10: c10_threshold = threshold;
	case 11: c11_threshold = threshold;
	case 12: c12_threshold = threshold;
	case 13: c13_threshold = threshold;
	case 14: c14_threshold = threshold;
	case 15: c15_threshold = threshold;
	case 16: c16_threshold = threshold;
	}
}