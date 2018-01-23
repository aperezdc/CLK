//
//  ASCII16kb.hpp
//  Clock Signal
//
//  Created by Thomas Harte on 04/01/2018.
//  Copyright © 2018 Thomas Harte. All rights reserved.
//

#ifndef ASCII16kb_hpp
#define ASCII16kb_hpp

#include "../ROMSlotHandler.hpp"

namespace MSX {
namespace Cartridge {

class ASCII16kbROMSlotHandler: public ROMSlotHandler {
	public:
		ASCII16kbROMSlotHandler(MSX::MemoryMap &map, int slot) :
			map_(map), slot_(slot) {}

		void write(uint16_t address, uint8_t value) {
			switch(address >> 11) {
				default:
					confidence_counter_.add_miss();
				break;
				case 0xc:
					if(address == 0x6000) confidence_counter_.add_hit(); else confidence_counter_.add_equivocal();
					map_.map(slot_, value * 8192, 0x4000, 0x4000);
				break;
				case 0xe:
					if(address == 0x7000 || address == 0x77ff) confidence_counter_.add_hit(); else confidence_counter_.add_equivocal();
					map_.map(slot_, value * 8192, 0x8000, 0x4000);
				break;
			}
		}

	private:
		MSX::MemoryMap &map_;
		int slot_;
};

}
}

#endif /* ASCII16kb_hpp */
