// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcomet2_cpu.h for the primary calling header

#include "Vcomet2_cpu__pch.h"

VL_ATTR_COLD void Vcomet2_cpu___024root___eval_static(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___eval_static\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->comet2_cpu__DOT__main__DOT__op = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 1435109353213419412ull);
    vlSelf->comet2_cpu__DOT__main__DOT__r = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 16283074615881838143ull);
    vlSelf->comet2_cpu__DOT__main__DOT__x = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15380278015949391399ull);
    vlSelf->comet2_cpu__DOT__main__DOT__rhs = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 5504499642294925935ull);
    vlSelf->comet2_cpu__DOT__main__DOT__result = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 15126122825638444800ull);
    vlSelf->comet2_cpu__DOT__main__DOT__wide = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 9344346851100930168ull);
    vlSelf->comet2_cpu__DOT__main__DOT__shifted_out = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7646842868642645548ull);
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
}

VL_ATTR_COLD void Vcomet2_cpu___024root___eval_static__TOP(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___eval_static__TOP\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->comet2_cpu__DOT__main__DOT__op = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 1435109353213419412ull);
    vlSelf->comet2_cpu__DOT__main__DOT__r = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 16283074615881838143ull);
    vlSelf->comet2_cpu__DOT__main__DOT__x = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 15380278015949391399ull);
    vlSelf->comet2_cpu__DOT__main__DOT__rhs = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 5504499642294925935ull);
    vlSelf->comet2_cpu__DOT__main__DOT__result = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 15126122825638444800ull);
    vlSelf->comet2_cpu__DOT__main__DOT__wide = VL_SCOPED_RAND_RESET_I(17, __VscopeHash, 9344346851100930168ull);
    vlSelf->comet2_cpu__DOT__main__DOT__shifted_out = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7646842868642645548ull);
}

VL_ATTR_COLD void Vcomet2_cpu___024root___eval_initial(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___eval_initial\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.read_req = 0U;
    vlSelfRef.read_addr = 0U;
    vlSelfRef.write_req = 0U;
    vlSelfRef.write_addr = 0U;
    vlSelfRef.write_data = 0U;
    vlSelfRef.memory_mask = 3U;
    vlSelfRef.error_instruction = 0U;
    vlSelfRef.debug = 0U;
    vlSelfRef.comet2_cpu__DOT__pr = 0U;
    vlSelfRef.comet2_cpu__DOT__sp = 0U;
    vlSelfRef.comet2_cpu__DOT__of = 0U;
    vlSelfRef.comet2_cpu__DOT__sf = 0U;
    vlSelfRef.comet2_cpu__DOT__zf = 0U;
    vlSelfRef.comet2_cpu__DOT__ir = 0U;
    vlSelfRef.comet2_cpu__DOT__operand = 0U;
    vlSelfRef.comet2_cpu__DOT__effective_addr = 0U;
    vlSelfRef.comet2_cpu__DOT__svc_vector_addr = 0U;
    vlSelfRef.comet2_cpu__DOT__write_word = 0U;
    vlSelfRef.comet2_cpu__DOT__mem_read_kind = 0U;
    vlSelfRef.comet2_cpu__DOT__mem_write_kind = 0U;
    vlSelfRef.comet2_cpu__DOT__dest_reg = 0U;
    vlSelfRef.comet2_cpu__DOT__state = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[0U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[1U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[2U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[3U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[4U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[5U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[6U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[7U] = 0U;
}

VL_ATTR_COLD void Vcomet2_cpu___024root___eval_initial__TOP(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___eval_initial__TOP\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.read_req = 0U;
    vlSelfRef.read_addr = 0U;
    vlSelfRef.write_req = 0U;
    vlSelfRef.write_addr = 0U;
    vlSelfRef.write_data = 0U;
    vlSelfRef.memory_mask = 3U;
    vlSelfRef.error_instruction = 0U;
    vlSelfRef.debug = 0U;
    vlSelfRef.comet2_cpu__DOT__pr = 0U;
    vlSelfRef.comet2_cpu__DOT__sp = 0U;
    vlSelfRef.comet2_cpu__DOT__of = 0U;
    vlSelfRef.comet2_cpu__DOT__sf = 0U;
    vlSelfRef.comet2_cpu__DOT__zf = 0U;
    vlSelfRef.comet2_cpu__DOT__ir = 0U;
    vlSelfRef.comet2_cpu__DOT__operand = 0U;
    vlSelfRef.comet2_cpu__DOT__effective_addr = 0U;
    vlSelfRef.comet2_cpu__DOT__svc_vector_addr = 0U;
    vlSelfRef.comet2_cpu__DOT__write_word = 0U;
    vlSelfRef.comet2_cpu__DOT__mem_read_kind = 0U;
    vlSelfRef.comet2_cpu__DOT__mem_write_kind = 0U;
    vlSelfRef.comet2_cpu__DOT__dest_reg = 0U;
    vlSelfRef.comet2_cpu__DOT__state = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[0U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[1U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[2U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[3U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[4U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[5U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[6U] = 0U;
    vlSelfRef.comet2_cpu__DOT__gr[7U] = 0U;
}

VL_ATTR_COLD void Vcomet2_cpu___024root___eval_final(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___eval_final\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vcomet2_cpu___024root___eval_settle(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___eval_settle\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

bool Vcomet2_cpu___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vcomet2_cpu___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vcomet2_cpu___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vcomet2_cpu___024root___ctor_var_reset(Vcomet2_cpu___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vcomet2_cpu___024root___ctor_var_reset\n"); );
    Vcomet2_cpu__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->reset = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9928399931838511862ull);
    vlSelf->read_req = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16973560346851689136ull);
    vlSelf->read_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10685798328807655045ull);
    vlSelf->read_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 13838377541112373472ull);
    vlSelf->read_data_valid = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10553029003516498094ull);
    vlSelf->write_req = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6938175717101323407ull);
    vlSelf->write_addr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 7370095266023931979ull);
    vlSelf->write_data = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11298066925140600626ull);
    vlSelf->memory_mask = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 7617782226862115874ull);
    vlSelf->error_instruction = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6942553916053254188ull);
    vlSelf->debug = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 8781005679405889265ull);
    for (int __Vi0 = 0; __Vi0 < 8; ++__Vi0) {
        vlSelf->comet2_cpu__DOT__gr[__Vi0] = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 7140272558065427045ull);
    }
    vlSelf->comet2_cpu__DOT__pr = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 1831044890112211481ull);
    vlSelf->comet2_cpu__DOT__sp = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 9490314481954343604ull);
    vlSelf->comet2_cpu__DOT__of = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9087860668397233409ull);
    vlSelf->comet2_cpu__DOT__sf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14698431262281185278ull);
    vlSelf->comet2_cpu__DOT__zf = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9705816642302917871ull);
    vlSelf->comet2_cpu__DOT__svc_vector_addr = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 18424280390544260888ull);
    vlSelf->comet2_cpu__DOT__state = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 564617017360114820ull);
    vlSelf->comet2_cpu__DOT__ir = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 18417466540016802973ull);
    vlSelf->comet2_cpu__DOT__operand = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 7880263445671515705ull);
    vlSelf->comet2_cpu__DOT__effective_addr = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 8401621142701330722ull);
    vlSelf->comet2_cpu__DOT__write_word = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 6955544580086558607ull);
    vlSelf->comet2_cpu__DOT__mem_read_kind = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17449697445473270401ull);
    vlSelf->comet2_cpu__DOT__mem_write_kind = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 10864397807513634884ull);
    vlSelf->comet2_cpu__DOT__dest_reg = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 7655065178515451967ull);
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
