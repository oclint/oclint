#include "oclint/helper/AttributeHelper.h"

#include <clang/AST/AST.h>
#include <clang/AST/Attr.h>

#include "oclint/RuleBase.h"

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
        const clang::AnnotateAttr *annotate = clang::dyn_cast<clang::AnnotateAttr>(*attr);
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
    const clang::Decl *decl, const std::string& action, const oclint::RuleBase& rule) {
    return declHasOCLintAttribute(decl, action + "[" + rule.attributeName() + "]");
}

bool declHasEnforceAttribute(const clang::Decl *decl, const oclint::RuleBase& rule) {
    return declHasActionAttribute(decl, "enforce", rule);
}
