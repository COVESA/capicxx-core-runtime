### CommonAPI C++ Core Runtime

##### Copyright
Copyright (C) 2016-2020, Bayerische Motoren Werke Aktiengesellschaft (BMW AG).

This file is part of GENIVI Project IPC Common API C++.
Contributions are licensed to the GENIVI Alliance under one or more Contribution License Agreements or MPL 2.0.

##### License
This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed with this file, you can obtain one at http://mozilla.org/MPL/2.0/.

##### CommonAPI C++ Specification and User Guide
The specification document and the user guide can be found in the CommonAPI documentation directory of the CommonAPI-Tools project as AsciiDoc document. A pdf version can be found at https://github.com/GENIVI/capicxx-core-tools/releases.

##### Further information
https://genivi.github.io/capicxx-core-tools/

##### Build Instructions for Linux

Use CMake to build this library. We assume that your source directory is _common-api-runtime_:
 
```bash
$ cd common-api-runtime
$ mkdir build
$ cd build
$ cmake -D CMAKE_INSTALL_PREFIX=/usr/local ..
$ make
$ make install
```

You can change the installation directory by the CMake variable _CMAKE_INSTALL_PREFIX_ or you can let it uninstalled (skip the _make install_ command). Please refer to the installation description of the binding runtime how to use uninstalled versions of CommonAPI.

For further build instructions (build for windows, build documentation, tests etc.) please refer to the CommonAPI tutorial.

##### Build Instructions for Android

In general for building the Android source tree the instructions found on the pages from the Android Open Source Project (AOSP) apply (https://source.android.com/setup/build/requirements).

To integrate the CommonAPI Core Runtime library into the build process, the source code together with the Android.bp file has to be inserted into the Android source tree (by simply copying or by fetching with a custom platform manifest).
When building the Android source tree, the Android.bp file is automatically found and considered by the build system.

In order that the CommonAPI Core Runtime library is also included in the Android image, the library has to be added to the PRODUCT_PACKAGES variable in one of a device/target specific makefile:

```
PRODUCT_PACKAGES += \
    libCommonAPI
```
