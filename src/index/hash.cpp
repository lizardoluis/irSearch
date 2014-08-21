/*
 * hash.cpp
 *
 *  Created on: Mar 9, 2014
 *      Author: lizardo
 */

#include "hash.h"

Hash::Hash(char **keys, unsigned int nkeys, string mphPath) {

	mphf_fd = fopen(mphPath.c_str(), "w");

//	printf("%u\n", nkeys);
	cmph_io_adapter_t *source = cmph_io_vector_adapter(keys, nkeys);

	//Create minimal perfect hash function using the bdz algorithm.
	cmph_config_t *config = cmph_config_new(source);
	cmph_config_set_algo(config, CMPH_BDZ);
	cmph_config_set_mphf_fd(config, mphf_fd);

	cmph = cmph_new(config);

	cmph_config_destroy(config);
	cmph_io_vector_adapter_destroy(source);
	cmph_dump(cmph, mphf_fd);
	cmph_destroy(cmph);
	fclose(mphf_fd);


	mphf_fd = fopen(mphPath.c_str(), "r");
	cmph = cmph_load(mphf_fd);
}

Hash::~Hash() {
	cmph_destroy(cmph);

	fclose(mphf_fd);
}

size_t Hash::search(string key) {

	unsigned int id = cmph_search(cmph, key.c_str(),
			(cmph_uint32) key.length());

	return id;
}

size_t Hash::getSize() {
	return cmph_size(cmph);
}
