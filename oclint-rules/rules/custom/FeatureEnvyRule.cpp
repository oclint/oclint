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
        vector<string> analyze(ObjCMethodDecl *decl)
        {
            string selfName = decl->getClassInterface()->getNameAsString();

            _receiversCount.clear();

            TraverseDecl(decl);

            return enviedClasses(self_messages(selfName));
        }

        bool VisitObjCMessageExpr(ObjCMessageExpr *node)
        {
            if (!node->isImplicit()) {
                countInterface(node->getReceiverInterface());
            }
            return true;
        }

        bool VisitObjCIvarRefExpr(ObjCIvarRefExpr *node)
        {
            countInterface(node->getDecl()->getContainingInterface());

            return true;
        }

        // bool VisitObjCPropertyRefExpr(ObjCPropertyRefExpr *node)
        // {
        //     if (node->isClassReceiver()) {
        //         cout << "Class Receiver" << endl;
        //         cout << "Property Receiver: " << node->getClassReceiver()->getNameAsString() << endl;
        //     } else if (node->isObjectReceiver()) {
        //         cout << "Object Receiver" << endl;
        //         node->getBase()->dump();
        //     } else if (node->isSuperReceiver()) {
        //         cout << "Super Receiver" << endl;
        //     }
        //     return true;
        // }

    private:
        void countInterface(const ObjCInterfaceDecl *interface)
        {
            string receiverClassName = interface->getNameAsString();
            ++_receiversCount[receiverClassName];
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
        vector<string> enviedClasses = analyzer.analyze(node);
        for (const auto& enviedClass : enviedClasses)
        {
            string methodName = node->getNameAsString();
            addViolation(node, this, description(methodName, enviedClass));
        }

        return true;
    }

    /* Visit CXXOperatorCallExpr
    bool VisitCXXOperatorCallExpr(CXXOperatorCallExpr *node)
    {
        return true;
    }
     */

    /* Visit CXXMemberCallExpr
    bool VisitCXXMemberCallExpr(CXXMemberCallExpr *node)
    {
        return true;
    }
     */

    /* Visit CXXMethodDecl
    bool VisitCXXMethodDecl(CXXMethodDecl *node)
    {
        return true;
    }
     */

    /* Visit ObjCSelectorExpr
    bool VisitObjCSelectorExpr(ObjCSelectorExpr *node)
    {
        return true;
    }
     */
};

RuleSet FeatureEnvyRule::rules(new FeatureEnvyRule());
