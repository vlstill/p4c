#include "backends/p4tools/common/compiler/convert_hs_index.h"
#include "backends/p4tools/common/lib/variables.h"
#include "fields.h"

namespace P4Tools {

template<typename ResolveType>
std::vector<IR::StateVariable> getFlatFields(
    const IR::Expression *parent, const IR::Type_StructLike *ts, ResolveType resolveType,
    std::vector<IR::StateVariable> *validVector = nullptr) {
    std::vector<IR::StateVariable> flatFields;
    for (const auto *field : ts->fields) {
        const IR::Type *fieldType = resolveType(field->type);
        if (const auto *ts = fieldType->to<IR::Type_StructLike>()) {
            auto subFields =
                getFlatFields(new IR::Member(fieldType, parent, field->name), ts, resolveType,
                              validVector);
            flatFields.insert(flatFields.end(), subFields.begin(), subFields.end());
        } else if (const auto *typeStack = fieldType->to<IR::Type_Stack>()) {
            const IR::Type *stackElementsType = resolveType(typeStack->elementType);
            for (size_t arrayIndex = 0; arrayIndex < typeStack->getSize(); arrayIndex++) {
                const auto *newArr = HSIndexToMember::produceStackIndex(
                    stackElementsType, new IR::Member(typeStack, parent, field->name), arrayIndex);
                BUG_CHECK(stackElementsType->is<IR::Type_StructLike>(),
                          "Try to make the flat fields for non Type_StructLike element : %1%",
                          stackElementsType);
                auto subFields = getFlatFields(newArr, stackElementsType->to<IR::Type_StructLike>(),
                                               resolveType, validVector);
                flatFields.insert(flatFields.end(), subFields.begin(), subFields.end());
            }
        } else {
            flatFields.push_back(new IR::Member(fieldType, parent, field->name));
        }
    }
    // If we are dealing with a header we also include the validity bit in the list of
    // fields.
    if (validVector != nullptr && ts->is<IR::Type_Header>()) {
        validVector->push_back(ToolsVariables::getHeaderValidity(parent));
    }
    return flatFields;
}
std::vector<IR::StateVariable> getFlatFields(
    const IR::Expression *parent, const IR::Type_StructLike *ts, const NamespaceContext *namespaces,
    std::vector<IR::StateVariable> *validVector) {
    return getFlatFields(parent, ts, [=](const IR::Type *t) { return namespaces->resolveType(t); },
                         validVector);
}

std::vector<IR::StateVariable> getFlatFields(
    const IR::Expression *parent, const IR::Type_StructLike *ts, const P4::ReferenceMap *refMap,
    std::vector<IR::StateVariable> *validVector) {
    return getFlatFields(parent, ts, [=](const IR::Type *t) {
            if (const auto *tn = t->to<IR::Type_Name>()) {
                const auto *path = tn->path;
                const auto *decl = refMap->getDeclaration(path, true)->to<IR::Type_Declaration>();
                BUG_CHECK(decl, "Not a type: %1%", path);
                return decl->to<IR::Type>();
            }
            return t;  // no need to resolve
        }, validVector);
}

}  // namespace P4Tools
