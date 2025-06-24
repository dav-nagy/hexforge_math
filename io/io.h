//
// Created by David on 6/1/2025.
//

#ifndef IO_H
//Include guard
#define IO_H

#include "input_output/ops/cio_out_ops.h"

     inline cio_out cio_out; //Linked to standard output (Use '<<' to chain arguments)
     //For some reason, constexpr will not work here, so we just stick with 'inline'

#endif //IO_H
