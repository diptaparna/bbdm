#ifndef analysis_cms_pp_bbdm_h
#define analysis_cms_pp_bbdm_h

#include "SampleAnalyzer/Process/Analyzer/AnalyzerBase.h"

namespace MA5
{
class cms_pp_bbdm : public AnalyzerBase
{
  INIT_ANALYSIS(cms_pp_bbdm,"cms_pp_bbdm")

 public:
  virtual bool Initialize(const MA5::Configuration& cfg, const std::map<std::string,std::string>& parameters);
  virtual void Finalize(const SampleFormat& summary, const std::vector<SampleFormat>& files);
  virtual bool Execute(SampleFormat& sample, const EventFormat& event);

 private:
  TH1F *MET_SR1;
  TH1F *MET_SR2;
};
}

#endif
