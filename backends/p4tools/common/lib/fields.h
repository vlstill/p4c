#ifndef BACKENDS_P4TOOLS_COMMON_LIB_FIELDS_H_
#define BACKENDS_P4TOOLS_COMMON_LIB_FIELDS_H_

#include <vector>

#include "ir/ir.h"
#include "frontends/common/resolveReferences/referenceMap.h"

#include "namespace_context.h"

namespace P4Tools {

/// Takes an input struct type @ts and a prefix @parent and returns a vector of references to
/// members of the struct. The vector contains all the Type_Base (bit and bool) members in
/// canonical representation (e.g.,
/// {"prefix.h.ethernet.dst_address", "prefix.h.ethernet.src_address", ...}). If @arg
/// validVector is provided, this function also collects the validity bits of the headers.
[[nodiscard]] std::vector<IR::StateVariable> getFlatFields(
    const IR::Expression *parent, const IR::Type_StructLike *ts, const NamespaceContext *namespaces,
    std::vector<IR::StateVariable> *validVector = nullptr);

/// Takes an input struct type @ts and a prefix @parent and returns a vector of references to
/// members of the struct. The vector contains all the Type_Base (bit and bool) members in
/// canonical representation (e.g.,
/// {"prefix.h.ethernet.dst_address", "prefix.h.ethernet.src_address", ...}). If @arg
/// validVector is provided, this function also collects the validity bits of the headers.
[[nodiscard]] std::vector<IR::StateVariable> getFlatFields(
    const IR::Expression *parent, const IR::Type_StructLike *ts, const P4::ReferenceMap *refMap,
    std::vector<IR::StateVariable> *validVector = nullptr);

}  // namespace P4Tools
#endif  // BACKENDS_P4TOOLS_COMMON_LIB_FIELDS_H_
