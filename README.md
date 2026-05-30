# lola-jedi — LOLA: Lat/Lon OOPS Layer/Adapter

LOLA is a minimal JEDI model interface for running `oops::HofX3D` and `oops::HofX4D`
on regular latitude/longitude grids. The checked-in examples cover GEOS-CF/TEMPO
HofX3D and MERRA-2-style 72-level model grids.
It reuses the `mist::base` infrastructure from OOPS so that only file I/O and grid
configuration need to be specified here.

## Design

LOLA is built on three layers:

```
oops (algorithms: HofX3D, HofX4D, PseudoModel, …)
  └── mist::base (Geometry, State, Increment, VariableChange, …)
        └── lola (read/write for lat/lon NetCDF files + Traits wiring)
```

| Concern | Implementation |
|---------|---------------|
| Geometry | `mist::base::Geometry` with `function space: StructuredColumns` YAML config |
| State / Increment I/O | `util::readFieldSet` / `util::writeFieldSet` via ATLAS FieldSet |
| HofX3D model | Not needed — HofX3D reads a single state directly |
| HofX4D model | `oops::PseudoModel` (registered automatically); reads successive files at each time step |
| Variable mapping | `mist::base::VariableChange` wrapping VADER; configured entirely in YAML |
| Observation operators | UFO, configured in YAML |

The `lola::Model` class is a stub that satisfies the C++ interface required by
`instantiateModelFactory`. At runtime, always use `name: PseudoModel` in the YAML
`model` section — the stub is never called.

## Grids

LOLA uses ATLAS `StructuredColumns` grids. The GEOS-CF HOFX test uses a coarse
regular lon/lat grid:

```yaml
geometry:
  function space: StructuredColumns
  grid:
    type: regular_lonlat
    nx: 48
    ny: 25
  levels: 72
  levels are top down: true
```

The MERRA-2 template uses the native **576 x 361** grid
(0.625 degree longitude x 0.5 degree latitude). The 72-level hybrid
sigma-pressure vertical coordinate uses 73 interface coefficients (`ak` in Pa,
`bk` dimensionless). Full coefficient values are in
[`src/lola/tests/testinput/geometry.yaml`](src/lola/tests/testinput/geometry.yaml)
and the HOFX templates.

## Building

LOLA is built as part of `jedi-bundle`. No extra options are required:

```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Release /path/to/jedi-bundle
make -j4
```

or use `ecbuild`:

```bash
cd build
ecbuild /path/to/jedi-bundle
make -j4
```


To run the tests:

```bash
ctest -R lola_test_geometry
ctest -R lola_hofx3d_geoscf
```

The GEOS-CF HOFX CTest reads its YAML and small NetCDF inputs from
[`src/lola/tests/testinput`](src/lola/tests/testinput):

- [`hofx3d_geoscf.yaml`](src/lola/tests/testinput/hofx3d_geoscf.yaml)
- `DATA/geoscf.latlon.7.5deg.2023-08-10T00:00:00Z.nc`
- `DATA/tempo_no2_tropo.20230809T210000Z_small.nc4`

## Executables

| Executable | Algorithm | Use |
|------------|-----------|-----|
| `lola_hofx3d.x` | `oops::HofX3D` | Single-time observation operator at one state |
| `lola_hofx4d.x` | `oops::HofX4D` + `PseudoModel` | 4D observation operator over a time window |

### HofX3D

```bash
lola_hofx3d.x hofx3d_geoscf.yaml
```

Minimum YAML structure — see
[`src/lola/tests/testinput/hofx3d_geoscf.yaml`](src/lola/tests/testinput/hofx3d_geoscf.yaml)
for the GEOS-CF/TEMPO CTest and
[`src/lola/tests/testinput/hofx3d_merra2.yaml`](src/lola/tests/testinput/hofx3d_merra2.yaml)
for a MERRA-2-style template.

```yaml
geometry:
  function space: StructuredColumns
  grid:
    type: regular_lonlat
    nx: 48
    ny: 25
  levels: 72
  levels are top down: true
  ...

state:
  date: '2023-08-10T00:00:00Z'
  variables: [...]
  filepath: testinput/DATA/geoscf.latlon.7.5deg.2023-08-10T00:00:00Z
  netcdf extension: nc
  latitude south to north: false

observations:
  observers:
  - obs space: ...
    obs operator: ...
```

### HofX4D with PseudoModel

In progress...

```bash
lola_hofx4d.x hofx4d.yaml
```

`PseudoModel` reads a sequence of MERRA-2 files to propagate the state through the
assimilation window. Use the `states from template` pattern to avoid listing every
file explicitly — see
[`src/lola/tests/testinput/hofx4d.yaml`](src/lola/tests/testinput/hofx4d.yaml).

```yaml
model:
  name: PseudoModel
  tstep: PT3H
  states from template:
    template:
      variables: [...]
      filepath: /path/to/MERRA2.inst3_3d_asm_Nv.20200115_%hh%00z.nc4
    start datetime: '2020-01-15T03:00:00Z'
    number of states: 2
    pattern: '%hh%'
    start: 3
    zero padding: 2
```

## Variable mapping

Input-file variable names can differ from the CF/GeoVaLs names expected by UFO
observation operators. Map them in the `state` section with `variable name map`;
larger transformations can still be configured through VADER:

```yaml
state:
  variables:
    - air_temperature
  variable name map:
    air_temperature: T
```

## Dependencies

- **oops** (`feature/mist_demo_using_base` branch) — algorithms and interfaces
- **mist** (part of oops) — `mist::base` geometry, state, increment, variable change
- **ATLAS** — `StructuredColumns` function space for lat/lon grids
- **ioda** / **ufo** — observation data and operators
- **saber** / **vader** — background error covariance and variable transforms
