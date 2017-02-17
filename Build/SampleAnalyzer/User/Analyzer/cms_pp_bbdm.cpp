#include "SampleAnalyzer/User/Analyzer/cms_pp_bbdm.h"

using namespace MA5;
using namespace std;

// -----------------------------------------------------------------------------
// Initialize
// function called one time at the beginning of the analysis
// -----------------------------------------------------------------------------
bool cms_pp_bbdm::Initialize(const MA5::Configuration& cfg,
		const std::map<std::string, std::string>& parameters) {
	// Information on Analysis
	INFO<<"Analysis: pp -> bb~ + DM"<<endmsg;

	cout << "BEGIN Initialization" << endl;
	// Declaring all signal regions

	// Declaring preselection cuts

	// Other cuts

	MET_SR1=new TH1F("MET_SR1","MET for SR1",8,200,1000);
	MET_SR2=new TH1F("MET_SR2","MET for SR2",8,200,1000);
	cout << "END Initialization" << endl;
	return true;
}

// -----------------------------------------------------------------------------
// Finalize
// function called one time at the end of the analysis
// -----------------------------------------------------------------------------
void cms_pp_bbdm::Finalize(const SampleFormat& summary,
		const std::vector<SampleFormat>& files) {
	cout << "BEGIN Finalization" << endl;

	TCanvas c;
	c.SetLogy();
	MET_SR1->Draw();
	c.SaveAs("MET_SR1.png");
	MET_SR2->Draw();
	c.SaveAs("MET_SR2.png");
	cout << "END Finalization" << endl;
}

// -----------------------------------------------------------------------------
// Execute
// function called each time one event is read
// -----------------------------------------------------------------------------
bool cms_pp_bbdm::Execute(SampleFormat& sample, const EventFormat& event) {

	// Keeping track of event weights.
	double myEventWeight;
	if (Configuration().IsNoEventWeight())
		myEventWeight = 1;
	else if (event.mc()->weight() != 0)
		myEventWeight = event.mc()->weight();
	else {
		WARNING<< "Found one event with a zero weight. Skipping..." << endmsg;
		return false;
	}
	Manager()->InitializeForNewEvent(myEventWeight);

	vector<const RecJetFormat*> SignalJets;

	TLorentzVector pTmiss = event.rec()->MET().momentum();
	double MET = pTmiss.Pt();

	// find the jets
	size_t noOfJets = event.rec()->jets().size();
	for (size_t i = 0; i < noOfJets; i++) {
		const RecJetFormat * CurrentJet = &(event.rec()->jets()[i]);
		SignalJets.push_back(CurrentJet);
	}

	// find event for any electron with pt>10
	bool noElectron = true;
	for (size_t i = 0; i < event.rec()->electrons().size(); i++) {
		const RecLeptonFormat * CurrentElectron = &(event.rec()->electrons()[i]);
		if (CurrentElectron->pt() > 10.0 && CurrentElectron->isolated())
			noElectron = false;
	}

	// find event for any muon with pT>10
	bool noMuon = true;
	for (size_t i = 0; i < event.rec()->muons().size(); i++) {
		const RecLeptonFormat * CurrentMuon = &(event.rec()->muons()[i]);
		if (CurrentMuon->pt() > 10 && CurrentMuon->isolated())
			noMuon = false;
	}

	// find event for any tau with pT>18
	bool noTau = true;
	for (size_t i = 0; i < event.rec()->taus().size(); i++) {
		const RecTauFormat * CurrentTau = &(event.rec()->taus()[i]);
		if (CurrentTau->pt() > 18 && CurrentTau->isolated())
			noTau = false;
	}

	// find event for any photon with pT>15
	bool noPhoton = true;
	for (size_t i = 0; i < event.rec()->photons().size(); i++) {
		const RecPhotonFormat * CurrentPhoton = &(event.rec()->photons()[i]);
		if (CurrentPhoton->pt() > 15 && CurrentPhoton->isolated())
			noPhoton = false;
	}

	SORTER->sort(SignalJets, PTordering);

	// Applying the Cuts

	// Cut: MET > 200
	if (MET <= 200)
		return false;
	// Cut: No of Jets <= 3
	if (noOfJets > 3)
		return false;
	// Cut: At least 1 jet
	if (noOfJets < 1)
		return false;
	// Cut: 1st Jet has pt > 50
	if (SignalJets[0]->pt() <= 50)
		return false;
	// Cut: 1st Jet has delta_phi with MET > 0.5
	if (SignalJets[0]->dphi_0_pi(pTmiss) <= 0.5)
		return false;

	// Check if the event satisfies SR1 conditions
	bool SR1 = true;
	if (noOfJets == 3)
		SR1 = false;
	if (noOfJets == 2) {
		if (SignalJets[1]->pt() <= 30
				|| SignalJets[1]->dphi_0_pi(pTmiss) <= 0.5)
			SR1 = false;
		if (SignalJets[0]->btag() && SignalJets[1]->btag())
			SR1 = false;
	}
	if (noOfJets == 1)
		if (!SignalJets[0]->btag())
			SR1 = false;

	// Check if the event satisfies SR2 conditions
	bool SR2 = true;
	if (noOfJets < 2)
		SR2 = false;
	if (noOfJets == 2) {
		if (SignalJets[1]->pt() <= 50
				|| SignalJets[1]->dphi_0_pi(pTmiss) <= 0.5)
			SR1 = false;
		if (!(SignalJets[0]->btag() && SignalJets[1]->btag()))
			SR2 = false;
	}
	if (noOfJets == 3) {
		if (SignalJets[2]->pt() <= 30
				|| SignalJets[2]->dphi_0_pi(pTmiss) <= 0.5)
			SR1 = false;
		size_t noBtaggedJets = 0;
		for (size_t i = 0; i < 3; i++)
			if (SignalJets[i]->btag())
				noBtaggedJets++;
		if (noOfJets != 2)
			SR2 = false;
	}

	// Cut: No lepton or photon
	if (!(noElectron && noMuon && noTau && noPhoton))
		return false;

	// Fill the histograms
	if (SR1)
		MET_SR1->Fill(MET, myEventWeight);
	if (SR2)
		MET_SR2->Fill(MET, myEventWeight);

	return (SR1 && SR2);
}
