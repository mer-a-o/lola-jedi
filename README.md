# lola-jedi — LOLA: Lat/Lon OOPS Layer/Adapter

LOLA is a minimal JEDI model interface for running `oops::HofX3D` and `oops::HofX4D`
on regular latitude/longitude grids, with MERRA-2 as the primary target grid.
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

## MERRA-2 grid

MERRA-2 native horizontal resolution: **576 × 361** (0.625° longitude × 0.5° latitude).

```yaml
geometry:
  function space: StructuredColumns
  grid:
    name: L576x361
  levels: 72
```

The 72-level hybrid sigma-pressure vertical coordinate uses 73 interface coefficients
(`ak` in Pa, `bk` dimensionless). Full coefficient values are in
[`src/lola/tests/testinput/geometry.yaml`](src/lola/tests/testinput/geometry.yaml).

## Building

LOLA is built as part of `jedi-bundle`. No extra options are required:

```bash
cmake -DCMAKE_BUILD_TYPE=Release /path/to/jedi-bundle
make lola lola_hofx3d.x lola_hofx4d.x
```

To run the geometry unit test:

```bash
make lola_test_geometry.x
ctest -R lola_test_geometry
```

## Executables

| Executable | Algorithm | Use |
|------------|-----------|-----|
| `lola_hofx3d.x` | `oops::HofX3D` | Single-time observation operator at one state |
| `lola_hofx4d.x` | `oops::HofX4D` + `PseudoModel` | 4D observation operator over a time window |

### HofX3D

```bash
lola_hofx3d.x hofx3d.yaml
```

Minimum YAML structure — see
[`src/lola/tests/testinput/hofx3d.yaml`](src/lola/tests/testinput/hofx3d.yaml)
for a full template.

```yaml
geometry:
  function space: StructuredColumns
  grid:
    name: L576x361
  levels: 72
  ...

state:
  date: '2020-01-15T00:00:00Z'
  variables: [...]
  filepath: /path/to/MERRA2.inst3_3d_asm_Nv.20200115_0000z.nc4

observations:
  - obs space: ...
    obs operator: ...
```

### HofX4D with PseudoModel

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

MERRA-2 variable names differ from the CF/GeoVaLs names expected by UFO observation
operators. Map them via VADER in the run YAML — no C++ code changes needed:

```yaml
variable change:
  variable change name: Model2GeoVaLs
  vader:
    vader variables:
      - name: air_temperature
        # add VADER recipes as needed
```

## Dependencies

- **oops** (`feature/mist_demo_using_base` branch) — algorithms and interfaces
- **mist** (part of oops) — `mist::base` geometry, state, increment, variable change
- **ATLAS** — `StructuredColumns` function space for lat/lon grids
- **ioda** / **ufo** — observation data and operators
- **saber** / **vader** — background error covariance and variable transforms
