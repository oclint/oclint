#include <vector>
#include <string>
#include <gtest/gtest.h>

using namespace std;

namespace llvm
{
    class Twine;
}
using llvm::Twine;

namespace oclint
{
    class RuleBase;
    class ViolationSet;
    class Violation;
    class RuleCarrier;
}
using namespace oclint;

bool computeViolationSet(const Twine &fileName,
    RuleBase *rule,
    const std::string &code,
    const std::vector<std::string> &args,
    ViolationSet *violationSet);

void validateViolation(const Violation& violation,
    int expectStartLine,
    int expectStartColumn,
    int expectEndLine,
    int expectEndColumn,
    const string& expectMessage = "");

void testRuleOnCode(const Twine &fileName,
    RuleBase *rule,
    const string &code,
    const std::vector<std::string> &args,
    int violationIndex,
    int startLine,
    int startColumn,
    int endLine,
    int endColumn,
    const string& message = "");

void testRuleOnCode(RuleBase *rule,
    const string &code,
    int violationIndex,
    int expectStartLine,
    int expectStartColumn,
    int expectEndLine,
    int expectEndColumn,
    const string& expectMessage = "");

void testRuleOnCXXCode(RuleBase *rule,
    const string &code,
    int violationIndex,
    int expectStartLine,
    int expectStartColumn,
    int expectEndLine,
    int expectEndColumn,
    const string& expectMessage = "");

void testRuleOnCXX11Code(RuleBase *rule,
    const string &code,
    int violationIndex,
    int expectStartLine,
    int expectStartColumn,
    int expectEndLine,
    int expectEndColumn,
    const string& expectMessage = "");

void testRuleOnObjCCode(RuleBase *rule,
    const string &code,
    int violationIndex,
    int expectStartLine,
    int expectStartColumn,
    int expectEndLine,
    int expectEndColumn,
    const string& expectMessage = "");

void testRuleOnCUDACode(RuleBase *rule,
    const string &code,
    int violationIndex,
    int expectStartLine,
    int expectStartColumn,
    int expectEndLine,
    int expectEndColumn,
    const string& expectMessage = "");

#define VIOLATION_START "/*VIOLATION_START*/"
#define VIOLATION_END "/*VIOLATION_END*/"

// Some aliases
#define TAG_START VIOLATION_START
#define TAG_END VIOLATION_END
#define LOC_START VIOLATION_START
#define LOC_END VIOLATION_END
#define V_START VIOLATION_START
#define V_END VIOLATION_END

void ComputeLocalisation(const std::string& code, size_t offset, int* line, int* column);

// {start/end offset} ordered by increasing start-location then end-location.
 std::vector<std::pair<size_t, size_t>> extractViolationLocations(std::string& code);

bool lessByLocation(const Violation& lhs, const Violation& rhs);

void testRuleOnCode(const Twine& filename,
    const std::vector<std::string>& args,
    RuleBase* rule,
    std::string code,
    const std::vector<std::string>& messages);

void testRuleOnCode(RuleBase* rule, std::string code,
    const std::vector<std::string>& messages = {});

void testRuleOnCXXCode(RuleBase* rule, std::string code,
    const std::vector<std::string>& messages = {});

void testRuleOnCXX11Code(RuleBase* rule, std::string code,
    const std::vector<std::string>& messages = {});

void testRuleOnObjCCode(RuleBase* rule, std::string code,
    const std::vector<std::string>& messages = {});

void testRuleOnCUDACode(RuleBase* rule, std::string code,
    const std::vector<std::string>& messages = {});
