// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcomet2_cpu.h for the primary calling header

#include "Vcomet2_cpu__pch.h"

void Vcomet2_cpu___024root___eval_triggers_vec__act(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___eval_triggers_vec__act\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    ((IData)(vlSelfRef.clk) 
                                                     & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0)))));
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
}

bool Vcomet2_cpu___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vcomet2_cpu___024root___nba_sequent__TOP__0(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___nba_sequent__TOP__0\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_word__1__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_word__1__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_word__1__data;
    __Vfunc_comet2_cpu__DOT__bus_word__1__data = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__3__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_addr__3__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__3__word_addr;
    __Vfunc_comet2_cpu__DOT__bus_addr__3__word_addr = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_word__4__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_word__4__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_word__4__data;
    __Vfunc_comet2_cpu__DOT__bus_word__4__data = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__5__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_addr__5__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__5__word_addr;
    __Vfunc_comet2_cpu__DOT__bus_addr__5__word_addr = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__6__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_addr__6__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__6__word_addr;
    __Vfunc_comet2_cpu__DOT__bus_addr__6__word_addr = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__7__Vfuncout;
    __Vfunc_comet2_cpu__DOT__indexed_addr__7__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__7__addr;
    __Vfunc_comet2_cpu__DOT__indexed_addr__7__addr = 0;
    CData/*3:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__7__x;
    __Vfunc_comet2_cpu__DOT__indexed_addr__7__x = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__8__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_addr__8__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__8__word_addr;
    __Vfunc_comet2_cpu__DOT__bus_addr__8__word_addr = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_word__9__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_word__9__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_word__9__data;
    __Vfunc_comet2_cpu__DOT__bus_word__9__data = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__10__result;
    __Vtask_comet2_cpu__DOT__set_fr__10__result = 0;
    CData/*0:0*/ __Vtask_comet2_cpu__DOT__set_fr__10__new_of;
    __Vtask_comet2_cpu__DOT__set_fr__10__new_of = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_sub_overflow__11__a;
    __Vfunc_comet2_cpu__DOT__signed_sub_overflow__11__a = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_sub_overflow__11__b;
    __Vfunc_comet2_cpu__DOT__signed_sub_overflow__11__b = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_sub_overflow__11__result;
    __Vfunc_comet2_cpu__DOT__signed_sub_overflow__11__result = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__12__result;
    __Vtask_comet2_cpu__DOT__set_fr__12__result = 0;
    CData/*0:0*/ __Vtask_comet2_cpu__DOT__set_fr__12__new_of;
    __Vtask_comet2_cpu__DOT__set_fr__12__new_of = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__13__result;
    __Vtask_comet2_cpu__DOT__set_fr__13__result = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__14__result;
    __Vtask_comet2_cpu__DOT__set_fr__14__result = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__15__result;
    __Vtask_comet2_cpu__DOT__set_fr__15__result = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__16__result;
    __Vtask_comet2_cpu__DOT__set_fr__16__result = 0;
    CData/*0:0*/ __Vtask_comet2_cpu__DOT__set_fr__16__new_of;
    __Vtask_comet2_cpu__DOT__set_fr__16__new_of = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__17__result;
    __Vtask_comet2_cpu__DOT__set_fr__17__result = 0;
    CData/*0:0*/ __Vtask_comet2_cpu__DOT__set_fr__17__new_of;
    __Vtask_comet2_cpu__DOT__set_fr__17__new_of = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_sub_overflow__18__a;
    __Vfunc_comet2_cpu__DOT__signed_sub_overflow__18__a = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_sub_overflow__18__b;
    __Vfunc_comet2_cpu__DOT__signed_sub_overflow__18__b = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_sub_overflow__18__result;
    __Vfunc_comet2_cpu__DOT__signed_sub_overflow__18__result = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__19__result;
    __Vtask_comet2_cpu__DOT__set_fr__19__result = 0;
    CData/*0:0*/ __Vtask_comet2_cpu__DOT__set_fr__19__new_of;
    __Vtask_comet2_cpu__DOT__set_fr__19__new_of = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_add_overflow__20__a;
    __Vfunc_comet2_cpu__DOT__signed_add_overflow__20__a = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_add_overflow__20__b;
    __Vfunc_comet2_cpu__DOT__signed_add_overflow__20__b = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_add_overflow__20__result;
    __Vfunc_comet2_cpu__DOT__signed_add_overflow__20__result = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__21__result;
    __Vtask_comet2_cpu__DOT__set_fr__21__result = 0;
    CData/*0:0*/ __Vtask_comet2_cpu__DOT__set_fr__21__new_of;
    __Vtask_comet2_cpu__DOT__set_fr__21__new_of = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__22__result;
    __Vtask_comet2_cpu__DOT__set_fr__22__result = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__23__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_addr__23__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__23__word_addr;
    __Vfunc_comet2_cpu__DOT__bus_addr__23__word_addr = 0;
    CData/*7:0*/ __Vfunc_comet2_cpu__DOT__uses_r_field__24__op;
    __Vfunc_comet2_cpu__DOT__uses_r_field__24__op = 0;
    CData/*7:0*/ __Vfunc_comet2_cpu__DOT__uses_x_field__25__op;
    __Vfunc_comet2_cpu__DOT__uses_x_field__25__op = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__27__Vfuncout;
    __Vfunc_comet2_cpu__DOT__indexed_addr__27__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__27__addr;
    __Vfunc_comet2_cpu__DOT__indexed_addr__27__addr = 0;
    CData/*3:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__27__x;
    __Vfunc_comet2_cpu__DOT__indexed_addr__27__x = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__32__Vfuncout;
    __Vfunc_comet2_cpu__DOT__indexed_addr__32__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__32__addr;
    __Vfunc_comet2_cpu__DOT__indexed_addr__32__addr = 0;
    CData/*3:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__32__x;
    __Vfunc_comet2_cpu__DOT__indexed_addr__32__x = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__44__Vfuncout;
    __Vfunc_comet2_cpu__DOT__indexed_addr__44__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__44__addr;
    __Vfunc_comet2_cpu__DOT__indexed_addr__44__addr = 0;
    CData/*3:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__44__x;
    __Vfunc_comet2_cpu__DOT__indexed_addr__44__x = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__47__Vfuncout;
    __Vfunc_comet2_cpu__DOT__indexed_addr__47__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__47__addr;
    __Vfunc_comet2_cpu__DOT__indexed_addr__47__addr = 0;
    CData/*3:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__47__x;
    __Vfunc_comet2_cpu__DOT__indexed_addr__47__x = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__48__Vfuncout;
    __Vfunc_comet2_cpu__DOT__indexed_addr__48__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__48__addr;
    __Vfunc_comet2_cpu__DOT__indexed_addr__48__addr = 0;
    CData/*3:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__48__x;
    __Vfunc_comet2_cpu__DOT__indexed_addr__48__x = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__49__Vfuncout;
    __Vfunc_comet2_cpu__DOT__indexed_addr__49__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__49__addr;
    __Vfunc_comet2_cpu__DOT__indexed_addr__49__addr = 0;
    CData/*3:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__49__x;
    __Vfunc_comet2_cpu__DOT__indexed_addr__49__x = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__50__Vfuncout;
    __Vfunc_comet2_cpu__DOT__indexed_addr__50__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__50__addr;
    __Vfunc_comet2_cpu__DOT__indexed_addr__50__addr = 0;
    CData/*3:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__50__x;
    __Vfunc_comet2_cpu__DOT__indexed_addr__50__x = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__51__Vfuncout;
    __Vfunc_comet2_cpu__DOT__indexed_addr__51__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__51__addr;
    __Vfunc_comet2_cpu__DOT__indexed_addr__51__addr = 0;
    CData/*3:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__51__x;
    __Vfunc_comet2_cpu__DOT__indexed_addr__51__x = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__52__Vfuncout;
    __Vfunc_comet2_cpu__DOT__indexed_addr__52__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__52__addr;
    __Vfunc_comet2_cpu__DOT__indexed_addr__52__addr = 0;
    CData/*3:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__52__x;
    __Vfunc_comet2_cpu__DOT__indexed_addr__52__x = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__56__Vfuncout;
    __Vfunc_comet2_cpu__DOT__indexed_addr__56__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__56__addr;
    __Vfunc_comet2_cpu__DOT__indexed_addr__56__addr = 0;
    CData/*3:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__56__x;
    __Vfunc_comet2_cpu__DOT__indexed_addr__56__x = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sla_result__57__Vfuncout;
    __Vfunc_comet2_cpu__DOT__sla_result__57__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sla_result__57__value;
    __Vfunc_comet2_cpu__DOT__sla_result__57__value = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sla_result__57__amount;
    __Vfunc_comet2_cpu__DOT__sla_result__57__amount = 0;
    CData/*0:0*/ __Vfunc_comet2_cpu__DOT__sla_overflow__58__Vfuncout;
    __Vfunc_comet2_cpu__DOT__sla_overflow__58__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sla_overflow__58__value;
    __Vfunc_comet2_cpu__DOT__sla_overflow__58__value = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sla_overflow__58__amount;
    __Vfunc_comet2_cpu__DOT__sla_overflow__58__amount = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sra_result__59__Vfuncout;
    __Vfunc_comet2_cpu__DOT__sra_result__59__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sra_result__59__value;
    __Vfunc_comet2_cpu__DOT__sra_result__59__value = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sra_result__59__amount;
    __Vfunc_comet2_cpu__DOT__sra_result__59__amount = 0;
    CData/*0:0*/ __Vfunc_comet2_cpu__DOT__sra_overflow__60__Vfuncout;
    __Vfunc_comet2_cpu__DOT__sra_overflow__60__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sra_overflow__60__value;
    __Vfunc_comet2_cpu__DOT__sra_overflow__60__value = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sra_overflow__60__amount;
    __Vfunc_comet2_cpu__DOT__sra_overflow__60__amount = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sll_result__61__Vfuncout;
    __Vfunc_comet2_cpu__DOT__sll_result__61__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sll_result__61__value;
    __Vfunc_comet2_cpu__DOT__sll_result__61__value = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sll_result__61__amount;
    __Vfunc_comet2_cpu__DOT__sll_result__61__amount = 0;
    CData/*0:0*/ __Vfunc_comet2_cpu__DOT__sll_overflow__62__Vfuncout;
    __Vfunc_comet2_cpu__DOT__sll_overflow__62__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sll_overflow__62__value;
    __Vfunc_comet2_cpu__DOT__sll_overflow__62__value = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__sll_overflow__62__amount;
    __Vfunc_comet2_cpu__DOT__sll_overflow__62__amount = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__srl_result__63__Vfuncout;
    __Vfunc_comet2_cpu__DOT__srl_result__63__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__srl_result__63__value;
    __Vfunc_comet2_cpu__DOT__srl_result__63__value = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__srl_result__63__amount;
    __Vfunc_comet2_cpu__DOT__srl_result__63__amount = 0;
    CData/*0:0*/ __Vfunc_comet2_cpu__DOT__srl_overflow__64__Vfuncout;
    __Vfunc_comet2_cpu__DOT__srl_overflow__64__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__srl_overflow__64__value;
    __Vfunc_comet2_cpu__DOT__srl_overflow__64__value = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__srl_overflow__64__amount;
    __Vfunc_comet2_cpu__DOT__srl_overflow__64__amount = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__65__result;
    __Vtask_comet2_cpu__DOT__set_fr__65__result = 0;
    CData/*0:0*/ __Vtask_comet2_cpu__DOT__set_fr__65__new_of;
    __Vtask_comet2_cpu__DOT__set_fr__65__new_of = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__68__result;
    __Vtask_comet2_cpu__DOT__set_fr__68__result = 0;
    CData/*0:0*/ __Vtask_comet2_cpu__DOT__set_fr__68__new_of;
    __Vtask_comet2_cpu__DOT__set_fr__68__new_of = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_sub_overflow__69__a;
    __Vfunc_comet2_cpu__DOT__signed_sub_overflow__69__a = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_sub_overflow__69__b;
    __Vfunc_comet2_cpu__DOT__signed_sub_overflow__69__b = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_sub_overflow__69__result;
    __Vfunc_comet2_cpu__DOT__signed_sub_overflow__69__result = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__70__result;
    __Vtask_comet2_cpu__DOT__set_fr__70__result = 0;
    CData/*0:0*/ __Vtask_comet2_cpu__DOT__set_fr__70__new_of;
    __Vtask_comet2_cpu__DOT__set_fr__70__new_of = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__74__result;
    __Vtask_comet2_cpu__DOT__set_fr__74__result = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__75__result;
    __Vtask_comet2_cpu__DOT__set_fr__75__result = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__76__result;
    __Vtask_comet2_cpu__DOT__set_fr__76__result = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__79__result;
    __Vtask_comet2_cpu__DOT__set_fr__79__result = 0;
    CData/*0:0*/ __Vtask_comet2_cpu__DOT__set_fr__79__new_of;
    __Vtask_comet2_cpu__DOT__set_fr__79__new_of = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__80__result;
    __Vtask_comet2_cpu__DOT__set_fr__80__result = 0;
    CData/*0:0*/ __Vtask_comet2_cpu__DOT__set_fr__80__new_of;
    __Vtask_comet2_cpu__DOT__set_fr__80__new_of = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_sub_overflow__81__a;
    __Vfunc_comet2_cpu__DOT__signed_sub_overflow__81__a = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_sub_overflow__81__b;
    __Vfunc_comet2_cpu__DOT__signed_sub_overflow__81__b = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_sub_overflow__81__result;
    __Vfunc_comet2_cpu__DOT__signed_sub_overflow__81__result = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__82__result;
    __Vtask_comet2_cpu__DOT__set_fr__82__result = 0;
    CData/*0:0*/ __Vtask_comet2_cpu__DOT__set_fr__82__new_of;
    __Vtask_comet2_cpu__DOT__set_fr__82__new_of = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_add_overflow__83__a;
    __Vfunc_comet2_cpu__DOT__signed_add_overflow__83__a = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_add_overflow__83__b;
    __Vfunc_comet2_cpu__DOT__signed_add_overflow__83__b = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__signed_add_overflow__83__result;
    __Vfunc_comet2_cpu__DOT__signed_add_overflow__83__result = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__84__result;
    __Vtask_comet2_cpu__DOT__set_fr__84__result = 0;
    CData/*0:0*/ __Vtask_comet2_cpu__DOT__set_fr__84__new_of;
    __Vtask_comet2_cpu__DOT__set_fr__84__new_of = 0;
    SData/*15:0*/ __Vtask_comet2_cpu__DOT__set_fr__88__result;
    __Vtask_comet2_cpu__DOT__set_fr__88__result = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__90__addr;
    __Vfunc_comet2_cpu__DOT__indexed_addr__90__addr = 0;
    CData/*3:0*/ __Vfunc_comet2_cpu__DOT__indexed_addr__90__x;
    __Vfunc_comet2_cpu__DOT__indexed_addr__90__x = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__95__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_addr__95__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__95__word_addr;
    __Vfunc_comet2_cpu__DOT__bus_addr__95__word_addr = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_word__96__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_word__96__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_word__96__data;
    __Vfunc_comet2_cpu__DOT__bus_word__96__data = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__97__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_addr__97__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__97__word_addr;
    __Vfunc_comet2_cpu__DOT__bus_addr__97__word_addr = 0;
    CData/*7:0*/ __Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op;
    __Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__99__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_addr__99__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__99__word_addr;
    __Vfunc_comet2_cpu__DOT__bus_addr__99__word_addr = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_word__100__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_word__100__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_word__100__data;
    __Vfunc_comet2_cpu__DOT__bus_word__100__data = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__101__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_addr__101__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_addr__101__word_addr;
    __Vfunc_comet2_cpu__DOT__bus_addr__101__word_addr = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_word__103__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_word__103__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_word__103__data;
    __Vfunc_comet2_cpu__DOT__bus_word__103__data = 0;
    SData/*15:0*/ __Vfunc_comet2_cpu__DOT__bus_word__106__Vfuncout;
    __Vfunc_comet2_cpu__DOT__bus_word__106__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_comet2_cpu__DOT__bus_word__106__data;
    __Vfunc_comet2_cpu__DOT__bus_word__106__data = 0;
    SData/*15:0*/ __Vdly__comet2_cpu__DOT__pr;
    __Vdly__comet2_cpu__DOT__pr = 0;
    SData/*15:0*/ __Vdly__comet2_cpu__DOT__sp;
    __Vdly__comet2_cpu__DOT__sp = 0;
    CData/*0:0*/ __Vdly__comet2_cpu__DOT__of;
    __Vdly__comet2_cpu__DOT__of = 0;
    CData/*0:0*/ __Vdly__comet2_cpu__DOT__sf;
    __Vdly__comet2_cpu__DOT__sf = 0;
    CData/*0:0*/ __Vdly__comet2_cpu__DOT__zf;
    __Vdly__comet2_cpu__DOT__zf = 0;
    SData/*15:0*/ __Vdly__comet2_cpu__DOT__ir;
    __Vdly__comet2_cpu__DOT__ir = 0;
    SData/*15:0*/ __Vdly__comet2_cpu__DOT__operand;
    __Vdly__comet2_cpu__DOT__operand = 0;
    SData/*15:0*/ __Vdly__comet2_cpu__DOT__effective_addr;
    __Vdly__comet2_cpu__DOT__effective_addr = 0;
    SData/*15:0*/ __Vdly__comet2_cpu__DOT__svc_vector_addr;
    __Vdly__comet2_cpu__DOT__svc_vector_addr = 0;
    SData/*15:0*/ __Vdly__comet2_cpu__DOT__write_word;
    __Vdly__comet2_cpu__DOT__write_word = 0;
    CData/*3:0*/ __Vdly__comet2_cpu__DOT__mem_read_kind;
    __Vdly__comet2_cpu__DOT__mem_read_kind = 0;
    CData/*3:0*/ __Vdly__comet2_cpu__DOT__mem_write_kind;
    __Vdly__comet2_cpu__DOT__mem_write_kind = 0;
    CData/*3:0*/ __Vdly__comet2_cpu__DOT__dest_reg;
    __Vdly__comet2_cpu__DOT__dest_reg = 0;
    CData/*7:0*/ __Vdly__comet2_cpu__DOT__state;
    __Vdly__comet2_cpu__DOT__state = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v0;
    __VdlySet__comet2_cpu__DOT__gr__v0 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v1;
    __VdlySet__comet2_cpu__DOT__gr__v1 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v8;
    __VdlyVal__comet2_cpu__DOT__gr__v8 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v8;
    __VdlyDim0__comet2_cpu__DOT__gr__v8 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v8;
    __VdlySet__comet2_cpu__DOT__gr__v8 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v9;
    __VdlyVal__comet2_cpu__DOT__gr__v9 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v9;
    __VdlyDim0__comet2_cpu__DOT__gr__v9 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v9;
    __VdlySet__comet2_cpu__DOT__gr__v9 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v10;
    __VdlyVal__comet2_cpu__DOT__gr__v10 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v10;
    __VdlyDim0__comet2_cpu__DOT__gr__v10 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v10;
    __VdlySet__comet2_cpu__DOT__gr__v10 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v11;
    __VdlyVal__comet2_cpu__DOT__gr__v11 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v11;
    __VdlyDim0__comet2_cpu__DOT__gr__v11 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v11;
    __VdlySet__comet2_cpu__DOT__gr__v11 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v12;
    __VdlyVal__comet2_cpu__DOT__gr__v12 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v12;
    __VdlyDim0__comet2_cpu__DOT__gr__v12 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v12;
    __VdlySet__comet2_cpu__DOT__gr__v12 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v13;
    __VdlyVal__comet2_cpu__DOT__gr__v13 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v13;
    __VdlyDim0__comet2_cpu__DOT__gr__v13 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v13;
    __VdlySet__comet2_cpu__DOT__gr__v13 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v14;
    __VdlyVal__comet2_cpu__DOT__gr__v14 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v14;
    __VdlyDim0__comet2_cpu__DOT__gr__v14 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v14;
    __VdlySet__comet2_cpu__DOT__gr__v14 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v15;
    __VdlyVal__comet2_cpu__DOT__gr__v15 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v15;
    __VdlyDim0__comet2_cpu__DOT__gr__v15 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v15;
    __VdlySet__comet2_cpu__DOT__gr__v15 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v16;
    __VdlyVal__comet2_cpu__DOT__gr__v16 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v16;
    __VdlyDim0__comet2_cpu__DOT__gr__v16 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v16;
    __VdlySet__comet2_cpu__DOT__gr__v16 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v17;
    __VdlyVal__comet2_cpu__DOT__gr__v17 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v17;
    __VdlyDim0__comet2_cpu__DOT__gr__v17 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v17;
    __VdlySet__comet2_cpu__DOT__gr__v17 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v18;
    __VdlyVal__comet2_cpu__DOT__gr__v18 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v18;
    __VdlyDim0__comet2_cpu__DOT__gr__v18 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v18;
    __VdlySet__comet2_cpu__DOT__gr__v18 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v19;
    __VdlyVal__comet2_cpu__DOT__gr__v19 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v19;
    __VdlyDim0__comet2_cpu__DOT__gr__v19 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v19;
    __VdlySet__comet2_cpu__DOT__gr__v19 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v20;
    __VdlyVal__comet2_cpu__DOT__gr__v20 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v20;
    __VdlyDim0__comet2_cpu__DOT__gr__v20 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v20;
    __VdlySet__comet2_cpu__DOT__gr__v20 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v21;
    __VdlyVal__comet2_cpu__DOT__gr__v21 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v21;
    __VdlyDim0__comet2_cpu__DOT__gr__v21 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v21;
    __VdlySet__comet2_cpu__DOT__gr__v21 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v22;
    __VdlyVal__comet2_cpu__DOT__gr__v22 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v22;
    __VdlyDim0__comet2_cpu__DOT__gr__v22 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v22;
    __VdlySet__comet2_cpu__DOT__gr__v22 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v23;
    __VdlyVal__comet2_cpu__DOT__gr__v23 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v23;
    __VdlyDim0__comet2_cpu__DOT__gr__v23 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v23;
    __VdlySet__comet2_cpu__DOT__gr__v23 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v24;
    __VdlyVal__comet2_cpu__DOT__gr__v24 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v24;
    __VdlyDim0__comet2_cpu__DOT__gr__v24 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v24;
    __VdlySet__comet2_cpu__DOT__gr__v24 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v25;
    __VdlyVal__comet2_cpu__DOT__gr__v25 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v25;
    __VdlyDim0__comet2_cpu__DOT__gr__v25 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v25;
    __VdlySet__comet2_cpu__DOT__gr__v25 = 0;
    SData/*15:0*/ __VdlyVal__comet2_cpu__DOT__gr__v26;
    __VdlyVal__comet2_cpu__DOT__gr__v26 = 0;
    CData/*2:0*/ __VdlyDim0__comet2_cpu__DOT__gr__v26;
    __VdlyDim0__comet2_cpu__DOT__gr__v26 = 0;
    CData/*0:0*/ __VdlySet__comet2_cpu__DOT__gr__v26;
    __VdlySet__comet2_cpu__DOT__gr__v26 = 0;
    // Body
    __Vdly__comet2_cpu__DOT__pr = vlSelfRef.comet2_cpu__DOT__pr;
    __Vdly__comet2_cpu__DOT__sp = vlSelfRef.comet2_cpu__DOT__sp;
    __Vdly__comet2_cpu__DOT__of = vlSelfRef.comet2_cpu__DOT__of;
    __Vdly__comet2_cpu__DOT__sf = vlSelfRef.comet2_cpu__DOT__sf;
    __Vdly__comet2_cpu__DOT__zf = vlSelfRef.comet2_cpu__DOT__zf;
    __Vdly__comet2_cpu__DOT__ir = vlSelfRef.comet2_cpu__DOT__ir;
    __Vdly__comet2_cpu__DOT__operand = vlSelfRef.comet2_cpu__DOT__operand;
    __Vdly__comet2_cpu__DOT__effective_addr = vlSelfRef.comet2_cpu__DOT__effective_addr;
    __Vdly__comet2_cpu__DOT__svc_vector_addr = vlSelfRef.comet2_cpu__DOT__svc_vector_addr;
    __Vdly__comet2_cpu__DOT__write_word = vlSelfRef.comet2_cpu__DOT__write_word;
    __Vdly__comet2_cpu__DOT__mem_read_kind = vlSelfRef.comet2_cpu__DOT__mem_read_kind;
    __Vdly__comet2_cpu__DOT__mem_write_kind = vlSelfRef.comet2_cpu__DOT__mem_write_kind;
    __Vdly__comet2_cpu__DOT__dest_reg = vlSelfRef.comet2_cpu__DOT__dest_reg;
    __Vdly__comet2_cpu__DOT__state = vlSelfRef.comet2_cpu__DOT__state;
    __VdlySet__comet2_cpu__DOT__gr__v0 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v1 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v8 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v9 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v10 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v11 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v12 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v13 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v14 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v15 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v16 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v17 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v18 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v19 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v20 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v21 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v22 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v23 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v24 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v25 = 0U;
    __VdlySet__comet2_cpu__DOT__gr__v26 = 0U;
    if (vlSelfRef.reset) {
        vlSelfRef.read_req = 0U;
        vlSelfRef.read_addr = 0U;
        vlSelfRef.write_req = 0U;
        vlSelfRef.write_addr = 0U;
        vlSelfRef.write_data = 0U;
        vlSelfRef.memory_mask = 3U;
        vlSelfRef.error_instruction = 0U;
        vlSelfRef.debug = 0U;
        __Vdly__comet2_cpu__DOT__pr = 0U;
        __Vdly__comet2_cpu__DOT__sp = 0U;
        __Vdly__comet2_cpu__DOT__of = 0U;
        __Vdly__comet2_cpu__DOT__sf = 0U;
        __Vdly__comet2_cpu__DOT__zf = 0U;
        __Vdly__comet2_cpu__DOT__ir = 0U;
        __Vdly__comet2_cpu__DOT__operand = 0U;
        __Vdly__comet2_cpu__DOT__effective_addr = 0U;
        __Vdly__comet2_cpu__DOT__svc_vector_addr = 0U;
        __Vdly__comet2_cpu__DOT__write_word = 0U;
        __Vdly__comet2_cpu__DOT__mem_read_kind = 0U;
        __Vdly__comet2_cpu__DOT__mem_write_kind = 0U;
        __Vdly__comet2_cpu__DOT__dest_reg = 0U;
        __VdlySet__comet2_cpu__DOT__gr__v0 = 1U;
        __Vdly__comet2_cpu__DOT__state = 0U;
        __VdlySet__comet2_cpu__DOT__gr__v1 = 1U;
    } else {
        vlSelfRef.read_req = 0U;
        vlSelfRef.write_req = 0U;
        vlSelfRef.memory_mask = 3U;
        vlSelfRef.write_data = vlSelfRef.comet2_cpu__DOT__write_word;
        vlSelfRef.debug = (((IData)(vlSelfRef.comet2_cpu__DOT__state) 
                            << 0x00000010U) | (IData)(vlSelfRef.comet2_cpu__DOT__pr));
        vlSelfRef.comet2_cpu__DOT__main__DOT__op = 
            (0x000000ffU & ((IData)(vlSelfRef.comet2_cpu__DOT__ir) 
                            >> 8U));
        vlSelfRef.comet2_cpu__DOT__main__DOT__r = (0x0000000fU 
                                                   & ((IData)(vlSelfRef.comet2_cpu__DOT__ir) 
                                                      >> 4U));
        vlSelfRef.comet2_cpu__DOT__main__DOT__x = (0x0000000fU 
                                                   & (IData)(vlSelfRef.comet2_cpu__DOT__ir));
        vlSelfRef.comet2_cpu__DOT__main__DOT__rhs = 0U;
        vlSelfRef.comet2_cpu__DOT__main__DOT__result = 0U;
        vlSelfRef.comet2_cpu__DOT__main__DOT__wide = 0U;
        vlSelfRef.comet2_cpu__DOT__main__DOT__shifted_out = 0U;
        if ((0x00000080U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
            vlSelfRef.error_instruction = 1U;
            __Vdly__comet2_cpu__DOT__state = 0U;
        } else if ((0x00000040U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
            vlSelfRef.error_instruction = 1U;
            __Vdly__comet2_cpu__DOT__state = 0U;
        } else if ((0x00000020U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
            vlSelfRef.error_instruction = 1U;
            __Vdly__comet2_cpu__DOT__state = 0U;
        } else if ((0x00000010U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
            if ((8U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                vlSelfRef.error_instruction = 1U;
                __Vdly__comet2_cpu__DOT__state = 0U;
            } else if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                vlSelfRef.error_instruction = 1U;
                __Vdly__comet2_cpu__DOT__state = 0U;
            } else if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                vlSelfRef.error_instruction = 1U;
                __Vdly__comet2_cpu__DOT__state = 0U;
            } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                vlSelfRef.error_instruction = 1U;
                __Vdly__comet2_cpu__DOT__state = 0U;
            } else {
                vlSelfRef.read_addr = 0x0000001fU;
                if (vlSelfRef.read_data_valid) {
                    __Vfunc_comet2_cpu__DOT__bus_word__1__data 
                        = vlSelfRef.read_data;
                    __Vfunc_comet2_cpu__DOT__bus_word__1__Vfuncout 
                        = (0x0000ffffU & __Vfunc_comet2_cpu__DOT__bus_word__1__data);
                    __Vdly__comet2_cpu__DOT__sp = __Vfunc_comet2_cpu__DOT__bus_word__1__Vfuncout;
                    __Vdly__comet2_cpu__DOT__state = 4U;
                } else {
                    vlSelfRef.read_req = 1U;
                }
            }
        } else if ((8U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
            if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                    if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                        vlSelfRef.read_addr = 0x0000001fU;
                        vlSelfRef.read_req = 1U;
                        __Vdly__comet2_cpu__DOT__state = 0x10U;
                    } else {
                        __Vfunc_comet2_cpu__DOT__bus_addr__3__word_addr 
                            = vlSelfRef.comet2_cpu__DOT__svc_vector_addr;
                        __Vfunc_comet2_cpu__DOT__bus_addr__3__Vfuncout 
                            = __Vfunc_comet2_cpu__DOT__bus_addr__3__word_addr;
                        vlSelfRef.read_addr = __Vfunc_comet2_cpu__DOT__bus_addr__3__Vfuncout;
                        if (vlSelfRef.read_data_valid) {
                            __Vfunc_comet2_cpu__DOT__bus_word__4__data 
                                = vlSelfRef.read_data;
                            __Vfunc_comet2_cpu__DOT__bus_word__4__Vfuncout 
                                = (0x0000ffffU & __Vfunc_comet2_cpu__DOT__bus_word__4__data);
                            __Vdly__comet2_cpu__DOT__pr 
                                = __Vfunc_comet2_cpu__DOT__bus_word__4__Vfuncout;
                            __Vdly__comet2_cpu__DOT__state = 4U;
                        } else {
                            vlSelfRef.read_req = 1U;
                        }
                    }
                } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                    __Vfunc_comet2_cpu__DOT__bus_addr__5__word_addr 
                        = vlSelfRef.comet2_cpu__DOT__svc_vector_addr;
                    __Vfunc_comet2_cpu__DOT__bus_addr__5__Vfuncout 
                        = __Vfunc_comet2_cpu__DOT__bus_addr__5__word_addr;
                    vlSelfRef.read_addr = __Vfunc_comet2_cpu__DOT__bus_addr__5__Vfuncout;
                    vlSelfRef.read_req = 1U;
                    __Vdly__comet2_cpu__DOT__state = 0x0eU;
                } else {
                    __Vfunc_comet2_cpu__DOT__bus_addr__6__word_addr 
                        = vlSelfRef.comet2_cpu__DOT__effective_addr;
                    __Vfunc_comet2_cpu__DOT__bus_addr__6__Vfuncout 
                        = __Vfunc_comet2_cpu__DOT__bus_addr__6__word_addr;
                    vlSelfRef.write_addr = __Vfunc_comet2_cpu__DOT__bus_addr__6__Vfuncout;
                    vlSelfRef.write_data = vlSelfRef.comet2_cpu__DOT__write_word;
                    vlSelfRef.memory_mask = 3U;
                    vlSelfRef.write_req = 1U;
                    if ((3U == (IData)(vlSelfRef.comet2_cpu__DOT__mem_write_kind))) {
                        __Vfunc_comet2_cpu__DOT__indexed_addr__7__x 
                            = (0x0000000fU & (IData)(vlSelfRef.comet2_cpu__DOT__ir));
                        __Vfunc_comet2_cpu__DOT__indexed_addr__7__addr 
                            = vlSelfRef.comet2_cpu__DOT__operand;
                        __Vfunc_comet2_cpu__DOT__indexed_addr__7__Vfuncout 
                            = (0x0000ffffU & ((0U == (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__7__x))
                                               ? (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__7__addr)
                                               : ((8U 
                                                   > (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__7__x))
                                                   ? 
                                                  ((IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__7__addr) 
                                                   + (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                             [
                                                             (7U 
                                                              & (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__7__x))]))
                                                   : (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__7__addr))));
                        __Vdly__comet2_cpu__DOT__state = 4U;
                        __Vdly__comet2_cpu__DOT__pr 
                            = __Vfunc_comet2_cpu__DOT__indexed_addr__7__Vfuncout;
                    } else {
                        __Vdly__comet2_cpu__DOT__state 
                            = ((4U == (IData)(vlSelfRef.comet2_cpu__DOT__mem_write_kind))
                                ? 0x0dU : 4U);
                    }
                }
            } else if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                    __Vfunc_comet2_cpu__DOT__bus_addr__8__word_addr 
                        = vlSelfRef.comet2_cpu__DOT__effective_addr;
                    __Vfunc_comet2_cpu__DOT__bus_addr__8__Vfuncout 
                        = __Vfunc_comet2_cpu__DOT__bus_addr__8__word_addr;
                    vlSelfRef.read_addr = __Vfunc_comet2_cpu__DOT__bus_addr__8__Vfuncout;
                    if (vlSelfRef.read_data_valid) {
                        __Vfunc_comet2_cpu__DOT__bus_word__9__data 
                            = vlSelfRef.read_data;
                        __Vfunc_comet2_cpu__DOT__bus_word__9__Vfuncout 
                            = (0x0000ffffU & __Vfunc_comet2_cpu__DOT__bus_word__9__data);
                        vlSelfRef.comet2_cpu__DOT__main__DOT__rhs 
                            = __Vfunc_comet2_cpu__DOT__bus_word__9__Vfuncout;
                        if ((8U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                            if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                                if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                                    vlSelfRef.error_instruction = 1U;
                                } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                                    __Vdly__comet2_cpu__DOT__pr 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                    if ((0U == (IData)(vlSelfRef.comet2_cpu__DOT__effective_addr))) {
                                        __Vdly__comet2_cpu__DOT__state = 0x0fU;
                                    }
                                } else {
                                    __Vdly__comet2_cpu__DOT__sp 
                                        = (0x0000ffffU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.comet2_cpu__DOT__sp)));
                                    __Vdly__comet2_cpu__DOT__pr 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                }
                            } else if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                                if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                                    __Vdly__comet2_cpu__DOT__sp 
                                        = (0x0000ffffU 
                                           & ((IData)(1U) 
                                              + (IData)(vlSelfRef.comet2_cpu__DOT__sp)));
                                    __VdlyVal__comet2_cpu__DOT__gr__v8 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                    __VdlyDim0__comet2_cpu__DOT__gr__v8 
                                        = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg));
                                    __VdlySet__comet2_cpu__DOT__gr__v8 = 1U;
                                } else {
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                        = (0x0000ffffU 
                                           & ((IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                      [
                                                      (7U 
                                                       & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))]) 
                                              - (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__rhs)));
                                    __Vtask_comet2_cpu__DOT__set_fr__10__new_of 
                                        = (vlSelfRef.comet2_cpu__DOT__gr
                                           [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))] 
                                           < (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__rhs));
                                    __Vtask_comet2_cpu__DOT__set_fr__10__result 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                    __Vdly__comet2_cpu__DOT__of 
                                        = __Vtask_comet2_cpu__DOT__set_fr__10__new_of;
                                    __Vdly__comet2_cpu__DOT__sf 
                                        = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__10__result) 
                                                 >> 0x0fU));
                                    __Vdly__comet2_cpu__DOT__zf 
                                        = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__10__result));
                                }
                            } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                                vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                    = (0x0000ffffU 
                                       & ((IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                  [
                                                  (7U 
                                                   & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))]) 
                                          - (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__rhs)));
                                __Vfunc_comet2_cpu__DOT__signed_sub_overflow__11__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vfunc_comet2_cpu__DOT__signed_sub_overflow__11__b 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                __Vfunc_comet2_cpu__DOT__signed_sub_overflow__11__a 
                                    = vlSelfRef.comet2_cpu__DOT__gr
                                    [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))];
                                vlSelfRef.comet2_cpu__DOT____VlemCall_9__signed_sub_overflow 
                                    = (((1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__11__a) 
                                               >> 0x0fU)) 
                                        != (1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__11__b) 
                                                  >> 0x0fU))) 
                                       & ((1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__11__result) 
                                                 >> 0x0fU)) 
                                          != (1U & 
                                              ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__11__a) 
                                               >> 0x0fU))));
                                __Vtask_comet2_cpu__DOT__set_fr__12__new_of 
                                    = vlSelfRef.comet2_cpu__DOT____VlemCall_9__signed_sub_overflow;
                                __Vtask_comet2_cpu__DOT__set_fr__12__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vdly__comet2_cpu__DOT__of 
                                    = __Vtask_comet2_cpu__DOT__set_fr__12__new_of;
                                __Vdly__comet2_cpu__DOT__sf 
                                    = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__12__result) 
                                             >> 0x0fU));
                                __Vdly__comet2_cpu__DOT__zf 
                                    = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__12__result));
                            } else {
                                vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                    = (vlSelfRef.comet2_cpu__DOT__gr
                                       [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))] 
                                       ^ (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__rhs));
                                __Vdly__comet2_cpu__DOT__of = 0U;
                                __VdlyVal__comet2_cpu__DOT__gr__v9 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __VdlyDim0__comet2_cpu__DOT__gr__v9 
                                    = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg));
                                __VdlySet__comet2_cpu__DOT__gr__v9 = 1U;
                                __Vtask_comet2_cpu__DOT__set_fr__13__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vdly__comet2_cpu__DOT__sf 
                                    = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__13__result) 
                                             >> 0x0fU));
                                __Vdly__comet2_cpu__DOT__zf 
                                    = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__13__result));
                            }
                        } else if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                            if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                                if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                        = (vlSelfRef.comet2_cpu__DOT__gr
                                           [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))] 
                                           | (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__rhs));
                                    __Vdly__comet2_cpu__DOT__of = 0U;
                                    __VdlyVal__comet2_cpu__DOT__gr__v10 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                    __VdlyDim0__comet2_cpu__DOT__gr__v10 
                                        = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg));
                                    __VdlySet__comet2_cpu__DOT__gr__v10 = 1U;
                                    __Vtask_comet2_cpu__DOT__set_fr__14__result 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                    __Vdly__comet2_cpu__DOT__sf 
                                        = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__14__result) 
                                                 >> 0x0fU));
                                    __Vdly__comet2_cpu__DOT__zf 
                                        = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__14__result));
                                } else {
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                        = (vlSelfRef.comet2_cpu__DOT__gr
                                           [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))] 
                                           & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__rhs));
                                    __Vdly__comet2_cpu__DOT__of = 0U;
                                    __VdlyVal__comet2_cpu__DOT__gr__v11 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                    __VdlyDim0__comet2_cpu__DOT__gr__v11 
                                        = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg));
                                    __VdlySet__comet2_cpu__DOT__gr__v11 = 1U;
                                    __Vtask_comet2_cpu__DOT__set_fr__15__result 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                    __Vdly__comet2_cpu__DOT__sf 
                                        = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__15__result) 
                                                 >> 0x0fU));
                                    __Vdly__comet2_cpu__DOT__zf 
                                        = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__15__result));
                                }
                            } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                                vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                    = (0x0000ffffU 
                                       & ((IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                  [
                                                  (7U 
                                                   & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))]) 
                                          - (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__rhs)));
                                __VdlyVal__comet2_cpu__DOT__gr__v12 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __VdlyDim0__comet2_cpu__DOT__gr__v12 
                                    = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg));
                                __VdlySet__comet2_cpu__DOT__gr__v12 = 1U;
                                __Vtask_comet2_cpu__DOT__set_fr__16__new_of 
                                    = (vlSelfRef.comet2_cpu__DOT__gr
                                       [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))] 
                                       < (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__rhs));
                                __Vtask_comet2_cpu__DOT__set_fr__16__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vdly__comet2_cpu__DOT__of 
                                    = __Vtask_comet2_cpu__DOT__set_fr__16__new_of;
                                __Vdly__comet2_cpu__DOT__sf 
                                    = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__16__result) 
                                             >> 0x0fU));
                                __Vdly__comet2_cpu__DOT__zf 
                                    = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__16__result));
                            } else {
                                vlSelfRef.comet2_cpu__DOT__main__DOT__wide 
                                    = (0x0001ffffU 
                                       & (vlSelfRef.comet2_cpu__DOT__gr
                                          [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))] 
                                          + (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__rhs)));
                                __VdlyVal__comet2_cpu__DOT__gr__v13 
                                    = (0x0000ffffU 
                                       & vlSelfRef.comet2_cpu__DOT__main__DOT__wide);
                                __VdlyDim0__comet2_cpu__DOT__gr__v13 
                                    = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg));
                                __VdlySet__comet2_cpu__DOT__gr__v13 = 1U;
                                __Vtask_comet2_cpu__DOT__set_fr__17__new_of 
                                    = (1U & (vlSelfRef.comet2_cpu__DOT__main__DOT__wide 
                                             >> 0x10U));
                                __Vtask_comet2_cpu__DOT__set_fr__17__result 
                                    = (0x0000ffffU 
                                       & vlSelfRef.comet2_cpu__DOT__main__DOT__wide);
                                __Vdly__comet2_cpu__DOT__of 
                                    = __Vtask_comet2_cpu__DOT__set_fr__17__new_of;
                                __Vdly__comet2_cpu__DOT__sf 
                                    = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__17__result) 
                                             >> 0x0fU));
                                __Vdly__comet2_cpu__DOT__zf 
                                    = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__17__result));
                            }
                        } else if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                            if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                                vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                    = (0x0000ffffU 
                                       & ((IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                  [
                                                  (7U 
                                                   & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))]) 
                                          - (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__rhs)));
                                __VdlyVal__comet2_cpu__DOT__gr__v14 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __VdlyDim0__comet2_cpu__DOT__gr__v14 
                                    = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg));
                                __VdlySet__comet2_cpu__DOT__gr__v14 = 1U;
                                __Vfunc_comet2_cpu__DOT__signed_sub_overflow__18__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vfunc_comet2_cpu__DOT__signed_sub_overflow__18__b 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                __Vfunc_comet2_cpu__DOT__signed_sub_overflow__18__a 
                                    = vlSelfRef.comet2_cpu__DOT__gr
                                    [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))];
                                vlSelfRef.comet2_cpu__DOT____VlemCall_8__signed_sub_overflow 
                                    = (((1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__18__a) 
                                               >> 0x0fU)) 
                                        != (1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__18__b) 
                                                  >> 0x0fU))) 
                                       & ((1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__18__result) 
                                                 >> 0x0fU)) 
                                          != (1U & 
                                              ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__18__a) 
                                               >> 0x0fU))));
                                __Vtask_comet2_cpu__DOT__set_fr__19__new_of 
                                    = vlSelfRef.comet2_cpu__DOT____VlemCall_8__signed_sub_overflow;
                                __Vtask_comet2_cpu__DOT__set_fr__19__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vdly__comet2_cpu__DOT__of 
                                    = __Vtask_comet2_cpu__DOT__set_fr__19__new_of;
                                __Vdly__comet2_cpu__DOT__sf 
                                    = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__19__result) 
                                             >> 0x0fU));
                                __Vdly__comet2_cpu__DOT__zf 
                                    = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__19__result));
                            } else {
                                vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                    = (0x0000ffffU 
                                       & ((IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                  [
                                                  (7U 
                                                   & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))]) 
                                          + (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__rhs)));
                                __VdlyVal__comet2_cpu__DOT__gr__v15 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __VdlyDim0__comet2_cpu__DOT__gr__v15 
                                    = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg));
                                __VdlySet__comet2_cpu__DOT__gr__v15 = 1U;
                                __Vfunc_comet2_cpu__DOT__signed_add_overflow__20__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vfunc_comet2_cpu__DOT__signed_add_overflow__20__b 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                __Vfunc_comet2_cpu__DOT__signed_add_overflow__20__a 
                                    = vlSelfRef.comet2_cpu__DOT__gr
                                    [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg))];
                                vlSelfRef.comet2_cpu__DOT____VlemCall_7__signed_add_overflow 
                                    = (((1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_add_overflow__20__a) 
                                               >> 0x0fU)) 
                                        == (1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_add_overflow__20__b) 
                                                  >> 0x0fU))) 
                                       & ((1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_add_overflow__20__result) 
                                                 >> 0x0fU)) 
                                          != (1U & 
                                              ((IData)(__Vfunc_comet2_cpu__DOT__signed_add_overflow__20__a) 
                                               >> 0x0fU))));
                                __Vtask_comet2_cpu__DOT__set_fr__21__new_of 
                                    = vlSelfRef.comet2_cpu__DOT____VlemCall_7__signed_add_overflow;
                                __Vtask_comet2_cpu__DOT__set_fr__21__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vdly__comet2_cpu__DOT__of 
                                    = __Vtask_comet2_cpu__DOT__set_fr__21__new_of;
                                __Vdly__comet2_cpu__DOT__sf 
                                    = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__21__result) 
                                             >> 0x0fU));
                                __Vdly__comet2_cpu__DOT__zf 
                                    = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__21__result));
                            }
                        } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind))) {
                            __VdlyVal__comet2_cpu__DOT__gr__v16 
                                = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                            __VdlyDim0__comet2_cpu__DOT__gr__v16 
                                = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__dest_reg));
                            __VdlySet__comet2_cpu__DOT__gr__v16 = 1U;
                            __Vdly__comet2_cpu__DOT__of = 0U;
                            __Vtask_comet2_cpu__DOT__set_fr__22__result 
                                = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                            __Vdly__comet2_cpu__DOT__sf 
                                = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__22__result) 
                                         >> 0x0fU));
                            __Vdly__comet2_cpu__DOT__zf 
                                = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__22__result));
                        } else {
                            vlSelfRef.error_instruction = 1U;
                        }
                        if ((1U & (~ ((0x0dU == (IData)(vlSelfRef.comet2_cpu__DOT__mem_read_kind)) 
                                      & (0U == (IData)(vlSelfRef.comet2_cpu__DOT__effective_addr)))))) {
                            __Vdly__comet2_cpu__DOT__state = 4U;
                        }
                    } else {
                        vlSelfRef.read_req = 1U;
                    }
                } else {
                    __Vfunc_comet2_cpu__DOT__bus_addr__23__word_addr 
                        = vlSelfRef.comet2_cpu__DOT__effective_addr;
                    __Vfunc_comet2_cpu__DOT__bus_addr__23__Vfuncout 
                        = __Vfunc_comet2_cpu__DOT__bus_addr__23__word_addr;
                    vlSelfRef.read_addr = __Vfunc_comet2_cpu__DOT__bus_addr__23__Vfuncout;
                    vlSelfRef.read_req = 1U;
                    __Vdly__comet2_cpu__DOT__state = 0x0bU;
                }
            } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                vlSelfRef.comet2_cpu__DOT__main__DOT__op 
                    = (0x000000ffU & ((IData)(vlSelfRef.comet2_cpu__DOT__ir) 
                                      >> 8U));
                vlSelfRef.comet2_cpu__DOT__main__DOT__r 
                    = (0x0000000fU & ((IData)(vlSelfRef.comet2_cpu__DOT__ir) 
                                      >> 4U));
                vlSelfRef.comet2_cpu__DOT__main__DOT__x 
                    = (0x0000000fU & (IData)(vlSelfRef.comet2_cpu__DOT__ir));
                __Vdly__comet2_cpu__DOT__mem_read_kind = 0U;
                __Vdly__comet2_cpu__DOT__mem_write_kind = 0U;
                __Vfunc_comet2_cpu__DOT__uses_r_field__24__op 
                    = vlSelfRef.comet2_cpu__DOT__main__DOT__op;
                vlSelfRef.comet2_cpu__DOT____VlemCall_1__uses_r_field 
                    = ((1U & (~ ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                 >> 7U))) && (1U & 
                                              ((0x00000040U 
                                                & (IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op))
                                                ? (
                                                   (0x00000020U 
                                                    & (IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op))
                                                    ? 
                                                   ((1U 
                                                     & ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                        >> 4U)) 
                                                    && ((1U 
                                                         & (~ 
                                                            ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                             >> 3U))) 
                                                        && ((1U 
                                                             & (~ 
                                                                ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                                 >> 2U))) 
                                                            && ((1U 
                                                                 & (~ 
                                                                    ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                                     >> 1U))) 
                                                                && (1U 
                                                                    & (IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op))))))
                                                    : 
                                                   ((0x00000010U 
                                                     & (IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op))
                                                     ? 
                                                    ((1U 
                                                      & (~ 
                                                         ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                          >> 3U))) 
                                                     && (1U 
                                                         & (~ 
                                                            ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                             >> 2U))))
                                                     : 
                                                    ((1U 
                                                      & (~ 
                                                         ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                          >> 3U))) 
                                                     && (1U 
                                                         & (~ 
                                                            ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                             >> 1U))))))
                                                : (
                                                   (0x00000020U 
                                                    & (IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op))
                                                    ? 
                                                   ((0x00000010U 
                                                     & (IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op))
                                                     ? 
                                                    ((1U 
                                                      & (~ 
                                                         ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                          >> 3U))) 
                                                     && ((1U 
                                                          & (~ 
                                                             ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                              >> 1U))) 
                                                         || (1U 
                                                             & (~ (IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op)))))
                                                     : 
                                                    (~ 
                                                     ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                      >> 3U)))
                                                    : 
                                                   ((1U 
                                                     & ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                        >> 4U)) 
                                                    && ((1U 
                                                         & (~ 
                                                            ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                             >> 3U))) 
                                                        && ((4U 
                                                             & (IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op))
                                                             ? 
                                                            ((1U 
                                                              & (~ 
                                                                 ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                                  >> 1U))) 
                                                             && (1U 
                                                                 & (~ (IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op))))
                                                             : 
                                                            ((1U 
                                                              & (~ 
                                                                 ((IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op) 
                                                                  >> 1U))) 
                                                             || (1U 
                                                                 & (~ (IData)(__Vfunc_comet2_cpu__DOT__uses_r_field__24__op)))))))))));
                __Vfunc_comet2_cpu__DOT__uses_x_field__25__op 
                    = vlSelfRef.comet2_cpu__DOT__main__DOT__op;
                vlSelfRef.comet2_cpu__DOT____VlemCall_2__uses_x_field 
                    = (1U & ((0x00000080U & (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))
                              ? ((0x00000040U & (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))
                                  ? ((1U & ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                            >> 5U)) 
                                     && ((1U & ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                >> 4U)) 
                                         && ((1U & 
                                              (~ ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                  >> 3U))) 
                                             && ((1U 
                                                  & (~ 
                                                     ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                      >> 2U))) 
                                                 && ((1U 
                                                      & (~ 
                                                         ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                          >> 1U))) 
                                                     && (1U 
                                                         & (~ (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))))))))
                                  : ((1U & (~ ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                               >> 5U))) 
                                     && ((1U & (~ ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                   >> 4U))) 
                                         && ((1U & 
                                              (~ ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                  >> 3U))) 
                                             && ((1U 
                                                  & (~ 
                                                     ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                      >> 2U))) 
                                                 && ((1U 
                                                      & (~ 
                                                         ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                          >> 1U))) 
                                                     && (1U 
                                                         & (~ (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op)))))))))
                              : ((0x00000040U & (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))
                                  ? ((0x00000020U & (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))
                                      ? ((0x00000010U 
                                          & (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))
                                          ? ((1U & 
                                              (~ ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                  >> 3U))) 
                                             && ((1U 
                                                  & (~ 
                                                     ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                      >> 2U))) 
                                                 && ((1U 
                                                      & (~ 
                                                         ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                          >> 1U))) 
                                                     && (1U 
                                                         & (~ (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))))))
                                          : ((1U & 
                                              (~ ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                  >> 3U))) 
                                             && ((4U 
                                                  & (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))
                                                  ? 
                                                 ((1U 
                                                   & (~ 
                                                      ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                       >> 1U))) 
                                                  || (1U 
                                                      & (~ (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))))
                                                  : 
                                                 ((1U 
                                                   & ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                      >> 1U)) 
                                                  || (1U 
                                                      & (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))))))
                                      : ((0x00000010U 
                                          & (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))
                                          ? ((1U & 
                                              (~ ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                  >> 3U))) 
                                             && (1U 
                                                 & (~ 
                                                    ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                     >> 2U))))
                                          : ((1U & 
                                              (~ ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                  >> 3U))) 
                                             && (1U 
                                                 & (~ 
                                                    ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                     >> 1U))))))
                                  : ((0x00000020U & (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))
                                      ? ((0x00000010U 
                                          & (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))
                                          ? ((1U & 
                                              (~ ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                  >> 3U))) 
                                             && ((1U 
                                                  & (~ 
                                                     ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                      >> 1U))) 
                                                 || (1U 
                                                     & (~ (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op)))))
                                          : (~ ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                >> 3U)))
                                      : ((1U & ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                >> 4U)) 
                                         && ((1U & 
                                              (~ ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                  >> 3U))) 
                                             && ((4U 
                                                  & (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))
                                                  ? 
                                                 ((1U 
                                                   & (~ 
                                                      ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                       >> 1U))) 
                                                  && (1U 
                                                      & (~ (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op))))
                                                  : 
                                                 ((1U 
                                                   & (~ 
                                                      ((IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op) 
                                                       >> 1U))) 
                                                  || (1U 
                                                      & (~ (IData)(__Vfunc_comet2_cpu__DOT__uses_x_field__25__op)))))))))));
                if ((((IData)(vlSelfRef.comet2_cpu__DOT____VlemCall_1__uses_r_field) 
                      & (7U < (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))) 
                     | ((IData)(vlSelfRef.comet2_cpu__DOT____VlemCall_2__uses_x_field) 
                        & (7U < (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))))) {
                    vlSelfRef.error_instruction = 1U;
                    __Vdly__comet2_cpu__DOT__state = 4U;
                } else {
                    __Vfunc_comet2_cpu__DOT__indexed_addr__27__x 
                        = vlSelfRef.comet2_cpu__DOT__main__DOT__x;
                    __Vfunc_comet2_cpu__DOT__indexed_addr__27__addr 
                        = vlSelfRef.comet2_cpu__DOT__operand;
                    __Vfunc_comet2_cpu__DOT__indexed_addr__27__Vfuncout 
                        = (0x0000ffffU & ((0U == (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__27__x))
                                           ? (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__27__addr)
                                           : ((8U > (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__27__x))
                                               ? ((IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__27__addr) 
                                                  + (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                            [
                                                            (7U 
                                                             & (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__27__x))]))
                                               : (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__27__addr))));
                    __Vdly__comet2_cpu__DOT__effective_addr 
                        = __Vfunc_comet2_cpu__DOT__indexed_addr__27__Vfuncout;
                    __Vdly__comet2_cpu__DOT__dest_reg 
                        = vlSelfRef.comet2_cpu__DOT__main__DOT__r;
                    if ((0x00000080U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                        if ((0x00000040U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            if ((0x00000020U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                if ((0x00000010U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    if ((8U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                        vlSelfRef.error_instruction = 1U;
                                        __Vdly__comet2_cpu__DOT__state = 4U;
                                    } else if ((4U 
                                                & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                        vlSelfRef.error_instruction = 1U;
                                        __Vdly__comet2_cpu__DOT__state = 4U;
                                    } else if ((2U 
                                                & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                        vlSelfRef.error_instruction = 1U;
                                        __Vdly__comet2_cpu__DOT__state = 4U;
                                    } else if ((1U 
                                                & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                        vlSelfRef.error_instruction = 1U;
                                        __Vdly__comet2_cpu__DOT__state = 4U;
                                    } else {
                                        __Vfunc_comet2_cpu__DOT__indexed_addr__32__x 
                                            = vlSelfRef.comet2_cpu__DOT__main__DOT__x;
                                        __Vfunc_comet2_cpu__DOT__indexed_addr__32__addr 
                                            = vlSelfRef.comet2_cpu__DOT__operand;
                                        __Vfunc_comet2_cpu__DOT__indexed_addr__32__Vfuncout 
                                            = (0x0000ffffU 
                                               & ((0U 
                                                   == (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__32__x))
                                                   ? (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__32__addr)
                                                   : 
                                                  ((8U 
                                                    > (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__32__x))
                                                    ? 
                                                   ((IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__32__addr) 
                                                    + (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                              [
                                                              (7U 
                                                               & (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__32__x))]))
                                                    : (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__32__addr))));
                                        vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                            = __Vfunc_comet2_cpu__DOT__indexed_addr__32__Vfuncout;
                                        if ((0U == 
                                             (0x000000ffU 
                                              & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__result)))) {
                                            __Vdly__comet2_cpu__DOT__effective_addr = 0U;
                                            __Vdly__comet2_cpu__DOT__mem_read_kind = 0x0dU;
                                            __Vdly__comet2_cpu__DOT__state = 0x0aU;
                                        } else {
                                            __Vdly__comet2_cpu__DOT__sp 
                                                = (0x0000ffffU 
                                                   & ((IData)(vlSelfRef.comet2_cpu__DOT__sp) 
                                                      - (IData)(1U)));
                                            __Vdly__comet2_cpu__DOT__effective_addr 
                                                = (0x0000ffffU 
                                                   & ((IData)(vlSelfRef.comet2_cpu__DOT__sp) 
                                                      - (IData)(1U)));
                                            __Vdly__comet2_cpu__DOT__svc_vector_addr 
                                                = (0x000000ffU 
                                                   & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__result));
                                            __Vdly__comet2_cpu__DOT__write_word 
                                                = vlSelfRef.comet2_cpu__DOT__pr;
                                            __Vdly__comet2_cpu__DOT__mem_read_kind = 0x0dU;
                                            __Vdly__comet2_cpu__DOT__mem_write_kind = 4U;
                                            __Vdly__comet2_cpu__DOT__state = 0x0cU;
                                        }
                                    }
                                } else {
                                    vlSelfRef.error_instruction = 1U;
                                    __Vdly__comet2_cpu__DOT__state = 4U;
                                }
                            } else {
                                vlSelfRef.error_instruction = 1U;
                                __Vdly__comet2_cpu__DOT__state = 4U;
                            }
                        } else if ((0x00000020U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            vlSelfRef.error_instruction = 1U;
                            __Vdly__comet2_cpu__DOT__state = 4U;
                        } else if ((0x00000010U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            vlSelfRef.error_instruction = 1U;
                            __Vdly__comet2_cpu__DOT__state = 4U;
                        } else if ((8U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            vlSelfRef.error_instruction = 1U;
                            __Vdly__comet2_cpu__DOT__state = 4U;
                        } else if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            vlSelfRef.error_instruction = 1U;
                            __Vdly__comet2_cpu__DOT__state = 4U;
                        } else if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            vlSelfRef.error_instruction = 1U;
                            __Vdly__comet2_cpu__DOT__state = 4U;
                        } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            __Vdly__comet2_cpu__DOT__effective_addr 
                                = vlSelfRef.comet2_cpu__DOT__sp;
                            __Vdly__comet2_cpu__DOT__mem_read_kind = 0x0cU;
                            __Vdly__comet2_cpu__DOT__state = 0x0aU;
                        } else {
                            __Vdly__comet2_cpu__DOT__sp 
                                = (0x0000ffffU & ((IData)(vlSelfRef.comet2_cpu__DOT__sp) 
                                                  - (IData)(1U)));
                            __Vdly__comet2_cpu__DOT__effective_addr 
                                = (0x0000ffffU & ((IData)(vlSelfRef.comet2_cpu__DOT__sp) 
                                                  - (IData)(1U)));
                            __Vdly__comet2_cpu__DOT__write_word 
                                = vlSelfRef.comet2_cpu__DOT__pr;
                            __Vdly__comet2_cpu__DOT__mem_write_kind = 3U;
                            __Vdly__comet2_cpu__DOT__state = 0x0cU;
                        }
                    } else if ((0x00000040U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                        if ((0x00000020U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            if ((0x00000010U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                if ((8U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    vlSelfRef.error_instruction = 1U;
                                    __Vdly__comet2_cpu__DOT__state = 4U;
                                } else if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    vlSelfRef.error_instruction = 1U;
                                    __Vdly__comet2_cpu__DOT__state = 4U;
                                } else if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    vlSelfRef.error_instruction = 1U;
                                    __Vdly__comet2_cpu__DOT__state = 4U;
                                } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    if ((7U < (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))) {
                                        vlSelfRef.error_instruction = 1U;
                                        __Vdly__comet2_cpu__DOT__state = 4U;
                                    } else {
                                        __Vdly__comet2_cpu__DOT__effective_addr 
                                            = vlSelfRef.comet2_cpu__DOT__sp;
                                        __Vdly__comet2_cpu__DOT__dest_reg 
                                            = vlSelfRef.comet2_cpu__DOT__main__DOT__r;
                                        __Vdly__comet2_cpu__DOT__mem_read_kind = 0x0bU;
                                        __Vdly__comet2_cpu__DOT__state = 0x0aU;
                                    }
                                } else {
                                    __Vdly__comet2_cpu__DOT__sp 
                                        = (0x0000ffffU 
                                           & ((IData)(vlSelfRef.comet2_cpu__DOT__sp) 
                                              - (IData)(1U)));
                                    __Vfunc_comet2_cpu__DOT__indexed_addr__44__x 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__x;
                                    __Vfunc_comet2_cpu__DOT__indexed_addr__44__addr 
                                        = vlSelfRef.comet2_cpu__DOT__operand;
                                    __Vdly__comet2_cpu__DOT__effective_addr 
                                        = (0x0000ffffU 
                                           & ((IData)(vlSelfRef.comet2_cpu__DOT__sp) 
                                              - (IData)(1U)));
                                    __Vfunc_comet2_cpu__DOT__indexed_addr__44__Vfuncout 
                                        = (0x0000ffffU 
                                           & ((0U == (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__44__x))
                                               ? (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__44__addr)
                                               : ((8U 
                                                   > (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__44__x))
                                                   ? 
                                                  ((IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__44__addr) 
                                                   + (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                             [
                                                             (7U 
                                                              & (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__44__x))]))
                                                   : (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__44__addr))));
                                    __Vdly__comet2_cpu__DOT__mem_write_kind = 2U;
                                    __Vdly__comet2_cpu__DOT__state = 0x0cU;
                                    __Vdly__comet2_cpu__DOT__write_word 
                                        = __Vfunc_comet2_cpu__DOT__indexed_addr__44__Vfuncout;
                                }
                            } else if ((8U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                vlSelfRef.error_instruction = 1U;
                                __Vdly__comet2_cpu__DOT__state = 4U;
                            } else if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                        vlSelfRef.error_instruction = 1U;
                                        __Vdly__comet2_cpu__DOT__state = 4U;
                                    } else {
                                        if (vlSelfRef.comet2_cpu__DOT__of) {
                                            __Vfunc_comet2_cpu__DOT__indexed_addr__47__x 
                                                = vlSelfRef.comet2_cpu__DOT__main__DOT__x;
                                            __Vfunc_comet2_cpu__DOT__indexed_addr__47__addr 
                                                = vlSelfRef.comet2_cpu__DOT__operand;
                                            __Vfunc_comet2_cpu__DOT__indexed_addr__47__Vfuncout 
                                                = (0x0000ffffU 
                                                   & ((0U 
                                                       == (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__47__x))
                                                       ? (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__47__addr)
                                                       : 
                                                      ((8U 
                                                        > (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__47__x))
                                                        ? 
                                                       ((IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__47__addr) 
                                                        + (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                                  [
                                                                  (7U 
                                                                   & (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__47__x))]))
                                                        : (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__47__addr))));
                                            __Vdly__comet2_cpu__DOT__pr 
                                                = __Vfunc_comet2_cpu__DOT__indexed_addr__47__Vfuncout;
                                        }
                                        __Vdly__comet2_cpu__DOT__state = 4U;
                                    }
                                } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    if ((1U & ((~ (IData)(vlSelfRef.comet2_cpu__DOT__sf)) 
                                               & (~ (IData)(vlSelfRef.comet2_cpu__DOT__zf))))) {
                                        __Vfunc_comet2_cpu__DOT__indexed_addr__48__x 
                                            = vlSelfRef.comet2_cpu__DOT__main__DOT__x;
                                        __Vfunc_comet2_cpu__DOT__indexed_addr__48__addr 
                                            = vlSelfRef.comet2_cpu__DOT__operand;
                                        __Vfunc_comet2_cpu__DOT__indexed_addr__48__Vfuncout 
                                            = (0x0000ffffU 
                                               & ((0U 
                                                   == (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__48__x))
                                                   ? (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__48__addr)
                                                   : 
                                                  ((8U 
                                                    > (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__48__x))
                                                    ? 
                                                   ((IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__48__addr) 
                                                    + (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                              [
                                                              (7U 
                                                               & (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__48__x))]))
                                                    : (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__48__addr))));
                                        __Vdly__comet2_cpu__DOT__pr 
                                            = __Vfunc_comet2_cpu__DOT__indexed_addr__48__Vfuncout;
                                    }
                                    __Vdly__comet2_cpu__DOT__state = 4U;
                                } else {
                                    __Vfunc_comet2_cpu__DOT__indexed_addr__49__x 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__x;
                                    __Vfunc_comet2_cpu__DOT__indexed_addr__49__addr 
                                        = vlSelfRef.comet2_cpu__DOT__operand;
                                    __Vfunc_comet2_cpu__DOT__indexed_addr__49__Vfuncout 
                                        = (0x0000ffffU 
                                           & ((0U == (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__49__x))
                                               ? (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__49__addr)
                                               : ((8U 
                                                   > (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__49__x))
                                                   ? 
                                                  ((IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__49__addr) 
                                                   + (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                             [
                                                             (7U 
                                                              & (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__49__x))]))
                                                   : (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__49__addr))));
                                    __Vdly__comet2_cpu__DOT__state = 4U;
                                    __Vdly__comet2_cpu__DOT__pr 
                                        = __Vfunc_comet2_cpu__DOT__indexed_addr__49__Vfuncout;
                                }
                            } else if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    if (vlSelfRef.comet2_cpu__DOT__zf) {
                                        __Vfunc_comet2_cpu__DOT__indexed_addr__50__x 
                                            = vlSelfRef.comet2_cpu__DOT__main__DOT__x;
                                        __Vfunc_comet2_cpu__DOT__indexed_addr__50__addr 
                                            = vlSelfRef.comet2_cpu__DOT__operand;
                                        __Vfunc_comet2_cpu__DOT__indexed_addr__50__Vfuncout 
                                            = (0x0000ffffU 
                                               & ((0U 
                                                   == (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__50__x))
                                                   ? (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__50__addr)
                                                   : 
                                                  ((8U 
                                                    > (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__50__x))
                                                    ? 
                                                   ((IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__50__addr) 
                                                    + (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                              [
                                                              (7U 
                                                               & (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__50__x))]))
                                                    : (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__50__addr))));
                                        __Vdly__comet2_cpu__DOT__pr 
                                            = __Vfunc_comet2_cpu__DOT__indexed_addr__50__Vfuncout;
                                    }
                                    __Vdly__comet2_cpu__DOT__state = 4U;
                                } else {
                                    if ((1U & (~ (IData)(vlSelfRef.comet2_cpu__DOT__zf)))) {
                                        __Vfunc_comet2_cpu__DOT__indexed_addr__51__x 
                                            = vlSelfRef.comet2_cpu__DOT__main__DOT__x;
                                        __Vfunc_comet2_cpu__DOT__indexed_addr__51__addr 
                                            = vlSelfRef.comet2_cpu__DOT__operand;
                                        __Vfunc_comet2_cpu__DOT__indexed_addr__51__Vfuncout 
                                            = (0x0000ffffU 
                                               & ((0U 
                                                   == (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__51__x))
                                                   ? (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__51__addr)
                                                   : 
                                                  ((8U 
                                                    > (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__51__x))
                                                    ? 
                                                   ((IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__51__addr) 
                                                    + (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                              [
                                                              (7U 
                                                               & (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__51__x))]))
                                                    : (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__51__addr))));
                                        __Vdly__comet2_cpu__DOT__pr 
                                            = __Vfunc_comet2_cpu__DOT__indexed_addr__51__Vfuncout;
                                    }
                                    __Vdly__comet2_cpu__DOT__state = 4U;
                                }
                            } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                if (vlSelfRef.comet2_cpu__DOT__sf) {
                                    __Vfunc_comet2_cpu__DOT__indexed_addr__52__x 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__x;
                                    __Vfunc_comet2_cpu__DOT__indexed_addr__52__addr 
                                        = vlSelfRef.comet2_cpu__DOT__operand;
                                    __Vfunc_comet2_cpu__DOT__indexed_addr__52__Vfuncout 
                                        = (0x0000ffffU 
                                           & ((0U == (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__52__x))
                                               ? (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__52__addr)
                                               : ((8U 
                                                   > (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__52__x))
                                                   ? 
                                                  ((IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__52__addr) 
                                                   + (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                             [
                                                             (7U 
                                                              & (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__52__x))]))
                                                   : (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__52__addr))));
                                    __Vdly__comet2_cpu__DOT__pr 
                                        = __Vfunc_comet2_cpu__DOT__indexed_addr__52__Vfuncout;
                                }
                                __Vdly__comet2_cpu__DOT__state = 4U;
                            } else {
                                vlSelfRef.error_instruction = 1U;
                                __Vdly__comet2_cpu__DOT__state = 4U;
                            }
                        } else if ((0x00000010U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            if ((8U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                vlSelfRef.error_instruction = 1U;
                                __Vdly__comet2_cpu__DOT__state = 4U;
                            } else if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                vlSelfRef.error_instruction = 1U;
                                __Vdly__comet2_cpu__DOT__state = 4U;
                            } else {
                                __Vfunc_comet2_cpu__DOT__indexed_addr__56__x 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__x;
                                __Vfunc_comet2_cpu__DOT__indexed_addr__56__addr 
                                    = vlSelfRef.comet2_cpu__DOT__operand;
                                __Vfunc_comet2_cpu__DOT__indexed_addr__56__Vfuncout 
                                    = (0x0000ffffU 
                                       & ((0U == (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__56__x))
                                           ? (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__56__addr)
                                           : ((8U > (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__56__x))
                                               ? ((IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__56__addr) 
                                                  + (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                            [
                                                            (7U 
                                                             & (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__56__x))]))
                                               : (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__56__addr))));
                                __Vdly__comet2_cpu__DOT__state = 4U;
                                vlSelfRef.comet2_cpu__DOT__main__DOT__rhs 
                                    = __Vfunc_comet2_cpu__DOT__indexed_addr__56__Vfuncout;
                                if ((0x50U == (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    __Vfunc_comet2_cpu__DOT__sla_result__57__amount 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                    __Vfunc_comet2_cpu__DOT__sla_result__57__value 
                                        = vlSelfRef.comet2_cpu__DOT__gr
                                        [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))];
                                    __Vfunc_comet2_cpu__DOT__sla_result__57__Vfuncout 
                                        = ((0U == (IData)(__Vfunc_comet2_cpu__DOT__sla_result__57__amount))
                                            ? (IData)(__Vfunc_comet2_cpu__DOT__sla_result__57__value)
                                            : ((0x000fU 
                                                <= (IData)(__Vfunc_comet2_cpu__DOT__sla_result__57__amount))
                                                ? (0x00008000U 
                                                   & (IData)(__Vfunc_comet2_cpu__DOT__sla_result__57__value))
                                                : (
                                                   (0x00008000U 
                                                    & (IData)(__Vfunc_comet2_cpu__DOT__sla_result__57__value)) 
                                                   | (0x00007fffU 
                                                      & ((IData)(__Vfunc_comet2_cpu__DOT__sla_result__57__value) 
                                                         << 
                                                         (0x0000000fU 
                                                          & (IData)(__Vfunc_comet2_cpu__DOT__sla_result__57__amount)))))));
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                        = __Vfunc_comet2_cpu__DOT__sla_result__57__Vfuncout;
                                    __Vfunc_comet2_cpu__DOT__sla_overflow__58__amount 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                    __Vfunc_comet2_cpu__DOT__sla_overflow__58__value 
                                        = vlSelfRef.comet2_cpu__DOT__gr
                                        [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))];
                                    __Vfunc_comet2_cpu__DOT__sla_overflow__58__Vfuncout 
                                        = ((0U != (IData)(__Vfunc_comet2_cpu__DOT__sla_overflow__58__amount)) 
                                           && (1U & 
                                               ((0x000fU 
                                                 <= (IData)(__Vfunc_comet2_cpu__DOT__sla_overflow__58__amount))
                                                 ? (IData)(__Vfunc_comet2_cpu__DOT__sla_overflow__58__value)
                                                 : 
                                                ((IData)(__Vfunc_comet2_cpu__DOT__sla_overflow__58__value) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & ((IData)(0x0fU) 
                                                     - (IData)(__Vfunc_comet2_cpu__DOT__sla_overflow__58__amount)))))));
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__shifted_out 
                                        = __Vfunc_comet2_cpu__DOT__sla_overflow__58__Vfuncout;
                                } else if ((0x51U == (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    __Vfunc_comet2_cpu__DOT__sra_result__59__amount 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                    __Vfunc_comet2_cpu__DOT__sra_result__59__value 
                                        = vlSelfRef.comet2_cpu__DOT__gr
                                        [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))];
                                    __Vfunc_comet2_cpu__DOT__sra_result__59__Vfuncout 
                                        = (0x0000ffffU 
                                           & ((0U == (IData)(__Vfunc_comet2_cpu__DOT__sra_result__59__amount))
                                               ? (IData)(__Vfunc_comet2_cpu__DOT__sra_result__59__value)
                                               : ((0x000fU 
                                                   <= (IData)(__Vfunc_comet2_cpu__DOT__sra_result__59__amount))
                                                   ? 
                                                  (- (IData)(
                                                             (1U 
                                                              & ((IData)(__Vfunc_comet2_cpu__DOT__sra_result__59__value) 
                                                                 >> 0x0fU))))
                                                   : 
                                                  VL_SHIFTRS_III(16,16,4, (IData)(__Vfunc_comet2_cpu__DOT__sra_result__59__value), 
                                                                 (0x0000000fU 
                                                                  & (IData)(__Vfunc_comet2_cpu__DOT__sra_result__59__amount))))));
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                        = __Vfunc_comet2_cpu__DOT__sra_result__59__Vfuncout;
                                    __Vfunc_comet2_cpu__DOT__sra_overflow__60__amount 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                    __Vfunc_comet2_cpu__DOT__sra_overflow__60__value 
                                        = vlSelfRef.comet2_cpu__DOT__gr
                                        [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))];
                                    __Vfunc_comet2_cpu__DOT__sra_overflow__60__Vfuncout 
                                        = ((0U != (IData)(__Vfunc_comet2_cpu__DOT__sra_overflow__60__amount)) 
                                           && (1U & 
                                               ((0x0010U 
                                                 <= (IData)(__Vfunc_comet2_cpu__DOT__sra_overflow__60__amount))
                                                 ? 
                                                ((IData)(__Vfunc_comet2_cpu__DOT__sra_overflow__60__value) 
                                                 >> 0x0fU)
                                                 : 
                                                ((IData)(__Vfunc_comet2_cpu__DOT__sra_overflow__60__value) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & ((IData)(__Vfunc_comet2_cpu__DOT__sra_overflow__60__amount) 
                                                     - (IData)(1U)))))));
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__shifted_out 
                                        = __Vfunc_comet2_cpu__DOT__sra_overflow__60__Vfuncout;
                                } else if ((0x52U == (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    __Vfunc_comet2_cpu__DOT__sll_result__61__amount 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                    __Vfunc_comet2_cpu__DOT__sll_result__61__value 
                                        = vlSelfRef.comet2_cpu__DOT__gr
                                        [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))];
                                    __Vfunc_comet2_cpu__DOT__sll_result__61__Vfuncout 
                                        = ((0x0010U 
                                            <= (IData)(__Vfunc_comet2_cpu__DOT__sll_result__61__amount))
                                            ? 0U : 
                                           (0x0000ffffU 
                                            & ((IData)(__Vfunc_comet2_cpu__DOT__sll_result__61__value) 
                                               << (0x0000000fU 
                                                   & (IData)(__Vfunc_comet2_cpu__DOT__sll_result__61__amount)))));
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                        = __Vfunc_comet2_cpu__DOT__sll_result__61__Vfuncout;
                                    __Vfunc_comet2_cpu__DOT__sll_overflow__62__amount 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                    __Vfunc_comet2_cpu__DOT__sll_overflow__62__value 
                                        = vlSelfRef.comet2_cpu__DOT__gr
                                        [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))];
                                    __Vfunc_comet2_cpu__DOT__sll_overflow__62__Vfuncout 
                                        = ((0U != (IData)(__Vfunc_comet2_cpu__DOT__sll_overflow__62__amount)) 
                                           && (1U & 
                                               ((0x0010U 
                                                 <= (IData)(__Vfunc_comet2_cpu__DOT__sll_overflow__62__amount))
                                                 ? (IData)(__Vfunc_comet2_cpu__DOT__sll_overflow__62__value)
                                                 : 
                                                ((IData)(__Vfunc_comet2_cpu__DOT__sll_overflow__62__value) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & (- (IData)(__Vfunc_comet2_cpu__DOT__sll_overflow__62__amount)))))));
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__shifted_out 
                                        = __Vfunc_comet2_cpu__DOT__sll_overflow__62__Vfuncout;
                                } else {
                                    __Vfunc_comet2_cpu__DOT__srl_result__63__amount 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                    __Vfunc_comet2_cpu__DOT__srl_result__63__value 
                                        = vlSelfRef.comet2_cpu__DOT__gr
                                        [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))];
                                    __Vfunc_comet2_cpu__DOT__srl_result__63__Vfuncout 
                                        = ((0x0010U 
                                            <= (IData)(__Vfunc_comet2_cpu__DOT__srl_result__63__amount))
                                            ? 0U : 
                                           ((IData)(__Vfunc_comet2_cpu__DOT__srl_result__63__value) 
                                            >> (0x0000000fU 
                                                & (IData)(__Vfunc_comet2_cpu__DOT__srl_result__63__amount))));
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                        = __Vfunc_comet2_cpu__DOT__srl_result__63__Vfuncout;
                                    __Vfunc_comet2_cpu__DOT__srl_overflow__64__amount 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__rhs;
                                    __Vfunc_comet2_cpu__DOT__srl_overflow__64__value 
                                        = vlSelfRef.comet2_cpu__DOT__gr
                                        [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))];
                                    __Vfunc_comet2_cpu__DOT__srl_overflow__64__Vfuncout 
                                        = ((0U != (IData)(__Vfunc_comet2_cpu__DOT__srl_overflow__64__amount)) 
                                           && (1U & 
                                               ((0x0010U 
                                                 <= (IData)(__Vfunc_comet2_cpu__DOT__srl_overflow__64__amount))
                                                 ? 
                                                ((IData)(__Vfunc_comet2_cpu__DOT__srl_overflow__64__value) 
                                                 >> 0x0fU)
                                                 : 
                                                ((IData)(__Vfunc_comet2_cpu__DOT__srl_overflow__64__value) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & ((IData)(__Vfunc_comet2_cpu__DOT__srl_overflow__64__amount) 
                                                     - (IData)(1U)))))));
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__shifted_out 
                                        = __Vfunc_comet2_cpu__DOT__srl_overflow__64__Vfuncout;
                                }
                                __VdlyVal__comet2_cpu__DOT__gr__v17 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __VdlyDim0__comet2_cpu__DOT__gr__v17 
                                    = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r));
                                __VdlySet__comet2_cpu__DOT__gr__v17 = 1U;
                                __Vtask_comet2_cpu__DOT__set_fr__65__new_of 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__shifted_out;
                                __Vtask_comet2_cpu__DOT__set_fr__65__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vdly__comet2_cpu__DOT__of 
                                    = __Vtask_comet2_cpu__DOT__set_fr__65__new_of;
                                __Vdly__comet2_cpu__DOT__sf 
                                    = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__65__result) 
                                             >> 0x0fU));
                                __Vdly__comet2_cpu__DOT__zf 
                                    = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__65__result));
                            }
                        } else if ((8U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            vlSelfRef.error_instruction = 1U;
                            __Vdly__comet2_cpu__DOT__state = 4U;
                        } else if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                vlSelfRef.error_instruction = 1U;
                                __Vdly__comet2_cpu__DOT__state = 4U;
                            } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                    = (0x0000ffffU 
                                       & ((IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                  [
                                                  (7U 
                                                   & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))]) 
                                          - (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                    [
                                                    (7U 
                                                     & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))])));
                                __Vtask_comet2_cpu__DOT__set_fr__68__new_of 
                                    = (vlSelfRef.comet2_cpu__DOT__gr
                                       [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))] 
                                       < vlSelfRef.comet2_cpu__DOT__gr
                                       [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))]);
                                __Vtask_comet2_cpu__DOT__set_fr__68__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vdly__comet2_cpu__DOT__state = 4U;
                                __Vdly__comet2_cpu__DOT__of 
                                    = __Vtask_comet2_cpu__DOT__set_fr__68__new_of;
                                __Vdly__comet2_cpu__DOT__sf 
                                    = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__68__result) 
                                             >> 0x0fU));
                                __Vdly__comet2_cpu__DOT__zf 
                                    = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__68__result));
                            } else {
                                vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                    = (0x0000ffffU 
                                       & ((IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                  [
                                                  (7U 
                                                   & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))]) 
                                          - (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                    [
                                                    (7U 
                                                     & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))])));
                                __Vfunc_comet2_cpu__DOT__signed_sub_overflow__69__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vfunc_comet2_cpu__DOT__signed_sub_overflow__69__b 
                                    = vlSelfRef.comet2_cpu__DOT__gr
                                    [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))];
                                __Vdly__comet2_cpu__DOT__state = 4U;
                                __Vfunc_comet2_cpu__DOT__signed_sub_overflow__69__a 
                                    = vlSelfRef.comet2_cpu__DOT__gr
                                    [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))];
                                vlSelfRef.comet2_cpu__DOT____VlemCall_6__signed_sub_overflow 
                                    = (((1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__69__a) 
                                               >> 0x0fU)) 
                                        != (1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__69__b) 
                                                  >> 0x0fU))) 
                                       & ((1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__69__result) 
                                                 >> 0x0fU)) 
                                          != (1U & 
                                              ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__69__a) 
                                               >> 0x0fU))));
                                __Vtask_comet2_cpu__DOT__set_fr__70__new_of 
                                    = vlSelfRef.comet2_cpu__DOT____VlemCall_6__signed_sub_overflow;
                                __Vtask_comet2_cpu__DOT__set_fr__70__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vdly__comet2_cpu__DOT__of 
                                    = __Vtask_comet2_cpu__DOT__set_fr__70__new_of;
                                __Vdly__comet2_cpu__DOT__sf 
                                    = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__70__result) 
                                             >> 0x0fU));
                                __Vdly__comet2_cpu__DOT__zf 
                                    = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__70__result));
                            }
                        } else if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            vlSelfRef.error_instruction = 1U;
                            __Vdly__comet2_cpu__DOT__state = 4U;
                        } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            __Vdly__comet2_cpu__DOT__mem_read_kind = 0x0aU;
                            __Vdly__comet2_cpu__DOT__state = 0x0aU;
                        } else {
                            __Vdly__comet2_cpu__DOT__mem_read_kind = 9U;
                            __Vdly__comet2_cpu__DOT__state = 0x0aU;
                        }
                    } else if ((0x00000020U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                        if ((0x00000010U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            if ((8U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                vlSelfRef.error_instruction = 1U;
                                __Vdly__comet2_cpu__DOT__state = 4U;
                            } else if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                        vlSelfRef.error_instruction = 1U;
                                        __Vdly__comet2_cpu__DOT__state = 4U;
                                    } else {
                                        vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                            = (vlSelfRef.comet2_cpu__DOT__gr
                                               [(7U 
                                                 & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))] 
                                               ^ vlSelfRef.comet2_cpu__DOT__gr
                                               [(7U 
                                                 & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))]);
                                        __Vdly__comet2_cpu__DOT__of = 0U;
                                        __Vdly__comet2_cpu__DOT__state = 4U;
                                        __VdlyVal__comet2_cpu__DOT__gr__v18 
                                            = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                        __VdlyDim0__comet2_cpu__DOT__gr__v18 
                                            = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r));
                                        __VdlySet__comet2_cpu__DOT__gr__v18 = 1U;
                                        __Vtask_comet2_cpu__DOT__set_fr__74__result 
                                            = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                        __Vdly__comet2_cpu__DOT__sf 
                                            = (1U & 
                                               ((IData)(__Vtask_comet2_cpu__DOT__set_fr__74__result) 
                                                >> 0x0fU));
                                        __Vdly__comet2_cpu__DOT__zf 
                                            = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__74__result));
                                    }
                                } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                        = (vlSelfRef.comet2_cpu__DOT__gr
                                           [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))] 
                                           | vlSelfRef.comet2_cpu__DOT__gr
                                           [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))]);
                                    __Vdly__comet2_cpu__DOT__of = 0U;
                                    __Vdly__comet2_cpu__DOT__state = 4U;
                                    __VdlyVal__comet2_cpu__DOT__gr__v19 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                    __VdlyDim0__comet2_cpu__DOT__gr__v19 
                                        = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r));
                                    __VdlySet__comet2_cpu__DOT__gr__v19 = 1U;
                                    __Vtask_comet2_cpu__DOT__set_fr__75__result 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                    __Vdly__comet2_cpu__DOT__sf 
                                        = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__75__result) 
                                                 >> 0x0fU));
                                    __Vdly__comet2_cpu__DOT__zf 
                                        = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__75__result));
                                } else {
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                        = (vlSelfRef.comet2_cpu__DOT__gr
                                           [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))] 
                                           & vlSelfRef.comet2_cpu__DOT__gr
                                           [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))]);
                                    __Vdly__comet2_cpu__DOT__of = 0U;
                                    __Vdly__comet2_cpu__DOT__state = 4U;
                                    __VdlyVal__comet2_cpu__DOT__gr__v20 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                    __VdlyDim0__comet2_cpu__DOT__gr__v20 
                                        = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r));
                                    __VdlySet__comet2_cpu__DOT__gr__v20 = 1U;
                                    __Vtask_comet2_cpu__DOT__set_fr__76__result 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                    __Vdly__comet2_cpu__DOT__sf 
                                        = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__76__result) 
                                                 >> 0x0fU));
                                    __Vdly__comet2_cpu__DOT__zf 
                                        = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__76__result));
                                }
                            } else if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    vlSelfRef.error_instruction = 1U;
                                    __Vdly__comet2_cpu__DOT__state = 4U;
                                } else {
                                    __Vdly__comet2_cpu__DOT__mem_read_kind = 8U;
                                    __Vdly__comet2_cpu__DOT__state = 0x0aU;
                                }
                            } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                __Vdly__comet2_cpu__DOT__mem_read_kind = 7U;
                                __Vdly__comet2_cpu__DOT__state = 0x0aU;
                            } else {
                                __Vdly__comet2_cpu__DOT__mem_read_kind = 6U;
                                __Vdly__comet2_cpu__DOT__state = 0x0aU;
                            }
                        } else if ((8U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            vlSelfRef.error_instruction = 1U;
                            __Vdly__comet2_cpu__DOT__state = 4U;
                        } else if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                        = (0x0000ffffU 
                                           & ((IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                      [
                                                      (7U 
                                                       & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))]) 
                                              - (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                        [
                                                        (7U 
                                                         & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))])));
                                    __Vdly__comet2_cpu__DOT__state = 4U;
                                    __VdlyVal__comet2_cpu__DOT__gr__v21 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                    __VdlyDim0__comet2_cpu__DOT__gr__v21 
                                        = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r));
                                    __VdlySet__comet2_cpu__DOT__gr__v21 = 1U;
                                    __Vtask_comet2_cpu__DOT__set_fr__79__new_of 
                                        = (vlSelfRef.comet2_cpu__DOT__gr
                                           [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))] 
                                           < vlSelfRef.comet2_cpu__DOT__gr
                                           [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))]);
                                    __Vtask_comet2_cpu__DOT__set_fr__79__result 
                                        = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                    __Vdly__comet2_cpu__DOT__of 
                                        = __Vtask_comet2_cpu__DOT__set_fr__79__new_of;
                                    __Vdly__comet2_cpu__DOT__sf 
                                        = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__79__result) 
                                                 >> 0x0fU));
                                    __Vdly__comet2_cpu__DOT__zf 
                                        = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__79__result));
                                } else {
                                    vlSelfRef.comet2_cpu__DOT__main__DOT__wide 
                                        = (0x0001ffffU 
                                           & (vlSelfRef.comet2_cpu__DOT__gr
                                              [(7U 
                                                & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))] 
                                              + vlSelfRef.comet2_cpu__DOT__gr
                                              [(7U 
                                                & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))]));
                                    __Vdly__comet2_cpu__DOT__state = 4U;
                                    __VdlyVal__comet2_cpu__DOT__gr__v22 
                                        = (0x0000ffffU 
                                           & vlSelfRef.comet2_cpu__DOT__main__DOT__wide);
                                    __VdlyDim0__comet2_cpu__DOT__gr__v22 
                                        = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r));
                                    __VdlySet__comet2_cpu__DOT__gr__v22 = 1U;
                                    __Vtask_comet2_cpu__DOT__set_fr__80__new_of 
                                        = (1U & (vlSelfRef.comet2_cpu__DOT__main__DOT__wide 
                                                 >> 0x10U));
                                    __Vtask_comet2_cpu__DOT__set_fr__80__result 
                                        = (0x0000ffffU 
                                           & vlSelfRef.comet2_cpu__DOT__main__DOT__wide);
                                    __Vdly__comet2_cpu__DOT__of 
                                        = __Vtask_comet2_cpu__DOT__set_fr__80__new_of;
                                    __Vdly__comet2_cpu__DOT__sf 
                                        = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__80__result) 
                                                 >> 0x0fU));
                                    __Vdly__comet2_cpu__DOT__zf 
                                        = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__80__result));
                                }
                            } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                    = (0x0000ffffU 
                                       & ((IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                  [
                                                  (7U 
                                                   & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))]) 
                                          - (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                    [
                                                    (7U 
                                                     & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))])));
                                __Vdly__comet2_cpu__DOT__state = 4U;
                                __VdlyVal__comet2_cpu__DOT__gr__v23 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __VdlyDim0__comet2_cpu__DOT__gr__v23 
                                    = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r));
                                __VdlySet__comet2_cpu__DOT__gr__v23 = 1U;
                                __Vfunc_comet2_cpu__DOT__signed_sub_overflow__81__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vfunc_comet2_cpu__DOT__signed_sub_overflow__81__b 
                                    = vlSelfRef.comet2_cpu__DOT__gr
                                    [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))];
                                __Vfunc_comet2_cpu__DOT__signed_sub_overflow__81__a 
                                    = vlSelfRef.comet2_cpu__DOT__gr
                                    [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))];
                                vlSelfRef.comet2_cpu__DOT____VlemCall_5__signed_sub_overflow 
                                    = (((1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__81__a) 
                                               >> 0x0fU)) 
                                        != (1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__81__b) 
                                                  >> 0x0fU))) 
                                       & ((1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__81__result) 
                                                 >> 0x0fU)) 
                                          != (1U & 
                                              ((IData)(__Vfunc_comet2_cpu__DOT__signed_sub_overflow__81__a) 
                                               >> 0x0fU))));
                                __Vtask_comet2_cpu__DOT__set_fr__82__new_of 
                                    = vlSelfRef.comet2_cpu__DOT____VlemCall_5__signed_sub_overflow;
                                __Vtask_comet2_cpu__DOT__set_fr__82__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vdly__comet2_cpu__DOT__of 
                                    = __Vtask_comet2_cpu__DOT__set_fr__82__new_of;
                                __Vdly__comet2_cpu__DOT__sf 
                                    = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__82__result) 
                                             >> 0x0fU));
                                __Vdly__comet2_cpu__DOT__zf 
                                    = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__82__result));
                            } else {
                                vlSelfRef.comet2_cpu__DOT__main__DOT__result 
                                    = (0x0000ffffU 
                                       & ((IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                  [
                                                  (7U 
                                                   & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))]) 
                                          + (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                    [
                                                    (7U 
                                                     & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))])));
                                __Vdly__comet2_cpu__DOT__state = 4U;
                                __VdlyVal__comet2_cpu__DOT__gr__v24 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __VdlyDim0__comet2_cpu__DOT__gr__v24 
                                    = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r));
                                __VdlySet__comet2_cpu__DOT__gr__v24 = 1U;
                                __Vfunc_comet2_cpu__DOT__signed_add_overflow__83__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vfunc_comet2_cpu__DOT__signed_add_overflow__83__b 
                                    = vlSelfRef.comet2_cpu__DOT__gr
                                    [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))];
                                __Vfunc_comet2_cpu__DOT__signed_add_overflow__83__a 
                                    = vlSelfRef.comet2_cpu__DOT__gr
                                    [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))];
                                vlSelfRef.comet2_cpu__DOT____VlemCall_4__signed_add_overflow 
                                    = (((1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_add_overflow__83__a) 
                                               >> 0x0fU)) 
                                        == (1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_add_overflow__83__b) 
                                                  >> 0x0fU))) 
                                       & ((1U & ((IData)(__Vfunc_comet2_cpu__DOT__signed_add_overflow__83__result) 
                                                 >> 0x0fU)) 
                                          != (1U & 
                                              ((IData)(__Vfunc_comet2_cpu__DOT__signed_add_overflow__83__a) 
                                               >> 0x0fU))));
                                __Vtask_comet2_cpu__DOT__set_fr__84__new_of 
                                    = vlSelfRef.comet2_cpu__DOT____VlemCall_4__signed_add_overflow;
                                __Vtask_comet2_cpu__DOT__set_fr__84__result 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__result;
                                __Vdly__comet2_cpu__DOT__of 
                                    = __Vtask_comet2_cpu__DOT__set_fr__84__new_of;
                                __Vdly__comet2_cpu__DOT__sf 
                                    = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__84__result) 
                                             >> 0x0fU));
                                __Vdly__comet2_cpu__DOT__zf 
                                    = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__84__result));
                            }
                        } else if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                __Vdly__comet2_cpu__DOT__mem_read_kind = 5U;
                                __Vdly__comet2_cpu__DOT__state = 0x0aU;
                            } else {
                                __Vdly__comet2_cpu__DOT__mem_read_kind = 4U;
                                __Vdly__comet2_cpu__DOT__state = 0x0aU;
                            }
                        } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            __Vdly__comet2_cpu__DOT__mem_read_kind = 3U;
                            __Vdly__comet2_cpu__DOT__state = 0x0aU;
                        } else {
                            __Vdly__comet2_cpu__DOT__mem_read_kind = 2U;
                            __Vdly__comet2_cpu__DOT__state = 0x0aU;
                        }
                    } else if ((0x00000010U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                        if ((8U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            vlSelfRef.error_instruction = 1U;
                            __Vdly__comet2_cpu__DOT__state = 4U;
                        } else if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                vlSelfRef.error_instruction = 1U;
                                __Vdly__comet2_cpu__DOT__state = 4U;
                            } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                vlSelfRef.error_instruction = 1U;
                                __Vdly__comet2_cpu__DOT__state = 4U;
                            } else {
                                __VdlyVal__comet2_cpu__DOT__gr__v25 
                                    = vlSelfRef.comet2_cpu__DOT__gr
                                    [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))];
                                __VdlyDim0__comet2_cpu__DOT__gr__v25 
                                    = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r));
                                __VdlySet__comet2_cpu__DOT__gr__v25 = 1U;
                                __Vtask_comet2_cpu__DOT__set_fr__88__result 
                                    = vlSelfRef.comet2_cpu__DOT__gr
                                    [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__x))];
                                __Vdly__comet2_cpu__DOT__of = 0U;
                                __Vdly__comet2_cpu__DOT__sf 
                                    = (1U & ((IData)(__Vtask_comet2_cpu__DOT__set_fr__88__result) 
                                             >> 0x0fU));
                                __Vdly__comet2_cpu__DOT__state = 4U;
                                __Vdly__comet2_cpu__DOT__zf 
                                    = (0U == (IData)(__Vtask_comet2_cpu__DOT__set_fr__88__result));
                            }
                        } else if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                                vlSelfRef.error_instruction = 1U;
                                __Vdly__comet2_cpu__DOT__state = 4U;
                            } else {
                                __Vfunc_comet2_cpu__DOT__indexed_addr__90__x 
                                    = vlSelfRef.comet2_cpu__DOT__main__DOT__x;
                                __Vfunc_comet2_cpu__DOT__indexed_addr__90__addr 
                                    = vlSelfRef.comet2_cpu__DOT__operand;
                                vlSelfRef.comet2_cpu__DOT____VlemCall_3__indexed_addr 
                                    = (0x0000ffffU 
                                       & ((0U == (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__90__x))
                                           ? (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__90__addr)
                                           : ((8U > (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__90__x))
                                               ? ((IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__90__addr) 
                                                  + (IData)(vlSelfRef.comet2_cpu__DOT__gr
                                                            [
                                                            (7U 
                                                             & (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__90__x))]))
                                               : (IData)(__Vfunc_comet2_cpu__DOT__indexed_addr__90__addr))));
                                __Vdly__comet2_cpu__DOT__state = 4U;
                                __VdlyVal__comet2_cpu__DOT__gr__v26 
                                    = vlSelfRef.comet2_cpu__DOT____VlemCall_3__indexed_addr;
                                __VdlyDim0__comet2_cpu__DOT__gr__v26 
                                    = (7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r));
                                __VdlySet__comet2_cpu__DOT__gr__v26 = 1U;
                            }
                        } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                            __Vdly__comet2_cpu__DOT__write_word 
                                = vlSelfRef.comet2_cpu__DOT__gr
                                [(7U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__r))];
                            __Vdly__comet2_cpu__DOT__mem_write_kind = 1U;
                            __Vdly__comet2_cpu__DOT__state = 0x0cU;
                        } else {
                            __Vdly__comet2_cpu__DOT__mem_read_kind = 1U;
                            __Vdly__comet2_cpu__DOT__state = 0x0aU;
                        }
                    } else if ((8U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                        vlSelfRef.error_instruction = 1U;
                        __Vdly__comet2_cpu__DOT__state = 4U;
                    } else if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                        vlSelfRef.error_instruction = 1U;
                        __Vdly__comet2_cpu__DOT__state = 4U;
                    } else if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                        vlSelfRef.error_instruction = 1U;
                        __Vdly__comet2_cpu__DOT__state = 4U;
                    } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__main__DOT__op))) {
                        vlSelfRef.error_instruction = 1U;
                        __Vdly__comet2_cpu__DOT__state = 4U;
                    } else {
                        __Vdly__comet2_cpu__DOT__state = 4U;
                    }
                }
            } else {
                __Vfunc_comet2_cpu__DOT__bus_addr__95__word_addr 
                    = vlSelfRef.comet2_cpu__DOT__pr;
                __Vfunc_comet2_cpu__DOT__bus_addr__95__Vfuncout 
                    = __Vfunc_comet2_cpu__DOT__bus_addr__95__word_addr;
                vlSelfRef.read_addr = __Vfunc_comet2_cpu__DOT__bus_addr__95__Vfuncout;
                if (vlSelfRef.read_data_valid) {
                    __Vfunc_comet2_cpu__DOT__bus_word__96__data 
                        = vlSelfRef.read_data;
                    __Vdly__comet2_cpu__DOT__pr = (0x0000ffffU 
                                                   & ((IData)(1U) 
                                                      + (IData)(vlSelfRef.comet2_cpu__DOT__pr)));
                    __Vfunc_comet2_cpu__DOT__bus_word__96__Vfuncout 
                        = (0x0000ffffU & __Vfunc_comet2_cpu__DOT__bus_word__96__data);
                    __Vdly__comet2_cpu__DOT__operand 
                        = __Vfunc_comet2_cpu__DOT__bus_word__96__Vfuncout;
                    __Vdly__comet2_cpu__DOT__state = 9U;
                } else {
                    vlSelfRef.read_req = 1U;
                }
            }
        } else if ((4U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
            if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                    __Vfunc_comet2_cpu__DOT__bus_addr__97__word_addr 
                        = vlSelfRef.comet2_cpu__DOT__pr;
                    __Vfunc_comet2_cpu__DOT__bus_addr__97__Vfuncout 
                        = __Vfunc_comet2_cpu__DOT__bus_addr__97__word_addr;
                    vlSelfRef.read_addr = __Vfunc_comet2_cpu__DOT__bus_addr__97__Vfuncout;
                    vlSelfRef.read_req = 1U;
                    __Vdly__comet2_cpu__DOT__state = 8U;
                } else {
                    vlSelfRef.comet2_cpu__DOT__main__DOT__op 
                        = (0x000000ffU & ((IData)(vlSelfRef.comet2_cpu__DOT__ir) 
                                          >> 8U));
                    __Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op 
                        = vlSelfRef.comet2_cpu__DOT__main__DOT__op;
                    vlSelfRef.comet2_cpu__DOT____VlemCall_0__is_two_word_instruction 
                        = ((0x00000080U & (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))
                            ? ((0x00000040U & (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))
                                ? ((1U & ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                          >> 5U)) && 
                                   ((1U & ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                           >> 4U)) 
                                    && ((1U & (~ ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                  >> 3U))) 
                                        && ((1U & (~ 
                                                   ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                    >> 2U))) 
                                            && ((1U 
                                                 & (~ 
                                                    ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                     >> 1U))) 
                                                && (1U 
                                                    & (~ (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))))))))
                                : ((1U & (~ ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                             >> 5U))) 
                                   && ((1U & (~ ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                 >> 4U))) 
                                       && ((1U & (~ 
                                                  ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                   >> 3U))) 
                                           && ((1U 
                                                & (~ 
                                                   ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                    >> 2U))) 
                                               && ((1U 
                                                    & (~ 
                                                       ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                        >> 1U))) 
                                                   && (1U 
                                                       & (~ (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op)))))))))
                            : ((0x00000040U & (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))
                                ? ((0x00000020U & (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))
                                    ? ((0x00000010U 
                                        & (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))
                                        ? ((1U & (~ 
                                                  ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                   >> 3U))) 
                                           && ((1U 
                                                & (~ 
                                                   ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                    >> 2U))) 
                                               && ((1U 
                                                    & (~ 
                                                       ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                        >> 1U))) 
                                                   && (1U 
                                                       & (~ (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))))))
                                        : ((1U & (~ 
                                                  ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                   >> 3U))) 
                                           && ((4U 
                                                & (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))
                                                ? (
                                                   (1U 
                                                    & (~ 
                                                       ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                        >> 1U))) 
                                                   || (1U 
                                                       & (~ (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))))
                                                : (
                                                   (1U 
                                                    & ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                       >> 1U)) 
                                                   || (1U 
                                                       & (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))))))
                                    : ((0x00000010U 
                                        & (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))
                                        ? ((1U & (~ 
                                                  ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                   >> 3U))) 
                                           && (1U & 
                                               (~ ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                   >> 2U))))
                                        : ((1U & (~ 
                                                  ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                   >> 3U))) 
                                           && ((1U 
                                                & (~ 
                                                   ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                    >> 2U))) 
                                               && (1U 
                                                   & (~ 
                                                      ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                       >> 1U)))))))
                                : ((0x00000020U & (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))
                                    ? ((0x00000010U 
                                        & (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))
                                        ? ((1U & (~ 
                                                  ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                   >> 3U))) 
                                           && ((1U 
                                                & (~ 
                                                   ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                    >> 2U))) 
                                               && ((1U 
                                                    & (~ 
                                                       ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                        >> 1U))) 
                                                   || (1U 
                                                       & (~ (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))))))
                                        : ((1U & (~ 
                                                  ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                   >> 3U))) 
                                           && (1U & 
                                               (~ ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                   >> 2U)))))
                                    : ((1U & ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                              >> 4U)) 
                                       && ((1U & (~ 
                                                  ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                   >> 3U))) 
                                           && ((1U 
                                                & (~ 
                                                   ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                    >> 2U))) 
                                               && ((1U 
                                                    & (~ 
                                                       ((IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op) 
                                                        >> 1U))) 
                                                   || (1U 
                                                       & (~ (IData)(__Vfunc_comet2_cpu__DOT__is_two_word_instruction__98__op))))))))));
                    __Vdly__comet2_cpu__DOT__state 
                        = ((IData)(vlSelfRef.comet2_cpu__DOT____VlemCall_0__is_two_word_instruction)
                            ? 7U : 9U);
                }
            } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                __Vfunc_comet2_cpu__DOT__bus_addr__99__word_addr 
                    = vlSelfRef.comet2_cpu__DOT__pr;
                __Vfunc_comet2_cpu__DOT__bus_addr__99__Vfuncout 
                    = __Vfunc_comet2_cpu__DOT__bus_addr__99__word_addr;
                vlSelfRef.read_addr = __Vfunc_comet2_cpu__DOT__bus_addr__99__Vfuncout;
                if (vlSelfRef.read_data_valid) {
                    __Vfunc_comet2_cpu__DOT__bus_word__100__data 
                        = vlSelfRef.read_data;
                    __Vdly__comet2_cpu__DOT__pr = (0x0000ffffU 
                                                   & ((IData)(1U) 
                                                      + (IData)(vlSelfRef.comet2_cpu__DOT__pr)));
                    __Vfunc_comet2_cpu__DOT__bus_word__100__Vfuncout 
                        = (0x0000ffffU & __Vfunc_comet2_cpu__DOT__bus_word__100__data);
                    __Vdly__comet2_cpu__DOT__ir = __Vfunc_comet2_cpu__DOT__bus_word__100__Vfuncout;
                    __Vdly__comet2_cpu__DOT__state = 6U;
                } else {
                    vlSelfRef.read_req = 1U;
                }
            } else {
                __Vfunc_comet2_cpu__DOT__bus_addr__101__word_addr 
                    = vlSelfRef.comet2_cpu__DOT__pr;
                __Vfunc_comet2_cpu__DOT__bus_addr__101__Vfuncout 
                    = __Vfunc_comet2_cpu__DOT__bus_addr__101__word_addr;
                vlSelfRef.read_addr = __Vfunc_comet2_cpu__DOT__bus_addr__101__Vfuncout;
                vlSelfRef.read_req = 1U;
                __Vdly__comet2_cpu__DOT__state = 5U;
            }
        } else if ((2U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
            if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
                vlSelfRef.read_addr = 0x0000001fU;
                if (vlSelfRef.read_data_valid) {
                    __Vfunc_comet2_cpu__DOT__bus_word__103__data 
                        = vlSelfRef.read_data;
                    __Vfunc_comet2_cpu__DOT__bus_word__103__Vfuncout 
                        = (0x0000ffffU & __Vfunc_comet2_cpu__DOT__bus_word__103__data);
                    __Vdly__comet2_cpu__DOT__sp = __Vfunc_comet2_cpu__DOT__bus_word__103__Vfuncout;
                    __Vdly__comet2_cpu__DOT__state = 4U;
                } else {
                    vlSelfRef.read_req = 1U;
                }
            } else {
                vlSelfRef.read_addr = 0x0000001fU;
                vlSelfRef.read_req = 1U;
                __Vdly__comet2_cpu__DOT__state = 3U;
            }
        } else if ((1U & (IData)(vlSelfRef.comet2_cpu__DOT__state))) {
            vlSelfRef.read_addr = 0U;
            if (vlSelfRef.read_data_valid) {
                __Vfunc_comet2_cpu__DOT__bus_word__106__data 
                    = vlSelfRef.read_data;
                __Vfunc_comet2_cpu__DOT__bus_word__106__Vfuncout 
                    = (0x0000ffffU & __Vfunc_comet2_cpu__DOT__bus_word__106__data);
                __Vdly__comet2_cpu__DOT__pr = __Vfunc_comet2_cpu__DOT__bus_word__106__Vfuncout;
                __Vdly__comet2_cpu__DOT__state = 2U;
            } else {
                vlSelfRef.read_req = 1U;
            }
        } else {
            vlSelfRef.read_addr = 0U;
            vlSelfRef.read_req = 1U;
            __Vdly__comet2_cpu__DOT__state = 1U;
        }
    }
    vlSelfRef.comet2_cpu__DOT__pr = __Vdly__comet2_cpu__DOT__pr;
    vlSelfRef.comet2_cpu__DOT__sp = __Vdly__comet2_cpu__DOT__sp;
    vlSelfRef.comet2_cpu__DOT__of = __Vdly__comet2_cpu__DOT__of;
    vlSelfRef.comet2_cpu__DOT__sf = __Vdly__comet2_cpu__DOT__sf;
    vlSelfRef.comet2_cpu__DOT__zf = __Vdly__comet2_cpu__DOT__zf;
    vlSelfRef.comet2_cpu__DOT__ir = __Vdly__comet2_cpu__DOT__ir;
    vlSelfRef.comet2_cpu__DOT__operand = __Vdly__comet2_cpu__DOT__operand;
    vlSelfRef.comet2_cpu__DOT__effective_addr = __Vdly__comet2_cpu__DOT__effective_addr;
    vlSelfRef.comet2_cpu__DOT__svc_vector_addr = __Vdly__comet2_cpu__DOT__svc_vector_addr;
    vlSelfRef.comet2_cpu__DOT__write_word = __Vdly__comet2_cpu__DOT__write_word;
    vlSelfRef.comet2_cpu__DOT__mem_read_kind = __Vdly__comet2_cpu__DOT__mem_read_kind;
    vlSelfRef.comet2_cpu__DOT__mem_write_kind = __Vdly__comet2_cpu__DOT__mem_write_kind;
    vlSelfRef.comet2_cpu__DOT__dest_reg = __Vdly__comet2_cpu__DOT__dest_reg;
    vlSelfRef.comet2_cpu__DOT__state = __Vdly__comet2_cpu__DOT__state;
    if (__VdlySet__comet2_cpu__DOT__gr__v0) {
        vlSelfRef.comet2_cpu__DOT__gr[0U] = 0U;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v1) {
        vlSelfRef.comet2_cpu__DOT__gr[1U] = 0U;
        vlSelfRef.comet2_cpu__DOT__gr[2U] = 0U;
        vlSelfRef.comet2_cpu__DOT__gr[3U] = 0U;
        vlSelfRef.comet2_cpu__DOT__gr[4U] = 0U;
        vlSelfRef.comet2_cpu__DOT__gr[5U] = 0U;
        vlSelfRef.comet2_cpu__DOT__gr[6U] = 0U;
        vlSelfRef.comet2_cpu__DOT__gr[7U] = 0U;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v8) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v8] 
            = __VdlyVal__comet2_cpu__DOT__gr__v8;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v9) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v9] 
            = __VdlyVal__comet2_cpu__DOT__gr__v9;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v10) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v10] 
            = __VdlyVal__comet2_cpu__DOT__gr__v10;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v11) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v11] 
            = __VdlyVal__comet2_cpu__DOT__gr__v11;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v12) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v12] 
            = __VdlyVal__comet2_cpu__DOT__gr__v12;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v13) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v13] 
            = __VdlyVal__comet2_cpu__DOT__gr__v13;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v14) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v14] 
            = __VdlyVal__comet2_cpu__DOT__gr__v14;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v15) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v15] 
            = __VdlyVal__comet2_cpu__DOT__gr__v15;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v16) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v16] 
            = __VdlyVal__comet2_cpu__DOT__gr__v16;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v17) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v17] 
            = __VdlyVal__comet2_cpu__DOT__gr__v17;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v18) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v18] 
            = __VdlyVal__comet2_cpu__DOT__gr__v18;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v19) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v19] 
            = __VdlyVal__comet2_cpu__DOT__gr__v19;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v20) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v20] 
            = __VdlyVal__comet2_cpu__DOT__gr__v20;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v21) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v21] 
            = __VdlyVal__comet2_cpu__DOT__gr__v21;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v22) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v22] 
            = __VdlyVal__comet2_cpu__DOT__gr__v22;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v23) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v23] 
            = __VdlyVal__comet2_cpu__DOT__gr__v23;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v24) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v24] 
            = __VdlyVal__comet2_cpu__DOT__gr__v24;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v25) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v25] 
            = __VdlyVal__comet2_cpu__DOT__gr__v25;
    }
    if (__VdlySet__comet2_cpu__DOT__gr__v26) {
        vlSelfRef.comet2_cpu__DOT__gr[__VdlyDim0__comet2_cpu__DOT__gr__v26] 
            = __VdlyVal__comet2_cpu__DOT__gr__v26;
    }
}

