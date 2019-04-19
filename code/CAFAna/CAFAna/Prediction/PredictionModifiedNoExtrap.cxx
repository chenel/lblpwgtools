#include "CAFAna/Prediction/PredictionModifiedNoExtrap.h"

#include "CAFAna/Core/HistCache.h"
#include "CAFAna/Core/LoadFromFile.h"
#include "CAFAna/Core/OscCurve.h"

#include "CAFAna/Core/Loaders.h"

#include "CAFAna/Extrap/IExtrap.h"
#include "CAFAna/Extrap/TrivialExtrap.h"

#include "TDirectory.h"
#include "TObjString.h"

#include "TH1D.h"

namespace ana {

//----------------------------------------------------------------------
PredictionModifiedNoExtrap::PredictionModifiedNoExtrap(PredictionExtrap *pred)
    : PredictionNoExtrap(pred->GetExtrap()) {}

//----------------------------------------------------------------------
PredictionModifiedNoExtrap::PredictionModifiedNoExtrap(Loaders &loaders,
                                                       const HistAxis &axis,
                                                       const Cut &cut,
                                                       const SystShifts &shift,
                                                       const Var &wei)
    : PredictionNoExtrap(new TrivialExtrap(loaders, axis, cut, shift, wei)) {}

//----------------------------------------------------------------------
Spectrum PredictionModifiedNoExtrap::PredictComponent(osc::IOscCalculator *calc,
                                                      Flavors::Flavors_t flav,
                                                      Current::Current_t curr,
                                                      Sign::Sign_t sign) const {
  Spectrum ret = fExtrap->NCComponent(); // Get binning
  ret.Clear();

  if (curr & Current::kCC) {
    if (flav & Flavors::kNuEToNuE && sign & Sign::kNu) {

      const OscCurve curve(calc, +12, +12);
      TH1D *osc = curve.ToTH1();
      if (fExtraWeight) {
        osc->Multiply(fExtraWeight);
      }

      ret += fExtrap->NueSurvComponent().WeightedBy(osc);
      HistCache::Delete(osc);
    }
    if (flav & Flavors::kNuEToNuE && sign & Sign::kAntiNu) {
      const OscCurve curve(calc, -12, -12);
      TH1D *osc = curve.ToTH1();
      if (fExtraWeight) {
        osc->Multiply(fExtraWeight);
      }
      ret += fExtrap->AntiNueSurvComponent().WeightedBy(osc);
      HistCache::Delete(osc);
    }

    if (flav & Flavors::kNuEToNuMu && sign & Sign::kNu) {
      const OscCurve curve(calc, +12, +14);
      TH1D *osc = curve.ToTH1();
      if (fExtraWeight) {
        osc->Multiply(fExtraWeight);
      }
      ret += fExtrap->NumuAppComponent().WeightedBy(osc);
      HistCache::Delete(osc);
    }
    if (flav & Flavors::kNuEToNuMu && sign & Sign::kAntiNu) {
      const OscCurve curve(calc, -12, -14);
      TH1D *osc = curve.ToTH1();
      if (fExtraWeight) {
        osc->Multiply(fExtraWeight);
      }
      ret += fExtrap->AntiNumuAppComponent().WeightedBy(osc);
      HistCache::Delete(osc);
    }

    if (flav & Flavors::kNuEToNuTau && sign & Sign::kNu) {
      const OscCurve curve(calc, +12, +16);
      TH1D *osc = curve.ToTH1();
      if (fExtraWeight) {
        osc->Multiply(fExtraWeight);
      }
      ret += fExtrap->TauFromEComponent().WeightedBy(osc);
      HistCache::Delete(osc);
    }
    if (flav & Flavors::kNuEToNuTau && sign & Sign::kAntiNu) {
      const OscCurve curve(calc, -12, -16);
      TH1D *osc = curve.ToTH1();
      if (fExtraWeight) {
        osc->Multiply(fExtraWeight);
      }
      ret += fExtrap->AntiTauFromEComponent().WeightedBy(osc);
      HistCache::Delete(osc);
    }

    if (flav & Flavors::kNuMuToNuE && sign & Sign::kNu) {
      const OscCurve curve(calc, +14, +12);
      TH1D *osc = curve.ToTH1();
      if (fExtraWeight) {
        osc->Multiply(fExtraWeight);
      }
      ret += fExtrap->NueAppComponent().WeightedBy(osc);
      HistCache::Delete(osc);
    }
    if (flav & Flavors::kNuMuToNuE && sign & Sign::kAntiNu) {
      const OscCurve curve(calc, -14, -12);
      TH1D *osc = curve.ToTH1();
      if (fExtraWeight) {
        osc->Multiply(fExtraWeight);
      }
      ret += fExtrap->AntiNueAppComponent().WeightedBy(osc);
      HistCache::Delete(osc);
    }

    if (flav & Flavors::kNuMuToNuMu && sign & Sign::kNu) {
      const OscCurve curve(calc, +14, +14);
      TH1D *osc = curve.ToTH1();
      if (fExtraWeight) {
        osc->Multiply(fExtraWeight);
      }
      ret += fExtrap->NumuSurvComponent().WeightedBy(osc);
      HistCache::Delete(osc);
    }
    if (flav & Flavors::kNuMuToNuMu && sign & Sign::kAntiNu) {
      const OscCurve curve(calc, -14, -14);
      TH1D *osc = curve.ToTH1();
      if (fExtraWeight) {
        osc->Multiply(fExtraWeight);
      }
      ret += fExtrap->AntiNumuSurvComponent().WeightedBy(osc);
      HistCache::Delete(osc);
    }

    if (flav & Flavors::kNuMuToNuTau && sign & Sign::kNu) {
      const OscCurve curve(calc, +14, +16);
      TH1D *osc = curve.ToTH1();
      if (fExtraWeight) {
        osc->Multiply(fExtraWeight);
      }
      ret += fExtrap->TauFromMuComponent().WeightedBy(osc);
      HistCache::Delete(osc);
    }
    if (flav & Flavors::kNuMuToNuTau && sign & Sign::kAntiNu) {
      const OscCurve curve(calc, -14, -16);
      TH1D *osc = curve.ToTH1();
      if (fExtraWeight) {
        osc->Multiply(fExtraWeight);
      }
      ret += fExtrap->AntiTauFromMuComponent().WeightedBy(osc);
      HistCache::Delete(osc);
    }
  }
  if (curr & Current::kNC) {
    assert(flav == Flavors::kAll); // Don't know how to calculate anything else
    assert(sign == Sign::kBoth); // Why would you want to split NCs out by sign?

    ret += fExtrap->NCComponent();
  }

  return ret;
}

//----------------------------------------------------------------------
void PredictionModifiedNoExtrap::SaveTo(TDirectory *dir) const {
  TDirectory *tmp = gDirectory;

  dir->cd();

  TObjString("PredictionModifiedNoExtrap").Write("type");

  fExtrap->SaveTo(dir->mkdir("extrap"));

  tmp->cd();
}

//----------------------------------------------------------------------
std::unique_ptr<PredictionModifiedNoExtrap>
PredictionModifiedNoExtrap::LoadFrom(TDirectory *dir) {
  assert(dir->GetDirectory("extrap"));
  IExtrap *extrap =
      ana::LoadFrom<IExtrap>(dir->GetDirectory("extrap")).release();
  PredictionExtrap *pred = new PredictionExtrap(extrap);
  return std::unique_ptr<PredictionModifiedNoExtrap>(
      new PredictionModifiedNoExtrap(pred));
}

//----------------------------------------------------------------------
PredictionModifiedNoExtrap::~PredictionModifiedNoExtrap() {
  // We created this in the constructor so it's our responsibility
  delete fExtrap;
}
} // namespace ana