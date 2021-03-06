#include "CAFAna/Analysis/common_fit_definitions.h"

using namespace ana;

// I miss python
std::string sanitize(std::string word) {
  uint i = 0;

  while (i < word.size()) {
    if (word[i] == '(' || word[i] == ')') {
      word.erase(i, 1);
      continue;
    }
    i++;
  }
  return word;
}

void RemovePars(std::vector<const IFitVar *> &osclist,
                std::vector<std::string> const &namesToRemove) {

  osclist.erase(std::remove_if(osclist.begin(), osclist.end(),
                               [&](const IFitVar *s) {
                                 return (std::find(namesToRemove.begin(),
                                                   namesToRemove.end(),
                                                   sanitize(s->ShortName())) !=
                                         namesToRemove.end());
                               }),
                osclist.end());
}

// Function to set the binning based on the parameter short name
void GetParameterBinning(std::string parName, int &nBins, double &min,
                         double &max) {

  // Defaults
  nBins = 200;
  min = 0;
  max = 1;

  if (parName == "th13") {
    nBins = 200;
    min = 0.12;
    max = 0.21;
    return;
  }
  if (parName == "deltapi") {
    nBins = 200;
    min = -1;
    max = 1;
  }
  if (parName == "dmsq32scaled" or parName == "dmsq32") {
    nBins = 200;
    min = 2.25;
    max = 2.65;
  }
  if (parName == "ssth23") {
    nBins = 200;
    min = 0.38;
    max = 0.62;
  }
  if (parName == "ss2th12") {
    nBins = 200;
    min = 0.8;
    max = 0.9;
  }
  if (parName == "dmsq21") {
    nBins = 200;
    min = 6e-5;
    max = 9e-5;
  }
  if (parName == "rho") {
    nBins = 200;
    min = 2.5;
    max = 3.2;
  }
  return;
}

// Likely to have bugs in the translation between what I want to look at, and
// what CAFAna wants to show me...
void SetOscillationParameter(osc::IOscCalcAdjustable *calc,
                             std::string parName, double parVal, int hie) {

  if (parName == "th13")
    calc->SetTh13(parVal);
  else if (parName == "deltapi")
    calc->SetdCP(TMath::Pi() * parVal);
  else if (parName == "dmsq32scaled" or parName == "dmsq32")
    calc->SetDmsq32(hie < 0 ? -1 * parVal / 1000. : parVal / 1000.);
  else if (parName == "ssth23")
    calc->SetTh23(asin(sqrt(parVal)));
  else if (parName == "ss2th12")
    calc->SetTh12(asin(sqrt(parVal)) / 2);
  else if (parName == "dmsq21")
    calc->SetDmsq21(parVal);
  else if (parName == "rho")
    calc->SetRho(parVal);
  return;
}

// This function unpacks the
TH1 *GetAsimovHist(std::vector<std::string> plotVarVect) {
  TH1 *returnHist = NULL;

  int nBinsX = 0, nBinsY = 0;
  double minX = 0, maxX = 0, minY = 0, maxY = 0;

  // Now get the binnings etc for the histograms
  if (plotVarVect.size() > 0)
    GetParameterBinning(plotVarVect[0], nBinsX, minX, maxX);
  if (plotVarVect.size() > 1)
    GetParameterBinning(plotVarVect[1], nBinsY, minY, maxY);

  if (plotVarVect.size() == 1) {
    returnHist = new TH1D(plotVarVect[0].c_str(),
                          (plotVarVect[0] + ";" + plotVarVect[0]).c_str(),
                          nBinsX, minX, maxX);
  } else if (plotVarVect.size() > 1) {
    returnHist = new TH2D((plotVarVect[0] + "_" + plotVarVect[1]).c_str(),
                          (plotVarVect[0] + "_" + plotVarVect[1] + ";" +
                           plotVarVect[0] + ";" + plotVarVect[1])
                              .c_str(),
                          nBinsX, minX, maxX, nBinsY, minY, maxY);
  } else {
    std::cerr << "Something went wrong when setting up the histogram!"
              << std::endl;
    exit(1);
  }

  return returnHist;
}

// This function finds OA parameters by name, and returns the value from a calculator
double FindOscVal(osc::IOscCalcAdjustable *calc, std::string name){

  if (name == "th13")
    return calc->GetTh13();
  else if (name == "deltapi")
    // return a value in [-1,1]
    return fmod(calc->GetdCP()/TMath::Pi()+1, 2)-1;
  else if (name == "dmsq32scaled" or name == "dmsq32")
    return calc->GetDmsq32();
  else if (name == "ssth23")
    return sin(calc->GetTh23())*sin(calc->GetTh23());
  else if (name == "ss2th12")
    return sin(2*calc->GetTh12())*sin(2*calc->GetTh12());
  else if (name == "dmsq21")
    return calc->GetDmsq21();
  else if (name == "rho")
    return calc->GetRho(); 
  return 0;
}

