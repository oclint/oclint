#include <clang/AST/Attr.h>

#include "oclint/helper/SuppressHelper.h"

bool markedAsSuppress(Decl *decl, RuleBase *rule)
{
    if (!decl)
    {
        return false;
    }

    for (specific_attr_iterator<AnnotateAttr> attr = decl->specific_attr_begin<AnnotateAttr>(),
        attrEnd = decl->specific_attr_end<AnnotateAttr>(); attr != attrEnd; attr++)
    {
        AnnotateAttr *annotate = dyn_cast<AnnotateAttr>(*attr);
        if (annotate && (annotate->getAnnotation() == "oclint:suppress" ||
            annotate->getAnnotation() == "oclint:suppress[" + rule->name() + "]"))
        {
            return true;
        }
    }

    return false;
}
