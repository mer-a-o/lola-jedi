/*
 * (C) Copyright 2026 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "oops/util/Duration.h"
#include "oops/util/Printable.h"

namespace eckit {
class Configuration;
}
namespace mist::base {
class Geometry;
class ModelAuxControl;
class ModelAuxIncrement;
}
namespace lola {
class Increment;
class State;
}

namespace lola {

// Stub satisfying the full MODEL::LinearModel interface required by instantiateLinearModelFactory.
// names() returns {} so no model-specific TLM factory entry is registered.
// At runtime, use "Identity" or "HTLM" linear model names in the YAML.
class LinearModel : public util::Printable {
 public:
  static std::vector<std::string> names() { return {}; }

  LinearModel(const mist::base::Geometry &, const eckit::Configuration &) {}

  void setTrajectory(const lola::State &, lola::State &,
                     const mist::base::ModelAuxControl &) {}
  void initializeTL(lola::Increment &) const {}
  void stepTL(lola::Increment &, const mist::base::ModelAuxIncrement &) const {}
  void finalizeTL(lola::Increment &) const {}
  void initializeAD(lola::Increment &) const {}
  void stepAD(lola::Increment &, mist::base::ModelAuxIncrement &) const {}
  void finalizeAD(lola::Increment &) const {}
  const util::Duration & timeResolution() const { return tstep_; }
  const util::Duration & stepTrajectory() const { return tstep_; }

 private:
  void print(std::ostream & os) const override { os << "lola::LinearModel (stub)"; }
  util::Duration tstep_;
};

}  // namespace lola
