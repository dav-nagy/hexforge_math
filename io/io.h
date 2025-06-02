//
// Created by David on 6/1/2025.
//

#ifndef IO_H
//Include guard
#define IO_H

//Currently for testing / this will not stay
#define io hexforge_io

#include "input/ops/cio_out_ops.h"
#include "input/write/str_write.h"

namespace hexforge_io {
     inline hexforge_cio_out::cio_out cio_out; //Linked to standard output (Use '<<' to chain arguments)
     //For some reason, constexpr will not work here, so we just stick with 'inline'
}

#endif //IO_H
