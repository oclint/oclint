#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/Version.h"
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
        writeHead(out);
        out << "<body>";
        out << "<h1>OCLint Report</h1>";
        out << "<hr />";
        out << "<h2>Summary</h2>";
        writeSummaryTable(out, *results);
        out << "<hr />";
        out << "<table><thead><tr><th>File</th><th>Location</th><th>Rule Name</th>"
            << "<th>Priority</th><th>Message</th></tr></thead><tbody>";
        vector<Violation> violationSet = results->allViolations();
        for (int index = 0, numberOfViolations = violationSet.size();
            index < numberOfViolations; index++)
        {
            writeViolation(out, violationSet.at(index));
        }
        out << "</tbody></table>";
        out << "<hr />";
        writeFooter(out, Version::identifier());
        out << "</body>";
        out << "</html>";
        out << endl;
    }

    void writeFooter(ostream &out, string version)
    {
        time_t now = time(0);
        out << "<p>" << ctime(&now)
            << "| Generated with <a href='http://oclint.org'>OCLint v" << version << "</a>.</p>";
    }

    void writeViolation(ostream &out, Violation &violation)
    {
        out << "<tr><td>" << violation.path << "</td><td>" << violation.startLine
            << ":" << violation.startColumn << "</td>";
        const RuleBase *rule = violation.rule;
        out << "<td>" << rule->name() << "</td><td class='priority" << rule->priority() << "'>"
            << rule->priority() << "</td><td>" << violation.message << "</td></tr>";
    }

    void writeSummaryTable(ostream &out, Results results)
    {
        out << "<table><thead><tr><th>Total Files</th><th>Files with Violations</th>"
            << "<th>Priority 1</th><th>Priority 2</th><th>Priority 3</th></tr></thead>";
        out << "<tbody><tr><td>" << results.numberOfFiles() << "</td><td>"
            << results.numberOfFilesWithViolations() << "</td><td class='priority1'>"
            << results.numberOfViolationsWithPriority(1) << "</td><td class='priority2'>"
            << results.numberOfViolationsWithPriority(2) << "</td><td class='priority3'>"
            << results.numberOfViolationsWithPriority(3) << "</td></tr></tbody></table>";
    }

    void writeHead(ostream &out)
    {
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
    }
};

extern "C" Reporter* create()
{
  return new HTMLReporter();
}
