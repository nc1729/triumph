#include "common/constants.h"
#include "common/Tryte.h"
#include "IO/Disk.h"
#include "Computer/Computer.h"

using namespace std::string_literals;

void Computer::BIOS()
{
	// write boot program to memory

	// SET A, DISK_NUM
	memory["000"s] = "maa"s;
	memory["00a"s] = constants::DISK_NUM;

	// CPZ A
	memory["00b"s] = "mCa"s;

	// JPZ no_disk
	memory["00c"s] = "jCj"s;
	memory["00d"s] = "0aB"s;

	// -- disk_found --
	// BANK 1 - change bank to first disk
	memory["00e"s] = "Mj0"s;
	memory["00f"s] = "00a"s;
	// request load of first page
	// ZERO B
	memory["00g"s] = "m0b"s;
	// SAVE B, $DISK_PAGE (page 0 is first page)
	memory["00h"s] = "mDb"s;
	memory["00i"s] = Disk::PAGE;
	// SET B, #00c (set READ_REQUEST_FLAG to 1)
	memory["00j"s] = "mab"s;
	memory["00k"s] = "00c"s;
	// LOAD C, $DISK_STATE
	memory["00l"s] = "mdc"s;
	memory["00m"s] = Disk::STATE;
	// OR C, B
	memory["0aM"s] = "Bcb"s;
	// SAVE C, $DISK_STATE
	memory["0aL"s] = "mDc"s;
	memory["0aK"s] = Disk::STATE;
	// disk read will begin - wait for it to finish
	// LOAD C, $DISK_STATE
	memory["0aJ"s] = "mdc"s;
	memory["0aI"s] = Disk::STATE;
	// STAR C, #00a
	memory["0aH"s] = "mkc"s;
	memory["0aG"s] = "00a"s;
	// JPZ $0aJ (keep loading disk state until its STATUS_FLAG is 1)
	memory["0aF"s] = "jCj"s;
	memory["0aE"s] = "0aJ"s;
	// if we're here, disk has finished loading
	// jump to bootcode on disk at $M00
	memory["0aD"s] = "jMj"s;
	memory["0aC"s] = "M00"s;
	// and execution continues from there

	// -- no disk found --
	// HALT
	memory["0aB"s] = "000"s;
}