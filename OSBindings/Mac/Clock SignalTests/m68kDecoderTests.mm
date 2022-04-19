//
//  m68kDecoderTests.m
//  Clock Signal
//
//  Created by Thomas Harte on 18/04/2022.
//  Copyright 2022 Thomas Harte. All rights reserved.
//

#import <XCTest/XCTest.h>

#include "../../../InstructionSets/68k/Decoder.hpp"

using namespace InstructionSet::M68k;

@interface m68kDecoderTests : XCTestCase
@end

namespace {

template <int index> NSString *operand(Preinstruction instruction) {
	switch(instruction.mode<index>()) {
		default:	return @"";

		case AddressingMode::DataRegisterDirect:
			return [NSString stringWithFormat:@"D%d", instruction.reg<index>()];

		case AddressingMode::AddressRegisterDirect:
			return [NSString stringWithFormat:@"A%d", instruction.reg<index>()];
		case AddressingMode::AddressRegisterIndirect:
			return [NSString stringWithFormat:@"(A%d)", instruction.reg<index>()];
		case AddressingMode::AddressRegisterIndirectWithPostincrement:
			return [NSString stringWithFormat:@"(A%d)+", instruction.reg<index>()];
		case AddressingMode::AddressRegisterIndirectWithPredecrement:
			return [NSString stringWithFormat:@"-(A%d)", instruction.reg<index>()];
	}
}

}

@implementation m68kDecoderTests

- (NSString *)operand:(int)operand instruction:(Preinstruction)instruction {
	return @"";
}

- (void)testInstructionSpecs {
	NSData *const testData =
		[NSData dataWithContentsOfURL:
			[[NSBundle bundleForClass:[self class]]
				URLForResource:@"68000ops"
				withExtension:@"json"
				subdirectory:@"68000 Decoding"]];

	NSDictionary<NSString *, NSString *> *const decodings = [NSJSONSerialization JSONObjectWithData:testData options:0 error:nil];
	XCTAssertNotNil(decodings);

	Predecoder<Model::M68000> decoder;
	for(int instr = 0; instr < 65536; instr++) {
		NSString *const instrName = [NSString stringWithFormat:@"%04x", instr];
		NSString *const expected = decodings[instrName];
		XCTAssertNotNil(expected);

		const auto found = decoder.decode(uint16_t(instr));

		// Hatch off no-instruction as a special case,
		// at least temporarily.
		if(found.operation == Operation::Undefined) {
			XCTAssertEqualObjects(@"None", expected, "%@ should decode as %@", instrName, expected);
			continue;
		}

		NSString *instruction;
		switch(found.operation) {
			case Operation::ABCD:	instruction = @"ABCD";	break;

			// For now, skip any unmapped operations.
			default: continue;
		}

		NSString *const operand1 = operand<0>(found);
		NSString *const operand2 = operand<1>(found);

		if(operand1.length) instruction = [instruction stringByAppendingFormat:@" %@", operand1];
		if(operand2.length) instruction = [instruction stringByAppendingFormat:@", %@", operand2];

		XCTAssertEqualObjects(instruction, expected, "%@ should decode as %@; got %@", instrName, expected, instruction);
	}

}

@end
