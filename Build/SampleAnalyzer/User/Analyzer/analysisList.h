#include "SampleAnalyzer/User/Analyzer/cms_pp_bbdm.h"
#include "SampleAnalyzer/Process/Analyzer/AnalyzerManager.h"
#include "SampleAnalyzer/Commons/Service/LogStream.h"

// -----------------------------------------------------------------------------
// BuildTable
// -----------------------------------------------------------------------------
void BuildUserTable(MA5::AnalyzerManager& manager)
{
  using namespace MA5;
  manager.Add("cms_pp_bbdm",new cms_pp_bbdm);
}
