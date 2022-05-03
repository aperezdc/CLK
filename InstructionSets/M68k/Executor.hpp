//
//  Executor.hpp
//  Clock Signal
//
//  Created by Thomas Harte on 29/04/2022.
//  Copyright © 2022 Thomas Harte. All rights reserved.
//

#ifndef InstructionSets_M68k_Executor_hpp
#define InstructionSets_M68k_Executor_hpp

#include "Decoder.hpp"
#include "Instruction.hpp"
#include "Model.hpp"
#include "Perform.hpp"
#include "Status.hpp"

namespace InstructionSet {
namespace M68k {

struct BusHandler {
	template <typename IntT> void write(uint32_t address, IntT value);
	template <typename IntT> IntT read(uint32_t address);
};

/// Ties together the decoder, sequencer and performer to provide an executor for 680x0 instruction streams.
/// As is standard for these executors, no bus- or cache-level fidelity to any real 680x0 is attempted. This is
/// simply an executor of 680x0 code.
template <Model model, typename BusHandler> class Executor {
	public:
		Executor(BusHandler &);

		/// Executes the number of instructions specified;
		/// other events — such as initial reset or branching
		/// to exceptions — may be zero costed, and interrupts
		/// will not necessarily take effect immediately when signalled.
		void run_for_instructions(int);


		// Flow control.
		void consume_cycles(int) {}
		void raise_exception(int);
		void stop();
		void set_pc(uint32_t);
		void add_pc(uint32_t);
		void decline_branch() {}
		void did_update_status();
		void bsr(uint32_t offset);
		void jsr(uint32_t offset);

		// TODO: ownership of this shouldn't be here.
		struct Registers {
			uint32_t data[8], address[7];
			uint32_t user_stack_pointer;
			uint32_t supervisor_stack_pointer;
			uint16_t status;
			uint32_t program_counter;
		};
		Registers get_state();
		void set_state(const Registers &);

	private:
		BusHandler &bus_handler_;
		Predecoder<model> decoder_;

		void reset();
		struct EffectiveAddress {
			CPU::SlicedInt32 value;
			bool requires_fetch;
		};
		EffectiveAddress calculate_effective_address(Preinstruction instruction, uint16_t opcode, int index);

		void read(DataSize size, uint32_t address, CPU::SlicedInt32 &value);
		void write(DataSize size, uint32_t address, CPU::SlicedInt32 value);
		template <typename IntT> IntT read_pc();
		uint32_t index_8bitdisplacement();

		// Processor state.
		Status status_;
		CPU::SlicedInt32 program_counter_;
		CPU::SlicedInt32 data_[8], address_[8];
		CPU::SlicedInt32 stack_pointers_[2];
		uint32_t instruction_address_;
		int active_stack_pointer_ = 0;

		// A lookup table to ensure that A7 is adjusted by 2 rather than 1 in
		// postincrement and predecrement mode.
		static constexpr uint32_t byte_increments[] = {
			1, 1, 1, 1, 1, 1, 1, 2
		};
};

}
}

#include "Implementation/ExecutorImplementation.hpp"

#endif /* InstructionSets_M68k_Executor_hpp */
