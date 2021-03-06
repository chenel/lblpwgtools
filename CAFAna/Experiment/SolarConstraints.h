#pragma once

#include "CAFAna/Experiment/IExperiment.h"

#include <memory>

namespace ana
{
  /// \brief Constraints on the parameters \f$ \Delta m^2_{21} \f$ and
  /// \f$ \sin^22\theta_{12} \f$ from solar experiments
  class SolarConstraints: public IExperiment
  {
  public:
    SolarConstraints();

    SolarConstraints(const double dmsq,  const double errorDmsq,
		     const double ss2th, const double errorSs2th);

    virtual double ChiSq(osc::IOscCalcAdjustable* osc,
                         const SystShifts& syst = SystShifts::Nominal()) const override;

    virtual void SaveTo(TDirectory* dir, const std::string& name) const override;
    static std::unique_ptr<SolarConstraints> LoadFrom(TDirectory* dir, const std::string& name);
  protected:
    double fCentralDmsq, fErrorDmsq;
    double fCentralAngle, fErrorAngle;
  };

  // http://pdg.lbl.gov/2017/tables/rpp2017-sum-leptons.pdf
  // ssth12 0.307 +/- 0.013 -> ss2th12 0.851 +/- 0.020
  const SolarConstraints kSolarConstraintsPDG2017(7.53e-5, 0.18e-5, 0.851, 0.020); 
}
