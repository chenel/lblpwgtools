#pragma once

#include "CAFAna/Core/ReweightableSpectrum.h"

#include "CAFAna/Core/Binning.h"
#include "CAFAna/Core/FwdDeclare.h"
#include "CAFAna/Core/Spectrum.h"
#include "CAFAna/Core/SpectrumLoaderBase.h"
#include "CAFAna/Core/StanTypedefs.h"

#include <string>

#include "TMD5.h"

class TH2;
class TH2D;

#include "CAFAna/Core/FwdDeclare.h"

namespace ana
{
  class Binning;

  struct OscCache
  {
    mutable std::unique_ptr<TMD5> fOscCacheHash;
    mutable Spectrum fOscCacheSpect;

    OscCache()
      : fOscCacheSpect(Spectrum::Uninitialized())
    {}
  };

  /// %Spectrum with true energy information, allowing it to be oscillated
  class OscillatableSpectrum: public ReweightableSpectrum, protected OscCache
  {
  public:
    friend class SpectrumLoaderBase;
    friend class SpectrumLoader;
    friend class NullLoader;

    OscillatableSpectrum(const std::string& label,
                         const Binning& bins,
                         SpectrumLoaderBase& loader,
                         const Var& var,
                         const Cut& cut,
                         const SystShifts& shift = kNoShift,
                         const Var& wei = kUnweighted);

    OscillatableSpectrum(SpectrumLoaderBase& loader,
                         const HistAxis& axis,
                         const Cut& cut,
                         const SystShifts& shift = kNoShift,
                         const Var& wei = kUnweighted);

    /// The only valid thing to do with such a spectrum is to assign something
    /// else into it.
    static OscillatableSpectrum Uninitialized(){return OscillatableSpectrum();}

    ~OscillatableSpectrum();

    /// Copy constructor
    OscillatableSpectrum(const OscillatableSpectrum& rhs);
    OscillatableSpectrum(OscillatableSpectrum&& rhs);
    /// Assignment operator
    OscillatableSpectrum& operator=(const OscillatableSpectrum& rhs);
    OscillatableSpectrum& operator=(OscillatableSpectrum&& rhs);

    // Expose these ones directly
    using ReweightableSpectrum::Fill;
    using ReweightableSpectrum::ToTH2;
    using ReweightableSpectrum::Clear;

    /// Rescale bins so that \ref TrueEnergy will return \a target
    using ReweightableSpectrum::ReweightToTrueSpectrum;
    /// Rescale bins so that \ref Unoscillated will return \a target
    using ReweightableSpectrum::ReweightToRecoSpectrum;

    // These under a different name
    Spectrum Unoscillated() const {return UnWeighted();}
    Spectrum TrueEnergy() const {return WeightingVariable();}

    Spectrum Oscillated(osc::IOscCalc* calc, int from, int to) const;
    Spectrum Oscillated(osc::IOscCalcStan* calc, int from, int to) const;

    OscillatableSpectrum& operator+=(const OscillatableSpectrum& rhs);
    OscillatableSpectrum operator+(const OscillatableSpectrum& rhs) const;

    OscillatableSpectrum& operator-=(const OscillatableSpectrum& rhs);
    OscillatableSpectrum operator-(const OscillatableSpectrum& rhs) const;

    void SaveTo(TDirectory* dir, const std::string& name) const;
    static std::unique_ptr<OscillatableSpectrum> LoadFrom(TDirectory* dir, const std::string& name);

  protected:
    // Derived classes can be trusted take care of their own construction
    OscillatableSpectrum(const HistAxis& recoAxis,
                         const Binning& binsY,
                         const Var& rwVar)
      : ReweightableSpectrum(recoAxis, binsY, rwVar)
    {
    }

    /// Constructor for Uninitialized()
    OscillatableSpectrum()
    {
    }

    template<class T> Spectrum _Oscillated(osc::_IOscCalc<T>* calc, int from, int to) const;
  };
}
