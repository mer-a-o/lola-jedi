/*
 * (C) Copyright 2026 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "lola/Increment.h"

#include "eckit/config/Configuration.h"
#include "eckit/exception/Exceptions.h"
#include "mist/base/Geometry.h"
#include "oops/base/Variables.h"
#include "oops/util/DateTime.h"
#include "oops/util/FieldSetHelpers.h"
#include "oops/util/Logger.h"

namespace lola {

void Increment::read(const eckit::Configuration & config) {
  oops::Log::trace() << "lola::Increment::read starting" << std::endl;
  for (const auto & var : vars_) {
    ASSERT(var.getLevels() != -1);
  }
  fields_.fieldSet().clear();
  util::readFieldSet(geomData_.comm(), geomData_.functionSpace(), vars_, config,
                     fields_.fieldSet());
  setFieldMetadata();
  oops::Log::trace() << "lola::Increment::read done" << std::endl;
}

void Increment::write(const eckit::Configuration & config) const {
  oops::Log::trace() << "lola::Increment::write starting" << std::endl;
  util::writeFieldSet(geomData_.comm(), config, fields_.fieldSet());
  oops::Log::trace() << "lola::Increment::write done" << std::endl;
}

}  // namespace lola
