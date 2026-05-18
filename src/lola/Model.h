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
}
namespace lola {
class State;
}

namespace lola {

// Stub satisfying the full MODEL::Model interface required by instantiateModelFactory.
// names() returns {} so no model-specific factory entry is registered.
// At runtime, specify "name: PseudoModel" in the model YAML section.
class Model : public util::Printable {
 public:
  static std::vector<std::string> names() { return {}; }

  Model(const mist::base::Geometry &, const eckit::Configuration &) {}

  void initialize(State &) const {}
  void step(State &, const mist::base::ModelAuxControl &) const {}
  void finalize(State &) const {}
  const util::Duration & timeResolution() const { return tstep_; }

 private:
  void print(std::ostream & os) const override { os << "lola::Model (stub)"; }
  util::Duration tstep_;  // default zero-duration; stub is never called at runtime
};

}  // namespace lola
