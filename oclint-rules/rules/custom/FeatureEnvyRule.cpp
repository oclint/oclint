#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include <unordered_map>
#include <sstream>

using namespace std;
using namespace clang;
using namespace oclint;

class FeatureEnvyRule : public AbstractASTVisitorRule<FeatureEnvyRule>
{
    class MessageAnalyzer : public RecursiveASTVisitor<MessageAnalyzer>
    {
        unordered_map<string, long> _receiversCount;

    public:
        vector<string> analyzeObjC(ObjCMethodDecl *decl)
        {
            string selfName = decl->getClassInterface()->getNameAsString();
            return analyzeCommon(selfName, decl);
        }

        vector<string> analyzeCXX(CXXMethodDecl *decl)
        {
            string selfName = decl->getParent()->getName();
            return analyzeCommon(selfName, decl);
        }

        bool VisitObjCMessageExpr(ObjCMessageExpr *node)
        {
            if (!node->isImplicit())
            {
                switch (node->getReceiverKind())
                {
                case ObjCMessageExpr::Instance:
                    if (node->getReceiverInterface())
                    {
                        countInterface(node->getReceiverInterface());
                    }
                    break;

                case ObjCMessageExpr::Class:
                    {
                        QualType qualType = node->getClassReceiver();
                        countClassName(QualType::getAsString(qualType.split()));
                    }
                    break;

                default:
                    break;
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

        bool VisitCXXMemberCallExpr(CXXMemberCallExpr *node)
        {
            countCXXRecord(node->getRecordDecl());
            return true;
        }

    private:
        vector<string> analyzeCommon(string selfName, Decl *decl)
        {
            _receiversCount.clear();
            TraverseDecl(decl);

            return enviedClasses(self_messages(selfName));
        }

        ObjCInterfaceDecl *getSetterInterface(ObjCPropertyRefExpr *node)
        {
            ObjCInterfaceDecl *result;

            if (node->isExplicitProperty())
            {
                ObjCPropertyDecl *decl = node->getExplicitProperty();
                result = decl->getSetterMethodDecl()->getClassInterface();
            }
            else
            {
                result = node->getImplicitPropertySetter()->getClassInterface();
            }

            return result;
        }

        ObjCInterfaceDecl *getGetterInterface(ObjCPropertyRefExpr *node)
        {
            ObjCInterfaceDecl *result;

            if (node->isExplicitProperty())
            {
                ObjCPropertyDecl *decl = node->getExplicitProperty();
                result = decl->getGetterMethodDecl()->getClassInterface();
            }
            else
            {
                result = node->getImplicitPropertyGetter()->getClassInterface();
            }

            return result;
        }

        void countClassName(string className)
        {
            ++_receiversCount[className];
        }

        void countInterface(const ObjCInterfaceDecl *interface)
        {
            countClassName(interface->getNameAsString());
        }

        void countIvar(ObjCIvarDecl *ivarDecl)
        {
            countInterface(ivarDecl->getContainingInterface());
        }

        void countCXXRecord(CXXRecordDecl *decl)
        {
            countClassName(decl->getName());
        }

        long self_messages(string selfname)
        {
            long occurances = 0;
            for (const auto& pair : _receiversCount)
            {
                if (pair.first == selfname)
                    occurances = pair.second;
            }

            return occurances;
        }

        vector<string>enviedClasses(long selfMessages)
        {
            vector<string> enviedClasses;
            enviedClasses.clear();

            for (const auto& pair : _receiversCount)
            {
                if (pair.second > selfMessages)
                    enviedClasses.push_back(pair.first);
            }

            return enviedClasses;
        }
    };

private:
    static RuleSet rules;

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
    virtual const string name() const
    {
        return "feature envy";
    }

    virtual int priority() const
    {
        return 3;
    }

    virtual void setUp() {}
    virtual void tearDown() {}
    
    bool VisitObjCMethodDecl(ObjCMethodDecl *node)
    {
        MessageAnalyzer analyzer;
        addViolationsForEnviedClasses(node, analyzer.analyzeObjC(node));

        return true;
    }

    bool VisitCXXMethodDecl(CXXMethodDecl *node)
    {
        MessageAnalyzer analyzer;
        addViolationsForEnviedClasses(node, analyzer.analyzeCXX(node));

        return true;
    }
};

RuleSet FeatureEnvyRule::rules(new FeatureEnvyRule());
