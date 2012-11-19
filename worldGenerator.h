#ifndef worldGenerator_H
#define worldGenerator_H

#include <iostream>
#include "perlin.h"
class worldGenerator
{

	private:
		int m_seed;
		int m_height;
		Perlin* noiseGen;
	
	public:
	worldGenerator(double p_frequency, int p_height, int p_octaves, int p_seed);
	double getNoiseData(int p_x, int p_z);


};
#endif






