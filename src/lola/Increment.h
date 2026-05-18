/*
 * (C) Copyright 2026 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#pragma once

#include "mist/base/Increment.h"

namespace eckit {
class Configuration;
}  // namespace eckit

namespace lola {

class Increment : public mist::base::Increment {
 public:
  using mist::base::Increment::Increment;

  void read(const eckit::Configuration & config);
  void write(const eckit::Configuration & config) const;
};

}  // namespace lola
