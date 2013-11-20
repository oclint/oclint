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

bool ObjCMethodDeclHasActionAttribute(
    const clang::ObjCMethodDecl *decl, const std::string& action, const oclint::RuleBase& rule) {
    // Check the method directly
    if(declHasActionAttribute(decl, action, rule)) {
        return true;
    }

    // That failed, check if it has a property declaration and use that
    if(decl->isPropertyAccessor() &&
       declHasActionAttribute(decl->findPropertyDecl(), action, rule)) {
        return true;
    }
  
     // Check if the method comes from a protocol, because then it doesn't have a class
     // interface
     const auto protocol = clang::dyn_cast<clang::ObjCProtocolDecl>(decl->getDeclContext());
     if(protocol) {
         return declHasActionAttribute(protocol->lookupMethod(decl->getSelector(), decl->isInstanceMethod()), action, rule);
     }
 
    // That failed, check if it has a redeclaration in a category and use that
    const auto interface = decl->getClassInterface();
    if(!interface) {
        return false;
    }

    for(auto it = interface->visible_categories_begin(),
             ite = interface->visible_categories_end();
             it != ite;
             ++it) {
        clang::ObjCMethodDecl* categoryMethodDecl =
            (*it)->getMethod(decl->getSelector(), decl->isInstanceMethod());
        if(declHasActionAttribute(categoryMethodDecl, action, rule)) {
            return true;
        }
    }
    return false;

}


