// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VCOMET2_CPU__SYMS_H_
#define VERILATED_VCOMET2_CPU__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vcomet2_cpu.h"

// INCLUDE MODULE CLASSES
#include "Vcomet2_cpu___024root.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES) Vcomet2_cpu__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vcomet2_cpu* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vcomet2_cpu___024root          TOP;

    // CONSTRUCTORS
    Vcomet2_cpu__Syms(VerilatedContext* contextp, const char* namep, Vcomet2_cpu* modelp);
    ~Vcomet2_cpu__Syms();

    // METHODS
    const char* name() const { return TOP.vlNamep; }
};

#endif  // guard
