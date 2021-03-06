#pragma once

#include "CAFAna/Extrap/IExtrap.h"

namespace ana
{
  class Loaders;

  /// "Extrapolation" that simply returns the FD MC prediction
  class TrivialExtrap: public IExtrap
  {
  public:
    // This is the DUNE constructor
    TrivialExtrap(SpectrumLoaderBase& loaderNonswap,
                  SpectrumLoaderBase& loaderNue,
                  SpectrumLoaderBase& loaderNuTau,
                  const HistAxis& axis,
                  const Cut& cut,
                  const SystShifts& shift,
                  const Var& wei);

    TrivialExtrap(SpectrumLoaderBase& loaderNonswap,
                  SpectrumLoaderBase& loaderNue,
                  SpectrumLoaderBase& loaderNuTau,
                  std::string label,
                  const Binning& bins,
                  const Var& var,
                  const Cut& cut,
                  const SystShifts& shift,
                  const Var& wei);

    TrivialExtrap(Loaders& loaders,
                  std::string label,
                  const Binning& bins,
                  const Var& var,
                  const Cut& cut,
                  const SystShifts& shift = kNoShift,
                  const Var& wei = kUnweighted);

    TrivialExtrap(Loaders& loaders,
                  const HistAxis& axis,
                  const Cut& cut,
                  const SystShifts& shift = kNoShift,
                  const Var& wei = kUnweighted);

    virtual OscillatableSpectrum NueSurvComponent()       {return fNueSurv;}
    virtual OscillatableSpectrum AntiNueSurvComponent()   {return fNueSurvAnti;}

    virtual OscillatableSpectrum NumuSurvComponent()      {return fNumuSurv;}
    virtual OscillatableSpectrum AntiNumuSurvComponent()  {return fNumuSurvAnti;}

    virtual OscillatableSpectrum NueAppComponent()        {return fNueApp;}
    virtual OscillatableSpectrum AntiNueAppComponent()    {return fNueAppAnti;}

    virtual OscillatableSpectrum NumuAppComponent()       {return fNumuApp;}
    virtual OscillatableSpectrum AntiNumuAppComponent()   {return fNumuAppAnti;}

    virtual OscillatableSpectrum TauFromEComponent()      {return fTauFromE;}
    virtual OscillatableSpectrum AntiTauFromEComponent()  {return fTauFromEAnti;}

    virtual OscillatableSpectrum TauFromMuComponent()     {return fTauFromMu;}
    virtual OscillatableSpectrum AntiTauFromMuComponent() {return fTauFromMuAnti;}

    virtual Spectrum NCComponent() {return fNC;}

    virtual void SaveTo(TDirectory* dir, const std::string& name) const;
    static std::unique_ptr<TrivialExtrap> LoadFrom(TDirectory* dir, const std::string& name);

  protected:
    TrivialExtrap()
      : fNueApp    (OscillatableSpectrum::Uninitialized()),
        fNueAppAnti(OscillatableSpectrum::Uninitialized()),
        fNumuSurv    (OscillatableSpectrum::Uninitialized()),
        fNumuSurvAnti(OscillatableSpectrum::Uninitialized()),
        fNumuApp    (OscillatableSpectrum::Uninitialized()),
        fNumuAppAnti(OscillatableSpectrum::Uninitialized()),
        fNueSurv    (OscillatableSpectrum::Uninitialized()),
        fNueSurvAnti(OscillatableSpectrum::Uninitialized()),
        fTauFromE    (OscillatableSpectrum::Uninitialized()),
        fTauFromEAnti(OscillatableSpectrum::Uninitialized()),
        fTauFromMu    (OscillatableSpectrum::Uninitialized()),
        fTauFromMuAnti(OscillatableSpectrum::Uninitialized()),
        fNC(Spectrum::Uninitialized())
    {}

    OscillatableSpectrum fNueApp,    fNueAppAnti;
    OscillatableSpectrum fNumuSurv,  fNumuSurvAnti;
    OscillatableSpectrum fNumuApp,   fNumuAppAnti;
    OscillatableSpectrum fNueSurv,   fNueSurvAnti;
    OscillatableSpectrum fTauFromE,  fTauFromEAnti;
    OscillatableSpectrum fTauFromMu, fTauFromMuAnti;
    Spectrum fNC;
  };
}
