// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vcomet2_cpu__pch.h"

//============================================================
// Constructors

Vcomet2_cpu::Vcomet2_cpu(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vcomet2_cpu__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , reset{vlSymsp->TOP.reset}
    , read_req{vlSymsp->TOP.read_req}
    , read_data_valid{vlSymsp->TOP.read_data_valid}
    , write_req{vlSymsp->TOP.write_req}
    , memory_mask{vlSymsp->TOP.memory_mask}
    , error_instruction{vlSymsp->TOP.error_instruction}
    , read_addr{vlSymsp->TOP.read_addr}
    , read_data{vlSymsp->TOP.read_data}
    , write_addr{vlSymsp->TOP.write_addr}
    , write_data{vlSymsp->TOP.write_data}
    , debug{vlSymsp->TOP.debug}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vcomet2_cpu::Vcomet2_cpu(const char* _vcname__)
    : Vcomet2_cpu(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vcomet2_cpu::~Vcomet2_cpu() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vcomet2_cpu___024root___eval_debug_assertions(Vcomet2_cpu___024root* vlSelf);
#endif  // VL_DEBUG
void Vcomet2_cpu___024root___eval_static(Vcomet2_cpu___024root* vlSelf);
void Vcomet2_cpu___024root___eval_initial(Vcomet2_cpu___024root* vlSelf);
void Vcomet2_cpu___024root___eval_settle(Vcomet2_cpu___024root* vlSelf);
void Vcomet2_cpu___024root___eval(Vcomet2_cpu___024root* vlSelf);

void Vcomet2_cpu::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vcomet2_cpu::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vcomet2_cpu___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vcomet2_cpu___024root___eval_static(&(vlSymsp->TOP));
        Vcomet2_cpu___024root___eval_initial(&(vlSymsp->TOP));
        Vcomet2_cpu___024root___eval_settle(&(vlSymsp->TOP));
        vlSymsp->__Vm_didInit = true;
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vcomet2_cpu___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vcomet2_cpu::eventsPending() { return false; }

uint64_t Vcomet2_cpu::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vcomet2_cpu::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vcomet2_cpu___024root___eval_final(Vcomet2_cpu___024root* vlSelf);

VL_ATTR_COLD void Vcomet2_cpu::final() {
    contextp()->executingFinal(true);
    Vcomet2_cpu___024root___eval_final(&(vlSymsp->TOP));
    contextp()->executingFinal(false);
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vcomet2_cpu::hierName() const { return vlSymsp->name(); }
const char* Vcomet2_cpu::modelName() const { return "Vcomet2_cpu"; }
unsigned Vcomet2_cpu::threads() const { return 1; }
void Vcomet2_cpu::prepareClone() const { contextp()->prepareClone(); }
void Vcomet2_cpu::atClone() const {
    contextp()->threadPoolpOnClone();
}
