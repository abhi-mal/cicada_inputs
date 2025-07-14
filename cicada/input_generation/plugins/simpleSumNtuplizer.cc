// Stuff for calculating the 
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/L1Trigger/interface/EtSum.h"

#include "TTree.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

class simpleSumNtuplizer : public edm::one::EDAnalyzer<edm::one::SharedResources>
{
public:
  explicit simpleSumNtuplizer(const edm::ParameterSet&);
  ~simpleSumNtuplizer() = default;
  static void fillDescriptions(edm::ConfigurationDescriptions &);

private:
  void beginJob() override {};
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override {};

  const edm::EDGetTokenT<l1t::EtSumBxCollection> etSumToken;

  edm::Service<TFileService> theFileService;  
  TTree* sumTree;

  double eventET = 0.0;
  double eventHT = 0.0;
  double eventMET = 0.0;
};

simpleSumNtuplizer::simpleSumNtuplizer(const edm::ParameterSet& iConfig):
  etSumToken(consumes<l1t::EtSumBxCollection>(iConfig.getParameter<edm::InputTag>("etSumToken")))
{
  usesResource("TFileService");

  sumTree = theFileService->make<TTree>("simpleEtSums", "Simple Et sums to make sure there are no mistakes");
  sumTree->Branch("eventET", &eventET);
  sumTree->Branch("eventHT", &eventHT);
  sumTree->Branch("eventMET", &eventMET);
}

void simpleSumNtuplizer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<l1t::EtSumBxCollection> etSums;
  iEvent.getByToken(etSumToken, etSums);

  eventET = 0.0;
  eventHT = 0.0;
  eventMET = 0.0;
  
  int centralBXSize = etSums->size(0);
  for(int i = 0; i<centralBXSize; ++i) {
    auto sum = etSums->at(0, i);
    if (sum.getType() == l1t::EtSum::kTotalEt) {
      eventET = sum.et();
    }
    if (sum.getType() == l1t::EtSum::kTotalHt) {
      eventHT = sum.et();
    }
    if (sum.getType() == l1t::EtSum::kMissingEt) {
      eventMET = sum.et();
    }
  }

  sumTree->Fill();
}

void simpleSumNtuplizer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  edm::ParameterSetDescription desc;
  desc.add<edm::InputTag>("etSumToken", edm::InputTag("caloStage2Digis","EtSum"));
  descriptions.add("simpleSumNtuplizer", desc);
}

DEFINE_FWK_MODULE(simpleSumNtuplizer);
