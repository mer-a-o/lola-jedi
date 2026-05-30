# Build LOLA-JEDI on Discover

LOLA-JEDI has been tested with SPACK-STACK 1.9 intel on DISCOVER. To load the modules:


```bash
source /discover/nobackup/projects/gmao/advda/swell/jedi_modules/spackstack_1.9_intel
```

After loading the modules, create a `jedi-bundle` directory and copy `doc/CMakeLists.txt` to the `jedi-bundle` directory.
Next, create a `build` directory, `cd build`, and run the `ecbuild` command:

```
mkdir build
cd build
ecbuild ../
```
