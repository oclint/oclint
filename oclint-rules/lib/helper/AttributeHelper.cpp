#include "oclint/helper/AttributeHelper.h"

bool declHasAttribute(const clang::Decl *decl, const std::string& attributeName) {
    if (!decl)
    {
        return false;
    }

    for (auto attr = decl->specific_attr_begin<clang::AnnotateAttr>(),
        attrEnd = decl->specific_attr_end<clang::AnnotateAttr>();
        attr != attrEnd;
        ++attr)
    {
        clang::AnnotateAttr *annotate = clang::dyn_cast<clang::AnnotateAttr>(*attr);
        if (annotate && annotate->getAnnotation() == attributeName)
        {
            return true;
        }
    }

    return false;
}

bool declHasOCLintAttribute(const clang::Decl *decl, const std::string& attributeName) {
    return declHasAttribute(decl, "oclint:" + attributeName);
}

bool declHasActionAttribute(
    const clang::Decl *decl, const std::string& action, oclint::RuleBase* rule) {
    return declHasOCLintAttribute(decl, action + "[" + rule->attributeName() + "]");
}

bool declHasEnforceAttribute(const clang::Decl *decl, oclint::RuleBase* rule) {
    return declHasActionAttribute(decl, "enforce", rule);
}
