/*
 * (C) Copyright 2026 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "lola/State.h"

#include "eckit/config/Configuration.h"
#include "eckit/exception/Exceptions.h"
#include "lola/IOUtils.h"
#include "mist/base/Geometry.h"
#include "oops/base/Variables.h"
#include "oops/util/DateTime.h"
#include "oops/util/FieldSetHelpers.h"
#include "oops/util/Logger.h"

namespace lola {

State::State(const mist::base::Geometry & geom, const eckit::Configuration & config)
    : mist::base::State(geom,
                        oops::Variables(config, "variables"),
                        util::DateTime(config.getString("date")),
                        false) {
  oops::Log::trace() << "lola::State::State from config starting" << std::endl;
  if (config.has("filepath")) {
    read(config);
  }
  oops::Log::trace() << "lola::State::State from config done" << std::endl;
}

void State::read(const eckit::Configuration & config) {
  oops::Log::trace() << "lola::State::read starting" << std::endl;
  for (const auto & var : vars_) {
    ASSERT(var.getLevels() != -1);
  }
  fields_.fieldSet().clear();
  readFieldSetMapped(geomData_.comm(), geomData_.functionSpace(), vars_, config,
                     fields_.fieldSet());
  setFieldMetadata();
  oops::Log::trace() << "lola::State::read done" << std::endl;
}

void State::write(const eckit::Configuration & config) const {
  oops::Log::trace() << "lola::State::write starting " << this->validTime() << std::endl;
  util::writeFieldSet(geomData_.comm(), config, fields_.fieldSet());
  oops::Log::trace() << "lola::State::write done" << std::endl;
}

}  // namespace lola
