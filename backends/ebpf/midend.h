#ifndef _BACKENDS_EBPF_MIDEND_H_
#define _BACKENDS_EBPF_MIDEND_H_

#include "ir/ir.h"
#include "ebpfOptions.h"

namespace EBPF {

class MidEnd {
    std::vector<DebugHook> hooks;
 public:
    void addDebugHook(DebugHook hook) { hooks.push_back(hook); }
    const IR::P4Program* run(EbpfOptions& options, const IR::P4Program* program);
};

}  // namespace EBPF

#endif /* _BACKENDS_EBPF_MIDEND_H_ */
