// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/L1CaloTrigger/interface/L1CaloCollections.h"
#include "DataFormats/L1CaloTrigger/interface/L1CaloRegion.h"

#include "TTree.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <string>

class CICADAInputNtuplizer : public edm::one::EDAnalyzer< edm::one::SharedResources >
{
public:
  explicit CICADAInputNtuplizer(const edm::ParameterSet&);
  ~CICADAInputNtuplizer() override = default;

private:
  void beginJob() override {};
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override {};

  edm::EDGetTokenT<L1CaloRegionCollection> regionsToken;
  std::vector<int> modelInput;
  std::vector<unsigned short int> tauBits;
  std::vector<unsigned short int> egBits;
  std::vector<int> iEta;
  std::vector<int> iPhi;
  //int modelInput[18][14];
  //unsigned short int tauBits[18][14];
  //unsigned short int egBits[18][14];

  edm::Service<TFileService> theFileService;
  TTree* triggerTree;
};

CICADAInputNtuplizer::CICADAInputNtuplizer(const edm::ParameterSet& iConfig):
  regionsToken(consumes<L1CaloRegionCollection>(iConfig.getParameter<edm::InputTag>("emuRegionsToken")))
{  
  usesResource("TFileService");
  triggerTree = theFileService->make< TTree >("CICADAInputTree","(emulator) region information");
  //triggerTree -> Branch("modelInput", &modelInput, "modelInput[18][14]/I");
  //triggerTree -> Branch("tauBits", &tauBits, "tauBits[18][14]/O");
  //triggerTree -> Branch("egBits", &egBits, "tauBits[18][14]/O");
  triggerTree -> Branch("modelInput", &modelInput);
  triggerTree -> Branch("tauBits", &tauBits);
  triggerTree -> Branch("egBits", &egBits);
  triggerTree -> Branch("iEta", &iEta);
  triggerTree -> Branch("iPhi", &iPhi);
}

void CICADAInputNtuplizer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  modelInput.clear();
  tauBits.clear();
  egBits.clear();
  iEta.clear();
  iPhi.clear();

  modelInput.reserve(18*14);
  tauBits.reserve(18*14);
  egBits.reserve(18*14);
  iEta.reserve(18*14);
  iPhi.reserve(18*14);
  
  edm::Handle<std::vector<L1CaloRegion>> emuRegions;
  iEvent.getByToken(regionsToken, emuRegions);
  for(const L1CaloRegion& theRegion: *emuRegions)
    {
      //tauBits[theRegion.gctPhi()][theRegion.gctEta()-4] = (unsigned short int) theRegion.tauVeto();
      //egBits[theRegion.gctPhi()][theRegion.gctEta()-4] = (unsigned short int) theRegion.overFlow();
      //modelInput[theRegion.gctPhi()][theRegion.gctEta()-4] = theRegion.et();
      modelInput.emplace_back(theRegion.et());
      tauBits.emplace_back((unsigned short int) theRegion.tauVeto());
      egBits.emplace_back((unsigned short int) theRegion.overFlow());
      iEta.emplace_back(theRegion.gctEta() - 4);
      iPhi.emplace_back(theRegion.gctPhi());
    }
  triggerTree->Fill();
}

DEFINE_FWK_MODULE(CICADAInputNtuplizer);
