import FWCore.ParameterSet.Config as cms

CICADAInputNtuplizer = cms.EDAnalyzer(
    'CICADAInputNtuplizer',
    emuRegionsToken = cms.InputTag("caloLayer1Digis")
)

simCICADAInputNtuplizer = cms.EDAnalyzer(
    'CICADAInputNtuplizer',
    emuRegionsToken = cms.InputTag("simCaloStage2Layer1Digis")
)
