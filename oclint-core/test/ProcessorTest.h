#include "oclint/Processor.h"
#include "oclint/Reporter.h"
#include "oclint/ViolationSet.h"

class ProcessorTest_ReporterMock : public Reporter
{
public:
    MOCK_METHOD2(report, void(ASTContext &astContext, ViolationSet *violationSet));
};

using testing::_;

TEST(ProcessorTest, TriggerReporter)
{
    ProcessorTest_ReporterMock reporter;
    EXPECT_CALL(reporter, report(_, _));

    Processor processor(&reporter);
    void *context;
    ASTContext *astContext = (ASTContext *)context;
    processor.HandleTranslationUnit(*astContext);
}
