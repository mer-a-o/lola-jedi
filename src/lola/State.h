/*
 * (C) Copyright 2026 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#pragma once

#include "mist/base/State.h"

namespace eckit {
class Configuration;
}  // namespace eckit

namespace mist::base {
class Geometry;
}  // namespace mist::base

namespace lola {

class State : public mist::base::State {
 public:
  // Constructor with file I/O — reads from MERRA-2 NetCDF if "filepath" is present in config
  State(const mist::base::Geometry &, const eckit::Configuration &);

  // All other constructors (copy, geometry-change, variable-change) from base
  using mist::base::State::State;

  void read(const eckit::Configuration & config);
  void write(const eckit::Configuration & config) const;
};

}  // namespace lola
