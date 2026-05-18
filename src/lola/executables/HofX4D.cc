/*
 * (C) Copyright 2026 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#include "lola/Traits.h"
#include "oops/runs/HofX4D.h"
#include "oops/runs/Run.h"
#include "ufo/ObsTraits.h"
#include "ufo/instantiateObsFilterFactory.h"

int main(int argc, char ** argv) {
  oops::Run run(argc, argv);
  ufo::instantiateObsFilterFactory();
  oops::HofX4D<lola::Traits, ufo::ObsTraits> hofx;
  return run.execute(hofx);
}
