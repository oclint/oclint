#include <ctime>

#include "oclint/Constants.h"
#include "oclint/Results.h"
#include "oclint/Reporter.h"
#include "oclint/RuleBase.h"
#include "oclint/ViolationSet.h"

using namespace oclint;

class HTMLReporter : public Reporter
{
public:
    virtual const std::string name() const override
    {
        return "html";
    }

    virtual void report(Results* results, std::ostream& out) override
    {
        out << "<!DOCTYPE html>";
        out << "<html>";
        writeHead(out);
        out << "<body>";
        out << "<h1>OCLint Report</h1>";
        out << "<h2>Summary</h2>";
        writeSummaryTable(out, *results);
        out << "<h2>Details</h2>";
        out << "<table id=\"violations-table\"><thead><tr>";
        out << "<th onclick=\"sortTable(0)\" class=\"sortable\">File</th>"
            << "<th onclick=\"sortTable(1)\" class=\"sortable\">Location</th>"
            << "<th onclick=\"sortTable(2)\" class=\"sortable\">Rule Name</th>"
            << "<th onclick=\"sortTable(3)\" class=\"sortable\">Rule Category</th>"
            << "<th onclick=\"sortTable(4)\" class=\"sortable\">Priority</th>"
            << "<th onclick=\"sortTable(5)\" class=\"sortable\">Message</th>"
            << "</tr></thead><tbody>";
        for (const auto& violation : results->allViolations())
        {
            writeViolation(out, violation);
        }
        if (results->hasErrors())
        {
            writeCompilerDiagnostics(out, results->allErrors(), "error");
        }
        if (results->hasWarnings())
        {
            writeCompilerDiagnostics(out, results->allWarnings(), "warning");
        }
        if (results->hasCheckerBugs())
        {
            writeCheckerBugs(out, results->allCheckerBugs());
        }
        out << "</tbody></table>";
        out << "<hr />";
        writeFooter(out, Constants::version());
        out << "</body>";
        out << "</html>";
        out << std::endl;
    }

    void writeFooter(std::ostream &out, std::string version)
    {
        time_t now = time(nullptr);
        out << "<p>" << ctime(&now)
            << "| Generated with <a href='" << Constants::homepage() << "'>"
            << "OCLint v" << version << "</a>.</p>";
    }

    void writeViolation(std::ostream &out, const Violation &violation)
    {
        out << "<tr><td>" << violation.path << "</td><td>" << violation.startLine
            << ":" << violation.startColumn << "</td>";
        const RuleBase *rule = violation.rule;
        out << "<td>" << rule->name() << "</td>"
            << "<td>" << rule->category() << "</td>"
            << "<td class='priority" << rule->priority() << "'>"
            << rule->priority() << "</td>"
            << "<td>" << violation.message << "</td></tr>";
    }

    void writeCompilerErrorOrWarning(std::ostream &out,
        const Violation &violation, std::string level)
    {
        out << "<tr><td>" << violation.path << "</td><td>" << violation.startLine
            << ":" << violation.startColumn << "</td>";
        out << "<td>compiler " << level << "</td><td></td><td class='cmplr-" << level << "'>"
            << level << "</td><td>" << violation.message << "</td></tr>";
    }

    void writeCompilerDiagnostics(std::ostream &out, std::vector<Violation> violations,
        std::string level)
    {
        for (const auto& violation : violations)
        {
            writeCompilerErrorOrWarning(out, violation, level);
        }
    }

    void writeCheckerBugs(std::ostream &out, std::vector<Violation> violations)
    {
        for (const auto& violation : violations)
        {
            out << "<tr><td>" << violation.path << "</td><td>" << violation.startLine
                << ":" << violation.startColumn << "</td>";
            out << "<td>clang static analyzer</td><td></td><td class='checker-bug'>"
                << "checker bug</td><td>" << violation.message << "</td></tr>";
        }
    }

