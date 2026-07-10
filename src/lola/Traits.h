/*
 * (C) Copyright 2026 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#pragma once

#include <string>

#include "mist/base/ErrorCovariance.h"
#include "mist/base/Geometry.h"
#include "mist/base/LinearVariableChange.h"
#include "mist/base/ModelAuxControl.h"
#include "mist/base/ModelAuxCovariance.h"
#include "mist/base/ModelAuxIncrement.h"
#include "mist/base/ModelData.h"
#include "mist/base/VariableChange.h"
#include "lola/Increment.h"
#include "lola/LinearModel.h"
#include "lola/Model.h"
#include "lola/State.h"
#include "oops/generic/UnstructuredInterpolator.h"

namespace lola {

struct Traits {
 public:
  static std::string name() { return "LOLA"; }
  static std::string nameCovar() { return "mist::ErrorCovariance::Identity"; }

  using Covariance           = mist::ErrorCovariance;
  using Increment            = lola::Increment;
  using State                = lola::State;

  using Geometry             = mist::Geometry;
  using LinearModel          = lola::LinearModel;
  using LinearVariableChange = mist::LinearVariableChange;
  using LocalInterpolator    = oops::UnstructuredInterpolator;
  using Model                = lola::Model;
  using ModelAuxControl      = mist::ModelAuxControl;
  using ModelAuxIncrement    = mist::ModelAuxIncrement;
  using ModelAuxCovariance   = mist::ModelAuxCovariance;
  using ModelData            = mist::ModelData;
  using VariableChange       = mist::VariableChange;
};

}  // namespace lola
