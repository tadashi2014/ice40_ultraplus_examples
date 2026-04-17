// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcomet2_cpu.h for the primary calling header

#include "Vcomet2_cpu__pch.h"

void Vcomet2_cpu___024root___ctor_var_reset(Vcomet2_cpu___024root* vlSelf);

Vcomet2_cpu___024root::Vcomet2_cpu___024root(Vcomet2_cpu__Syms* symsp, const char* namep)
 {
    vlSymsp = symsp;
    vlNamep = strdup(namep);
    // Reset structure values
    Vcomet2_cpu___024root___ctor_var_reset(this);
}

void Vcomet2_cpu___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vcomet2_cpu___024root::~Vcomet2_cpu___024root() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
