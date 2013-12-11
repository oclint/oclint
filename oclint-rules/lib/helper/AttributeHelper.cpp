#include "oclint/helper/AttributeHelper.h"

#include <clang/AST/AST.h>
#include <clang/AST/Attr.h>

#include "oclint/RuleBase.h"

bool attributeHasAnnotation(
    const std::string& annotation,
    const std::string& attributeName,
    std::string* comment) {
    // Unqualified actions must compare directly e.g. we don't want to return a match for 
    // for 'oclint:suppress' when the annotation is 'oclint::suppress[foo]'
    if(attributeName.find('[') == std::string::npos) {
        return attributeName == annotation;
    }
    
    // Otherwise, check if the attributeName is a prefix of the annotation
    // We need to check prefix and not equality in case there's a comment
    if(annotation.length() < attributeName.length() ||
        !std::equal(attributeName.begin(), attributeName.end(), annotation.begin())) {
        return false;
    }
    // The attributes match.
    // Try to pick out a comment if we need to
    if(comment != nullptr) {
        const auto commentStart = annotation.find('[', attributeName.length());
    
        if(commentStart != std::string::npos && annotation.back() == ']') {
            *comment =
               annotation.substr(commentStart + 1, annotation.length() - commentStart - 2);
        }
    }
    return true;
}

bool declHasAttribute(
    const clang::Decl *decl,
    const std::string& attributeName,
    std::string* comment) {
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
        if (!annotate) {
            continue;
        }
        
        const std::string annotation = annotate->getAnnotation();
        if(attributeHasAnnotation(annotation, attributeName, comment)) {
            return true;
        }
    }

    return false;
}

bool declHasOCLintAttribute(
    const clang::Decl *decl,
    const std::string& attributeName,
    std::string* comment) {
    return declHasAttribute(decl, "oclint:" + attributeName, comment);
}

bool baseDeclHasActionAttributeImpl (
    const clang::Decl *decl,
    const std::string& action,
    const oclint::RuleBase& rule,
    std::string* comment) {
    return declHasOCLintAttribute(decl, action + "[" + rule.attributeName() + "]", comment);
}

bool objCMethodDeclHasAttributeFromCategory(
    const clang::ObjCMethodDecl *decl,
    const std::string& action,
    const oclint::RuleBase& rule,
    std::string* comment) {
    // If the method is already from a category, we don't need to traverse any other categories
    if(clang::dyn_cast<clang::ObjCCategoryDecl>(decl->getDeclContext())) {
        return false;
    }

    // But if we're not, check if it has a redeclaration in a category and use that
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
        if(declHasActionAttribute(categoryMethodDecl, action, rule, comment)) {
            return true;
        }
    }
    return false;
}

bool objCMethodDeclHasActionAttributeImpl(
    const clang::ObjCMethodDecl *decl,
    const std::string& action,
    const oclint::RuleBase& rule,
    std::string* comment) {
    if(decl == nullptr) {
        return false;
    }

    // Check the method directly
    if(baseDeclHasActionAttributeImpl(decl, action, rule, comment)) {
        return true;
    }

    // That failed, check if it has a property declaration and use that
    if(decl->isPropertyAccessor() &&
       baseDeclHasActionAttributeImpl(decl->findPropertyDecl(), action, rule, comment)) {
        return true;
    }
  
    // Check if the method comes from a protocol, because then it doesn't have a class
    // interface
    const auto protocol = clang::dyn_cast<clang::ObjCProtocolDecl>(decl->getDeclContext());
    if(protocol) {
       const auto method = protocol->lookupMethod(decl->getSelector(), decl->isInstanceMethod());
       return baseDeclHasActionAttributeImpl(method, action, rule, comment);
    }
 
    return objCMethodDeclHasAttributeFromCategory(decl, action, rule, comment);

}

bool declHasActionAttribute(
    const clang::Decl *decl,
    const std::string& action,
    const oclint::RuleBase& rule,
    std::string* comment) {
    const clang::ObjCMethodDecl* method = clang::dyn_cast_or_null<clang::ObjCMethodDecl>(decl);
    if(method) {
        return objCMethodDeclHasActionAttributeImpl(method, action, rule, comment);
    }
    return baseDeclHasActionAttributeImpl(decl, action, rule, comment);
}

