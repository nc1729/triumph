#include "common/constants.h"
#include "common/Tryte.h"
#include "IO/Disk.h"
#include "Computer/BIOS.h"

using namespace std::string_literals;

BIOS::BIOS()
{
	// write boot program to BIOS chip

	// SET A, DISK_NUM
	chip["000"s] = "maa"s;
	chip["00a"s] = constants::DISK_NUM;

	// CPZ A
	chip["00b"s] = "mCa"s;

	// JPZ no_disk
	chip["00c"s] = "jCj"s;
	chip["00d"s] = "0aB"s;

	// -- disk_found --
	// BANK 1 - change bank to first disk
	chip["00e"s] = "Mj0"s;
	chip["00f"s] = "00a"s;
	// request load of first page
	// ZERO B
	chip["00g"s] = "m0b"s;
	// SAVE B, $DISK_PAGE (page 0 is first page)
	chip["00h"s] = "mDb"s;
	chip["00i"s] = Disk::PAGE;
	// SET B, #00c (set READ_REQUEST_FLAG to 1)
	chip["00j"s] = "mab"s;
	chip["00k"s] = "00c"s;
	// LOAD C, $DISK_STATE
	chip["00l"s] = "mdc"s;
	chip["00m"s] = Disk::STATE;
	// OR C, B
	chip["0aM"s] = "Bcb"s;
	// SAVE C, $DISK_STATE
	chip["0aL"s] = "mDc"s;
	chip["0aK"s] = Disk::STATE;
	// disk read will begin - wait for it to finish
	// LOAD C, $DISK_STATE
	chip["0aJ"s] = "mdc"s;
	chip["0aI"s] = Disk::STATE;
	// STAR C, #00a
	chip["0aH"s] = "mkc"s;
	chip["0aG"s] = "00a"s;
	// JPZ $0aJ (keep loading disk state until its STATUS_FLAG is 1)
	chip["0aF"s] = "jCj"s;
	chip["0aE"s] = "0aJ"s;
	// if we're here, disk has finished loading
	// jump to bootcode on disk at $M00
	chip["0aD"s] = "jMj"s;
	chip["0aC"s] = "M00"s;
	// and execution continues from there

	// -- no disk found --
	// HALT
	chip["0aB"s] = "000"s;



}