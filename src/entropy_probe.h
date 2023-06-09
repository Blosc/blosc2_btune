#ifndef BLOSC2_EP_H
#define BLOSC2_EP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <blosc2.h>

#define ENTROPY_PROBE_ID 244
void register_entropy_codec(blosc2_codec *codec);
#define FILTER_STOP 3
float get_zeros_speed(int32_t chunksize);
#ifdef __cplusplus
}
#endif

#endif
