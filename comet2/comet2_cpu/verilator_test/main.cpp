#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Vcomet2_cpu.h"
#include "verilated.h"

static const uint32_t MEM_WORDS = 65536;

struct Memory {
  uint16_t word[MEM_WORDS];
  bool pending_read;
  uint32_t pending_addr;

  Memory() : pending_read(false), pending_addr(0) {
    memset(word, 0, sizeof(word));
  }
};

static uint16_t insn(uint8_t op, uint8_t r, uint8_t x) {
  return (uint16_t(op) << 8) | ((r & 0x0f) << 4) | (x & 0x0f);
}

static void load_word(Memory &mem, uint16_t addr, uint16_t value) {
  mem.word[addr] = value;
}

static void load_2word(Memory &mem, uint16_t &pc, uint16_t op, uint16_t operand) {
  load_word(mem, pc++, op);
  load_word(mem, pc++, operand);
}

static void tick(Vcomet2_cpu *tb, Memory &mem, bool reset) {
  tb->clk = 0;
  tb->reset = reset;

  if (mem.pending_read) {
    uint32_t word_addr = mem.pending_addr;
    if (word_addr >= MEM_WORDS) {
      printf("bad read address: 0x%08x\n", word_addr);
      exit(1);
    }
    tb->read_data = mem.word[word_addr];
    tb->read_data_valid = 1;
  } else {
    tb->read_data = 0;
    tb->read_data_valid = 0;
  }

  tb->eval();
  tb->clk = 1;
  tb->eval();

  if (tb->write_req) {
    uint32_t word_addr = tb->write_addr;
    if (word_addr >= MEM_WORDS) {
      printf("bad write address: 0x%08x\n", word_addr);
      exit(1);
    }
    if (tb->memory_mask != 0x3) {
      printf("unexpected memory_mask on write: 0x%x\n", tb->memory_mask);
      exit(1);
    }
    mem.word[word_addr] = uint16_t(tb->write_data & 0xffff);
  }

  mem.pending_read = tb->read_req;
  mem.pending_addr = tb->read_addr;
}

static void run_cycles(Vcomet2_cpu *tb, Memory &mem, int cycles) {
  for (int i = 0; i < cycles; i++) {
    tick(tb, mem, false);
    if (tb->error_instruction) {
      printf("CPU raised error_instruction at cycle %d, debug=0x%08x\n", i, tb->debug);
      exit(1);
    }
  }
}

static void reset_cpu(Vcomet2_cpu *tb, Memory &mem) {
  mem.pending_read = false;
  mem.pending_addr = 0;
  tick(tb, mem, true);
  tick(tb, mem, true);
  tick(tb, mem, false);
}

static void expect_word(const Memory &mem, uint16_t addr, uint16_t expected, const char *name) {
  uint16_t actual = mem.word[addr];
  if (actual != expected) {
    printf("%s failed: mem[0x%04x] = 0x%04x, expected 0x%04x\n",
           name, addr, actual, expected);
    exit(1);
  }
}

static void test_lad_st_and_svc_reset(Vcomet2_cpu *tb) {
  Memory mem;
  load_word(mem, 0, 0x0100);
  load_word(mem, 31, 0x0200);

  uint16_t pc = 0x0100;
  load_2word(mem, pc, insn(0x12, 1, 0), 0x1234); // LAD GR1,#1234
  load_2word(mem, pc, insn(0x11, 1, 0), 0x0300); // ST GR1,RESULT
  load_2word(mem, pc, insn(0xf0, 0, 0), 0x0000); // SVC 0, jumps through vector 0

  reset_cpu(tb, mem);
  run_cycles(tb, mem, 80);

  expect_word(mem, 0x0300, 0x1234, "LAD/ST/SVC0");
  uint16_t pr = tb->debug & 0xffff;
  if (pr < 0x0100 || pr > 0x0106) {
    printf("SVC0 did not return near reset vector, debug=0x%08x\n", tb->debug);
    exit(1);
  }
}

