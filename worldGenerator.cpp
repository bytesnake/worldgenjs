#include "worldGenerator.h"

worldGenerator::worldGenerator(double p_frequency, int p_height, int p_octaves, int p_seed) 
{
	noiseGen = new Perlin(p_octaves, p_frequency, p_height, p_seed);

	m_height = p_height;
	m_seed = p_seed;
}

double worldGenerator::getNoiseData(int p_x, int p_z) {
	return noiseGen->Get2(p_x,p_z);
}