TGraph* GetFitPoint(std::vector<std::string> plotVarVect, osc::IOscCalcAdjustable *oscCalc){
  
  // Get the values of the x and y co-ordinate. If this is 1-D, y=0
  double xVal = 0;
  double yVal = 0;
  
  if (plotVarVect.size() > 0) xVal = FindOscVal(oscCalc, plotVarVect[0]);
  if (plotVarVect.size() > 1) yVal = FindOscVal(oscCalc, plotVarVect[1]);

  // Now make a TGraph to return
  TGraph *ret = new TGraph(1, &xVal, &yVal);
  return ret;
}

char const *def_stateFname = "common_state_mcc11v3_broken.root";
char const *def_outputFname = "asimov_test.root";
char const *def_plotVars = "th13:deltapi";
char const *def_systSet = "nosyst";
char const *def_sampleString = "ndfd";
char const *def_penaltyString = "";
int const def_hie = 1;
char const *def_asimov_set = "0";
char const *def_fakeDataShift = "";

// Acceptable parameter names: th13, ss2th13, delta(pi), th23, ssth23, ss2th23,
// dmsq32, dmsq32scaled, tsth12, ss2th12, dmsq21, rho
void asimov_joint(std::string stateFname = def_stateFname,
                  std::string outputFname = def_outputFname,
                  std::string plotVars = def_plotVars,
                  std::string systSet = def_systSet,
                  std::string sampleString = def_sampleString,
                  std::string penaltyString = def_penaltyString,
                  int hie = def_hie, std::string asimov_set = def_asimov_set,
                  std::string fakeDataShift = def_fakeDataShift) {

  gROOT->SetBatch(1);
  gRandom->SetSeed(0);

  std::vector<std::string> plotVarVect = SplitString(plotVars, ':');

  // Get the systematics to use
  std::vector<const ISyst *> systlist = GetListOfSysts(systSet);

  // Oscillation parameters to start with
  std::vector<const IFitVar *> oscVarsAll = GetOscVars("alloscvars", hie);

  // Remove the parameters to be scanned
  std::vector<const IFitVar *> oscVars = oscVarsAll;

  // This is very hacky... third elements in the list are used to define the
  // strip to loop at this time
  int yVal = -999;
  int xVal = -999;

  bool isGlobal = true;
  bool isSinglePoint = false;

  if (plotVarVect.size() > 0)
    RemovePars(oscVars, {plotVarVect[0]});
  if (plotVarVect.size() > 1)
    RemovePars(oscVars, {plotVarVect[1]});
  if (plotVarVect.size() > 2)
    xVal = stoi(plotVarVect[2]);
  if (plotVarVect.size() > 3) {
    isSinglePoint = true;
    yVal = stoi(plotVarVect[3]);
  }

  // Should we save the best fit information as a TGraph?
  bool saveBestFit = true;
  if (xVal > 0) saveBestFit = false;
  if (isSinglePoint) saveBestFit = true;

  // One man's continuing struggle with hacky fixes stemming from the
  // oscillation probability being buggy as hell
  if (plotVars.find("dmsq32") != std::string::npos)
    RemovePars(oscVars, {"dmsq32NHscaled", "dmsq32IHscaled"});

  TFile *fout = new TFile(outputFname.c_str(), "RECREATE");
  fout->cd();

  FitTreeBlob asimov_tree("asimov_tree", "meta_tree");
  asimov_tree.SetDirectory(fout);

  // This remains the same throughout... there is one true parameter set for
  // this Asimov set
  osc::IOscCalcAdjustable *trueOsc = NuFitOscCalc(hie, 1, asimov_set);

  // Save the true value
  if (saveBestFit){
    TGraph *true_point = GetFitPoint(plotVarVect, trueOsc);
    true_point->Write("true_point");
    delete true_point;
  }

  // Save the true information
  SaveTrueOAParams(fout, trueOsc);

  // Start by performing a minimization across the whole space, this defines the
  // minimum chi2!
  osc::IOscCalcAdjustable *testOsc = NuFitOscCalc(hie, 1, asimov_set);

  IExperiment *penalty_nom = GetPenalty(hie, 1, penaltyString, asimov_set);
  SystShifts trueSyst = GetFakeDataGeneratorSystShift(fakeDataShift);
  SystShifts testSyst = kNoShift;

  // For the nominal, try all octant/dCP combos (shouldn't get it wrong)
  std::map<const IFitVar *, std::vector<double>> oscSeedsAll = {};
  oscSeedsAll[&kFitSinSqTheta23] = {.4, .6};
  oscSeedsAll[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};

  // For the asimov, seed whatever isn't fixed
  std::map<const IFitVar *, std::vector<double>> oscSeeds = {};
  if (std::find(plotVarVect.begin(), plotVarVect.end(), "deltapi") ==
      plotVarVect.end())
    oscSeeds[&kFitDeltaInPiUnits] = {-1, -0.5, 0, 0.5};
  if (std::find(plotVarVect.begin(), plotVarVect.end(), "ssth23") ==
      plotVarVect.end())
    oscSeeds[&kFitSinSqTheta23] = {.4, .6};

  // Only do this if you're told to?
  double globalmin = 0;

  if (isGlobal) {
    globalmin = RunFitPoint(stateFname, sampleString, trueOsc, trueSyst, false,
                            oscVarsAll, systlist, testOsc, testSyst,
                            oscSeedsAll, penalty_nom, MinuitFitter::kNormal, nullptr);
    
    // Save this info
    if (saveBestFit){
      fout->cd();
      TGraph *best_fit = GetFitPoint(plotVarVect, testOsc);
      best_fit->Write("best_fit");
      delete best_fit;
    }
  }
  delete penalty_nom;
  delete testOsc;
  std::cout << "Found a minimum global chi2 of: " << globalmin << std::endl;

  // Need to set up the histogram to fill
  TH1 *sens_hist = GetAsimovHist(plotVarVect);
  if (!sens_hist) {
    std::cout << "ERROR: sens_hist not correctly produced!" << std::endl;
    abort();
  }

  int xMin = 0;
  int xMax = sens_hist->GetNbinsX();

  if (xVal >= 0) {
    xMin = xVal;
    xMax = xVal + 1;
  }

  int yMin = 0;
  int yMax = sens_hist->GetNbinsY();

  if (yVal >= 0) {
    yMin = yVal;
    yMax = yVal + 1;
  }

  // Loop over whatever bins you're supposed to
  for (int xBin = xMin; xBin < xMax; ++xBin) {
    double xCenter = sens_hist->GetXaxis()->GetBinCenter(xBin + 1);

    for (int yBin = yMin; yBin < yMax; ++yBin) {
      double yCenter = sens_hist->GetYaxis()->GetBinCenter(yBin + 1);

      osc::IOscCalcAdjustable *testOsc = NuFitOscCalc(hie, 1, asimov_set);
      if (plotVarVect.size() > 0)
        SetOscillationParameter(testOsc, plotVarVect[0], xCenter, hie);
      if (plotVarVect.size() > 1)
        SetOscillationParameter(testOsc, plotVarVect[1], yCenter, hie);

      IExperiment *penalty = GetPenalty(hie, 1, penaltyString, asimov_set);

      double chisqmin =
          RunFitPoint(stateFname, sampleString, trueOsc, trueSyst, false,
                      oscVars, systlist, testOsc, testSyst, oscSeeds, penalty,
                      MinuitFitter::kNormal, nullptr, &asimov_tree);

      // Save relevant values into the tree and histogram
      double chisqdiff = chisqmin - globalmin;
      asimov_tree.throw_tree->Branch("chisqmin", &chisqmin);
      asimov_tree.throw_tree->Branch("chisqdiff", &chisqdiff);
      asimov_tree.throw_tree->Branch("globalmin", &globalmin);
      asimov_tree.throw_tree->Branch("xVal", &xCenter);
      asimov_tree.throw_tree->Branch("xName", &plotVarVect[0]);
      asimov_tree.throw_tree->Branch("isGlobal", &isGlobal);
      if (plotVarVect.size() > 1)
        asimov_tree.throw_tree->Branch("yVal", &yCenter);
      if (plotVarVect.size() > 1)
        asimov_tree.throw_tree->Branch("yName", &plotVarVect[1]);
      asimov_tree.Fill();
      sens_hist->SetBinContent(xBin + 1, yBin + 1, chisqdiff);

      delete penalty;
      delete testOsc;

      if (isSinglePoint) { // exit early if we are only running 1 point
        break;
      }
    }
    if (isGlobal &&
        isSinglePoint) { // exit early if we are only running 1 point
      break;
    }
  }

  // Save the histogram, and do something sensible with the name
  fout->cd();
  asimov_tree.Write();
  sens_hist->Write();
  fout->Write();
  fout->Close();
}

#ifndef __CINT__
int main(int argc, char const *argv[]) {

  gROOT->SetMustClean(false);

  std::string stateFname = (argc > 1) ? argv[1] : def_stateFname;
  std::string outputFname = (argc > 2) ? argv[2] : def_outputFname;
  std::string plotVars = (argc > 3) ? argv[3] : def_plotVars;
  std::string systSet = (argc > 4) ? argv[4] : def_systSet;
  std::string sampleString = (argc > 5) ? argv[5] : def_sampleString;
  std::string penaltyString = (argc > 6) ? argv[6] : def_penaltyString;
  int hie = (argc > 7) ? atoi(argv[7]) : def_hie;
  std::string asimov_set = (argc > 8) ? argv[8] : def_asimov_set;
  std::string fakeDataShift = (argc > 9) ? argv[9] : def_fakeDataShift;

  asimov_joint(stateFname, outputFname, plotVars, systSet, sampleString,
               penaltyString, hie, asimov_set, fakeDataShift);
}
#endif
