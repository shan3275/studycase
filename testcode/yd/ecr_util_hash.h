/*
 * ecr_util_hash.h
 *
 *  Created on: Nov 13, 2015
 *      Author: velna
 */

#ifndef ECR_UTIL_HASH
#define ECR_UTIL_HASH

void ecr_crc32_mix(const unsigned char *s, unsigned int len, void * out) ;

void ecr_crc32_hash_mix(const void *s, int len, uint32_t seed, void *out) ;

uint32_t ecr_crc32_ch(uint32_t crc_magic, char ch) ;

void ecr_murmur_hash3_x86_32(const void * key, int len, uint32_t seed, void * out) ;

void ecr_murmur_hash3_x86_128(const void * key, const int len, uint32_t seed, void * out) ;

void ecr_murmur_hash3_x64_128(const void * key, const int len, const uint32_t seed, void * out) ;

uint64_t ecr_murmur_hash2_x64(const void * key, int len, uint32_t seed) ;

#endif
