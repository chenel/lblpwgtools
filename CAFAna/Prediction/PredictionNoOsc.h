#pragma once

#include "CAFAna/Prediction/IPrediction.h"
#include "CAFAna/Prediction/PredictionGenerator.h"

#include "CAFAna/Core/Loaders.h"
#include "CAFAna/Core/IRecordSource.h"

namespace ana {
/// Prediction that wraps a simple Spectrum
class PredictionNoOsc : public IPrediction {
public:
  PredictionNoOsc(IInteractionSource& src, const HistAxis& axis);

  static std::unique_ptr<PredictionNoOsc> LoadFrom(TDirectory *dir, const std::string& name);
  virtual void SaveTo(TDirectory *dir, const std::string& name) const override;

  double POT() { return fSpectrum.POT(); }

  void OverridePOT(double pot) {
    fSpectrum.OverridePOT(pot);
    fSpectrumNCTot.OverridePOT(pot);
    fSpectrumNC.OverridePOT(pot);
    fSpectrumNCAnti.OverridePOT(pot);
    fSpectrumNumu.OverridePOT(pot);
    fSpectrumNumubar.OverridePOT(pot);
    fSpectrumNue.OverridePOT(pot);
    fSpectrumNuebar.OverridePOT(pot);
  }

  virtual Spectrum Predict(osc::IOscCalc * /*calc*/) const override {
    return fSpectrum;
  }

  virtual Spectrum PredictComponent(osc::IOscCalc *calc,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override;

#ifdef CAFANA_USE_STAN
  virtual Spectrum Predict(osc::IOscCalcStan* /*calc*/) const override {
    return fSpectrum;
  }
  virtual Spectrum PredictComponent(osc::IOscCalcStan* /*calc*/,
                                    Flavors::Flavors_t flav,
                                    Current::Current_t curr,
                                    Sign::Sign_t sign) const override
  {
    return PredictComponent((osc::IOscCalc*)0, flav, curr, sign);
  }
#endif

protected:
  PredictionNoOsc(const Spectrum &s, const Spectrum &sNCTot, const Spectrum &sNC, 
                  const Spectrum &sNCbar, const Spectrum &sNumu,
                  const Spectrum &sNumubar, const Spectrum &sNue,
                  const Spectrum &sNuebar)
      : fSpectrum(s), fSpectrumNCTot(sNCTot), fSpectrumNC(sNC), fSpectrumNCAnti(sNCbar), 
        fSpectrumNumu(sNumu), fSpectrumNumubar(sNumubar), fSpectrumNue(sNue),
        fSpectrumNuebar(sNuebar) {}

  Spectrum fSpectrum;

  Spectrum fSpectrumNCTot;
  Spectrum fSpectrumNC; 
  Spectrum fSpectrumNCAnti;
  Spectrum fSpectrumNumu;
  Spectrum fSpectrumNumubar;
  Spectrum fSpectrumNue;
  Spectrum fSpectrumNuebar;
};

  /* TODO think about generators
class NoOscPredictionGenerator : public IPredictionGenerator {
public:
  NoOscPredictionGenerator(HistAxis axis, Cut cut, Weight wei = kUnweighted)
      : fAxis(axis), fCut(cut), fWei(wei) {
  }

  virtual std::unique_ptr<IPrediction>
  Generate(Loaders &loaders,
           const SystShifts &shiftMC = kNoShift) const override {
    SpectrumLoaderBase &loader = loaders.GetLoader(caf::kNEARDET, Loaders::kMC);
    return std::unique_ptr<IPrediction>(
        new PredictionNoOsc(loader, fAxis, fCut, shiftMC, fWei));
  }

protected:
  HistAxis fAxis;
  Cut fCut;
  Weight fWei;
};
  */

} // namespace ana
