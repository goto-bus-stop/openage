#ifndef _NYAN_NYAN_H_
#define _NYAN_NYAN_H_

#include "nyan_spec.h"
#include "nyan_spec_analyzer.h"
#include "nyan_spec_parser.h"
#include "tokenizer.h"

namespace nyan {

std::unique_ptr<NyanSpec> parse_spec(const std::string &input);

}

#endif
