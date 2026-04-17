// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vcomet2_cpu.h for the primary calling header

#ifndef VERILATED_VCOMET2_CPU___024ROOT_H_
#define VERILATED_VCOMET2_CPU___024ROOT_H_  // guard

#include "verilated.h"


class Vcomet2_cpu__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vcomet2_cpu___024root final {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(reset,0,0);
    VL_OUT8(read_req,0,0);
    VL_IN8(read_data_valid,0,0);
    VL_OUT8(write_req,0,0);
    VL_OUT8(memory_mask,3,0);
    VL_OUT8(error_instruction,0,0);
    CData/*0:0*/ comet2_cpu__DOT____VlemCall_9__signed_sub_overflow;
    CData/*0:0*/ comet2_cpu__DOT____VlemCall_8__signed_sub_overflow;
    CData/*0:0*/ comet2_cpu__DOT____VlemCall_7__signed_add_overflow;
    CData/*0:0*/ comet2_cpu__DOT____VlemCall_6__signed_sub_overflow;
    CData/*0:0*/ comet2_cpu__DOT____VlemCall_5__signed_sub_overflow;
    CData/*0:0*/ comet2_cpu__DOT____VlemCall_4__signed_add_overflow;
    CData/*0:0*/ comet2_cpu__DOT____VlemCall_2__uses_x_field;
    CData/*0:0*/ comet2_cpu__DOT____VlemCall_1__uses_r_field;
    CData/*0:0*/ comet2_cpu__DOT____VlemCall_0__is_two_word_instruction;
    CData/*0:0*/ comet2_cpu__DOT__of;
    CData/*0:0*/ comet2_cpu__DOT__sf;
    CData/*0:0*/ comet2_cpu__DOT__zf;
    CData/*7:0*/ comet2_cpu__DOT__state;
    CData/*3:0*/ comet2_cpu__DOT__mem_read_kind;
    CData/*3:0*/ comet2_cpu__DOT__mem_write_kind;
    CData/*3:0*/ comet2_cpu__DOT__dest_reg;
    CData/*7:0*/ comet2_cpu__DOT__main__DOT__op;
    CData/*3:0*/ comet2_cpu__DOT__main__DOT__r;
    CData/*3:0*/ comet2_cpu__DOT__main__DOT__x;
    CData/*0:0*/ comet2_cpu__DOT__main__DOT__shifted_out;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __VactPhaseResult;
    CData/*0:0*/ __VnbaPhaseResult;
    SData/*15:0*/ comet2_cpu__DOT____VlemCall_3__indexed_addr;
    SData/*15:0*/ comet2_cpu__DOT__pr;
    SData/*15:0*/ comet2_cpu__DOT__sp;
    SData/*15:0*/ comet2_cpu__DOT__svc_vector_addr;
    SData/*15:0*/ comet2_cpu__DOT__ir;
    SData/*15:0*/ comet2_cpu__DOT__operand;
    SData/*15:0*/ comet2_cpu__DOT__effective_addr;
    SData/*15:0*/ comet2_cpu__DOT__write_word;
    SData/*15:0*/ comet2_cpu__DOT__main__DOT__rhs;
    SData/*15:0*/ comet2_cpu__DOT__main__DOT__result;
    VL_OUT(read_addr,31,0);
    VL_IN(read_data,31,0);
    VL_OUT(write_addr,31,0);
    VL_OUT(write_data,31,0);
    VL_OUT(debug,31,0);
    IData/*16:0*/ comet2_cpu__DOT__main__DOT__wide;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<SData/*15:0*/, 8> comet2_cpu__DOT__gr;
    VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vcomet2_cpu__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vcomet2_cpu___024root(Vcomet2_cpu__Syms* symsp, const char* namep);
    ~Vcomet2_cpu___024root();
    VL_UNCOPYABLE(Vcomet2_cpu___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
