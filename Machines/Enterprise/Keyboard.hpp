//
//  Keyboard.hpp
//  Clock Signal
//
//  Created by Thomas Harte on 17/06/2021.
//  Copyright © 2021 Thomas Harte. All rights reserved.
//

#ifndef Machines_Enterprise_Keyboard_hpp
#define Machines_Enterprise_Keyboard_hpp

#include "../KeyboardMachine.hpp"

namespace Enterprise {

#define KeyCode(line, mask)	(line << 8) | mask

enum class Key: uint16_t {
	N		= 0x0000 | 0x01,	BackSlash	= 0x0000 | 0x02,	B		= 0x0000 | 0x04,	C			= 0x0000 | 0x08,
	V		= 0x0000 | 0x10,	X 			= 0x0000 | 0x20,	Z		= 0x0000 | 0x40,	LeftShift	= 0x0000 | 0x80,

	H		= 0x0100 | 0x01,	Lock		= 0x0100 | 0x02,	G		= 0x0100 | 0x04,	D			= 0x0100 | 0x08,
	F		= 0x0100 | 0x10,	S			= 0x0100 | 0x20,	A		= 0x0100 | 0x40,	Control		= 0x0100 | 0x80,

	U		= 0x0200 | 0x01,	Q			= 0x0200 | 0x02,	Y		= 0x0200 | 0x04,	R			= 0x0200 | 0x08,
	T		= 0x0200 | 0x10,	E			= 0x0200 | 0x20,	W		= 0x0200 | 0x40,	Tab			= 0x0200 | 0x80,

	k7		= 0x0300 | 0x01,	k1			= 0x0300 | 0x02,	k6		= 0x0300 | 0x04,	k4			= 0x0300 | 0x08,
	k5		= 0x0300 | 0x10,	k3			= 0x0300 | 0x20,	k2		= 0x0300 | 0x40,	Escape		= 0x0300 | 0x80,

	F4		= 0x0400 | 0x01,	F8			= 0x0400 | 0x02,	F3		= 0x0400 | 0x04,	F6			= 0x0400 | 0x08,
	F5		= 0x0400 | 0x10,	F7			= 0x0400 | 0x20,	F2		= 0x0400 | 0x40,	F1			= 0x0400 | 0x80,

	k8		= 0x0500 | 0x01,									k9		= 0x0500 | 0x04,	Hyphen		= 0x0500 | 0x08,
	k0		= 0x0500 | 0x10,	Tilde		= 0x0500 | 0x20,	Erase	= 0x0500 | 0x40,

	J		= 0x0600 | 0x01,									K		= 0x0600 | 0x04,	SemiColon	= 0x0600 | 0x08,
	L		= 0x0600 | 0x10,	Colon		= 0x0600 | 0x20,	CloseSquareBracket		= 0x0600 | 0x40,

	Stop	= 0x0700 | 0x01,	Down		= 0x0700 | 0x02,	Right	= 0x0700 | 0x04,	Up			= 0x0700 | 0x08,
	Hold	= 0x0700 | 0x10,	Left		= 0x0700 | 0x20,	Enter	= 0x0700 | 0x40,	Option		= 0x0700 | 0x80,

	M		= 0x0800 | 0x01,	Delete		= 0x0800 | 0x02,	Comma	= 0x0800 | 0x04,
	ForwardSlash	= 0x0800 | 0x08,
	FullStop		= 0x0800 | 0x10,
								RightShift	= 0x0800 | 0x20,	Space	= 0x0800 | 0x40,	Insert		= 0x0800 | 0x80,

	I		= 0x0900 | 0x01,									O		= 0x0900 | 0x04,	At			= 0x0900 | 0x08,
	P		= 0x0900 | 0x10,
	OpenSquareBracket		= 0x0900 | 0x20
};

#undef KeyCode

struct KeyboardMapper: public MachineTypes::MappedKeyboardMachine::KeyboardMapper {
	uint16_t mapped_key_for_key(Inputs::Keyboard::Key key) const final;
};

}

#endif /* Keyboard_hpp */
