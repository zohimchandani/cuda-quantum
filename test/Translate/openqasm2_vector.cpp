/*******************************************************************************
 * Copyright (c) 2022 - 2025 NVIDIA Corporation & Affiliates.                  *
 * All rights reserved.                                                        *
 *                                                                             *
 * This source code and the accompanying materials are made available under    *
 * the terms of the Apache License 2.0 which accompanies this distribution.    *
 ******************************************************************************/

// clang-format off
// RUN: cudaq-quake  %s | cudaq-opt -canonicalize -cse -lift-array-alloc -globalize-array-values -state-prep | cudaq-translate --convert-to=openqasm2 | FileCheck %s
// clang-format on

#include <cudaq.h>
#include <fstream>

struct kernel {

  void operator()() __qpu__ {
    cudaq::qvector q(std::vector<cudaq::complex>({ M_SQRT1_2, M_SQRT1_2, 0., 0.}));
    auto result = mz(q);
  }
};

int main() {
  auto counts = cudaq::sample(kernel{});
  counts.dump();
}

// CHECK:  // Code generated by NVIDIA's nvq++ compiler
// CHECK:  OPENQASM 2.0;

// CHECK:  include "qelib1.inc";

// CHECK:  gate ZN6kernelclEv(param0)  {
// CHECK:  }

// CHECK:  qreg var0[2];
// CHECK:  ry(0.000000e+00) var0[1];
// CHECK:  ry(7.853982e-01) var0[0];
// CHECK:  cx var0[1], var0[0];
// CHECK:  ry(7.853982e-01) var0[0];
// CHECK:  cx var0[1], var0[0];
// CHECK:  creg var3[2];
// CHECK:  measure var0 -> var3;
