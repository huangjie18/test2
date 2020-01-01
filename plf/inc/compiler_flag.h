#ifndef COMPILER_FLAG_H_
#define COMPILER_FLAG_H_


#define USED_FLAG __attribute__((used))
#define XIP_SECTION __attribute__((section("xip_section")))
#define N_XIP_SECTION __attribute__((section("n_xip_section")))
#define N_XIP_VARIABLE __attribute__((section("n_xip_variable")))
#endif