void Vcomet2_cpu___024root___eval_nba(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___eval_nba\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vcomet2_cpu___024root___nba_sequent__TOP__0(vlSelf);
    }
}

void Vcomet2_cpu___024root___trigger_orInto__act_vec_vec(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___trigger_orInto__act_vec_vec\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((0U >= n));
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcomet2_cpu___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

bool Vcomet2_cpu___024root___eval_phase__act(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___eval_phase__act\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vcomet2_cpu___024root___eval_triggers_vec__act(vlSelf);
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vcomet2_cpu___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
    Vcomet2_cpu___024root___trigger_orInto__act_vec_vec(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    return (0U);
}

void Vcomet2_cpu___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vcomet2_cpu___024root___eval_phase__nba(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___eval_phase__nba\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vcomet2_cpu___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vcomet2_cpu___024root___eval_nba(vlSelf);
        Vcomet2_cpu___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vcomet2_cpu___024root___eval(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___eval\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00002710U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vcomet2_cpu___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("../comet2_cpu.v", 7, "", "DIDNOTCONVERGE: NBA region did not converge after '--converge-limit' of 10000 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00002710U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vcomet2_cpu___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("../comet2_cpu.v", 7, "", "DIDNOTCONVERGE: Active region did not converge after '--converge-limit' of 10000 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactPhaseResult = Vcomet2_cpu___024root___eval_phase__act(vlSelf);
        } while (vlSelfRef.__VactPhaseResult);
        vlSelfRef.__VnbaPhaseResult = Vcomet2_cpu___024root___eval_phase__nba(vlSelf);
    } while (vlSelfRef.__VnbaPhaseResult);
}

#ifdef VL_DEBUG
void Vcomet2_cpu___024root___eval_debug_assertions(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___eval_debug_assertions\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");
    }
    if (VL_UNLIKELY(((vlSelfRef.reset & 0xfeU)))) {
        Verilated::overWidthError("reset");
    }
    if (VL_UNLIKELY(((vlSelfRef.read_data_valid & 0xfeU)))) {
        Verilated::overWidthError("read_data_valid");
    }
}
#endif  // VL_DEBUG
