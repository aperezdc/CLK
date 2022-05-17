//
//  68000Mk2Storage.hpp
//  Clock Signal
//
//  Created by Thomas Harte on 16/05/2022.
//  Copyright © 2022 Thomas Harte. All rights reserved.
//

#ifndef _8000Mk2Storage_h
#define _8000Mk2Storage_h

#include "../../../InstructionSets/M68k/Decoder.hpp"
#include "../../../InstructionSets/M68k/Perform.hpp"
#include "../../../InstructionSets/M68k/Status.hpp"

namespace CPU {
namespace MC68000Mk2 {

struct ProcessorBase: public InstructionSet::M68k::NullFlowController {
	/// States for the state machine which are named by
	/// me for their purpose rather than automatically by file position.
	/// These are negative to avoid ambiguity with the other group.
	enum State: int {
		Reset			= -1,
		Decode	 		= -2,
		WaitForDTACK	= -3,
		FetchOperand	= -4,
		StoreOperand	= -5,

		// Various different effective address calculations.

		CalculateAnDn	= -5,

		// Various forms of perform; each of these will
		// perform the current instruction, then do the
		// indicated bus cycle.

		Perform_np		= -6,
		Perform_np_n	= -7,
	};
	int state_ = State::Reset;

	/// Counts time left on the clock before the current batch of processing
	/// is complete; may be less than zero.
	HalfCycles time_remaining_;

	/// Current supervisor state, for direct provision to the bus handler.
	int is_supervisor_ = 1;

	// A decoder for instructions, plus all collected information about the
	// current instruction.
	InstructionSet::M68k::Predecoder<InstructionSet::M68k::Model::M68000> decoder_;
	InstructionSet::M68k::Preinstruction instruction_;
	uint16_t opcode_;
	uint8_t operand_flags_;
	uint32_t instruction_address_;

	// Register state.
	InstructionSet::M68k::Status status_;
	SlicedInt32 program_counter_;
	SlicedInt32 registers_[16];		// D0–D7 followed by A0–A7.
	SlicedInt32 stack_pointers_[2];

	/// Current state of the DTACK input.
	bool dtack_ = false;
	/// Current state of the VPA input.
	bool vpa_ = false;
	/// Current state of the BERR input.
	bool berr_ = false;

	/// Contains the prefetch queue; the most-recently fetched thing is the
	/// low portion of this word, and the thing fetched before that has
	/// proceeded to the high portion.
	SlicedInt32 prefetch_;

	// Temporary storage for the current instruction's operands
	// and the corresponding effective addresses.
	CPU::SlicedInt32 operand_[2];
	uint32_t effective_address_[2];

	/// If currently in the wait-for-DTACK state, this indicates where to go
	/// upon receipt of DTACK or VPA. BERR will automatically segue
	/// into the proper exception.
	int post_dtack_state_ = 0;

	/// The perform state for this operation.
	int perform_state_ = 0;

	/// When fetching or storing operands, this is the next one to fetch
	/// or store.
	int next_operand_ = 0;

	// Flow controller... all TODO.
	using Preinstruction = InstructionSet::M68k::Preinstruction;

	template <typename IntT> void did_mulu(IntT) {}
	template <typename IntT> void did_muls(IntT) {}
	void did_chk([[maybe_unused]] bool was_under, [[maybe_unused]] bool was_over) {}
	void did_shift([[maybe_unused]] int bit_count) {}
	template <bool did_overflow> void did_divu([[maybe_unused]] uint32_t dividend, [[maybe_unused]] uint32_t divisor) {}
	template <bool did_overflow> void did_divs([[maybe_unused]] int32_t dividend, [[maybe_unused]] int32_t divisor) {}
	void did_bit_op([[maybe_unused]] int bit_position) {}
	inline void did_update_status();
	template <typename IntT> void complete_bcc(bool matched_condition, IntT offset) {}
	void complete_dbcc(bool matched_condition, bool overflowed, int16_t offset) {}
	void bsr(uint32_t offset) {}
	void jsr(uint32_t address) {}
	void jmp(uint32_t address) {}
	void rtr() {}
	void rte() {}
	void rts() {}
	void stop() {}
	void reset() {}
	void link(Preinstruction instruction, uint32_t offset) {}
	void unlink(uint32_t &address) {}
	void pea(uint32_t address) {}
	void move_to_usp(uint32_t address) {}
	void move_from_usp(uint32_t &address) {}
	void tas(Preinstruction instruction, uint32_t address) {}
	template <typename IntT> void movep(Preinstruction instruction, uint32_t source, uint32_t dest) {}
	template <typename IntT> void movem_toM(Preinstruction instruction, uint32_t mask, uint32_t address) {}
	template <typename IntT> void movem_toR(Preinstruction instruction, uint32_t mask, uint32_t address) {}
	template <bool use_current_instruction_pc = true> void raise_exception([[maybe_unused]] int vector) {}
};

}
}

#endif /* _8000Mk2Storage_h */
