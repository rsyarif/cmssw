#ifndef FWCore_Framework_global_EDFilterBase_h
#define FWCore_Framework_global_EDFilterBase_h
// -*- C++ -*-
//
// Package:     FWCore/Framework
// Class  :     EDFilterBase
// 
/**\class EDFilterBase EDFilterBase.h "EDFilterBase.h"

 Description: [one line class summary]

 Usage:
    <usage>

*/
//
// Original Author:  Chris Jones
//         Created:  Thu, 18 Jul 2013 11:51:14 GMT
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/ProducerBase.h"
#include "FWCore/Framework/interface/EDConsumerBase.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/Provenance/interface/ModuleDescription.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"

// forward declarations

namespace edm {
  class ModuleCallingContext;
  class PreallocationConfiguration;
  class StreamID;
  class ActivityRegistry;
  
  namespace maker {
    template<typename T> class ModuleHolderT;
  }

  namespace global {
    
    class EDFilterBase : public ProducerBase, public EDConsumerBase
    {
      
    public:
      template <typename T> friend class edm::maker::ModuleHolderT;
      template <typename T> friend class edm::WorkerT;
      typedef EDFilterBase ModuleType;

      EDFilterBase();
      virtual ~EDFilterBase();

      static void fillDescriptions(ConfigurationDescriptions& descriptions);
      static void prevalidate(ConfigurationDescriptions& descriptions);
      static const std::string& baseType();

      // Warning: the returned moduleDescription will be invalid during construction
      ModuleDescription const& moduleDescription() const { return moduleDescription_; }

    private:
      bool doEvent(EventPrincipal& ep, EventSetup const& c,
                   ActivityRegistry*,
                   ModuleCallingContext const*);
      void doPreallocate(PreallocationConfiguration const&);
      void doBeginJob();
      void doEndJob();
      
      void doBeginStream(StreamID id);
      void doEndStream(StreamID id);
      void doStreamBeginRun(StreamID id,
                            RunPrincipal& ep,
                            EventSetup const& c,
                            ModuleCallingContext const*);
      void doStreamEndRun(StreamID id,
                          RunPrincipal& ep,
                          EventSetup const& c,
                          ModuleCallingContext const*);
      void doStreamBeginLuminosityBlock(StreamID id,
                                        LuminosityBlockPrincipal& ep,
                                        EventSetup const& c,
                                        ModuleCallingContext const*);
      void doStreamEndLuminosityBlock(StreamID id,
                                      LuminosityBlockPrincipal& ep,
                                      EventSetup const& c,
                                      ModuleCallingContext const*);

      
      void doBeginRun(RunPrincipal& rp, EventSetup const& c,
                      ModuleCallingContext const*);
      void doEndRun(RunPrincipal& rp, EventSetup const& c,
                    ModuleCallingContext const*);
      void doBeginLuminosityBlock(LuminosityBlockPrincipal& lbp, EventSetup const& c,
                                  ModuleCallingContext const*);
      void doEndLuminosityBlock(LuminosityBlockPrincipal& lbp, EventSetup const& c,
                                ModuleCallingContext const*);
      
      void doPreForkReleaseResources();
      void doPostForkReacquireResources(unsigned int iChildIndex, unsigned int iNumberOfChildren);

      //For now, the following are just dummy implemenations with no ability for users to override
      void doRespondToOpenInputFile(FileBlock const& fb);
      void doRespondToCloseInputFile(FileBlock const& fb);
      
      
      void registerProductsAndCallbacks(EDFilterBase* module, ProductRegistry* reg) {
        registerProducts(module, reg, moduleDescription_);
      }
      std::string workerType() const {return "WorkerT<EDProducer>";}
      
      virtual bool filter(StreamID, Event&, EventSetup const&) const= 0;
      virtual void beginJob() {}
      virtual void endJob(){}

      virtual void preForkReleaseResources() {}
      virtual void postForkReacquireResources(unsigned int /*iChildIndex*/, unsigned int /*iNumberOfChildren*/) {}
      
      virtual void preallocStreams(unsigned int);
      virtual void doBeginStream_(StreamID id);
      virtual void doEndStream_(StreamID id);
      virtual void doStreamBeginRun_(StreamID id, Run const& rp, EventSetup const& c);
      virtual void doStreamEndRun_(StreamID id, Run const& rp, EventSetup const& c);
      virtual void doStreamEndRunSummary_(StreamID id, Run const& rp, EventSetup const& c);
      virtual void doStreamBeginLuminosityBlock_(StreamID id, LuminosityBlock const& lbp, EventSetup const& c);
      virtual void doStreamEndLuminosityBlock_(StreamID id, LuminosityBlock const& lbp, EventSetup const& c);
      virtual void doStreamEndLuminosityBlockSummary_(StreamID id, LuminosityBlock const& lbp, EventSetup const& c);

      virtual void doBeginRun_(Run const& rp, EventSetup const& c);
      virtual void doBeginRunSummary_(Run const& rp, EventSetup const& c);
      virtual void doEndRunSummary_(Run const& rp, EventSetup const& c);
      virtual void doEndRun_(Run const& rp, EventSetup const& c);
      virtual void doBeginLuminosityBlock_(LuminosityBlock const& lbp, EventSetup const& c);
      virtual void doBeginLuminosityBlockSummary_(LuminosityBlock const& rp, EventSetup const& c);
      virtual void doEndLuminosityBlockSummary_(LuminosityBlock const& lb, EventSetup const& c);
      virtual void doEndLuminosityBlock_(LuminosityBlock const& lb, EventSetup const& c);
      
      virtual void doBeginRunProduce_(Run& rp, EventSetup const& c);
      virtual void doEndRunProduce_(Run& rp, EventSetup const& c);
      virtual void doBeginLuminosityBlockProduce_(LuminosityBlock& lbp, EventSetup const& c);
      virtual void doEndLuminosityBlockProduce_(LuminosityBlock& lbp, EventSetup const& c);
      
      
      void setModuleDescription(ModuleDescription const& md) {
        moduleDescription_ = md;
      }
      ModuleDescription moduleDescription_;
      std::unique_ptr<std::vector<BranchID>[]> previousParentages_; //Per stream in the future?
      std::unique_ptr<ParentageID[]> previousParentageIds_;
    };

  }
}

#endif