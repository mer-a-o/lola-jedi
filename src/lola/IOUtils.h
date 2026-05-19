/*
 * (C) Copyright 2026 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#pragma once

#include <map>
#include <string>
#include <vector>

#include "atlas/array.h"
#include "atlas/field/FieldSet.h"
#include "atlas/functionspace/FunctionSpace.h"
#include "atlas/option.h"
#include "eckit/config/LocalConfiguration.h"
#include "oops/base/Variables.h"
#include "oops/util/FieldSetHelpers.h"
#include "oops/util/Logger.h"

namespace lola {

/// Read a FieldSet from file with optional variable name remapping.
///
/// Reads the optional "variable name map" key from config, which maps JEDI/CF
/// variable names to the names as they appear in the NetCDF file. Variables
/// not listed in the map are read using their JEDI name directly.
///
/// Example config:
///   variable name map:
///     air_temperature: T
///     specific_humidity: Q
///     eastward_wind: U
///     northward_wind: V
///     air_pressure_at_surface: PS
inline void readFieldSetMapped(const eckit::mpi::Comm & comm,
                               const atlas::FunctionSpace & fspace,
                               const oops::Variables & vars,
                               const eckit::Configuration & config,
                               atlas::FieldSet & fset) {
  // Parse optional name map: JEDI name -> file name
  std::map<std::string, std::string> nameMap;
  if (config.has("variable name map")) {
    const eckit::LocalConfiguration mapConf(config, "variable name map");
    for (const auto & jediName : mapConf.keys()) {
      nameMap[jediName] = mapConf.getString(jediName);
    }
  }

  if (nameMap.empty()) {
    util::readFieldSet(comm, fspace, vars, config, fset);
    return;
  }

  oops::Log::trace() << "lola::readFieldSetMapped: applying variable name map" << std::endl;

  // Build file-name and level-count vectors, preserving level metadata from vars
  std::vector<std::string> fileNames;
  std::vector<size_t> levelCounts;
  fileNames.reserve(vars.size());
  levelCounts.reserve(vars.size());
  for (size_t i = 0; i < vars.size(); ++i) {
    const auto & var = vars[i];
    auto it = nameMap.find(var.name());
    fileNames.push_back(it != nameMap.end() ? it->second : var.name());
    levelCounts.push_back(static_cast<size_t>(var.getLevels()));
  }

  // Read into a temporary FieldSet using the file names
  atlas::FieldSet tempFset;
  util::readFieldSet(comm, fspace, levelCounts, fileNames, config, tempFset);

  // Copy fields into fset under their JEDI/CF names
  for (size_t i = 0; i < vars.size(); ++i) {
    const std::string & cfName   = vars[i].name();
    const std::string & fileName = fileNames[i];
    const atlas::Field & srcField = tempFset.field(fileName);
    atlas::Field dstField = fspace.createField<double>(
        atlas::option::name(cfName) | atlas::option::levels(levelCounts[i]));
    auto srcView = atlas::array::make_view<double, 2>(srcField);
    auto dstView = atlas::array::make_view<double, 2>(dstField);
    for (atlas::idx_t j = 0; j < srcView.shape(0); ++j)
      for (atlas::idx_t k = 0; k < srcView.shape(1); ++k)
        dstView(j, k) = srcView(j, k);
    fset.add(dstField);
  }
}

}  // namespace lola
