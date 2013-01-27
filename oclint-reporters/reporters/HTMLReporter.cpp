#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/ViolationSet.h"

class HTMLReporter : public Reporter
{
public:
    virtual const string name() const
    {
        return "html";
    }

    virtual void report(Results *results, ostream &out)
    {
        out << "<!DOCTYPE html>";
        out << "<html>";
        out << "<head>";
        out << "<title>OCLint Report</title>";
        out << "<style type='text/css'>"
            << "                            \
.priority1, .priority2, .priority3 {        \
    font-weight: bold;                      \
    text-align: center;                     \
    color: #B40527;                         \
}                                           \
.priority1 { background-color: #FFC200; }   \
.priority2 { background-color: #FFD3A6; }   \
.priority3 { background-color: #FFEEB5; }   \
table {                                     \
    border: 2px solid gray;                 \
    border-collapse: collapse;              \
    -moz-box-shadow: 3px 3px 4px #AAA;      \
    -webkit-box-shadow: 3px 3px 4px #AAA;   \
    box-shadow: 3px 3px 4px #AAA;           \
}                                           \
td, th {                                    \
    border: 1px solid #D3D3D3;              \
    padding: 4px 20px 4px 20px;             \
}                                           \
th {                                        \
    text-shadow: 2px 2px 2px white;         \
    border-bottom: 1px solid gray;          \
    background-color: #E9F4FF;              \
}"
            << "</style>";
        out << "</head>";
        out << "<body>";
        out << "<h1>OCLint Report</h1>";
        out << "<hr />";
        out << "<h2>Summary</h2>";
        out << "<table><thead><tr><th>Total Files</th><th>Files with Violations</th>"
            << "<th>Priority 1</th><th>Priority 2</th><th>Priority 3</th></tr></thead>";
        out << "<tbody><tr><td>" << results->numberOfFiles() << "</td><td>"
            << results->numberOfFilesWithViolations() << "</td><td class='priority1'>"
            << results->numberOfViolationsWithPriority(1) << "</td><td class='priority2'>"
            << results->numberOfViolationsWithPriority(2) << "</td><td class='priority3'>"
            << results->numberOfViolationsWithPriority(3) << "</td></tr></tbody></table>";
        out << "<hr />";
        out << "<table><thead><tr><th>File</th><th>Location</th><th>Rule Name</th>"
            << "<th>Priority</th><th>Message</th></tr></thead><tbody>";
        vector<Violation> violationSet = results->allViolations();
        for (int index = 0, numberOfViolations = violationSet.size();
            index < numberOfViolations; index++)
        {
            Violation violation = violationSet.at(index);
            out << "<tr><td>" << violation.path << "</td><td>" << violation.startLine
                << ":" << violation.startColumn << "</td>";
            const RuleBase *rule = violation.rule;
            out << "<td>" << rule->name() << "</td><td class='priority" << rule->priority() << "'>"
                << rule->priority() << "</td><td>" << violation.message << "</td></tr>";
        }
        out << "</tbody></table>";
        out << "<hr />";
        time_t now = time(0);
        out << ctime(&now)
            << "| Generated with <a href='http://oclint.org'>OCLint v0.7</a>.</p>";
        out << "</body>";
        out << "</html>" << endl;
    }
};

extern "C" Reporter* create()
{
  return new HTMLReporter();
}