static void test_arithmetic_compare_and_branch(Vcomet2_cpu *tb) {
  Memory mem;
  load_word(mem, 0, 0x0100);
  load_word(mem, 31, 0x0200);
  load_word(mem, 0x0310, 12);

  uint16_t pc = 0x0100;
  load_2word(mem, pc, insn(0x12, 1, 0), 5);      // LAD GR1,5
  load_2word(mem, pc, insn(0x12, 2, 0), 7);      // LAD GR2,7
  load_word(mem, pc++, insn(0x24, 1, 2));        // ADDA GR1,GR2
  load_2word(mem, pc, insn(0x40, 1, 0), 0x0310); // CPA GR1,=12
  load_2word(mem, pc, insn(0x63, 0, 0), 0x0110); // JZE PASS
  load_2word(mem, pc, insn(0x12, 3, 0), 0xbad0); // fail path
  load_2word(mem, pc, insn(0x11, 3, 0), 0x0301);
  load_2word(mem, pc, insn(0xf0, 0, 0), 0x0000);

  pc = 0x0110;
  load_2word(mem, pc, insn(0x12, 3, 0), 0xbeef); // PASS: LAD GR3,#BEEF
  load_2word(mem, pc, insn(0x11, 3, 0), 0x0301); // ST GR3,RESULT
  load_2word(mem, pc, insn(0xf0, 0, 0), 0x0000);

  reset_cpu(tb, mem);
  run_cycles(tb, mem, 140);

  expect_word(mem, 0x0301, 0xbeef, "ADDA/CPA/JZE");
}

static void test_call_ret_push_pop(Vcomet2_cpu *tb) {
  Memory mem;
  load_word(mem, 0, 0x0100);
  load_word(mem, 31, 0x0200);

  uint16_t pc = 0x0100;
  load_2word(mem, pc, insn(0x12, 1, 0), 0x4444); // LAD GR1,#4444
  load_2word(mem, pc, insn(0x70, 0, 0), 0x7777); // PUSH #7777
  load_word(mem, pc++, insn(0x71, 2, 0));        // POP GR2
  load_2word(mem, pc, insn(0x80, 0, 0), 0x0120); // CALL SUB
  load_2word(mem, pc, insn(0x11, 1, 0), 0x0302); // ST GR1,RESULT1
  load_2word(mem, pc, insn(0x11, 2, 0), 0x0303); // ST GR2,RESULT2
  load_2word(mem, pc, insn(0xf0, 0, 0), 0x0000);

  pc = 0x0120;
  load_2word(mem, pc, insn(0x12, 1, 0), 0x5555); // SUB: LAD GR1,#5555
  load_word(mem, pc++, insn(0x81, 0, 0));        // RET

  reset_cpu(tb, mem);
  run_cycles(tb, mem, 180);

  expect_word(mem, 0x0302, 0x5555, "CALL/RET result");
  expect_word(mem, 0x0303, 0x7777, "PUSH/POP result");
}

static void test_svc_ret(Vcomet2_cpu *tb) {
  Memory mem;
  load_word(mem, 0, 0x0100);
  load_word(mem, 1, 0x0120);
  load_word(mem, 31, 0x0200);

  uint16_t pc = 0x0100;
  load_2word(mem, pc, insn(0xF0, 0, 0), 0x0001); // SVC 1
  load_2word(mem, pc, insn(0x11, 1, 0), 0x0304); // ST GR1,RESULT
  load_2word(mem, pc, insn(0xF0, 0, 0), 0x0000); // SVC 0

  pc = 0x0120;
  load_2word(mem, pc, insn(0x12, 1, 0), 0xCAFE); // handler: LAD GR1,#CAFE
  load_word(mem, pc++, insn(0x81, 0, 0));        // RET

  reset_cpu(tb, mem);
  run_cycles(tb, mem, 120);

  expect_word(mem, 0x0304, 0xCAFE, "SVC/RET result");
}

int main(int argc, char **argv) {
  Verilated::commandArgs(argc, argv);

  Vcomet2_cpu *tb = new Vcomet2_cpu;

  test_lad_st_and_svc_reset(tb);
  test_arithmetic_compare_and_branch(tb);
  test_call_ret_push_pop(tb);
  test_svc_ret(tb);

  printf("COMET II CPU tests passed\n");
  delete tb;
  return 0;
}