    void writeSummaryTable(std::ostream &out, Results &results)
    {
        out << "<table><thead><tr><th>Total Files</th><th>Files with Violations</th>"
            << "<th>Priority 1</th><th>Priority 2</th><th>Priority 3</th>"
            << "<th>Compiler Errors</th><th>Compiler Warnings</th>"
            << "<th>Clang Static Analyzer</th></tr></thead>";
        out << "<tbody><tr><td>" << results.numberOfFiles() << "</td><td>"
            << results.numberOfFilesWithViolations() << "</td><td class='priority1'>"
            << results.numberOfViolationsWithPriority(1) << "</td><td class='priority2'>"
            << results.numberOfViolationsWithPriority(2) << "</td><td class='priority3'>"
            << results.numberOfViolationsWithPriority(3) << "</td><td class='cmplr-error'>"
            << results.allErrors().size() << "</td><td class='cmplr-warning'>"
            << results.allWarnings().size() << "</td><td class='checker-bug'>"
            << results.allCheckerBugs().size() << "</td></tr></tbody></table>";
    }

    void writeHead(std::ostream &out)
    {
        out << "<head>";
        out << "<title>OCLint Report</title>";
        out << "<style type='text/css'>"
            << "* { color: #4A4A4A ; }                                                  \
.sortable { cursor: pointer; }                                                          \
.priority1, .priority2, .priority3, .cmplr-error,                                       \
    .cmplr-warning, .checker-bug { font-weight: bold; text-align: center; }             \
.priority1, .priority2, .priority3 { color: #4A4A4A; }                                  \
.priority1 { background-color: #FFCC96; }                                               \
.priority2 { background-color: #FFFFD5; }                                               \
.priority3 { background-color: #75B3DA; }                                               \
.cmplr-error, .cmplr-warning { background-color: #ED686A; color: #FFFFD5; }             \
.checker-bug { background-color: #75B3DA; color: #4A4A4A; }                             \
table { border: 1px solid #2B2B2B; border-collapse: collapse; }                         \
td, th { border: 1px solid #2B2B2B; padding: 4px 10px 4px 10px; }                       \
th { border-bottom: 1px solid #2B2B2B; text-align: center; background-color: #eaeaea; } \
hr { border: none; border-top: 1px solid #EAEAEA; } "
            << "</style>";
        out << "<script>"
            << "function sortTable(n) {                                                 \
  var table, rows, switching, i, x, y, shouldSwitch, dir, switchcount = 0;              \
  table = document.getElementById(\"violations-table\");                                \
  switching = true;                                                                     \
  dir = \"asc\";                                                                        \
  while (switching) {                                                                   \
    switching = false;                                                                  \
    rows = table.rows;                                                                  \
    for (i = 1; i < (rows.length - 1); i++) {                                           \
      shouldSwitch = false;                                                             \
      x = rows[i].getElementsByTagName(\"TD\")[n];                                      \
      y = rows[i + 1].getElementsByTagName(\"TD\")[n];                                  \
      if ((dir == \"asc\" && x.innerHTML.toLowerCase() > y.innerHTML.toLowerCase()) ||  \
         (dir == \"desc\" && x.innerHTML.toLowerCase() < y.innerHTML.toLowerCase())) {  \
          shouldSwitch = true;                                                          \
          break;                                                                        \
      }                                                                                 \
    }                                                                                   \
    if (shouldSwitch) {                                                                 \
      rows[i].parentNode.insertBefore(rows[i + 1], rows[i]);                            \
      switching = true;                                                                 \
      switchcount++;                                                                    \
    } else {                                                                            \
      if (switchcount == 0 && dir == \"asc\") {                                         \
        dir = \"desc\";                                                                 \
        switching = true;                                                               \
      }                                                                                 \
    }                                                                                   \
  }                                                                                     \
}"
            << "</script>";
        out << "</head>";
    }
};

extern "C" Reporter* create()
{
  return new HTMLReporter();
}
