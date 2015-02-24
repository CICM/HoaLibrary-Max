/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef DEF_HOA_COMMON_MAX
#define DEF_HOA_COMMON_MAX

#include "../hoa.max.h"

#ifdef HOA_PACKED_LIB
int c_convolve_main();
int c_freeverb_main();
int hoa_connect_main();
int hoa_dac_main();
int hoa_gain_main();
int hoa_in_main();
int hoa_in_sig_main();
int hoa_map_gui_main();
int hoa_out_main();
int hoa_out_sig_main();
int hoa_pi_main();
int hoa_pi_sig_main();
int hoa_play_main();
int hoa_process_main();
int hoa_record_main();
int hoa_thisprocess_main();
#endif

using namespace hoa;

#endif
