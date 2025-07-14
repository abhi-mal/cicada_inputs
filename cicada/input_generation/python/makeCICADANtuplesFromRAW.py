import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run2_2017_cff import Run2_2017

import FWCore.ParameterSet.VarParsing as VarParsing
options = VarParsing.VarParsing('analysis')
options.register(
    'isData',
    False,
    VarParsing.VarParsing.multiplicity.singleton,
    VarParsing.VarParsing.varType.bool,
    "Use data configuration options or not",
)
options.register(
    'secondaryFiles',
    [],
    VarParsing.VarParsing.multiplicity.list,
    VarParsing.VarParsing.varType.string
)
options.parseArguments()

process = cms.Process("NTUPLIZE",Run2_2017)
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)

process.MessageLogger.cerr.FwkReport.reportEvery = 10000

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(options.inputFiles),
                            secondaryFileNames = cms.untracked.vstring(options.secondaryFiles),
)

from Configuration.AlCa.GlobalTag import GlobalTag
if options.isData:
    print("Treating config as data.")
    process.GlobalTag = GlobalTag(process.GlobalTag, '106X_dataRun2_v37', '')
else:
    print("Treating config as simulation.")
    process.GlobalTag = GlobalTag(process.GlobalTag, '106X_mcRun2_asymptotic_v17', '')

process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)

process.schedule = cms.Schedule(process.raw2digi_step, process.endjob_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

from L1Trigger.Configuration.customiseReEmul import L1TReEmulFromRAWCalo,L1TReEmulFromRAW,L1TReEmulMCFromRAW

process = L1TReEmulFromRAW(process)

from L1Trigger.L1TNtuples.customiseL1Ntuple import L1NtupleRAWEMUCalo, L1NtupleEMU

process = L1NtupleRAWEMUCalo(process)

process.load('cicada.input_generation.cicadaProducer_cfi')
process.load("cicada.input_generation.CICADAInputNtuplizer_cfi")
process.load('cicada.input_generation.simpleSumNtuplizer_cfi')

process.NtuplePath = cms.Path(
    process.CICADAInputNtuplizer +
    process.simpleSumNtuplizer
)

process.schedule.append(process.NtuplePath)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string(options.outputFile)
)
#process.TFileService.fileName = cms.string(options.outputFile)

print("schedule:")
print(process.schedule)
print("schedule contents:")
print([x for x in process.schedule])
