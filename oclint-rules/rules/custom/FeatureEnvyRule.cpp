#include "oclint/AbstractASTVisitorRule.h"
#include "oclint/RuleConfiguration.h"
#include "oclint/RuleSet.h"
#include <unordered_map>

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
                ObjCInterfaceDecl *interface = node->getReceiverInterface();
                string receiverClassName = interface->getNameAsString();
                ++_receiversCount[receiverClassName];
            }
            return true;
        }

    private:
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
    
    /* Visit ObjCMethodDecl */
    bool VisitObjCMethodDecl(ObjCMethodDecl *node)
    {
        MessageAnalyzer analyzer;
        vector<string> enviedClasses = analyzer.analyze(node);
        for (const auto& enviedClass : enviedClasses)
        {
            ostringstream os;
            os << "Method " << node->getNameAsString() << " messages " << enviedClass << " more than self.";
            addViolation(node, this, os.str());
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

    /* Visit ObjCProtocolExpr
    bool VisitObjCProtocolExpr(ObjCProtocolExpr *node)
    {
        return true;
    }
     */

    /* Visit ObjCIvarRefExpr
    bool VisitObjCIvarRefExpr(ObjCIvarRefExpr *node)
    {
        return true;
    }
     */

    /* Visit ObjCPropertyRefExpr
    bool VisitObjCPropertyRefExpr(ObjCPropertyRefExpr *node)
    {
        return true;
    }
     */
};

RuleSet FeatureEnvyRule::rules(new FeatureEnvyRule());
