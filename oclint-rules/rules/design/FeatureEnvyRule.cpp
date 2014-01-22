#include <sstream>
#include <unordered_map>

#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"

using namespace std;
using namespace clang;
using namespace oclint;

// TODO Count ObjC fast enumeration as a message

class FeatureEnvyRule : public AbstractASTVisitorRule<FeatureEnvyRule>
{
    class MessageAnalyzer : public RecursiveASTVisitor<MessageAnalyzer>
    {
        unordered_map<string, long> _receiversCount;

    public:
        vector<string> analyzeObjC(ObjCMethodDecl *decl)
        {
            ObjCInterfaceDecl *interface = getClassInterface(decl);
            if (interface)
            {
                return analyzeCommon(interface->getNameAsString(), decl);
            }
            return vector<string>();
        }

        // vector<string> analyzeCXX(CXXMethodDecl *decl)
        // {
        //     string selfName = decl->getParent()->getName();
        //     return analyzeCommon(selfName, decl);
        // }

        bool VisitObjCMessageExpr(ObjCMessageExpr *node)
        {
            if (!node->isImplicit())
            {
                if (node->getReceiverKind() == ObjCMessageExpr::Instance)
                {
                    if (node->getReceiverInterface())
                    {
                        countInterface(node->getReceiverInterface());
                    }
                }
                else if (node->getReceiverKind() == ObjCMessageExpr::Class)
                {
                    QualType qualType = node->getClassReceiver();
                    countClassName(QualType::getAsString(qualType.split()));
                }
            }
            return true;
        }

        bool VisitObjCIvarRefExpr(ObjCIvarRefExpr *node)
        {
            countIvar(node->getDecl());

            return true;
        }

        bool VisitObjCPropertyRefExpr(ObjCPropertyRefExpr *node)
        {
            if (node->isMessagingSetter())
            {
                countInterface(getSetterInterface(node));
            }
            else
            {
                countInterface(getGetterInterface(node));
            }

            return true;
        }

        // bool VisitCXXMemberCallExpr(CXXMemberCallExpr *node)
        // {
        //     countCXXRecord(node->getRecordDecl());
        //     return true;
        // }

    private:
        vector<string> analyzeCommon(string selfName, Decl *decl)
        {
            _receiversCount.clear();
            TraverseDecl(decl);

            return enviedClasses(selfMessages(selfName));
        }

        ObjCInterfaceDecl *getClassInterface(ObjCMethodDecl *decl)
        {
            if (!decl || isa<ObjCProtocolDecl>(decl->getDeclContext()))
            {
                return nullptr;
            }
            return decl->getClassInterface();
        }

        ObjCInterfaceDecl *getSetterInterface(ObjCPropertyRefExpr *node)
        {
            if (node->isExplicitProperty())
            {
                ObjCPropertyDecl *decl = node->getExplicitProperty();
                return getClassInterface(decl->getSetterMethodDecl());
            }
            return getClassInterface(node->getImplicitPropertySetter());
        }

        ObjCInterfaceDecl *getGetterInterface(ObjCPropertyRefExpr *node)
        {
            if (node->isExplicitProperty())
            {
                ObjCPropertyDecl *decl = node->getExplicitProperty();
                return getClassInterface(decl->getGetterMethodDecl());
            }
            return getClassInterface(node->getImplicitPropertyGetter());
        }

        void countClassName(const string& className)
        {
            ++_receiversCount[className];
        }

        void countInterface(const ObjCInterfaceDecl *interface)
        {
            if (interface)
            {
                countClassName(interface->getNameAsString());
            }
        }

        void countIvar(ObjCIvarDecl *ivarDecl)
        {
            countInterface(ivarDecl->getContainingInterface());
        }

        // void countCXXRecord(CXXRecordDecl *decl)
        // {
        //     countClassName(decl->getName());
        // }

        long selfMessages(string selfname)
        {
            const auto& found = _receiversCount.find(selfname);
            if (found == _receiversCount.end()) {
                return 0;
            }
            return found->second;
        }

        vector<string> enviedClasses(long selfMessages)
        {
            vector<string> enviedClasses;

            for (const auto& pair : _receiversCount)
            {
                if (pair.second > selfMessages)
                    enviedClasses.push_back(pair.first);
            }

            return enviedClasses;
        }
    };

private:
    string description(string methodName, string enviedClass)
    {
        ostringstream stream;
        stream << "Method " <<  methodName << " messages " << enviedClass << " more than self.";
        return stream.str();
    }

    void addViolationsForEnviedClasses(NamedDecl *node, vector<string> enviedClasses)
    {
        for (const auto& enviedClass : enviedClasses)
        {
            string methodName = node->getNameAsString();
            addViolation(node, this, description(methodName, enviedClass));
        }
    }

public:
    virtual const string name() const override
    {
        return "feature envy";
    }

    virtual int priority() const override
    {
        return 3;
    }

    bool VisitObjCMethodDecl(ObjCMethodDecl *node)
    {
        MessageAnalyzer analyzer;
        addViolationsForEnviedClasses(node, analyzer.analyzeObjC(node));

        return true;
    }

    // Disable C++ for now
    // bool VisitCXXMethodDecl(CXXMethodDecl *node)
    // {
    //     MessageAnalyzer analyzer;
    //     addViolationsForEnviedClasses(node, analyzer.analyzeCXX(node));
    //
    //     return true;
    // }
};

static RuleSet rules(new FeatureEnvyRule());
