#include <clang/AST/Attr.h>

#include "oclint/helper/SuppressHelper.h"

bool markedAsSuppress(clang::Decl *decl, oclint::RuleBase *rule)
{
    if (!decl)
    {
        return false;
    }

    for (clang::specific_attr_iterator<clang::AnnotateAttr> attr =
        decl->specific_attr_begin<clang::AnnotateAttr>(),
        attrEnd = decl->specific_attr_end<clang::AnnotateAttr>();
        attr != attrEnd;
        attr++)
    {
        clang::AnnotateAttr *annotate = clang::dyn_cast<clang::AnnotateAttr>(*attr);
        if (annotate && (annotate->getAnnotation() == "oclint:suppress" ||
            annotate->getAnnotation() == "oclint:suppress[" + rule->name() + "]"))
        {
            return true;
        }
    }

    return false;
}
